#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "button.h"

int ScriptFreeMove::HandleRScript(RedeScreenPtr scr)
{
	history->isFreeMove = actorid;
	if (history->isFreeMove)
	{
		scr->setMenuVisible(true);
	}
	else
	{
		if(scr->GetMenuVisible())
		{
			scr->setMenuVisible(true);
		}
	}
	return 0;
}

int ScriptFreeMove::initPara(char*SceneData, int index){
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actorid = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}