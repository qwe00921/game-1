#ifndef _ScriptTestActorBorder_H_
#define _ScriptTestActorBorder_H_

class ScriptTestActorBorder  : public Script
{
public:
	ScriptTestActorBorder() {
		cmd = CMD_TEST_ACTOR_BORDER;
	}
	short unkown1;
	short actor_id1;
	short unkown2;
	short actor_id2;
	short unkown3;
	short attackAreaCareLess;

	int HandleScript();
	int initPara(char*SceneData,int index);
	bool testBorder(int id1,int id2);
	bool testBorder2(int id1,int id2);
};
#endif