#ifndef _ScriptShowChoice_H_
#define _ScriptShowChoice_H_


class ScriptShowChoice  : public Script
{
public:
	 ScriptShowChoice() {
		cmd = CMD_SHOW_CHOICE;
	}

	 short unkown1; // 5对话内容

	 DString info;

	 short unkown2; // 2武将序号

	 short actor_id;
	 int HandleRScript(RedeScreenPtr scr);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif