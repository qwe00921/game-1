#ifndef _ScriptTestRound_H_
#define _ScriptTestRound_H_



class ScriptTestRound : public Script
{
public:
	 ScriptTestRound() {
		cmd = CMD_TEST_ROUND;
	}
	
	 short unkown1;

	 int round_num;

	 short unkown2;

	 short relation;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif