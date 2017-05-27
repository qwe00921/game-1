#ifndef _ScriptMapFaceMove_H_
#define _ScriptMapFaceMove_H_

class ScriptMapFaceMove  : public Script
{
public:
	ScriptMapFaceMove()
	{
		cmd = CMD_MAP_FACE_MOVE;
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