#include "Common.h"
#include "Scripts.h"
#include "StatConfig.h"
int ScriptTestFail::HandleScript() {
	test_result = false;
	if (history->GetMaxRound() < history->GetCurRound())
	{
		test_result = true;
		char filename[40];
		if (history->game_scene < 10)
		{
			sprintf(filename, "%s%d%s", "S_0", history->game_scene, ".eex_new");
		}
		else
		{
			sprintf(filename, "%s%d%s", "S_", history->game_scene, ".eex_new");
		}
		StatConfig::onFailedMission(filename,"maxround");
	}
// 	else if(history->own_skipper_index>=0)
// 	{
// 		HistoryActorPtr actor  = history->FindBattleActor(history->own_skipper_index);
// 		if (actor != NULL)
// 		{
// 			if (actor->style == ACTOR_STATE_LEAVE_DEAD ||
// 				actor->style == ACTOR_STATE_LEAVE_HIDE ||
// 				actor->style == ACTOR_STATE_LEAVE_NORMAL)
// 			{
// 				test_result = true;
// 				char filename[40];
// 				if (history->game_scene < 10)
// 				{
// 					sprintf(filename, "%s%d%s", "S_0", history->game_scene, ".eex_new");
// 				}
// 				else
// 				{
// 					sprintf(filename, "%s%d%s", "S_", history->game_scene, ".eex_new");
// 				}
// 				StatConfig::onFailedMission(filename, "ownskipperdead");
// 			}
// 		}
// 		
// 	}
	else
	{
 		test_result = true;
 		int i;
 		for (i = 0; i < history->battleOwnNum; i++) {
 			if (history->battleOwn[i]->hide!=1)
 			{
 				test_result = false;
 			}
 		}
	}
	if(!get_test_result())
		return -1;
	return 0;
}