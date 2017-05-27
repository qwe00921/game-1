#include "Common.h"
#include "Scripts.h"

int ScriptTestValuePlus::getNum1()
{
	switch (value_type1)
	{
	case 0:
	case 1:
		return num1;
// 	case 1:
// 		return *(history->var3[num1]);
	case 2:
		return (int)((long)history->var3[num1]);
	case 3:
		return (int)((long)&(history->var3[num1]));
	case 4:
		return history->var2[num1];
	case 5:
		return (int)((long)&(history->var2[num1]));
	default:
		return num1;
	}
}
int ScriptTestValuePlus::getNum2()
{
	switch (value_type2)
	{
	case 0:
		return num2;
	case 1:
		return *(history->var3[num2]);
	case 2:
		return (int)((long)history->var3[num2]);
	case 3:
		return (int)((long)&(history->var3[num2]));
	case 4:
		return history->var2[num2];
	case 5:
		return (int)((long)&(history->var2[num2]));
	default:
		return num2;
	}
}

int ScriptTestValuePlus::HandleScript() {
	test_result = false;

	switch (operation_type)
	{
	case 0:
		if (getNum1() == getNum2())
			test_result = true;
		break;
	case 1:
		if (getNum1() >= getNum2())
			test_result = true;
		break;
	case 2:
		if (getNum1() <= getNum2())
			test_result = true;
		break;
	default:
		break;
	}

	if (!get_test_result()) {
		return -1;
	}
	return 0;
}

int ScriptTestValuePlus::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value_type1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num1 = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	operation_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value_type2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num2 = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}

