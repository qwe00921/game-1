#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "BattleScreen.h"

int ScriptSkipUncondition::HandleRScript(RedeScreenPtr scr)
{
	scr->handler.SkipScript(script_id);
	return 0;
}
int ScriptSkipUncondition::HandleScript() {
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	scr->handler.handler.SkipScript(script_id);
	return 0;
}

int ScriptSkipUncondition::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	script_id = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	return index;
}
