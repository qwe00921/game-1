#ifndef _ScriptTestActionSide_H_
#define _ScriptTestActionSide_H_


class ScriptTestActionSide  : public Script
{
public:
	 ScriptTestActionSide() {
		cmd = CMD_TEST_ACTION_SIDE;
	}
	
	 short unkown1;

	 short actionside;

	 int HandleScript();
	 int HandleRScript(RedeScreenPtr scr);
	 int initPara(char*SceneData,int index);
};
#endif