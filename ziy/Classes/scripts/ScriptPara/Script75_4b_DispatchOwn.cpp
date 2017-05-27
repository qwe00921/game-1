#include "Common.h"
#include "Scripts.h"
#include "PVPData.h"

int ScriptDispatchOwn::HandleScript()
{
	if (history->m_iBattleType == BATTLE_PVP)
	{
		initPVPOwnData();
		return 0;
	}

	if(history->var2[5004] == 1 && history->var2[5006] > 0)
	{
		out_index += history->var2[5006] * MAX_FORCE_LIMIT;
	}

	if(out_index < history->battleOwnNum)
	{
		HistoryActorPtr historyActor = history->battleOwn[out_index];
		historyActor->x = (short)x;
		historyActor->y = (short)y;
		historyActor->direction = direction;
		historyActor->hide = hide;
		historyActor->is_leave = 0;
		historyActor->style = ACTOR_STATE_NORMAL;
		historyActor->action_finished = 0;
		historyActor->relation = 0;
		historyActor->ai_type = 1;

		int iActorMaxLevel = 0;
		int iActorGqLimit = trData->m_mNetWarInfo[history->m_iCurWarId]->iGkLv;
		if ((iActorGqLimit == 2|| iActorGqLimit == 3) && trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv>0)
		{
			int iCurActorLv = historyActor->GetActorLv();
			iActorMaxLevel = trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv;

			if (iCurActorLv > iActorMaxLevel)
			{
				historyActor->SetActorOldLv(iCurActorLv);
				historyActor->SetActorLv(iActorMaxLevel);
			}
			else
				iActorMaxLevel = 0;

			if (iActorGqLimit == 3 && LEVELGROWTH)
			{
				int iMaxQuality = ceill(1.0*trData->m_mNetWarInfo[history->m_iCurWarId]->iEnemyLv / LEVELGROWTH);

				if (historyActor->actor_quality > iMaxQuality)
				{
					historyActor->actor_old_quality = historyActor->actor_quality;
					historyActor->actor_quality = iMaxQuality;
				}
			}
		}

		historyActor->InitFiveBaseAbility();
		historyActor->UpdataGoodEffect();
		historyActor->InitHPAndMPAbility();
		historyActor->UpdateSkills(iActorMaxLevel);
	}
	return 0;
}

int ScriptDispatchOwn::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_index = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction = bytesToShort(SceneData, index);
	if(direction == -1 || direction == 65535)
	{
		direction = 2;
	}
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	hide = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}

void ScriptDispatchOwn::initPVPOwnData()
{
	PVPData* ptrData = PVPData::getInstance();
	map<int, PVPArrayItem*> mArrayItem = ptrData->m_mArrayInfo[ARRAY_ATTACK]->m_mArray;
	int iArrayIndex = ptrData->m_mArrayInfo[ARRAY_ATTACK]->getArrayID();
	for (UInt i = 0; i < mArrayItem.size(); i++)
	{
		PVPArrayItem* ptrItem = mArrayItem[i];
		int iActorId = ptrItem->getActorID() - 1;
		PVPArray* ptrArray = trData->m_mPVPArray[iArrayIndex];
		int iPosIndex = ptrArray->mPosGroup[ptrItem->getPointID()];
		int iPosX = trData->m_mPVPPoint[iPosIndex]->iPosX;
		int iPosY = trData->m_mPVPPoint[iPosIndex]->iPosY;

		HistoryActor* ptrActor = &history->actors[iActorId];
		ptrActor->x = iPosX;
		ptrActor->y = iPosY;
		ptrActor->direction = direction;
		ptrActor->hide = hide;
		ptrActor->is_leave = 0;
		ptrActor->style = ACTOR_STATE_NORMAL;
		ptrActor->action_finished = 0;
		ptrActor->relation = 0;
		ptrActor->ai_type = 1;

		//PVP地形特技
		ptrActor->m_mPVPLandSeid.clear();
		ptrActor->m_mPVPLandSeid = ptrArray->mSeidGroup[ptrItem->getPointID()];
		ptrActor->UpdataGoodEffect();
	}
}
