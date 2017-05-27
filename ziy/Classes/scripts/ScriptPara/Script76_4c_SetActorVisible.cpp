#include "Common.h"
#include "Scripts.h"

int ScriptSetActorVisible::HandleScript(){
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

		HistoryActorPtr hisActor = NULL;
		if (which_index <= 0)
		{
			int setted = 0;
			for (int i = 0; i < history->battleOwnNum;i++)
			{	
				if (history->battleOwn[i]->actor_id == actor_id_replace)
				{
					hisActor = history->battleOwn[i];
					history->FindNearestBlackPos(hisActor, hisActor->x, hisActor->y);
					history->battleOwn[i]->hide = 0;
					history->battleOwn[i]->is_leave = 0;
					history->CenterActor(history->battleOwn[i]);
					setted = 1;
				}
			}
			if (setted == 0)
			{
				hisActor = &history->actors[actor_id_replace];
				history->FindNearestBlackPos(hisActor, hisActor->x, hisActor->y);
				history->actors[actor_id_replace].hide = 0;
				history->actors[actor_id_replace].is_leave = 0;
				history->CenterActor(&history->actors[actor_id_replace]);
			}
		}
		else
		{
			if (history->battleOwn[out_index] != NULL)
			{
				hisActor = history->battleOwn[out_index];
				history->FindNearestBlackPos(hisActor, hisActor->x, hisActor->y);
				history->battleOwn[out_index]->hide = 0;
				history->battleOwn[out_index]->is_leave = 0;
				history->CenterActor(history->battleOwn[out_index]);
			}
		}

		if(hisActor)
		{
			hisActor->InitAbilityForRevive();
		}
		playSound(SOUND_ACTOR_APPEAR);
		need_frame = TIMER_ACTOR_APPEAR;
	}
	return need_frame - frame++;
}

int ScriptSetActorVisible::initPara(char*SceneData,int index){
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
	out_index =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	return index;
}

