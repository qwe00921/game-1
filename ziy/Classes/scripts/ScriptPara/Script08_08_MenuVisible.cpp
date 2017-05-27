#include "Common.h"
#include "Scripts.h"
//#include "ScriptMenuVisible.h"
#include "RedeScreen.h"

int ScriptMenuVisible::HandleRScript(RedeScreenPtr scr)
{
	scr->setMenuVisible(visible ? true : false);
	return 0;
}

int ScriptMenuVisible::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	visible =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
