#ifndef _ScriptPKStart_H_
#define _ScriptPKStart_H_

class ScriptPKStart : public Script
{
public:
	ScriptPKStart()
	{
		actor_id1 = 0;
		actor_id2 = 0;
		logoIndex = 0;
		cmd = CMD_PK_START;
	}
	int HandleScript();
	int initPara(char*SceneData, int index);
public:
	short unkown1; // 2武将序号
	short actor_id1;
	short unkown2; // 0d 动作形态<19
	short actor_id2;
	short unkown3; // 0d 动作形态<19
	short logoIndex; //背景图
};
#endif