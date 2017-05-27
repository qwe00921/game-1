#ifndef _Script118_76_SkipUncondition_H_
#define _Script118_76_SkipUncondition_H_
class ScriptSkipUncondition : public Script
{
public:
	ScriptSkipUncondition() {
		cmd = CMD_SKIP_UNCONDITION;
	}

	short unkown1;
	int script_id;

	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
	int initPara(char*SceneData, int index);
};
#endif