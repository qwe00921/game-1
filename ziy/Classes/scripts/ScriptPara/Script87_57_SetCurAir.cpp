#include "Common.h"
#include "Scripts.h"

int ScriptSetCurAir::HandleScript()
{
	history->updateWeather(history->m_iAirArray[history->m_iAirType * 6 + cur_air] + 1);
	return 0;
}

int ScriptSetCurAir::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	cur_air = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}