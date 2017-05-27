#ifndef _ScriptSetCurAir_H_
#define _ScriptSetCurAir_H_



class ScriptSetCurAir : public Script
{
public:
	ScriptSetCurAir()
	{
		cmd = CMD_SET_CUR_AIR;
	}
	short unkown1;
	short cur_air;
	int HandleScript();
	int initPara(char*SceneData, int index);
};
#endif