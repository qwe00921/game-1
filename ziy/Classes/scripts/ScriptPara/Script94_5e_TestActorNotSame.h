#ifndef _ScriptTestActorNotSame_H_
#define _ScriptTestActorNotSame_H_



class ScriptTestActorNotSame : public Script
{
public:
	 ScriptTestActorNotSame() {
		cmd = CMD_TEST_ACTOR_NOT_SAME;
	}
	 short unkown1;
	 short actor_id1;
	 short unkown2;
	 short actor_id2;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif