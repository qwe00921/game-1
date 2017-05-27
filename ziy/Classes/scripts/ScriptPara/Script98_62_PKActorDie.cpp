#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorDie::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorDie(relation);
	}
	return 1 - frame++;
}

int ScriptPKActorDie::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
