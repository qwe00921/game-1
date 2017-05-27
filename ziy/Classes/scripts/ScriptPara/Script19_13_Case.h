#ifndef _ScriptCase_H_
#define _ScriptCase_H_


class ScriptCase  : public Script
{
public:
	 ScriptCase() {
		cmd = CMD_CASE;
	}

	 short unkown1; // 4 选项序号

	 int id;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif