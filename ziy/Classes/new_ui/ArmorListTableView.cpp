#include "ArmorListTableView.h"
#include "Defines.h"
#include "ActorInfoLayer.h"
#include "ActorSourceLayer.h"
#include "PopLayer.h"
#include "ActorJoinOwnLayer.h"
#include "GoldExchangeLayer.h"
#include "NoticeActOpenLayer.h"
#include "NetPromptTool.h"
#include "Net_HomePage.h"

ArmorListTableView* ArmorListTableView::create(EventDelegate* ptrDelegate)
{
	ArmorListTableView* pRet = new ArmorListTableView();
	if (pRet&&pRet->init(ptrDelegate))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ArmorListTableView::init(EventDelegate* ptrDelegate)
{
    if ( !Layer::init() )
    {
        return false;
    }
	mDelegate = ptrDelegate;
	show_child_layer = false;
	click_index = -1;
	InitElemList();

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(530,270));
//GHB	tableView->setTouchPriority(LAYER_SECOND-1);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(40,37));
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
	{
		tableView->setTouchEnabled(false);
	}
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
    
    return true;
}

ArmorListTableView::ArmorListTableView()
{

}

ArmorListTableView::~ArmorListTableView()
{
	for (size_t i = 0; i < armor_list.size(); i++)
	{
		delete armor_list[i];
	}
	HongDian::getInstence()->HongDianNodeisVisibol[1001] = 0;
}
bool ArmorListTableView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    return true;
}

void ArmorListTableView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void ArmorListTableView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event) {

}

void ArmorListTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ArmorListTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ArmorListTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ArmorListTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ArmorListTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ArmorListTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(530,106);
}

TableViewCell* ArmorListTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ArmorListCell::create(this,idx);
    }
    else
    {
		ArmorListCell* c = (ArmorListCell*)cell;
		c->mALTB = this;
		c->UpDateCell(idx);
    }
    return cell;
}


ssize_t ArmorListTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ArmorListDate* ArmorListTableView::GetCellElem(int iIndex)
{
	return armor_list[iIndex];
}

void ArmorListTableView::AddActorInfoLayer(int iIndex)
{
	click_index = iIndex;
	
	show_child_layer = true;
	if (armor_list[iIndex]->addOwnArmList)
	{
		ChessApplication::getInstance()->AddFunctionLayer(ActorInfoLayer::create(&history->actors[armor_list[iIndex]->actorId-1],READ_STATE_INFO,this));
	}
	else
	{
		if (armor_list[iIndex]->current_soul >= armor_list[iIndex]->need_soul)
		{
			PopLayer* pop = PopLayer::create(POP_ZM_WJ, this);
			Director::getInstance()->getRunningScene()->addChild(pop, 98);
		}
		else
			ChessApplication::getInstance()->AddFunctionLayer(ActorSourceLayer::create(armor_list[iIndex]->actorId, armor_list[iIndex]->current_soul, armor_list[iIndex]->need_soul, this));
	}
}

void ArmorListTableView::SetPopLayerElement(Sprite* ptrBg)
{
	int iQuality = trData->actors[armor_list[click_index]->actorId-1].actor_quality;
	__String* GoldNumCs = __String::createWithFormat(LL("zmyxxyhfdjb"),trData->net_hero_quality[iQuality]->gold);
	Label* title = LabelSystemFont(GoldNumCs->getCString(), 18);
	title->setPosition(Vec2(ptrBg->getContentSize().width / 2, 132));
	ptrBg->addChild(title);
}

void ArmorListTableView::RunFunction(int iNomal)
{
	int iQuality = trData->actors[armor_list[click_index]->actorId - 1].actor_quality;
	if (history->money >= trData->net_hero_quality[iQuality]->gold)
	{
		SendNetDate();
	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
		show_child_layer = false;
	}
}

void ArmorListTableView::updateShow(int iNomal)
{
	updateMoneyShow();
}

void ArmorListTableView::updateMoneyShow()
{
	if (mDelegate)
		mDelegate->updateMoneyShow();
}

void ArmorListTableView::RemoveAllParentLayer()
{
	if (mDelegate)
		mDelegate->RemoveAllParentLayer();
}

void ArmorListTableView::updateParentShow()
{
	InitElemList();
	Vec2 offset_Point = tableView->getContentOffset();
	tableView->reloadData();
	tableView->setContentOffset(offset_Point);
}

void ArmorListTableView::ChildCloseLayer(int iNomal)
{
	show_child_layer = false;
}

bool ArmorListTableView::EnableShowChild()
{
	return !show_child_layer;
}

void ArmorListTableView::InitElemList()
{
	for (map<int, ArmorListDate*>::iterator it = armor_list.begin(); it != armor_list.end(); it++)
	{
		delete it->second;
	}
	armor_list.clear();
	int armor_list_num = 0;
	map<int, int> addOwnArmy;
	for (int i = 0; i < history->ownArmyNum; i++)
	{
		ArmorListDate* ptrALD = new ArmorListDate();
		ptrALD->actorId = history->ownArmy[i] + 1;
		ptrALD->addOwnArmList = true;
		ptrALD->current_soul = 0;
		ptrALD->need_soul = 0;
		armor_list[armor_list_num++] = ptrALD;
		addOwnArmy[ptrALD->actorId] = 1;
	}

	SortElemList();

	for (map<int, NetHeroListPtr>::iterator it = trData->net_hero_list.begin(); it != trData->net_hero_list.end(); it++)
	{
		int iActorId = it->first;
		int iCurActorSoul = history->mChipSoulDate->GetActorChipNumByActorId(iActorId);
		int iNeedActorSoul = trData->net_hero_quality[trData->actors[iActorId - 1].actor_quality]->call_soul;
		if (addOwnArmy.find(iActorId) != addOwnArmy.end() || iCurActorSoul < iNeedActorSoul)
			continue;
		ArmorListDate* ptrALD = new ArmorListDate();
		ptrALD->actorId = iActorId;
		ptrALD->addOwnArmList = false;
		ptrALD->enable_recruit = true;
		ptrALD->current_soul = iCurActorSoul;
		ptrALD->need_soul = iNeedActorSoul;
		armor_list[armor_list_num++] = ptrALD;
	}

	for (map<int, NetHeroListPtr>::iterator it = trData->net_hero_list.begin(); it != trData->net_hero_list.end(); it++)
	{
		int iActorId = it->first;
		int iCurActorSoul = history->mChipSoulDate->GetActorChipNumByActorId(iActorId);
		int iNeedActorSoul = trData->net_hero_quality[trData->actors[iActorId - 1].actor_quality]->call_soul;
		if (addOwnArmy.find(iActorId) != addOwnArmy.end() || iCurActorSoul >= iNeedActorSoul || !iCurActorSoul)
			continue;
		ArmorListDate* ptrALD = new ArmorListDate();
		ptrALD->actorId = iActorId;
		ptrALD->addOwnArmList = false;
		ptrALD->current_soul = iCurActorSoul;
		ptrALD->need_soul = iNeedActorSoul;
		armor_list[armor_list_num++] = ptrALD;
	}
	int null_elem = armor_list_num % 5;
	if (null_elem)
	{
		for (int i = 0; i < 5 - null_elem; i++)
		{
			armor_list[armor_list_num++] = new ArmorListDate();
		}
	}
	cell_num = armor_list_num / 5;
}

void ArmorListTableView::SortElemList()
{
	int iActorId1 = -1, iActorId2 = -1;
	ArmorListDate* ptrArmorListDate = NULL;

	for (size_t i = 1; i < armor_list.size(); i++)
	{
		for (size_t j = armor_list.size() - 1; j > i; j--)
		{
			iActorId1 = armor_list[j]->actorId - 1;
			iActorId2 = armor_list[j-1]->actorId - 1;
			if (history->actors[iActorId1].actor_quality > history->actors[iActorId2].actor_quality ||
				(history->actors[iActorId1].actor_quality == history->actors[iActorId2].actor_quality&&history->actors[iActorId1].GetActorLv() > history->actors[iActorId2].GetActorLv()))
			{
				ptrArmorListDate = armor_list[j];
				armor_list[j] = armor_list[j-1];
				armor_list[j-1] = ptrArmorListDate;
			}
		}
	}
}

void ArmorListTableView::SendNetDate()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	int iNetDateIndex = history->mChipSoulDate->GetSoulElemByActorId(armor_list[click_index]->actorId)->id;
	__String* validateCs = __String::createWithFormat("id=%d&gameToken=%s", iNetDateIndex,GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iNetDateIndex, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("addOrUpHero",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ArmorListTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ArmorListTableView::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		UpdateArmorDate(value);
	}
}

void ArmorListTableView::UpdateArmorDate(Json::Value JsonValue)
{
	show_child_layer = false;

	if (JsonValue["result"].isObject())
	{
		Json::Value NetDate = JsonValue["result"];
		if (NetDate["gold"].isInt())
			history->money = NetDate["gold"].asInt();
		int iMinus = 0;
		if (NetDate["minus"].isInt())
			iMinus = NetDate["minus"].asInt();
		int iActorId = armor_list[click_index]->actorId;
		history->mChipSoulDate->ReduceActorSoulNumByActorId(iActorId, iMinus);

		if (NetDate["hero"].isObject())
		{
			Json::Value HeroDate = NetDate["hero"];
			ActorJoinOwnDate NetActorDate;
			if (HeroDate["heroID"].isInt())
				NetActorDate.setActorId(HeroDate["heroID"].asInt());
			if (HeroDate["EXP"].isInt())
				NetActorDate.setExp(HeroDate["EXP"].asInt());
			if (HeroDate["lv"].isInt())
				NetActorDate.setLv(HeroDate["lv"].asInt());
			if (HeroDate["jobID"].isInt())
				NetActorDate.setJobID(HeroDate["jobID"].asInt());
			if (HeroDate["fashion1"].isInt())
				NetActorDate.setFashion1(HeroDate["fashion1"].asInt());
			if (HeroDate["fashion2"].isInt())
				NetActorDate.setFashion2(HeroDate["fashion2"].asInt());
			if (HeroDate["quality"].isInt())
				NetActorDate.setQuality(HeroDate["quality"].asInt());
			if (HeroDate["gender"].isInt())
				NetActorDate.setGender(HeroDate["gender"].asInt());

			NetActorDate.setSoulBox(HeroDate["isSoulBox"].asInt());

			if (NetDate["goods"].isArray())
			{
				NetActorDate.setGoodsValue(NetDate["goods"]);
			}

			history->NewActorJoinOwn(&NetActorDate);
			//history->popOpenNotice(1);
		}
		if(history->m_iOpenId)
		{
			ChessApplication::getInstance()->AddFunctionLayer(NoticeActOpenLayer::create(history->m_iOpenId));
			history->m_iOpenId = 0;
		}
		ChessApplication::getInstance()->AddFunctionLayer(ActorJoinOwnLayer::create(iActorId));
		InitElemList();
		tableView->reloadData();
		mDelegate->RunFunction(0);
		NetPromptTool::getInstance()->AddNetPrompt(PROMPT_WJZM, iActorId);
	}
}

ArmorListCell* ArmorListCell::create(ArmorListTableView* ptrALTB, int idx)
{
	ArmorListCell* pRet = new ArmorListCell();
	pRet->mALTB = ptrALTB;
	if (pRet&&pRet->init(idx))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ArmorListCell::init(int idx)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int iWidth = 106;
	int iHeight = 106;

	for (int i = 0; i < 5; i++)
	{
		Sprite* elem_bg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
		elem_bg->setPosition(Vec2(iWidth / 2 + i * iWidth, iHeight / 2));
		this->addChild(elem_bg);

		int iBgWidth = elem_bg->getContentSize().width;
		int iBgHeight = elem_bg->getContentSize().height;

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_fashion,1);
		elem_fashion->setVisible(false);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_img,1);

		Label* elem_name = LabelSystemFont("", 16);
		elem_name->setPosition(Vec2(iBgWidth/2, 21));
		elem_bg->addChild(elem_name,3);
		elem_name->enableOutline(Color4B::BLACK, 1);

		Label* elem_lv = LabelSystemFont("", 16);
		elem_lv->setAnchorPoint(Vec2(1, 1));
		elem_lv->setPosition(Vec2(83, 81));
		elem_bg->addChild(elem_lv,3);
		elem_lv->setColor(Color3B::YELLOW);
		elem_lv->enableOutline(Color4B::BLACK, 1);

		Sprite* elem_quality = ReadSpriteName("new_ui/hero/herobox_q1.png");
		elem_quality->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_quality);

		Sprite* elem_soul_bg = ReadSpriteName("new_ui/hero/herobox_jiangh1.png");
		elem_soul_bg->setAnchorPoint(Vec2(0.5, 0.5));
		elem_soul_bg->setPosition(Vec2(iBgWidth / 2, 33));
		elem_bg->addChild(elem_soul_bg,3);

		Sprite* elem_cursoul = ReadSpriteName("new_ui/hero/herobox_jiangh2.png");
		elem_cursoul->setAnchorPoint(Vec2(0,0.5));
		elem_cursoul->setPosition(Vec2(6, elem_soul_bg->getContentSize().height/2));
		elem_soul_bg->addChild(elem_cursoul,3);

		Label* elem_soul_num = LabelSystemFont("20/80", 12);
		elem_soul_num->setAnchorPoint(Vec2(1, 0));
		elem_soul_num->setPosition(Vec2(elem_soul_bg->getContentSize().width - 5, elem_soul_bg->getContentSize().height / 2 + elem_cursoul->getContentSize().height/2));
		elem_soul_num->enableOutline(Color4B::BLACK,1);
		elem_soul_bg->addChild(elem_soul_num,3);

		Sprite* elem_recruit = ReadSpriteName("new_ui/hero/herobox_xz1.png");
		elem_recruit->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_recruit);
		elem_recruit->setVisible(false);

		Sprite* elem_soul = ReadSpriteName("new_ui/hero/icon_soul.png");
		elem_soul->setAnchorPoint(Vec2(0, 0.5));
		elem_soul->setPosition(Vec2(10, 33));
		elem_soul->setScale(0.5f);
		elem_bg->addChild(elem_soul,3);

		Sprite* elem_occupation = ReadSpriteName("new_ui/hero/job1.png");
		elem_occupation->setAnchorPoint(Vec2(0, 1));
		elem_occupation->setPosition(Vec2(4,iBgHeight-5));
		elem_occupation->setScale(0.65f);
		elem_bg->addChild(elem_occupation,4);


		elem[i] = new ArmorListInfo();
		elem[i]->setBG(elem_bg);
		elem[i]->setIMG(elem_img);
		elem[i]->setFASHION(elem_fashion);
		elem[i]->setQUALITY(elem_quality);
		elem[i]->setTNAME(elem_name);
		elem[i]->setLV(elem_lv);
		elem[i]->setSOUL(elem_soul_bg);
		elem[i]->setSOULNUM(elem_soul_num);
		elem[i]->setACTORID(-1);
		elem[i]->setMINDEX(idx * 5 + i);
		elem[i]->setMISOWN(0);
		elem[i]->setMRECRUIT(elem_recruit);
		elem[i]->setOCCUPATION(elem_occupation);
		elem[i]->setMSOULICON(elem_soul);
		elem[i]->setMCURSOUL(elem_cursoul);
	}


	UpDateCell(idx);
    initTouches();
	//for (auto _vk: elem)
	//{
	//	/*if (_vk.second->getBG()->getChildByTag(3021)==NULL)
	//	{*/
	//		HongDian::getInstence()->addHongDianNode(_vk.second->getBG(), 1000 + _vk.second->getACTORID());
	//	
	//}

	return true;
}

void ArmorListCell::initTouches()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        click_elem_index = -1;
        for (int i = 0; i < 5; i++)
        {
            Rect elemBox = elem[i]->getBG()->getBoundingBox();
            if (Rect(0, 0, elemBox.size.width, elemBox.size.height).containsPoint(elem[i]->getBG()->convertToNodeSpace(touch->getLocation()))
                &&elem[i]->getACTORID()>0)
            {
                click_elem_index = i;
                return true;
            }
        }
        return true;
    };
    listener->onTouchEnded = [&](Touch* touch, Event* event) {
        if (click_elem_index > -1 && mALTB->EnableShowChild()&&(abs((int)(touch->getStartLocation().y - touch->getLocation().y)) < 10 || Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)) //新手引导开启时总是能进入
        {
			playSound(SOUND_LEFT_MOUSE_CLICK);
			mALTB->AddActorInfoLayer(elem[click_elem_index]->getMINDEX());
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void ArmorListCell::UpDateCell(int idx)
{
	for (int i = 0; i < 5; i++)
	{
		ArmorListDate* ptrALD = mALTB->GetCellElem(idx * 5 + i);
		elem[i]->setMINDEX(idx * 5 + i);

		if (ptrALD->actorId <= 0)
		{
			elem[i]->getBG()->setVisible(false);
			elem[i]->setACTORID(-1);
		}
		else
		{
			elem[i]->getBG()->setVisible(true);

			HistoryActorPtr ptrActor = &history->actors[ptrALD->actorId - 1];

			int fashion_width = 48;
			if (ptrActor->mFashion[0] <= 0)
			{
				elem[i]->getFASHION()->setVisible(false);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
				fashion_width = tex->getContentSize().width;
				elem[i]->getIMG()->setTexture(tex);
			}
			else
			{
				elem[i]->getFASHION()->setVisible(true);
				__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
				elem[i]->getIMG()->setTexture(texfa);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
				fashion_width = tex->getContentSize().width;
				elem[i]->getFASHION()->setTexture(tex);
				elem[i]->getFASHION()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
				int iZOrder = 0;
				if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
					iZOrder = 2;
				elem[i]->getFASHION()->setLocalZOrder(iZOrder);
				fashion_width = texfa->getContentSize().width;
			}
			elem[i]->getIMG()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

			elem[i]->getTNAME()->setString(trData->actors[ptrActor->actor_id].getActorName());
			elem[i]->getTNAME()->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));

			elem[i]->setACTORID(ptrALD->actorId);

			elem[i]->getMRECRUIT()->setVisible(ptrALD->enable_recruit);

			elem[i]->getOCCUPATION()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));

			if (ptrALD->addOwnArmList)
			{
				__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
				elem[i]->getQUALITY()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

				elem[i]->getLV()->setVisible(true);
				elem[i]->getLV()->setString(IntToString(ptrActor->GetActorLv()));
				elem[i]->getSOUL()->setVisible(false);
				elem[i]->getMSOULICON()->setVisible(false);
				
				removeSpriteGray(elem[i]->getQUALITY());
				removeSpriteGray(elem[i]->getBG());
				removeSpriteGray(elem[i]->getIMG());
				removeSpriteGray(elem[i]->getFASHION());
				removeSpriteGray(elem[i]->getOCCUPATION());
				elem[i]->setMISOWN(1);
			}
			else
			{
				__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", trData->actors[ptrActor->actor_id].actor_quality);
				elem[i]->getQUALITY()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

				elem[i]->getLV()->setVisible(false);
				elem[i]->getSOUL()->setVisible(true);
				__String* ptrSoulN = __String::createWithFormat("%d/%d", ptrALD->current_soul, ptrALD->need_soul);
				elem[i]->getSOULNUM()->setString(ptrSoulN->getCString());
				elem[i]->getMSOULICON()->setVisible(true);
				float fSoulScalx = (float)ptrALD->current_soul / (float)ptrALD->need_soul;
				if (fSoulScalx > 1) fSoulScalx = 1.0f;
				elem[i]->getMCURSOUL()->setScaleX(fSoulScalx);
				
				addSpriteGray(elem[i]->getQUALITY());
				addSpriteGray(elem[i]->getBG());
				addSpriteGray(elem[i]->getIMG());
				addSpriteGray(elem[i]->getFASHION());
				addSpriteGray(elem[i]->getOCCUPATION());
				elem[i]->setMISOWN(0);
			}
			if (elem[i]->getBG()->getChildByTag(3021)==NULL)
			{
				HongDian::getInstence()->addHongDianNode(elem[i]->getBG(), 1000 + elem[i]->getACTORID());
				HongDian::getInstence()->HongDianelem[1000 + elem[i]->getACTORID()] = elem[i];
			}
			else
			{
				elem[i]->getBG()->getChildByTag(3021)->setVisible(false);
				elem[i]->getBG()->getChildByTag(3021)->setTag(3022);
				HongDian::getInstence()->addHongDianNode(elem[i]->getBG(), 1000 + elem[i]->getACTORID());
				HongDian::getInstence()->HongDianelem[1000 + elem[i]->getACTORID()] = elem[i];
				if (HongDian::getInstence()->HongDianNodeisVisibol[1001]==1)
				{
					HongDian::getInstence()->autoUpdateWuJiang(2);
				}
			}
					
		}
		
	}

}
ArmorListCell::ArmorListCell()
{
	
}
void ArmorListCell::onEnter()
{
	TableViewCell::onEnter();
}
void ArmorListCell::onExit()
{
	TableViewCell::onExit();
}