#ifndef _Script121_79_TestValuePlus_H_
#define _Script121_79_TestValuePlus_H_
class ScriptTestValuePlus : public Script
{
public:
	ScriptTestValuePlus() {
		cmd = CMD_TEST_VALUEPLUS;
	}
	
	short unkown1;
	short value_type1;
	short unkown2;
	int num1;
	short unkown3;
	short operation_type;
	short unkown4;
	short value_type2;
	short unkown5;
	int num2;

	int HandleScript();
	int initPara(char*SceneData, int index);
	int getNum1();
	int getNum2();
};
#endif