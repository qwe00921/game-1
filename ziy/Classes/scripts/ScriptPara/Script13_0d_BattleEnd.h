#ifndef _ScriptBattleEnd_H_
#define _ScriptBattleEnd_H_



class ScriptBattleEnd : public Script
{
public:
	 ScriptBattleEnd() {
		cmd = CMD_BATTLE_END;
	}
	int HandleRScript(RedeScreenPtr scr); 
	int HandleScript();
};
#endif