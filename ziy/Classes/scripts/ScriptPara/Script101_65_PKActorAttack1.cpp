#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorAttack1::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorAttack1(relation,attack_type,is_deadly);
	}
	return 1 - frame++;
}

int ScriptPKActorAttack1::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	attack_type =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	is_deadly =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}
