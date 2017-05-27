#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
int ScriptBattleFail::HandleScript() 
{
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	scr->battle_sec = 0;
	return 0;
}
