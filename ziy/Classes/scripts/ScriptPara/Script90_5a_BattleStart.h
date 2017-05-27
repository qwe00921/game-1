#ifndef _ScriptBattleStart_H_
#define _ScriptBattleStart_H_


class ScriptBattleStart : public Script
{
public:
	ScriptBattleStart()
	{
		cmd = CMD_BATTLE_START;
	}
	int HandleScript();
};
#endif