#ifndef _ScriptTestAsk_H_
#define _ScriptTestAsk_H_



class ScriptTestAsk : public Script
{
public:
	ScriptTestAsk() {
		 cmd = CMD_REQUEST_TEST;
	}
	
	short unkown1; // 

	short value_true; // 1;
     int HandleRScript(RedeScreenPtr scr);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif