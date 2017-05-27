#ifndef _ScriptGoodSet_H_
#define _ScriptGoodSet_H_

class ScriptGoodSet  : public Script
{
public:
	 ScriptGoodSet() {
		cmd = CMD_GOOD_SET;
	}

	 short unkown1; // 02武将ID

	 short actor_id;

	 short unkown2; // 59武器ID

	 short weapon_id;

	 short unkown3; // 73武器等级

	 short weapon_level;

	 short unkown4; // 60护具ID

	 short coat_id;

	 short unkown5; // 73护具等级

	 short coat_level;

	 short unkown6; // 61辅助装备ID

	 short assist_good_id;

	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif