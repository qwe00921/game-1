#ifndef _Script115_73_ActorProperty_H_
#define _Script115_73_ActorProperty_H_
class ScriptActorProperty : public Script
{
public:
	ScriptActorProperty() {
		cmd = CMD_ACTOR_PROPERTY;
	}

	short unkown1;
	short actor_id1;
	short unkown2;
	int actor_id2;
	short unkown3;
	short actor_id3;
	short unkown4;
	short actor_id4;
	short unkown5;
	short actor_id5;
	short unkown6;
	short actor_id6;
	short unkown7;
	short actor_id7;
	short unkown8;
	short actor_id8;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif