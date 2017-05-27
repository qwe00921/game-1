#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"

int ScriptRelaReviveOrMove::HandleRScript(RedeScreenPtr scr)
{
	if (frame++ == 0)
	{
		HistoryActorPtr actor = NULL;
		if (which_index <= 0)
		{
			if (history->var2[5007] == 1 && actor_id1<-2 && history->var2[-actor_id1 - 2] != 5011)
				actor_id1 = history->var2[-actor_id1 - 2];
			if (actor_id1 > trData->actor_num || actor_id1 < 0)
			{
				return 0;
			}
			actor = &history->actors[actor_id1];
		}
		else
		{
			actor = history->battleOwn[out_index];
		}
		if (history->var2[5007] == 1 && actor_id2<-2 && history->var2[-actor_id2 - 2] != 5011)
			actor_id2 = history->var2[-actor_id2 - 2];
		if (actor_id2 > trData->actor_num || actor_id2 < 0)
		{
			return 0;
		}
		actor->dest_x = history->actors[actor_id2].x + rela_x;
		actor->dest_y = history->actors[actor_id2].y + rela_y;
		if (actor->x < actor->dest_x)
		{
			if (actor->y < actor->dest_y)
			{
				if ((int)abs(actor->dest_x - actor->x) < (int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 2;
				}
				else if ((int)abs(actor->dest_x - actor->x) >= (int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 1;
				}
			}
			else
			{
				if ((int)abs(actor->dest_x - actor->x) < (int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 0;
				}
				else if ((int)abs(actor->dest_x - actor->x) >= (int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 1;
				}
			}
		}
		else
		{
			if (actor->y < actor->dest_y)
			{
				if ((int)abs(actor->x - actor->dest_x) < (int)abs(actor->dest_y - actor->y))
				{
					actor->direction = 2;
				}
				else if ((int)abs(actor->dest_x - actor->x) >= (int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 3;
				}
			}
			else
			{
				if ((int)abs(actor->x - actor->dest_x) < (int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 0;
				}
				else if ((int)abs(actor->dest_x - actor->x) >= (int)abs(actor->y - actor->dest_y))
				{
					actor->direction = 3;
				}
			}
		}
		if (actor->style == 0)
			actor->style = 1;
		scr->moveList->addElement(actor);
		scr->rede_state = REDE_STATE_ACTOR_MOVE;
		actor->ai_type = direction;
		return 1;
	}
	return 0;
}
int ScriptRelaReviveOrMove::HandleScript()
{
	if (isRevive)
	{
		return ActorRevive();
	}

	if (frame == 0)
	{
		BattleActorPtr actor = NULL;
		if (which_index <= 0)
		{
			if (history->var2[5007] == 1 && actor_id1<-2 && history->var2[-actor_id1 - 2] != 5011)
				actor_id1 = history->var2[-actor_id1 - 2];
			if (actor_id1 > trData->actor_num || actor_id1 < 0)
			{
				return 0;
			}
			actor = BattleControler::getBattleActor(actor_id1);
		}
		else
		{
			if (out_index >= 0 && out_index < history->battleOwnNum)
			{
				actor = BattleControler::getBattleActor(history->battleOwn[out_index]->actor_id);
			}
		}
		//大于40代表负
		if (rela_x >= 40)
		{
			rela_x = 40 - rela_x;
		}
		if (rela_y >= 40)
		{
			rela_y = 40 - rela_y;
		}
		if (history->var2[5007] == 1 && actor_id2<-2 && history->var2[-actor_id2 - 2] != 5011)
			actor_id2 = history->var2[-actor_id2 - 2];
		if (actor_id2 > trData->actor_num || actor_id2 < 0)
		{
			return 0;
		}
		int dest_x = history->actors[actor_id2].x + rela_x;
		int dest_y = history->actors[actor_id2].y + rela_y;
		if (actor != NULL&&actor->hisActor->hide != 1 && (actor->hisActor->x != dest_x || actor->hisActor->y != dest_y))
		{
			unsigned short x1 = dest_x;
			unsigned short y1 = dest_y;
			history->FindNearestBlackPos(actor->hisActor, x1, y1);
			dest_x = x1;
			dest_y = y1;
			actor->move_end_direction = direction;
			if (actor->moveList == NULL)
			{
				actor->GenerateDefaultMove(dest_x, dest_y);
			}
			need_frame = actor->setState(ACTOR_STATE_MOVE);
		}
		else
			need_frame = 0;
	}

	return need_frame - frame++;

}

int ScriptRelaReviveOrMove::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_index = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_index = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	rela_x = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	rela_y = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	isRevive = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}

int ScriptRelaReviveOrMove::ActorRevive()
{
	HistoryActorPtr actor = NULL;
	if (which_index <= 0)
	{
		if (history->var2[5007] == 1 && actor_id1<-2 && history->var2[-actor_id1 - 2] != 5011)
			actor_id1 = history->var2[-actor_id1 - 2];
		if (actor_id1 > trData->actor_num || actor_id1 < 0)
		{
			return 0;
		}
		actor = &history->actors[actor_id1];
	}
	else
	{
		actor = history->battleOwn[out_index];
	}
	if (actor != NULL)
	{

		if (actor->hide == 1 || actor->style == ACTOR_STATE_LEAVE_DEAD || actor->style == ACTOR_STATE_LEAVE_NORMAL || actor->style == ACTOR_STATE_LEAVE_HIDE)
		{
			history->CenterActor(actor);
			actor->style = ACTOR_STATE_NORMAL;
			//大于40代表负
			if (rela_x >= 40)
			{
				rela_x = 40 - rela_x;
			}
			if (rela_y >= 40)
			{
				rela_y = 40 - rela_y;
			}
			if (history->var2[5007] == 1 && actor_id2<-2 && history->var2[-actor_id2 - 2] != 5011)
				actor_id2 = history->var2[-actor_id2 - 2];
			if (actor_id2 > trData->actor_num || actor_id2 < 0)
			{
				return 0;
			}
			actor->x = history->actors[actor_id2].x + rela_x;
			actor->y = history->actors[actor_id2].y + rela_y;
			history->FindNearestBlackPos(actor, actor->x, actor->y);
			actor->hide = 0;
			actor->is_leave = 0;
			actor->direction = direction;
			actor->InitAbility();
			actor->InitAbilityForRevive();
			history->CheckPosActor(actor);
		}
	}

	return 0;
}

