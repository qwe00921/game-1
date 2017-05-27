#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorAction::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorAction(relation,style);
	}
	return 1 - frame++;
}

int ScriptPKActorAction::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	style =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}
