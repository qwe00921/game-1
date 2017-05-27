#ifndef _ScriptBattleActorTurn_H_
#define _ScriptBattleActorTurn_H_

class ScriptBattleActorTurn  : public Script
{
public:
	 ScriptBattleActorTurn() {
		cmd = CMD_BATTLE_ACTOR_TURN;
	}
	
	 ScriptBattleActorTurn(int actor_id1,int actor_id2,int delay1,int delay2) {
		cmd = CMD_BATTLE_ACTOR_TURN;
		this->actor_id1 = (short) actor_id1;
		this->actor_id2 = (short) actor_id2;
		this->direction = 0;
		this->is_opposite = 0;
		this->delay1 = (short) delay1;
		this->delay2 = (short) delay2;
	}

	
	 short unkown1;

	 short actor_id1;

	 short unkown2;

	 short actor_id2;

	 short unkown3;

	 short direction;

	 short unkown4;

	 short is_opposite;

	 short unkown5;

	 short delay1;

	 short unkown6;

	 short delay2;
	 int HandleScript();
	 int initPara(char*SceneData,int index);
};
#endif