#include "Common.h"
#include "Scripts.h"

int ScriptSetAirType::HandleScript() {
	history->m_iAirType = air_id;
	return 0;
}

int ScriptSetAirType::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	air_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
