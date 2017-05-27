#ifndef _BattleActor_H_
#define _BattleActor_H_
enum{
	ACTOR_STATE_NORMAL = 0,	//角色正常状态
		ACTOR_STATE_CELEBRATE = 1,
		ACTOR_STATE_DEF = 2,
		ACTOR_STATE_HITTED = 3, //用于剧本
		ACTOR_STATE_WEAK = 4,
		ACTOR_STATE_ATTACK_PREPARE = 5,
		ACTOR_STATE_ATTACK = 6,
		ACTOR_STATE_ATTACK_TWICE = 7,
		ACTOR_STATE_ROUND_SLOW = 8,
		ACTOR_STATE_BREATH = 9,
		ACTOR_STATE_FAINT = 10,
		ACTOR_STATE_ROUND_QUICK = 11,
		ACTOR_STATE_ROUND_NORMAL = 12,
		ACTOR_STATE_MOVE = 13,
		ACTOR_STATE_LEAVE_NORMAL = 14,
		ACTOR_STATE_LEAVE_HIDE = 15,
		ACTOR_STATE_LEAVE_DEAD = 16,
		ACTOR_STATE_ATTACKED = 17,
		ACTOR_STATE_DEADLY_ATTACK_PREPARE = 18, 
		ACTOR_STATE_NORMAL_ATTACK = 19,
		ACTOR_STATE_GOOD_GIVE = 20,
		ACTOR_STATE_GOOD_ACCEPT = 21,
		ACTOR_STATE_SKILL_PREPARE = 22,
		ACTOR_STATE_ATTACK_PREPARE_SOUND = 23,
		ACTOR_ACTION_DEF = 24,
		ACTOR_STATE_CELEBRATE_NO_SOUND = 25,
		ACTOR_STATE_CHANGE_FASHION = 26,
		ACTOR_STATE_RUN_ACTION = 27,
		ACTOR_STATE_MAX
};

//0
//一般
//1
//面向南将武器举过头顶
//2
//防御
//3
//被攻击
//4
//虚弱
//5
//准备攻击
//6
//攻击
//7
//二次攻击
//8
//慢速转圈，有音效
//9
//喘气
//10
//晕倒
//11
//快速转圈（无音效）
//12
//中速转圈（无音效）

#include "BattleAI.h"

class BattleActor
{
public:
	BattleActor(HistoryActorPtr actor);
	~BattleActor();
	void paintLand(DGraphics* g);
	void paintSpecialEffect(DGraphics* g);
	void paintFashion(DGraphics* g, int battle_state, SkillAttrPtr curSkill);
	void paint(DGraphics* g, int battle_state, SkillAttrPtr curSkill);
	void GenerateDefaultMove(int des_x,int des_y);
	static void setMoveList(DList<MovePath>* moveList);
	static void clearMoveList();
	static void setMoveIndex(int i);
	static void SetGoodImage(int good_id);
	int setState(int state,int iSetFrame = 0);
	void paintUnnormalState(DGraphics* g, int battle_state, SkillAttrPtr curSkill);
private:
	void DrawGainText();
public:
	static int normal_frame;
	static int move_end_direction;
	static int move_x;
	static int move_y;
	static int move_step;
	static int move_frame;
	static int move_index;
	static int PIXEL_PER_FRAME;
	static DList<MovePath>* moveList;
	static int is_default_move;
	static TRImagePtr goodImg;
public:
	HistoryActorPtr hisActor;
	int hurt_hp;
	int hurt_mp;
	int hurt_money;
	int hp_gain;
	int mp_gain;
	int cur_probability;//本次受到攻击的命中率
	bool m_bIsDeally; //本次是否被爆击
	bool m_bSkillMiss; //技能MISS显示未命中
	Sprite* m_spGain;
private:
	int frame;
	int m_need_frame;
	bool unnormal_state;
	TRImagePtr imgBlue;
	TRImagePtr imgGreen;
	TRImagePtr imgOrange;
	TRImagePtr imgRed;
	TRImagePtr imgYellow;
	TRImagePtr imgAttack;

	Label* m_labPercent;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	BattleActor(const BattleActor&);
	BattleActor& operator=(const BattleActor&);
};
#endif