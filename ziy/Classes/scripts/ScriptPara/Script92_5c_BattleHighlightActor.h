#ifndef _ScriptBattleHighlightActor_H_
#define _ScriptBattleHighlightActor_H_
class ScriptBattleHighlightActor : public Script
{
public:
	 ScriptBattleHighlightActor() {
		cmd = CMD_BATTLE_HIGHLIGHT_ACTOR;
	}
	 short unkown1;
	 short actor_id;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif