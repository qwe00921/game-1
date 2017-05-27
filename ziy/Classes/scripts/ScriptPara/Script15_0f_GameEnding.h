#ifndef _ScriptGameEnding_H_
#define _ScriptGameEnding_H_

class ScriptGameEnding : public Script
{
public:
	short unkown1; 
	short ending_id;

	ScriptGameEnding() {
		cmd = CMD_GAME_ENDING;
	}
	int HandleRScript(RedeScreenPtr scr);
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif


