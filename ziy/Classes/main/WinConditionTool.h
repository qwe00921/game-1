#ifndef __TRGAME_WINCONDITIONTOOL_
#define __TRGAME_WINCONDITIONTOOL_

#include <stdio.h>
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class WinConditionTool{
public:
	~WinConditionTool();
	static WinConditionTool* getInstance();
	void InitBattleCondition();
	void StartBattleSetCondition();
	void BattleSetCondition(ActionGainPtr ptrGain,HistoryActorPtr curActor);
	void BattleRoundEndSetCon();
	void ActorRetreatCondition(int iActorId);
	void BattleEndSetCondition();
	map<int, int> ReachConditionResult(bool bBattleWin = false);

	void AddBattleConsumes(int iGoodsId,int iNum = 1);
	map<int, int> GetConsumesList();
	void AddScriptCodeElem(int iScode);
	map<int, int> GetScriptCodeList();
	void AddScriptActorElem(int iActorId,int iOwnListIndex);
	map<int, int> GetScriptActorList();
	void BattleFailedSetDate();
	map<int,int> GetSeckillEnemy();
private:
	WinConditionTool();
	void SetWinConditionEnable();
	void ClearAllMap();
private:
	static WinConditionTool* m_instance;
	//死亡的敌军
	int death_enemy_enable;
	map<int, int> death_enemy;
	map<int, map<int, int> > not_included_death_enemy;
	//撤退的敌军
	int retreat_enemy_enable;
	map<int, int> retreat_enemy;
	//死亡的我军
	int death_own_enable;
	map<int, int> death_own;
	//撤退的我军
	int retreat_own_enable;
	map<int, int> retreat_own;
	//必使用兵种
	int use_occupation_enable;
	map<int, int> use_occupation;
	bool reach_use_occupation;
	//不能使用的兵种
	int cnuse_occupation_enable;
	map<int, int> cnuse_occupation;
	bool reach_cnuse_occupation;
	//不能使用的武将
	int cnuse_actor_enable;
	map<int, int> cnuse_actor;
	bool reach_cnuse_actor;
	//回合限制
	int round_limit_enable;
	bool reach_round_limit;
	//武将指定回合消灭敌军数
	int actor_kill_enemy_enable;
	map<int, map<int, int> > actor_round_info;
	map<int, map<int,int> > actor_kill_enemy;
	//死亡的友军
	int death_friend_enable;
	map<int, int> death_friend;
	//友军撤离人数
	int retreat_friend_enable;
	map<int, int> retreat_friend;
	//出战武将数量限制
	int actor_num_limit_enable;
	bool reach_actor_num_limit;
	//不使用任何道具
	int cnuse_goods_enable;
	bool reach_use_goods;
	//占领据点
	int occupation_stronghold_enable;
	map<int, map<int,int> > occupation_stronghold;
	//消灭指定武将
	int destroy_designated_generals_enable;
	map<int, map<int, int> > destroy_designated_generals;
	//指定回合消灭N个敌军
	int round_kill_Enemy_enable;
	map<int, map<int,int> > round_kill_Enemy;
	//指定回合消灭指定敌军
	int round_kill_appoint_Enemy_enable;
	map<int, map<int, int> > round_kill_appoint_Enemy;
	//秒杀的敌军
	map<int,int> own_seckill_enemy;

	//整形变量
	map<int,map<int,int> > shaping_variable_data;
	map<int, map<int, int> > shaping_variable_data_battled;//战斗结束触发

	//战场使用消耗品
	map<int, int> consumesList;
	//战场全局变量
	map<int, int> scriptCode;
	//战场加入武将
	map<int, int> scriptAddActor;
};
#endif /* __TRGAME_WINCONDITIONTOOL_ */
