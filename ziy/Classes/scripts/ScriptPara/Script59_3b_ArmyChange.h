#ifndef _ScriptArmyChange_H_
#define _ScriptArmyChange_H_

class ScriptArmyChange : public Script
{
public:
	 ScriptArmyChange() {
		cmd = CMD_ARMY_CHANGE;
	}

	 short unkown1; // 02武将ID

	 short actor_id;

	 short unkown2; // 0E

	 short come_or_leave;

	 short unkown3; // 3E

	 short level_add;
	 short actor_id_replace;
	 int HandleRScript(RedeScreenPtr scr);
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};



#endif