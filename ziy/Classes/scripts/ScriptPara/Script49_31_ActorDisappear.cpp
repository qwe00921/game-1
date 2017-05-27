#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
void ScriptActorDisappear::getActorList()
{
	int i,j;
	HistoryActorPtr actor = NULL; 
	for(i = left_top_y;i<=right_bottom_y;i++)
	{
		for(j = left_top_x;j<=right_bottom_x;j++)
		{
			actor = history->FindActorPosition(j,i);
			if(actor!=NULL)
			{
				switch(relation)
				{
				case 0:
					if(actor->relation == 0)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 1:
					if(actor->relation == 1)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 2:
					if(actor->relation == 2)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 3:
					break;
				case 4:
					if(actor->relation == 0||actor->relation == 1)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 5:
					if(actor->relation == 2)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 6:
					actor_id_list[actor_num++] = actor->actor_id;
					break;
				}
			}
		}
	}
}
int ScriptActorDisappear::HandleRScript(RedeScreenPtr scr)
{
	if (actor_id < -1)//变量武将
	{
		actor_id_replace = history->var2[(int)abs(actor_id) - 2];
	}
	else
	{
		actor_id_replace = actor_id;
	}
	HistoryActorPtr actor = history->FindActor(actor_id_replace);
	for(int i = 0;i<history->RedeActorNum;i++)
	{
		if(history->RedeActor[i] == actor)
		{
			for(int j = i;j<history->RedeActorNum-1;j++)
			{
				history->RedeActor[j] = history->RedeActor[j+1];
			}
			break;
		}
	}
	history->RedeActorNum --;
	scr->InitActorQueue();
	return 0;
}
int ScriptActorDisappear::HandleScript() {
	BattleActorPtr battle_actor;
	if(frame == 0)
	{
		if (actor_id < -1)//变量武将
		{
			actor_id_replace = history->var2[(int)abs(actor_id) - 2];
		}
		else
		{
			actor_id_replace = actor_id;
		}
		actor_num = 0;
		actor_index= 0;
		if(which_index <= 0)
		{
			actor_id_list[actor_num++] = actor_id_replace;
		}
		else
		{
			getActorList();
		}
		if (actor_num == 0)
		{
			need_frame = 0;
		}
	}
	if(frame == need_frame)
	{
		if(actor_index!=actor_num)
		{
			frame = 0;
			battle_actor = BattleControler::getBattleActor(actor_id_list[actor_index++]);
			if (battle_actor && battle_actor->hisActor != NULL && battle_actor->hisActor->hide !=1)//隐藏或死亡的不进行重新消失
			{
				need_frame = battle_actor->setState(ACTOR_STATE_LEAVE_HIDE);
			}

			if (trData->occupations[battle_actor->hisActor->occupation_id].is_robot == 2&& battle_actor->hisActor->relation == 0)
			{
				for (int i = 900; i < 904; i++)
				{
					battle_actor = BattleControler::getBattleActor(i);
					if (battle_actor)
					{
						battle_actor->setState(ACTOR_STATE_LEAVE_HIDE);
					}
				}
			}
		}
	}
	return need_frame - frame++;
}

int ScriptActorDisappear::initPara(char* SceneData, int index) {
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_index =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	left_top_x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	left_top_y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	right_bottom_x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	right_bottom_y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown7 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
