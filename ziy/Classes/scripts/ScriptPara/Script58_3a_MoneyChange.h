#ifndef _ScriptMoneyChange_H_
#define _ScriptMoneyChange_H_

class ScriptMoneyChange  : public Script
{
public:
	 ScriptMoneyChange() {
		 cmd = CMD_MONEY_CHANGE;
	}

	 short unkown1; // 28

	 short field;

	 short unkown2; // 34

	 short set_type;

	 short unkown3; // 04

	 int num;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif