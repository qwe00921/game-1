#include "Common.h"
#include "Scripts.h"
#include "ScriptDispatchOneFriend.h"

int ScriptDispatchOneFriend::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	hide =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	level_add =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown7 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	occu_limit =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown8 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	ai_type =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown9 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	oppo_actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown10 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	dest_x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown11 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	dest_y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;		
	return index;
}