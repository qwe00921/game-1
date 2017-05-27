#ifndef _ScriptChangeActorAI_H_
#define _ScriptChangeActorAI_H_
class ScriptChangeActorAI : public Script
{
public:
	ScriptChangeActorAI()
	{
		cmd = CMD_CHANGE_ACTOR_AI;
	}
	short unkown1;
	short which_index;
	short unkown2;
	short actor_id;
	short unkown3;
	int x1;
	short unkown4;
	int y1;
	short unkown5;
	int x2;
	short unkown6;
	int y2;
	short unkown7;
	short relation;
	short unkown8;
	short ai;
	short unkown9;
	short actor_id2;
	short unkown10;
	int desc_x;
	short unkown11;
	int desc_y;
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif