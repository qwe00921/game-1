#ifndef _ScriptTestArmyCome_H_
#define _ScriptTestArmyCome_H_
class ScriptTestArmyCome : public Script
{
public:
	 ScriptTestArmyCome() {
		cmd = CMD_TEST_ARMY_COME;
	}
	int HandleScript();
};
#endif