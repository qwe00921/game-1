#ifndef _ScriptActorLevelUp_H_
#define _ScriptActorLevelUp_H_



class ScriptActorLevelUp : public Script
{
public:
	 ScriptActorLevelUp() {
		cmd = CMD_ACTOR_LEVEL_UP;
	}
	 short unkown1;
	 short actor_id;
	 short unkown2;
	 int level_num;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif