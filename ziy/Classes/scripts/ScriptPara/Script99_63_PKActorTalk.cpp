#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKActorTalk::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKActorTalk(relation,buf,delay);
	}
	return 1 - frame++;
}

int ScriptPKActorTalk::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	buf = bytesToString(SceneData, index);
	index += buf->length()+1;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
