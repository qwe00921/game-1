#include "Common.h"
#include "Scripts.h"
//#include "ScriptDraw.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
int ScriptDraw::HandleRScript(RedeScreenPtr scr) 
{
	return 0;
}
int ScriptDraw::HandleScript() {
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	if(frame == 0)
	{
		need_frame = 1;
		scr->handler.battle_state = BATTLE_STATE_SHOW_BATTLE_START;
		scr->handler.need_frame = 201;
	}
	return need_frame - frame++;
}