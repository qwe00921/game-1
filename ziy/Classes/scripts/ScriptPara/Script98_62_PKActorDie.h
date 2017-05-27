#ifndef _ScriptPKActorDie_H_
#define _ScriptPKActorDie_H_

class ScriptPKActorDie  : public Script
{
public:
	short unkown1; // 2武将序号
	short relation;

	ScriptPKActorDie() {
		 cmd = CMD_PK_ACTOR_DIE;
	}
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif