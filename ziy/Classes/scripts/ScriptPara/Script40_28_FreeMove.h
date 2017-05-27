#ifndef _ScriptFreeMove_H_
#define _ScriptFreeMove_H_

class ScriptFreeMove : public Script
{
public:
	ScriptFreeMove() {
		cmd = CMD_28;
	}
	short unkown1; // 36

	short actorid;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData, int index);
};
#endif