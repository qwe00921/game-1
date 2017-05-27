#ifndef _ScriptBattleFightEnd_H_
#define _ScriptBattleFightEnd_H_

class ScriptBattleFightEnd  : public Script
{
public:
	 ScriptBattleFightEnd()
	 {
		cmd = CMD_BATTLE_FIGHT_END;
	 }
	 int HandleScript();
};
#endif