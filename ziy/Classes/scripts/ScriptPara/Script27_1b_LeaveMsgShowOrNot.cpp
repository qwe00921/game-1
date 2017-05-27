#include "Common.h"
#include "Scripts.h"

int ScriptLeaveMsgShowOrNot::HandleScript() {
	return 0;
}

int ScriptLeaveMsgShowOrNot::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	show = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
