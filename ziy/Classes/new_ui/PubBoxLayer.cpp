//
//  PubBoxLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/11/01.
//
//

#include "PubBoxLayer.h"
#include "GoodsSprite.h"
#include "HeroSprite.h"

#define REWARD_LINE_NUM 3

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SoulBoxInfo::SoulBoxInfo()
{
	m_iHotId = 0;
	m_iPrice = 0;
	m_strTime = "";
}

SoulBoxInfo::~SoulBoxInfo()
{

}

bool SoulBoxInfo::loadJson(Json::Value valJson)
{
	Json::Value valSoul = valJson["soulBox"];
	if (!valSoul.size())
	{
		return false;
	}

	ParseInt(m_iHotId, valSoul["mainHid"]);
	ParseInt(m_iPrice, valSoul["price"]);
	ParseString(m_strTime, valSoul["timeStr"]);

	Json::Value valArray = valSoul["viceHotspot"];
	if (valArray.isArray())
	{
		for(UInt i = 0; i < valArray.size(); i++)
		{
			ParseInt(m_mGoodsId[i], valArray[i]["goodID"]);
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PubBoxLayer::PubBoxLayer()
{

}

PubBoxLayer::~PubBoxLayer()
{
	SAFE_DELETE(m_ptrSoul);
}

PubBoxLayer* PubBoxLayer::create(SoulBoxInfo* ptrSoul, EventDelegate* ptrDelegate)
{
	PubBoxLayer* pRet = new PubBoxLayer();
	if(pRet&&pRet->init(ptrSoul, ptrDelegate))
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

bool PubBoxLayer::init(SoulBoxInfo* ptrSoul, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrSoul = ptrSoul;
	m_pDelegate = ptrDelegate;

	initControl();
	return true;
}

void PubBoxLayer::initControl()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	auto spBoxBG = ReadSpriteName("new_ui/img_global/hx_box_bg1.png");
	spBoxBG->setPosition(point);
	this->addChild(spBoxBG);

	//热门武将
	auto spHotHero = HeroSprite::create(m_ptrSoul->getHotID(), 2, 1.0f);
	spHotHero->setPosition(point + Vec2(-129, 22));
	this->addChild(spHotHero);

	//今日热门
	map<int, int> mGoodsId = m_ptrSoul->m_mGoodsId;
	for (int i = 0; i < (int)mGoodsId.size(); i++)
	{
		auto spHero = GoodsSprite::create(mGoodsId[i], 1, 1.0f);
		spHero->setPosition(point + Vec2(-28 + 80 * i, 22));
		this->addChild(spHero);
	}

	//开放时间
	auto labTime = LabelSystemFont(m_ptrSoul->getContent(), 16);
	labTime->setPosition(point + Vec2(0, -64));
	labTime->setColor(Color3B(102,0,0));
	this->addChild(labTime);
	
	//按钮菜单
	auto ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//购买
	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spJadeN = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeN->setPosition(Vec2(30, 18));
	spBuyN->addChild(spJadeN);
	Label* labBuyN = LabelSystemFont(StringUtils::format("%d", m_ptrSoul->getPrice()), 18);
	labBuyN->setPosition(Vec2(65, 23));
	labBuyN->enableOutline(Color4B(102, 50, 0, 255), 1);
	spBuyN->addChild(labBuyN);
	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spJadeS = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJadeS->setPosition(Vec2(30, 18));
	spBuyS->addChild(spJadeS);
	Label* labBuyS = LabelSystemFont(StringUtils::format("%d", m_ptrSoul->getPrice()), 18);
	labBuyS->setPosition(Vec2(65, 23));
	labBuyS->enableOutline(Color4B(102, 50, 0, 255), 1);
	spBuyS->addChild(labBuyS);
	spBuyS->setScale(0.9f);
	auto btnBuy = MenuItemSprite::create(spBuyN, spBuyS, CC_CALLBACK_1(PubBoxLayer::callBackBuy, this));
	spBuyS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spBuyS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
	btnBuy->setPosition(point + Vec2(0, -106));
	ptrMenu->addChild(btnBuy);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PubBoxLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(202, 127+10));
	ptrMenu->addChild(btnClose);
}

void PubBoxLayer::callBackBuy(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_pDelegate->callBackConfirm();
	this->removeFromParent();
}

void PubBoxLayer::callBackClose(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PubRewardLayer::PubRewardLayer()
{

}

PubRewardLayer::~PubRewardLayer()
{
	for(UInt i = 0; i < m_mReward.size(); i++)
	{
		SAFE_DELETE(m_mReward[i]);
	}
}

bool PubRewardLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void PubRewardLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void PubRewardLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

PubRewardLayer* PubRewardLayer::create(map<int, ConsumeElem*> mReward, int iType, EventDelegate* ptrDelegate, int iPrice /*= 0*/)
{
	PubRewardLayer* pRet = new PubRewardLayer();
	if(pRet&&pRet->init(mReward, iType, ptrDelegate, iPrice))
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

bool PubRewardLayer::init(map<int, ConsumeElem*> mReward, int iType, EventDelegate* ptrDelegate, int iPrice)
{
	if(!Layer::init())
	{
		return false;
	}

	m_mReward = mReward;
	m_iType = iType;
	m_pDelegate = ptrDelegate;
	m_iPrice = iPrice;

	initControl();
	return true;
}

void PubRewardLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	int iOffsetX = m_iType == REWARD_DRINK ? 0 : 90;
	int iOffsetY = m_iType == REWARD_DRINK ? 35 : 88;

	for(int i = 0; i < (int)m_mReward.size(); i++)
	{
		Vec2 vecPos = point + Vec2(-140 + 140 * (i % REWARD_LINE_NUM), iOffsetY - 135 * (i / REWARD_LINE_NUM));

		HistoryGoods hisGoods(m_mReward[i]->getGoodID());
		int iCount = m_mReward[i]->getCount();

		int iPlayIndex = -1;
		if(hisGoods.isPiece()) //碎片
		{
			iPlayIndex = 3;
		}
		else if(hisGoods.isSoul()) //武魂
		{
			iPlayIndex = 4;
		}

		if(iPlayIndex >= 0)
		{
			Armature* armLight = Armature::create("icon_eff");
			armLight->getAnimation()->play(StringUtils::format("hero_q%d", iPlayIndex));
			armLight->setScale(0.8f);
			armLight->setPosition(vecPos);
			this->addChild(armLight);
		}

		auto spGoods = GoodsSprite::create(m_mReward[i]->getGoodID(), iCount, 1.0f);
		spGoods->setPosition(vecPos);
		this->addChild(spGoods);
	}

	//按钮菜单
	auto ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	if (m_iType == REWARD_BOX)
	{
		//勾玉背景
		auto spJadeBG = ReadSpriteName("new_ui/img_global/hx_box_bg2.png");
		spJadeBG->setPosition(point + Vec2(-iOffsetX, -112));
		this->addChild(spJadeBG);

		//勾玉ICON
		Sprite* spJade = ReadSpriteName("new_ui/img_global/icon_gy.png");
		spJade->setPosition(Vec2(32, 14));
		spJadeBG->addChild(spJade);

		//勾玉数目
		Label* labJade = LabelSystemFont(StringUtils::format("%d", m_iPrice), 16);
		labJade->setPosition(Vec2(72, 14));
		labJade->setColor(Color3B::GREEN);
		labJade->enableOutline(Color4B::BLACK, 1);
		spJadeBG->addChild(labJade);

		//再来一次
		Sprite* spAgainN = ReadSpriteName("new_ui/img_global/global_ban1.png");
		Sprite* spAgainTextN = ReadSpriteName("new_ui/img_text/imgtxt5_1.png");
		spAgainTextN->setPosition(Vec2(spAgainN->getContentSize().width / 2, spAgainN->getContentSize().height / 2));
		spAgainN->addChild(spAgainTextN);
		Sprite* spAgainS = ReadSpriteName("new_ui/img_global/global_ban2.png");
		Sprite* spAgainTextS = ReadSpriteName("new_ui/img_text/imgtxt5_2.png");
		spAgainTextS->setPosition(Vec2(spAgainS->getContentSize().width / 2, spAgainS->getContentSize().height / 2));
		spAgainS->addChild(spAgainTextS);
		MenuItemSprite* btnAgain = MenuItemSprite::create(spAgainN, spAgainS, CC_CALLBACK_1(PubRewardLayer::callBackAgain, this));
		btnAgain->setPosition(point + Vec2(-iOffsetX, -150));
		ptrMenu->addChild(btnAgain);
	}

	//确定
	Sprite* spConfirmN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spConTextN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	spConTextN->setPosition(Vec2(spConfirmN->getContentSize().width / 2, spConfirmN->getContentSize().height / 2));
	spConfirmN->addChild(spConTextN);
	Sprite* spConfirmS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spConTextS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	spConTextS->setPosition(Vec2(spConfirmS->getContentSize().width / 2, spConfirmS->getContentSize().height / 2));
	spConfirmS->addChild(spConTextS);
	MenuItemSprite* btnConfirm = MenuItemSprite::create(spConfirmN, spConfirmS, CC_CALLBACK_1(PubRewardLayer::callBackConfirm, this));
	btnConfirm->setPosition(point + Vec2(iOffsetX, -150));
	ptrMenu->addChild(btnConfirm);
}

void PubRewardLayer::callBackAgain(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_pDelegate->callBackConfirm();
	this->removeFromParent();
}

void PubRewardLayer::callBackConfirm(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}