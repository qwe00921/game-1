#include "NetLevelInfoLayer.h"
#include "Defines.h"
#include "NetLevelInfoTableView.h"
#include "GetStarLayer.h"
#include "DispatchLayer.h"
#include "GQRankLayer.h"
#include "ShortcutFunctionLayer.h"
#include "GetNetDateTool.h"
#include "SweepTableView.h"
#include "GoodsSprite.h"

NetLevelInfoLayer* NetLevelInfoLayer::create(int iIndex, EventDelegate* ptrDelegate)
{
	NetLevelInfoLayer* pRet = new NetLevelInfoLayer();
	pRet->mIndex = iIndex;
	pRet->mDelegate = ptrDelegate;
	if (pRet&&pRet->init())
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


bool NetLevelInfoLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	mSweepType = -1;

	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	DSize size = ChessViewTool::getInstance()->getFullSize();
	
	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/battle/battle_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	
	auto mBossId = ptrNetmw->mBossId[0];
	if (mBossId < 2) {
		mBossId = 2;
	}
	__String* img_path = __String::create("");
    img_path = __String::createWithFormat("img/Face/%d.png", trData->actors[mBossId -1].face_id+1);
    Sprite* img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
	img->setPosition(Vec2(138,282));
	img->setScale(0.62f);
	bg->addChild(img);

	//81,145
	Sprite* ptrLine = ReadSpriteName("new_ui/battle/battle_line1.png");
	ptrLine->setPosition(Vec2(bg->getContentSize().width/2,145));
	bg->addChild(ptrLine);

	Sprite* bossname_bg = ReadSpriteName("new_ui/battle/battle_bg6.png");
	bossname_bg->setPosition(Vec2(123,163));
	bg->addChild(bossname_bg);

	int iPointY = bossname_bg->getContentSize().height / 2;
	Label* boss_lv = LabelSystemFont(IntToString(ptrNetmw->mBossId[1]), 16);
	boss_lv->setColor(Color3B(55,217,252));
	boss_lv->setPosition(Vec2(23, iPointY));
	bossname_bg->addChild(boss_lv);

	Label* boss_name = LabelSystemFont(trData->actors[mBossId - 1].getActorName(), 16);
	boss_name->setPosition(Vec2(70, iPointY));
	bossname_bg->addChild(boss_name);

	Sprite* tjdj_bg = ReadSpriteName("new_ui/battle/battle_bg5.png");
	tjdj_bg->setPosition(Vec2(327,346));
	bg->addChild(tjdj_bg);

	__String* tjdj_ttf = __String::createWithFormat("%s:%d", LL("tjdj"), ptrNetmw->iLevel);
	Label* tjdj = LabelSystemFont(tjdj_ttf->getCString(), 16);
	tjdj->setColor(Color3B(55, 217, 252));
	tjdj->setPosition(Vec2(tjdj_bg->getContentSize().width / 2-15, tjdj_bg->getContentSize().height/2));
	tjdj_bg->addChild(tjdj);

	//397,305
	Sprite* ptrLine1 = ReadSpriteName("new_ui/battle/battle_line2.png");
	ptrLine1->setPosition(Vec2(397, 295));
	bg->addChild(ptrLine1);

	__String* ptrCityNameS = __String::createWithFormat("%s (%d%s)", ptrNetmw->strName.c_str(), ptrNetmw->iRound,LL("huih"));
	Label* ptrCityName = LabelSystemFont(ptrCityNameS->getCString(), 17);
	ptrCityName->setColor(Color3B(102,0,0));
	ptrCityName->setAnchorPoint(Vec2(0, 0.5));
	ptrCityName->setPosition(Vec2(260,319));
	bg->addChild(ptrCityName);

	//512,326
	Sprite* cktjN = ReadSpriteName("new_ui/battle/battle_bg8.png");
	Sprite* cktjS = ReadSpriteName("new_ui/battle/battle_bg8.png");
	mCkjl = Menu::create();
	mCkjl->setPosition(Vec2(512, 326));
	bg->addChild(mCkjl);
	MenuItemSprite* ckjl_item = MenuItemSprite::create(cktjN, cktjS, CC_CALLBACK_1(NetLevelInfoLayer::clickCkjlMenu, this));
	mCkjl->addChild(ckjl_item);

	MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrNetmw->iWarId);
	int iStarNum = 0;
	if (ptrMapDate != NULL)
		iStarNum = ptrMapDate->getSTAR();
	int star_x = 512;
	int star_y = cktjN->getContentSize().height / 2 + 326;
	Sprite* star1 = ReadSpriteName("new_ui/battle/s1.png");
	star1->setPosition(Vec2(star_x, star_y));
	bg->addChild(star1);
	if (iStarNum<1)
		addSpriteGray(star1);

	Sprite* star2 = ReadSpriteName("new_ui/battle/s2.png");
	star2->setPosition(Vec2(star_x, star_y));
	bg->addChild(star2);
	if (iStarNum<2)
		addSpriteGray(star2);

	Sprite* star3 = ReadSpriteName("new_ui/battle/s3.png");
	star3->setPosition(Vec2(star_x, star_y));
	bg->addChild(star3);
	if (iStarNum<3)
		addSpriteGray(star3);

	__String* txt = __String::create(ptrNetmw->strInfo);
	Label* gk_info = LabelSystemFont(txt->getCString(), 14);
	gk_info->setAnchorPoint(Vec2(0, 1));
	gk_info->setPosition(Vec2(260, 291));
	gk_info->setDimensions(320,77);
	gk_info->setHorizontalAlignment(TextHAlignment::LEFT);
	gk_info->setColor(Color3B(164,77,101));
	bg->addChild(gk_info);

	NetLevelInfoTableView* nliTbv = NetLevelInfoTableView::create(ptrNetmw->mEnemyMap);
	bg->addChild(nliTbv);

	mClose = Menu::create();
	mClose->setPosition(Vec2(587,372));
	bg->addChild(mClose);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(NetLevelInfoLayer::clickCloseLayer, this));
	mClose->addChild(close_menu);

	mBattle = Menu::create();
	mBattle->setPosition(Vec2(545-20, 58+15));
	bg->addChild(mBattle);
	Sprite* mBattleN = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* mBattleS = ReadSpriteName("new_ui/battle/battle_xyb.png");
	MenuItemSprite* battle_menu = MenuItemSprite::create(mBattleN, mBattleS, CC_CALLBACK_1(NetLevelInfoLayer::clickBattleM, this));
	mBattleS->setAnchorPoint(Vec2(0.5, 0.5));
	mBattleS->setPosition(Vec2(mBattleS->getContentSize().width / 2, mBattleS->getContentSize().height / 2));
	mBattleS->setScale(0.9f);
	mBattle->addChild(battle_menu);

	mSweep = Menu::create();
	mSweep->setPosition(Vec2(450-20, 58+15));
	bg->addChild(mSweep);
	Sprite* mSweepN = ReadSpriteName("new_ui/battle/battle_sd.png");
	Sprite* mSweepS = ReadSpriteName("new_ui/battle/battle_sd.png");
	Sprite* mSweepD = ReadSpriteName("new_ui/battle/battle_sd.png");
	addSpriteGray(mSweepD);
	mSweepItem = MenuItemSprite::create(mSweepN, mSweepS, mSweepD, CC_CALLBACK_1(NetLevelInfoLayer::clickSweepM, this));
	mSweepS->setAnchorPoint(Vec2(0.5, 0.5));
	mSweepS->setPosition(Vec2(mSweepS->getContentSize().width / 2, mSweepS->getContentSize().height / 2));
	mSweepS->setScale(0.9f);
	mSweep->addChild(mSweepItem);

	mSweepTtf = LabelSystemFont(LL("sanxingsaodang"),16);
	mSweepTtf->setColor(Color3B::YELLOW);
	mSweepTtf->setPosition(Vec2(450, 72));
	mSweepTtf->enableOutline(Color4B::BLACK, 1);
	bg->addChild(mSweepTtf);
	mSweepTtf->setVisible(false);

	Sprite* num_bg = ReadSpriteName("new_ui/battle/battle_bg7.png");
	num_bg->setPosition(Vec2(479, 122));
	bg->addChild(num_bg);

	Label* sycs = LabelSystemFont(LL("shengyucishu"), 16);
	sycs->setAnchorPoint(Vec2(0,0.5));
	num_bg->addChild(sycs);


	int iCurWarNum = 0;
	if (ptrMapDate != NULL)
		iCurWarNum = ptrMapDate->getATTACKNUM();

	if (iCurWarNum >= ptrNetmw->iMaxTimes)
		iCurWarNum = ptrNetmw->iMaxTimes;

	mSurplusTtf = LabelSystemFont(__String::createWithFormat("%d/%d", ptrNetmw->iMaxTimes - iCurWarNum, ptrNetmw->iMaxTimes)->getCString(), 16);
	mSurplusTtf->setAnchorPoint(Vec2(0, 0.5));
	mSurplusTtf->setColor(Color3B::YELLOW);
	num_bg->addChild(mSurplusTtf);

	int iSycsNWidth = sycs->getContentSize().width + mSurplusTtf->getContentSize().width;
	int iSycsNX = (num_bg->getContentSize().width - iSycsNWidth) / 2;
	sycs->setPosition(Vec2(iSycsNX-30, num_bg->getContentSize().height / 2));
	mSurplusTtf->setPosition(Vec2(iSycsNX + sycs->getContentSize().width, num_bg->getContentSize().height / 2));

	mPrompt = Menu::create();
	mPrompt->setPosition(Vec2(498 + num_bg->getContentSize().width/2-40, 122));
	bg->addChild(mPrompt);
	Sprite* mPromptN = ReadSpriteName("new_ui/img_global/icon_ztx.png");
	Sprite* mPromptS = ReadSpriteName("new_ui/img_global/icon_ztx.png");
	MenuItemSprite* prompt_menu = MenuItemSprite::create(mPromptN, mPromptS, CC_CALLBACK_1(NetLevelInfoLayer::clickPromptM, this));
	mPromptS->setAnchorPoint(Vec2(0.5, 0.5));
	mPromptS->setPosition(Vec2(mPromptS->getContentSize().width / 2, mPromptS->getContentSize().height / 2));
	mPromptS->setScale(0.9f);
	mPrompt->addChild(prompt_menu);

	Sprite* dljl_bg = ReadSpriteName("new_ui/battle/battle_bg9.png");
	dljl_bg->setPosition(Vec2(96, 122));
	bg->addChild(dljl_bg);

	Label* dljl = LabelSystemFont(LL("kndl"), 16);
	dljl->setPosition(Vec2(dljl_bg->getContentSize().width/2-15, dljl_bg->getContentSize().height/2));
	dljl_bg->addChild(dljl);

	//掉落奖励
	addWarDropGoodsElem(bg);

	//224,193
	Sprite* l_arrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	l_arrow->setPosition(Vec2(234, 193));
	bg->addChild(l_arrow);

	Sprite* r_arrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	r_arrow->setPosition(Vec2(578, 193));
	r_arrow->setFlippedX(true);
	bg->addChild(r_arrow);

	for (auto bgchild : bg->getChildren())
	{
		bgchild->setPosition(Vec2(bgchild->getPositionX()+20, bgchild->getPositionY()-15));
	}

	updateMenuState();
	getWinCondition();

	return true;
}

void NetLevelInfoLayer::addWarDropGoodsElem(Sprite* ptrBg)
{
	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	if (trData->net_war_goods.count(ptrNetmw->iWarId))
	{
		int iIndex = 0;
		map<int, int> mDropGoods = trData->net_war_goods[ptrNetmw->iWarId];
		for (map<int, int>::iterator it = mDropGoods.begin(); it != mDropGoods.end(); it++)
		{
			int iGoodsId = it->first;
			GoodsSprite* spItem = GoodsSprite::create(iGoodsId);
			spItem->setPosition(Vec2(75 + 56 * iIndex++, 70));
			ptrBg->addChild(spItem);
		}
	}
}

void NetLevelInfoLayer::getWinCondition()
{
	history->net_win_condition.clear();
	int win_num = 0;
	int war_id = trData->m_mNetWarInfo[mIndex]->iWarId;
	for (int i = 0; i < trData->win_condition_num; i++)
	{
		if (trData->win_condition[i].warid == war_id)
		{
			history->net_win_condition[win_num++] = i;
		}
	}
}

void NetLevelInfoLayer::clickCloseLayer(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void NetLevelInfoLayer::clickBattleM(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->m_iBattleType = BATTLE_SIXWAR;
	history->m_iCurWarId = mIndex;
	history->m_iBattleStage = 0;
	auto _dispatch = DispatchLayer::create(mDelegate);

	
	

	ChessApplication::getInstance()->AddFunctionLayer(_dispatch);
	this->removeFromParent();
}

void NetLevelInfoLayer::clickSweepM(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrNetmw->iWarId);

	int iSweepNum = ptrNetmw->iMaxTimes - ptrMapDate->getATTACKNUM();

	if (iSweepNum <= 0 || ptrMapDate==NULL)
		return;

	int iGoodsNum = 0;
	if (history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if (iGoodsNum)
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_JSD, iSweepNum, iGoodsNum));
	else
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
}

void NetLevelInfoLayer::updateRTDate(int iNomal, int iType)
{
	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrNetmw->iWarId);

	mSweepType = iNomal;

	int iSweepNum = ptrNetmw->iMaxTimes - ptrMapDate->getATTACKNUM();
	int iGoodsNum = 0;
	if (history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if (iType == SHORTCUT_TYPE_JSD&&iNomal == 1 && iSweepNum > iGoodsNum)
	{
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
	}
	else if (iType == SHORTCUT_TYPE_GYSD && ((iNomal == 1 && history->m_iJade < iSweepNum - iGoodsNum) || (iNomal == 0 && history->m_iJade < 1)))
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
	}
	else
	{
		GetNetDateTool::getInstance()->SendSweepDate(ptrNetmw->iWarId, iNomal + 1, this);
	}
}

void NetLevelInfoLayer::SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType)
{
	if (iType == SEND_NET_SWEEP)
	{
		if (bIsSuccess == true)
		{
			NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
			MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrNetmw->iWarId);

			int iBattleNum = 1;
			if (mSweepType > 0)
				iBattleNum = ptrNetmw->iMaxTimes - ptrMapDate->getATTACKNUM();

			history->mMapDateInfo->AddBattleWinDate(ptrNetmw->iMapIndex, ptrNetmw->iWarId, 0, 0, iBattleNum);
			
			int iCurWarNum = ptrMapDate->getATTACKNUM();

			if (iCurWarNum >= ptrNetmw->iMaxTimes)
				iCurWarNum = ptrNetmw->iMaxTimes;

			mSurplusTtf->setString(__String::createWithFormat("%d/%d", ptrNetmw->iMaxTimes - iCurWarNum, ptrNetmw->iMaxTimes)->getCString());

			if (history->m_mConsuNum.count(SWEEP_VOLUME_ID) > 0)
			{
				if (history->m_mConsuNum[SWEEP_VOLUME_ID] >= iBattleNum)
					history->m_mConsuNum[SWEEP_VOLUME_ID] -= iBattleNum;
				else
					history->m_mConsuNum[SWEEP_VOLUME_ID] = 0;
			}

			updateMenuState();

			history->onHttpGoodsAdd(jsonResult, false, true);
			if(jsonResult["goods"].isArray())
				history->m_iCurWarId = mIndex;
				ChessApplication::getInstance()->AddFunctionLayer(SweepTableView::create(jsonResult["goods"], ptrNetmw->iGold));
			updateShow(0);
		}
		else
		{
			//TODO
		}
	}
}

void NetLevelInfoLayer::updateMenuState()
{
	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[mIndex];
	MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(ptrNetmw->iWarId);

	mSweepItem->setEnabled(false);
	if (ptrMapDate == NULL)
	{
		mSweepTtf->setVisible(true);
		return;
	}

	if (ptrMapDate->getSTAR() >= 3)
		mSweepTtf->setVisible(false);
	else
		mSweepTtf->setVisible(true);

	if (ptrMapDate->getSTAR() >= 3 && ptrMapDate->getATTACKNUM()<ptrNetmw->iMaxTimes)
		mSweepItem->setEnabled(true);

}

void NetLevelInfoLayer::clickPromptM(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(NetLevelPromptLayer::create());
}

void NetLevelInfoLayer::clickRankList(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GQRankLayer::create(trData->m_mNetWarInfo[mIndex]->iWarId));
}

void NetLevelInfoLayer::clickCkjlMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GetStarLayer::create(mIndex));
}

void NetLevelInfoLayer::updateShow(int iNomal)
{
	if (mDelegate)
		mDelegate->updateMoneyShow();
}

bool NetLevelInfoLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void NetLevelInfoLayer::onTouchesMoved(Touch *pTouches, Event *pEvent)
{

}
void NetLevelInfoLayer::onTouchesEnded(Touch *pTouches, Event *pEvent)
{

}

void NetLevelInfoLayer::onExit()
{
	Layer::onExit();
}

void NetLevelInfoLayer::onEnter()
{
	Layer::onEnter();
}

NetLevelInfoLayer::NetLevelInfoLayer()
{

}

NetLevelInfoLayer::~NetLevelInfoLayer()
{

}


bool NetLevelPromptLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	DSize size = ChessViewTool::getInstance()->getFullSize();
	
	LayerColor* black_bg = LayerColor::create(Color4B(0,0,0,100));
	black_bg->setPosition(Vec2(0, 0));
	this->addChild(black_bg);

	Sprite* bg = ReadSpriteName("new_ui/battle/battle_ts1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	return true;
}

void NetLevelPromptLayer::onExit()
{
	Layer::onExit();
}

bool NetLevelPromptLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void NetLevelPromptLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

NetLevelPromptLayer::NetLevelPromptLayer()
{
}