#ifndef _ScriptMapFaceAppear_H_
#define _ScriptMapFaceAppear_H_

class ScriptMapFaceAppear  : public Script
{
public:
	ScriptMapFaceAppear()
	{
		cmd = CMD_MAP_FACE_APPEAR;
	}
	short unkown1; // 5对话内容
	short actor_id;
	short unkown2;
	int x;
	short unkown4;
	int y;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif 