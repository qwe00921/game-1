#include "Common.h"
#include "Scripts.h"

int ScriptTestActorNotSame::HandleScript()
{
	if(actor_id1 == actor_id2)
		return -1;
	return 0;
}

int ScriptTestActorNotSame::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
