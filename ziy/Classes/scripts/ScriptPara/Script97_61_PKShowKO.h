#ifndef _ScriptPKShowKO_H_
#define _ScriptPKShowKO_H_

class ScriptPKShowKO  : public Script
{
public:
	ScriptPKShowKO() {
		cmd = CMD_PK_SHOW_K_O;
	}
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif