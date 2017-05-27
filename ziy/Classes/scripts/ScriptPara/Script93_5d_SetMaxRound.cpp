#include "Common.h"
#include "Scripts.h"

int ScriptSetMaxRound::HandleScript() {
	switch(set_type) {
	case 0:
		history->SetMaxRound(num + history->iExtraAddMaxRound);
		break;
	case 1:
		history->SetMaxRound(-1, num);
		break;
	case 2:
		history->SetMaxRound(-1, -num);
		break;
	}

	return 0;
}

int ScriptSetMaxRound::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
