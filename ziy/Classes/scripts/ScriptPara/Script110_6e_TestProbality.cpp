#include "Common.h"
#include "Scripts.h"

int ScriptTestProbality::HandleScript()
{
	test_result = rand() % 100 < probality ? true : false;

	if (!get_test_result())
	{
		return -1;
	}
	return 0;
}

int ScriptTestProbality::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	probality = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
