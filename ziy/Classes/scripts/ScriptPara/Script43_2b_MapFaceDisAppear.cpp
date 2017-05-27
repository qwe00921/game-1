#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptMapFaceDisAppear::HandleRScript(RedeScreenPtr scr)
{
	if(frame == 0)
	{
		scr->cur_actor = &history->actors[actor_id];
		scr->rede_state = REDE_STATE_MAP_FACE_DISAPPEAR;
		scr->frames = 0;
		need_frame = 9*REDE_MAP_FACE_APPEAR_FRAME;
	}
	if(need_frame == frame+1)
	{
		for(int i = 0;i<history->RedeActorNum;i++)
		{
			if(history->RedeActor[i] == scr->cur_actor)
			{
				for(int j = i;j<history->RedeActorNum-1;j++)
				{
					history->RedeActor[j] = history->RedeActor[j + 1];
				}
				break;
			}
		}
		history->RedeActorNum--;
		scr->cur_actor = NULL;
		scr->InitActorQueue();
	}
	return need_frame-frame++;
}
int ScriptMapFaceDisAppear::initPara(char*SceneData,int index)
{
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
