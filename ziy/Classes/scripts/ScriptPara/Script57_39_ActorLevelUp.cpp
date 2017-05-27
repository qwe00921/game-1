#include "Common.h"
#include "Scripts.h"

int ScriptActorLevelUp::HandleScript() 
{
	return 0;
}

int ScriptActorLevelUp::initPara(char* SceneData, int index) 
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	level_num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
