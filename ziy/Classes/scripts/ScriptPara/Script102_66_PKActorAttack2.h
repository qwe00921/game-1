#ifndef _ScriptPKActorAttack2_H_
#define _ScriptPKActorAttack2_H_

class ScriptPKActorAttack2  : public Script
{
public:
	 short unkown1; // 2武将序号
	 short relation;
	 short unkown2; // 0d 动作形态<19
	 short attack_type;
	 short unkown3;
	 short is_hitted;
	 ScriptPKActorAttack2() {
		 cmd = CMD_PK_ATTACK2;
	 }
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif