#ifndef _Script112_70_AbilityChooseCopy_H_
#define _Script112_70_AbilityChooseCopy_H_
class ScriptAbilityChooseCopy : public Script
{
public:
	ScriptAbilityChooseCopy() {
		cmd = CMD_ABILITY_CHOOSE_COPY;
	}

	short unkown1;
	short actor_id1;
	short unkown2;
	short actor_id2;
	short unkown3;
	int copy_what;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif