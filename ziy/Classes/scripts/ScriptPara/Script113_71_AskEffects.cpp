#include "Common.h"
#include "Scripts.h"

int ScriptAskEffects::HandleScript() {
	return 0;
}

int ScriptAskEffects::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
