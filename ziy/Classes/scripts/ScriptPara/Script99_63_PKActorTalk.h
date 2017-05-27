#ifndef _ScriptPKActorTalk_H_
#define _ScriptPKActorTalk_H_

class ScriptPKActorTalk  : public Script
{
public:
	 short unkown1; // 2武将序号
	 short relation;
	 short unkown2; // 0d 动作形态<19
	 DString buf;
	 short unkown3;
	 short delay;
	 ScriptPKActorTalk() {
		 cmd = CMD_PK_ACTOR_TALK;
	 }
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif