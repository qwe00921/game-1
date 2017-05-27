#include "Common.h"
#include "Scripts.h"
#include "PKScreen.h"
int ScriptPKEnd::HandleScript(){
	if(frame == 0)
	{
		PKScreenPtr scr = (PKScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_PK);
		if(scr!=NULL)
			scr->PKEnd();
	}
	return 1 - frame++;
}

int ScriptPKEnd::initPara(char*SceneData,int index){
	return index;
}
