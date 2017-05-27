#include "HeroicAmbitionLayer.h"
#include "Defines.h"
#include "Net_HomePage.h"
#include "DispatchLayer.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "SweepTableView.h"
#include "ShortcutFunctionLayer.h"
#include "GetNetDateTool.h"
#include "PopLayer.h"
#include "GoodsSprite.h"
#include "DestroySixCountriesLayer.h"

HeroicAmbitionLayer* HeroicAmbitionLayer::create(int iCurWarId)
{
	HeroicAmbitionLayer* pRet = new HeroicAmbitionLayer();
	if (pRet&&pRet->init(iCurWarId))
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

bool HeroicAmbitionLayer::init(int iCurWarId)
{
	if (!Layer::init())
	{
		return false;
	}

	ChessPlayMp3(MUSIC_BATTLE_SCENE);
	mCurWarId = -1;
	if (iCurWarId > 0)
	{
		mCurWarId = iCurWarId;
	}
	else if (history->m_iBattleType == BATTLE_HEROIC&&history->m_iCurWarId>0)
	{
		mCurWarId = history->m_iCurWarId;
	}

	mShowElemIndex = 1;
	mDateElemIndex = 0;
	mEnableClick = true;
	mSweepType = -1;
	mEnableMove = true;
	mSize = ChessViewTool::getInstance()->getFullSize();

	bg = ITools::getInstance()->getSpriteWithPath("img/heroz/heroz_bg3.jpg");
	bg->setPosition(Vec2(mSize.width / 2, mSize.height / 2));
	this->addChild(bg);

	Sprite* infoTitleSp = ReadSpriteName("new_ui/battle/battle_til3.png");
	infoTitleSp->setPosition(Vec2(0, mSize.height - 10));
	infoTitleSp->setAnchorPoint(Vec2(0, 1));
	this->addChild(infoTitleSp, 1);

	//返回
	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(HeroicAmbitionLayer::clickMenuBack, this));

	Menu* mBack = Menu::create();
	mBack->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8), mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mBack, 1);
	mBack->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width/2, mb_select->getContentSize().height/2));

	//普通难度
	Sprite* ordinary_nomal = ReadSpriteName("new_ui/battle/battle_icon1.png");
	Sprite* ordinary_select = ReadSpriteName("new_ui/battle/battle_icon1.png");
	ordinary_select->setScale(0.9f);
	MenuItemSprite* ordinary_item = MenuItemSprite::create(ordinary_nomal, ordinary_select, CC_CALLBACK_1(HeroicAmbitionLayer::clickOrdinaryMenu, this));

	Menu* mOMenu = Menu::create();
	mOMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8) - 150, mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mOMenu, 1);
	mOMenu->addChild(ordinary_item);
	ordinary_select->setAnchorPoint(Vec2(0.5, 0.5));
	ordinary_select->setPosition(Vec2(ordinary_select->getContentSize().width / 2, ordinary_select->getContentSize().height / 2));

	//困难难度
	Sprite* difficulty_nomal = ReadSpriteName("new_ui/battle/battle_icon2.png");
	Sprite* difficulty_select = ReadSpriteName("new_ui/battle/battle_icon2.png");
	difficulty_select->setScale(0.9f);
	MenuItemSprite* difficulty_item = MenuItemSprite::create(difficulty_nomal, difficulty_select, CC_CALLBACK_1(HeroicAmbitionLayer::clickDifficultyMenu, this));

	Menu* mDMenu = Menu::create();
	mDMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8) - 80, mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mDMenu, 1);
	mDMenu->addChild(difficulty_item);
	difficulty_select->setAnchorPoint(Vec2(0.5, 0.5));
	difficulty_select->setPosition(Vec2(difficulty_select->getContentSize().width / 2, difficulty_select->getContentSize().height / 2));

	GetDateElem();
	GetShowElem();

	//扫荡
	Sprite* SweepN = ReadSpriteName("new_ui/battle/battle_sd.png");
	Sprite* SweepS = ReadSpriteName("new_ui/battle/battle_sd.png");
	Sprite* SweepD = ReadSpriteName("new_ui/battle/battle_sd.png");
	addSpriteGray(SweepD);
	SweepItem = MenuItemSprite::create(SweepN, SweepS, SweepD, CC_CALLBACK_1(HeroicAmbitionLayer::clickSweepMenu, this));
	SweepS->setAnchorPoint(Vec2(0.5, 0.5));
	SweepS->setScale(0.9f);
	SweepS->setPosition(Vec2(SweepS->getContentSize().width / 2, SweepS->getContentSize().height/2));

	Menu* mSweep = Menu::create();
	mSweep->setPosition(Vec2(mSize.width - 166, 46));
	this->addChild(mSweep, 1);
	mSweep->addChild(SweepItem);
	SweepItem->setEnabled(false);

	//出兵
	Sprite* DispatchN = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* DispatchS = ReadSpriteName("new_ui/battle/battle_xyb.png");
	Sprite* DispatchD = ReadSpriteName("new_ui/battle/battle_xyb.png");
	DispatchS->setScale(0.9f);
	addSpriteGray(DispatchD);
	DispatchItem = MenuItemSprite::create(DispatchN, DispatchS, DispatchD, CC_CALLBACK_1(HeroicAmbitionLayer::clickDispatchMenu, this));
	DispatchS->setAnchorPoint(Vec2(0.5, 0.5));
	DispatchS->setPosition(Vec2(DispatchS->getContentSize().width / 2, DispatchS->getContentSize().height / 2));

	Menu* mDispatch = Menu::create();
	mDispatch->setPosition(Vec2(mSize.width - 70, 46));
	this->addChild(mDispatch, 1);
	mDispatch->addChild(DispatchItem);

	SweepPrompt = LabelSystemFont(LL("guoguansaodang"),16);
	SweepPrompt->setColor(Color3B::YELLOW);
	SweepPrompt->setPosition(Vec2(mSize.width - 166, 56));
	this->addChild(SweepPrompt,2);

	updateMenuState();
	return true;
}

bool HeroicAmbitionLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	mEnableMove = true;
	return true;
}

void HeroicAmbitionLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (mEnableClick&&mEnableMove)
	{
		float fOffsetX = pTouch->getStartLocation().x - pTouch->getLocation().x;
		if (fOffsetX > 5)
		{
			ShowAction(false);
			mEnableMove = false;
		}
		else if (fOffsetX < -5)
		{
			ShowAction(true);
			mEnableMove = false;
		}
	}
}

void HeroicAmbitionLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{

	mEnableMove = true;
}

void HeroicAmbitionLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
	this->removeFromParent();
}

void HeroicAmbitionLayer::RunFunction(int iNomal)
{

	int iWarRefNum = history->mMapDateInfo->getHeroicMapDateElem(mDateElem[mDateElemIndex]->getWarId())->getRefNum();
	int iSpeedJade = 0;
	if (trData->net_heroz_price.count(iWarRefNum + 1) > 0)
	{
		iSpeedJade = trData->net_heroz_price[iWarRefNum + 1];
	}
	else if (iWarRefNum + 1 > trData->net_heroz_price[0])
	{
		iSpeedJade = trData->net_heroz_price[trData->net_heroz_price[0]];
	}

	if (iSpeedJade <= history->m_iJade)
		GetNetDateTool::getInstance()->SendRefWarCount(mDateElem[mDateElemIndex]->getWarId(), this);
	else
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
}

void HeroicAmbitionLayer::clickAddNumMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* mItem = (MenuItemSprite*)sender;
	int mTag = mItem->getTag();
	if (mTag != mShowElemIndex) return;

	GetNetDateTool::getInstance()->SendUpdataWarRefNum(mDateElem[mDateElemIndex]->getWarId(), this);
}

void HeroicAmbitionLayer::clickDispatchMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->m_iBattleType = BATTLE_HEROIC;
	history->m_iCurWarId = mDateElem[mDateElemIndex]->getWarId();
	history->m_iBattleStage = 0;
	ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
}

void HeroicAmbitionLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if(trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
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
void HeroicAmbitionLayer::clickSweepMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iSweepNum =mDateElem[mDateElemIndex]->getCurNum();
	int iGoodsNum = 0;
	if (history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if (iGoodsNum)
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_JSD, iSweepNum, iGoodsNum));
	else
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
}

void HeroicAmbitionLayer::clickOrdinaryMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
	this->removeFromParent();
}
void HeroicAmbitionLayer::clickDifficultyMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (!trData->isSystemOpen(SystemOpenIDS::E_YOULIJINGYING, true))
	{
		//ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("tgztxddgkq"), trData->m_mNetWarInfo[129]->strName.c_str())->getCString(), false);
		return;
	}

	ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create(-1,DISSICULT_MODEL));
	this->removeFromParent();
}

void HeroicAmbitionLayer::setEnableClick(float f)
{
	mEnableClick = true;
}

void HeroicAmbitionLayer::updateRTDate(int iNomal,int iType)
{
	mSweepType = iNomal;
	int iSweepNum = mDateElem[mDateElemIndex]->getCurNum();
	int iGoodsNum = 0;
	history->m_iCurWarId = mDateElem[mDateElemIndex]->getWarId();
	if (history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if (iType == SHORTCUT_TYPE_JSD&&iNomal == 1 && iSweepNum>iGoodsNum)
	{
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
	}
	else if (iType == SHORTCUT_TYPE_GYSD && ((iNomal == 1 && history->m_iJade<iSweepNum - iGoodsNum) || (iNomal == 0 && history->m_iJade<1)))
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"),false);
	}
	else
	{
		GetNetDateTool::getInstance()->SendSweepDate(mDateElem[mDateElemIndex]->getWarId(), iNomal+1, this);
	}
}

void HeroicAmbitionLayer::SendNetDateResult(bool bIsSuccess, Json::Value jsonResult,int iType)
{
	if (iType == SEND_NET_SWEEP)
	{
		if (bIsSuccess == true)
		{
			int iBattleNum = 1;
			if (mSweepType > 0)
				iBattleNum = mDateElem[mDateElemIndex]->getCurNum();

			history->mMapDateInfo->AddHeroicBattleWinDate(mDateElem[mDateElemIndex]->getWarId(), iBattleNum);
			mDateElem[mDateElemIndex]->setCurNum(mDateElem[mDateElemIndex]->getCurNum() - iBattleNum);
			mShowElem[mShowElemIndex]->getSurplusNum()->setString(__String::createWithFormat("%d/%d", mDateElem[mDateElemIndex]->getCurNum(), mDateElem[mDateElemIndex]->getMaxNum())->getCString());

			if (history->m_mConsuNum.count(SWEEP_VOLUME_ID) > 0)
			{
				if (history->m_mConsuNum[SWEEP_VOLUME_ID] >= iBattleNum)
					history->m_mConsuNum[SWEEP_VOLUME_ID] -= iBattleNum;
				else
					history->m_mConsuNum[SWEEP_VOLUME_ID] = 0;
			}

			updateMenuState();

			history->onHttpGoodsAdd(jsonResult, false, true);
			if (jsonResult["goods"].isArray())
				ChessApplication::getInstance()->AddFunctionLayer(SweepTableView::create(jsonResult["goods"], trData->m_mNetWarInfo[mDateElem[mDateElemIndex]->getWarId()]->iGold));
			updateShow(0);
		}
	}
	else if (iType == SEND_NET_REFRESH)
	{
		if (bIsSuccess)
		{
			if (jsonResult["jade"].isInt())
			{
				history->m_iJade = jsonResult["jade"].asInt();
			}

			int iWarId = mDateElem[mDateElemIndex]->getWarId();
			int iMaxWarCount = trData->m_mNetWarInfo[iWarId]->iMaxTimes;
			MapDateElem* ptrMDE = history->mMapDateInfo->getHeroicMapDateElem(iWarId);
			ptrMDE->setATTACKNUM(0);
			ptrMDE->setRefNum(ptrMDE->getRefNum() + 1);
			history->m_pPlayerInfo->m_mMapAttackNum[iWarId][1]++;

			mDateElem[mDateElemIndex]->setCurNum(iMaxWarCount);
			mShowElem[mShowElemIndex]->getSurplusNum()->setString(__String::createWithFormat("%d/%d", iMaxWarCount, mDateElem[mDateElemIndex]->getMaxNum())->getCString());

			updateMenuState();

			updateShow(0);
		}
	}
	else if (iType == SEND_NET_UPDATA_REFNUM)
	{
		if (bIsSuccess)
		{
			if (jsonResult["warCount"].isObject() && jsonResult["warCount"]["refNum"].isInt())
			{
				int iRefNum = jsonResult["warCount"]["refNum"].asInt();
				int iWarId = mDateElem[mDateElemIndex]->getWarId();
				MapDateElem* ptrMDE = history->mMapDateInfo->getHeroicMapDateElem(iWarId);
				ptrMDE->setRefNum(iRefNum);
				history->m_pPlayerInfo->m_mMapAttackNum[iWarId][1] = iRefNum;
				ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_BUY_WARCOUNT, this));
			}
		}
	}
}

void HeroicAmbitionLayer::SetPopLayerElement(Sprite* ptrBg)
{
	Label* titleTtf1 = LabelSystemFont(LL("czgqjrcsxhf"),18);
	titleTtf1->setAnchorPoint(Vec2(0, 0.5));
	titleTtf1->setPosition(Vec2(66,135));
	ptrBg->addChild(titleTtf1);

	int iWarRefNum = history->mMapDateInfo->getHeroicMapDateElem(mDateElem[mDateElemIndex]->getWarId())->getRefNum();
	int iSpeedJade = 0;
	if (trData->net_heroz_price.count(iWarRefNum + 1) > 0)
	{
		iSpeedJade = trData->net_heroz_price[iWarRefNum + 1];
	}
	else if (iWarRefNum + 1 > trData->net_heroz_price[0])
	{
		iSpeedJade = trData->net_heroz_price[trData->net_heroz_price[0]];
	}
	Label* titleTtf2 = LabelSystemFont(__String::createWithFormat("%d%s", iSpeedJade, LL("gouyu"))->getCString(), 18);
	titleTtf2->setColor(Color3B(0,255,255));
	titleTtf2->setAnchorPoint(Vec2(0, 0.5));
	titleTtf2->setPosition(Vec2(73 + titleTtf1->getContentSize().width, 135));
	ptrBg->addChild(titleTtf2);

	Label* titleTtf3 = LabelSystemFont(__String::createWithFormat(LL("shifoujixu"), iWarRefNum)->getCString(), 18);
	titleTtf3->setPosition(Vec2(ptrBg->getContentSize().width / 2, 129 - titleTtf1->getContentSize().height));
	ptrBg->addChild(titleTtf3);
}

void HeroicAmbitionLayer::GetDateElem()
{
	int iDateIndex = 0;
	int iCurWarDateindex = -1;
	for (map<int, NetWarInfo*>::iterator NhwIt = trData->m_mNetWarInfo.begin(); NhwIt != trData->m_mNetWarInfo.end(); NhwIt++)
	{
		int iWarId = NhwIt->first;
		NetWarInfo* ptrNHW = NhwIt->second;
		if (ptrNHW->iType == BATTLE_HEROIC)
		{
			HeroicDateElemPtr ptrHDE = new HeroicDateElem();

			int iOpenWarId = ptrNHW->iOpenWarId;
			int iIsOpen = 0, iAttackNum = 0, iAMaxNum = 0, iIsPass = 0;
			if(history->mMapDateInfo->getMapDateElem(iOpenWarId) != NULL)
				iIsOpen = 1;
			if(history->mMapDateInfo->getHeroicMapDateElem(iWarId) != NULL)
			{
				iAttackNum = history->mMapDateInfo->getHeroicMapDateElem(iWarId)->getATTACKNUM();
				iIsPass = 1;
			}
			iAMaxNum = trData->m_mNetWarInfo[iWarId]->iMaxTimes;
			ptrHDE->setWarId(iWarId);
			ptrHDE->setOpenLevel(iOpenWarId);
			ptrHDE->setIsOpen(iIsOpen);
			ptrHDE->setMapId(ptrNHW->iMapId);
			ptrHDE->setCurNum(iAMaxNum - iAttackNum);
			ptrHDE->setMaxNum(iAMaxNum);
			ptrHDE->setIsPass(iIsPass);
			int iGoodsListIndex = 0;
			map<int, int> drop_goods;
			if(trData->net_war_goods.count(iWarId))
				drop_goods = trData->net_war_goods[iWarId];
			for(map<int, int>::iterator GIt = drop_goods.begin(); GIt != drop_goods.end(); GIt++)
			{
				int iGoodsId = GIt->first;
				int iCount = GIt->second;

				ptrHDE->GoodsList[iGoodsListIndex][0] = iGoodsId;
				ptrHDE->GoodsList[iGoodsListIndex][1] = iCount;
				iGoodsListIndex++;
				if(iGoodsListIndex > 1) break;
			}
			ptrHDE->GoodsList[iGoodsListIndex][0] = -1;
			ptrHDE->GoodsList[iGoodsListIndex][1] = ptrNHW->iStore;
			if (mCurWarId>0&&iWarId == mCurWarId)
				iCurWarDateindex = iDateIndex;
			mDateElem[iDateIndex++] = ptrHDE;
		}
	}

	if (iCurWarDateindex >= 0)
		mDateElemIndex = iCurWarDateindex;
}

void HeroicAmbitionLayer::GetShowElem()
{
	for (int i =  - 1; i < 3; i++)
	{
		int iDElemId = mDateElemIndex + i;
		int iSElemId = mShowElemIndex + i;

		__String* eBg_pathCs = NULL;//背景路径
		__String* ePromptTxtCs = NULL;//开启条件
		__String* eActorImg_pathCs = NULL;//武将头像
		__String* eActorName_pathCs = NULL;//武将名图片
		__String* eSurplusNumCs = NULL;//剩余关卡
		map<int, map<int, int> > eGoodsList;
		bool bIsOpen = false;
		bool bIsVisibal = true;
		bool isFirst = false;
		int eWarId = -1;
		int eDateindex = -1;

		if (iDElemId >= 0 && iDElemId < (int)mDateElem.size())
		{
			eBg_pathCs = __String::createWithFormat("new_ui/img_global/heroz_bg%d.png", (mDateElem[iDElemId]->getIsOpen()!=0?1:2));
			ePromptTxtCs = __String::createWithFormat(LL("tgztxddgkq"), trData->m_mNetWarInfo[mDateElem[iDElemId]->getOpenLevel()]->strName.c_str());
			eActorImg_pathCs = __String::createWithFormat("img/heroz/hero%d.png", mDateElem[iDElemId]->getWarId());
			eActorName_pathCs = __String::createWithFormat("img/heroz/hero_name%d.png", mDateElem[iDElemId]->getWarId());
			eSurplusNumCs = __String::createWithFormat("%d/%d", mDateElem[iDElemId]->getCurNum(), mDateElem[iDElemId]->getMaxNum());
			eGoodsList = mDateElem[iDElemId]->GoodsList;
			bIsOpen = mDateElem[iDElemId]->getIsOpen()!=0?true:false;
			isFirst = mDateElem[iDElemId]->getIsPass() != 0 ? false : true;
			eWarId = mDateElem[iDElemId]->getWarId();
			eDateindex = iDElemId;
		}
		else
		{
			eBg_pathCs = __String::create("new_ui/img_global/heroz_bg1.png");
			ePromptTxtCs = __String::create("");
			eActorImg_pathCs = __String::createWithFormat("new_ui/img_global/heroz_bg1.png");
			eActorName_pathCs = __String::createWithFormat("new_ui/hyaline/116.png");
			eSurplusNumCs = __String::create("3/3");
			bIsVisibal = false;
		}

		int iPointX = mSize.width/2+i*240;
		int iPointY = mSize.height / 2 - abs(i) * 18+24;
		Sprite* eBg = ReadSpriteName(eBg_pathCs->getCString());
		eBg->setPosition(Vec2(iPointX, iPointY));
		this->addChild(eBg);
		if (i != 0)
			eBg->setScale(0.7f);

		int iBgWidth = eBg->getContentSize().width;
		int iBgHeight = eBg->getContentSize().height;

		Sprite* eActorImg = NULL;
		if (bIsVisibal)
			eActorImg = ITools::getInstance()->getSpriteWithPath(eActorImg_pathCs->getCString());
		else
			eActorImg = ReadSpriteName(eActorImg_pathCs->getCString());
		eActorImg->setPosition(Vec2(iBgWidth / 2, iBgHeight / 2));
		eBg->addChild(eActorImg);

		Sprite* eActorNameBg = ReadSpriteName("new_ui/img_global/heroz_txtbg1.png");
		eActorNameBg->setPosition(Vec2(222, 250));
		eActorNameBg->setAnchorPoint(Vec2(0, 1));
		eBg->addChild(eActorNameBg);

		Sprite* eActorName = NULL;
		if (bIsVisibal)
			eActorName = ITools::getInstance()->getSpriteWithPath(eActorName_pathCs->getCString());
		else
			eActorName = ReadSpriteName(eActorName_pathCs->getCString());

		eActorName->setAnchorPoint(Vec2(0.5, 1));
		eActorName->setPosition(Vec2(eActorNameBg->getContentSize().width / 2, eActorNameBg->getContentSize().height - 18));
		eActorNameBg->addChild(eActorName);

		//提示首次通关
		Sprite* ePromptImg = ReadSpriteName("new_ui/img_global/heroz_txt1.png");
		ePromptImg->setAnchorPoint(Vec2(0.5, 0));
		ePromptImg->setPosition(Vec2(iBgWidth / 2,70));
		eBg->addChild(ePromptImg);

		Sprite* ePromptBg = ReadSpriteName("new_ui/img_global/heroz_txtbg3.png");
		ePromptBg->setAnchorPoint(Vec2(0.5, 1));
		ePromptBg->setPosition(Vec2(iBgWidth/2,69));
		eBg->addChild(ePromptBg);

		Label* ePromptTxt = LabelSystemFont(ePromptTxtCs->getCString(),16);
		ePromptTxt->setColor(Color3B(255, 153, 0));
		ePromptTxt->setPosition(Vec2(ePromptBg->getContentSize().width / 2, ePromptBg->getContentSize().height/2));
		ePromptBg->addChild(ePromptTxt);
		ePromptTxt->enableOutline(Color4B::BLACK, 1);
		ePromptBg->setVisible(false);
		
		Sprite* eBarrierBg = ReadSpriteName("new_ui/img_global/heroz_txtbg2.png");
		eBarrierBg->setAnchorPoint(Vec2(0.5, 1));
		eBarrierBg->setPosition(Vec2(iBgWidth / 2, 69));
		eBg->addChild(eBarrierBg);

		int iSurplusBgWidth = eBarrierBg->getContentSize().width;
		int iSurplusBgHeight = eBarrierBg->getContentSize().height;
		Label* eSurplusTxt = LabelSystemFont(LL("shengyucishu"),16);
		eSurplusTxt->setAnchorPoint(Vec2(0, 1));
		eSurplusTxt->setPosition(Vec2(22, iSurplusBgHeight - 14));
		eSurplusTxt->setColor(Color3B::YELLOW);
		eBarrierBg->addChild(eSurplusTxt);

		Label* eSurplusNum = LabelSystemFont(eSurplusNumCs->getCString(), 16);
		eSurplusNum->setAnchorPoint(Vec2(0, 1));
		eSurplusNum->setPosition(Vec2(24 + eSurplusTxt->getContentSize().width, iSurplusBgHeight - 14));
		eBarrierBg->addChild(eSurplusNum);

		Menu* eAddMenu = Menu::create();
		eAddMenu->setPosition(Vec2(iSurplusBgWidth - 34, iSurplusBgHeight - 22));
		eBarrierBg->addChild(eAddMenu);
		if (i!=0)
			eAddMenu->setEnabled(false);
		eAddMenu->setVisible(false);

		Sprite* eAddMenu_N = ReadSpriteName("new_ui/img_global/heroz_add1.png");
		Sprite* eAddMenu_S = ReadSpriteName("new_ui/img_global/heroz_add1.png");
		eAddMenu_S->setScale(0.9f);

		MenuItemSprite* eAddMenuitem = MenuItemSprite::create(eAddMenu_N, eAddMenu_S, CC_CALLBACK_1(HeroicAmbitionLayer::clickAddNumMenu, this));
		eAddMenu_S->setAnchorPoint(Vec2(0.5, 0.5));
		eAddMenu_S->setPosition(Vec2(eAddMenu_S->getContentSize().width / 2, eAddMenu_S->getContentSize().height/2));
		eAddMenuitem->setTag(iSElemId);
		eAddMenu->addChild(eAddMenuitem);

		Label* eGetTxt = LabelSystemFont(LL("kndl"),16);
		eGetTxt->setColor(Color3B::YELLOW);
		eGetTxt->setPosition(Vec2(22,iSurplusBgHeight-40));
		eGetTxt->setAnchorPoint(Vec2(0, 1));
		eBarrierBg->addChild(eGetTxt);

		map<int, Sprite*> eGoodsbg;
		for (int j = 0; j < 3; j++)
		{
			eGoodsbg[j] = ReadSpriteName("new_ui/img_global/item_box_bg.png");
			eGoodsbg[j]->setPosition(Vec2(44+j*65,12));
			eBarrierBg->addChild(eGoodsbg[j]);

			if (eGoodsList.count(j))
			{
				AddGoodsElem(eGoodsbg[j], eGoodsList[j][0], eGoodsList[j][1]);
			}
		}

		if (!bIsVisibal||i==2)
		{
			eBg->setVisible(false);
		}
		else
		{
			if (!bIsOpen)
			{
				eActorImg->setColor(Color3B::BLACK);
				eActorNameBg->setVisible(false);
				ePromptImg->setVisible(false);
				ePromptBg->setVisible(true);
				eBarrierBg->setVisible(false);
				eGoodsbg[0]->setVisible(false);
				eGoodsbg[1]->setVisible(false);
				eGoodsbg[2]->setVisible(false);
			}
			else if (!isFirst)
			{
				ePromptImg->setVisible(false);
			}
		}

		HeroicShowElemPtr ptrHSE = new HeroicShowElem();
		ptrHSE->setWarId(eWarId);
		ptrHSE->setDateIndex(eDateindex);
		ptrHSE->setBg(eBg);
		ptrHSE->setPromptBg(ePromptBg);
		ptrHSE->setOpenPrompt(ePromptTxt);
		ptrHSE->setActorIcon(eActorImg);
		ptrHSE->setActorName(eActorName);
		ptrHSE->setNameBg(eActorNameBg);
		ptrHSE->setPromptImg(ePromptImg);
		ptrHSE->setBarrierBg(eBarrierBg);
		ptrHSE->setSurplusNum(eSurplusNum);
		ptrHSE->setAddNum(eAddMenu);
		ptrHSE->setGoodsBg1(eGoodsbg[0]);
		ptrHSE->setGoodsBg2(eGoodsbg[1]);
		ptrHSE->setGoodsBg3(eGoodsbg[2]);
		mShowElem[i+1] = ptrHSE;
	}
}

void HeroicAmbitionLayer::AddGoodsElem(Sprite* GoodsBg, int iGoodsId, int iCount)
{
	if(iGoodsId == -1)
	{
		iGoodsId = 10002;
	}
	GoodsSprite* spItem = GoodsSprite::create(iGoodsId);
	spItem->setPosition(Vec2(GoodsBg->getContentSize().width / 2, GoodsBg->getContentSize().height / 2));
	GoodsBg->addChild(spItem);
}

void HeroicAmbitionLayer::updateMenuState()
{
	int iWarId = mShowElem[mShowElemIndex]->getWarId();
	int iAttackNum = 0, iAMaxNum = 0;
	if (history->mMapDateInfo->getHeroicMapDateElem(iWarId) != NULL)
	{
		iAttackNum = history->mMapDateInfo->getHeroicMapDateElem(iWarId)->getATTACKNUM();
	}
	iAMaxNum = trData->m_mNetWarInfo[iWarId]->iMaxTimes;
	bool bEnableBattle = (iAMaxNum - iAttackNum) > 0 ? true : false;

	if (mDateElem[mDateElemIndex]->getIsOpen() && bEnableBattle)
	{
		DispatchItem->setEnabled(true);
	}
	else
	{
		DispatchItem->setEnabled(false);
	}
	if (mDateElem[mDateElemIndex]->getIsPass())
	{
		SweepItem->setEnabled(true);
		SweepPrompt->setVisible(false);
	}
	else
	{
		SweepItem->setEnabled(false);
		SweepPrompt->setVisible(true);
	}

	if (!bEnableBattle)
	{
		SweepItem->setEnabled(false);
		mShowElem[mShowElemIndex]->getAddNum()->setVisible(true);
	}
	else
	{
		mShowElem[mShowElemIndex]->getAddNum()->setVisible(false);
	}
}

void HeroicAmbitionLayer::ShowAction(bool bIsNext)
{
	float fRunTime = 0.6f;
	int iDateEIndex = mDateElemIndex + (bIsNext ? -1 : 1);
	if (iDateEIndex < 0 || iDateEIndex >= (int)mDateElem.size())
		return;
	playSound(SOUND_LEFT_MOUSE_CLICK);
	mEnableClick = false;
	this->scheduleOnce(schedule_selector(HeroicAmbitionLayer::setEnableClick), fRunTime);

	int iUpdateShowElemIndex = (mShowElemIndex + 2) % 4;
	UpdateNextHeroicShow(mDateElemIndex + (bIsNext ? -2 : 2), iUpdateShowElemIndex);

	int iMoveX = 240 * (bIsNext ? 1 : -1);
	mShowElem[iUpdateShowElemIndex]->getBg()->setPosition(Vec2(mSize.width/2 - 2*iMoveX, mSize.height / 2+6));

	MoveBy* ptrMoveBy1 = MoveBy::create(fRunTime, Vec2(iMoveX, 18));
	MoveBy* ptrMoveBy2 = MoveBy::create(fRunTime, Vec2(iMoveX, -18));

	MoveBy* ptrMoveBy3 = MoveBy::create(fRunTime, Vec2(iMoveX, 0));
	MoveBy* ptrMoveBy4 = MoveBy::create(fRunTime, Vec2(iMoveX, 0));

	ScaleTo* ptrScaleTo1 = ScaleTo::create(fRunTime, 1);
	ScaleTo* ptrScaleTo2 = ScaleTo::create(fRunTime, 0.7f);


	if (bIsNext)
	{
		mShowElem[mShowElemIndex]->getBg()->runAction(ptrMoveBy2);
		mShowElem[mShowElemIndex]->getBg()->runAction(ptrScaleTo2);

		mShowElem[(mShowElemIndex + 1) % 4]->getBg()->runAction(ptrMoveBy3);

		mShowElem[(mShowElemIndex + 2) % 4]->getBg()->runAction(ptrMoveBy4);

		mShowElem[(mShowElemIndex + 3) % 4]->getBg()->runAction(ptrMoveBy1);
		mShowElem[(mShowElemIndex + 3) % 4]->getBg()->runAction(ptrScaleTo1);
	}
	else
	{
		mShowElem[mShowElemIndex]->getBg()->runAction(ptrMoveBy2);
		mShowElem[mShowElemIndex]->getBg()->runAction(ptrScaleTo2);

		mShowElem[(mShowElemIndex + 3) % 4]->getBg()->runAction(ptrMoveBy3);

		mShowElem[(mShowElemIndex + 2) % 4]->getBg()->runAction(ptrMoveBy4);

		mShowElem[(mShowElemIndex + 1) % 4]->getBg()->runAction(ptrMoveBy1);
		mShowElem[(mShowElemIndex + 1) % 4]->getBg()->runAction(ptrScaleTo1);
	}

	mDateElemIndex = iDateEIndex;
	mShowElem[mShowElemIndex]->getAddNum()->setEnabled(false);
	if (!bIsNext)
	{
		mShowElemIndex++;
		mShowElemIndex = mShowElemIndex % 4;
	}
	else
	{
		mShowElemIndex--;
		if (mShowElemIndex < 0)
			mShowElemIndex = 3;
	}
	mShowElem[mShowElemIndex]->getAddNum()->setEnabled(true);
	updateMenuState();
}

void HeroicAmbitionLayer::UpdateNextHeroicShow(int iDateEIndex, int iShowEIndex)
{
	if (iDateEIndex < 0 || iDateEIndex >= (int)mDateElem.size())
	{
		mShowElem[iShowEIndex]->setWarId(-1);
		mShowElem[iShowEIndex]->setDateIndex(-1);
		mShowElem[iShowEIndex]->getBg()->setVisible(false);
	}
	else
	{
		__String* eBg_pathCs = NULL;//背景路径
		__String* ePromptTxtCs = NULL;//开启条件
		__String* eActorImg_pathCs = NULL;//武将头像
		__String* eActorName_pathCs = NULL;//武将名图片
		__String* eSurplusNumCs = NULL;//剩余关卡
		map<int, map<int, int> > eGoodsList;
		bool bIsOpen = false;
		bool bIsVisibal = true;
		bool isFirst = false;
		int eWarId = -1;
		HeroicDateElemPtr ptrHDE = mDateElem[iDateEIndex];

		eBg_pathCs = __String::createWithFormat("new_ui/img_global/heroz_bg%d.png", ptrHDE->getIsOpen() != 0 ? 1 : 2);
		ePromptTxtCs = __String::createWithFormat(LL("tgztxddgkq"),trData->m_mNetWarInfo[ptrHDE->getOpenLevel()]->strName.c_str());
		eActorImg_pathCs = __String::createWithFormat("img/heroz/hero%d.png", ptrHDE->getWarId());
		eActorName_pathCs = __String::createWithFormat("img/heroz/hero_name%d.png", ptrHDE->getWarId());
		eSurplusNumCs = __String::createWithFormat("%d/%d", ptrHDE->getCurNum(), ptrHDE->getMaxNum());
		eGoodsList = ptrHDE->GoodsList;
		bIsOpen = ptrHDE->getIsOpen() != 0 ? true : false;
		isFirst = ptrHDE->getIsPass() != 0 ? false : true;
		eWarId = ptrHDE->getWarId();

		HeroicShowElemPtr ptrHShowE = mShowElem[iShowEIndex];

		mShowElem[iShowEIndex]->setWarId(ptrHDE->getWarId());
		mShowElem[iShowEIndex]->setDateIndex(iDateEIndex);

		ptrHShowE->getBg()->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(eBg_pathCs->getCString())));
		ptrHShowE->getActorIcon()->setTexture(ITools::getInstance()->getTextureWithPath(eActorImg_pathCs->getCString()));

		if (bIsOpen)
		{
			ptrHShowE->getActorIcon()->setColor(Color3B::WHITE);
			ptrHShowE->getActorName()->setTexture(ITools::getInstance()->getTextureWithPath(eActorName_pathCs->getCString()));
			ptrHShowE->getSurplusNum()->setString(eSurplusNumCs->getCString());
			ptrHShowE->getGoodsBg1()->setVisible(true);
			ptrHShowE->getGoodsBg2()->setVisible(true);
			ptrHShowE->getGoodsBg3()->setVisible(true);
			ptrHShowE->getGoodsBg1()->removeAllChildren();
			ptrHShowE->getGoodsBg2()->removeAllChildren();
			ptrHShowE->getGoodsBg3()->removeAllChildren();
			if (eGoodsList.count(0))
				AddGoodsElem(ptrHShowE->getGoodsBg1(), eGoodsList[0][0], eGoodsList[0][1]);
			if (eGoodsList.count(1))
				AddGoodsElem(ptrHShowE->getGoodsBg2(), eGoodsList[1][0], eGoodsList[1][1]);
			if (eGoodsList.count(2) && eGoodsList[2][1]>0)
				AddGoodsElem(ptrHShowE->getGoodsBg3(), eGoodsList[2][0], eGoodsList[2][1]);
		}
		else
		{
 			ptrHShowE->getGoodsBg1()->setVisible(false);
 			ptrHShowE->getGoodsBg2()->setVisible(false);
 			ptrHShowE->getGoodsBg3()->setVisible(false);
			ptrHShowE->getActorIcon()->setColor(Color3B::BLACK);
			ptrHShowE->getOpenPrompt()->setString(ePromptTxtCs->getCString());
		}

		ptrHShowE->getBg()->setVisible(true);
		if (bIsOpen)
		{
			ptrHShowE->getNameBg()->setVisible(true);
			ptrHShowE->getPromptBg()->setVisible(false);
			ptrHShowE->getBarrierBg()->setVisible(true);
			if (isFirst)
				ptrHShowE->getPromptImg()->setVisible(true);
			else
				ptrHShowE->getPromptImg()->setVisible(false);
		}
		else
		{
			ptrHShowE->getNameBg()->setVisible(false);
			ptrHShowE->getPromptBg()->setVisible(true);
			ptrHShowE->getBarrierBg()->setVisible(false);
			ptrHShowE->getPromptImg()->setVisible(false);
		}
	}
}

void HeroicAmbitionLayer::updateShow(int index)
{
	//mTitleBar->updateShow(index);
}
void HeroicAmbitionLayer::updateMoneyShow()
{
	//mTitleBar->updateShow(0);
}

void HeroicAmbitionLayer::onExit()
{
	Layer::onExit();
}

HeroicAmbitionLayer::HeroicAmbitionLayer()
{

}

HeroicAmbitionLayer::~HeroicAmbitionLayer()
{
	for (map<int, HeroicDateElemPtr>::iterator hdIt = mDateElem.begin(); hdIt != mDateElem.end(); hdIt++)
	{
		delete hdIt->second;
	}

	for (map<int, HeroicShowElemPtr>::iterator hsIt = mShowElem.begin(); hsIt != mShowElem.end(); hsIt++)
	{
		delete hsIt->second;
	}
}