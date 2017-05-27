#ifndef _ScriptActorTalk_H_
#define _ScriptActorTalk_H_

class ScriptActorTalk : public Script
{
public:
	ScriptActorTalk()
	{
		cmd = CMD_ACTOR_TALK;
		this->actor_index1 = -1;
	}

	ScriptActorTalk(char* info, int actor_index)
	{
		cmd = CMD_ACTOR_TALK;
		this->info = info;
		this->actor_index1 = actor_index;
	}

	short unkown1; // 5对话内容

	DString info;
	short actor_index1;

	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif