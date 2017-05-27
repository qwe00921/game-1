#ifndef _ScriptTestFail_H_
#define _ScriptTestFail_H_



class ScriptTestFail : public Script
{
public:
	 ScriptTestFail() {
		cmd = CMD_TEST_FAIL;
	}
	int HandleScript();
};
#endif