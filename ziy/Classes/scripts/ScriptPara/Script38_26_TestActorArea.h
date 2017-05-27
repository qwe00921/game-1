#ifndef _ScriptTestActorArea_H_
#define _ScriptTestActorArea_H_


class ScriptTestActorArea  : public Script
{
public:
	 ScriptTestActorArea() {
		cmd = CMD_TEST_ACTOR_AREA;
	}

	 short unkown1;

	 short actor_id;

	 short unkown2;

	 int x1;

	 short unkown3;

	 int y1;

	 short unkown4;

	 int x2;

	 short unkown5;

	 int y2;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif