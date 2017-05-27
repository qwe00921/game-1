#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptMapFaceMove::HandleRScript(RedeScreenPtr scr)
{
	if(frame == 0)
	{
		scr->cur_actor = &history->actors[actor_id];
		scr->cur_actor->dest_x = x;
		scr->cur_actor->dest_y = y;
		scr->rede_state = REDE_STATE_MAP_FACE_MOVE;
		scr->frames = 0;
		int dx = (int)abs(x - scr->cur_actor->x)/2+1;
		int dy = (int)abs(y - scr->cur_actor->y)/2+1;
		need_frame = dx<dy?dy:dx;
	}
	return need_frame-frame++;
}
int ScriptMapFaceMove::initPara(char*SceneData,int index)
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
