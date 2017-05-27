#ifndef _ScriptSceneChange_H_
#define _ScriptSceneChange_H_
class ScriptSceneChange : public Script
{
public:
	 ScriptSceneChange() {
		cmd = CMD_SCENE_CHANGE;
	}
	 short unkown1;
	 short scene_id;
	 short scene_replace;
	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif