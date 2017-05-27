#ifndef _ScriptInitValue_H_
#define _ScriptInitValue_H_


class ScriptInitValue : public Script
{
public:
	ScriptInitValue() {
		cmd = CMD_INIT_VALUE;
	}
	int HandleScript();
};
#endif