#ifndef _ScriptBattleActorRelive_H_
#define _ScriptBattleActorRelive_H_

class ScriptBattleActorRelive  : public Script
{
public:
	ScriptBattleActorRelive() {
		cmd = CMD_BATTLE_ACTOR_RELIVE;
        actor_level = 0;
		iIsTower = 0;
	}

	ScriptBattleActorRelive(int actorId,int px,int py) {
		cmd = CMD_BATTLE_ACTOR_RELIVE;

		which_index = -1;
		actor_id = actorId;
		
		x =px;
		
		y = py;
	
		direction = 0;//方向
        actor_level = 0;
		iIsTower = 0;
	}
	ScriptBattleActorRelive(HistoryActorPtr ptrCurActor,int actorId, int px, int py,int iActorLv,int iType = 1) {
		cmd = CMD_BATTLE_ACTOR_RELIVE;

		which_index = -1;
		actor_id = actorId;

		x = px;

		y = py;

		direction = ptrCurActor->direction;//方向
		actor_level = iActorLv;

		tCurActor = ptrCurActor;

		iIsTower = iType;
	}
    ScriptBattleActorRelive(int actorId,int level, int px,int py) {
        cmd = CMD_BATTLE_ACTOR_RELIVE;

        which_index = -1;
        actor_id = actorId;

        x =px;

        y = py;

        direction = 0;//方向
        actor_level = level;
		iIsTower = 0;
    }
    int actor_level; //指定复活的武将等级

	short unkown1; // 40 //决定武将的方式
	
	short which_index;
	
	short unkown2; // 2武将序号
	
	short actor_id;
	
	short unkown3; // 4出场顺序
	
	int out_index;
	
	short unkown4; // 4横坐标
	
	int x;
	
	short unkown5; // 4纵坐标
	
	int y;
	
	short unkown6;
	
	short direction;
	
	short delay2;

	HistoryActorPtr tCurActor;
	int iIsTower;
	int HandleScript();
	void AddTowerActor();
	void DecomposeTowerActor();
	int initPara(char*SceneData,int index);
};
#endif