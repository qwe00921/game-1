//
//  PubDrinkLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/01.
//
//

#include "PubDrinkLayer.h"
#include "PubBoxLayer.h"
#include "PopLayer.h"
#include "GetNetDateTool.h"
#include "GuideLayer.h"

enum 
{
	PUB_INIT = 0,
	PUB_REFRESH = 1,
	PUB_BOX_INIT = 2,
	PUB_BOX_BUY = 3
};

enum
{
	PUB_BUY_NONE = 0,
	PUB_BUY_GOLD,
	PUB_BUY_JADE
};

bool PubDrinkLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	m_guideStep = 0;
	m_iRefresh = 0;
	m_iClickHero = -1;
	m_iMaxRef = trData->m_mPubRefPrice.size();
	m_bClickBox = false;
	m_iBuyType = -1;
	m_iBoxPrice = 0;

	initControl();
	m_layTitleBar = TitleBarLayer::create();
	this->addChild(m_layTitleBar);
	onHttpRequest(PUB_INIT);

	return true;
}

void PubDrinkLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//背景
	m_spPubBG = ReadSpriteName("new_ui/img_global/pub_bg1.png");
	m_spPubBG->setPosition(point);
	this->addChild(m_spPubBG);

	//刷新时间
	m_labRefTime = LabelSystemFont(__String::createWithFormat(LL("nextrefreshtime"), 0)->getCString(), 16);
	m_labRefTime->setColor(Color3B::YELLOW);
	m_labRefTime->setPosition(point);
	this->addChild(m_labRefTime);
	
	//魂匣
	m_armBox = Armature::create("icon_eff");
	m_armBox->getAnimation()->play("box_001");
	m_armBox->setPosition(point + Vec2(0, 82));
	this->addChild(m_armBox);

	//功能按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//敲花鼓
	Sprite* spRefreshN = ReadSpriteName("new_ui/img_global/pub_ban1.png");
	Sprite* spRefreshS = ReadSpriteName("new_ui/img_global/pub_ban1.png");
	spRefreshS->setScale(fScale);
	MenuItemSprite* btnRefresh = MenuItemSprite::create(spRefreshN, spRefreshS, CC_CALLBACK_1(PubDrinkLayer::callBackRefresh, this));
	spRefreshS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRefreshS->setPosition(Vec2(spRefreshN->getContentSize().width / 2, spRefreshN->getContentSize().height / 2));
	btnRefresh->setPosition(point + Vec2(0, -130));
	ptrMenu->addChild(btnRefresh);

	//返回
	Sprite* spCloseN = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/battle/flippage3.png");
	spCloseS->setScale(fScale);
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PubDrinkLayer::callBackClose, this));
	spCloseS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spCloseS->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
	btnClose->setPosition(size.width - 32, size.height - 32);
	ptrMenu->addChild(btnClose);

	//Touch屏蔽层
	MenuItem* btnNull = MenuItem::create(CC_CALLBACK_1(PubDrinkLayer::callBackNull, this));
	btnNull->setContentSize(size);
	btnNull->setPosition(point);
	m_menuNull = Menu::create(btnNull, NULL);
	m_menuNull->setPosition(Vec2(0, 0));
	m_menuNull->setVisible(false);
	this->addChild(m_menuNull, 99);
}

bool PubDrinkLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (m_armBox->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())) && m_armBox->isVisible())
	{
		m_armBox->setScale(0.9f);
		m_bClickBox = true;
	}
	else
	{
		for(UInt i = 0; i < m_mSpHero.size(); i++)
		{
			Rect rectHero = m_mSpHero[i]->getBoundingBox();
			if(m_mSpHero[i]->isVisible() && rectHero.containsPoint(m_spPubBG->convertToNodeSpace(touch->getLocation())))
			{
				m_mSpHero[i]->setScale(0.9f);
				m_iClickHero = i;
				break;
			}
		}
	}

	return true;
}

void PubDrinkLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (m_bClickBox && !m_armBox->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_armBox->setScale(1.0f);
		m_bClickBox = false;
	}
	else if(m_iClickHero >= 0 && !m_mSpHero[m_iClickHero]->getBoundingBox().containsPoint(m_spPubBG->convertToNodeSpace(touch->getLocation())))
	{
		m_mSpHero[m_iClickHero]->setScale(1.0f);
		m_iClickHero = -1;
	}
}

void PubDrinkLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bClickBox)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_armBox->setScale(1.0f);
		m_bClickBox = false;

		if(history->m_iVIP)
		{
			onHttpRequest(PUB_BOX_INIT);
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("huangzukaifang"), false);
		}
	}
	else if(m_iClickHero >= 0)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_mSpHero[m_iClickHero]->setScale(1.0f);
		auto popLayer = PubPopupLayer::create(m_mDrinker[m_iClickHero], this);
		this->addChild(popLayer);
		m_iClickHero = -1;
	}
}

PubDrinkLayer::PubDrinkLayer()
{

}

PubDrinkLayer::~PubDrinkLayer()
{
	for(UInt i = 0; i < m_mDrinker.size(); i++)
	{
		SAFE_DELETE(m_mDrinker[i]);
	}
}

void PubDrinkLayer::updateShow(int index)
{
	if (index >= 0 && index < (int)m_mSpHero.size())
	{
		bool bVisible = m_mDrinker[index]->getIsSell() ? false : true;
		m_mSpHero[index]->setVisible(bVisible);
	}

	m_layTitleBar->updateShow(index);
	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(index);
	}
}

void PubDrinkLayer::callBackConfirm()
{
	int iPrice = 0;
	if (m_iBuyType == PUB_REFRESH)
	{
		int iRefTime = (m_iRefresh + 1) < m_iMaxRef ? (m_iRefresh + 1) : m_iMaxRef;
		iPrice = trData->m_mPubRefPrice[iRefTime];
	}
	else if (m_iBuyType == PUB_BOX_BUY)
	{
		iPrice = m_iBoxPrice;
	}

	if(history->m_iJade < iPrice)
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
		return;
	}

	onHttpRequest(m_iBuyType);
	history->updateGuide(ZHUJIEMIAN6_GUIDE);
}

void PubDrinkLayer::callBackRefresh(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iRefTime = (m_iRefresh + 1) < m_iMaxRef ? (m_iRefresh + 1) : m_iMaxRef;
	int iPrice = trData->m_mPubRefPrice[iRefTime];
	m_iBuyType = PUB_REFRESH;
	Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_PUB_REF, this, iPrice), 98);
}

void PubDrinkLayer::callBackClose(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void PubDrinkLayer::callBackNull(Ref *sender)
{
	//屏蔽点击事件，不做任何处理
}

void PubDrinkLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case PUB_INIT:
	{
		strURL = "getPubData";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_PUB_INIT";
	}
		break;
	case PUB_REFRESH:
	{
		strURL = "refreshPubData";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_PUB_REFRESH";
	}
		break;
	case PUB_BOX_INIT:
	{
		strURL = "getSoulBox";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "PUB_BOX_INIT";
	}
		break;
	case PUB_BOX_BUY:
	{
		strURL = "buySoulBox";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "PUB_BOX_BUY";
	}
	   break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(PubDrinkLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void PubDrinkLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	string responseData = temp;
	CCLOG("%s...", responseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData, value) && value.size() > 0)
	{
		if (strTag == "PUB_BOX_INIT")
		{
			onBoxInitSucceed(value["result"]);
		}
		else if (strTag == "PUB_BOX_BUY")
		{
			onBoxBuySucceed(value["result"]);
		}
		else
		{
			parseJson(value["result"]);
			updateHeroAction();
		}
	}
}

bool PubDrinkLayer::onBoxInitSucceed(Json::Value valRes)
{
	m_iBuyType = PUB_BOX_BUY;
	SoulBoxInfo* ptrSoul = new SoulBoxInfo();
	if (ptrSoul->loadJson(valRes))
	{
		m_iBoxPrice = ptrSoul->getPrice();
		this->addChild(PubBoxLayer::create(ptrSoul, this));
	}
	else
	{
		SAFE_DELETE(ptrSoul);
		ITools::getInstance()->createGlobalPrompt(LL("soulboxclosed"), false);
	}
	return true;
}

bool PubDrinkLayer::onBoxBuySucceed(Json::Value valRes)
{
	m_armBox->setVisible(false); //暂时隐藏宝箱
	m_menuNull->setVisible(true); //屏蔽点击事件

	//宝箱动画
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Armature* armOpen = Armature::create("icon_eff");
	armOpen->getAnimation()->play("box_002");
	armOpen->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(armOpen);
	armOpen->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if(type == COMPLETE)
		{
			armature->removeFromParent();
			m_armBox->setVisible(true);

			ParseInt(history->m_iJade, valRes["jade"]);
			Json::Value valGoods = valRes["goods"];
			if(valGoods.isArray())
			{
				map<int, ConsumeElem*> mReward;
				map<int, int> mIndex = getRandMap(valGoods.size());
				for(UInt i = 0; i < valGoods.size(); i++)
				{
					ConsumeElem* ptrConsume = new ConsumeElem();
					ptrConsume->loadJson(valGoods[i]);
					history->packGoodsAdd(ptrConsume->getGoodID(), ptrConsume->getNetID(), ptrConsume->getCount());
					mReward[mIndex[i]] = ptrConsume;
				}
				this->addChild(PubRewardLayer::create(mReward, REWARD_BOX, this, m_iBoxPrice));
			}
			updateShow(-1);
			m_menuNull->setVisible(false); //取消屏蔽点击事件
		}
	});

	return true;
}

bool PubDrinkLayer::parseJson(Json::Value valRes)
{
	Json::Value heroList = valRes["heros"];
	for(UInt i = 0; i < heroList.size(); i++)
	{
		if(m_mDrinker[i] == NULL)
		{
			m_mDrinker[i] = new DrinkerItem();
		}

		string strKey = IntToString(i + 1);
		int iHeroId = heroList[strKey.c_str()]["hid"].asInt();
		int iChipId = heroList[strKey.c_str()]["id"].asInt();
		int iIsSell = heroList[strKey.c_str()]["isBuy"].asInt();
		m_mDrinker[i]->setKeyId(i + 1);
		m_mDrinker[i]->setHeroId(iHeroId);
		m_mDrinker[i]->setChipId(iChipId);
		m_mDrinker[i]->setIsSell(iIsSell);
	}
	m_iRefresh = valRes["refreshCount"].asInt();

	if (valRes["jade"].isInt())
	{
		history->m_iJade = valRes["jade"].asInt();
		updateShow(-1);
	}

	if(valRes["nextRefTime"].isInt())
	{
		int iHour = valRes["nextRefTime"].asInt();
		m_labRefTime->setString(__String::createWithFormat(LL("nextrefreshtime"), iHour)->getCString());
	}

	//随机初始化武将位置
	map<int, PubPosition*> mPubPosition = trData->m_mPubPosition;
	UInt iSize = trData->m_mPubPosition.size() - 4;
	UInt iStart = 5;
	if(history->m_iVIP)
	{
		iSize = trData->m_mPubPosition.size();
		iStart = 1;
	}

	for(UInt i = 0; i < m_mDrinker.size(); i++)
	{
		while(1)
		{
			int iIndex = rand() % iSize + iStart;
			if (heroList.size() == 1)//酒馆引导固定位置领取
			{
				if (history->m_pPlayerInfo->getGuideStatus("_zhujiemian4") == 0|| history->m_pPlayerInfo->getGuideStatus("_zhujiemian4_1")==0) {
					iIndex = 6;
					m_guideStep = 1;
				}
				else {
					if (history->m_pPlayerInfo->getGuideStatus("_zhujiemian6") == 0||history->m_pPlayerInfo->getGuideStatus("_zhujiemian6_1") == 0) {
						iIndex = 6;
						m_guideStep = 2;
					}
				}
				
			}
			if(mPubPosition[iIndex])
			{
				m_mDrinker[i]->setPosIndex(iIndex);
				mPubPosition[iIndex] = NULL;
				break;
			}
		}
	}

	return true;
}

void PubDrinkLayer::updateHeroAction()
{
	for(UInt i = 0; i < m_mDrinker.size(); i++)
	{
		string strPath = "";
		string strName = trData->actors[m_mDrinker[i]->getHeroId() - 1].getActorName();
		if(m_mDrinker[i]->getHeroId() == 1)
		{
			HeroElem* ptrHero = history->m_pPlayerInfo->getMainHero();
			strPath = StringUtils::format("new_ui/pub_icon/%d_%d.png", m_mDrinker[i]->getHeroId(), ptrHero->getGender());
		}
		else
		{
			strPath = StringUtils::format("new_ui/pub_icon/%d.png", m_mDrinker[i]->getHeroId());
		}

		if (m_mSpHero.count(i))
		{
			m_mSpHero[i]->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
			m_mLabName[i]->setString(strName);
		}
		else
		{
			//头像
			m_mSpHero[i] = ReadSpriteName(strPath.c_str());
			m_spPubBG->addChild(m_mSpHero[i]);

			//名称
			m_mLabName[i] = LabelSystemFont(strName, 14);
			m_mLabName[i]->setPosition(Vec2(m_mSpHero[i]->getContentSize().width / 2, 12));
			m_mLabName[i]->enableOutline(Color4B(0, 72, 72, 255), 1);
			m_mSpHero[i]->addChild(m_mLabName[i]);
		}

		int iPointX = trData->m_mPubPosition[m_mDrinker[i]->getPosIndex()]->iPointX;
		int iPointY = m_spPubBG->getContentSize().height - trData->m_mPubPosition[m_mDrinker[i]->getPosIndex()]->iPointY;
		m_mSpHero[i]->setPosition(Vec2(iPointX, iPointY));

		bool bFlipped = m_mDrinker[i]->getPosIndex() % 2 ? false : true;
		m_mSpHero[i]->setFlippedX(bFlipped);

		bool bVisible = m_mDrinker[i]->getIsSell() ? false : true;
		m_mSpHero[i]->setVisible(bVisible);
	}
}


map<int, int> PubDrinkLayer::getRandMap(int iSize)
{
	map<int, int> mTemp;
	map<int, int> mReturn;
	for (int i = 0; i < iSize; i++)
	{
		while(true)
		{
			int iRand = rand() % iSize;
			if (!mTemp[iRand])
			{
				mTemp[iRand] = 1;
				mReturn[i] = iRand;
				break;
			}
		}
	}
	return mReturn;
}

PubPopupLayer::PubPopupLayer()
{

}

PubPopupLayer::~PubPopupLayer()
{

}

PubPopupLayer* PubPopupLayer::create(DrinkerItem* ptrDrinker, EventDelegate* ptrDelegate)
{
	PubPopupLayer* pRet = new PubPopupLayer();
	if(pRet&&pRet->init(ptrDrinker, ptrDelegate))
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

bool PubPopupLayer::init(DrinkerItem* ptrDrinker, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrDrinker = ptrDrinker;
	m_pDelegate = ptrDelegate;
	m_iBuyType = PUB_BUY_NONE;

	initControl();

	return true;
}

bool PubPopupLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}

void PubPopupLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void PubPopupLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void PubPopupLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/pub_bg2.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//邀请喝酒
	__String* strName = __String::createWithFormat(LL("pubdrinkname"), trData->actors[m_ptrDrinker->getHeroId() - 1].getActorName());
	Label* labName = LabelSystemFont(strName->getCString(), 18);
	labName->setColor(Color3B(102, 0, 0));
	labName->setPosition(point + Vec2(0, 63));
	this->addChild(labName);

	//买美酒
	Label* labNotice1 = LabelSystemFont(LL("pubdrinknotice1"), 16);
	labNotice1->setPosition(point + Vec2(0, 25));
	this->addChild(labNotice1);

	//买杜康
	Label* labNotice2 = LabelSystemFont(LL("pubdrinknotice2"), 16);
	labNotice2->setPosition(point + Vec2(0, 0));
	this->addChild(labNotice2);

	//金币ICON
	Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGoldIcon->setPosition(point + Vec2(-122, -58+3));
	this->addChild(spGoldIcon);

	//金币值
	Label* labGold = LabelSystemFont(IntToString(trData->m_mPubInvite[1]->iGold), 16);
	labGold->setColor(Color3B::YELLOW);
	labGold->setPosition(point + Vec2(-78, -58+3));
	this->addChild(labGold);

	//勾玉ICON
	Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeIcon->setPosition(point + Vec2(54, -58+3));
	this->addChild(spJadeIcon);

	//勾玉值
	Label* labJade = LabelSystemFont(IntToString(trData->m_mPubInvite[1]->iJade), 16);
	labJade->setColor(Color3B::GREEN);
	labJade->setPosition(point + Vec2(97, -58+3));
	this->addChild(labJade);

	//按钮菜单
	m_pMenu = Menu::create();
	m_pMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PubPopupLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(200, 125));
	m_pMenu->addChild(btnClose);

	//金币购买
	Sprite* spGoldN = ReadSpriteName("new_ui/img_global/pub_ban2_1.png");
	Sprite* spGoldS = ReadSpriteName("new_ui/img_global/pub_ban2_2.png");
	MenuItemSprite* btnGoldBuy = MenuItemSprite::create(spGoldN, spGoldS, CC_CALLBACK_1(PubPopupLayer::callBackBuy, this));
	btnGoldBuy->setTag(PUB_BUY_GOLD);
	btnGoldBuy->setPosition(point + Vec2(-90, -100));
	m_pMenu->addChild(btnGoldBuy);

	//勾玉购买
	Sprite* spJadeN = ReadSpriteName("new_ui/img_global/pub_ban3_1.png");
	Sprite* spJadeS = ReadSpriteName("new_ui/img_global/pub_ban3_2.png");
	MenuItemSprite* btnJadeBuy = MenuItemSprite::create(spJadeN, spJadeS, CC_CALLBACK_1(PubPopupLayer::callBackBuy, this));
	btnJadeBuy->setTag(PUB_BUY_JADE);
	btnJadeBuy->setPosition(point + Vec2(90, -100));
	m_pMenu->addChild(btnJadeBuy);
}

void PubPopupLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void PubPopupLayer::callBackBuy(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* ptrMenuItem = (MenuItemSprite*)sender;
	m_iBuyType = ptrMenuItem->getTag();
	buyHttpRequest();
}

void PubPopupLayer::buyHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("payType=%d&key=%d&gameToken=%s", m_iBuyType, m_ptrDrinker->getKeyId(), strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"payType\":%d,\"key\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iBuyType, m_ptrDrinker->getKeyId(), strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("drink",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PubPopupLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void PubPopupLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		onBuySucceed(value["result"]);
	}
}

void PubPopupLayer::onBuySucceed(Json::Value valRes)
{
	if(valRes["gold"].isInt())
	{
		history->money = valRes["gold"].asInt();
	}

	if(valRes["jade"].isInt())
	{
		history->m_iJade = valRes["jade"].asInt();
	}

	m_ptrDrinker->setIsSell(1);
	m_pDelegate->updateShow(m_ptrDrinker->getKeyId() - 1);

	Json::Value valGoods = valRes["goods"];
	if(valGoods.isArray())
	{
		map<int, ConsumeElem*> mReward;
		for(UInt i = 0; i < valGoods.size(); i++)
		{
			ConsumeElem* ptrConsume = new ConsumeElem();
			ptrConsume->loadJson(valGoods[i]);
			history->packGoodsAdd(ptrConsume->getGoodID(), ptrConsume->getNetID(), ptrConsume->getCount());
			mReward[i] = ptrConsume;
		}
		this->getParent()->addChild(PubRewardLayer::create(mReward, REWARD_DRINK, this));
	}
	this->removeFromParent();
	history->updateGuide(HECHENG_GUIDE);
	history->updateGuide(ZHUJIEMIAN6_GUIDE);
}