#ifndef _ScriptTestActorPos_H_
#define _ScriptTestActorPos_H_


class ScriptTestActorPos : public Script
{
public:
	 ScriptTestActorPos() {
		cmd = CMD_TEST_ACTOR_POS;
	}
	
	 short unkown1;
	
	 short actor_id;

	 short unkown2;

	 int x;

	 short unkown3;

	 int y;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif