#ifndef _ScriptConfigOwnForce_H_
#define _ScriptConfigOwnForce_H_


class ScriptConfigOwnForce : public Script
{
public:
	ScriptConfigOwnForce()
	{
		cmd = CMD_CONFIG_OWN_FORCE;
	}

	short unkown1; // 04

	int num;

	short unkown2;// 38

	short out_actor_index1;

	short unkown3;// 38

	short out_actor_index2;

	short unkown4;// 38

	short out_actor_index3;

	short unkown5;// 38

	short out_actor_index4;

	short unkown6;// 38

	short out_actor_index5;

	short unkown7;// 38

	short in_actor_index1;

	short unkown8;// 38

	short in_actor_index2;

	short unkown9;// 38

	short in_actor_index3;

	short unkown10;// 38

	short in_actor_index4;

	short unkown11;// 38

	short in_actor_index5;

	short out_actor_index1_replace;
	short out_actor_index2_replace;
	short out_actor_index3_replace;
	short out_actor_index4_replace;
	short out_actor_index5_replace;
	short in_actor_index1_replace;
	short in_actor_index2_replace;
	short in_actor_index3_replace;
	short in_actor_index4_replace;
	short in_actor_index5_replace;


	int HandleScript();
	int initPara(char*SceneData, int index);

	static int scene_flag;
};
#endif