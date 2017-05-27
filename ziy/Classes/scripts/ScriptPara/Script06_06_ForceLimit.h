#ifndef _ScriptForceLimit_H_
#define _ScriptForceLimit_H_

class ScriptForceLimit : public Script
{
public:
	ScriptForceLimit()
	{
		cmd = CMD_FORCE_LIMIT;
	}
	short unkown1;
	short limit;
	short unkown2;
	int max_num;
	short unkown3;
	short actor_out_id1;
	short unkown4;
	short actor_out_id2;
	short unkown5;
	short actor_out_id3;
	short unkown6;
	short actor_out_id4;
	short unkown7;
	short actor_out_id5;
	short unkown8;
	short actor_in_id1;
	short unkown9;
	short actor_in_id2;
	short unkown10;
	short actor_in_id3;
	short unkown11;
	short actor_in_id4;
	short unkown12;
	short actor_in_id5;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif 