#ifndef _Script116_74_SwitchSavefile_H_
#define _Script116_74_SwitchSavefile_H_
class ScriptSwitchSavefile : public Script
{
public:
	ScriptSwitchSavefile() {
		cmd = CMD_SWITCH_SAVEFILE;
	}

	short unkown1;
	short actor_id1;

	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif