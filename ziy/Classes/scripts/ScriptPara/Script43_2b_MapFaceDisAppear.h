#ifndef _ScriptMapFaceDisAppear_H_
#define _ScriptMapFaceDisAppear_H_

class ScriptMapFaceDisAppear  : public Script
{
public:
	ScriptMapFaceDisAppear()
	{
		cmd = CMD_MAP_FACE_DISAPPEAR;
	}
	short unkown1; // 5对话内容
	short actor_id;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif 