#include "Common.h"
#include "Scripts.h"

int ScriptDispatchFriend::HandleScript()
{

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

	for(int i = 0; i < SCRIPT_MAX_FRIEND_ARMY_NUM; i++)
	{
		if(para[i].actor_id > trData->actor_num || para[i].actor_id < 0)
		{
			continue;
		}

		if(history->battleFriendNum >= modConst->NEW_MAX_FRIEND_ARMY_NUM)
		{
			break;
		}

		HistoryActorPtr historyActor = &history->actors[para[i].actor_id];
		history->battleFriend[history->battleFriendNum] = historyActor;
		historyActor->ai_type = para[i].ai_type;
		historyActor->oppo_actor_id = para[i].oppo_actor_id;
		historyActor->dest_x = (short)para[i].dest_x;
		historyActor->dest_y = (short)para[i].dest_y;
		historyActor->direction = para[i].direction;
		if(historyActor->direction < 0)
			historyActor->direction = 2;
		if(historyActor->ai_type < 0)
			historyActor->ai_type = 1;
		historyActor->hide = para[i].hide;
		if(historyActor->hide < 0)
		{
			historyActor->hide = 0;
		}
		historyActor->is_leave = 0;

		int ownarmy = 0;
		for(int j = 0; j < history->ownArmyNum; j++)
		{
			if(para[i].actor_id == history->ownArmy[j])
			{
				ownarmy = 1;
				break;
			}
		}
		if(ownarmy == 0)
		{
			if(para[i].level_add < 0)
				para[i].level_add = 0;
			else if(para[i].level_add > 50)
				para[i].level_add = 50 - para[i].level_add;
			historyActor->SetActorLv(trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv + para[i].level_add);
			if (historyActor->GetActorLv() < 1)
				historyActor->SetActorLv(1);
			historyActor->actor_quality = 1;
			historyActor->exp = 0;
			historyActor->InitAbility();
			historyActor->SetNullGood();
		}

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
			if (mBattleMaxStage > 0)
				historyActor->SetActorLv(mBattleMaxStage + para[i].level_add);
		}

		if (historyActor->GetActorLv() > 100)
			historyActor->SetActorLv(100);

		historyActor->x = (short)para[i].x;
		historyActor->y = (short)para[i].y;
		historyActor->style = ACTOR_STATE_NORMAL;
		historyActor->action_finished = false;
		historyActor->relation = ACTOR_RELATION_FRIEND;
		historyActor->InitFiveBaseAbility();
		historyActor->UpdataGoodEffect();
		historyActor->InitHPAndMPAbility();
		historyActor->UpdateSkills();
		history->LoadBattleActorImage(historyActor->actor_id);
		history->battleFriendNum++;
	}
	return 0;
}

int ScriptDispatchFriend::initPara(char*SceneData, int index)
{
	for(int i = 0; i < SCRIPT_MAX_FRIEND_ARMY_NUM; i++)
	{
		index = para[i].initPara(SceneData, index);
	}
	return index;
}

