#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"

int ScriptPaletteSet::HandleRScript(RedeScreenPtr scr) {
	
	if (frame == 0)
	{
		/*if (history->imgBG != NULL)
		{
			DelImage(history->imgBG);
			history->imgBG = NULL;
			history->RedeActorNum = 0;
			scr->InitActorQueue();
		}
		need_frame = 20;*/
	}
	return need_frame - frame++;
}
int ScriptPaletteSet::HandleScript() {
	
	return 0;
}