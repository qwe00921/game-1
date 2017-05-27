#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorAttack2::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorAttack2(relation,attack_type,is_hitted);
	}
	return 1 - frame++;
}

int ScriptPKActorAttack2::initPara(char*SceneData,int index){
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
	is_hitted =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}
