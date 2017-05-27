#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "BattleScreen.h"


int ScriptTestValue::HandleRScript(RedeScreenPtr scr)
{
	int i;
	test_result = true;
	for (i = 0; i < value_true; i++) {
		if (!history->var[true_value_array[i]]) {
			test_result = false;
		}
	}
	if (test_result) {
		for (i = 0; i < value_false; i++) {
			if (history->var[false_value_array[i]]) {
				test_result = false;
			}
		}
	}
	if (!get_test_result()) {
		scr->handler.script_skipto_begin = false;
		return -1;
	}
	scr->handler.script_skipto_begin = true;
	return 0;
}


int ScriptTestValue::HandleScript() {
	int i;
	test_result = true;
	for (i = 0; i < value_true; i++) {
		if (!history->var[true_value_array[i]]) {
			test_result = false;
		}
	}
	if (test_result) {
		for (i = 0; i < value_false; i++) {
			if (history->var[false_value_array[i]]) {
				test_result = false;
			}
		}
	}
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	if (!get_test_result()) {
		scr->handler.handler.script_skipto_begin = false;
		return -1;
	}
	scr->handler.handler.script_skipto_begin = true;
	return 0;
}

int ScriptTestValue::initPara(char* SceneData, int index) {
	int i;
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value_true = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	true_value_array = new short[value_true];
	for (i = 0; i < value_true; i++) {
		true_value_array[i] = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
	}
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value_false = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	false_value_array = new short[value_false];
	for (i = 0; i < value_false; i++) {
		false_value_array[i] = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;
	}
	return index;
}
