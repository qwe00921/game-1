#include "Common.h"
#include "Scripts.h"
#include "StatConfig.h"
int ScriptTestVictory::HandleScript() {
	test_result = false;
	if(history->enemy_skipper_index>=0)
	{
		HistoryActorPtr actor = history->FindBattleActor(history->enemy_skipper_index);
		if (actor != NULL)
		{
			if (actor->style == ACTOR_STATE_LEAVE_DEAD ||
				actor->style == ACTOR_STATE_LEAVE_HIDE ||
				actor->style == ACTOR_STATE_LEAVE_NORMAL)
			{
				test_result = true;
			}
		}
	}
// 	if(history->is_victory_area)
// 	{
// 		for(int i = 0;i<history->battleOwnNum;i++)
// 		{
// 			if(history->battleOwn[i]->inArea(history->victory_x1,history->victory_y1,history->victory_x2,history->victory_y2))
// 			{
// 				test_result = true;
// 				break;
// 			}
// 		}
// // 		for(int i = 0;i<history->battleFriendNum;i++)
// // 		{
// // 			if(history->battleFriend[i]->inArea(history->victory_x1,history->victory_y1,history->victory_x2,history->victory_y2))
// // 			{
// // 				test_result = true;
// // 				break;
// // 			}
// // 		}
// 	}
	if(test_result==false)
	{
		test_result = true;
		int i;
		for (i = 0; i < history->battleEnemyNum; i++) {
			if (history->battleEnemy[i]->hide!=1)
			{
				test_result = false;
			}
		}

	}
	if (test_result == true)
	{
		for (int i = 0; i < history->battleOwnNum; i++) {
			if (history->battleOwn[i]->hide != 1 && history->battleOwn[i]->controlled_by_enemy)
			{
				test_result = false;
			}
		}
	}
	if(!get_test_result())
		return -1;
	return 0;
}