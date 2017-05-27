#include "Common.h"
#include "Scripts.h"
#include "WinConditionTool.h"

int ScriptSetValue::HandleScript()
{
	if(value == 0)
		history->var[id] = 0;
	else
	{
		history->var[id] = 1;
		WinConditionTool::getInstance()->AddScriptCodeElem(id);
	}
	return 0;
}


int ScriptSetValue::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	id = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
