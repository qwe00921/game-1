#ifndef _Script113_71_AskEffects_H_
#define _Script113_71_AskEffects_H_
class ScriptAskEffects : public Script
{
public:
	ScriptAskEffects() {
		cmd = CMD_ASK_EFFECTS;
	}

	short unkown1;
	int actor_id1;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif