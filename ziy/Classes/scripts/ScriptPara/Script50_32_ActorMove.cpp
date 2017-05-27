#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptActorMove::HandleRScript(RedeScreenPtr scr)
{
	if(frame++ == 0)
	{
		HistoryActorPtr actor;
		if(which_index == 1)
		{
			actor = (HistoryActorPtr)history->battleOwn[out_index];
		}
		else
		{
			if (history->var2[5007] == 1 && actor_id<-2 && history->var2[-actor_id - 2] != 5011)
				actor_id = history->var2[-actor_id - 2];
			if (actor_id > trData->actor_num || actor_id < 0)
			{
				return 0;
			}
			actor = &history->actors[actor_id];
		}
		actor->dest_x = x;
		actor->dest_y = y;
		if(actor->x < actor->dest_x)
		{
			if(actor->y < actor->dest_y)
			{
				if((int)abs(actor->dest_x - actor->x)<(int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 2;
				}
				else if((int)abs(actor->dest_x - actor->x)>=(int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 1;
				}
			}
			else
			{
				if((int)abs(actor->dest_x - actor->x)<(int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 0;
				}
				else if((int)abs(actor->dest_x - actor->x)>=(int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 1;
				}
			}
		}
		else
		{
			if(actor->y < actor->dest_y)
			{
				if((int)abs(actor->x - actor->dest_x)<(int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 2;
				}
				else if((int)abs(actor->dest_x - actor->x)>=(int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 3;
				}
			}
			else
			{
				if((int)abs(actor->x - actor->dest_x)<(int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 0;
				}
				else if((int)abs(actor->dest_x - actor->x)>=(int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 3;
				}
			}
		}
		if(actor->style == 0)
			actor->style = 1;
		scr->moveList->addElement(actor);
		scr->rede_state = REDE_STATE_ACTOR_MOVE;
		actor->ai_type = direction;
		return 1;
	}
	return 0;
}
int ScriptActorMove::HandleScript(){
	if(frame == 0)
	{
		BattleActorPtr actor = NULL;
		if(which_index <= 0)
		{
			if (history->var2[5007] == 1 && actor_id<-2 && history->var2[-actor_id - 2] != 5011)
				actor_id = history->var2[-actor_id - 2];
			if (actor_id > trData->actor_num || actor_id < 0)
			{
				return 0;
			}
			actor = BattleControler::getBattleActor(actor_id);
		}
		else
		{
			if (out_index >= 0 && out_index < history->battleOwnNum)
			{
				actor = BattleControler::getBattleActor(history->battleOwn[out_index]->actor_id);
			}
		}
		if(actor!=NULL&&actor->hisActor->hide!=1&&(actor->hisActor->x!=x||actor->hisActor->y!=y))
		{
			unsigned short x1 = x;
			unsigned short y1 = y;
			history->FindNearestBlackPos(actor->hisActor, x1, y1);
			x = x1;
			y = y1;
			actor->move_end_direction = direction;
			if (actor->moveList == NULL)
			{
				actor->GenerateDefaultMove(x,y);
			}
			need_frame = actor->setState(ACTOR_STATE_MOVE);
		}
		else
			need_frame = 0;
	}

	return need_frame - frame++;
}

int ScriptActorMove::initPara(char*SceneData,int index){
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
	out_index =  bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}