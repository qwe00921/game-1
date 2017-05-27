#include "EquipFashionLayer.h"
#include "Defines.h"
#include "Net_HomePage.h"

EquipFashionLayer* EquipFashionLayer::create(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate)
{
	EquipFashionLayer* pRet = new EquipFashionLayer();
	if (pRet&&pRet->init(ptrActor,ptrDelegate))
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

bool EquipFashionLayer::init(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	mDelegate = ptrDelegate;
	curActor = ptrActor;
	m_iCurPage = 0;
	m_iMaxPage = 1;
	m_iClickDataIndex = 0;
	m_bEnableClick = true;
	actor_fashion = NULL;
	m_bEnableUpDataFatherShow = false;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	GetDataElem();
	if (!mDataList.size())
		return false;

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	bg = ReadSpriteName("new_ui/hero/hero_dress_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	string nameS = trData->actors[curActor->actor_id].getActorName();
	//name
	Label* nameLab = LabelSystemFont(nameS.c_str(),16);
	nameLab->setColor(ITools::getInstance()->getActorPzColor(ptrActor->actor_quality));
	nameLab->setPosition(Vec2(100,338));
	nameLab->enableOutline(Color4B::BLACK,1);
	bg->addChild(nameLab);

	//武将背景
	img_bg = ReadSpriteName("new_ui/hero/arms_bg.png");
	img_bg->setPosition(Vec2(100, 270));
	bg->addChild(img_bg);

	__String* f_path = NULL;
	__String* img_path = NULL;
	//时装
	if (curActor->mFashion[0])
	{
		f_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[3], curActor->mFashion[4]);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[1], curActor->mFashion[2]);
		actor_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
		int fashion_width = actor_fashion->getContentSize().width;
		actor_fashion->setTextureRect(Rect(0, fashion_width * 6, fashion_width, fashion_width));
		actor_fashion->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2 + 24));
		img_bg->addChild(actor_fashion, trData->net_hero_dress[curActor->mFashion[4]]->mMov[6]+1);
	}
	else
	{
		img_path = __String::create(curActor->getActorSImgPath("mov", 0));
	}

	//武将
	actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	int img_width = actor_img->getContentSize().width;
	actor_img->setTextureRect(Rect(0, img_width * 6, img_width, img_width));
	actor_img->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2+ 24));
	img_bg->addChild(actor_img,1);

	//58,192
	Sprite* wap_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	wap_bg->setPosition(Vec2(58,192));
	bg->addChild(wap_bg);

	wapSp = ReadSpriteNormal("img/Items/a1.png");
	wapSp->setPosition(Vec2(33,33));
	wap_bg->addChild(wapSp);

	//148,192
	Sprite* coat_bg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	coat_bg->setPosition(Vec2(148, 192));
	bg->addChild(coat_bg);

	coatSp = ReadSpriteNormal("img/Items/a1.png");
	coatSp->setPosition(Vec2(33, 33));
	coat_bg->addChild(coatSp);

	fashionEffect = LabelSystemFont("",16);
	fashionEffect->setPosition(Vec2(25,144));
	fashionEffect->setAnchorPoint(Vec2(0,1));
	fashionEffect->setColor(Color3B(255,255,153));
	fashionEffect->enableOutline(Color4B(102,51,0,255),1);
	fashionEffect->setDimensions(164,0);
	bg->addChild(fashionEffect);

	GetShowElem();

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(532, 344));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(EquipFashionLayer::ClickCloseMenu, this));
	mCloseMenu->addChild(close_menu);

	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;

	Menu* proMenu = Menu::create();
	proMenu->setPosition(Vec2(234, 175));
	bg->addChild(proMenu);

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(EquipFashionLayer::ClickProMenu, this));
	proMenu->addChild(pro_mItem);
	ProSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	ProSSp->setPosition(Vec2(ProSSp->getContentSize().width / 2, ProSSp->getContentSize().height / 2));

	Sprite* NextNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* NextSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	NextNSp->setFlippedX(true);
	NextSSp->setFlippedX(true);
	NextSSp->setScale(0.9f);

	Menu* NextMenu = Menu::create();
	NextMenu->setPosition(Vec2(534,175));
	bg->addChild(NextMenu);

	MenuItemSprite* Next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(EquipFashionLayer::ClickNextMenu, this));
	NextMenu->addChild(Next_mItem);
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));

	if (m_iMaxPage <= 1)
	{
		proMenu->setVisible(false);
		NextMenu->setVisible(false);
	}

	int mElemNum = ceil(mDataList.size()/4.0);
	if (mElemNum <= 0) mElemNum = 1;
	int iOffsetX = 18;

	int iStarPointX = 384 - (mElemNum - 1) * iOffsetX / 2;

	for (int i = 0; i < mElemNum; i++)
	{
		mElemDot[i] = ReadSpriteName("new_ui/img_global/suit_bnt3.png");
		mElemDot[i]->setPosition(Vec2(iStarPointX + i*iOffsetX, 20));
		bg->addChild(mElemDot[i]);
	}
	mCurDotSp = mElemDot[m_iCurPage];

	UpdataElemShow();
	UpdataLeftShow();

	return true;
}

void EquipFashionLayer::GetShowElem()
{
	int iElemBgWidth = 0;
	int iElemBgHeight = 0;
	for (int i = 0; i < 4; i++)
	{
		Sprite* elemBg = ReadSpriteName("new_ui/hero/hero_dress_bg3.png");
		elemBg->setAnchorPoint(Vec2(0,1));
		elemBg->setPosition(Vec2(264,312 - i*70));
		bg->addChild(elemBg);

		iElemBgWidth = elemBg->getContentSize().width;
		iElemBgHeight = elemBg->getContentSize().height;

		Sprite* itemBg = ReadSpriteName("new_ui/hero/hero_dress_bg4.png");
		itemBg->setPosition(Vec2(40,iElemBgHeight / 2));
		elemBg->addChild(itemBg);

		Sprite* itemImg = ReadSpriteNormal("img/Items/a1.png");
		itemImg->setPosition(Vec2(25,25));
		itemImg->setScale(0.8f);
		itemBg->addChild(itemImg);

		Label* itemName = LabelSystemFont("name",18);
		itemName->setAnchorPoint(Vec2(0,0));
		itemName->setColor(Color3B(102,0,0));
		itemName->setPosition(Vec2(66,iElemBgHeight / 2 + 2));
		elemBg->addChild(itemName);

		Label* itemEffect = LabelSystemFont("name", 16);
		itemEffect->setAnchorPoint(Vec2(0, 1));
		itemEffect->setColor(Color3B(102, 51, 0));
		itemEffect->setPosition(Vec2(66, iElemBgHeight / 2 - 2));
		elemBg->addChild(itemEffect);

		Sprite* lockSp = ReadSpriteName("new_ui/img_global/imgs_lock.png");
		lockSp->setPosition(Vec2(iElemBgWidth - 34,iElemBgHeight/2));
		elemBg->addChild(lockSp);

		Sprite* selectedSp = ReadSpriteName("new_ui/hero/hero_dress_bnt1.png");
		selectedSp->setPosition(Vec2(iElemBgWidth - 34, iElemBgHeight / 2));
		elemBg->addChild(selectedSp);
		selectedSp->setVisible(false);

		FashionElemShow* ptrFashionElemShow = new FashionElemShow();
		ptrFashionElemShow->seteBG(elemBg);
		ptrFashionElemShow->seteITEMIMG(itemImg);
		ptrFashionElemShow->seteNAME(itemName);
		ptrFashionElemShow->seteEFFECT(itemEffect);
		ptrFashionElemShow->seteLock(lockSp);
		ptrFashionElemShow->seteSELECTED(selectedSp);
		ptrFashionElemShow->seteDataIndex(-1);

		mShowElem[i] = ptrFashionElemShow;
	}
}

void EquipFashionLayer::GetDataElem()
{
	int iListIndex = 0;
	map<int, int> mGoodsId;
	for (int i = 0; i < history->goods->size(); i++)
	{
		HistoryGoodsPtr ptrHistoryGoods = history->goods->elementAt(i);
		int iGoodsType = ptrHistoryGoods->GetGoodsType();

		if ((iGoodsType == GOOD_TYPE_FASHING || iGoodsType == GOOD_TYPE_WAP_FASHING)&&trData->m_mGoodsItem[ptrHistoryGoods->good_id]->mHid.count(curActor->actor_id+1))
		{
			FashionElemData* ptrFEData = new FashionElemData();
			ptrFEData->seteGOODSID(ptrHistoryGoods->good_id);
			ptrFEData->seteLOCK(0);
			ptrFEData->seteSELECTED(0);
			ptrFEData->seteGOODSITEM(ptrHistoryGoods);
			mDataList[iListIndex++] = ptrFEData;
			mGoodsId[ptrHistoryGoods->good_id] = 1;
		}
	}

	HistoryGoodsPtr ptrEquipGoods = NULL;
	if (curActor->fashion.isNormalGood())
	{
		ptrEquipGoods = &curActor->fashion;
		FashionElemData* ptrFEData = new FashionElemData();
		ptrFEData->seteGOODSID(ptrEquipGoods->good_id);
		ptrFEData->seteLOCK(0);
		ptrFEData->seteSELECTED(1);
		ptrFEData->seteGOODSITEM(ptrEquipGoods);
		mDataList[iListIndex++] = ptrFEData;
		mGoodsId[ptrEquipGoods->good_id] = 1;
	}
	if (curActor->wap_fashion.isNormalGood())
	{
		ptrEquipGoods = &curActor->wap_fashion;
		FashionElemData* ptrFEData = new FashionElemData();
		ptrFEData->seteGOODSID(ptrEquipGoods->good_id);
		ptrFEData->seteLOCK(0);
		ptrFEData->seteSELECTED(1);
		ptrFEData->seteGOODSITEM(ptrEquipGoods);
		mDataList[iListIndex++] = ptrFEData;
		mGoodsId[ptrEquipGoods->good_id] = 1;
	}

	for(map<int,int>::iterator eIt = trData->mFashionList.begin(); eIt != trData->mFashionList.end();eIt++)
	{
		GoodsItem*  ptrGItem = trData->m_mGoodsItem[eIt->first];
		if (ptrGItem->mHid.count(curActor->actor_id + 1)&&!mGoodsId.count(ptrGItem->iGoodsId))
		{
			FashionElemData* ptrFEData = new FashionElemData();
			ptrFEData->seteGOODSID(ptrGItem->iGoodsId);
			ptrFEData->seteLOCK(1);
			ptrFEData->seteSELECTED(0);
			ptrFEData->seteGOODSITEM(NULL);
			mDataList[iListIndex++] = ptrFEData;
		}
	}


	FashionElemData* ptrFEData = NULL;
	for (int i = 0; i < iListIndex; i++)
	{
		for (int j = iListIndex - 1; j > i; j--)
		{
			if (mDataList[j]->geteGOODSID() < mDataList[j - 1]->geteGOODSID())
			{
				ptrFEData = mDataList[j];
				mDataList[j] = mDataList[j - 1];
				mDataList[j - 1] = ptrFEData;
			}
		}
	}

	m_iMaxPage = ceil(iListIndex/4.0);
}

void EquipFashionLayer::UpdataElemShow()
{
	mCurDotSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt3.png"));
	mCurDotSp = mElemDot[m_iCurPage];
	mCurDotSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt2.png"));

	int iMaxGoodsNum = mDataList.size();
	int iDataIndex = 0;
	for (int i = 0; i < 4; i++)
	{
		iDataIndex = i + m_iCurPage * 4;

		if (iDataIndex < iMaxGoodsNum)
		{
			mShowElem[i]->geteBG()->setVisible(true);

			if (mDataList[iDataIndex]->geteSELECTED())
			{
				mShowElem[i]->geteBG()->setTexture(ReadSpriteTexture("new_ui/hero/hero_dress_bg2.png"));
				mShowElem[i]->geteSELECTED()->setVisible(true);
			}
			else
			{
				mShowElem[i]->geteBG()->setTexture(ReadSpriteTexture("new_ui/hero/hero_dress_bg3.png"));
				mShowElem[i]->geteSELECTED()->setVisible(false);
			}

			if (mDataList[iDataIndex]->geteLOCK())
			{
				addSpriteGray(mShowElem[i]->geteBG());
				mShowElem[i]->geteLock()->setVisible(true);
				mShowElem[i]->geteNAME()->setColor(Color3B(102, 102, 102));
				mShowElem[i]->geteEFFECT()->setColor(Color3B::WHITE);
			}
			else
			{
				removeSpriteGray(mShowElem[i]->geteBG());
				mShowElem[i]->geteLock()->setVisible(false);
				mShowElem[i]->geteNAME()->setColor(Color3B(102, 0, 0));
				mShowElem[i]->geteEFFECT()->setColor(Color3B(102, 51, 0));
			}

			GoodsItem* ptrGItem = trData->m_mGoodsItem[mDataList[iDataIndex]->geteGOODSID()];

			string itemNameS = ptrGItem->strName;

			mShowElem[i]->geteNAME()->setString(itemNameS.c_str());

			mShowElem[i]->geteEFFECT()->setString(ptrGItem->GetSpecialEffectInfo(1));

			string imgPath = history->GetFashionGoodsPath(ptrGItem->iGoodsId,curActor);

			mShowElem[i]->geteITEMIMG()->setTexture(GetSpriteTexture(imgPath.c_str()));

			mShowElem[i]->seteDataIndex(iDataIndex);
		}
		else
		{
			mShowElem[i]->geteBG()->setVisible(false);
			mShowElem[i]->seteDataIndex(-1);
		}
	}
}

void EquipFashionLayer::ClickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_iMaxPage <= 1)
		return;

	if (m_iCurPage <= 0)
	{
		m_iCurPage = m_iMaxPage - 1;
	}
	else
	{
		m_iCurPage--;
	}
	UpdataElemShow();
}

void EquipFashionLayer::ClickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_iMaxPage <= 1)
		return;

	if (m_iCurPage < m_iMaxPage - 1)
	{
		m_iCurPage++;
	}
	else
	{
		m_iCurPage = 0;
	}
	UpdataElemShow();
}

void EquipFashionLayer::ClickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_bEnableUpDataFatherShow&&mDelegate)
	{
		mDelegate->updateParentShow();
	}
	HongDian::getInstence()->autoUpadteInfo(curActor->actor_id,ANNIU_HONGDIAN);

	this->removeFromParent();
}

void EquipFashionLayer::EquipFashion(int iDataIndex)
{
	m_iClickDataIndex = iDataIndex;
	FashionElemData* ptrFElemData = mDataList[iDataIndex];

	if (ptrFElemData->geteLOCK() >= 1)
	{
		m_bEnableClick = true;
		return;
	}

	int iZid = 0, iXid = 0;
	if (ptrFElemData->geteSELECTED() < 1)
	{
		HistoryGoodsPtr ptrGoods = ptrFElemData->geteGOODSITEM();
		iZid = ptrGoods->getNetID();

		if (!history->ActorEnableEquipGoods(ptrGoods, curActor->actor_id + 1))
			return;

		if (ptrGoods->GetGoodsType() == GOOD_TYPE_FASHING&&curActor->fashion.isNormalGood())
		{
			iXid = curActor->fashion.getNetID();
		}
		else if (ptrGoods->GetGoodsType() == GOOD_TYPE_WAP_FASHING&&curActor->wap_fashion.isNormalGood())
		{
			iXid = curActor->wap_fashion.getNetID();
		}
	}
	else
	{
		iXid = ptrFElemData->geteGOODSITEM()->getNetID();
	}
	SendNetDate(iZid, iXid);
}

bool EquipFashionLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_bEnableClick)
	{
		for (size_t i = 0; i < mShowElem.size(); i++)
		{
			Rect BgBox = mShowElem[i]->geteBG()->getBoundingBox();

			if (mShowElem[i]->geteDataIndex() > -1 && Rect(0, 0, BgBox.size.width, BgBox.size.height).containsPoint(mShowElem[i]->geteBG()->convertToNodeSpace(touch->getLocation())))
			{
				m_bEnableClick = false;
				EquipFashion(mShowElem[i]->geteDataIndex());
				return true;
			}
		}
	}

	return true;
}

void EquipFashionLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void EquipFashionLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

EquipFashionLayer::EquipFashionLayer()
{

}

EquipFashionLayer::~EquipFashionLayer()
{
	for (size_t i = 0; i < mShowElem.size(); i++)
	{
		delete mShowElem[i];
	}
	mShowElem.clear();

	for (size_t i = 0; i < mDataList.size(); i++)
	{
		delete mDataList[i];
	}
	mDataList.clear();
}


void EquipFashionLayer::SendNetDate(int iZid, int iXid)
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("zid=%d&xid=%d&hid=%d&gameToken=%s", iZid, iXid, curActor->actor_id + 1, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"zid\":%d,\"xid\":%d,\"hid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iZid, iXid, curActor->actor_id + 1, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("equipment",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(EquipFashionLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void EquipFashionLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
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
		if (value["result"]["isTrue"].isInt() && value["result"]["isTrue"].asInt() == 1)
		{
			ActorSetEquip();
		}
	}
}

void EquipFashionLayer::ActorSetEquip()
{
	HistoryGoodsPtr EquipGoods = NULL;
	HistoryGoodsPtr UploadGoods = NULL;

	FashionElemData* ptrFElemData = mDataList[m_iClickDataIndex];
	HistoryGoodsPtr ptrGood = ptrFElemData->geteGOODSITEM();
	HistoryGoods tempGood(ptrGood);

	if (ptrFElemData->geteSELECTED() < 1)
	{
		if (ptrGood->GetGoodsType() == GOOD_TYPE_FASHING)
		{
			if (curActor->fashion.isNormalGood())
			{
				HistoryGoodsPtr UpGoods = new HistoryGoods(&curActor->fashion);
				history->goods->addElement(UpGoods);
				curActor->fashion.good_id = -1;
				UploadGoods = UpGoods;
			}

			curActor->fashion.setGood(&tempGood);
			history->goods->removeElement(ptrGood);
			curActor->UpdataGoodEffect();
			EquipGoods = &curActor->fashion;
		}
		else if (ptrGood->GetGoodsType() == GOOD_TYPE_WAP_FASHING)
		{
			if (curActor->wap_fashion.isNormalGood())
			{
				HistoryGoodsPtr UpGoods = new HistoryGoods(&curActor->wap_fashion);
				history->goods->addElement(UpGoods);
				curActor->wap_fashion.good_id = -1;
				UploadGoods = UpGoods;
			}

			curActor->wap_fashion.setGood(&tempGood);
			history->goods->removeElement(ptrGood);
			curActor->UpdataGoodEffect();
			EquipGoods = &curActor->wap_fashion;
		}
		__String* str = __String::createWithFormat(LL("zbzb"), tempGood.GetGoodsName());
		ITools::getInstance()->createGlobalPrompt(str->getCString(), true);
	}
	else
	{
		HistoryGoodsPtr UpGoods = new HistoryGoods(ptrGood);
		history->goods->addElement(UpGoods);
		ptrGood->good_id = -1;
		curActor->UpdataGoodEffect();
		UploadGoods = UpGoods;

		__String* str = __String::createWithFormat(LL("xxzb"), UpGoods->GetGoodsName());
		ITools::getInstance()->createGlobalPrompt(str->getCString(), false);
	}

	EquipmentUpdataDataList(EquipGoods,UploadGoods);

	UpdataLeftShow();
	UpdataElemShow();
	m_bEnableUpDataFatherShow = true;
	m_bEnableClick = true;
}

void EquipFashionLayer::EquipmentUpdataDataList(HistoryGoodsPtr EquipGoods, HistoryGoodsPtr UploadGoods)
{

	FashionElemData* ptrFElemData = mDataList[m_iClickDataIndex];

	if (ptrFElemData->geteSELECTED() < 1)
	{
		if (UploadGoods)
		{
			for (map<int, FashionElemData*>::iterator eIt = mDataList.begin(); eIt != mDataList.end(); eIt++)
			{
				if (UploadGoods->good_id == eIt->second->geteGOODSID() && eIt->second->geteSELECTED())
				{
					eIt->second->seteSELECTED(0);
					eIt->second->seteGOODSITEM(UploadGoods);
					break;
				}
			}
		}

		if (EquipGoods)
		{
			ptrFElemData->seteSELECTED(1);
			ptrFElemData->seteGOODSITEM(EquipGoods);
		}
	}
	else
	{
		ptrFElemData->seteSELECTED(0);
		ptrFElemData->seteGOODSITEM(UploadGoods);
	}
}

void EquipFashionLayer::UpdataLeftShow()
{
	__String* f_path = NULL;
	__String* img_path = NULL;
	Texture2D* ptrTexture = NULL;
	//时装
	if (curActor->mFashion[0])
	{
		f_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[3], curActor->mFashion[4]);
		img_path = __String::createWithFormat("img/Nunits/%d_%d_mov.png", curActor->mFashion[1], curActor->mFashion[2]);

		if (actor_fashion != NULL)
		{
			actor_fashion->setVisible(true);
			ptrTexture = GetSpriteTexture(f_path->getCString());
			int fashion_width = ptrTexture->getContentSize().width;
			actor_fashion->setTexture(ptrTexture);
			actor_fashion->setTextureRect(Rect(0, fashion_width * 6, fashion_width, fashion_width));
		}
		else
		{
			actor_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
			int fashion_width = actor_fashion->getContentSize().width;
			actor_fashion->setTextureRect(Rect(0, fashion_width * 6, fashion_width, fashion_width));
			actor_fashion->setPosition(Vec2(img_bg->getContentSize().width / 2 - 2, img_bg->getContentSize().height / 2 + 24));
			img_bg->addChild(actor_fashion);
		}

		int iZOrdeer = 0;
		if (trData->net_hero_dress[curActor->mFashion[4]]->mMov[6])
			iZOrdeer = 2;

		actor_fashion->setLocalZOrder(iZOrdeer);
	}
	else
	{
		img_path = __String::create(curActor->getActorSImgPath("mov", 0));
		if (actor_fashion != NULL)
		{
			actor_fashion->setVisible(false);
		}
	}

	ptrTexture = GetSpriteTexture(img_path->getCString());
	int img_width = ptrTexture->getContentSize().width;
	actor_img->setTexture(ptrTexture);
	actor_img->setTextureRect(Rect(0, img_width * 6, img_width, img_width));


	if (curActor->fashion.isNormalGood())
	{
		coatSp->setVisible(true);
		string imgS = history->GetFashionGoodsPath(curActor->fashion.good_id, curActor);
		coatSp->setTexture(GetSpriteTexture(imgS.c_str()));
	}
	else
	{
		coatSp->setVisible(false);
	}

	if (curActor->wap_fashion.isNormalGood())
	{
		wapSp->setVisible(true);
		string imgS = history->GetFashionGoodsPath(curActor->wap_fashion.good_id, curActor);
		wapSp->setTexture(GetSpriteTexture(imgS.c_str()));
	}
	else
	{
		wapSp->setVisible(false);
	}

	string goodsEffect = "";
	map<int, int> mFashionSeid = curActor->GetFashionSeid();
	SeidPtr ptrSeid = NULL;
	for (size_t i = 0; i < mFashionSeid.size(); i++)
	{
		ptrSeid = trData->m_mSeid[mFashionSeid[i]];
		goodsEffect += ptrSeid->Info;

		if (i < mFashionSeid.size() - 1)
			goodsEffect += "\n";
	}
	fashionEffect->setString(goodsEffect.c_str());
}