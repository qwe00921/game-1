#ifndef _Script110_6e_TestProbality_H_
#define _Script110_6e_TestProbality_H_
class ScriptTestProbality : public Script
{
public:
	ScriptTestProbality() {
		cmd = CMD_TEST_PROBALITY;
	}

	short unkown1;
	int probality;
	
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif