#include "Defines.h"
#include "NetWarRewardShow.h"
#include "DispatchLayer.h"

NetWarRewardShow* NetWarRewardShow::create(int iWarid,EventDelegate* ptrDelegate)
{
	NetWarRewardShow* pRet = new NetWarRewardShow();
	if (pRet&&pRet->init(iWarid, ptrDelegate))
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

bool NetWarRewardShow::init(int iWarid, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mWarId = iWarid;
	mDelegate = ptrDelegate;
	mBattleStage = 1;

	DSize size = ChessViewTool::getInstance()->getFullSize();
	NetWarInfo* ptrNetWarInfo = trData->m_mNetWarInfo[mWarId];

	mBattleMaxStage = ptrNetWarInfo->iStage;

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	this->addChild(black_layer);

	bg = ReadSpriteName("new_ui/battle/battle_sbg3.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	Sprite* bg_title = ReadSpriteName("new_ui/battle/battle_sbg3_txt1.png");
	bg_title->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	bg->addChild(bg_title);

	Label* AllStageLab = LabelSystemFont(StringUtils::format(LL("bggdgjd"), ptrNetWarInfo->iStage).c_str(),16);
	AllStageLab->setPosition(Vec2(146,272));
	AllStageLab->enableOutline(Color4B(153, 102, 51, 255), 1);
	bg->addChild(AllStageLab);

	mEnemyLevel = LabelSystemFont(StringUtils::format(LL("djdjd"), mBattleStage).c_str(), 16);
	mEnemyLevel->setPosition(Vec2(254, 272));
	mEnemyLevel->enableOutline(Color4B(153, 102, 51, 255), 1);
	bg->addChild(mEnemyLevel);

	mStageGoodsT = LabelSystemFont(StringUtils::format(LL("jddknhddjl"), mBattleStage).c_str(), 16);
	mStageGoodsT->setPosition(Vec2(iBgWidth / 2, 250));
	mStageGoodsT->enableOutline(Color4B(153, 102, 51, 255), 1);
	bg->addChild(mStageGoodsT);

	Sprite* GoodsSpB = NULL;
	for (int i = 0; i < 8; i++)
	{
		GoodsSpB = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		GoodsSpB->setPosition(Vec2(0,0));
		GoodsSpB->setVisible(false);
		bg->addChild(GoodsSpB);
		GoodsBgSp[i] = GoodsSpB;
	}

	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;

	Menu* proMenu = Menu::create();
	proMenu->setPosition(Vec2(iSwitchWidth / 2, iBgHeight / 2));
	bg->addChild(proMenu);

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(NetWarRewardShow::clickProMenu, this));
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

	MenuItemSprite* Next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(NetWarRewardShow::clickNextMenu, this));
	NextMenu->addChild(Next_mItem);
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));


	Menu* mClose = Menu::create();
	mClose->setPosition(Vec2(365, 296));
	bg->addChild(mClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(NetWarRewardShow::clickCloseMenu, this));
	mClose->addChild(close_menu);


	//出兵
	Menu* mDisp = Menu::create();
	mDisp->setPosition(Vec2(iBgWidth/2,40));
	bg->addChild(mDisp);
	Sprite* spSendN = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* spSendS = ReadSpriteName("new_ui/battle/battle_xyb.png");
	spSendS->setScale(0.9f);
	MenuItemSprite* btnSend = MenuItemSprite::create(spSendN, spSendS, CC_CALLBACK_1(NetWarRewardShow::clickDispatchMenu, this));
	spSendS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSendS->setPosition(Vec2(spSendS->getContentSize().width / 2, spSendS->getContentSize().height / 2));
	mDisp->addChild(btnSend);

	UpdataStageShow();
	return true;
}

void NetWarRewardShow::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void NetWarRewardShow::clickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mBattleStage == 1)
		mBattleStage = mBattleMaxStage;
	else
		mBattleStage--;

	UpdataStageShow();
}

void NetWarRewardShow::clickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mBattleStage == mBattleMaxStage)
		mBattleStage = 1;
	else
		mBattleStage++;

	UpdataStageShow();
}

void NetWarRewardShow::UpdataStageShow()
{
	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	int iWarId = mWarId * 100 + mBattleStage;

	if (!trData->net_war_goods.count(iWarId)) return;

	map<int, int> mWarGoods = trData->net_war_goods[iWarId];

	int eGoodsListIndex = mWarGoods.size();
	int iGoodsId = 0, iCount = 0;

	int iPointSNum = 0;
	if (eGoodsListIndex > 4)
		iPointSNum = 4;
	else
		iPointSNum = eGoodsListIndex;

	Sprite* eGoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	int ieGoodsBgWidth = eGoodsBG->getContentSize().width;
	int iPointX = (iBgWidth - ieGoodsBgWidth*iPointSNum - 10 * (iPointSNum - 1)) / 2 + ieGoodsBgWidth / 2;

	int iGoodsIndex = 0;
	for (map<int, int>::iterator eIt = mWarGoods.begin(); eIt != mWarGoods.end(); eIt++)
	{
		iGoodsId = eIt->first;
		iCount = eIt->second;

		GoodsBgSp[iGoodsIndex]->removeAllChildren();
		GoodsBgSp[iGoodsIndex]->setPosition(Vec2(iPointX + (iGoodsIndex % 4)*(ieGoodsBgWidth + 10), 200 - (iGoodsIndex / 4)*(ieGoodsBgWidth + 10)));

		//物品ICON
		Sprite* spIcon = history->getGoodsSprite(iGoodsId);
		spIcon->setPosition(Vec2(33, 33));
		GoodsBgSp[iGoodsIndex]->addChild(spIcon);

		//数目
		if (iCount > 1)
		{
			Sprite* spNum = ITools::getInstance()->getNumSprite(iCount, true);
			spNum->setAnchorPoint(Vec2(1, 0));
			spNum->setPosition(Vec2(62, 4));
			GoodsBgSp[iGoodsIndex]->addChild(spNum);
		}

		GoodsBgSp[iGoodsIndex]->setVisible(true);
		iGoodsIndex++;
	}

	for (int i = iGoodsIndex; i < 8; i++)
	{
		GoodsBgSp[i]->setVisible(false);
	}

	mStageGoodsT->setString(StringUtils::format(LL("jddknhddjl"), mBattleStage).c_str());

	if (mBattleMaxStage)
	{
		int iAve = 100 / mBattleMaxStage;
		int iRem = 0;

		if(mBattleStage == mBattleMaxStage)
			iRem = 100 % mBattleMaxStage;

		int iEnemyLv = mBattleStage*iAve - (iAve + iRem)/2;

		mEnemyLevel->setString(StringUtils::format(LL("djdjd"), iEnemyLv).c_str());
	}
}

void NetWarRewardShow::clickDispatchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->m_iBattleStage = mBattleStage;
	ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(mDelegate));
}

void NetWarRewardShow::onExit()
{
	Layer::onExit();
}

void NetWarRewardShow::onEnter()
{
	Layer::onEnter();
}

NetWarRewardShow::NetWarRewardShow()
{

}

NetWarRewardShow::~NetWarRewardShow()
{

}
