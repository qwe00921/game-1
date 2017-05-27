#ifndef _ScriptTestValue_H_
#define _ScriptTestValue_H_



class ScriptTestValue : public Script
{
public:
	 ScriptTestValue() {
		cmd = CMD_TEST_VALUE;
	}
	
	short unkown1; // 35 =

	short value_true; // 1;

	short *true_value_array;

	short value_false; // 0

	short *false_value_array;

	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
	 int initPara(char*SceneData,int index);
};
#endif