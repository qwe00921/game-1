#ifndef _ScriptPKActorAttack1_H_
#define _ScriptPKActorAttack1_H_

class ScriptPKActorAttack1 : public Script
{
public:
	short unkown1; // 2武将序号
	short relation;
	short unkown2; // 0d 动作形态<19
	short attack_type;
	short unkown3;
	short is_deadly;
	ScriptPKActorAttack1() {
		 cmd = CMD_PK_ATTACK1;
	}
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif