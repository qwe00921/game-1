#include "GuardLayer.h"
#include "Defines.h"


GuardLayer* GuardLayer::create(int iWarId, EventDelegate* ptrDelegate)
{
	GuardLayer* pRet = new GuardLayer();
	if (pRet&&pRet->init(iWarId,ptrDelegate))
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

bool GuardLayer::init(int iWarId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iClickSIndex = -1;
	m_iWarId = iWarId;
	mDelegate = ptrDelegate;
	m_bEnableDispatch = true;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	NetWarInfo* ptrNetWarInfo = trData->m_mNetWarInfo[iWarId];

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/glc_war/glc_bg4.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	int iTBgWidth = bg->getContentSize().width;
	int iTBgHeight = bg->getContentSize().height;

	int iGuardActorLv = ptrNetWarInfo->guarding.begin()->second;

	Label* numInfoLab = LabelSystemFont(StringUtils::format(LL("xydjjysdwjzsgk"), iGuardActorLv).c_str(),16);
	numInfoLab->setPosition(Vec2(iTBgWidth /2,286));
	bg->addChild(numInfoLab);


	int iShowElemNum = ptrNetWarInfo->guarding.size();

	Sprite* actorImgbg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
	int iShowESpaace = actorImgbg->getContentSize().width + 20;
	
	int iPointX = (iTBgWidth - iShowESpaace*(iShowElemNum - 1))/2;

	for (int i = 0; i < iShowElemNum; i++)
	{
		Sprite* elem_bg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
		elem_bg->setPosition(Vec2(iPointX + i*iShowESpaace,232));
		bg->addChild(elem_bg);

		int iBgWidth = elem_bg->getContentSize().width;
		int iBgHeight = elem_bg->getContentSize().height;

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_fashion, 1);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_img, 1);

		Label* elem_name = LabelSystemFont("", 16);
		elem_name->setPosition(Vec2(iBgWidth / 2, 21));
		elem_bg->addChild(elem_name, 3);
		elem_name->enableOutline(Color4B::BLACK, 1);

		Label* elem_lv = LabelSystemFont("", 16);
		elem_lv->setAnchorPoint(Vec2(1, 1));
		elem_lv->setPosition(Vec2(83, 81));
		elem_bg->addChild(elem_lv, 3);
		elem_lv->setColor(Color3B::YELLOW);
		elem_lv->enableOutline(Color4B::BLACK, 1);

		Sprite* elem_quality = ReadSpriteName("new_ui/hero/herobox_q1.png");
		elem_quality->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_quality);

		Sprite* elem_occupation = ReadSpriteName("new_ui/hero/job1.png");
		elem_occupation->setAnchorPoint(Vec2(0, 1));
		elem_occupation->setPosition(Vec2(4, iBgHeight - 5));
		elem_occupation->setScale(0.65f);
		elem_bg->addChild(elem_occupation, 4);

		Sprite* elem_addSp = ReadSpriteName("new_ui/hero/hero_add.png");
		elem_addSp->setPosition(Vec2(iBgWidth/2,iBgHeight/2));
		elem_bg->addChild(elem_addSp,4);

		GuardShowElem* ptrGuardSE = new GuardShowElem();
		ptrGuardSE->setBG(elem_bg);
		ptrGuardSE->setFASHION(elem_fashion);
		ptrGuardSE->setIMG(elem_img);
		ptrGuardSE->setTNAME(elem_name);
		ptrGuardSE->setLV(elem_lv);
		ptrGuardSE->setQUALITY(elem_quality);
		ptrGuardSE->setOCCUPATION(elem_occupation);
		ptrGuardSE->setADDACTOR(elem_addSp);
		ptrGuardSE->setACTORID(-1);
		mShowElem[i] = ptrGuardSE;
	}

	int iRGoodsNum = ptrNetWarInfo->drop.size();
	int iRGPointX = (iTBgWidth - 70 * iRGoodsNum)/2;
	int iRGIndex = 0;
	for (map<int, int>::iterator eIt = ptrNetWarInfo->drop.begin(); eIt != ptrNetWarInfo->drop.end(); eIt++)
	{
		Sprite* RGoodsImgSp = ReadSpriteName(StringUtils::format("new_ui/glc_war/icon_%d.png", eIt->first).c_str());
		RGoodsImgSp->setPosition(Vec2(iRGPointX + iRGIndex*70,153));
		RGoodsImgSp->setAnchorPoint(Vec2(0,0.5));
		bg->addChild(RGoodsImgSp);

		Label* RGoodsNumLab = LabelSystemFont("3000",16);
		RGoodsNumLab->setColor(Color3B::YELLOW);
		RGoodsNumLab->setAnchorPoint(Vec2(0,0.5));
		RGoodsNumLab->setPosition(Vec2(RGoodsImgSp->getContentSize().width, RGoodsImgSp->getContentSize().height/2));
		RGoodsImgSp->addChild(RGoodsNumLab);

		mGuardReward[eIt->first] = RGoodsNumLab;
		iRGIndex++;
	}

	Label* info1Lab = LabelSystemFont(LL("zgwjyzgdjyxcc"), 16);
	info1Lab->setPosition(Vec2(iTBgWidth/2,124));
	bg->addChild(info1Lab);

	Label* info2Lab = LabelSystemFont(LL("zgwjbzbntzzhdgq"), 16);
	info2Lab->setPosition(Vec2(iTBgWidth/2, 104));
	bg->addChild(info2Lab);

	//确定
	Menu* sure = Menu::create();
	sure->setPosition(Vec2(bg->getContentSize().width / 2, 46));
	Sprite* sureNormalImage = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* sureSelectImage = ReadSpriteName("new_ui/img_global/global_ban4.png");
	MenuItemSprite* right = MenuItemSprite::create(sureNormalImage, sureSelectImage, CC_CALLBACK_1(GuardLayer::clickSureMenu, this));
	sure->addChild(right);
	bg->addChild(sure);

	Sprite* sureTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_9.png");
	sureTXTN->setPosition(Vec2(40, 18));
	sureNormalImage->addChild(sureTXTN);
	Sprite* sureTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_10.png");
	sureTXTS->setPosition(Vec2(40, 18));
	sureSelectImage->addChild(sureTXTS);

	//关闭
	Menu* mMenuClose = Menu::create();
	mMenuClose->setPosition(Vec2(376, 310));
	bg->addChild(mMenuClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(GuardLayer::clickCloseMenu, this));
	mMenuClose->addChild(close_menu);

	if (history->mMapDateInfo->GetBattleWolfData()->GetWolfGuardData(m_iWarId).size() > 0)
	{
		sure->setVisible(false);
		mActorIdList = history->mMapDateInfo->GetBattleWolfData()->GetWolfGuardData(m_iWarId);
		m_bEnableDispatch = false;
	}

	////////////////
	UpdataElemShow();
	return true;
}

bool GuardLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (!m_bEnableDispatch) return true;

	m_iClickSIndex = -1;
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		Rect RGBgBox = mShowElem[i]->getBG()->getBoundingBox();
		if (Rect(0, 0, RGBgBox.size.width, RGBgBox.size.height).containsPoint(mShowElem[i]->getBG()->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickSIndex = i;
			return true;
		}
	}

	return true;
}
void GuardLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (m_bEnableDispatch&&m_iClickSIndex > -1)
	{
		Rect RGBgBox = mShowElem[m_iClickSIndex]->getBG()->getBoundingBox();
		if (!Rect(0, 0, RGBgBox.size.width, RGBgBox.size.height).containsPoint(mShowElem[m_iClickSIndex]->getBG()->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickSIndex = -1;
			return;
		}
	}
}
void GuardLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (m_bEnableDispatch&&m_iClickSIndex > -1)
	{
		SelectGuradActorTableView* ptrSGATBV = SelectGuradActorTableView::create(mActorIdList, m_iWarId, this);
		if (ptrSGATBV)
			ChessApplication::getInstance()->AddFunctionLayer(ptrSGATBV);
		else
			ITools::getInstance()->createGlobalPrompt(LL("mqmyshdwjzs"), false);
	}
}

void GuardLayer::onTouchesCancelled(Touch * touch, Event * unused_event)
{
}

void GuardLayer::UpdataElemShow()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		if (mActorIdList.count(i)&& mActorIdList[i] > 0)
		{
			HistoryActorPtr ptrActor = &history->actors[mActorIdList[i] - 1];

			int fashion_width = 48;
			mShowElem[i]->getIMG()->setVisible(true);
			if (ptrActor->mFashion[0] <= 0)
			{
				mShowElem[i]->getFASHION()->setVisible(false);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
				fashion_width = tex->getContentSize().width;
				mShowElem[i]->getIMG()->setTexture(tex);
			}
			else
			{
				mShowElem[i]->getFASHION()->setVisible(true);
				__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
				mShowElem[i]->getIMG()->setTexture(texfa);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
				fashion_width = tex->getContentSize().width;
				mShowElem[i]->getFASHION()->setTexture(tex);
				mShowElem[i]->getFASHION()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
				int iZOrder = 0;
				if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
					iZOrder = 2;
				mShowElem[i]->getFASHION()->setLocalZOrder(iZOrder);
				fashion_width = texfa->getContentSize().width;
			}
			mShowElem[i]->getIMG()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

			mShowElem[i]->getTNAME()->setVisible(true);
			mShowElem[i]->getTNAME()->setString(trData->actors[ptrActor->actor_id].getActorName());
			mShowElem[i]->getTNAME()->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));

			mShowElem[i]->setACTORID(mActorIdList[i]);

			mShowElem[i]->getOCCUPATION()->setVisible(true);
			mShowElem[i]->getOCCUPATION()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));

			mShowElem[i]->getQUALITY()->setVisible(true);
			__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
			mShowElem[i]->getQUALITY()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

			mShowElem[i]->getLV()->setVisible(true);
			mShowElem[i]->getLV()->setString(IntToString(ptrActor->GetActorLv()));

			mShowElem[i]->getADDACTOR()->setVisible(false);
		}
		else
		{
			mShowElem[i]->getQUALITY()->setTexture(ReadSpriteTexture("new_ui/hero/herobox_q1.png"));
			mShowElem[i]->getFASHION()->setVisible(false);
			mShowElem[i]->getIMG()->setVisible(false);
			mShowElem[i]->getTNAME()->setVisible(false);
			mShowElem[i]->getLV()->setVisible(false);
			mShowElem[i]->getOCCUPATION()->setVisible(false);
			mShowElem[i]->getADDACTOR()->setVisible(true);
			mShowElem[i]->setACTORID(-1);
		}
	}


	HistoryActorPtr ptrHighestLvActor = NULL;
	if (mActorIdList.size()&& mActorIdList.begin()->second>0)
	{
		ptrHighestLvActor = &history->actors[mActorIdList.begin()->second -1];

		for (map<int, int>::iterator iIt = mActorIdList.begin(); iIt != mActorIdList.end(); iIt++)
		{
			if (iIt->second > 0 && history->actors[iIt->second - 1].actor_quality > ptrHighestLvActor->actor_quality)
			{
				ptrHighestLvActor = &history->actors[iIt->second - 1];
			}
		}
	}

	int iProduce = 0;
	NetWarInfo* ptrNetWarInfo = trData->m_mNetWarInfo[m_iWarId];

	for (map<int, Label*>::iterator eIt = mGuardReward.begin(); eIt != mGuardReward.end(); eIt++)
	{
		iProduce = 0;
		if (ptrHighestLvActor)
		{
			iProduce = ptrNetWarInfo->drop[eIt->first] * ((1+(ptrHighestLvActor->GetActorLv()-1)/50)*((ptrHighestLvActor->actor_quality+1)/3));
		}
		eIt->second->setString(IntToString(iProduce));
	}
}

void GuardLayer::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	//镇守武将不足
	if (trData->m_mNetWarInfo[m_iWarId]->guarding.size()!= mActorIdList.size())
	{
		ITools::getInstance()->createGlobalPrompt(LL("zswjslbz"), false);
	}
	else
	{
		SendNetDate();
	}
}

void GuardLayer::RunFunction(int iNomal)
{
	if (iNomal > 0&& m_iClickSIndex>-1)
	{
		mActorIdList[m_iClickSIndex] = iNomal;
		UpdataElemShow();
	}
}

void GuardLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void GuardLayer::SendNetDate()
{
	ITools::getInstance()->addWaitLayer();

	Json::Value HeroList(arrayValue);
	int iHeroLIndex = 0;
	for (map<int, int>::iterator iIt = mActorIdList.begin(); iIt != mActorIdList.end(); iIt++)
	{
		if(iIt->second>0)
			HeroList[iHeroLIndex++] = iIt->second;
	}

	FastWriter fastWriter;
	string heroesS = fastWriter.write(HeroList);
	heroesS.erase(heroesS.end() - 1);


	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s&warID=%d&heroes=%s",GameTokenS.c_str(),m_iWarId, heroesS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"warID\":%d,\"heroes\":%s,\"validateCode\":\"%s\"}", GameTokenS.c_str(), m_iWarId, heroesS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("setGaoLangHero",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GuardLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);
}

void GuardLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"]["isTrue"].isInt()&& value["result"]["isTrue"].asInt() == 1)
		{
			if (mDelegate) mDelegate->updateRTDate(m_iWarId);
			__String* ptrScreenName = __String::createWithFormat(LL("sgyzswj"), trData->m_mNetWarInfo[m_iWarId]->strName.c_str());
			ITools::getInstance()->createGlobalPrompt(ptrScreenName->getCString(), true);
			history->mMapDateInfo->GetBattleWolfData()->SetWolfGuardData(m_iWarId, mActorIdList);
			this->removeFromParent();
		}
	}

	ITools::getInstance()->removeWaitLayer();
}

GuardLayer::GuardLayer()
{

}

GuardLayer::~GuardLayer()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		delete mShowElem[i];
	}
}


//////////////////////////////////////Gorgeous split line
#define GUARD_VIEW_W 530
#define GUARD_VIEW_H 220
#define GUARD_ITEM_W 106
#define GUARD_ITEM_H 110
SelectGuradActorTableView* SelectGuradActorTableView::create(map<int, int> mGuradList,int iWarId, EventDelegate* ptrDelegate, int iType)
{
	SelectGuradActorTableView* pRet = new SelectGuradActorTableView();
	if (pRet&&pRet->init(mGuradList,iWarId,ptrDelegate,iType))
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

bool SelectGuradActorTableView::init(map<int, int> mGuradList,int iWarId, EventDelegate* ptrDelegate, int iType)
{
	if (!Layer::init())
	{
		return false;
	}

	mDelegate = ptrDelegate;
	m_iWarId = iWarId;
	m_iType = iType;

	GetElemData(mGuradList);
	if (!mActorList.size()) return false;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	//遮罩
	LayerColor* spBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	spBlack->setPosition(Vec2(0, 0));
	this->addChild(spBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/hero/hero_exp_bg.png");
	spBG->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(spBG);

	if (m_iType == GUARD_SELECT_ACTOR)
	{
		//提示
		int iActorMinLv = 10000;
		if (trData->m_mNetWarInfo.count(m_iWarId) && trData->m_mNetWarInfo[m_iWarId]->guarding.size())
			iActorMinLv = trData->m_mNetWarInfo[m_iWarId]->guarding.begin()->second;
		Label* infoLab = LabelSystemFont(StringUtils::format(LL("qxzsdjdwjzsgq"), iActorMinLv).c_str(), 16);
		infoLab->setPosition(Vec2(spBG->getContentSize().width / 2, spBG->getContentSize().height - 37));
		spBG->addChild(infoLab);
	}

	//关闭
	Menu* m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(290 + spBG->getContentSize().width/2, 160+ spBG->getContentSize().height / 2));
	spBG->addChild(m_pBtnMenu);

	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(SelectGuradActorTableView::clickCloseMenu, this));
	m_pBtnMenu->addChild(btnClose);

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
	tableView = TableView::create(this, Size(GUARD_VIEW_W, GUARD_VIEW_H));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	tableView->setPosition(Vec2(size.width / 2 - GUARD_VIEW_W / 2, size.height / 2 - GUARD_VIEW_H / 2 - 10));
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


void SelectGuradActorTableView::GetElemData(map<int, int> mGuradList)
{
	int iActorListIndex = 0;
	if (m_iType == GUARD_SELECT_ACTOR)
	{
		if (!trData->m_mNetWarInfo.count(m_iWarId)) return;

		map<int, int> eGuradActor;
		for (map<int, int>::iterator iIt = mGuradList.begin(); iIt != mGuradList.end(); iIt++)
		{
			eGuradActor[iIt->second] = 1;
		}

		map<int, int> eAllGuradList = history->mMapDateInfo->GetBattleWolfData()->getAllGuardActor();

		NetWarInfo* ptrNetWarInfo = trData->m_mNetWarInfo[m_iWarId];

		int iActorMinLv = 10000;
		if (ptrNetWarInfo->guarding.size())
			iActorMinLv = ptrNetWarInfo->guarding.begin()->second;

		HistoryActorPtr ptrActor = NULL;
		for (int i = 0; i < history->ownArmyNum; i++)
		{
			ptrActor = &history->actors[history->ownArmy[i]];

			if (ptrActor->GetActorLv() >= iActorMinLv && !eGuradActor.count(history->ownArmy[i] + 1) && !eAllGuradList.count(history->ownArmy[i] + 1))
			{
				mActorList[iActorListIndex++] = history->ownArmy[i] + 1;
			}
		}
	}
	else if (m_iType == ARRAY_SELECT_ACTOR)
	{
		map<int, int> eGuradActor;
		for (map<int, int>::iterator iIt = mGuradList.begin(); iIt != mGuradList.end(); iIt++)
		{
			eGuradActor[iIt->second] = 1;
		}

		for (int i = 0; i < history->ownArmyNum; i++)
		{
			if (!eGuradActor.count(history->ownArmy[i] + 1))
			{
				mActorList[iActorListIndex++] = history->ownArmy[i] + 1;
			}
		}
	}


	cell_num = ceil(iActorListIndex / 5.0f);
}

void SelectGuradActorTableView::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

map<int, int> SelectGuradActorTableView::getCellElem(int iIdx)
{
	map<int, int> mRet;
	int iStarindex = iIdx * 5;
	int iElemindex = 0;
	for (int i = iStarindex; i < (int)mActorList.size() && i < iStarindex + 5; i++)
	{
		mRet[iElemindex++] = mActorList[i];
	}

	return mRet;
}

void SelectGuradActorTableView::RunFunction(int iNomal)
{
	if (iNomal > 0&&mDelegate)
	{
		mDelegate->RunFunction(iNomal);
		this->removeFromParent();
	}
}

SelectGuradActorTableView::SelectGuradActorTableView()
{
	cell_num = 1;
}

SelectGuradActorTableView::~SelectGuradActorTableView()
{

}

void SelectGuradActorTableView::scrollViewDidScroll(ScrollView* view)
{

}

void SelectGuradActorTableView::scrollViewDidZoom(ScrollView* view)
{

}

void SelectGuradActorTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void SelectGuradActorTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void SelectGuradActorTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size SelectGuradActorTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(GUARD_VIEW_W, GUARD_ITEM_H);
}

TableViewCell* SelectGuradActorTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();

	if (!cell) {
		cell = SelectGuradActorCell::create(getCellElem(idx),this);
	}
	else
	{
		SelectGuradActorCell* c = (SelectGuradActorCell*)cell;
		c->UpDateCell(getCellElem(idx),this);
	}
	return cell;
}

ssize_t SelectGuradActorTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

//////////////////////////////////////Gorgeous split line

SelectGuradActorCell* SelectGuradActorCell::create(map<int, int> mElemData, EventDelegate* ptrDelegate)
{
	SelectGuradActorCell* pRet = new SelectGuradActorCell();
	if (pRet&&pRet->init(mElemData, ptrDelegate))
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

bool SelectGuradActorCell::init(map<int, int> mElemData, EventDelegate* ptrDelegate)
{
	if (!TableViewCell::init())
	{
		return false;
	}

	int iPointX = GUARD_ITEM_W / 2;
	for (int i = 0; i < 5; i++)
	{
		Sprite* elem_bg = ReadSpriteName("new_ui/hero/herobox_bg1.png");
		elem_bg->setPosition(Vec2(iPointX + i*GUARD_ITEM_W, GUARD_ITEM_H/2));
		this->addChild(elem_bg);

		int iBgWidth = elem_bg->getContentSize().width;
		int iBgHeight = elem_bg->getContentSize().height;

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_fashion, 1);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iBgWidth / 2, 57));
		elem_bg->addChild(elem_img, 1);

		Label* elem_name = LabelSystemFont("", 16);
		elem_name->setPosition(Vec2(iBgWidth / 2, 21));
		elem_bg->addChild(elem_name, 3);
		elem_name->enableOutline(Color4B::BLACK, 1);

		Label* elem_lv = LabelSystemFont("", 16);
		elem_lv->setAnchorPoint(Vec2(1, 1));
		elem_lv->setPosition(Vec2(83, 81));
		elem_bg->addChild(elem_lv, 3);
		elem_lv->setColor(Color3B::YELLOW);
		elem_lv->enableOutline(Color4B::BLACK, 1);

		Sprite* elem_quality = ReadSpriteName("new_ui/hero/herobox_q1.png");
		elem_quality->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		elem_bg->addChild(elem_quality);

		Sprite* elem_occupation = ReadSpriteName("new_ui/hero/job1.png");
		elem_occupation->setAnchorPoint(Vec2(0, 1));
		elem_occupation->setPosition(Vec2(4, iBgHeight - 5));
		elem_occupation->setScale(0.65f);
		elem_bg->addChild(elem_occupation, 4);

		GuardShowElem* ptrGuardSE = new GuardShowElem();
		ptrGuardSE->setBG(elem_bg);
		ptrGuardSE->setFASHION(elem_fashion);
		ptrGuardSE->setIMG(elem_img);
		ptrGuardSE->setTNAME(elem_name);
		ptrGuardSE->setLV(elem_lv);
		ptrGuardSE->setQUALITY(elem_quality);
		ptrGuardSE->setOCCUPATION(elem_occupation);
		ptrGuardSE->setADDACTOR(NULL);
		ptrGuardSE->setACTORID(-1);
		mShowElem[i] = ptrGuardSE;
	}

	UpDateCell(mElemData,ptrDelegate);
	initTouches();
	return true;
}

void SelectGuradActorCell::UpDateCell(map<int, int> mElemData, EventDelegate* ptrDelegate)
{
	mDelegate = ptrDelegate;

	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		if (mElemData.count(i))
		{
			mShowElem[i]->getBG()->setVisible(true);

			HistoryActorPtr ptrActor = &history->actors[mElemData[i] - 1];

			int fashion_width = 48;
			if (ptrActor->mFashion[0] <= 0)
			{
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
				fashion_width = tex->getContentSize().width;
				mShowElem[i]->getIMG()->setTexture(tex);
				mShowElem[i]->getFASHION()->setVisible(false);
			}
			else
			{
				__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

				mShowElem[i]->getFASHION()->setVisible(true);
				Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
				mShowElem[i]->getIMG()->setTexture(texfa);
				Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
				fashion_width = tex->getContentSize().width;
				mShowElem[i]->getFASHION()->setTexture(tex);
				mShowElem[i]->getFASHION()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
				int iZOrder = 0;
				if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
					iZOrder = 2;
				mShowElem[i]->getFASHION()->setLocalZOrder(iZOrder);
				fashion_width = texfa->getContentSize().width;
			}
			mShowElem[i]->getIMG()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

			mShowElem[i]->getTNAME()->setString(trData->actors[ptrActor->actor_id].getActorName());
			mShowElem[i]->getTNAME()->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));

			mShowElem[i]->setACTORID(mElemData[i]);

			mShowElem[i]->getOCCUPATION()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId()).c_str())));

			__String* quality_icon_path = __String::createWithFormat("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
			mShowElem[i]->getQUALITY()->setTexture(Director::getInstance()->getTextureCache()->addImage(getRespath(quality_icon_path->getCString(), 0, 1)));

			mShowElem[i]->getLV()->setString(IntToString(ptrActor->GetActorLv()));
		}
		else
		{
			mShowElem[i]->getBG()->setVisible(false);
			mShowElem[i]->setACTORID(-1);
		}
	}
}

void SelectGuradActorCell::initTouches()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_iClickShowindex = -1;
		for (size_t i = 0; i < mShowElem.size(); i++)
		{
			Rect EBgBox = mShowElem[i]->getBG()->getBoundingBox();
			if (mShowElem[i]->getACTORID() > -1 && Rect(0, 0, EBgBox.size.width, EBgBox.size.height).containsPoint(mShowElem[i]->getBG()->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickShowindex = i;
				return true;
			}
		}
		return true;
	};

	listener->onTouchMoved = [&](Touch *touch, Event *unused_event)
	{
		if (m_iClickShowindex > -1)
		{
			Rect EBgBox = mShowElem[m_iClickShowindex]->getBG()->getBoundingBox();
			if (!Rect(0, 0, EBgBox.size.width, EBgBox.size.height).containsPoint(mShowElem[m_iClickShowindex]->getBG()->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickShowindex = -1;
				return;
			}
		}
	};

	listener->onTouchEnded = [&](Touch *touch, Event *unused_event)
	{
		if (m_iClickShowindex > -1 && mDelegate)
		{
			mDelegate->RunFunction(mShowElem[m_iClickShowindex]->getACTORID());
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

SelectGuradActorCell::SelectGuradActorCell()
{

}
SelectGuradActorCell::~SelectGuradActorCell()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		delete mShowElem[i];
	}
	mShowElem.clear();
}