#ifndef _ScriptBattleFail_H_
#define _ScriptBattleFail_H_
class ScriptBattleFail : public Script
{
public:
	 ScriptBattleFail() {
		cmd = CMD_BATTLE_FAIL;
	}
	int HandleScript();
};
#endif