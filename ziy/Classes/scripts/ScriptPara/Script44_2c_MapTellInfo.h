#ifndef _ScriptMapTellInfo_H_
#define _ScriptMapTellInfo_H_

class ScriptMapTellInfo  : public Script
{
public:
	 ScriptMapTellInfo() {
		cmd = CMD_MAP_TELL_INFO;
	}
	short unkown1; // 5对话内容
	DString info;
	short unkown2;
	short page; //0不换页
	short unkown3;
	short line; //0 换行
	short unkown4;
	short delay;//0 停顿
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif