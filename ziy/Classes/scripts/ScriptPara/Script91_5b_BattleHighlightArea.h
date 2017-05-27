#ifndef _ScriptBattleHighlightArea_H_
#define _ScriptBattleHighlightArea_H_
class ScriptBattleHighlightArea : public Script
{
public:
	ScriptBattleHighlightArea() {
		cmd = CMD_BATTLE_HIGHLIGHT_AREA;
	}
	
	short unkown1;
	int x1;
	short unkown2;
	int y1;
	short unkown3;
	int x2;
	short unkown4;
	int y2;
	short unkown5;
	short is_in_battle;
	int HandleScript();
	int initPara(char*SceneData,int index);
};
#endif