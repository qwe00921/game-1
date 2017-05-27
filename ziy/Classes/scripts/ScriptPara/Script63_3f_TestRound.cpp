#include "Common.h"
#include "Scripts.h"

int ScriptTestRound::HandleScript() {
	if (!((relation == 0 && history->GetCurRound() >= round_num)
		|| (relation == 1 && history->GetCurRound() < round_num) || (relation == 2 && history->GetCurRound() == round_num))) {
		return -1;
	}
	return 0;
}

int ScriptTestRound::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	round_num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}

