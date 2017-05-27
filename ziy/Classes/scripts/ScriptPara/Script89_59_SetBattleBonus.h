#ifndef _ScriptSetBattleBonus_H_
#define _ScriptSetBattleBonus_H_
class ScriptSetBattleBonus : public Script
{
public:
	 ScriptSetBattleBonus() {
		cmd = CMD_SET_BATTLE_BONUS;
	}
	short unkown1;
	int money;
	short unkown2;
	short good_id1;
	short unkown3;
	short good_level1;
	short unkown4;
	short good_id2;
	short unkown5;
	short good_level2;
	short unkown6;
	short good_id3;
	short unkown7;
	short good_level3;
	short unkown8;
	short is_end;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif