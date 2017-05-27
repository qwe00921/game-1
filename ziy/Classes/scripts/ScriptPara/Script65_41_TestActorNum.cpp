#include "Common.h"
#include "Scripts.h"

int ScriptTestActorNum::HandleScript()
{
	test_result = false;
	int cur_num = 0;

	switch(relation) //0我军1友军2敌军3援军4我军及友军5敌军及援军6所有部队
	{
	case 0:
		cur_num = history->getBattleOwnNum(region, x1, y1, x2, y2);
		break;
	case 1:
		cur_num = history->getBattleFriendNum(region, x1, y1, x2, y2);
		break;
	case 2:
		cur_num = history->getBattleEnemyNum(region, x1, y1, x2, y2);
		break;
	case 3:
		cur_num = history->getBattleAssistNum(region, x1, y1, x2, y2);
		break;
	case 4:
		cur_num = history->getBattleOwnNum(region, x1, y1, x2, y2) + history->getBattleFriendNum(region, x1, y1, x2, y2);
		break;
	case 5:
		cur_num = history->getBattleEnemyNum(region, x1, y1, x2, y2) + history->getBattleAssistNum(region, x1, y1, x2, y2);
		break;
	case 6:
		cur_num = history->getBattleOwnNum(region, x1, y1, x2, y2) + history->getBattleFriendNum(region, x1, y1, x2, y2) + history->getBattleEnemyNum(region, x1, y1, x2, y2) + history->getBattleAssistNum(region, x1, y1, x2, y2);
		break;
	default:
		break;
	}

	switch(compare_type)
	{
	case 0:
		if(cur_num >= num)
			test_result = true;
		break;
	case 1:
		if(cur_num < num)
			test_result = true;
		break;
	case 2:
		if(cur_num == num)
			test_result = true;
		break;
	}
	if(!get_test_result())
		return -1;
	return 0;
}

int ScriptTestActorNum::initPara(char* SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	compare_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	region = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown5 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y1 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown7 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y2 = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
