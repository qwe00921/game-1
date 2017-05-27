#ifndef _ScriptClearActor_H_
#define _ScriptClearActor_H_



class ScriptClearActor : public Script
{
public:
	 ScriptClearActor() {
		cmd = CMD_CLEAR_ACTOR;
	}
	
	int HandleRScript(RedeScreenPtr scr);
};
#endif