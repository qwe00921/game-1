#ifndef _ScriptMusicSet_H_
#define _ScriptMusicSet_H_


class ScriptMusicSet  : public Script
{
public:
	 ScriptMusicSet() {
		cmd = CMD_MUSIC_SET;
	}

	 short unkown1; // 9音轨序号

	 short music_id;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif