#include "Common.h"
#include "Scripts.h"

int ScriptChangeActorAI::HandleScript()
{
	if(which_index <= 0 || relation == -1)
	{
		HistoryActorPtr actor = &history->actors[actor_id];
		actor->ai_type = ai;
		actor->oppo_actor_id = actor_id2;
		actor->dest_x = desc_x;
		actor->dest_y = desc_y;
	}
	else
	{
		//0我军1友军2敌军3援军4我军及友军5敌军及援军6所有部队
		int i;
		HistoryActorPtr actor;
		switch(relation)
		{
		case 0:
			for(i = 0; i < history->battleOwnNum; i++)
			{
				actor = history->battleOwn[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 1:
			for(i = 0; i < history->battleFriendNum; i++)
			{
				actor = history->battleFriend[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 2:
			for(i = 0; i < history->battleEnemyNum; i++)
			{
				actor = history->battleEnemy[i];
				if(!actor->isAssistArmy && actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 3:
			for(i = 0; i < history->battleEnemyNum; i++)
			{
				actor = history->battleEnemy[i];
				if(actor->isAssistArmy && actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 4:
			for(i = 0; i < history->battleOwnNum; i++)
			{
				actor = history->battleOwn[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			for(i = 0; i < history->battleFriendNum; i++)
			{
				actor = history->battleFriend[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 5:
			for(i = 0; i < history->battleEnemyNum; i++)
			{
				actor = history->battleEnemy[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		case 6:
			for(i = 0; i < history->battleOwnNum; i++)
			{
				actor = history->battleOwn[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			for(i = 0; i < history->battleFriendNum; i++)
			{
				actor = history->battleFriend[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			for(i = 0; i < history->battleEnemyNum; i++)
			{
				actor = history->battleEnemy[i];
				if(actor->inArea(x1, y1, x2, y2))
				{
					actor->ai_type = ai;
					actor->oppo_actor_id = actor_id2;
					actor->dest_x = desc_x;
					actor->dest_y = desc_y;
				}
			}
			break;
		}
	}
	return 0;
}

int ScriptChangeActorAI::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_index = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	ai = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown9 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown10 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	desc_x = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown11 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	desc_y = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
