#ifndef _ScriptTestActorNum_H_
#define _ScriptTestActorNum_H_



class ScriptTestActorNum : public Script
{
public:
	 ScriptTestActorNum() {
		cmd = CMD_TEST_ACTOR_NUM;
	}
	 short unkown1;
	 short relation; //0我军1友军2敌军3援军4我军及友军5敌军及援军6所有部队
	 short unkown2;
	 int num;
	 short unkown3;
	 short compare_type; //0大于等于，1小于，2等于
	 short unkown4;
	 short region; //0全场，1范围
	 short unkown5;
	 int x1;
	 short unkown6;
	 int y1;
	 short unkown7;
	 int x2;
	 short unkown8;
	 int y2;

	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif