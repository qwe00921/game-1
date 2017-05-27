#include "Common.h"
#include "main.h"
#include "RedeMenuLayer.h"
#include "RedeScreen.h"
#include "ActorInfoLayer.h"
#include "EquipmentUp.h"
#include "RedeMenuLayer.h"
#include "otherfunction.h"
#include "ITools.h"
#include "BackpackLayer.h"
#include "DispatchLayer.h"
#include "Defines.h"

RedeMenuLayer::RedeMenuLayer()
{

}

RedeMenuLayer::~RedeMenuLayer()
{

}

RedeMenuLayer* RedeMenuLayer::create(ScreenPtr ptrScreen)
{ 
	RedeMenuLayer *pRet = new RedeMenuLayer();
	if(pRet && pRet->init(ptrScreen))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool RedeMenuLayer::init(ScreenPtr ptrScreen)
{
	if (!Layer::init())
	{
		return false;
	}
	this->m_pScreen = ptrScreen;
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float fScale = 0.90f;

	Sprite* pSprite = ReadSpriteName("new_ui/icon_ui/icon1_1.png");
	int iSpWidth = pSprite->getContentSize().width; //精灵的宽度
	int iOffset = iSpWidth;
	int iOriginX = iSpWidth / 2 + (size.width - screen_size_x) / 2; //菜单的起始位置
	int iOriginY = iSpWidth / 2 + (size.height - screen_size_y) / 2 + 4;

	m_pFunc = Menu::create();
	m_pFunc->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pFunc);

	m_pMain = Menu::create();
	m_pMain->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pMain);

	return true;

	//出征
	Sprite* send = ReadSpriteName("new_ui/icon_ui/icon1_1.png");
	send->setScale(fScale);
	MenuItemSprite* btnSend = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_1.png"), send, CC_CALLBACK_1(RedeMenuLayer::callBackSend, this));
	send->setAnchorPoint(Vec2(0.5f, 0.5f));
	send->setPosition(send->getPosition() + Vec2(send->getContentSize().width / 2, send->getContentSize().height / 2));
	btnSend->setPosition(Vec2(0, 0));
	//装备
	Sprite* equip = ReadSpriteName("new_ui/icon_ui/icon1_2.png");
	equip->setScale(fScale);
	MenuItemSprite* btnEquip = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_2.png"), equip, CC_CALLBACK_1(RedeMenuLayer::callBackEquip, this));
	equip->setAnchorPoint(Vec2(0.5f, 0.5f));
	equip->setPosition(equip->getPosition() + Vec2(equip->getContentSize().width / 2, equip->getContentSize().height / 2));
	btnEquip->setPosition(Vec2(iOffset, 0));
	//锻造
	Sprite* forge = ReadSpriteName("new_ui/icon_ui/icon1_3.png");
	forge->setScale(fScale);
	MenuItemSprite* btnForge = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_3.png"), forge, CC_CALLBACK_1(RedeMenuLayer::callBackForge, this));
	forge->setAnchorPoint(Vec2(0.5f, 0.5f));
	forge->setPosition(forge->getPosition() + Vec2(forge->getContentSize().width / 2, forge->getContentSize().height / 2));
	btnForge->setPosition(Vec2(iOffset * 2, 0));
	//灵果
	Sprite* fruit = ReadSpriteName("new_ui/icon_ui/icon1_4.png");
	fruit->setScale(fScale);
	MenuItemSprite* btnFruit = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_4.png"), fruit, CC_CALLBACK_1(RedeMenuLayer::callBackFruit, this));
	fruit->setAnchorPoint(Vec2(0.5f, 0.5f));
	fruit->setPosition(fruit->getPosition() + Vec2(fruit->getContentSize().width / 2, fruit->getContentSize().height / 2));
	btnFruit->setPosition(Vec2(iOffset * 3, 0));
	//卖出
	Sprite* sell = ReadSpriteName("new_ui/icon_ui/icon1_5.png");
	sell->setScale(fScale);
	MenuItemSprite* btnSell = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_5.png"), sell, CC_CALLBACK_1(RedeMenuLayer::callBackSell, this));
	sell->setAnchorPoint(Vec2(0.5f, 0.5f));
	sell->setPosition(sell->getPosition() + Vec2(sell->getContentSize().width / 2, sell->getContentSize().height / 2));
	btnSell->setPosition(Vec2(iOffset * 4, 0));
	//买进
	Sprite* buy = ReadSpriteName("new_ui/icon_ui/icon1_6.png");
	buy->setScale(fScale);
	MenuItemSprite* btnBuy = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_6.png"), buy, CC_CALLBACK_1(RedeMenuLayer::callBackBuy, this));
	buy->setAnchorPoint(Vec2(0.5f, 0.5f));
	buy->setPosition(buy->getPosition() + Vec2(buy->getContentSize().width / 2, buy->getContentSize().height / 2));
	btnBuy->setPosition(Vec2(iOffset * 5, 0));
	//商城
	Sprite* mall = ReadSpriteName("new_ui/icon_ui/icon1_11.png");
	Sprite* mall_nomal = ReadSpriteName("new_ui/icon_ui/icon1_11.png");
	mall->setScale(fScale);
	MenuItemSprite* btnMall = MenuItemSprite::create(mall_nomal, mall, CC_CALLBACK_1(RedeMenuLayer::callBackMall, this));
	mall->setAnchorPoint(Vec2(0.5f, 0.5f));
	mall->setPosition(mall->getPosition() + Vec2(mall->getContentSize().width / 2, mall->getContentSize().height / 2));
	btnMall->setPosition(Vec2(screen_size_x - mall->getContentSize().width, 280));
	
	NewGoodsIcon = ReadSpriteName("new_ui/shop/icon_news.png");
	NewGoodsIcon->setAnchorPoint(Vec2(0.8f, 0.5f));
	NewGoodsIcon->setPosition(Vec2(0, mall->getContentSize().height/2+23));
	mall_nomal->addChild(NewGoodsIcon);
	MoveBy* act_mb = MoveBy::create(1, Vec2(0, -6));
	Sequence* act_mb_seq = Sequence::create(act_mb, act_mb->reverse(), NULL);
	RepeatForever* act_mb_loop = RepeatForever::create(act_mb_seq);
	NewGoodsIcon->runAction(act_mb_loop);
	NewGoodsIcon->setVisible(false);
	

	m_pFunc = Menu::create(btnSend, btnEquip, btnForge, btnFruit, btnSell, btnBuy, btnMall, NULL);
	m_pFunc->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pFunc);

	//菜单
	Sprite* menu = ReadSpriteName("new_ui/icon_ui/icon1_10.png");
	menu->setScale(fScale);
	MenuItemSprite* btnMenu = MenuItemSprite::create(ReadSpriteName("new_ui/icon_ui/icon1_10.png"), menu, CC_CALLBACK_1(RedeMenuLayer::callBackMenu, this));
	menu->setAnchorPoint(Vec2(0.5f, 0.5f));
	menu->setPosition(menu->getPosition() + Vec2(menu->getContentSize().width / 2, menu->getContentSize().height / 2));
	btnMenu->setPosition(Vec2(screen_size_x - menu->getContentSize().width, 0));

	m_pMain = Menu::create(btnMenu, NULL);
	m_pMain->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pMain);

	return true;
}

void RedeMenuLayer::onEnter()
{
	Layer::onEnter();
}

void RedeMenuLayer::onExit()
{
	Layer::onExit();
}

bool RedeMenuLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return false;
}
void RedeMenuLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}

void RedeMenuLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void RedeMenuLayer::setLayerVisible(int iType)
{
	bool bFunc = false;
	bool bMain = false;

	if(iType == SHOW_MENU)
	{
		bMain = true;
	}
	else if(iType == SHOW_ALL)
	{
		bFunc = true;
		bMain = true;
	}

	m_pFunc->setVisible(bFunc);
	m_pMain->setVisible(bMain);
}

void RedeMenuLayer::setNewGoodsIconVisible(bool bVisible)
{
	NewGoodsIcon->setVisible(bVisible);
}

void RedeMenuLayer::callBackSend(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create());
}

void RedeMenuLayer::callBackEquip(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
}

void RedeMenuLayer::callBackForge(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
}

void RedeMenuLayer::callBackFruit(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
}

void RedeMenuLayer::callBackSell(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(BackpackLayer::create());
}

void RedeMenuLayer::callBackBuy(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
}

void RedeMenuLayer::callBackMall(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
}

void RedeMenuLayer::callBackMenu(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(history->game_state == GAME_STATE_REDE)
	{
		RedeScreenPtr ptrScreen = (RedeScreenPtr)m_pScreen;
		ptrScreen->m_pMenuLayer->setVisible(false);
		ptrScreen->m_pBarLayer->setVisible(true);
	}
}