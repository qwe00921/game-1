#include "WinConditionTool.h"


WinConditionTool* WinConditionTool::m_instance = NULL;
WinConditionTool* WinConditionTool::getInstance(){
	if (m_instance == NULL){
		m_instance = new WinConditionTool();
	}
	return m_instance;
}

void WinConditionTool::InitBattleCondition()
{
	SetWinConditionEnable();
	ClearAllMap();
	for (size_t i = 0; i < history->net_win_condition.size(); i++)
	{
		WinConditionPtr ptrWinCon = &trData->win_condition[history->net_win_condition[i]];
		switch (ptrWinCon->c_type)
		{
		case 100:
		{
					death_enemy_enable = ptrWinCon->data[0];
					not_included_death_enemy[0][0] = 0;
					for (int j = 1; j < ptrWinCon->data_num; j++)
					{
						not_included_death_enemy[1][ptrWinCon->data[j]] = 1;
					}
		}
			break;
		case 101:
		{
					death_enemy_enable = ptrWinCon->data[0];
					retreat_enemy_enable = 1;
		}
			break;
		case 102:
		{
					death_own_enable = ptrWinCon->data[0];
		}
			break;
		case 103:
		{
					use_occupation_enable = 1;
					for (int j = 0; j < ptrWinCon->data_num; j++)
					{
						use_occupation[ptrWinCon->data[j]] = 1;
					}
		}
			break;
		case 104:
		{
					cnuse_occupation_enable = 1;
					for (int j = 0; j < ptrWinCon->data_num; j++)
					{
						cnuse_occupation[ptrWinCon->data[j]] = 1;
					}
		}
			break;
		case 105:
		{
					occupation_stronghold_enable = 1;
					occupation_stronghold[0][0] = ptrWinCon->data[0];//指定回合数
					int occu_str_num = 1;
					int iDateNum = (ptrWinCon->data_num -1) % 2 == 0 ? (ptrWinCon->data_num-1) : (ptrWinCon->data_num - 2);
					for (int j = 1; j <= iDateNum; j += 2)
					{
						occupation_stronghold[occu_str_num][0] = ptrWinCon->data[j];
						occupation_stronghold[occu_str_num][1] = ptrWinCon->data[j+1];
						occupation_stronghold[occu_str_num][2] = 0;
						occu_str_num++;
					}
		}
			break;
		case 106:
		{
					round_limit_enable = ptrWinCon->data[0];
		}
			break;
		case 107:
		{
					actor_kill_enemy_enable = 1;
					actor_round_info[ptrWinCon->data[0]][0] = ptrWinCon->data[1];//限制回合数
					actor_round_info[ptrWinCon->data[0]][1] = ptrWinCon->data[2];//需要杀敌数
					actor_round_info[ptrWinCon->data[0]][2] = 1;//当前回合id
					actor_round_info[ptrWinCon->data[0]][3] = 0;//限制回合内最多的杀敌数
					actor_kill_enemy[ptrWinCon->data[0]][1] = 0;//每一回合杀敌数
		}
			break;
		case 108:
		{
					death_friend_enable = ptrWinCon->data[0];
		}
			break;
		case 109:
		{
					actor_num_limit_enable = ptrWinCon->data[0];
		}
			break;
		case 110:
		{
					destroy_designated_generals_enable = 1;
					destroy_designated_generals[0][0] = 0;//已杀掉敌军数量
					for (int j = 0; j < ptrWinCon->data_num; j++)
					{
						if (ptrWinCon->data[j] < 0)
						{
							destroy_designated_generals[1][-ptrWinCon->data[j]] = 0;
							destroy_designated_generals[0][1]++;//需要杀掉的敌军数量
						}
						else if(ptrWinCon->data[j] > 0)
						{
							destroy_designated_generals[2][ptrWinCon->data[j]] = 0;
						}
					}
		}
			break;
		case 111:
		{
					round_kill_Enemy_enable = 1;
					round_kill_Enemy[1][0] = ptrWinCon->data[0];//回合数
					round_kill_Enemy[1][1] = ptrWinCon->data[1];//杀敌数
					round_kill_Enemy[1][2] = 0;//当前回合
					round_kill_Enemy[1][3] = 0;//N回合最大杀敌数
		}
			break;
		case 112:
		{
					round_kill_appoint_Enemy_enable = 1;
					round_kill_appoint_Enemy[0][0] = ptrWinCon->data[0];//回合数
					round_kill_appoint_Enemy[0][1] = 0;//已杀敌数
					for (int j = 1; j < ptrWinCon->data_num; j++)
					{
						round_kill_appoint_Enemy[1][ptrWinCon->data[j]] = 1;
					}
		}
			break;
		case 113:
		{
			cnuse_actor_enable = 1;
			for (int j = 0; j < ptrWinCon->data_num; j++)
			{
				cnuse_actor[ptrWinCon->data[j]] = 1;
			}
		}
		break;
		case 200:
		{
					cnuse_goods_enable = 1;
		}
			break;
		case 300:
		{
			map<int, int> svMap;
			for (int j = 0; j < ptrWinCon->data_num; j++)
			{
				svMap[j] = ptrWinCon->data[j];
			}
			shaping_variable_data[history->net_win_condition[i]] = svMap;
		}
			break;
		case 301:
		{
			map<int, int> svMap;
			for (int j = 0; j < ptrWinCon->data_num; j++)
			{
				svMap[j] = ptrWinCon->data[j];
			}
			shaping_variable_data_battled[history->net_win_condition[i]] = svMap;
		}
		break;
		}
	}
}

void WinConditionTool::StartBattleSetCondition()
{
	if (use_occupation_enable>-1 || cnuse_occupation_enable>-1 || cnuse_actor_enable>-1)
	{
		map<int, int> occuArr;
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			occuArr[history->battleOwn[i]->occupation_id+1] = 1;
		}
		if (use_occupation_enable)
		{
			bool all_occupation = true;
			for (map<int, int>::iterator it = use_occupation.begin(); it != use_occupation.end(); it++)
			{
				if (occuArr.find(it->first) == occuArr.end())
				{
					all_occupation = false;
					break;
				}
			}
			reach_use_occupation = all_occupation;
		}
		if (cnuse_occupation_enable)
		{
			bool all_occupation = true;
			for (map<int, int>::iterator it = cnuse_occupation.begin(); it != cnuse_occupation.end(); it++)
			{
				if (occuArr.find(it->first) != occuArr.end())
				{
					all_occupation = false;
					break;
				}
			}
			reach_cnuse_occupation = all_occupation;
		}
		if (cnuse_actor_enable)
		{
			bool bCnuseActor = true;
			for (int i = 0; i < history->battleOwnNum; i++)
			{
				if (cnuse_actor.count(history->battleOwn[i]->actor_id+1))
				{
					bCnuseActor = false;
					break;
				}
			}
			reach_cnuse_actor = bCnuseActor;
		}
	}
	if (actor_num_limit_enable > -1)
	{
		if (history->battleOwnNum <= actor_num_limit_enable)
			reach_actor_num_limit = true;
		else
			reach_actor_num_limit = false;
	}
}

void WinConditionTool::BattleSetCondition(ActionGainPtr ptrGain, HistoryActorPtr curActor)
{
	if (death_enemy_enable > 0 || death_own_enable >= 0 || death_friend_enable >=0|| actor_kill_enemy_enable > 0 || destroy_designated_generals_enable>0 || round_kill_Enemy_enable>0
		|| round_kill_appoint_Enemy_enable>0)
	{
		int kill_num = 0;
		for (int i = 0; i < ptrGain->gains->size(); i++)
		{
			ActionSingleGainPtr ptrSingleGain = ptrGain->gains->elementAt(i);
			if (ptrSingleGain->oppo_actor->GetCurHp() + ptrSingleGain->gain_hp <= 0 &&
				(curActor->relation == 2 && ptrSingleGain->oppo_actor->relation != 2 || 
				curActor->relation != 2 && ptrSingleGain->oppo_actor->relation == 2)&&
				trData->occupations[ptrSingleGain->oppo_actor->occupation_id].is_robot!=1)
			{
				int oppo_actorid = ptrSingleGain->oppo_actor->actor_id + 1;
				HistoryActorPtr ptrOppoActor = ptrSingleGain->oppo_actor;
				if (ptrSingleGain->share_actor)
				{
					if (ptrSingleGain->share_actor->GetCurHp() + ptrSingleGain->gain_hp <= 0)
					{
						oppo_actorid = ptrSingleGain->share_actor->actor_id + 1;
						ptrOppoActor = ptrSingleGain->share_actor;
					}
					else
						continue;
				}

				int iRelation = ptrOppoActor->relation;
				if (ptrOppoActor->controlled_by_enemy)
					iRelation = 2;

				if (death_enemy_enable>0 && iRelation == 2)
				{
					death_enemy[oppo_actorid] = 1;
				}
				if (death_enemy_enable > 0 && iRelation == 2 && not_included_death_enemy[1].count(oppo_actorid))
				{
					not_included_death_enemy[0][0]++;
				}

				if (death_own_enable >= 0 && iRelation == 0)//警戒不算死亡
				{
					death_own[oppo_actorid] = 1;
				}

				if (death_friend_enable >= 0 && iRelation == 1)
				{
					death_friend[oppo_actorid] = 1;
				}

				if (destroy_designated_generals_enable > 0 && iRelation == 2
					&&(!destroy_designated_generals[2].size() ||destroy_designated_generals[2].count(curActor->actor_id+1)>0)
					&& destroy_designated_generals[1].count(oppo_actorid)>0)
				{
					destroy_designated_generals[0][0]++;
				}
				if (round_kill_appoint_Enemy_enable > 0 && history->GetCurRound() <= round_kill_appoint_Enemy[0][0] && round_kill_appoint_Enemy[1].count(oppo_actorid))
				{
					round_kill_appoint_Enemy[0][1]++;
				}
				kill_num++;
			}
		}

		if (actor_kill_enemy_enable > 0)
		{
			int iActorId = curActor->actor_id + 1;
			if (actor_round_info.find(iActorId) != actor_round_info.end())
			{
				int arr_size = actor_kill_enemy[iActorId].size();
				if (actor_round_info[iActorId][2] != history->GetCurRound())
				{
					arr_size += 1;
					actor_kill_enemy[iActorId][arr_size] = kill_num;
					actor_round_info[iActorId][2] = history->GetCurRound();
				}
				else
				{
					actor_kill_enemy[iActorId][arr_size] += kill_num;
				}

				int limit_round_num = actor_round_info[iActorId][0];
				if (limit_round_num <= 0)
					limit_round_num = 999;
				int kill_enemy_num_max = 0;
				for (int i = arr_size; i > 0 && i > arr_size - limit_round_num; i--)
				{
					kill_enemy_num_max += actor_kill_enemy[iActorId][i];
				}
				if (kill_enemy_num_max > actor_round_info[iActorId][3])
					actor_round_info[iActorId][3] = kill_enemy_num_max;
			}
		}

		if (round_kill_Enemy_enable > 0)
		{
			int arr_size = round_kill_Enemy[0].size();
			if (round_kill_Enemy[1][2] != history->GetCurRound())
			{
				arr_size += 1;
				round_kill_Enemy[0][arr_size] = kill_num;
				round_kill_Enemy[1][2] = history->GetCurRound();
			}
			else
			{
				round_kill_Enemy[0][arr_size] += kill_num;
			}

			int limit_round_num = round_kill_Enemy[1][0];
			int kill_enemy_num_max = 0;
			for (int i = arr_size; i > 0 && i > arr_size - limit_round_num; i--)
			{
				kill_enemy_num_max += round_kill_Enemy[0][i];
			}
			if (kill_enemy_num_max > round_kill_Enemy[1][3])
				round_kill_Enemy[1][3] = kill_enemy_num_max;
		}
	}
	if (history->m_iBattleType == BATTLE_WOLFCITY)
	{
		for (int i = 0; i < ptrGain->gains->size(); i++)
		{
			ActionSingleGainPtr ptrSingleGain = ptrGain->gains->elementAt(i);
			if (((curActor->relation != 2 && ptrSingleGain->oppo_actor->relation == 2)|| (curActor->relation == 2 && ptrSingleGain->oppo_actor->controlled_by_enemy)) &&
				ptrSingleGain->oppo_actor->GetCurHp() == ptrSingleGain->oppo_actor->GetBaseHp() + ptrSingleGain->oppo_actor->GetGoodsAddHp()&&
				ptrSingleGain->oppo_actor->GetCurHp() + ptrSingleGain->gain_hp <= 0 &&
				trData->occupations[ptrSingleGain->oppo_actor->occupation_id].is_robot != 1)
			{
				own_seckill_enemy[ptrSingleGain->oppo_actor->actor_id + 1] = 1;
			}
		}
	}
	if (cnuse_goods_enable > -1 && ptrGain->good != NULL)
		reach_use_goods = true;

}

void WinConditionTool::ActorRetreatCondition(int iActorId)
{
	HistoryActorPtr ptrActor = &history->actors[iActorId];
	if (retreat_own_enable>=0&&ptrActor->relation == 0)
	{
		retreat_own[iActorId + 1] = 1;
	}
	else if (retreat_enemy_enable >= 0 && ptrActor->relation == 2)
	{
		retreat_enemy[iActorId + 1] = 1;
	}
	else if (retreat_friend_enable >= 0 && ptrActor->relation == 1)
	{
		retreat_friend[iActorId + 1] = 1;
	}
}

void WinConditionTool::BattleEndSetCondition()
{
	BattleRoundEndSetCon();
	if (history->GetCurRound() <= round_limit_enable)
		reach_round_limit = true;
}
void WinConditionTool::BattleRoundEndSetCon()
{
	if (occupation_stronghold_enable > 0)
	{
		for (size_t i = 1; i <= occupation_stronghold.size()-1; i++)
		{
			if (occupation_stronghold[i][2]>0 || history->GetCurRound() > occupation_stronghold[0][0])
				continue;
			HistoryActorPtr ptrActor = history->FindActorPosition(occupation_stronghold[i][0], occupation_stronghold[i][1]);
			if (ptrActor != NULL&&ptrActor->relation == 0)
			{
				occupation_stronghold[i][2] = 1;
			}
		}
	}
}

map<int, int> WinConditionTool::ReachConditionResult(bool bBattleWin)
{
	map<int, int> ResultRet;

	for (size_t i = 0; i < history->net_win_condition.size(); i++)
	{
		WinConditionPtr ptrWinCon = &trData->win_condition[history->net_win_condition[i]];
		switch (ptrWinCon->c_type)
		{
		case 100:
		{
					if ((int)death_enemy.size() - not_included_death_enemy[0][0] >= death_enemy_enable)
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 101:
		{
					if (int(death_enemy.size()+retreat_enemy.size()) >= death_enemy_enable)
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 102:
		{
					if (bBattleWin && (int)death_own.size() <= death_own_enable)
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 103:
		{
					ResultRet[history->net_win_condition[i]] = reach_use_occupation ? 1 : 0;
		}
			break;
		case 104:
		{
					ResultRet[history->net_win_condition[i]] = reach_cnuse_occupation ? 1 : 0;
		}
			break;
		case 105:
		{
					int bResult = 1;
					for (size_t j = 1; j <= occupation_stronghold.size()-1; j++)
					{
						if (occupation_stronghold[j][2] <= 0)
						{
							bResult = 0;
							break;
						}
					}
					ResultRet[history->net_win_condition[i]] = bResult;
		}
			break;
		case 106:
		{
					ResultRet[history->net_win_condition[i]] = reach_round_limit ? 1 : 0;
		}
			break;
		case 107:
		{
					if (actor_round_info[ptrWinCon->data[0]][3] >= actor_round_info[ptrWinCon->data[0]][1])
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 108:
		{
					if (bBattleWin&& (int)death_friend.size()<=death_friend_enable)
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 109:
		{
					actor_num_limit_enable = ptrWinCon->data[0];
					ResultRet[history->net_win_condition[i]] = reach_actor_num_limit ? 1 : 0;
		}
			break;
		case 110:
		{
					ResultRet[history->net_win_condition[i]] = 0;
					if (destroy_designated_generals[0][0] >= destroy_designated_generals[0][1])
						ResultRet[history->net_win_condition[i]] = 1;
		}
			break;
		case 111:
		{
					ResultRet[history->net_win_condition[i]] = 0;
					if (round_kill_Enemy[1][3] >= round_kill_Enemy[1][1])
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
		}
			break;
		case 112:
		{
					ResultRet[history->net_win_condition[i]] = 0;
					if (round_kill_appoint_Enemy[0][1] >= (int)round_kill_appoint_Enemy[1].size())
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
		}
			break;
		case 113:
		{
			ResultRet[history->net_win_condition[i]] = reach_cnuse_actor ? 1 : 0;
		}
		break;
		case 200:
		{
					if (bBattleWin&&!reach_use_goods)
					{
						ResultRet[history->net_win_condition[i]] = 1;
					}
					else
					{
						ResultRet[history->net_win_condition[i]] = 0;
					}
		}
			break;
		case 300:
		{
			ResultRet[history->net_win_condition[i]] = 0;
			if (shaping_variable_data.count(history->net_win_condition[i]))
			{
				map<int, int> svMap = shaping_variable_data[history->net_win_condition[i]];
				if((int)svMap.size()<3) break;
				if ((svMap[1] == 0 && history->var2[svMap[0]] < svMap[2])|| (svMap[1] == 1 && history->var2[svMap[0]] == svMap[2]) || (svMap[1] == 2 && history->var2[svMap[0]] > svMap[2]))
				{
					ResultRet[history->net_win_condition[i]] = 1;
				}
			}
		}
			break;
		case 301:
		{
			ResultRet[history->net_win_condition[i]] = 0;
			if (bBattleWin&&shaping_variable_data_battled.count(history->net_win_condition[i]))
			{
				map<int, int> svMap = shaping_variable_data_battled[history->net_win_condition[i]];
				if ((int)svMap.size() < 3) break;
				if ((svMap[1] == 0 && history->var2[svMap[0]] < svMap[2]) || (svMap[1] == 1 && history->var2[svMap[0]] == svMap[2]) || (svMap[1] == 2 && history->var2[svMap[0]] > svMap[2]))
				{
					ResultRet[history->net_win_condition[i]] = 1;
				}
			}
		}
		break;
		}
	}
	return ResultRet;
}

void WinConditionTool::AddBattleConsumes(int iGoodsId, int iNum)
{
	if (consumesList.find(iGoodsId) != consumesList.end())
		consumesList[iGoodsId] += iNum;
	else
		consumesList[iGoodsId] = iNum;
}

map<int, int> WinConditionTool::GetConsumesList()
{
	return consumesList;
}

void WinConditionTool::BattleFailedSetDate()
{
	for (map<int, int>::iterator it = consumesList.begin(); it != consumesList.end(); it++)
	{
		int iGoodsId = it->first;
		history->m_mConsuNum[iGoodsId] += it->second;
	}
	if (scriptAddActor.size())
	{
		for (map<int, int>::iterator mIt = scriptAddActor.begin(); mIt != scriptAddActor.end(); mIt++)
		{
			for (int j = mIt->second; j < history->ownArmyNum - 1; j++)
			{
				history->ownArmy[j] = history->ownArmy[j + 1];
			}
			history->ownArmyNum--;
		}
	}
}

void WinConditionTool::AddScriptCodeElem(int iScode)
{
	scriptCode[iScode] = 1;
}

map<int, int> WinConditionTool::GetScriptCodeList()
{
	return scriptCode;
}

void WinConditionTool::AddScriptActorElem(int iActorId,int iOwnListIndex)
{
	scriptAddActor[iActorId] = iOwnListIndex;
}

map<int, int> WinConditionTool::GetScriptActorList()
{
	return scriptAddActor;
}

map<int, int> WinConditionTool::GetSeckillEnemy()
{
	return own_seckill_enemy;
}

void WinConditionTool::SetWinConditionEnable()
{
	death_enemy_enable = -1;

	retreat_enemy_enable = -1;

	death_own_enable = -1;

	retreat_own_enable = -1;

	use_occupation_enable = -1;
	reach_use_occupation = false;

	cnuse_occupation_enable = -1;
	reach_cnuse_occupation = false;

	cnuse_actor_enable = -1;
	reach_cnuse_actor = false;

	round_limit_enable = -1;
	reach_round_limit = false;

	actor_kill_enemy_enable = -1;

	death_friend_enable = -1;

	retreat_friend_enable = -1;

	actor_num_limit_enable = -1;
	reach_actor_num_limit = false;

	cnuse_goods_enable = -1;
	reach_use_goods = false;

	occupation_stronghold_enable = -1;

	destroy_designated_generals_enable = -1;

	round_kill_Enemy_enable = -1;

	round_kill_appoint_Enemy_enable = -1;
}

void WinConditionTool::ClearAllMap()
{
	death_enemy.clear();
	retreat_enemy.clear();
	death_own.clear();
	retreat_own.clear();
	use_occupation.clear();
	cnuse_occupation.clear();
	cnuse_actor.clear();
	actor_round_info.clear();
	actor_kill_enemy.clear();
	death_friend.clear();
	retreat_friend.clear();
	occupation_stronghold.clear();
	consumesList.clear();
	destroy_designated_generals.clear();
	round_kill_Enemy.clear();
	round_kill_appoint_Enemy.clear();
	not_included_death_enemy.clear();
	scriptCode.clear();
	scriptAddActor.clear();
	own_seckill_enemy.clear();
	shaping_variable_data.clear();
	shaping_variable_data_battled.clear();
}

WinConditionTool::WinConditionTool()
{
	SetWinConditionEnable();
}

WinConditionTool::~WinConditionTool()
{

}