#ifndef _ScriptActorAction_H_
#define _ScriptActorAction_H_

class ScriptActorAction  : public Script
{
public:
	 short unkown1; // 2武将序号
	 short actor_id;
	 short unkown2; // 0d 动作形态<19
	 short style;
	 ScriptActorAction() {
		 cmd = CMD_ACTOR_ACTION;
	 }
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
	int HandleScript();
};
#endif