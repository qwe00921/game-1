#ifndef _ScriptSetMaxRound_H_
#define _ScriptSetMaxRound_H_



class ScriptSetMaxRound : public Script
{
public:
	 ScriptSetMaxRound() {
		cmd = CMD_SET_MAX_ROUND;
	}
	 short unkown1;
	 short set_type;
	 short unkown2;
	 int num;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif