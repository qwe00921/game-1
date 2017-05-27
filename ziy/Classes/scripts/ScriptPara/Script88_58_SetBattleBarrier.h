#ifndef _ScriptSetBattleBarrier_H_
#define _ScriptSetBattleBarrier_H_
class ScriptSetBattleBarrier : public Script
{
public:
	ScriptSetBattleBarrier() {
		cmd = CMD_SET_BATTLE_BARRIER;
	}
	short unkown1;
	short barrier_id;
	short unkown2;
	short set;
	short unkown3;
	short land_id;
	short unkown4;
	int x;
	short unkown5;
	int y;
	short unkown6;
	short delay1;
	short unkown7;
	short delay2;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif