#ifndef _ScriptChangeActorOccu_H_
#define _ScriptChangeActorOccu_H_
class ScriptChangeActorOccu : public Script
{
public:
	 ScriptChangeActorOccu() {
		cmd = CMD_CHANGE_ACTOR_OCCU;
	}
	 short unkown1;
	 short actor_id;
	 short unkown2;
	 short occu_id;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif