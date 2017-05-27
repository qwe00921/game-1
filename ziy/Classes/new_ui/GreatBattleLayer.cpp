#include "GreatBattleLayer.h"
#include "Defines.h"
#include "GreatBattleScrollview.h"
#include "GoodsSprite.h"
#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "BattleScreen.h"


bool GreatBattleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);


	Sprite* bg = ReadSpriteName("new_ui/zmzy/zmzy_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2 - 10));
	this->addChild(bg);

	Sprite* bg_title = ReadSpriteName("new_ui/zmzy/zmzy_tit1.png");
	bg_title->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height - 5));
	bg->addChild(bg_title);

	int iBattleNum = 1 - history->mMapDateInfo->GetBattleGreatData()->GetAllBattleData().size();
	if (iBattleNum < 0) iBattleNum = 0;
	//次数
	Label* attackNumLab = LabelSystemFont(StringUtils::format(LL("mztzcsdd"), iBattleNum, 1), 16);
	attackNumLab->setPosition(Vec2(bg->getContentSize().width / 2, 328));
	attackNumLab->setColor(Color3B(255, 204, 0));
	attackNumLab->enableOutline(Color4B::BLACK, 1);
	bg->addChild(attackNumLab,1);

	bg->addChild(GreatBattleScrollview::create(this));

	//关闭
	Menu* mMenuClose = Menu::create();
	mMenuClose->setPosition(bg->getPosition() + Vec2(bg->getContentSize().width/2 - 16,bg->getContentSize().height/2 - 10));
	this->addChild(mMenuClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(GreatBattleLayer::clickCloseLayer, this));
	mMenuClose->addChild(close_menu);


	return true;
}

void GreatBattleLayer::RunFunction(int iNomal)
{
	history->m_iBattleType = BATTLE_GREATBATTLE;
	history->m_iCurWarId = iNomal;
	ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
}

void GreatBattleLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if (trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
	{
		history->game_state = GAME_STATE_REDE;
		Chess::getInstance()->appendScreen(new RedeScreen());
	}
	else
	{
		history->game_state = GAME_STATE_BATTLE;
		history->mTitleLayer = BattleTitleLayer::create();
		ChessApplication::getInstance()->AddFunctionLayer(history->mTitleLayer);
		Chess::getInstance()->appendScreen(new BattleScreen());
	}
}

void GreatBattleLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

GreatBattleLayer::GreatBattleLayer()
{

}

GreatBattleLayer::~GreatBattleLayer()
{

}


/////////////////////////////////////////

GreatBattleSelectLayer* GreatBattleSelectLayer::create(int iId,EventDelegate* ptrDelegate)
{
	GreatBattleSelectLayer* pRet = new GreatBattleSelectLayer();
	if (pRet&&pRet->init(iId,ptrDelegate))
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
bool GreatBattleSelectLayer::init(int iId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iStage = 1;
	mDelegate = ptrDelegate;
	m_iBattleId = iId;

	NetZmzyMap* ptrNetZmzyMap = trData->net_zmzy_map[iId];
	m_iMaxStage = ptrNetZmzyMap->warid.size();

	MapDateElem* ptrDataElem = NULL;
	for (size_t i = 0; i < ptrNetZmzyMap->warid.size(); i++)
	{
		ptrDataElem = history->mMapDateInfo->getHeroicMapDateElem(ptrNetZmzyMap->warid[i]);
		if (ptrDataElem)
			mAttackBattle[ptrNetZmzyMap->warid[i]] = 1;
	}

	m_iAllAttackNum = history->mMapDateInfo->GetBattleGreatData()->GetAllBattleData().size();

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/battle/battle_sbg3.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	m_spBg = bg;

	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	m_spBgTitle = ReadSpriteName("new_ui/zmzy/zmzy_gd1.png");
	m_spBgTitle->setPosition(Vec2(iBgWidth / 2, iBgHeight-4));
	bg->addChild(m_spBgTitle);

	m_AllStageLab = LabelSystemFont(StringUtils::format(LL("bgzdgnd"), m_iMaxStage,m_iStage).c_str(), 16);
	m_AllStageLab->setPosition(Vec2(bg->getContentSize().width/2, 272));
	m_AllStageLab->enableOutline(Color4B(153, 102, 51, 255), 1);
	bg->addChild(m_AllStageLab);

	m_warInfoLab = LabelSystemFont(trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->strInfo.c_str(),16);
	m_warInfoLab->setPosition(Vec2(48,254));
	m_warInfoLab->setAnchorPoint(Vec2(0,1));
	m_warInfoLab->setDimensions(304, 0);
	m_warInfoLab->setColor(Color3B(102,51,0));
	bg->addChild(m_warInfoLab);

	Sprite* tiaozhanSp = ReadSpriteName("new_ui/zmzy/zmzy_tit2.png");
	tiaozhanSp->setPosition(Vec2(138,94));
	bg->addChild(tiaozhanSp);

	int iIndex = 0;
	for (map<int, int>::iterator eIt = trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->drop.begin(); eIt != trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->drop.end(); eIt++)
	{
		GoodsSprite* ptrGoodsSprite = GoodsSprite::create(eIt->first, eIt->second);
		ptrGoodsSprite->setPosition(Vec2(44 + iIndex * 50,50));
		bg->addChild(ptrGoodsSprite);
		mGoodList[iIndex++] = ptrGoodsSprite;
	}

	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;
	Menu* proMenu = Menu::create();
	proMenu->setPosition(Vec2(iSwitchWidth / 2, iBgHeight / 2));
	bg->addChild(proMenu);

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(GreatBattleSelectLayer::clickProMenu, this));
	proMenu->addChild(pro_mItem);
	ProSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	ProSSp->setPosition(Vec2(ProSSp->getContentSize().width / 2, ProSSp->getContentSize().height / 2));

	Sprite* NextNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* NextSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	NextNSp->setFlippedX(true);
	NextSSp->setFlippedX(true);
	NextSSp->setScale(0.9f);

	Menu* NextMenu = Menu::create();
	NextMenu->setPosition(Vec2(iBgWidth - iSwitchWidth / 2, iBgHeight / 2));
	bg->addChild(NextMenu);

	MenuItemSprite* Next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(GreatBattleSelectLayer::clickNextMenu, this));
	NextMenu->addChild(Next_mItem);
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));


	Menu* mClose = Menu::create();
	mClose->setPosition(Vec2(365, 296));
	bg->addChild(mClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(GreatBattleSelectLayer::clickCloseMenu, this));
	mClose->addChild(close_menu);


	//出兵
	Menu* mDisp = Menu::create();
	mDisp->setPosition(Vec2(343, 42));
	bg->addChild(mDisp);
	Sprite* spSendN = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* spSendS = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* spSendD = ReadSpriteName("new_ui/battle/battle_xyb.png");
	addSpriteGray(spSendD);
	spSendS->setScale(0.9f);
	m_btnSend = MenuItemSprite::create(spSendN, spSendS, spSendD, CC_CALLBACK_1(GreatBattleSelectLayer::clickDispatchMenu, this));
	spSendS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSendS->setPosition(Vec2(spSendS->getContentSize().width / 2, spSendS->getContentSize().height / 2));
	mDisp->addChild(m_btnSend);

	if(m_iAllAttackNum > 0||(m_iStage>1&& mAttackBattle[ptrNetZmzyMap->warid[m_iStage - 2]] < 1) || !history->mMapDateInfo->GetBattleGreatData()->GreatBattleIsOpen())
		m_btnSend->setEnabled(false);
	return true;
}

void GreatBattleSelectLayer::updataShow()
{
	NetZmzyMap* ptrNetZmzyMap = trData->net_zmzy_map[m_iBattleId];
	m_iMaxStage = ptrNetZmzyMap->warid.size();

	m_spBgTitle->setTexture(ReadSpriteTexture(StringUtils::format("new_ui/zmzy/zmzy_gd%d.png",m_iStage).c_str()));

	m_AllStageLab->setString(StringUtils::format(LL("bgzdgnd"), m_iMaxStage, m_iStage).c_str());

	m_warInfoLab->setString(trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->strInfo.c_str());

	if (m_iAllAttackNum > 0 || (m_iStage > 1 && mAttackBattle[ptrNetZmzyMap->warid[m_iStage - 2]] < 1)||!history->mMapDateInfo->GetBattleGreatData()->GreatBattleIsOpen())
		m_btnSend->setEnabled(false);
	else
		m_btnSend->setEnabled(true);

	for (size_t i = 0; i < mGoodList.size(); i++)
	{
		mGoodList[i]->setVisible(false);
	}

	int iListIndex = 0, iAllGoodsSpNum = mGoodList.size();
	for (map<int, int>::iterator eIt = trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->drop.begin(); eIt != trData->m_mNetWarInfo[ptrNetZmzyMap->warid[m_iStage - 1]]->drop.end(); eIt++)
	{
		if (iListIndex < iAllGoodsSpNum)
		{
			mGoodList[iListIndex]->setVisible(true);
			mGoodList[iListIndex]->updateSprite(eIt->first, eIt->second);
		}
		else
		{
			GoodsSprite* ptrGoodsSprite = GoodsSprite::create(eIt->first, eIt->second);
			ptrGoodsSprite->setPosition(Vec2(44 + iListIndex * 50, 50));
			m_spBg->addChild(ptrGoodsSprite);
			mGoodList[iListIndex] = ptrGoodsSprite;
		}
		iListIndex++;
	}
}

void GreatBattleSelectLayer::clickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_iStage == 1)
		m_iStage = m_iMaxStage;
	else
		m_iStage--;

	updataShow();
}

void GreatBattleSelectLayer::clickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_iStage == m_iMaxStage)
		m_iStage = 1;
	else
		m_iStage++;

	updataShow();
}

void GreatBattleSelectLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void GreatBattleSelectLayer::clickDispatchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (mDelegate)
		mDelegate->RunFunction(trData->net_zmzy_map[m_iBattleId]->warid[m_iStage - 1]);
}

GreatBattleSelectLayer::GreatBattleSelectLayer()
{

}

GreatBattleSelectLayer::~GreatBattleSelectLayer()
{

}