#include "Common.h"
#include "Scripts.h"
int ScriptMoneyTest::HandleScript(){
	test_result = false;
	//0 钱 1 剧本 2 野心
	switch (field) {
	case 0:
		if ((set_type == 2 && history->money == num)
			||(set_type == 0 && history->money >= num)
			|| (set_type == 1 && history->money < num)) 
		{
			test_result = true;
		}
		break;
	case 1:
		if ((set_type == 2 && history->game_scene == num)
			||(set_type == 0 && history->game_scene >= num)
			|| (set_type == 1 && history->game_scene < num)) 
		{
			test_result = true;
		}
		break;
	case 2:
		if ((set_type == 2 && history->extend_value == num)
			||(set_type == 0 && history->extend_value >= num)
			|| (set_type == 1 && history->extend_value < num)) 
		{
			test_result = true;
		}
		break;
	}
	if (!get_test_result()) {
		return -1;
	}
	return 0;

}

int ScriptMoneyTest::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	field =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	set_type =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;	
	return index;
}
