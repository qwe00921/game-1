#ifndef _ScriptMenuVisible_H_
#define _ScriptMenuVisible_H_


class ScriptMenuVisible : public Script
{
public:
	 ScriptMenuVisible() {
		cmd = CMD_MENU_VISIBLE;
	}

	 short unkown1; // 2E

	 short visible;
	 int HandleRScript(RedeScreenPtr scr);
	 int initPara(char*SceneData,int index);
};
#endif