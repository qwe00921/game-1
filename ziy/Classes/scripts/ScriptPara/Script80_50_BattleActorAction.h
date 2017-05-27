#ifndef _ScriptBattleActorAction_H_
#define _ScriptBattleActorAction_H_

class ScriptBattleActorAction : public Script
{
public:
	ScriptBattleActorAction()
	{
		cmd = CMD_BATTLE_ACTOR_ACTION;
		this->hurt_hp = 0;
		this->hurt_mp = 0;
		this->hurt_money = 0;
		m_bIsDeally = false;
		m_iShareId = -1;
	}
	ScriptBattleActorAction(int actor_id, int action, int delay1, int delay2)
	{
		cmd = CMD_BATTLE_ACTOR_ACTION;
		this->actor_id = (short)actor_id;
		this->action = (short)action;
		this->delay1 = (short)delay1;
		this->delay2 = (short)delay2;
		this->hurt_hp = 0;
		this->hurt_mp = 0;
		this->hurt_money = 0;
		m_bIsDeally = false;
		m_iShareId = -1;
	}
	ScriptBattleActorAction(int actor_id, int action, int delay1, int delay2, int hurt_hp, int hurt_mp, bool bIsDeally, int iShareId)
	{
		cmd = CMD_BATTLE_ACTOR_ACTION;
		this->actor_id = (short)actor_id;
		this->action = (short)action;
		this->delay1 = (short)delay1;
		this->delay2 = (short)delay2;
		this->hurt_hp = -hurt_hp;
		this->hurt_mp = -hurt_mp;
		this->hurt_money = 0;
		m_bIsDeally = bIsDeally;
		m_iShareId = iShareId;
	}
	//显示金钱变化
	ScriptBattleActorAction(int actor_id, int action, int delay1, int delay2, int hurt_money)
	{
		cmd = CMD_BATTLE_ACTOR_ACTION;
		this->actor_id = (short)actor_id;
		this->action = (short)action;
		this->delay1 = (short)delay1;
		this->delay2 = (short)delay2;
		this->hurt_hp = 0;
		this->hurt_mp = 0;
		this->hurt_money = hurt_money;
		m_bIsDeally = false;
		m_iShareId = -1;
	}

	short unkown1;

	short actor_id;

	short unkown2;

	short action;

	short unkown3;

	short delay1;

	short unkown4;

	short delay2;

	short hurt_hp;
	short hurt_mp;
	short hurt_money;//金钱变化
	bool m_bIsDeally; //是否是爆击
	int m_iShareId; //分担伤害的武将ID
	int HandleScript();
	int initPara(char*SceneData, int index);
};

// 0
// 一般
// 1
// 面向南将武器举过头顶
// 2
// 防御
// 3
// 被攻击
// 4
// 虚弱
// 5
// 准备攻击
// 6
// 攻击
// 7
// 二次攻击
// 8
// 慢速转圈，有音效
// 9
// 喘气
// 10
// 晕倒
// 11
// 快速转圈（无音效）
// 12
// 中速转圈（无音效）
#endif