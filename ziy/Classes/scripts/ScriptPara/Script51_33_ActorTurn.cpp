#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"


int ScriptActorTurn::HandleRScript(RedeScreenPtr scr)
{
	history->actors[actor_id].direction = direction;
	history->actors[actor_id].style = style>0?style+2:0;
	return 0;
}

int ScriptActorTurn::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	style =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}