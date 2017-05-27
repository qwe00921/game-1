#ifndef _Script119_77_ValueOperation_H_
#define _Script119_77_ValueOperation_H_
class ScriptValueOperation : public Script
{
public:
	ScriptValueOperation() {
		cmd = CMD_VALUE_OPERATION;
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
	int getNum2();
};
#endif