//
//  TitleBarLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/29.
//
//


#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "TitleBarLayer.h"
#include "GoldExchangeLayer.h"
#include "PlayerInfoLayer.h"
#include "BuyJadeLayer.h"

TitleBarLayer::TitleBarLayer()
{

}

TitleBarLayer::~TitleBarLayer()
{

}

void TitleBarLayer::onEnter()
{
	Layer::onEnter();
}

void TitleBarLayer::onExit()
{
	Layer::onExit();
}

bool TitleBarLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return false;
}

void TitleBarLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void TitleBarLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event *unused_event)
{

}

TitleBarLayer* TitleBarLayer::create(int iShowType /*= LAYER_BOTTOM*/)
{
	TitleBarLayer* pRet = new TitleBarLayer();
	if (pRet&&pRet->init(iShowType))
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

bool TitleBarLayer::init(int iShowType)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iShowType = iShowType;
	initControl();
	return true;
}

void TitleBarLayer::initControl()
{
	m_spFashion = Sprite::create();
	m_spWapFashion = Sprite::create();

	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	Vec2 leftTop = Vec2(0, size.height);

	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//左上角角色
	Sprite* spHeroN = ReadSpriteName("new_ui/main/main_bg1.png");
	Sprite* spHeroS = ReadSpriteName("new_ui/main/main_bg1.png");
	MenuItemSprite* btnHero = MenuItemSprite::create(spHeroN, spHeroS, CC_CALLBACK_1(TitleBarLayer::callBackHero, this));
	btnHero->setPosition(leftTop + Vec2(spHeroN->getContentSize().width / 2, -spHeroN->getContentSize().height / 2));
	m_pBtnMenu->addChild(btnHero);

	//时装或形象
	BgAddFashionSImg(1, history->actors[0].mFashion, leftTop + Vec2(- 6,16), 6, this, &m_spFashion, &m_spWapFashion, "mov",2,-2);

	//VIP图标
	__String* strVIP = __String::createWithFormat("new_ui/main/vip%d.png", history->m_iVIP);
	m_spVIP = ReadSpriteName(strVIP->getCString());
	m_spVIP->setPosition(leftTop + Vec2(32, -55));
	this->addChild(m_spVIP);

	//角色名字
	m_labName = LabelSystemFont(history->m_pPlayerInfo->getName().c_str(), 16);
	m_labName->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labName->setPosition(leftTop + Vec2(62, -25));
	this->addChild(m_labName);

	if (m_iShowType == TITLE_SHOW_JB_GY)
	{
		//金币充值
		Sprite* spGoldN = ReadSpriteName("new_ui/main/main_bg2.png");
		Sprite* spGoldS = ReadSpriteName("new_ui/main/main_bg2.png");
		MenuItemSprite* btnGold = MenuItemSprite::create(spGoldN, spGoldS, CC_CALLBACK_1(TitleBarLayer::callBackGold, this));
		btnGold->setTag(16);
		btnGold->setPosition(leftTop + Vec2(235, -25));
		m_pBtnMenu->addChild(btnGold);

		//魂玉充值
		Sprite* spJadeN = ReadSpriteName("new_ui/main/main_bg2.png");
		Sprite* spJadeS = ReadSpriteName("new_ui/main/main_bg2.png");
		MenuItemSprite* btnJade = MenuItemSprite::create(spJadeN, spJadeS, CC_CALLBACK_1(TitleBarLayer::callBackJade, this));
		btnJade->setTag(17);
		btnJade->setPosition(leftTop + Vec2(375, -25));
		m_pBtnMenu->addChild(btnJade);

		//金币
		m_labGold = LabelSystemFont(IntToStringEx(history->money), 16);
		m_labGold->setColor(Color3B::YELLOW);
		m_labGold->setAnchorPoint(Vec2(1.0f, 0.5f));
		m_labGold->setPosition(leftTop + Vec2(271, -25));
		this->addChild(m_labGold);

		Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
		spGoldIcon->setPosition(leftTop + Vec2(285, -25));
		this->addChild(spGoldIcon);

		//魂玉
		m_labJade = LabelSystemFont(IntToString(history->m_iJade), 16);
		m_labJade->setColor(Color3B::GREEN);
		m_labJade->setAnchorPoint(Vec2(1.0f, 0.5f));
		m_labJade->setPosition(leftTop + Vec2(411, -25));
		this->addChild(m_labJade);

		Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
		spJadeIcon->setPosition(leftTop + Vec2(425, -25));
		this->addChild(spJadeIcon);

		m_labSeal = NULL;
	}
	else
	{
		//魂玉充值
		Sprite* spJadeN = ReadSpriteName("new_ui/main/main_bg2.png");
		Sprite* spJadeS = ReadSpriteName("new_ui/main/main_bg2.png");
		MenuItemSprite* btnJade = MenuItemSprite::create(spJadeN, spJadeS, CC_CALLBACK_1(TitleBarLayer::callBackJade, this));
		btnJade->setTag(17);
		btnJade->setPosition(leftTop + Vec2(235, -25));
		m_pBtnMenu->addChild(btnJade);

		//魂玉
		m_labJade = LabelSystemFont(IntToString(history->m_iJade), 16);
		m_labJade->setColor(Color3B::GREEN);
		m_labJade->setAnchorPoint(Vec2(1.0f, 0.5f));
		m_labJade->setPosition(leftTop + Vec2(265, -25));
		this->addChild(m_labJade);

		Sprite* spJadeIcon = ReadSpriteName("new_ui/img_global/icon_gy.png");
		spJadeIcon->setPosition(leftTop + Vec2(285, -25));
		this->addChild(spJadeIcon);

		//玉玺
		Sprite* spSealN = ReadSpriteName("new_ui/main/main_bg3.png");
		Sprite* spSealS = ReadSpriteName("new_ui/main/main_bg3.png");
		MenuItemSprite* btnSeal = MenuItemSprite::create(spSealN, spSealS, CC_CALLBACK_1(TitleBarLayer::callBackSeal, this));
		btnSeal->setTag(16);
		btnSeal->setPosition(leftTop + Vec2(375, -25));
		m_pBtnMenu->addChild(btnSeal);

		//玉玺
		m_labSeal = LabelSystemFont(IntToString(history->m_iJadeSeal), 16);
		m_labSeal->setColor(Color3B::GREEN);
		m_labSeal->setAnchorPoint(Vec2(1.0f, 0.5f));
		m_labSeal->setPosition(leftTop + Vec2(411, -25));
		this->addChild(m_labSeal);

		Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_yx.png");
		spGoldIcon->setPosition(leftTop + Vec2(425, -25));
		this->addChild(spGoldIcon);

		m_labGold = NULL;

	}
}

void TitleBarLayer::callBackHero(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(PlayerInfoLayer::create(this));
}

void TitleBarLayer::callBackGold(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
}

void TitleBarLayer::callBackJade(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this));
}

void TitleBarLayer::callBackSeal(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(PicturePopLayer::create("new_ui/img_global/imgs_bg8.png", true));
}

void TitleBarLayer::updateShow(int index)
{
	m_labJade->setString(IntToString(history->m_iJade));
	if (m_iShowType == TITLE_SHOW_JB_GY)
		m_labGold->setString(IntToStringEx(history->money));
	else
		m_labSeal->setString(IntToString(history->m_iJadeSeal));

	//VIP图标
	string strPath = StringUtils::format("new_ui/main/vip%d.png", history->m_iVIP);
	m_spVIP->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));

	//刷新名字
	m_labName->setString(history->m_pPlayerInfo->getName());
}
void TitleBarLayer::updateMoneyShow()
{
	updateShow(0);
}

void TitleBarLayer::fireToJade() {
	ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this,true));
}

void TitleBarLayer::UpdataFashionShow()
{
	if (!m_spFashion || !m_spWapFashion) return;
	HistoryActorPtr ptrActor = &history->actors[0];
	__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
	__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

	Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
	m_spFashion->setTexture(texfa);
	int fashion_width = texfa->getContentSize().width;
	m_spFashion->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

	Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
	fashion_width = tex->getContentSize().width;
	m_spWapFashion->setTexture(tex);
	m_spWapFashion->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
	m_spWapFashion->setLocalZOrder(trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6]);
}