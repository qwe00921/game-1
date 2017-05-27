#ifndef _ScriptDelaySet_H_
#define _ScriptDelaySet_H_


class ScriptDelaySet  : public Script
{
public:
	 ScriptDelaySet() {
		cmd = CMD_DELAY_SET;
	}

	 ScriptDelaySet(int delay) {
		cmd = CMD_DELAY_SET;
		this->delay = delay;
	}
	 short unkown1; // 4 延时时间
	 int delay;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif