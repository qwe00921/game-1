#ifndef _ScriptHeadStateSet_H_
#define _ScriptHeadStateSet_H_
class ScriptHeadStateSet : public Script
{
public:
	 ScriptHeadStateSet() {
		cmd = CMD_HEAD_STATE_SET;
	}
	 short unkown1;
	 short id;
	int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif