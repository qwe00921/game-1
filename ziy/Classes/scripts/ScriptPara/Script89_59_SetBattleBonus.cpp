#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "StatConfig.h"

int ScriptSetBattleBonus::HandleScript()
{
	return 0;
}

int ScriptSetBattleBonus::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	money = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_id1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_level1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_id2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_level2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_id3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	good_level3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	is_end = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
