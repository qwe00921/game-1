#ifndef _Script108_6c_ActorAbilityCopy_H_
#define _Script108_6c_ActorAbilityCopy_H_
class ScriptActorAbilityCopy : public Script
{
public:
	ScriptActorAbilityCopy() {
		cmd = CMD_ACTOR_ABILITY_COPY;
	}

	short unkown1;
	short actor_id1;
	short unkown2;
	short actor_id2;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif