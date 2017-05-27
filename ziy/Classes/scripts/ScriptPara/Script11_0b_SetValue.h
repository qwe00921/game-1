#ifndef _ScriptSetValue_H_
#define _ScriptSetValue_H_


class ScriptSetValue  : public Script
{
public:
	 ScriptSetValue() {
		cmd = CMD_SET_VALUE;
	}
	
	 short unkown1; // 4变量序号

	 int id;

	 short unkown2; // 39 变量值

	 short value;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif