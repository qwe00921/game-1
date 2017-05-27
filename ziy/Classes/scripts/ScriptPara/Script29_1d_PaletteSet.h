#ifndef _ScriptPaletteSet_H_
#define _ScriptPaletteSet_H_

class ScriptPaletteSet  : public Script
{
public:
	ScriptPaletteSet() {
		cmd = CMD_1d;
	}
	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
};
#endif