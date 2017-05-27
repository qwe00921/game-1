#include "Common.h"
#include "Scripts.h"

int ScriptChangeViewPoint::HandleScript() {
	history->CenterPos(x,y);
	return 0;
}

int ScriptChangeViewPoint::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
