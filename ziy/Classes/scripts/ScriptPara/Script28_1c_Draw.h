#ifndef _ScriptDraw_H_
#define _ScriptDraw_H_



class ScriptDraw : public Script
{
public:
	 ScriptDraw() {
		cmd = CMD_DRAW;
	}
	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
};
#endif