#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "BattleScreen.h"

int ScriptGameEnding::HandleRScript(RedeScreenPtr scr)
{
	scr->gameEnding();
	return 1;//·µ»Ø1½áÊø 0¼ÌÐø
}

int ScriptGameEnding::HandleScript()
{
	return 1;
}


int ScriptGameEnding::initPara(char*SceneData, int index){
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	ending_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	
	return index;
}
