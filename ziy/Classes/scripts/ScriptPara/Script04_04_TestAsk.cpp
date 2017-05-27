#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "PopLayer.h"
int ScriptTestAsk::HandleRScript(RedeScreenPtr scr){
	if (frame++ == 0)
	{
		chess->testAsk = 0;
		if (scr != NULL)
		{
			ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_JB_KYM,NULL));
		}
		return 1;
	}
	if (value_true != chess->testAsk)
	{
		frame = 0;
		return -1;
	}
	return 0;
}

int ScriptTestAsk::HandleScript(){
	if (frame++ == 0)
	{
		chess->testAsk = 0;
		BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
		if (scr != NULL)
		{
			ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_JB_KYM, NULL));
		}
		return 1;
	}
	if (value_true != chess->testAsk)
	{
		frame = 0;
		return -1;
	}
	return 0;
}

int ScriptTestAsk::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	value_true = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
