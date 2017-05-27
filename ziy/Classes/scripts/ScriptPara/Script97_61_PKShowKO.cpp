#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKShowKO::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKShowKO();
	}
	return 1 - frame++;
}

int ScriptPKShowKO::initPara(char*SceneData,int index){
	return index;
}
