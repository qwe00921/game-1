#include "LadderLayer.h"
#include "Defines.h"
#include "BattleLadder.h"
#include "DispatchLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleTitleLayer.h"
#include "PopLayer.h"
#include "GetNetDateTool.h"
#include "GoodsShopLayer.h"


bool LadderLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	int iCurBoxIndex = 0;
	mClickBoxIndex = -1;

	mCurDayIndex = 0;

	GetElemData();
	mMaxDayIndex = mLadderElemData.size()/2;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/daily/stalker_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	int iBgWidth = bg->getContentSize().width;
	int iBgHeight = bg->getContentSize().height;

	//天数
	Label* tsLab = LabelSystemFont(__String::createWithFormat(LL("didtian"), 1)->getCString(), 18);
	tsLab->setPosition(Vec2(76,334));
	tsLab->setColor(Color3B::YELLOW);
	tsLab->enableOutline(Color4B(102, 51, 0,255), 1);
	bg->addChild(tsLab);

	//开启时间
	Sprite* timeSp = ReadSpriteName("new_ui/img_global/global_ph_txt3.png");
	timeSp->setPosition(Vec2(iBgWidth/2, iBgHeight - 83));
	bg->addChild(timeSp);

	//关卡1
	Sprite* battleElem1Sp = ReadSpriteName("new_ui/daily/stalker_gk1.png");
	battleElem1Sp->setPosition(Vec2((iBgWidth - 230) / 2, iBgHeight / 2));
	bg->addChild(battleElem1Sp);

	Sprite* battleWin1Sp = ReadSpriteName("new_ui/daily/stalker_txt3.png");
	battleWin1Sp->setPosition(battleElem1Sp->getContentSize().width / 2, battleElem1Sp->getContentSize().height / 2);
	battleElem1Sp->addChild(battleWin1Sp);

	//名称
	Sprite* battleNameSp = ReadSpriteName("new_ui/daily/stalker_bg2.png");
	battleNameSp->setPosition(Vec2((iBgWidth - 230) / 2, iBgHeight / 2+86));
	bg->addChild(battleNameSp);

	Label* nameLab = LabelSystemFont("S",16);
	nameLab->setPosition(Vec2(battleNameSp->getContentSize().width / 2, 24));
	battleNameSp->addChild(nameLab);

	//箱子
	Sprite* chestSp = ReadSpriteName("new_ui/daily/stalker_xz1.png");
	chestSp->setPosition(Vec2((iBgWidth - 230) / 2, iBgHeight / 2 - 96));
	bg->addChild(chestSp);

	Sprite* chestTextSp = ReadSpriteName("new_ui/daily/stalker_txt1.png");
	chestTextSp->setPosition(Vec2(chestSp->getContentSize().width / 2, 0));
	chestSp->addChild(chestTextSp);

	//关卡2
	Sprite* battleElem2Sp = ReadSpriteName("new_ui/daily/stalker_gk2.png");
	battleElem2Sp->setPosition(Vec2((iBgWidth + 230) / 2, iBgHeight / 2));
	bg->addChild(battleElem2Sp);

	Sprite* battleWin2Sp = ReadSpriteName("new_ui/daily/stalker_txt3.png");
	battleWin2Sp->setPosition(Vec2(battleElem1Sp->getContentSize().width / 2, battleElem1Sp->getContentSize().height / 2));
	battleElem2Sp->addChild(battleWin2Sp);

	//箱子
	Sprite* chest2Sp = ReadSpriteName("new_ui/daily/stalker_xz2.png");
	chest2Sp->setPosition(Vec2((iBgWidth + 230) / 2, iBgHeight / 2 - 96));
	bg->addChild(chest2Sp);

	Sprite* chestText2Sp = ReadSpriteName("new_ui/daily/stalker_txt2.png");
	chestText2Sp->setPosition(Vec2(chest2Sp->getContentSize().width / 2, 0));
	chest2Sp->addChild(chestText2Sp);

	//名称
	Sprite* battleName2Sp = ReadSpriteName("new_ui/daily/stalker_bg2.png");
	battleName2Sp->setPosition(Vec2((iBgWidth + 230) / 2, iBgHeight / 2 + 86));
	bg->addChild(battleName2Sp);

	Label* name2Lab = LabelSystemFont("S", 16);
	name2Lab->setPosition(Vec2(battleName2Sp->getContentSize().width / 2, 24));
	battleName2Sp->addChild(name2Lab);

	//排行榜
	Sprite* RankNSp = ReadSpriteName("new_ui/img_global/global_ph_icon4.png");
	Sprite* RankSSp = ReadSpriteName("new_ui/img_global/global_ph_icon4.png");
	RankSSp->setScale(0.9f);
	Menu* RankMenu = Menu::create();
	RankMenu->setPosition(Vec2(476, 344));

	MenuItemSprite* Rank_mItem = MenuItemSprite::create(RankNSp, RankSSp, CC_CALLBACK_1(LadderLayer::clickRankMenu, this));
	RankSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	RankSSp->setPosition(Vec2(RankSSp->getContentSize().width / 2, RankSSp->getContentSize().height / 2));
	RankMenu->addChild(Rank_mItem);
	bg->addChild(RankMenu);

	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0,0));
	bg->addChild(ptrMenu);

	Sprite* closeMenuNSp = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeMenuSSp = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeMenuNSp, closeMenuSSp, CC_CALLBACK_1(LadderLayer::cliskCloseMenu, this));
	close_menu->setPosition(Vec2(iBgWidth-18,iBgHeight-45));
	ptrMenu->addChild(close_menu);

	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;

	proMenu = Menu::create();
	proMenu->setPosition(Vec2(iSwitchWidth / 2, iBgHeight / 2));

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(LadderLayer::clickProMenu, this));
	ProSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	ProSSp->setPosition(Vec2(ProSSp->getContentSize().width / 2, ProSSp->getContentSize().height / 2));
	proMenu->addChild(pro_mItem);
	bg->addChild(proMenu);

	Sprite* NextNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* NextSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	NextNSp->setFlippedX(true);
	NextSSp->setFlippedX(true);
	NextSSp->setScale(0.9f);

	NextMenu = Menu::create();
	NextMenu->setPosition(Vec2(iBgWidth - iSwitchWidth / 2, iBgHeight / 2));

	MenuItemSprite* Next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(LadderLayer::clickNextMenu, this));
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));
	NextMenu->addChild(Next_mItem);
	bg->addChild(NextMenu);

	//兑换
	Sprite* spExchN = ReadSpriteName("new_ui/img_global/global_ph_icon6.png");
	Sprite* spExchS = ReadSpriteName("new_ui/img_global/global_ph_icon6.png");
	spExchS->setScale(0.9f);
	MenuItemSprite* btnExch = MenuItemSprite::create(spExchN, spExchS, CC_CALLBACK_1(LadderLayer::clickExchMenu, this));
	spExchS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spExchS->setPosition(Vec2(spExchN->getContentSize().width / 2, spExchN->getContentSize().height / 2));
	auto exchMenu = Menu::create();
	exchMenu->setPosition(Vec2(iBgWidth - iSwitchWidth / 2 - 14, iBgHeight / 2 - 142));
	exchMenu->addChild(btnExch);
	bg->addChild(exchMenu);

	mSpBoxList[0] = battleElem1Sp;
	mSpBoxList[1] = battleElem2Sp;
	mSpBoxList[2] = chestSp;
	mSpBoxList[3] = chest2Sp;

	mLadderShowElem.seteName1Lab(nameLab);
	mLadderShowElem.seteName2Lab(name2Lab);
	mLadderShowElem.seteWin1Sp(battleWin1Sp);
	mLadderShowElem.seteWin2Sp(battleWin2Sp);
	mLadderShowElem.seteBox1Sp(chestSp);
	mLadderShowElem.seteBox2Sp(chest2Sp);
	mLadderShowElem.seteDayLab(tsLab);
	mLadderShowElem.seteElem1BgSp(battleElem1Sp);
	mLadderShowElem.seteElem2BgSp(battleElem2Sp);

	int iDocStartX = (iBgWidth - 140) / 2;

	for (int i = 0; i < 7; i++)
	{
		mElemDoc[i] = ReadSpriteName("new_ui/img_global/suit_bnt3.png");
		mElemDoc[i]->setPosition(Vec2(iDocStartX + i*20, 35));
		bg->addChild(mElemDoc[i]);
	}

	mCurDocSp = mElemDoc[mCurDayIndex];

	updataElemShow();
	return true;
}

void LadderLayer::updataElemShow()
{

	mBoxCanClick.clear();

	mCurDocSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt3.png"));
	mCurDocSp = mElemDoc[mCurDayIndex];
	mCurDocSp->setTexture(ReadSpriteTexture("new_ui/img_global/suit_bnt2.png"));

	mLadderShowElem.geteDayLab()->setString(__String::createWithFormat(LL("didtian"), mCurDayIndex+1)->getCString());

	mLadderShowElem.geteName1Lab()->setString(mLadderElemData[mCurDayIndex * 2]->geteNameS().c_str());
	mLadderShowElem.geteName2Lab()->setString(mLadderElemData[mCurDayIndex * 2+1]->geteNameS().c_str());

	if (mLadderElemData[mCurDayIndex * 2]->getePassBattle())
	{
		mLadderShowElem.geteWin1Sp()->setVisible(true);
		mLadderShowElem.geteBox1Sp()->setTexture(ReadSpriteTexture("new_ui/daily/stalker_xz3.png"));
	}
	else
	{
		mLadderShowElem.geteWin1Sp()->setVisible(false);
		mLadderShowElem.geteBox1Sp()->setTexture(ReadSpriteTexture("new_ui/daily/stalker_xz1.png"));
		mBoxCanClick[0] = 1;
		mBoxCanClick[2] = 1;
	}

	if (mLadderElemData[mCurDayIndex * 2+1]->getePassBattle())
	{
		mLadderShowElem.geteWin2Sp()->setVisible(true);
		mLadderShowElem.geteBox2Sp()->setTexture(ReadSpriteTexture("new_ui/daily/stalker_xz3.png"));
	}
	else
	{
		mLadderShowElem.geteWin2Sp()->setVisible(false);
		mLadderShowElem.geteBox2Sp()->setTexture(ReadSpriteTexture("new_ui/daily/stalker_xz2.png"));
		mBoxCanClick[1] = 1;
		mBoxCanClick[3] = 1;
	}

	if (mLadderElemData[mCurDayIndex * 2]->geteIsOpen())
	{
		mLadderShowElem.geteElem1BgSp()->setColor(Color3B::WHITE);
		mLadderShowElem.geteElem2BgSp()->setColor(Color3B::WHITE);
		removeSpriteGray(mLadderShowElem.geteElem1BgSp());
		removeSpriteGray(mLadderShowElem.geteElem2BgSp());
	}
	else
	{
		addSpriteGray(mLadderShowElem.geteElem1BgSp());
		addSpriteGray(mLadderShowElem.geteElem2BgSp());
		mBoxCanClick[0] = 0;
		mBoxCanClick[1] = 0;
	}

	proMenu->setVisible(true);
	NextMenu->setVisible(true);
	if (mCurDayIndex <= 0)
	{
		proMenu->setVisible(false);
	}
	else if (mCurDayIndex >= mMaxDayIndex - 1)
	{
		NextMenu->setVisible(false);
	}
}

void LadderLayer::GetElemData()
{
	int iElemIndex = 0;

	int iCurWarId = 0;
	int iCurElemIndex = -1;
	if (history->m_iBattleType == BATTLE_LADDER)
		iCurWarId = history->m_iCurWarId;

	map<int, map<int, LadderElemData*> >* ptrLadderElem = BattleLadder::getInstance()->GetLadderHeroData();

	map<int, int> PassWar = BattleLadder::getInstance()->GetLadderLevelData();

	map<int, int> OpenWar = BattleLadder::getInstance()->GetLadderOpenWarData();

	for (map<int, map<int, LadderElemData*> >::iterator mIt = ptrLadderElem->begin(); mIt != ptrLadderElem->end(); mIt++)
	{
		int iWarId = mIt->first;

		LadderShowElemData* ptrLadderElemData = new LadderShowElemData();

		ptrLadderElemData->seteWarId(iWarId);

		if (PassWar.count(iWarId) && OpenWar.count(iWarId))
		{
			ptrLadderElemData->setePassBattle(true);
		}
		else
		{
			ptrLadderElemData->setePassBattle(false);
		}

		if (OpenWar.count(iWarId))
			ptrLadderElemData->seteIsOpen(true);
		else
			ptrLadderElemData->seteIsOpen(false);

		ptrLadderElemData->seteNameS(trData->m_mNetWarInfo[iWarId]->strName.c_str());

		if (trData->m_mNetWarInfo.count(iWarId))
		{
			ptrLadderElemData->seteGold(trData->m_mNetWarInfo[iWarId]->iGold);
			ptrLadderElemData->seteJade(trData->m_mNetWarInfo[iWarId]->iJade);
		}
		else
		{
			ptrLadderElemData->seteIsOpen(false);
			ptrLadderElemData->seteGold(0);
			ptrLadderElemData->seteJade(0);
		}

		if (iCurWarId&&iCurWarId == iWarId)
			iCurElemIndex = iElemIndex;
		else if (!iCurWarId&&OpenWar.count(iWarId))
		{
			iCurElemIndex = iElemIndex;
		}
		mLadderElemData[iElemIndex++] = ptrLadderElemData;

	}

	if (iCurElemIndex > -1)
		mCurDayIndex = iCurElemIndex / 2;
}

bool LadderLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	mClickBoxIndex = -1;
	for (size_t i = 0; i < mSpBoxList.size(); i++)
	{
		Rect boxRect = mSpBoxList[i]->getBoundingBox();
		if (mBoxCanClick[i]&&Rect(0, 0, boxRect.size.width, boxRect.size.height).containsPoint(mSpBoxList[i]->convertToNodeSpace(touch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			mClickBoxIndex = i;
			ScaleTo* ptrScaleTo = ScaleTo::create(0.1f, 0.9f);
			mSpBoxList[i]->runAction(ptrScaleTo);
			return true;
		}
	}
	return true;
}

void LadderLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	if (mClickBoxIndex > -1)
	{
		Rect boxRect = mSpBoxList[mClickBoxIndex]->getBoundingBox();
		if (!Rect(0, 0, boxRect.size.width, boxRect.size.height).containsPoint(mSpBoxList[mClickBoxIndex]->convertToNodeSpace(touch->getLocation())))
		{
			ScaleTo* ptrScaleTo = ScaleTo::create(0.1f, 1);
			mSpBoxList[mClickBoxIndex]->runAction(ptrScaleTo);
			mClickBoxIndex = -1;
			return;
		}
	}
}
void LadderLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (mClickBoxIndex > -1)
	{
		ScaleTo* ptrScaleTo = ScaleTo::create(0.1f, 1);
		mSpBoxList[mClickBoxIndex]->runAction(ptrScaleTo);

		if (mClickBoxIndex == 0)
		{
			//人物关卡
			history->m_iBattleType = BATTLE_LADDER;
			history->m_iCurWarId = mLadderElemData[mCurDayIndex*2]->geteWarId();
			history->m_iBattleStage = 0;
			ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
		}
		else if (mClickBoxIndex == 1)
		{
			//全明星
			history->m_iBattleType = BATTLE_LADDER;
			history->m_iCurWarId = mLadderElemData[mCurDayIndex*2+1]->geteWarId();
			history->m_iBattleStage = 0;
			ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
		}
		else if (mClickBoxIndex == 2 || mClickBoxIndex == 3)
		{
			//普通箱子或玉龙箱子
			ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_YZ_BOX,this));
		}
	}
}

void LadderLayer::ChildCloseLayer(int iNomal)
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

void LadderLayer::cliskCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void LadderLayer::clickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (mMaxDayIndex <= 1||mCurDayIndex==0)
		return;
	
	mCurDayIndex--;
	updataElemShow();
}

void LadderLayer::clickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (mMaxDayIndex <= 1 || mCurDayIndex >= mMaxDayIndex-1)
		return;

	mCurDayIndex++;
	updataElemShow();
}

void LadderLayer::clickRankMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	GetNetDateTool::getInstance()->SendLadderRankData();
}

void LadderLayer::clickExchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoodsExchLayer::create());
}

void LadderLayer::SetPopLayerElement(Sprite* ptrBg)
{
	int iBgWidth = ptrBg->getContentSize().width;
	int iBgHeight = ptrBg->getContentSize().height;

	Sprite* tgjlSp = ReadSpriteName("new_ui/daily/stalker_txt4.png");
	tgjlSp->setAnchorPoint(Vec2(0.5,1));
	tgjlSp->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	ptrBg->addChild(tgjlSp);


	map<int, int> goodsNum;
	if (mClickBoxIndex == 2)
	{
		goodsNum[10000] = mLadderElemData[mCurDayIndex * 2]->geteJade();
		goodsNum[10001] = mLadderElemData[mCurDayIndex * 2]->geteGold();
	}
	else if (mClickBoxIndex == 3)
	{
		goodsNum[10000] = mLadderElemData[mCurDayIndex * 2 + 1]->geteJade();
		goodsNum[10001] = mLadderElemData[mCurDayIndex * 2 + 1]->geteGold();
	}

	for (int i = 10000; i < 10002; i++)
	{
		Sprite* GoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		GoodsBG->setPosition(Vec2(iBgWidth / 2 - GoodsBG->getContentSize().width + (i-10000)*2*GoodsBG->getContentSize().width, iBgHeight / 2 +23));
		ptrBg->addChild(GoodsBG);

		//物品ICON
		Sprite* spIcon = history->getGoodsSprite(i);
		spIcon->setPosition(Vec2(33, 33));
		GoodsBG->addChild(spIcon);

		//数目
		if (goodsNum[i] > 1)
		{
			Sprite* spNum = ITools::getInstance()->getNumSprite(goodsNum[i], true);
			spNum->setAnchorPoint(Vec2(1, 0));
			spNum->setPosition(Vec2(62, 4));
			GoodsBG->addChild(spNum);
		}
	}
}

LadderLayer::LadderLayer()
{

}

LadderLayer::~LadderLayer()
{
	for (size_t i = 0; i < mLadderElemData.size(); i++)
	{
		delete mLadderElemData[i];
	}
	mLadderElemData.clear();
}