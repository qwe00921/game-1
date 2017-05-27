#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorAppear::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorAppear(relation,action,info);
	}
	return 1 - frame++;
}

int ScriptPKActorAppear::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	info = bytesToString(SceneData, index);
	index += info->length()+1;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	action =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
