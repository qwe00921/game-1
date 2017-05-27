#ifndef _ScriptTestStartBattle_H_
#define _ScriptTestStartBattle_H_

class ScriptTestStartBattle  : public Script
{
public:
	 ScriptTestStartBattle()
	 {
		cmd = CMD_TEST_START_BATTLE;
	 }
	 int HandleRScript(RedeScreenPtr scr);
};
#endif