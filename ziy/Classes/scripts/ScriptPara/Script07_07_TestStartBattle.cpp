#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
int ScriptTestStartBattle::HandleRScript(RedeScreenPtr scr)
{
	if(scr->rede_state == REDE_STATE_START_BATTLE)
		return 0;
	else
		return -1;
}
