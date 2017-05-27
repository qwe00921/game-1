#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
int ScriptBattleFightEnd::HandleScript()
{
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	BattleActorPtr actor;
	if(scr!=NULL)
	{
		for(int i = 0;i<scr->handler.actor_list->size();i++)
		{
			actor = (BattleActorPtr)scr->handler.actor_list->elementAt(i);
			actor->setState(ACTOR_STATE_NORMAL);
		}
	}
	return 0;
}
