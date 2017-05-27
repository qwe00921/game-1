#ifndef _ScriptSectionEnd_H_
#define _ScriptSectionEnd_H_

class ScriptSectionEnd : public Script
{
public:
	ScriptSectionEnd() {
		cmd = CMD_c;
	}
	int HandleScript();
	int HandleRScript(RedeScreenPtr scr);
};
#endif