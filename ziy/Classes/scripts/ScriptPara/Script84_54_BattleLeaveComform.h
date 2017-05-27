#ifndef _ScriptBattleLeaveComform_H_
#define _ScriptBattleLeaveComform_H_
class ScriptBattleLeaveComform : public Script
{
public:
	 ScriptBattleLeaveComform() {
		cmd = CMD_BATTLE_LEAVE_COMFORM;
	}
	int HandleScript();
};
#endif