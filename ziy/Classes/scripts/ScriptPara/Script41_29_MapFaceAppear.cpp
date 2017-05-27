#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptMapFaceAppear::HandleRScript(RedeScreenPtr scr)
{
	if(frame == 0)
	{
		scr->cur_actor = &history->actors[actor_id];
		scr->cur_actor->x = x;
		scr->cur_actor->y = y;
		history->RedeActor[history->RedeActorNum++] = scr->cur_actor;
		scr->rede_state = REDE_STATE_MAP_FACE_APPEAR;
		scr->frames = 0;
		need_frame = 9*REDE_MAP_FACE_APPEAR_FRAME;
		scr->InitActorQueue();
	}
	return need_frame-frame++;
}
int ScriptMapFaceAppear::initPara(char*SceneData,int index)
{
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
