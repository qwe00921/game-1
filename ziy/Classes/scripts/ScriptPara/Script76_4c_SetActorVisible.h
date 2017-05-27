#ifndef _ScriptSetActorVisible_H_
#define _ScriptSetActorVisible_H_

class ScriptSetActorVisible  : public Script
{
public:
	 ScriptSetActorVisible() {
		cmd = CMD_SET_ACTOR_VISIBLE;
	}

	 short unkown1;

	 short which_index;

	 short unkown2;

	 short actor_id;

	 short unkown3;

	 int out_index;

	 short actor_id_replace;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif
