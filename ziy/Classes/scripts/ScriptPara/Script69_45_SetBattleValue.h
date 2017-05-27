#ifndef _ScriptSetBattleValue_H_
#define _ScriptSetBattleValue_H_

class ScriptSetBattleValue  : public Script
{
public:
	 ScriptSetBattleValue() {
		cmd = CMD_BATTLE_SET_VALUE;
	}

	 short unkown1;

	 short nouse1;

	 short unkown2;

	 short nouse2;

	 short unkown3;

	 int maxRound;

	 short unkown4;

	 short level_add;

	 short unkown5;

	 short nouse3;

	 short unkown6;

	 short enemy_skipper_index;

	 short unkown7;

	 short nouse4;

	 short unkown8;

	 short own_skipper_index;

	 short unkown9;

	 short air_type;

	 short unkown10;

	 short start_air;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif