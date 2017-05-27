#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "BattleScreen.h"

int ScriptTestActionSide::HandleRScript(RedeScreenPtr scr)
{
	if (actionside != history->curActionSide)
	{
		scr->handler.script_skipto_begin = false;
		return -1;
	}
	scr->handler.script_skipto_begin = true;
	return 0;
}

int ScriptTestActionSide::HandleScript(){
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	if(actionside != history->curActionSide)
	{
		scr->handler.handler.script_skipto_begin = false;
		return -1;
	}
	scr->handler.handler.script_skipto_begin = true;
	return 0;
}

int ScriptTestActionSide::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actionside =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
