#include "Common.h"
#include "Scripts.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "BattleEndLayer.h"
#include "BattleReplay.h"

int ScriptBattleEnd::HandleRScript(RedeScreenPtr scr)
{
	if(history->isFreeMove)
	{
		scr->handler.list_index ++;
	}
	return 0;
}
int ScriptBattleEnd::HandleScript() 
{
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	BattleReplay::getInstance()->stopSaveReplay();

	if(scr->battle_sec || history->m_iBattleType == BATTLE_CLANARMYBATTLE)//var[0]代表成功还是失败	
	{
		scr->handler.battle_state = BATTLE_STATE_ENDING_WIN;
		scr->handler.handler.SetScriptList(2);
		history->game_scene++;
		return 1;
	}
	else
	{
		scr->handler.battle_state = BATTLE_STATE_ENDING_LOSE;
		scr->resetWindow();
		ChessStopMp3();
	}
	return 0;
}