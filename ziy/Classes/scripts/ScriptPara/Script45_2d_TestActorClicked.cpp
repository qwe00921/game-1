#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptTestActorClicked::HandleRScript(RedeScreenPtr scr){
	if (scr->cur_actor != NULL && (scr->cur_actor->actor_id == actor_id))
	{
		if (history->isFreeMove)
		{
			scr->cur_actor = NULL;
		}
		return 0;
	}
	else
		return -1;
}

int ScriptTestActorClicked::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
