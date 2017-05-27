#ifndef _ScriptMoneyTest_H_
#define _ScriptMoneyTest_H_

class ScriptMoneyTest  : public Script
{
public:
	 ScriptMoneyTest() {
		 cmd = CMD_MONEY_SCENE_ROUTE_TEST;
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