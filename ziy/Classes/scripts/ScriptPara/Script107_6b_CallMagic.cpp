#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"

int ScriptCallMagic::HandleScript()
{
	return need_frame - frame++;
}

int ScriptCallMagic::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	magic_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	visible = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
