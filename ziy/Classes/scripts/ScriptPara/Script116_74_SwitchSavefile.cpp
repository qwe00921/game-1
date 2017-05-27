#include "Common.h"
#include "Scripts.h"

int ScriptSwitchSavefile::HandleScript() {
	return 0;
}

int ScriptSwitchSavefile::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
