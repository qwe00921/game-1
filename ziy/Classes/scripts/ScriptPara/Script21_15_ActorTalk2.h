#ifndef _ScriptActorTalk2_H_
#define _ScriptActorTalk2_H_

class ScriptActorTalk2 : public Script
{
public:
	ScriptActorTalk2()
	{
		cmd = CMD_ACTOR_TALK2;
	}
	ScriptActorTalk2(int actor_index, char* info)
	{
		cmd = CMD_ACTOR_TALK2;
		this->actor_index1 = actor_index;
		this->info = info;
	}
	short unkown1; // 5对话内容

	short actor_index1;

	short unkown2;

	short actor_index2;

	short unkown3;

	DString info;

	short actor_id_replace;
	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif