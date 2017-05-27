#include "Common.h"
#include "Scripts.h"

int ScriptSetEnemyGoods::HandleScript()
{
	return 0;
}

int ScriptSetEnemyGoods::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	weapon_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	weapon_level = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	coat_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	coat_level = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	assist_good_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
