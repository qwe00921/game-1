#ifndef _ScriptChapterName_H_
#define _ScriptChapterName_H_

class ScriptChapterName  : public Script
{
public:
	ScriptChapterName()
	{
		cmd = CMD_CHAPTER_NAME;
	}
	short unkown1; // 5对话内容
	int chapt_index;
	short unkown2;
	DString info;
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData,int index);
};
#endif 