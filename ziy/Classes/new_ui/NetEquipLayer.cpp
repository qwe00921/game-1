#include "NetEquipLayer.h"
#include "Defines.h"
#include "EquipmentUp.h"
#include "GuideLayer.h"
#include "Net_HomePage.h"


NetEquipLayer* NetEquipLayer::create(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate, int iEtype)
{
	NetEquipLayer* pRet = new NetEquipLayer();
	if (pRet&&pRet->init(ptrActor, ptrDelegate, iEtype))
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

bool NetEquipLayer::init(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate, int iEtype)
{
	if (!Layer::init())
	{
		return false;
	}
	curActor = ptrActor;
	mDelegate = ptrDelegate;
	mEtype = iEtype;
	curPage = 1;
	maxPage = 1;
	selectIndex = 0;
	mFunctionType  = -1;

	mSize = ChessViewTool::getInstance()->getFullSize();
	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	if (!getModels())
	{
		return false;
	}
	addLeftLayer();

	int iFNType = mBoxDate[0]->getIsOwn().empty() ? EQ_TYPE_EQUIP : EQ_TYPE_UPLOAD;
	mGoodsInfo = GoodsInfoLayer::create(mGoodsDate[0],ptrActor, iFNType, this);
	mGoodsInfo->setBlackLayerVisible(false);
	mGoodsInfo->setBgPoint(mSize.width / 2 + 5, mSize.height / 2,0,0.5);
	mGoodsInfo->setEnableRemove(false);
	this->addChild(mGoodsInfo);
	history->updateGuide(EMIL_GUIDE);
	return true;
}

bool NetEquipLayer::getModels()
{
	int j = 0, own_index = -1;
	if (mEtype == GOOD_TYPE_WEAPON&&curActor->weapon.isNormalGood())
	{
		own_index = j;
		mGoodsDate[j++] = &curActor->weapon;
	}
	else if (mEtype == GOOD_TYPE_COAT&&curActor->coat.isNormalGood())
	{
		own_index = j;
		mGoodsDate[j++] = &curActor->coat;
	}
	else if (mEtype == GOOD_TYPE_ASSIST&&curActor->assist_good.isNormalGood())
	{
		own_index = j;
		mGoodsDate[j++] = &curActor->assist_good;
	}

	for (int i = 0; i < history->goods->size(); i++)
	{
		HistoryGoodsPtr ptrGoods = history->goods->elementAt(i);
		if (ptrGoods->GetGoodsType() != mEtype||trData->occupation_good_info[curActor->occupation_id][ptrGoods->getItId()] == 0||ptrGoods->isFashion()|| !ptrGoods->EnableEquip(curActor->actor_id+1))
		{
			continue;
		}
		mGoodsDate[j++] = history->goods->elementAt(i);
	}
	if (mGoodsDate.size() == 0)
		return false;

	maxPage = ceil(mGoodsDate.size() / 16.0);

	for (int i = 0, j = 0; i < 16 * maxPage; i++, j++)
	{
		if (mGoodsDate.count(i)>0)
		{
			boxModel* bm = new boxModel();
			bm->setBGPath("new_ui/img_global/item_box_bg.png");
			bm->setSelectImagePath("new_ui/img_global/selected_box1.png");
			bm->setUIType(3);

			int goodID = mGoodsDate[i]->good_id;

			bm->setStarLV(mGoodsDate[i]->iQuality);

			int pz = 0;
			if (mGoodsDate[i]->iQuality != 0)
			{
				pz = trData->strengthen_consume[mGoodsDate[i]->iQuality - 1].quality;
			}
			bm->setPZLV(pz);
			bm->setIsSelected(false);
			bm->setTBPath(history->GetGoodsImagePath(mGoodsDate[i]->good_id).c_str());
			bm->setIndex(i);
			bm->setIsOwn("");
			bm->setStoneID(mGoodsDate[i]->getStoneID());

			mBoxDate[j] = bm;
		}
		else
		{
			boxModel* bm = new boxModel();
			bm->setBGPath("new_ui/img_global/item_box_bg.png");
			bm->setSelectImagePath("new_ui/img_global/selected_box1.png");
			bm->setUIType(3);

			bm->setStarLV(0);
			bm->setPZLV(0);
			bm->setIsSelected(false);
			bm->setTBPath("");
			bm->setIndex(-1);
			bm->setIsOwn("");
			bm->setStoneID(0);

			mBoxDate[j] = bm;
		}
	}

	if (own_index >= 0)
		mBoxDate[own_index]->setIsOwn("new_ui/hero/hero_e.png");
	mBoxDate[0]->setIsSelected(true);
	return true;
}

void NetEquipLayer::addLeftLayer()
{
	Sprite* bg = ReadSpriteName("new_ui/img_global/box_bg1.png");
	bg->setAnchorPoint(Vec2(1,0.5));
	bg->setPosition(Vec2(mSize.width / 2 + 6, mSize.height / 2));
	this->addChild(bg,1);

	int bg_width = bg->getContentSize().width;
	int bg_height = bg->getContentSize().height;
	Sprite* bg_info = ReadSpriteName("new_ui/hero/hero_txt5.png");
	bg_info->setPosition(Vec2(bg_width/2-15,344));
	bg->addChild(bg_info);

	for (int i = 0; i < 16; i++)
	{
		if (mBoxDate[i])
		{
			BoxSprite* bs = BoxSprite::create(mBoxDate[i], this);
			bs->setPosition(Vec2((i % 4 * 62), (i / 4 * -62)) + Vec2(58+12+19, 296));
			bg->addChild(bs);
			mBoxsList[i] = bs;
		}
	}

	page_M = Menu::create();
	page_M->setPosition(Vec2(bg_width / 2, 45));
	bg->addChild(page_M);

	Sprite* next_N = ReadSpriteName("new_ui/img_global/switch1.png");
	next_N->setFlippedX(true);
	Sprite* next_S = ReadSpriteName("new_ui/img_global/switch1.png");
	next_S->setFlippedX(true);
	MenuItemSprite* next_item = MenuItemSprite::create(next_N, next_S, CC_CALLBACK_1(NetEquipLayer::ClickNextMenu, this));
	next_item->setPosition(Vec2(45, 0));
	page_M->addChild(next_item);

	Sprite* pre_N = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* pre_S = ReadSpriteName("new_ui/img_global/switch1.png");
	MenuItemSprite* pre_item = MenuItemSprite::create(pre_N, pre_S, CC_CALLBACK_1(NetEquipLayer::ClickPreMenu, this));
	pre_item->setPosition(Vec2(-45, 0));
	page_M->addChild(pre_item);

	__String* page_Cs = __String::createWithFormat("%d/%d", curPage, maxPage);
	page_ttf = LabelSystemFont(page_Cs->getCString(),18);
	page_ttf->setColor(Color3B(102,0,0));
	page_ttf->setPosition(Vec2(bg_width / 2, 45));
	bg->addChild(page_ttf);

	close_M = Menu::create();
	close_M->setPosition(Vec2(680,364));
	bg->addChild(close_M);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* close_MIt = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(NetEquipLayer::ClickCloseMenu, this));
	close_M->addChild(close_MIt);
}

void NetEquipLayer::updateShow(int index, int type)
{
	int box_list_index = selectIndex % 16;
	mBoxDate[selectIndex]->setIsSelected(false);
	mBoxsList[box_list_index]->updateBox(mBoxDate[selectIndex]);

	box_list_index = index % 16;//转换下标
	selectIndex = index;
	mBoxDate[index]->setIsSelected(true);
	mBoxsList[box_list_index]->updateBox(mBoxDate[index]);

	UpdateRightLayer(index);
}

void NetEquipLayer::UpdateRightLayer(int iIndex)
{
	if (mGoodsDate.count(iIndex))
	{
		int iFNType = mBoxDate[iIndex]->getIsOwn().empty() ? EQ_TYPE_EQUIP : EQ_TYPE_UPLOAD;
		GoodsInfoLayer* oldGoodsInfo = mGoodsInfo;
		mGoodsInfo = GoodsInfoLayer::create(mGoodsDate[iIndex],curActor, iFNType, this);
		mGoodsInfo->setBlackLayerVisible(false);
		mGoodsInfo->setEnableRemove(false);
		mGoodsInfo->setBgPoint(mSize.width / 2 + 5, mSize.height / 2, 0, 0.5);
		this->addChild(mGoodsInfo);
		oldGoodsInfo->removeFromParent();
	}
}

void NetEquipLayer::ClickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (maxPage != 1)
	{
		curPage++;
		if (curPage > maxPage)
			curPage = 1;
		upDateAllBox();
	}
}

void NetEquipLayer::ClickPreMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (maxPage != 1)
	{
		curPage--;
		if (curPage <1)
			curPage = maxPage;
		upDateAllBox();
	}
}

void NetEquipLayer::ClickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
	this->removeFromParent();
}

void NetEquipLayer::upDateAllBox()
{
	mBoxDate[selectIndex]->setIsSelected(false);

	selectIndex = (curPage - 1) * 16;
	mBoxDate[selectIndex]->setIsSelected(true);
	for (int i = 0; i < 16; i++)
	{
		mBoxsList[i]->updateBox(mBoxDate[i+(curPage-1)*16],true);
	}

	page_ttf->setString(__String::createWithFormat("%d/%d", curPage, maxPage)->getCString());
	UpdateRightLayer(selectIndex);
}

void NetEquipLayer::UpdateEquipment(int iFNType)
{
	ITools::getInstance()->addWaitLayer();
	mFunctionType = iFNType;
	int iZid = 0, iXid = 0;

	if (mFunctionType == EQ_TYPE_EQUIP)
	{
		HistoryGoodsPtr ptrGood = mGoodsDate[selectIndex];
		iZid = ptrGood->getNetID();

		if (!history->ActorEnableEquipGoods(ptrGood, curActor->actor_id + 1))
			return;

		if (ptrGood->GetGoodsType() == GOOD_TYPE_WEAPON&&curActor->weapon.isNormalGood())
		{
			iXid = curActor->weapon.getNetID();
		}
		else if (ptrGood->GetGoodsType() == GOOD_TYPE_COAT&&curActor->coat.isNormalGood())
		{
			iXid = curActor->coat.getNetID();
		}
		else if (ptrGood->GetGoodsType() == GOOD_TYPE_ASSIST&&curActor->assist_good.isNormalGood())
		{
			iXid = curActor->assist_good.getNetID();
		}
	}
	else if (mFunctionType == EQ_TYPE_UPLOAD)
	{
		if (mEtype == GOOD_TYPE_WEAPON&&curActor->weapon.isNormalGood())
		{
			iXid = curActor->weapon.getNetID();
		}
		else if (mEtype == GOOD_TYPE_COAT&&curActor->coat.isNormalGood())
		{
			iXid = curActor->coat.getNetID();
		}
		else if (mEtype == GOOD_TYPE_ASSIST&&curActor->assist_good.isNormalGood())
		{
			iXid = curActor->assist_good.getNetID();
		}
	}


	SendNetDate(iZid,iXid);
}

bool NetEquipLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void NetEquipLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void NetEquipLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

}


void NetEquipLayer::onExit()
{
	Layer::onExit();
}

void NetEquipLayer::onEnter()
{
	Layer::onEnter();
}

NetEquipLayer::NetEquipLayer()
{

}
NetEquipLayer::~NetEquipLayer()
{
	for (size_t i = 0; i < mBoxDate.size(); i++)
	{
		delete mBoxDate[i];
		mBoxDate[i] = NULL;
	}
}

void NetEquipLayer::SendNetDate(int iZid, int iXid)
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("zid=%d&xid=%d&hid=%d&gameToken=%s", iZid, iXid ,curActor->actor_id+1, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"zid\":%d,\"xid\":%d,\"hid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iZid, iXid, curActor->actor_id + 1, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("equipment",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(NetEquipLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

}

void NetEquipLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
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
		if(value["result"]["isTrue"].isInt() && value["result"]["isTrue"].asInt() == 1)
		{
			ActorSetEquip();
			
		}
	}
}

void NetEquipLayer::ActorSetEquip()
{
	if (mFunctionType == EQ_TYPE_EQUIP)
	{
		HistoryGoodsPtr ptrGood = mGoodsDate[selectIndex];
		int curGoodID = ptrGood->good_id;
		HistoryGoods tempGood(ptrGood);
		if (ptrGood->GetGoodsType() == GOOD_TYPE_WEAPON)
		{
			if (curActor->weapon.isNormalGood())
			{
				history->goods->addElement(new HistoryGoods(&curActor->weapon));
				curActor->weapon.good_id = -1;
			}

			curActor->weapon.setGood(&tempGood);
			history->goods->removeElement(ptrGood);
			curActor->UpdataGoodEffect();
		}
		else if (ptrGood->GetGoodsType() == GOOD_TYPE_COAT)
		{
			if (curActor->coat.isNormalGood())
			{
				history->goods->addElement(new HistoryGoods(&curActor->coat));
				curActor->coat.good_id = -1;
			}

			curActor->coat.setGood(&tempGood);
			history->goods->removeElement(ptrGood);
			curActor->UpdataGoodEffect();
		}
		else if (ptrGood->GetGoodsType() == GOOD_TYPE_ASSIST)
		{
			if (curActor->assist_good.isNormalGood())
			{
				history->goods->addElement(new HistoryGoods(&curActor->assist_good));
				curActor->assist_good.good_id = -1;
			}

			curActor->assist_good.setGood(&tempGood);
			history->goods->removeElement(ptrGood);
			curActor->UpdataGoodEffect();
		}
		__String* str = __String::createWithFormat(LL("zbzb"), tempGood.GetGoodsName());
		ITools::getInstance()->createGlobalPrompt(str->getCString(), true);
	}
	else if (mFunctionType == EQ_TYPE_UPLOAD)
	{
		HistoryGoodsPtr ptrGoods = NULL;
		if (mEtype == GOOD_TYPE_WEAPON)
		{
			ptrGoods = new HistoryGoods(&curActor->weapon);
			history->goods->addElement(ptrGoods);
			curActor->weapon.good_id = -1;
			curActor->UpdataGoodEffect();
		}
		else if (mEtype == GOOD_TYPE_COAT)
		{
			ptrGoods = new HistoryGoods(&curActor->coat);
			history->goods->addElement(ptrGoods);
			curActor->coat.good_id = -1;
			curActor->UpdataGoodEffect();
		}
		else if (mEtype == GOOD_TYPE_ASSIST)
		{
			ptrGoods = new HistoryGoods(&curActor->assist_good);
			history->goods->addElement(ptrGoods);
			curActor->assist_good.good_id = -1;
			curActor->UpdataGoodEffect();
		}
		__String* str = __String::createWithFormat(LL("xxzb"), ptrGoods->GetGoodsName());
		ITools::getInstance()->createGlobalPrompt(str->getCString(), true);
	}

	this->mDelegate->updateShow(0);
	history->updateGuide(EMIL_GUIDE);
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id, ANNIU_HONGDIAN);
	this->removeFromParent();
}

void NetEquipLayer::RunFunction(int iNomal)
{
	if (iNomal == 1)
	{
		if (mDelegate)
			mDelegate->RemoveAllParentLayer();
		HistoryGoodsPtr ptreGoods = NULL;
		if (mGoodsDate.count(selectIndex))
			ptreGoods = mGoodsDate[selectIndex];
		ChessApplication::getInstance()->AddFunctionLayer(EquipmentUp::create(ptreGoods));
		this->removeFromParent();
	}
}