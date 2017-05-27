#ifndef _ScriptSoundSet_H_
#define _ScriptSoundSet_H_

class ScriptSoundSet  : public Script
{
public:
	 ScriptSoundSet() {
		cmd = CMD_SOUND_SET;
	}
	 ScriptSoundSet(int id) {
		cmd = CMD_SOUND_SET;
		sound_id = id;
	}
	 short unkown1; // 1e音效类型

	 short sound_id;

	 short unkown2; // 04重复次数

	 int repeat;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif