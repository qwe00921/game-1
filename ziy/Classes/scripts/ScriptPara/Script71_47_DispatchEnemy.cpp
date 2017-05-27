#include "Common.h"
#include "Scripts.h"
#include "BattleLadder.h"
#include "PVPData.h"

int ScriptDispatchEnemy::HandleScript()
{
	if (history->m_iBattleType == BATTLE_PVP)
	{
		initPVPEnemyData();
		return 0;
	}

	int iOwnAvgLv = 0;
	int iEnemyStageLv = 0;
	int mBattleMaxStage = trData->m_mNetWarInfo[history->m_iCurWarId]->iStage;

	if (trData->m_mNetWarInfo[history->m_iCurWarId]->iGkLv == 4 && !mBattleMaxStage)
	{
		map<int, int> mOwnAvgLv;
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			mOwnAvgLv[i] = history->battleOwn[i]->GetActorLv();
		}

		int iTemp = 0;
		for (int i = 0; i < (int)mOwnAvgLv.size(); i++)
		{
			for (int j = mOwnAvgLv.size() - 1; j > i; j--)
			{
				if (mOwnAvgLv[i] < mOwnAvgLv[j])
				{
					iTemp = mOwnAvgLv[i];
					mOwnAvgLv[i] = mOwnAvgLv[j];
					mOwnAvgLv[j] = iTemp;
				}
			}
		}

		if ((int)mOwnAvgLv.size() > 2)
		{
			iOwnAvgLv = (mOwnAvgLv[0] + mOwnAvgLv[1] + mOwnAvgLv[2]) / 3;
		}
		else
		{
			int iNum = 1;
			for (int i = 0; i < (int)mOwnAvgLv.size(); i++)
			{
				iOwnAvgLv += mOwnAvgLv[i];
				iNum = i + 1;
			}

			iOwnAvgLv = iOwnAvgLv / iNum;

		}

		if (mOwnAvgLv[0] > iOwnAvgLv + 20)
			iOwnAvgLv = mOwnAvgLv[0] - 20;
	}
	else if (mBattleMaxStage)
	{
		int iAve = 100 / mBattleMaxStage;
		int iRem = 0;

		if (history->m_iBattleStage == mBattleMaxStage)
			iRem = 100 % mBattleMaxStage;

		mBattleMaxStage = history->m_iBattleStage*iAve - (iAve + iRem) / 2;
	}

	for(int i = 0; i < SCRIPT_MAX_ENEMY_ARMY_NUM; i++)
	{
		if (history->var2[5007] == 1 && para[i].actor_id<-2 && history->var2[-para[i].actor_id - 2]!=5011)
			para[i].actor_id = history->var2[-para[i].actor_id - 2];
		if(para[i].actor_id > trData->actor_num || para[i].actor_id < 0)
		{
			continue;
		}

		if(history->battleEnemyNum >= modConst->NEW_MAX_ENEMY_ARMY_NUM|| (m_iIs72ScriptAdd&&i >= m_iIs72ScriptAdd))
		{
			break;
		}

		if (!((history->m_iBattleType == BATTLE_LADDER|| history->m_iBattleType == BATTLE_IMAGE)&&para[i].actor_id<LADDER_ACTOR_MAX_ID && para[i].actor_id>= LADDER_ACTOR_MIN_ID))
		{
			HistoryActorPtr historyActor = &history->actors[para[i].actor_id];

			history->battleEnemy[history->battleEnemyNum] = historyActor;
			historyActor->ai_type = para[i].ai_type;
			historyActor->oppo_actor_id = para[i].oppo_actor_id;
			historyActor->dest_x = (short)para[i].dest_x;
			historyActor->dest_y = (short)para[i].dest_y;
			historyActor->direction = para[i].direction;
			historyActor->isAssistArmy = para[i].isAssist;
			if (historyActor->direction < 0)
				historyActor->direction = 2;
			if (historyActor->ai_type < 0)
				historyActor->ai_type = 1;
			if (historyActor->isAssistArmy < 0)
			{
				historyActor->isAssistArmy = 0;
			}
			historyActor->hide = para[i].hide;
			if (historyActor->hide < 0)
			{
				historyActor->hide = 0;
			}
			historyActor->is_leave = 0;
			if (para[i].level_add < 0)
				para[i].level_add = 0;
			else if (para[i].level_add > 50)
				para[i].level_add = 50 - para[i].level_add;
			int i72AddLv = 0;
			if (m_iIs72ScriptAdd) i72AddLv = para[i].m_iRandAddLv;

			historyActor->SetActorLv(trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv + para[i].level_add + i72AddLv);
			if (historyActor->GetActorLv() < 1)
				historyActor->SetActorLv(1);

			if (!mBattleMaxStage)
			{
				int iEnemyGkLv = trData->m_mNetWarInfo[history->m_iCurWarId]->iGkLv;
				if (iEnemyGkLv == 1)
				{
					int iMaxLv = 0;
					for (int i = 0; i < history->battleOwnNum; i++)
					{
						if (iMaxLv < history->battleOwn[i]->GetActorLv())
							iMaxLv = history->battleOwn[i]->GetActorLv();
					}

					historyActor->SetActorLv(iMaxLv + para[i].level_add);
				}
				else if (iEnemyGkLv == 4)
				{
					historyActor->SetActorLv(iOwnAvgLv + para[i].level_add);
				}
			}
			else
			{
				if(mBattleMaxStage > 0)
					historyActor->SetActorLv(mBattleMaxStage + para[i].level_add);
			}

			if (historyActor->GetActorLv() > 100)
				historyActor->SetActorLv(100);

			historyActor->x = (short)para[i].x;
			historyActor->y = (short)para[i].y;
			historyActor->style = ACTOR_STATE_NORMAL;
			historyActor->action_finished = false;
			historyActor->relation = ACTOR_RELATION_ENEMY;
			historyActor->exp = 0;
			historyActor->actor_quality = 1;
			historyActor->InitAbility();
			historyActor->SetNullGood();
			historyActor->SetCurHp(historyActor->GetBaseHp() + historyActor->GetGoodsAddHp());
			historyActor->SetCurMp(historyActor->GetBaseMp() + historyActor->GetGoodsAddMp());
			historyActor->UpdateSkills();
			history->LoadBattleActorImage(historyActor->actor_id);
			history->battleEnemyNum++;
		}
		else if(history->m_iBattleType == BATTLE_LADDER&&para[i].actor_id<LADDER_ACTOR_MAX_ID && para[i].actor_id >= LADDER_ACTOR_MIN_ID)
		{
			SetLadderEnemyData(i, mActorListIndex++);
		}
		else if (history->m_iBattleType == BATTLE_IMAGE&&para[i].actor_id < LADDER_ACTOR_MAX_ID && para[i].actor_id >= LADDER_ACTOR_MIN_ID)
		{
			SetImageEnemyData(i);
		}
	}

	return 0;
}

int ScriptDispatchEnemy::initPara(char*SceneData, int index)
{
	mActorListIndex = 0;
	if (history->m_iBattleType == BATTLE_LADDER)
		BattleLadder::getInstance()->ClearBattleActorName();
	for(int i = 0; i < SCRIPT_MAX_ENEMY_ARMY_NUM; i++)
	{
		index = para[i].initPara(SceneData, index);
	}
	return index;
}

void ScriptDispatchEnemy::SetLadderEnemyData(int i, int iActorListIndex)
{
	int iNetActorId = 0;

	map<int, map<int, LadderElemData*> >* ptrActorList = BattleLadder::getInstance()->GetLadderHeroData();

	if (ptrActorList->count(history->m_iCurWarId))
	{
		int iElemIndex = 0;
		HeroElem* ptrHeroElem = BattleLadder::getInstance()->getIndexHeroElem(history->m_iCurWarId, iActorListIndex + 1, iElemIndex);
		map<int, GoodsElem*> ptrGoodsElem;
		int iGoodsElemIndex = 0;

		if (ptrHeroElem)
		{
			iNetActorId = ptrHeroElem->getHeroID();
			for (size_t j = 0; j < ptrActorList->at(history->m_iCurWarId)[iElemIndex]->mGoodsItem.size(); j++)
			{
				if (ptrActorList->at(history->m_iCurWarId)[iElemIndex]->mGoodsItem[j]->getHeroID() == iNetActorId)
				{
					ptrGoodsElem[iGoodsElemIndex++] = ptrActorList->at(history->m_iCurWarId)[iElemIndex]->mGoodsItem[j];
				}
			}
		}
		else
			return;

		{
			if (iNetActorId>1)
				BattleLadder::getInstance()->SetBattleActorName(para[i].actor_id + 1, trData->actors[iNetActorId - 1].actor_name, TYPE_ACTOR_NAME);
			else
				BattleLadder::getInstance()->SetBattleActorName(para[i].actor_id + 1, ptrActorList->at(history->m_iCurWarId)[iElemIndex]->mName, TYPE_ACTOR_NAME);
			BattleLadder::getInstance()->SetBattleActorName(para[i].actor_id + 1, ptrActorList->at(history->m_iCurWarId)[iElemIndex]->mName, TYPE_PLAYER_NAME);
		}

		trData->CopyActor2toActor1(&trData->actors[para[i].actor_id], &trData->actors[iNetActorId - 1]);
		history->mLadderEActor[para[i].actor_id] = iNetActorId;

		InitHeroElemData(ptrHeroElem,i);

		HistoryActorPtr historyActor = &history->actors[para[i].actor_id];
		for (size_t j = 0; j < ptrGoodsElem.size(); j++)
		{
			if (ptrGoodsElem[j]->getHeroID() == iNetActorId)
			{
				HistoryGoods hisGoods(ptrGoodsElem[j]);
				if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
				{
					historyActor->weapon.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
				{
					historyActor->coat.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
				{
					historyActor->assist_good.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
				{
					historyActor->fashion.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
				{
					historyActor->wap_fashion.setGood(&hisGoods);
				}

			}
		}
		historyActor->UpdataGoodEffect();
	}
}

void ScriptDispatchEnemy::SetImageEnemyData(int i)
{
	int iCopyActorId = history->mMapDateInfo->GetBattleImageData()->GetEnemyImageId(para[i].actor_id + 1);
	if (iCopyActorId < 1)
		return;
	trData->CopyActor2toActor1(&trData->actors[para[i].actor_id], &trData->actors[iCopyActorId - 1]);

	HistoryActorPtr ptrCopyActor = &history->actors[iCopyActorId - 1];
	history->mLadderEActor[para[i].actor_id] = iCopyActorId;

	HeroElem eHeroElem;
	eHeroElem.setEXP(ptrCopyActor->exp);
	eHeroElem.setLV(ptrCopyActor->GetActorLv());
	eHeroElem.setJobID(ptrCopyActor->occupation_id + 1);
	if (ptrCopyActor->mInitFashion[0])
	{
		eHeroElem.setFashion1(ptrCopyActor->mInitFashion[2]);
		eHeroElem.setFashion2(ptrCopyActor->mInitFashion[4]);
	}
	eHeroElem.setQuality(ptrCopyActor->actor_quality);
	if(iCopyActorId == 1)
		eHeroElem.setGender(history->m_pPlayerInfo->getMainHero()->getGender());

	eHeroElem.mSkillLv = ptrCopyActor->GetAllSelfSkillLevel();

	InitHeroElemData(&eHeroElem, i);

	HistoryActorPtr historyActor = &history->actors[para[i].actor_id];

	if (ptrCopyActor->weapon.isNormalGood())
	{
		historyActor->weapon.setGood(&ptrCopyActor->weapon);
	}
	if (ptrCopyActor->coat.isNormalGood())
	{
		historyActor->coat.setGood(&ptrCopyActor->coat);
	}
	if (ptrCopyActor->assist_good.isNormalGood())
	{
		historyActor->assist_good.setGood(&ptrCopyActor->assist_good);
	}
	if (ptrCopyActor->fashion.isNormalGood())
	{
		historyActor->fashion.setGood(&ptrCopyActor->fashion);
	}
	if (ptrCopyActor->wap_fashion.isNormalGood())
	{
		historyActor->wap_fashion.setGood(&ptrCopyActor->wap_fashion);
	}

	historyActor->UpdataGoodEffect();
}

void ScriptDispatchEnemy::initPVPEnemyData()
{
	PVPData* ptrData = PVPData::getInstance();
	PVPActorData* pActorData = ptrData->m_mPVPActor[ptrData->getSelectRank()];
	pActorData->initHeroList();
	map<int, PVPArrayItem*> mArrayItem = pActorData->m_pDefend->m_mArray;
	history->battleEnemyNum = mArrayItem.size();
	int iArrayIndex = pActorData->m_pDefend->getArrayID();
	for (UInt i = 0; i < mArrayItem.size(); i++)
	{
		PVPArrayItem* ptrItem = mArrayItem[i];
		int iActorId = pActorData->m_mMirrorId[ptrItem->getActorID()];
		int iIndex = ptrItem->getOrderID();
		PVPArray* ptrArray = trData->m_mPVPArray[iArrayIndex];
		int iPosIndex = ptrArray->mPosGroup[ptrItem->getPointID()];
		int iPosX = trData->m_mPVPPoint[iPosIndex]->iPosX;
		int iPosY = trData->m_mPVPPoint[iPosIndex]->iPosY;
		iPosX = ptrData->getMidIndex() * 2 - iPosX;

		HistoryActor* ptrActor = &history->actors[iActorId];
		history->battleEnemy[iIndex] = ptrActor;
		ptrActor->ai_type = para[0].ai_type;
		ptrActor->oppo_actor_id = para[0].oppo_actor_id;
		ptrActor->dest_x = (short)para[0].dest_x;
		ptrActor->dest_y = (short)para[0].dest_y;
		ptrActor->direction = para[0].direction;
		ptrActor->isAssistArmy = para[0].isAssist;
		if (ptrActor->direction < 0)
			ptrActor->direction = 2;
		if (ptrActor->ai_type < 0)
			ptrActor->ai_type = 1;
		if (ptrActor->isAssistArmy < 0)
		{
			ptrActor->isAssistArmy = 0;
		}
		ptrActor->hide = para[0].hide;
		if (ptrActor->hide < 0)
		{
			ptrActor->hide = 0;
		}
		ptrActor->is_leave = 0;
		ptrActor->x = iPosX;
		ptrActor->y = iPosY;
		ptrActor->style = ACTOR_STATE_NORMAL;
		ptrActor->action_finished = false;
		ptrActor->relation = ACTOR_RELATION_ENEMY;

		//PVP地形特技
		ptrActor->m_mPVPLandSeid.clear();
		ptrActor->m_mPVPLandSeid = ptrArray->mSeidGroup[ptrItem->getPointID()];
		ptrActor->UpdataGoodEffect();
	}
}

void ScriptDispatchEnemy::InitHeroElemData(HeroElem* ptrHeroElem, int i)
{
	HistoryActorPtr historyActor = &history->actors[para[i].actor_id];
	historyActor->init(para[i].actor_id);

	ActorJoinOwnDate NetActorDate;
	NetActorDate.setActorId(para[i].actor_id + 1);
	NetActorDate.setExp(ptrHeroElem->getEXP());
	NetActorDate.setLv(ptrHeroElem->getLV());
	NetActorDate.setJobID(ptrHeroElem->getJobID());
	NetActorDate.setFashion1(ptrHeroElem->getFashion1());
	NetActorDate.setFashion2(ptrHeroElem->getFashion2());
	NetActorDate.setQuality(ptrHeroElem->getQuality());
	NetActorDate.setGrade(ptrHeroElem->getGrade());
	NetActorDate.setGender(ptrHeroElem->getGender());
	NetActorDate.setSoulBox(ptrHeroElem->geteSoulBox());
	NetActorDate.mSkillLv = ptrHeroElem->mSkillLv;
	NetActorDate.mSpecialItem = ptrHeroElem->mSpecialItem;
	NetActorDate.m_mPractice = ptrHeroElem->m_mPractice;
	history->NewActorJoinOwn(&NetActorDate, false);


	history->battleEnemy[history->battleEnemyNum] = historyActor;
	historyActor->ai_type = para[i].ai_type;
	historyActor->oppo_actor_id = para[i].oppo_actor_id;
	historyActor->dest_x = (short)para[i].dest_x;
	historyActor->dest_y = (short)para[i].dest_y;
	historyActor->direction = para[i].direction;
	historyActor->isAssistArmy = para[i].isAssist;
	if (historyActor->direction < 0)
		historyActor->direction = 2;
	if (historyActor->ai_type < 0)
		historyActor->ai_type = 1;
	if (historyActor->isAssistArmy < 0)
	{
		historyActor->isAssistArmy = 0;
	}
	historyActor->hide = para[i].hide;
	if (historyActor->hide < 0)
	{
		historyActor->hide = 0;
	}
	historyActor->is_leave = 0;
	//historyActor->SetActorLv(trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv + para[i].level_add);
	if (historyActor->GetActorLv() < 1)
		historyActor->SetActorLv(1);

	historyActor->x = (short)para[i].x;
	historyActor->y = (short)para[i].y;
	historyActor->style = ACTOR_STATE_NORMAL;
	historyActor->action_finished = false;
	historyActor->relation = ACTOR_RELATION_ENEMY;

	//history->LoadBattleActorImage(historyActor->actor_id);
	history->battleEnemyNum++;
}