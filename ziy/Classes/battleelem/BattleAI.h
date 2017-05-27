#ifndef _BattleAI_H_
#define _BattleAI_H_

enum
{
	MAP_NO_REACH = 0,
	MAP_CAN_REACH = 1,
	MAP_CAN_MOVE = 2
};

enum
{
	ATTACK_TYPE_NORMAL = 0x0,//第一次攻击
	ATTACK_TYPE_TWICE = 0x2,//第二次攻击
	ATTACK_TYPE_THIRD = 0x4,//第三次攻击
	ATTACK_TYPE_BACK = 0x8,//反击
	ATTACK_TYPE_BACK_BACK = 0x10,//反击后反击
	ATTACK_TYPE_PURSUIT_ATTACK = 0x11,//追击攻击
	ATTACK_TYPE_ATTACK_SWEEP = 0x12,//横扫
	ATTACK_TYPE_DEADLY = 0x20,//致命一击
	ATTACK_TYPE_INDUCT = 0x40, //引导攻击
};

enum
{
	SKILL_TYPE_NORMAL = 0x0, //第一次法术攻击
	SKILL_TYPE_TWICE = 0x2,  //法术连击
	SKILL_TYPE_DEALLY = 0x4, //法术暴击
	SKILL_TYPE_FOLLOW = 0x8, //法术追击
	SKILL_TYPE_BACK = 0x10, //法术反击
	SKILL_TYPE_BRAVELY = 0x30//法术奋战
};

///////////////////////////////////////////////////////////////////////////////////////////

class AStarNode;
typedef AStarNode* AStarNodePtr;

class MovePath
{
public:
	MovePath(const AStarNode* node);
	~MovePath();
public:
	DPoint point;
	DList<DPoint>* path;
	int m_iUsedSpeed;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	MovePath(const MovePath&);
	MovePath& operator=(const MovePath&);
};
typedef MovePath* MovePathPtr;

///////////////////////////////////////////////////////////////////////////////////////////

class AStarNode
{
public:
	AStarNode()
	{
		x = 0;
		y = 0;
		NextNode = NULL;
		Parent = NULL;
		step = 0;
	}
	~AStarNode()
	{
		x = 0;
		y = 0;
		NextNode = NULL;
		Parent = NULL;
		step = 0;
	}
public:
	int x;
	int y;
    AStarNodePtr NextNode;
    AStarNodePtr Parent;
	int step;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	AStarNode(const AStarNode&);
	AStarNode& operator=(const AStarNode&);
};

///////////////////////////////////////////////////////////////////////////////////////////

/**单个收益*/
class ActionSingleGain
{
public:
	ActionSingleGain();
	~ActionSingleGain();
	void readSingleGain(DMemoryReader* reader);
	void saveSingleGain(DMemoryWriter* writer);
public:
	HistoryActor* oppo_actor;
	HistoryActor* share_actor; //分担伤害的武将
	int gain_hp; //获得的HP,对敌人造成伤害时该值为负数，
	int gain_mp;//获得的MP,对敌人造成伤害时该值为负数，
	SSCHAR state_mustHit; //百分百命中
	SSCHAR state_power;
	SSCHAR state_intellegence;
	SSCHAR state_defence;
	SSCHAR state_explosion;//敏捷，爆发力
	SSCHAR state_fortune;//士气
	SSCHAR state_puzzle;//混乱
	SSCHAR state_blind;//失明 被攻击必中 不可攻击
	SSCHAR state_obfuscation;//昏迷，无法行动，被攻击必中，并且解除状态
	SSCHAR state_poison;//中毒
	SSCHAR state_palsy;//麻痹
	SSCHAR state_stun;//禁咒
	SSCHAR state_speed;
	SSCHAR state_weaked; //被虚弱，受伤害增加
	SSCHAR state_skillhurt; //法伤加成
	SSCHAR state_adddef_redpow;//降攻加防
	SSCHAR state_loseblood;//失血
	SSCHAR state_addhp;//恢复
	SSCHAR state_sual_link;//灵魂链接
	SSCHAR state_reduceSkill; //洞察
	SSCHAR state_reduceHurt; //惨痛压制
	SSCHAR state_emergency_command; //冲锋号令

	SSCHAR probability;//命中率
	SSCHAR action_again;
	SSCHAR land_strict;
	SSCHAR enemy_skipper_strict;//主将限制
	SSCHAR cCurAir;
	SSCHAR actor_relive; //召唤人物助阵
	SSCHAR fast_move;
	SSCHAR actor_tower_def;	//召唤塔防
	SSCHAR state_SkillHurtAndRebound;//增加法术和伤害反弹
	DPoint cur_point;
	int actor_relive_id;
	int actor_relive_px;
	int actor_relive_py;
	int controlled_enemy_actorId;//控制的敌军id
	int border_on_death;
	int actor_leveraging_power;//标记是否是借力打力对象
	int actor_knockback;//武将被击退
	int reduce_skill_hurt;//减伤光环
	int add_skill_hurt;//增伤光环
	int state_shield_range; //庇护光环
	int through_the_target;//穿越目标
	int new_assault;//使自己和被攻击者前进一格
	int actor_can_drag;//拖到身前
	int skill_dispatch_actor;//遣将

	int iIsHitback; //是否刀轮的击退
	int iHitbackId; //击退人物ID
	DPoint curMovePos; //当前武将终点
	DPoint oppoMovePos; //被攻击武将终点
	map<int, DPoint> m_mEffectArea;
	map<int, int> mSeidAddGain;//TYPE724, seid附加的gain , 0攻击者 1被攻击者
};
typedef ActionSingleGain* ActionSingleGainPtr;

/**总体收益*/
class ActionGain
{
public:
	ActionGain();
	~ActionGain();
	void readGain(DMemoryReader* reader);
	void saveGain(DMemoryWriter* writer);
public:
	int moveIndex;//MoveList中的下标
	DPoint point;//人物位置
	DPoint hitPoint;//攻击位置
	int actor_kill;//单次收益的杀敌数
	int cur_hp_hurt;//对当前攻击目标的伤害量
	int total_hp_hurt;//单次攻击多个目标时的总伤害量
	int gain_mp;//获取mp
	int gain_hp;//获取hp
	int need_hp;//消耗hp
	int need_mp;//消耗mp
	int need_back;//反击
	int deadly_attack;//致命一击
	int next_round_gain_hp;//下回合回复的血量
	int iRealKillNum; //实际击杀的人数
	int iDontShowSkillName;//不显示技能名
	SkillAttr* skill;//使用技能
	GoodsItem* good;//使用物品
	DList<ActionSingleGain>* gains;//一次攻击多个目标时的收益
};
typedef ActionGain* ActionGainPtr;

///////////////////////////////////////////////////////////////////////////////////////////
class BattleAI 
{
public:
	BattleAI();
	~BattleAI();
	void readBattleAI(DMemoryReader* reader);
	void saveBattleAI(DMemoryWriter* writer);
public:
	void GenerateGain_AttackType(HistoryActorPtr actor,int x,int y,int attack_type);
	void ClearGain();	
	/**获取移动路径*/
	void GenerateMoveList(HistoryActorPtr curActor);	
	void GenerateDispatchMoveList(HistoryActorPtr curActor);
	ActionGainPtr GetSkillGain(SkillAttrPtr skill,int cur_x, int cur_y, int x, int y);
	ActionGainPtr GetAttackGain(int cur_x, int cur_y, int x, int y, HistoryActorPtr cur_Actor = NULL,bool isVisible = true); //cur_Actor不为空时为获取反击攻击的收益
	/**获取最优攻击（须先获取移动路径）*/
	void GenerateMostGain_old();
	void GenerateMostGain();
	void GenerateRealGain();
	void GenerateRealGainOld();
	void GenerateAttackGainList(int index);
	void GenerateGoodGainList(GoodsItem* good, int index);
	void GenerateSkillGainList(SkillAttrPtr skill,int index);
	void ClearGainList();
	void GenerateAttackType();
	void GenerateGain_SkillType(SkillAttrPtr curSkill, HistoryActorPtr actor, int x, int y, int skill_type);
	void GenerateSkillType(SkillAttrPtr skill);
	void GenerateSkillType(ActionGainPtr ptrGain);
private:
	int GetMoveAttr(int x, int y, short occupation_id);
	void UpdateMostGain(ActionGainPtr mostGain, ActionGainPtr curGain, int index);
	ActionGainPtr GeneratePointMostGain(int index);
	bool IsDeadlyAttack();
	void GenerateMostNearPos();
	int GenerateMostNearPos(int x, int y);
	ActionGainPtr GeneratePointMostGainOnlyAddHp(int index); //只加血
	void BounceAbnormalAttack(ActionGainPtr mostGain, HistoryActorPtr actor, ActionSingleGainPtr curSingleGain);
	bool GetTowerEnable(int iSkillId,int iX,int iY,int iCurX,int iCurY);
public:
	ActionGain* mostGain;
	DList<ActionGain>* gainList;    //手动状态下专用

	HistoryActor* curActor;
	int need_back_attack_twice;//反击可以触发双击
	int need_twice;
	int need_xianshou;
	int need_back_back;
	int need_induct;
	int induct_num;//当前引导次数
	int bravely_fight_num;//当前奋战次数
	int need_intensify_back_hit;//强化反击百分比
	int need_bravely_fight;//奋战攻击
	int need_pursuit_attack;//追击攻击	
	int need_assault_attack;//突击攻击
	int skill_need_twice;  //法术连击
	int skill_need_follow; //法术追击
	int skill_need_back; //法术反击
	int need_attact_all_enemy;//攻击范围内所有人攻击一次
	int need_attact_all_enemy_num;//攻击范围内所有人攻击一次攻击到第几人
	int skill_double_hit;//法术技能连击
    int need_siege;
	int is_xianshou;//标记先手攻击
	int bear_hand_attack;//使自己后退一格，被攻击者同方向移动一格
	int two_order_spike;//二阶突刺
	int new_assault;//使自己和被攻击者都前进一格
	int need_follow_probably; // 几率追击的概率
	int need_follow_hit_times; // 几率追击N次攻击
	int through_the_target;//穿越攻击
	int need_attack_sweep;//横扫
	int actor_can_drag;//拖敌人到身前
	int skill_dispatch_actor;//遣将
	int need_vampire_attack;//攻击伤害
	int attack_type;
	int skill_type; //技能特效
	int skill_need_deally; //法术暴击
	int iActiveSkillId; //主动使用技能
	int need_pursuit_attack_actor[10];//所有触发追击攻击的人物id，[0]为受到物理伤害的武将id
	int skill_need_follow_actor[10];//所有触发法术追击的人物id，[0]为受到物理伤害的武将id
	map<int, int> bravely_fight_oppo_actor;//奋战攻击的目标id集合
	map<int, int> skill_bravely_fight;//法术奋战 0:使能 1:已奋战人数 2:不能奋战的武将id
	map<int, int> need_spontaneous_detonation;//自爆
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	BattleAI(const BattleAI&);
	BattleAI& operator=(const BattleAI&);
};

#endif