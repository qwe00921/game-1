#include "Common.h"
#include "Scripts.h"


int ScriptArmyChange::HandleRScript(RedeScreenPtr scr){
	if (actor_id < -1)//变量武将
	{
		actor_id_replace = history->var2[(int)abs(actor_id) - 2];
	}
	else
	{
		actor_id_replace = actor_id;
	}
	if(come_or_leave == 0)//Data加入
	{
		history->ArmyCome(actor_id_replace, level_add);
	}
	else if(come_or_leave == 1)//内存加入
	{
        int tmp_max = 99;
		if(tmp_max + 1 == level_add) //还原离开时的状态
		{
			history->ArmyComeReturnToLeave(actor_id_replace);
		}
		else // 平均等级加入
		{
			history->ArmyCome(actor_id_replace, 0,true);
		}
	}
	else//离开255
	{
		if (1== level_add){
			history->ArmyLeave(actor_id_replace, 0);
		}
		else{
			history->ArmyLeave(actor_id_replace);
		}
		history->actors[actor_id_replace].force_save = 1;
	}
	return 0;
}

int ScriptArmyChange::HandleScript(){
	if (actor_id < -1)//变量武将
	{
		actor_id_replace = history->var2[(int)abs(actor_id) - 2];
	}
	else
	{
		actor_id_replace = actor_id;
	}
	switch (come_or_leave)
	{
		case 0://0 data加入
		{
			int i;
			bool isAdd = false;
			for (i = 0; i < history->battleFriendNum; i++) {
				if (history->battleFriend[i]->actor_id == actor_id_replace && history->battleFriend[i]->GetCurHp()>0)
				{
					history->battleOwn[history->battleOwnNum] = history->battleFriend[i];
					history->battleOwn[history->battleOwnNum]->relation = ACTOR_RELATION_OWN;
					history->battleOwn[history->battleOwnNum]->ai_type = 1;//修改为主动出击
					history->battleOwnNum++;
					for (int j = i; j < history->battleFriendNum - 1; j++)
					{
						history->battleFriend[j] = history->battleFriend[j + 1];
					}
					history->battleFriendNum--;
					isAdd = true;
					break;
				}
			}
			if (!isAdd)
			{
				for (i = 0; i < history->battleEnemyNum; i++) {
					if (history->battleEnemy[i]->actor_id == actor_id_replace && history->battleEnemy[i]->GetCurHp()>0)
					{
						history->battleOwn[history->battleOwnNum] = history->battleEnemy[i];
						history->battleOwn[history->battleOwnNum]->relation = ACTOR_RELATION_OWN;
						history->battleOwn[history->battleOwnNum]->ai_type = 1;//修改为主动出击
						history->battleOwnNum++;
						for (int j = i; j < history->battleEnemyNum - 1; j++)
						{
							history->battleEnemy[j] = history->battleEnemy[j + 1];
						}
						history->battleEnemyNum--;
						isAdd = true;
						break;
					}
				}
			}
			if (isAdd)
			{
				for (i = 0; i < history->ownArmyNum; i++)
				{
					if (actor_id_replace == history->ownArmy[i])
						return 0;
				}
				history->ownArmy[history->ownArmyNum] = actor_id_replace;
				history->ownArmyNum++;
				history->LoadBattleActorImage(actor_id_replace);
			}
			break;
		}
		case 1://1 内存加入
		{
			int i;
			bool isAdd = false;
			for (i = 0; i < history->battleOwnNum; i++) {
				if (history->battleOwn[i]->actor_id == actor_id_replace && history->battleOwn[i]->GetCurHp()>0)
				{
					history->battleFriend[history->battleFriendNum] = history->battleOwn[i];
					history->battleFriend[history->battleFriendNum]->relation = ACTOR_RELATION_FRIEND;
					history->battleFriend[history->battleFriendNum]->ai_type = 1;//修改为主动出击
					history->battleFriendNum++;
					for (int j = i; j < history->battleOwnNum - 1; j++)
					{
						history->battleOwn[j] = history->battleOwn[j + 1];
					}
					history->battleOwnNum--;
					isAdd = true;
					for (int i = 0; i < history->ownArmyNum; i++)
					{
						if (actor_id_replace == history->ownArmy[i])
						{
							for (int j = i; j < history->ownArmyNum - 1; j++)
							{
								history->ownArmy[j] = history->ownArmy[j + 1];
							}
							history->ownArmyNum--;
							break;
						}
					}
					break;
				}
			}
			if (!isAdd)
			{
				for (i = 0; i < history->battleEnemyNum; i++) {
					if (history->battleEnemy[i]->actor_id == actor_id_replace && history->battleEnemy[i]->GetCurHp()>0)
					{
						history->battleFriend[history->battleFriendNum] = history->battleEnemy[i];
						history->battleFriend[history->battleFriendNum]->relation = ACTOR_RELATION_FRIEND;
						history->battleFriend[history->battleFriendNum]->ai_type = 1;//修改为主动出击
						history->battleFriendNum++;
						for (int j = i; j < history->battleEnemyNum - 1; j++)
						{
							history->battleEnemy[j] = history->battleEnemy[j + 1];
						}
						history->battleEnemyNum--;
						break;
					}
				}
			}
			history->LoadBattleActorImage(actor_id_replace);
			history->actors[actor_id_replace].force_save = 1;
			break;
		}
		case 255://255 离开
		{
			int i;
			bool isAdd = false;
			for (i = 0; i < history->battleOwnNum; i++) 
			{
				if (history->battleOwn[i]->actor_id == actor_id_replace && history->battleOwn[i]->GetCurHp()>0)
				{
					history->battleEnemy[history->battleEnemyNum] = history->battleOwn[i];
					history->battleEnemy[history->battleEnemyNum]->relation = ACTOR_RELATION_ENEMY;
					history->battleEnemy[history->battleEnemyNum]->ai_type = 1;//修改为主动出击
					history->battleEnemyNum++;
					for (int j = i; j < history->battleOwnNum - 1; j++)
					{
						history->battleOwn[j] = history->battleOwn[j + 1];
					}
					history->battleOwnNum--;
					isAdd = true;
					for (int i = 0; i < history->ownArmyNum; i++)
					{
						if (actor_id_replace == history->ownArmy[i])
						{
							for (int j = i; j < history->ownArmyNum - 1; j++)
							{
								history->ownArmy[j] = history->ownArmy[j + 1];
							}
							history->ownArmyNum--;
							break;
						}
					}
					break;
				}
			}
			if (!isAdd)
			{
				for (i = 0; i < history->battleFriendNum; i++) {
					if (history->battleFriend[i]->actor_id == actor_id_replace && history->battleFriend[i]->GetCurHp()>0)
					{
						history->battleEnemy[history->battleEnemyNum] = history->battleFriend[i];
						history->battleEnemy[history->battleEnemyNum]->relation = ACTOR_RELATION_ENEMY;
						history->battleEnemy[history->battleEnemyNum]->ai_type = 1;//修改为主动出击
						history->battleEnemyNum++;
						for (int j = i; j < history->battleFriendNum - 1; j++)
						{
							history->battleFriend[j] = history->battleFriend[j + 1];
						}
						history->battleFriendNum--;
						break;
					}
				}
			}
			history->actors[actor_id_replace].force_save = 1;
			break;
		}
		default:
			break;
	}

	return 0;
}

int ScriptArmyChange::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);//-1 无
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	come_or_leave =  bytesToShort(SceneData, index);//0 data加入 1 内存加入 255 离开
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	level_add =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}



