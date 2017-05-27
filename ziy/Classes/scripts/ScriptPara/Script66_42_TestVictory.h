#ifndef _ScriptTestVictory_H_
#define _ScriptTestVictory_H_



class ScriptTestVictory : public Script
{
public:
	 ScriptTestVictory() {
		cmd = CMD_TEST_VICTORY;
	}
	int HandleScript();
};
#endif