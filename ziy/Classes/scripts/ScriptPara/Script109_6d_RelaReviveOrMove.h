#ifndef _Script109_6d_RelaReviveOrMove_H_
#define _Script109_6d_RelaReviveOrMove_H_
class ScriptRelaReviveOrMove : public Script
{
public:
	ScriptRelaReviveOrMove() {
		cmd = CMD_RELA_REVIVEORMOVE;
	}

	short unkown1;
	short which_index;
	short unkown2;
	short actor_id1;
	short unkown3;
	int out_index;
	short unkown4;
	short actor_id2;
	short unkown5;
	int rela_x;
	short unkown6;
	int rela_y;
	short unkown7;
	short direction;
	short unkown8;
	short isRevive;

	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData, int index);
	int ActorRevive();
};
#endif