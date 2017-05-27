#ifndef _ScriptCallMagic_H_
#define _ScriptCallMagic_H_


class ScriptCallMagic : public Script
{
public:
	ScriptCallMagic()
	{
		cmd = CMD_CALL_MAGIC;
		delay = 1;
	}
	ScriptCallMagic(int magic_id, int x, int y)
	{
		cmd = CMD_CALL_MAGIC;
		this->magic_id = (short)magic_id;
		this->x = x;
		this->y = y;
		this->visible = 1;
		delay = 0;
	}
	short unkown1;

	int x;

	short unkown2;

	int y;

	short unkown3;

	short magic_id;

	short unkown4;

	short visible;

	int delay;
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif
