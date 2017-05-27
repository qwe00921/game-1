#ifndef _ScriptPKActorAppear_H_
#define _ScriptPKActorAppear_H_

class ScriptPKActorAppear  : public Script
{
public:
	 short unkown1; // 2武将序号
	 short relation;
	 short unkown2;
	 DString info;
	 short unkown3;
	 short action;
	 ScriptPKActorAppear() {
		 cmd = CMD_PK_ACTOR_APPEAR;
	 }
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif