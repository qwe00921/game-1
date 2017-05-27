#ifndef _ScriptElse_H_
#define _ScriptElse_H_


class ScriptElse  : public Script
{
public:
	ScriptElse() {
		cmd = CMD_ELSE;
	}
	int HandleScript();
};
#endif