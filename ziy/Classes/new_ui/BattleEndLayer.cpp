#include "BattleEndLayer.h"
#include "Defines.h"
#include "WinConditionTool.h"
#include "CocoMd5.h"
#include "MapDateInfo.h"
#include "GetStarLayer.h"
#include "BattleLadder.h"
#include "GetNetDateTool.h"
#include "PVPData.h"
#include "GuideLayer.h"
#include "StatConfig.h"
#include "LogoLayer.h"
enum
{
	REQ_RESULT_PVP = 0,
};

BattleEndLayer* BattleEndLayer::create(bool is_fail)
{
	BattleEndLayer* pRet = new BattleEndLayer();
	if (pRet&&pRet->init(is_fail))
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

bool BattleEndLayer::init(bool is_fail)
{
	if (!Layer::init())
	{
		return false;
	}
	Director::getInstance()->getScheduler()->setTimeScale(1.0f);
	get_star_num = 0;
	reach_conditions_num = 0;
	mScore = 0;
	m_bIsFail = is_fail;

	if (history->m_iBattleType == BATTLE_PVP)
	{
		onHttpRequest(REQ_RESULT_PVP);
		return true;
	}
	else if (is_fail)
	{
		SetFailShow();
		return true;
	}
	else if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
	{
		get_star_num = 1;
		InitSendDate();
		SendClanFBWinDate();
		return true;
	}
	else if (history->m_iBattleType != BATTLE_SIXWAR)
	{
		get_star_num = 1;
		InitSendDate();
		SendWinDate();
		return true;
	}

	NetWarInfo* ptrNetmw = trData->m_mNetWarInfo[history->m_iCurWarId];
	map<int,int> win_result = WinConditionTool::getInstance()->ReachConditionResult(true);
	int reach_condition = 0;
	for (map<int, int>::iterator it = win_result.begin(); it != win_result.end(); it++)
	{
		if (it->second > 0)
			reach_condition++;
	}
	reach_conditions_num = reach_condition;
    if (reach_condition >= ptrNetmw->mStarLimit[2])
        get_star_num=3;
    else if(reach_condition >= ptrNetmw->mStarLimit[1])
        get_star_num = 2;
    else if(reach_condition >= ptrNetmw->mStarLimit[0])
        get_star_num = 1;
    if (get_star_num < 1)
	{
		SetFailShow();
	}
	else
	{
		InitSendDate();
		SendWinDate();
	}

	return true;
}

void BattleEndLayer::SetWinShow()
{
	ChessPlayNetSound(MUSIC_BATTLE_WIN);
	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	int iOffsetX = 20;

	Armature* ptrArma = Armature::create("icon_eff");
	ptrArma->setPosition(Vec2(252+iOffsetX, 333));
	ptrArma->getAnimation()->play("war_tx1");
	this->addChild(ptrArma);

	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
		if (history->m_iCurWarId>=94&& history->m_iCurWarId<100)
		{
			string _Type = GuideLayer::IntToGuideString(history->m_iCurWarId-94);
			auto netTool = GetNetDateTool::getInstance();
			history->m_pPlayerInfo->setGuideStatus(_Type.c_str(), 1);
			netTool->setGuideStatusReq(_Type.c_str(), 1);
		}
		Sprite* star1 = ReadSpriteName("new_ui/battle/st1.png");
		star1->setPosition(Vec2(169 + iOffsetX, 333));
		this->addChild(star1);
		star1->setScale(0.1f);
		star1->setVisible(false);
		{
			DelayTime* ptrDelayTime = DelayTime::create(0.16f);
			CallFuncN* ptrCallFN = CallFuncN::create(CC_CALLBACK_1(BattleEndLayer::StarActionCallF, this));
			ScaleTo* ptrScaleTo = ScaleTo::create(0.16f, 1.0f);
			Sequence* ptrSequence = Sequence::create(ptrDelayTime, ptrCallFN, ptrScaleTo, NULL);
			star1->runAction(ptrSequence);
		}

		Sprite* star2 = ReadSpriteName("new_ui/battle/st2.png");
		star2->setPosition(Vec2(252 + iOffsetX, 344));
		this->addChild(star2);
		if (get_star_num < 2)
			addSpriteGray(star2);
		star2->setScale(0.1f);
		star2->setVisible(false);
		{
			DelayTime* ptrDelayTime = DelayTime::create(0.48f);
			CallFuncN* ptrCallFN = CallFuncN::create(CC_CALLBACK_1(BattleEndLayer::StarActionCallF, this));
			ScaleTo* ptrScaleTo = ScaleTo::create(0.16f, 1.0f);
			Sequence* ptrSequence = Sequence::create(ptrDelayTime, ptrCallFN, ptrScaleTo, NULL);
			star2->runAction(ptrSequence);
		}

		Sprite* star3 = ReadSpriteName("new_ui/battle/st1.png");
		star3->setPosition(Vec2(335 + iOffsetX, 333));
		star3->setFlippedX(true);
		this->addChild(star3);
		if (get_star_num < 3)
			addSpriteGray(star3);
		star3->setScale(0.1f);
		star3->setVisible(false);
		{
			DelayTime* ptrDelayTime = DelayTime::create(0.8f);
			CallFuncN* ptrCallFN = CallFuncN::create(CC_CALLBACK_1(BattleEndLayer::StarActionCallF, this));
			ScaleTo* ptrScaleTo = ScaleTo::create(0.16f, 1.0f);
			Sequence* ptrSequence = Sequence::create(ptrDelayTime, ptrCallFN, ptrScaleTo, NULL);
			star3->runAction(ptrSequence);
		}
	}
	else if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
	{
		Sprite* winSp = ReadSpriteName("new_ui/battle/war_sl2.png");
		winSp->setPosition(Vec2(252 + iOffsetX, 344));
		this->addChild(winSp);
	}
	else
	{
		Sprite* winSp = ReadSpriteName("new_ui/battle/war_sl1.png");
		winSp->setPosition(Vec2(252 + iOffsetX, 344));
		this->addChild(winSp);
	}

	Sprite* info_bg = ReadSpriteName("new_ui/battle/result_bg1.png");
	info_bg->setPosition(Vec2(252 + iOffsetX,281));
	this->addChild(info_bg);

	if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
	{
		__String* armyCs = __String::createWithFormat("%s%d", LL("dslsx"), history->mArmyFuBenBattle->GetAllEnemyGainHp());
		Label* army_lv = LabelSystemFont(armyCs->getCString(), 16);
		army_lv->setAnchorPoint(Vec2(0, 0.5));
		army_lv->setPosition(Vec2(46, 35));
		army_lv->setColor(Color3B(153, 51, 0));
		info_bg->addChild(army_lv);
	}
	else
	{
		__String* armyCs = __String::createWithFormat("%s%d", LL("bencijifen"), (int)mScore);
		Label* army_lv = LabelSystemFont(armyCs->getCString(), 16);
		army_lv->setAnchorPoint(Vec2(0, 0.5));
		army_lv->setPosition(Vec2(46, 35));
		army_lv->setColor(Color3B(153, 51, 0));
		info_bg->addChild(army_lv);

		Sprite* gold_img = ReadSpriteName("new_ui/img_global/icon_gb.png");
		gold_img->setPosition(Vec2(216, 35));
		info_bg->addChild(gold_img);

		int iGetGold = 0;
		if (mEnableGetGold)
		{
			iGetGold = trData->m_mNetWarInfo[history->m_iCurWarId]->iGold;
		}

		Label* gold_num = LabelSystemFont(IntToString_Sign(iGetGold), 16);
		gold_num->setAnchorPoint(Vec2(0, 0.5));
		gold_num->setColor(Color3B(153, 51, 0));
		gold_num->setPosition(Vec2(216 + gold_img->getContentSize().width, 35));
		info_bg->addChild(gold_num);
	}

	AddActorElem();

	Sprite* lineSp = ReadSpriteName("new_ui/battle/battle_line3.png");
	lineSp->setAnchorPoint(Vec2(0, 1));
	lineSp->setPosition(Vec2(59 + iOffsetX,89));
	this->addChild(lineSp);

	AddGoodsElem();


	Menu* backM = Menu::create();
	backM->setPosition(Vec2(592, 55));
	this->addChild(backM);

	Sprite* back_n = ReadSpriteName("new_ui/battle/battle_xyb1.png");
	Sprite* back_s = ReadSpriteName("new_ui/battle/battle_xyb1.png");
	back_s->setScale(0.9f);
	MenuItemSprite* back_item = MenuItemSprite::create(back_n, back_s, CC_CALLBACK_1(BattleEndLayer::clickBackMenu, this));
	back_s->setPosition(Vec2(back_s->getContentSize().width / 2, back_s->getContentSize().height / 2));
	back_s->setAnchorPoint(Vec2(0.5, 0.5));
	backM->addChild(back_item);

	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
		Menu* GetStarM = Menu::create();
		GetStarM->setPosition(Vec2(592, 130));
		this->addChild(GetStarM);
		Sprite* GetStarN = ReadSpriteName("new_ui/icon_ui/icon2_7.png");
		Sprite* GetStarS = ReadSpriteName("new_ui/icon_ui/icon2_7.png");
		GetStarS->setScale(0.9f);
		MenuItemSprite* GetStarItem = MenuItemSprite::create(GetStarN, GetStarS, CC_CALLBACK_1(BattleEndLayer::clickGetStarMenu, this));
		GetStarS->setPosition(Vec2(GetStarS->getContentSize().width / 2, GetStarS->getContentSize().height / 2));
		GetStarS->setAnchorPoint(Vec2(0.5, 0.5));
		GetStarM->addChild(GetStarItem);
	}
}

void BattleEndLayer::StarActionCallF(Node* sender)
{
	sender->setVisible(true);
}

void BattleEndLayer::AddActorElem()
{
	int iGetExp = trData->m_mNetWarInfo[history->m_iCurWarId]->iExp;
	bool bEnableGetExp = false;
	if(mEnableGetExp && history->m_iBattleType == BATTLE_SIXWAR)
	{
		MapDateElem* ptrMapDate = history->mMapDateInfo->getMapDateElem(history->m_iCurWarId);
		if(ptrMapDate)
		{
			bEnableGetExp = true;
		}
	}
	else if (history->m_iBattleType == BATTLE_HEROIC)
	{
		bEnableGetExp = true;
	}
	else if ((history->m_iBattleType == BATTLE_ROMANCE || history->m_iBattleType == BATTLE_LADDER || history->m_iBattleType == BATTLE_WOLFCITY) && !iGetExp)
	{
		bEnableGetExp = false;
	}
	DVector<int> _actorsShow;
	for (int i = 0; i < 12; i++)
	{
		Sprite* img_bg = ReadSpriteName("new_ui/battle/battle_bg2.png");
		img_bg->setPosition(Vec2(72 + _actorsShow.size() % 6 * 80, 206 - _actorsShow.size() / 6 * 80));
		this->addChild(img_bg);

		if (i < history->battleOwnNum)
		{
			HistoryActorPtr ptrActor = history->battleOwn[i];
			if (_actorsShow.contains(ptrActor->actor_id))
			{
				continue;
			}
			//时装
			_actorsShow.addElement(ptrActor->actor_id);

			__String* f_path = NULL;
			__String* img_path = NULL;
			if (ptrActor->mFashion[0])
			{
				f_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);
				img_path = __String::createWithFormat("img/Nunits/%d_%d_spc.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
				Sprite* actor_fashion = ITools::getInstance()->getSpriteWithPath(f_path->getCString());
				int fashion_width = actor_fashion->getContentSize().width;
				actor_fashion->setTextureRect(Rect(0, fashion_width * 4, fashion_width, fashion_width));
				actor_fashion->setPosition(Vec2(img_bg->getContentSize().width / 2, img_bg->getContentSize().height / 2+24));
				img_bg->addChild(actor_fashion,trData->net_hero_dress[ptrActor->mFashion[4]]->mSpc[4]);
			}
			else
			{
				img_path = __String::create(ptrActor->getActorSImgPath("spc", 0));
			}

			//武将
			Sprite* actor_img = ITools::getInstance()->getSpriteWithPath(img_path->getCString());
			int img_width = actor_img->getContentSize().width;
			actor_img->setTextureRect(Rect(0, img_width * 4, img_width, img_width));
			actor_img->setPosition(Vec2(img_bg->getContentSize().width / 2, img_bg->getContentSize().height / 2+24));
			img_bg->addChild(actor_img);

			int actor_get_exp = -1;
			NetHeroLvUpExpPtr ptrNHLUE = trData->net_hero_exp[ptrActor->GetActorLv()];

			if (ptrActor->exp != ptrNHLUE->mExp)
			{
				actor_get_exp = iGetExp;
			}

			if (!bEnableGetExp)
			{
				Sprite* expNull = ReadSpriteName("new_ui/hero/hero_exp_txt2.png");
				expNull->setPosition(Vec2(img_bg->getContentSize().width / 2, -9));
				img_bg->addChild(expNull);
			}
			else if (actor_get_exp > -1)
			{
				Label* exp_ttf = LabelSystemFont(__String::createWithFormat("%s+%d",LL("jingyan"), actor_get_exp)->getCString(), 14);
				exp_ttf->setColor(Color3B::GREEN);
				exp_ttf->setPosition(Vec2(img_bg->getContentSize().width / 2, -9));
				img_bg->addChild(exp_ttf);
			}
			else
			{
				Sprite* expFull = ReadSpriteName("new_ui/hero/hero_exp_txt1.png");
				expFull->setPosition(Vec2(img_bg->getContentSize().width / 2, -9));
				img_bg->addChild(expFull);
			}



			Sprite* lv_bg = ReadSpriteName("new_ui/battle/result_lv_bg.png");
			lv_bg->setAnchorPoint(Vec2(0, 0));
			lv_bg->setPosition(Vec2(-4,0));
			img_bg->addChild(lv_bg,2);

			Label* lv_txt = LabelSystemFont(IntToString(ptrActor->GetActorLv()), 16);
			lv_txt->setPosition(Vec2(lv_bg->getContentSize().width / 2, lv_bg->getContentSize().height / 2));
			lv_bg->addChild(lv_txt);

			if (mEnableGetExp&&mActorLvUp.find(ptrActor->actor_id + 1) != mActorLvUp.end())
			{
				Armature* lv_up = Armature::create("icon_eff");
				lv_up->setPosition(Vec2(img_bg->getContentSize().width / 2, img_bg->getContentSize().height / 2));
				lv_up->getAnimation()->play("war_tx2");
				img_bg->addChild(lv_up);
			}
		}
		else
		{
			addSpriteGray(img_bg);
		}
	}
}

void BattleEndLayer::AddGoodsElem()
{
	int iIndex = 0;
	for (map<int, int>::iterator it = mDropGoods.begin(); it != mDropGoods.end(); it++)
	{
		//71,27
		Sprite* bg = ReadSpriteName("new_ui/img_global/item_box_bg.png");
		bg->setAnchorPoint(Vec2(0, 0));
		bg->setPosition(Vec2(71 + iIndex*(bg->getContentSize().width + 6), 27));
		this->addChild(bg);
		iIndex++;

		int iGoodsId = it->first;
		int iCount = it->second;
		int bg_width = bg->getContentSize().width;
		int bg_height = bg->getContentSize().height;

		Sprite* img_goods = history->getGoodsSprite(iGoodsId);

		img_goods->setScale(0.8f);
		img_goods->setPosition(Vec2(bg_width / 2, bg_height / 2));
		bg->addChild(img_goods);

		if (iGoodsId<10000&&trData->m_mGoodsItem[iGoodsId]->iItId == 98)
		{
			Sprite* chip_img = ReadSpriteName("new_ui/img_global/icon_sp.png");
			chip_img->setAnchorPoint(Vec2(0, 1));
			chip_img->setScale(0.8f);
			chip_img->setPosition(Vec2(0, bg_height));
			bg->addChild(chip_img);
		}

		if (iGoodsId<10000 && trData->m_mGoodsItem[iGoodsId]->iItId == 99)
		{
			Sprite* chip_img = ReadSpriteName("new_ui/hero/icon_soul.png");
			chip_img->setAnchorPoint(Vec2(0, 1));
			chip_img->setScale(0.8f);
			chip_img->setPosition(Vec2(0, bg_height));
			bg->addChild(chip_img);
		}

		if (iCount > 1)
		{
			Sprite* numSp = ITools::getInstance()->getNumSprite(iCount, true);
			numSp->setAnchorPoint(Vec2(1, 0));
			numSp->setPosition(Vec2(bg_width,0));
			bg->addChild(numSp);
		}
	}
}

void BattleEndLayer::SetFailShow()
{
	ChessPlayNetSound(MUSIC_BATTLE_FAIL);
	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2(0, 0));
	this->addChild(black_layer);

	//253,291
	Sprite* tzsb = ReadSpriteName("new_ui/battle/war_sb1.png");
	tzsb->setPosition(Vec2(253, 291));
	this->addChild(tzsb);

	//136,219
	Sprite* ptrPrompt = ReadSpriteName("new_ui/battle/war_sb2.png");
	ptrPrompt->setAnchorPoint(Vec2(0,1));
	ptrPrompt->setPosition(Vec2(136,226));
	this->addChild(ptrPrompt);

	Menu* backM = Menu::create();
	backM->setPosition(Vec2(592,89));
	this->addChild(backM);

	Sprite* back_n = ReadSpriteName("new_ui/battle/battle_xyb1.png");
	Sprite* back_s = ReadSpriteName("new_ui/battle/battle_xyb1.png");
	back_s->setScale(0.9f);
	MenuItemSprite* back_item = MenuItemSprite::create(back_n, back_s, CC_CALLBACK_1(BattleEndLayer::clickBackMenu, this));
	back_s->setPosition(Vec2(back_s->getContentSize().width / 2, back_s->getContentSize().height / 2));
	back_s->setAnchorPoint(Vec2(0.5,0.5));
	backM->addChild(back_item);
}

void BattleEndLayer::clickBackMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	
	
	history->is_battle_win = false;
	if(!m_bIsFail)
	{
		int iOpenId = trData->getOpenHeroicId(history->m_iCurWarId);
		if(iOpenId && history->m_pLoginInfo->getHeroicOpen(iOpenId) == 0)
		{
			history->m_iOpenId = iOpenId;
			history->m_pLoginInfo->setHeroicOpen(iOpenId);
		}
	}
	this->removeFromParent();
}

void BattleEndLayer::clickGetStarMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GetStarLayer::create(history->m_iCurWarId, 3));
}

BattleEndLayer::BattleEndLayer()
{

}

BattleEndLayer::~BattleEndLayer()
{
	
}


////网络数据

void BattleEndLayer::InitSendDate()
{
	Json::Value HeroIdList(arrayValue);
	for (int i = 0; i < history->battleOwnNum; i++)
	{
		HeroIdList[i] = history->battleOwn[i]->actor_id + 1;
	}
	JsonSendDate["heroes"] = HeroIdList;

	Json::Value ConsumesList(arrayValue);

	map<int, int> ConsumesListDate = WinConditionTool::getInstance()->GetConsumesList();
	for (map<int, int>::iterator it = ConsumesListDate.begin(); it != ConsumesListDate.end(); it++)
	{
		Json::Value ConsumesListElem;
		ConsumesListElem["id"] = history->m_mConsuNetId[it->first];
		ConsumesListElem["count"] = it->second;
		ConsumesList.append(ConsumesListElem);
	}

	JsonSendDate["consumes"] = ConsumesList;

	Json::Value ScriptCode(arrayValue);
	map<int,int> ScriptCodeList = WinConditionTool::getInstance()->GetScriptCodeList();
	int iListIndex = 0;
	for (map<int, int>::iterator it = ScriptCodeList.begin(); it != ScriptCodeList.end(); it++)
	{
		ScriptCode[iListIndex++] = it->first;
	}
	JsonSendDate["scriptCode"] = ScriptCode;


	int surplus_round = trData->m_mNetWarInfo[history->m_iCurWarId]->iRound - history->GetCurRound();
    int Enemy_level = 0;
    int n =1;
    for (int i = 0; i < history->battleEnemyNum; i++,n++)
    {
		Enemy_level += history->battleEnemy[i]->GetActorLv();
    }
    int EnemyLV_AVG = Enemy_level / n;
    
    int fullMarks = 1000;
    
    int ownLV_Score = fullMarks * 0.1;//我军等级加成   每多一个人物>敌军等级平均等级 －10分
    int noDeath = 0;//敌军没死亡人数
    for (int i = 0; i < history->battleOwnNum; i++)
    {
		if (history->battleOwn[i]->GetActorLv() > EnemyLV_AVG)
            ownLV_Score-=10;
    }
    
    for(int i = 0; i < history->battleEnemyNum; i++)
    {
		if (history->battleEnemy[i]->GetCurHp() >0)
            noDeath++;
    }
    
    if (history->m_iBattleType == BATTLE_SIXWAR)
    {
        //mScore = reach_conditions_num * 10 + surplus_round * 5 + Enemy_level - own_level;
        if(ownLV_Score<0)
            ownLV_Score = 0;
        
        float enemyDeathScore = fullMarks * 0.2 / history->battleEnemyNum;
        float roundScore = fullMarks * 0.4/trData->m_mNetWarInfo[history->m_iCurWarId]->iRound;
        
        int win_conditions = history->net_win_condition.size();//总条件数

        mScore = (reach_conditions_num * fullMarks * 0.3 /win_conditions) + (surplus_round * roundScore) + ownLV_Score + (fullMarks * 0.2 - (noDeath * enemyDeathScore));
    }
	else if (history->m_iBattleType == BATTLE_LADDER)
	{
		ownLV_Score += fullMarks * 0.1;
		if (ownLV_Score < 0)
			ownLV_Score = 0;

		map<int, int> SeckillEnemy = WinConditionTool::getInstance()->GetSeckillEnemy();

		float roundScore = fullMarks * 0.5 / trData->m_mNetWarInfo[history->m_iCurWarId]->iRound;

		float enemyDeathScore = fullMarks * 0.15 / history->battleEnemyNum;

		mScore = surplus_round * roundScore + ownLV_Score+ (fullMarks * 0.15 - (noDeath * enemyDeathScore)) + SeckillEnemy.size() * enemyDeathScore;
	}
	else
		mScore = 0;

	if (mScore < 0)
		mScore = 0;
}

void BattleEndLayer::SendWinDate()
{
	//数据
	FastWriter fastWriter;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	int iMapIndex = MAPID_SCENE_HEROIC;
	int iWarId = history->m_iCurWarId;
	int iScore = mScore;
	int iGetStar = 0;
	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
		iMapIndex = trData->m_mNetWarInfo[history->m_iCurWarId]->iMapIndex;
		iGetStar = get_star_num;
	}
	else if (history->m_iBattleType == BATTLE_HEROIC)
	{
		iMapIndex = MAPID_SCENE_HEROIC;
	}
	else if(history->m_iBattleType == BATTLE_ACTSCENE)
	{
		iMapIndex = MAPID_SCENE_ACTIVITY;
	}
	else if (history->m_iBattleType == BATTLE_ROMANCE||history->m_iBattleType == BATTLE_WOLFCITY)
	{
		iMapIndex = trData->m_mNetWarInfo[history->m_iCurWarId]->iMapIndex;
		iGetStar = 1;
	}
	else if (history->m_iBattleType == BATTLE_IMAGE)
	{
		iMapIndex = MAPID_SCENE_IMAGE;
		iGetStar = history->mMapDateInfo->GetBattleImageData()->GetBattleStar(history->m_iCurWarId,history->m_iBattleStage);
	}
	else
	{
		iMapIndex = trData->m_mNetWarInfo[history->m_iCurWarId]->iMapIndex;
	}

	if (trData->m_mNetWarInfo[history->m_iCurWarId]->iStage)
	{
		iWarId = iWarId * 100 + history->m_iBattleStage;
	}

	string heroesS = fastWriter.write(JsonSendDate["heroes"]);
	heroesS.erase(heroesS.end() - 1);
	string consumesS = fastWriter.write(JsonSendDate["consumes"]);
	consumesS.erase(consumesS.end() - 1);
	string scriptCodeS = fastWriter.write(JsonSendDate["scriptCode"]);
	scriptCodeS.erase(scriptCodeS.end() - 1);

	__String* validateCs = __String::createWithFormat("gameToken=%s&mapID=%d&warID=%d&star=%d&score=%d&heroes=%s&consumes=%s&scriptCode=%s",
		GameTokenS.c_str(), iMapIndex, iWarId, iGetStar, iScore, heroesS.c_str(), consumesS.c_str(),scriptCodeS.c_str());

	CocosMD5 md5;
	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"mapID\":%d,\"warID\":%d,\"star\":%d,\"score\":%d,\"heroes\":%s,\"consumes\":%s,\"scriptCode\":%s,\"validateCode\":\"%s\"}", 
		GameTokenS.c_str(), iMapIndex, iWarId, iGetStar, iScore, heroesS.c_str(), consumesS.c_str(), scriptCodeS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("warEnd",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(BattleEndLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendNomalWinDate",
		true);

	ITools::getInstance()->addWaitLayer();
}

void BattleEndLayer::SendClanFBWinDate()
{
	//数据
	FastWriter fastWriter;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	int iWarId = history->m_iCurWarId;

	int iHurtHp = history->mArmyFuBenBattle->GetAllEnemyGainHp();

	string consumesS = fastWriter.write(JsonSendDate["consumes"]);
	consumesS.erase(consumesS.end() - 1);

	__String* validateCs = __String::createWithFormat("warID=%d&hurt=%d&consumes=%s&gameToken=%s",iWarId, iHurtHp, consumesS.c_str(),GameTokenS.c_str());

	CocosMD5 md5;
	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"warID\":%d,\"hurt\":%d,\"consumes\":%s,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}",iWarId, iHurtHp, consumesS.c_str(), GameTokenS.c_str(),validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("guildWarEnd",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(BattleEndLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendClanFBWinDate",
		true);

	ITools::getInstance()->addWaitLayer();
}

void BattleEndLayer::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch (iReqType)
	{
	case REQ_RESULT_PVP:
	{
		strURL = "finishPVP";
		string strMark = PVPData::getInstance()->getMarkID();
		int iIsTrue = m_bIsFail ? 0 : 1;
		string validCode = md5.ToMD5(StringUtils::format("tid=%s&isTrue=%d&gameToken=%s", strMark.c_str(), iIsTrue, strToken.c_str()));
		strData = StringUtils::format("{\"tid\":\"%s\",\"isTrue\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strMark.c_str(), iIsTrue, strToken.c_str(), validCode.c_str());
		strTag = "REQ_RESULT_PVP";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(BattleEndLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void BattleEndLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	int iResCode = response->getResponseCode();
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (strTag == "REQ_RESULT_PVP")
		{
			onPVPResultSucceed(value["result"]);
		}
		else
		{
			if (iResCode == 200 || iResCode == 201)
			{
				LoadBattleWinDate(value["result"]);
				SetWinShow();
				return;
			}
			else
			{
				history->parseErrorMsg(value, strTag);
				if (!value["code"].isNull() && value["code"].asInt() != 124)
				{
					SetFailShow();
					WinConditionTool::getInstance()->BattleFailedSetDate();//战斗失败退还物品等
				}
			}
		}
	}
}

void BattleEndLayer::onPVPResultSucceed(Json::Value valRes)
{
	if (!m_bIsFail && valRes["isTrue"].isInt() && valRes["isTrue"].asInt())
	{
		SetWinShow();
	}
	else
	{
		if (valRes["message"].isString())
		{
			ITools::getInstance()->createGlobalPrompt(valRes["message"].asString(), false);
		}
		SetFailShow();
	}
}

void BattleEndLayer::LoadBattleWinDate(Json::Value JsonDate)
{
	mEnableGetExp = true;
	mEnableGetGold = true;

	//胜利更新地图信息
	if (history->m_iBattleType == BATTLE_SIXWAR)
	{
		NetWarInfo* ptrNMWP = trData->m_mNetWarInfo[history->m_iCurWarId];
		history->mMapDateInfo->AddBattleWinDate(ptrNMWP->iMapIndex, ptrNMWP->iWarId, get_star_num, mScore);
	}
	else
	{
		int iWarId = history->m_iCurWarId;
		if (trData->m_mNetWarInfo[history->m_iCurWarId]->iStage)
		{
			iWarId = iWarId * 100 + history->m_iBattleStage;
		}
		history->mMapDateInfo->AddHeroicBattleWinDate(iWarId);
	}

	if (history->m_iBattleType == BATTLE_LADDER)
	{
		BattleLadder::getInstance()->SetLadderLevelData(history->m_iCurWarId);
	}

	if (history->m_iBattleType == BATTLE_WOLFCITY)
	{
		history->mMapDateInfo->GetBattleWolfData()->SetWolfBattleData(history->m_iCurWarId);
	}

	if (history->m_iBattleType == BATTLE_IMAGE)
	{
		history->mMapDateInfo->GetBattleImageData()->SetBattleData(history->m_iCurWarId);
	}

	if (history->m_iBattleType == BATTLE_GREATBATTLE)
	{
		history->mMapDateInfo->GetBattleGreatData()->SetBattleData(history->m_iCurWarId);
	}

	if (!JsonDate["consumes"].isNull())
	{
		Json::Value JsonConsumes = JsonDate["consumes"];
		if (JsonConsumes.isArray())
		{
			for (UInt i = 0; i < JsonConsumes.size(); i++)
			{
				int iGoodsId = 0, iCount = 0, iNetId = 0;
				if (JsonConsumes[i]["id"].isInt())
					iNetId = JsonConsumes[i]["id"].asInt();
				if (JsonConsumes[i]["goodID"].isInt())
					iGoodsId = JsonConsumes[i]["goodID"].asInt();
				if (JsonConsumes[i]["count"].isInt())
					iCount = JsonConsumes[i]["count"].asInt();

				history->m_mConsuNum[iGoodsId] = iCount;
				history->m_mConsuNetId[iGoodsId] = iNetId;
			}
		}
	}

	mActorLvUp.clear();
	map<int, int> mNetScriptHeroList;
	map<int, int> ScriptActorElem = WinConditionTool::getInstance()->GetScriptActorList();
	if (!JsonDate["hero"].isNull())
	{
		Json::Value JsonHero = JsonDate["hero"];
		if (JsonHero.isArray())
		{
			for (UInt i = 0; i < JsonHero.size(); i++)
			{
				int iHeroId = JsonHero[i]["heroID"].asInt();
				if (!ScriptActorElem.count(iHeroId))
				{
					int iLv = JsonHero[i]["lv"].asInt();
					int iEXP = JsonHero[i]["EXP"].asInt();

					HistoryActorPtr ptrEActor = &history->actors[iHeroId - 1];

					int iActorOldQuality = ptrEActor->actor_old_quality;
					if (iActorOldQuality > 0)
					{
						ptrEActor->actor_quality = iActorOldQuality;
						ptrEActor->actor_old_quality = 0;
					}

					int iCurLevel = ptrEActor->GetActorLv();
					int iOldLevel = ptrEActor->GetActorOldLv();
					if (iOldLevel > 0)
						iCurLevel = iOldLevel;
					ptrEActor->SetActorLv(iLv);
					ptrEActor->exp = iEXP;

					bool bEnableRef = true;
					if (iCurLevel < iLv)
					{
						ptrEActor->InitFiveAllAbility();
						mActorLvUp[iHeroId] = 1;
						if (iHeroId==1)
						{
							HeroElem* mainHero = history->m_pPlayerInfo->getMainHero();
							if (mainHero) {
								mainHero->setLV(iLv);
							}
							StatConfig::submitExtendData(LogoLayer::getDataForSDKAnalys("levelChange", iLv).data());
						}
						ptrEActor->UpdataGoodEffect();
						bEnableRef = false;
					}
					if (bEnableRef&&iOldLevel > 0)
					{
						ptrEActor->InitFiveAllAbility();
						ptrEActor->UpdataGoodEffect();
					}
					ptrEActor->UpdateSkills();
					ptrEActor->SetActorOldLv(0);
				}
				else
				{
					int iOwnListIndex = ScriptActorElem[iHeroId];
					for (int j = iOwnListIndex; j < history->ownArmyNum-1; j++)
					{
						history->ownArmy[j] = history->ownArmy[j + 1];
					}
					history->ownArmyNum--;

					HistoryActorPtr CurActor = &history->actors[iHeroId - 1];
					CurActor->init(iHeroId - 1);
					HeroElem InitHeroE;
					InitHeroE.loadJson(JsonHero[i]);
					history->initNetHeroElem(&InitHeroE);
					//history->popOpenNotice(1);
					if (history->actors[iHeroId - 1].GetActorLv() > 1)
					{
						mActorLvUp[iHeroId] = 1;
					}
					mNetScriptHeroList[iHeroId] = 1;
					history->actors[iHeroId - 1].SetActorOldLv(0);
					history->actors[iHeroId - 1].actor_old_quality = 0;
				}
			}
		}
	}

	if (mNetScriptHeroList.size() != ScriptActorElem.size())
	{
		for (map<int, int>::iterator mIt = ScriptActorElem.begin(); mIt != ScriptActorElem.end(); mIt++)
		{
			if (!mNetScriptHeroList.count(mIt->first))
			{
				int iOwnListIndex = mIt->second;
				for (int j = iOwnListIndex; j < history->ownArmyNum - 1; j++)
				{
					history->ownArmy[j] = history->ownArmy[j + 1];
				}
				history->ownArmyNum--;

				for (int i = 0; i < history->battleOwnNum; i++)
				{
					if (mIt->first == history->battleOwn[i]->actor_id + 1)
					{
						history->battleOwn[i]->recoverState();
						if (history->battleOwn[i]->controlled_by_enemy)
							history->battleOwn[i]->delete_controlled_by_enemy();
						else
						{
							for (int j = i; j < history->battleOwnNum - 1; j++)
							{
								history->battleOwn[j] = history->battleOwn[j + 1];
							}
							history->battleOwn[history->battleOwnNum - 1] = NULL;
							history->battleOwnNum--;
						}

						i--;
					}
				}
			}
		}
	}

	if (!JsonDate["gold"].isNull())
	{
		if (JsonDate["gold"].isInt())
			history->money = JsonDate["gold"].asInt();
	}
	else
	{
		mEnableGetGold = false;
	}
	if (JsonDate["jade"].isInt())
	{
		history->m_iJade = JsonDate["jade"].asInt();
	}
	if (JsonDate["improvedStone"].isInt())
	{
		history->stone = JsonDate["improvedStone"].asInt();
	}
	if (JsonDate["jadeSeal"].isInt())
	{
		history->m_iJadeSeal = JsonDate["jadeSeal"].asInt();
	}
	if (JsonDate["hasExp"].isBool())
	{
		mEnableGetExp = JsonDate["hasExp"].asBool();
	}
	mDropGoods.clear();
	if (!JsonDate["goods"].isNull())
	{
		Json::Value JsonGoods = JsonDate["goods"];
		if (JsonGoods.isArray())
		{
			for (UInt i = 0; i < JsonGoods.size(); i++)
			{
				AddOneGoods(JsonGoods[i]);
			}
		}
	}
	if (!JsonDate["scriptGoods"].isNull())
	{
		Json::Value JsonGoods = JsonDate["scriptGoods"];
		if (JsonGoods.isArray())
		{
			for (UInt i = 0; i < JsonGoods.size(); i++)
			{
				AddOneGoods(JsonGoods[i]);
			}
		}
	}

	if (!JsonDate["heroGoods"].isNull())
	{
		Json::Value jsonGoods = JsonDate["heroGoods"];
		for (int i = 0; i < (int)jsonGoods.size(); i++)
		{
			GoodsElem tempGoogsItem;
			tempGoogsItem.loadJson(jsonGoods[i]);
			HistoryActorPtr ptrActor = &history->actors[tempGoogsItem.getHeroID() - 1];
			HistoryGoods hisGoods(&tempGoogsItem);
			if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
			{
				ptrActor->weapon.setGood(&hisGoods);
			}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
			{
				ptrActor->coat.setGood(&hisGoods);
			}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
			{
				ptrActor->assist_good.setGood(&hisGoods);
			}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
			{
				ptrActor->fashion.setGood(&hisGoods);
			}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
			{
				ptrActor->wap_fashion.setGood(&hisGoods);
			}
			ptrActor->UpdataGoodEffect();
		}
	}

	if (!JsonDate["openWarID"].isNull())
	{
		Json::Value jsonOpenWar = JsonDate["openWarID"];
		for (UInt i = 0; i < jsonOpenWar.size(); i++)
		{
			int iWarId = jsonOpenWar[i].asInt();
			if (trData->m_mNetWarInfo.count(iWarId) && trData->m_mNetWarInfo[iWarId]->iType == BATTLE_ROMANCE)
			{
				history->mRomanceOpenData[trData->m_mNetWarInfo[iWarId]->iMapIndex][iWarId] = 1;
			}
			history->popOpenNotice(iWarId);
		}
	}



	map<int, int> ScriptCodeList = WinConditionTool::getInstance()->GetScriptCodeList();//战场变量
	for (map<int, int>::iterator it = ScriptCodeList.begin(); it != ScriptCodeList.end(); it++)
	{
		if (it->first >= 5000)
		{
			history->m_mScriptCode[GLOBAL_VAR_MAPID][it->first] = 1;
		}
		else
		{
			history->m_mScriptCode[history->m_iCurWarId][it->first] = 1;
		}
	}

}

void BattleEndLayer::AddOneGoods(Json::Value JsonGoods)
{
	int iGoodsId = JsonGoods["goodID"].asInt();
	int iId = JsonGoods["id"].asInt();
	int iCount = JsonGoods["count"].asInt();
	mDropGoods[iGoodsId] += iCount;
	history->packGoodsAdd(iGoodsId, iId, iCount, false);
}