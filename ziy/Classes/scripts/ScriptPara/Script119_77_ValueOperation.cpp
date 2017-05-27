#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"

int ScriptValueOperation::getNum2()
{
	switch (value_type2)
	{
	case 0:
		return num2;
	case 1:
		return *((int*)history->var3[num2]);
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
int ScriptValueOperation::HandleScript() {
	switch (operation_type)
	{
	case 0:
		switch (value_type1)
		{
		case 0:
			break;
		case 1:
 			break;
		case 2:
			history->var2[num1] += getNum2();
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (value_type1)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			history->var2[num1] -= getNum2();
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (value_type1)
		{
		case 0:
			break;
 		case 1:
 			break;
		case 2:
			history->var2[num1] = getNum2();
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (value_type1)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
            history->var2[num1] *= getNum2();
			break;
		default:
			break;
		}
		break;
	case 4:
		switch (value_type1)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			history->var2[num1] /= getNum2();
			break;
		default:
			break;
		}
		break;
	case 5:
		switch (value_type1)
		{
 		case 0:
 			break;
 		case 1:
 			break;
		case 2:
			history->var2[num1] %= getNum2();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}

int ScriptValueOperation::initPara(char*SceneData, int index)
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
