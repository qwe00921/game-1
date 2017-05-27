#ifndef _ScriptActorTurn_H_
#define _ScriptActorTurn_H_


class ScriptActorTurn  : public Script
{
public:
	 ScriptActorTurn() {
		cmd = CMD_ACTOR_TURN;
	}

	 short unkown1; // 2武将序号

	 short actor_id;

	 short unkown2; // 0d 动作形态<19

	 short style;

	 short unkown3; // 43朝向

	 short direction;
	 int HandleRScript(RedeScreenPtr scr);
	 int initPara(char*SceneData,int index);
};
#endif