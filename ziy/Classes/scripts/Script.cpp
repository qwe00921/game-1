#include "Common.h"
#include "Scripts.h"
bool Script::test_res[10000] = {true};
int Script::test_index = 0;
bool Script::in_child_first_time = false;
Script::Script()
{
	cmd = 0;
	subScript = 0;
	this->fineshed = 0;
	chess = Chess::getInstance();
	frame = 0;
	need_frame = 0;
}
Script::Script(int cmd)
{
	this->cmd = (short)cmd;
	this->fineshed = 0;
	chess = Chess::getInstance();
	frame = 0;
	need_frame = 0;
	subScript = 0;
}
bool Script::get_test_result()
{
	if (test_res[test_index])
	{

		return true;
	}
	else
	{
		if (test_index>0)
			test_index--;
		return false;
	}
}
int Script::HandleScript()
{
	if (cmd == CMD_CHILD_ACTION)
	{
		test_index++;
	}
	if (cmd == CMD_ACTION_END)
	{
		if (test_index>0)
			test_index--;
	}
	return 0;
}
int Script::HandleRScript(RedeScreenPtr scr)
{
	return HandleScript();
}
int Script::initPara(char* SceneData, int index) {
	return index;
}
void Script::setSubScript(int subScript)
{
	this->subScript = subScript;
}