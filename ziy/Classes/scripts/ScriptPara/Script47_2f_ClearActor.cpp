#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"

int ScriptClearActor::HandleRScript(RedeScreenPtr scr) 
{
	history->RedeActorNum= 0;
	scr->InitActorQueue();
	return 0;
}
