#include "Common.h"
#include "main.h"
#include "MenuBarLayer.h"
#include "BattleControler.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "ActorInfoLayer.h"
#include "EquipmentUp.h"
#include "MenuBarLayer.h"
#include "GetStarLayer.h"
#include "Defines.h"
#include "ShortcutFunctionLayer.h"
#include "SysSetLayer.h"
#include "BattleWinConditionLayer.h"
#include "BattleReplay.h"
#include "GuideLayer.h"

#ifndef ENABLE_AUTO
	#define ENABLE_AUTO//客户端自动战斗开关
#endif // !ENABLE_AUTO


#ifdef ENABLE_AUTO
	#include "BattleEndLayer.h"
	#include "Net_HomePage.h"
#endif
MenuBarLayer::MenuBarLayer()
{

}

MenuBarLayer::~MenuBarLayer()
{

}

MenuBarLayer* MenuBarLayer::create(ScreenPtr ptrScreen)
{ 
	MenuBarLayer *pRet = new MenuBarLayer();
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

bool MenuBarLayer::init(ScreenPtr ptrScreen)
{
	if (!Layer::init())
	{
		return false;
	}
	this->m_pScreen = ptrScreen;
	m_iSpeed = 0;

	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	float fScale = 0.90f;

	Sprite* pSprite = ReadSpriteName("new_ui/icon_ui/icon2_1.png");
	int iSpWidth = pSprite->getContentSize().width; //精灵的宽度
	int iOffset = iSpWidth;
	int iOriginX = iSpWidth / 2 + (size.width - screen_size_x) / 2; //菜单的起始位置
	int iOriginY = iSpWidth / 2 + (size.height - screen_size_y) / 2 + 4;
	int iIndex = 0;

	//主菜单列表
	m_pFunc = Menu::create();
	m_pFunc->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pFunc);

	//设置
	Sprite* spSetN = ReadSpriteName("new_ui/icon_ui/icon2_1.png");
	Sprite* spSetS = ReadSpriteName("new_ui/icon_ui/icon2_1.png");
	spSetS->setScale(fScale);
	Sprite* spSetD = ReadSpriteName("new_ui/icon_ui/icon2_2.png");
	MenuItemSprite* btnSet = MenuItemSprite::create(spSetN, spSetS, spSetD, CC_CALLBACK_1(MenuBarLayer::callBackSet, this));
	spSetS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSetS->setPosition(Vec2(spSetS->getContentSize().width / 2, spSetS->getContentSize().height / 2));
	btnSet->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnSet);

	//撤军
	Sprite* spBackN = ReadSpriteName("new_ui/icon_ui/icon2_3.png");
	Sprite* spBackS = ReadSpriteName("new_ui/icon_ui/icon2_3.png");
	spBackS->setScale(fScale);
	Sprite* spBackD = ReadSpriteName("new_ui/icon_ui/icon2_4.png");
	MenuItemSprite* btnBack = MenuItemSprite::create(spBackN, spBackS, spBackD, CC_CALLBACK_1(MenuBarLayer::callBackBack, this));
	spBackS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spBackS->setPosition(Vec2(spBackS->getContentSize().width / 2, spBackS->getContentSize().height / 2));
	btnBack->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnBack);

	//条件
	Sprite* spConditionN = ReadSpriteName("new_ui/icon_ui/icon2_5.png");
	Sprite* spConditionS = ReadSpriteName("new_ui/icon_ui/icon2_5.png");
	spConditionS->setScale(fScale);
	Sprite* spConditionD = ReadSpriteName("new_ui/icon_ui/icon2_6.png");
	MenuItemSprite* btnCondition = MenuItemSprite::create(spConditionN, spConditionS, spConditionD, CC_CALLBACK_1(MenuBarLayer::callBackCondition, this));
	spConditionS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spConditionS->setPosition(Vec2(spConditionS->getContentSize().width / 2, spConditionS->getContentSize().height / 2));
	btnCondition->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnCondition);

	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
		//夺星
		Sprite* spCollectN = ReadSpriteName("new_ui/icon_ui/icon2_7.png");
		Sprite* spCollectS = ReadSpriteName("new_ui/icon_ui/icon2_7.png");
		spCollectS->setScale(fScale);
		Sprite* spCollectD = ReadSpriteName("new_ui/icon_ui/icon2_8.png");
		MenuItemSprite* btnCollect = MenuItemSprite::create(spCollectN, spCollectS, spCollectD, CC_CALLBACK_1(MenuBarLayer::callBackCollect, this));
		spCollectS->setAnchorPoint(Vec2(0.5f, 0.5f));
		spCollectS->setPosition(Vec2(spCollectS->getContentSize().width / 2, spCollectS->getContentSize().height / 2));
		btnCollect->setPosition(Vec2(iOffset * iIndex++, 0));
		m_pFunc->addChild(btnCollect);
	}

	//自动战斗
	Sprite* spAutoN = ReadSpriteName("new_ui/icon_ui/icon10_2.png");
	Sprite* spAutoS = ReadSpriteName("new_ui/icon_ui/icon10_2.png");
	spAutoS->setScale(fScale);
	Sprite* spAutoD = ReadSpriteName("new_ui/icon_ui/icon10_1.png");
	//MenuItemLabel* btnAutoRun = MenuItemLabel::create(LabelSystemFont("[Auto]", 16), CC_CALLBACK_1(MenuBarLayer::callBackAutoRun, this));
	MenuItemSprite* btnAutoRun = MenuItemSprite::create(spAutoN, spAutoS, spAutoD, CC_CALLBACK_1(MenuBarLayer::callBackAutoRun, this));
	spAutoS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spAutoS->setPosition(Vec2(spAutoS->getContentSize().width / 2, spAutoS->getContentSize().height / 2));
	btnAutoRun->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnAutoRun);

	//结束
	Sprite* spFinishN = ReadSpriteName("new_ui/icon_ui/icon2_9.png");
	Sprite* spFinishS = ReadSpriteName("new_ui/icon_ui/icon2_9.png");
	spFinishS->setScale(fScale);
	Sprite* spFinishD = ReadSpriteName("new_ui/icon_ui/icon2_10.png");
	MenuItemSprite* btnFinish = MenuItemSprite::create(spFinishN, spFinishS, spFinishD, CC_CALLBACK_1(MenuBarLayer::callBackFinish, this));
	spFinishS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spFinishS->setPosition(Vec2(spFinishS->getContentSize().width / 2, spFinishS->getContentSize().height / 2));
	btnFinish->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnFinish);
#ifdef ENABLE_AUTO
	

	//直接结束
#ifdef WIN32
	MenuItemLabel* btnWarEnd = MenuItemLabel::create(LabelSystemFont("[End]", 16), CC_CALLBACK_1(MenuBarLayer::callBackWarEnd, this));
	btnWarEnd->setPosition(Vec2(iOffset * iIndex++, 0));
	m_pFunc->addChild(btnWarEnd);
#endif // WIN32


#endif

	//菜单
	Sprite* spMenuN = ReadSpriteName("new_ui/icon_ui/icon2_99.png");
	Sprite* spMenuS = ReadSpriteName("new_ui/icon_ui/icon2_99.png");
	spMenuS->setScale(fScale);
	m_pBtnMenu = MenuItemSprite::create(spMenuN, spMenuS, CC_CALLBACK_1(MenuBarLayer::callBackMenu, this));
	spMenuS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spMenuS->setPosition(Vec2(spMenuS->getContentSize().width / 2, spMenuS->getContentSize().height / 2));
	m_pMenuPos = Vec2(screen_size_x - spMenuS->getContentSize().width, 0);
	m_iMenuWidth = spMenuS->getContentSize().width;
	m_pBtnMenu->setPosition(m_pMenuPos);

	//加速
	m_spSpeedN = ReadSpriteName("new_ui/icon_ui/icon2_100.png");
	m_spSpeedS = ReadSpriteName("new_ui/icon_ui/icon2_100.png");
	m_spSpeedS->setScale(fScale);
	auto btnSpeed = MenuItemSprite::create(m_spSpeedN, m_spSpeedS, CC_CALLBACK_1(MenuBarLayer::callBackSpeed, this));
	m_spSpeedS->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_spSpeedS->setPosition(Vec2(m_spSpeedS->getContentSize().width / 2, m_spSpeedS->getContentSize().height / 2));
	btnSpeed->setPosition(Vec2(0, 0));

	if (history->m_iBattleType == BATTLE_PVP)
	{
		m_pMain = Menu::create(btnSpeed, NULL);
	}
	else
	{
		m_pMain = Menu::create(m_pBtnMenu, NULL);
		scheduleOnce(schedule_selector(MenuBarLayer::menuAction), 5.0f);
	}
	m_pMain->setPosition(Vec2(iOriginX, iOriginY));
	this->addChild(m_pMain);



	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* touch, Event* event) {
		if (history->m_iBattleType != BATTLE_PVP)
		{
			history->m_bAutoRun = false;
		}
		return false;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MenuBarLayer::onEnter()
{
	Layer::onEnter();
}

void MenuBarLayer::onExit()
{
	Layer::onExit();
}

bool MenuBarLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(history->game_state == GAME_STATE_REDE)
	{
		RedeScreenPtr ptrScreen = (RedeScreenPtr)m_pScreen;
		if (ptrScreen->m_pRedeLayer->isVisible() && isVisible())
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			setVisible(false);
			ptrScreen->m_pMenuLayer->setVisible(true);
			return true;
		}
	}
	else if(history->game_state == GAME_STATE_BATTLE)
	{
		BattleScreenPtr ptrScreen = (BattleScreenPtr)m_pScreen;
		if(isVisible() && m_pFunc->isVisible())
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			m_pFunc->setVisible(false);
			return true;
		}
	}

	return false;
}
void MenuBarLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
}

void MenuBarLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void MenuBarLayer::callBackSet(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(SysSetLayer::create());
}

void MenuBarLayer::callBackBack(Ref *sender)
{
	if ((history->m_iCurWarId >= 94 && history->m_iCurWarId <= 99) && !history->mMapDateInfo->isPassed(history->m_iCurWarId))
	{
			ITools::getInstance()->createGlobalPrompt(LL("wufatuichu"), false);
		
	}
	else
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(scr, SHORTCUT_TYPE_CCZC, 0, 0));
	}

}

void MenuBarLayer::callBackCondition(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
	ChessApplication::getInstance()->AddFunctionLayer(BattleWinConditionLayer::create(scr, false));
}

void MenuBarLayer::callBackCollect(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GetStarLayer::create(history->m_iCurWarId, 1));
}

void MenuBarLayer::callBackFinish(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	BattleScreenPtr ptrBs = (BattleScreenPtr)m_pScreen;
	ptrBs->nextRound();
}

void MenuBarLayer::callBackAutoRun(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->m_bAutoRun = !history->m_bAutoRun;
	ITools::getInstance()->createGlobalPrompt(LL("tuichuzidong"), true);


	//history->m_bTakeTurns = history->m_bAutoRun;
}

void MenuBarLayer::callBackWarEnd(Ref* sender) {
#ifdef ENABLE_AUTO
	history->is_battle_win = true;
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create());
	ChessApplication::getInstance()->AddFunctionLayer(BattleEndLayer::create(false));
#endif
}

void MenuBarLayer::callBackMenu(Ref *sender)
{
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer")!=NULL)
	{
		return;
	}
	playSound(SOUND_LEFT_MOUSE_CLICK);
	unschedule(schedule_selector(MenuBarLayer::menuAction));
	m_pBtnMenu->stopAllActions();
	MoveTo* action = MoveTo::create(0.5f, m_pMenuPos);
	m_pBtnMenu->runAction(action);
	scheduleOnce(schedule_selector(MenuBarLayer::menuAction), 5.0f);

	BattleScreenPtr ptrScreen = (BattleScreenPtr)m_pScreen;
	ptrScreen->m_pBatBarLayer->m_pFunc->setVisible(!ptrScreen->m_pBatBarLayer->m_pFunc->isVisible());
}

void MenuBarLayer::callBackSpeed(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	map<int, float> mSpeed;
	mSpeed[0] = 1.0f;
	mSpeed[1] = 5.0f;
	mSpeed[2] = 10.0f;

	if (++m_iSpeed >= (int)mSpeed.size())
	{
		m_iSpeed = 0;
	}

	string strPath = StringUtils::format("new_ui/icon_ui/icon2_%d.png", m_iSpeed + 100);
	m_spSpeedN->setTexture(ReadSpriteTexture(strPath.c_str()));
	m_spSpeedS->setTexture(ReadSpriteTexture(strPath.c_str()));
	Director::getInstance()->getScheduler()->setTimeScale(mSpeed[m_iSpeed]);
}

void MenuBarLayer::SetMenuEnabled(Menu* ptrMenu, bool bEnabled)
{
	if(MenuBarLayer::getMenuEnabled(ptrMenu) == bEnabled)
	{
		return;
	}
	
	if (ptrMenu)
	{
		Vector<Node*> childArray = ptrMenu->getChildren();
		for(ssize_t i = 0; i < childArray.size(); i++)
		{
			MenuItemSprite* sprite = (MenuItemSprite*)childArray.at(i);
			sprite->setEnabled(bEnabled);
		}
	}
}

bool MenuBarLayer::getMenuEnabled(Menu* ptrMenu)
{
	bool bEnabled = false;
	if(ptrMenu)
	{
		Vector<Node*> childArray = ptrMenu->getChildren();
		if(childArray.size() > 0)
		{
			MenuItemSprite* sprite = (MenuItemSprite*)childArray.at(0);
			bEnabled = sprite->isEnabled();
		}
	}
	return bEnabled;
}

void MenuBarLayer::menuAction(float fDelay)
{
	MoveBy* action = MoveBy::create(0.5f, Vec2(m_iMenuWidth / 2, 0));
	m_pBtnMenu->runAction(action);
}