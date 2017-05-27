#ifndef _ScriptPKActorAction_H_
#define _ScriptPKActorAction_H_

class ScriptPKActorAction  : public Script
{
public:
	 short unkown1; // 2武将序号
	 short relation;
	 short unkown2; // 0d 动作形态<19
	 short style;
	 ScriptPKActorAction() {
		 cmd = CMD_PK_ACTION;
	 }
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif