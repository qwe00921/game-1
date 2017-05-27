#ifndef _HistoryActor_H_
#define _HistoryActor_H_

#include "BattleAI.h"
#include "BattleAI.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "editor-support/cocostudio/CCArmature.h"
using namespace cocostudio;
USING_NS_CC;

enum EncryptKey
{
	BASE_POWER,
	BASE_DEFENCE,
	BASE_INTELLEGENCE,
	BASE_EXPLOSION,
	BASE_FORTUNE,
	BASE_SPEED,
	BASE_HP,
	BASE_MP,
	CUR_POWER,
	CUR_DEFENCE,
	CUR_INTELLEGENCE,
	CUR_EXPLOSION,
	CUR_FORTUNE,
	CUR_HP,
	CUR_MP,
	ACTOR_LEVEL,
	GOODS_ADD_POWER,
	GOODS_ADD_DEFENCE,
	GOODS_ADD_INTELLEGENCE,
	GOODS_ADD_EXPLOSION,
	GOODS_ADD_FORTUNE,
	GOODS_ADD_HP,
	GOODS_ADD_MP,
	GOODS_ADD_SPEED,
	ACTOR_OLDLEVEL
};

enum TotalAbility
{
	TOTAL_POWER = 0,
	TOTAL_INTELLEGENCE = 1,
	TOTAL_DEFENCE = 2,
	TOTAL_EXPLOSION = 3,
	TOTAL_FORTUNE = 4,
	TOTAL_SPEED = 5,
	TOTAL_HP = 6,
	TOTAL_MP = 7
};

enum 
{
	TYPE001 = 1, //每回合恢复HP(百分数)
	TYPE007 = 7, //辅助攻击力(整数)
	TYPE008 = 8, //辅助精神力(整数)
	TYPE009 = 9, //辅助防御力(整数)
	TYPE010 = 10, //辅助爆发力(整数)
	TYPE011 = 11, //辅助运气(整数)
	TYPE012 = 12, //辅助HP增加(整数)
	TYPE015 = 15, //辅助移动力(整数)
	TYPE022 = 22, //辅助攻击命中(百分数)
	TYPE023 = 23, //反击后反击
	TYPE024 = 24, //致命一击攻击
	TYPE025 = 25, //远距离攻击(没羽箭)
	TYPE026 = 26, //穿透效果(整数)
	TYPE028 = 28, //骑马攻击
	TYPE036 = 36, //辅助攻击防御(百分数)
	TYPE107 = 107, //先手攻击
	TYPE108 = 108, //引导攻击(整数)
	TYPE109 = 109, //奋战攻击
	TYPE110 = 110, //强化反击（百分比）
	TYPE111 = 111, //强化暴击（百分比）
	TYPE112 = 112, //吸血攻击(百分比)
	TYPE114 = 114, //追击攻击（伤害系数）
	TYPE135 = 135, //法术MP消耗(百分数)
	TYPE150 = 150, //致盲攻击（百分数）
	TYPE151 = 151, //混乱攻击(百分数)
	TYPE152 = 152, //中毒攻击(百分数)
	TYPE153 = 153, //麻痹攻击(百分数)
	TYPE154 = 154, //禁咒攻击(百分数)
	TYPE155 = 155, //昏迷攻击（百分比）
	TYPE183 = 183, //反弹伤害(百分比)
	TYPE201 = 201, //辅助攻击力(百分比)
	TYPE202 = 202, //辅助精神力(百分比)
	TYPE203 = 203, //辅助防御力(百分比)
	TYPE206 = 206, //辅助HP增加(百分比)
	TYPE240 = 240, //每回合恢复HP(百分数)
	TYPE301 = 301, //克制兵种输出
	TYPE302 = 302, //克制兵种防御
	TYPE303 = 303, //辅助法术输出
	TYPE309 = 309, //当身边无队友时，攻击力+%
	TYPE310 = 310, //损血增益(百分比)
	TYPE320 = 320, //辅助地形效果
	TYPE351 = 351, //范围属性增强
	TYPE359 = 359, //精神防御（百分比）
	TYPE361 = 361, //每回合恢复固定的MP
	TYPE372 = 372, //双倍伤害
	TYPE381 = 381, //冲锋攻击
	TYPE383 = 383, //仇恨攻击
	TYPE397 = 397, //不移动时增加普攻命中率%
	TYPE421 = 421, //最大伤害为生命值的%
	TYPE424 = 424, //物理伤害%
	TYPE428 = 428, //几率N攻击
	TYPE600 = 600, //提升对X防具伤害
	TYPE601 = 601, //提升技能伤害
	TYPE602 = 602, //指定地形移动能力提升
	TYPE603 = 603, //装备剑后提升攻击力15%
	TYPE604 = 604, //装备剑加攻击15%，扇加精神15%
	TYPE605 = 605, //分担近身队友50%受到的致死攻击，每回合1次。
	TYPE606 = 606, //几率抵挡什么技能
	TYPE607 = 607, //几率造成失血
	TYPE700 = 700, //回复、伤害+%
	TYPE701 = 701, //当处在目标的侧面或者背面时，伤害增加X%
	TYPE702 = 702, //攻击时可以对目标造成防御值x/100的额外伤害
	TYPE703 = 703, //值固定为1，目标在受到攻击时，绝对不会发生位移
	TYPE704 = 704, //在目标的任何方向对目标发动攻击时，都视为“在目标背后”，固定值为1
	TYPE705 = 705, //在受到近战攻击时会被击退，被击退后可以减少X%所受到的伤害
	TYPE706 = 706, //在受到致命一击攻击时减少X%的伤害
	TYPE707 = 707, //X为每次被攻击消耗mp的百分比，Y为mp转化为hp的转化率
	TYPE708 = 708, //过量治疗时，根据过量治疗的比例然后*X\100计算返还法力值
	TYPE709 = 709, //抵挡技能伤害
	TYPE710 = 710, //附加自身等级X百分比伤害
	TYPE711 = 711, //增加双击几率
	TYPE712 = 712, //增加爆击几率
	TYPE713 = 713, //保持地形能力
	TYPE714 = 714, //未穿透增伤
	TYPE715 = 715, //攻击，施法时消耗敌军mp
	TYPE716 = 716, //对移动力越低的敌军造成的伤害越高
	TYPE717 = 717, //技能所造成的有效治疗或伤害X%会治疗自己
	TYPE718 = 718, //免疫特定状态
	TYPE719 = 719, //减少恶劣天气和加成有利天气对法术的影响
	TYPE720 = 720, //身经百战，范围X内所有友军属性提升Y
	TYPE721 = 721, //多重打击，每次普攻对目标相邻血量最低敌军造成附加伤害
	TYPE722 = 722, //厚重，无法被击退，也不会陷入昏迷的状态。
	TYPE723 = 723, //减少法伤，提高治疗量
	TYPE724 = 724, //噬灵
	TYPE725 = 725, //横扫
	TYPE726 = 726, //杀神
	TYPE727 = 727, //魔战士之心
	TYPE728 = 728, //减少反击伤害
	TYPE729 = 729, //钢铁，免疫反弹伤害
	TYPE730 = 730, //魅影，复制最后一次被攻击的物理技能
	TYPE731 = 731, //庇护，代替范围内友军成为伤害目标
	TYPE732 = 732, //灵质，普攻恢复自身X点MP
	TYPE733 = 733, //武将专属武器限定
	TYPE734 = 734, //职业专属武器限定
	TYPE735 = 735, //虎咬，无视距离反击
	TYPE736 = 736, //反技，受到技能攻击可反击
	TYPE737 = 737, //迅猛，增加自身移动力*X%的伤害
	TYPE738 = 738, //随步，剩余移动力可再次移动
	TYPE739 = 739, //凤凰涅槃，五围中成长较低的X项增加Y点
	TYPE740 = 740, //激战，增加相邻部队数Y%的X属性
	TYPE741 = 741, //回马，转身攻击时，增加X伤害，未转身攻击时，增加Y伤害
	TYPE742 = 742, //弱点攻击，视敌人五围最低一项为防御计算伤害
	TYPE743 = 743, //BOSS特效，免疫特定BUFF, 免疫TYPE412、725
	TYPE744 = 744, //抗击，减少物理普攻正面X、侧面Y、背面Z的伤害
	TYPE745 = 745, //阵亡使用技能
	TYPE747 = 747, //提升单体技能X效果Y%
	TYPE750 = 750, //攻击时无视防御
	TYPE751 = 751, //增加双击伤害
	TYPE752 = 752, //伤害限制
	TYPE753 = 753, //增加闪避，闪避后反击加伤害
	TYPE754 = 754,//特技习得技能
	TYPE755 = 755,//攻击加攻击力
	TYPE756 = 756,//出场效果
	TYPE757 = 757,//单独减伤
	TYPE759 = 759,//精神力转化为防御力
	TYPE760 = 760,// 攻击增加防御
	TYPE761 = 761,//吴子兵法
	TYPE762 = 762,// 天空盔
	TYPE763 = 763,//血书
	TYPE764 = 764,//天竺葵
	TYPE766 = 766,//混沌
	TYPE767 = 767,//星火燎原
};

enum
{
	LIFE_RIGHTEOUSNESS = 50,//生身取义
	CHANGE_OCCO_ATTRIBUTE = 51,//改变兵种属性
	MOVE_ADDTWICE = 53,//移动加双击率
	SPEED_ATTACK = 54,//疾风攻击N&	对移动力越低的敌人造成越高的伤害，物理伤害加成N*（14-目标移动力）%
	FIGHT_ATTACK = 55,//激斗攻击N&	九宫范围内每多一个人，增加固定伤害,固定伤害为（N+武器Lv）*九宫范围内人数
	LGNORE_ABILITY = 56,//无视能力。攻击时无视敌人N%防御力，防御时无视敌人N%攻击力
	REDUCE_LAND = 57,//攻击时，将敌方地形能力视为N%
	ATTACK_AFTER_MOVE = 58,//若行动前未移动，行动后可移动
	CHANGE_WEATHER = 59,//回合开始强制改变天气
	BLOOD_SHIELD = 60,//鲜血护盾
	LIGHT_REFLE = 61,//闪电反射
	VITAL_SPIRIT = 62,//免受伤害
	BERA_HAND = 63,//使自己和被攻击者都后退一格
	TWOORDER_SPIKE = 64,//二阶突刺
	NEW_ASSAULT = 65,//使自己和被攻击者都前进一格
	ARBALESTER = 66,//劲弩
	ADD_ATTACK_ELEM = 67,//攻击时部分精神力转化为攻击力
	TRANSFER_HURT = 68,//转移伤害
	ADD_ATKORDEF = 69,//本方回合面板攻击力上升X%，敌方行动回合防御力增加X%
	DECREASES_DAMA = 70,//减轻不与自身相邻的敌军的攻击伤害X%，减轻近身伤害x/2%
	HP_CHANGE_MP = 71,//当MP＝０时，消耗等同于MP上限的HP，并且令MP回复１
	TWICEHITORDEADLY_RATE = 73,//增加双击和暴击率
	HIT_INJURY = 74,//击中敌人后，触发自身减伤X%，受到伤害后，触发自身增伤X%
	HITBACK_DOUBLE = 76, // TYPE 425，反击几率双击，参数X，双击几率X%
	ROUND_SPEED_UP = 77, // TYPE 426，每回合几率加移动，参数X，增加几率X%
	ROUND_DEF_UP = 78, // TYPE 427，每回合几率加防御，参数X，增加几率X%

	SHARE_DAMAGE = 105, //TYPE 605, 警戒，分担伤害

	ENMU_MAX
};

//BUFF相关
enum
{
	BUFF_STATE_POW = 1,//攻击buff
	BUFF_STATE_INT,//精神buff
	BUFF_STATE_DEF,//防御buff
	BUFF_STATE_EXPLOSION,//爆发buff
	BUFF_STATE_FORTUNE,//士气buff
	BUFF_STATE_SPEED,//移动buff
	BUFF_STATE_WEAKED,//虚弱buff
	BUFF_STATE_OBFU,//昏迷buff
	BUFF_STATE_BLIND,//失明buff
	BUFF_STATE_PUZZLE,//混乱buff
	BUFF_STATE_POISON,//中毒buff
	BUFF_STATE_PALSY,//麻痹buff
	BUFF_STATE_STUN,//禁咒buff
	BUFF_STATE_SKILLHURT, //法术伤害提升
	BUFF_SKILLHURTANDREBOUND,//法术增伤和增加物理反弹
	RANGE_BUFF_REDUCE_SKILLHURT = 961,//法术减伤光环
	RANGE_BUFF_ADD_SKILLHURT = 962, //法术增伤光环
	RANGE_BUFF_ADD_POWINT = 963, //范围增加攻击精神
	RANGE_CONTROL_TOWER = 964,//范围内赋予机甲可移动
	RANGE_ADD_TOWER_ABILITY = 965, //范围内提升机甲某属性
	RANGE_ADD_LAND_ABILITY = 966, //范围内保持地形效果
	RANGE_BE_ATTACK_ADD_MP = 967//范围内友军被攻击增加MP
};

enum 
{
	ACTOR_BUFF_PUZZLE = 1, //混乱
	ACTOR_BUFF_OBFU = 2, //眩晕
	ACTOR_BUFF_STUN = 3, //封技
	ACTOR_BUFF_WEAK = 4, //虚弱
	ACTOR_BUFF_BLIND = 5, //失明
	ACTOR_BUFF_PALSY = 6, //麻痹
	ACTOR_BUFF_BLOOD = 20, //失血
	ACTOR_BUFF_POISON = 21, //中毒
	ACTOR_BUFF_ADDHP = 30, //回复
	ACTOR_BUFF_POWUP = 100, //攻击↑
	ACTOR_BUFF_POWDW = 101, //攻击↓
	ACTOR_BUFF_INTUP = 102, //精神↑
	ACTOR_BUFF_INTDW = 103, //精神↓
	ACTOR_BUFF_DEFUP = 104, //防御↑
	ACTOR_BUFF_DEFDW = 105, //防御↓
	ACTOR_BUFF_EXPUP = 106, //爆发↑
	ACTOR_BUFF_EXPDW = 107, //爆发↓
	ACTOR_BUFF_FORTUP = 108, //士气↑
	ACTOR_BUFF_FORTDW = 109, //士气↓
	ACTOR_BUFF_SPEEDUP = 110, //移动↑
	ACTOR_BUFF_SPEEDDW = 111, //移动↓
	ACTOR_BUFF_HURTUP = 500, //法伤↑
	ACTOR_BUFF_HURTDW = 501, //法伤↓
	ACTOR_BUFF_FIRESHIELD = 502, //烈火盾
	ACTOR_BUFF_ADDDEFREDPOW = 503,//降攻加防
	ACTOR_BUFF_SUALLINK = 504,//灵魂链接
	ACTOR_BUFF_REDUCESKILL = 505,//洞察
	ACTOR_BUFF_REDUCEHURT = 506,//惨痛压制
	ACTOR_BUFF_SHIELD_RANGE = 507, //庇护光环
	ACTOR_BUFF_EMERGENCYCOMMAND = 508,//冲锋号令
};

enum
{
	DIRECT_UP = 0, //朝向上
	DIRECT_RT = 1, //朝向右
	DIRECT_DW = 2, //朝向下
	DIRECT_LT = 3 //朝向左
};

class ActorBuff
{
public:
	ActorBuff()
	{
		iBuffId = 0;
		iCurRound = 0;
		iMaxRound = 0;
		iPercent = 0;
		iPercentNum = 0;
		iArmIndex = 0;
		iCasterActorId = 0;
	}
public:
	int iBuffId;
	int iCurRound;
	int iMaxRound;
	float iPercent;
	float iPercentNum;
	int iArmIndex;
	int iCasterActorId;//施法对象
};

class CopySkill
{
public:
	CopySkill()
	{
		iSkillId = 0;
		iLevel = 0;
		iEncryptKey = 0;
		iUsed = 0;
	}
public:
	int iSkillId;
	int iLevel;
	int iEncryptKey;
	int iUsed;
};

class HistoryActor
{
public:
	HistoryActor();
public:
	unsigned short actor_id;
	short occupation_id;
	signed short direction;
	signed short ai_type;
	signed short hide;
	signed short oppo_actor_id;
	unsigned short dest_x;
	unsigned short dest_y;
	unsigned short x;
	unsigned short y;
	int exp;
	unsigned short actor_quality; //武将品质
	int actor_old_quality;//武将品质
	unsigned short actor_grade;	  //武将品阶(绿、蓝、紫、橙、金)

	HistoryGoods weapon;
	HistoryGoods coat;
	HistoryGoods assist_good;
	HistoryGoods fashion;
	HistoryGoods wap_fashion;
	UUCHAR action_finished;
	UUCHAR relation;	
	/////////////////////////////////////////////////////////////////////
	UUCHAR good_recover_hp_round; //每回合恢复Hp百分数
	UUCHAR good_recover_mp_round; //每回合恢复Mp百分数
	UUCHAR good_recover_state;	//每回合恢复状态	
	UUCHAR good_shock_move; //突击移动
	UUCHAR good_rough_move; //恶路移动
    UUCHAR good_passtrough_move; //穿越移动，可以穿越我军、友军、敌军的任何人员
	UUCHAR good_puzzle_hit;  //混乱攻击机率
	UUCHAR good_poison_hit;  //中毒攻击机率
	UUCHAR good_palsy_hit; //麻痹攻击机率
	UUCHAR good_stun_hit; //封杀策略机率
	UUCHAR good_hit_probability; //攻击命中(百分数)
	UUCHAR good_hit_back_back; //反击后反击机率
	UUCHAR good_deadliness_hit; //致命一击机率
	UUCHAR good_hit_distance; //攻击距离类型
	UUCHAR good_hit_area;     //攻击范围类型
	UUCHAR good_hit_no_back; //无反击攻击
	UUCHAR good_induct_hit; //引导攻击机率
	UUCHAR good_assist_fire; //辅助火系攻击
	UUCHAR good_assist_wind; //辅助风系攻击
	UUCHAR good_retrench_MP; //节省MP
	UUCHAR good_call_myth; //召唤四神
	UUCHAR good_skill_probability; //辅助法术命中(百分数)
	UUCHAR good_hit_defence;		//辅助攻击防御率(百分数)(对方命中率下降)
	UUCHAR good_skill_defence;   //辅助法术防御率(百分数)(对方命中率下降)
	UUCHAR good_total_defence;   //辅助全防御
	UUCHAR good_hold_far_hit; //防御远距离攻击
	UUCHAR good_hold_deadliness_hit; //防御致命一击
	UUCHAR good_hold_twice_hit; //防御二次攻击
	UUCHAR good_reduce_skill_hurt; //降低策略伤害
	UUCHAR good_mp_hold_hurt; //mp辅助防御
	UUCHAR good_reduce_far_hit; //降低远距离攻击伤害
	UUCHAR good_auto_use_good; //自动使用道具不消耗库存
	
/////////////////////////////////////////////////////////////////////////////
	SSCHAR state_power;
	SSCHAR state_intellegence;
	SSCHAR state_defence;
	SSCHAR state_explosion;
	SSCHAR state_fortune;
	SSCHAR state_puzzle;    //混乱
	SSCHAR state_poison;    //中毒
	SSCHAR state_palsy;     //麻痹
	SSCHAR state_stun;      //禁咒
	SSCHAR state_speed;
	SSCHAR state_skillhurt; //法伤加成
	SSCHAR state_loseblood;//失血
	SSCHAR state_addhp;//回血
	SSCHAR state_reduceSkill;//洞察
	SSCHAR state_reduceHurt;//惨痛压制
	SSCHAR state_emergency_command;//冲锋号令
	////////////////////////////////////////////////////////////////////////////////
	UUCHAR style;

	int battleTimes;//出战次数
	int retreatTimes;//撤退次数
	int face_id;//对话时头像
	int good_hit_twice;//物品攻击两次
	int good_xianshou;//先手攻击
	int good_hurt_back;//伤害反弹
	int good_vampire_attack;//吸血攻击
    int good_suck_mp_attack;//吸MP攻击
	int good_hit_fixharm; //破甲攻击
	int good_hit_fixharm_begin; //破甲攻击初始伤害
	int good_randbuff_hit; //随机状态攻击几率
	int good_init_ability; //凤凰奥义
	int good_back_attack_twice;//反击的时候可以双击
	int good_add_power_round;						//每回合加攻击力
	int good_add_defence_round;					//每回合加防御力
	int good_add_intellegence_round;				//每回合加精神力
	int good_add_explosion_round;						//每回合加爆发力（命中率）
	int good_add_fortune_round; //每回合加士气
	int good_add_speed_round; //每回合加移动
	int good_twice_action; //二次移动
////////////////////////////////////////////////////////////////////////////////

    class RangeEffect{
        public:
        int area;
        int value;
        int probability;
        RangeEffect(int area, int value, int probability){
            this->area= area;
            this->value= value;
            this->probability= probability;
        }
    };
	DList<RangeEffect> * good_up_power_range;					//范围增加攻击力
	DList<RangeEffect> * good_down_power_range;					//范围减少攻击力
	DList<RangeEffect> * good_up_defence_range;					//范围增加防御力
	DList<RangeEffect> * good_down_defence_range;				//范围减少防御力
	DList<RangeEffect> * good_up_intellegence_range;				//范围增加精神力
	DList<RangeEffect> * good_down_intellegence_range;			//范围减少精神力
	DList<RangeEffect> * good_up_explosion_range;				//范围增加爆发力（命中率）
	DList<RangeEffect> * good_down_explosion_range;				//范围减少爆发力（命中率）
	DList<RangeEffect> * good_up_fortune_range; //范围增加士气
	DList<RangeEffect> * good_down_fortune_range; //范围减少士气
	DList<RangeEffect> * good_up_speed_range; //范围增加移动
    DList<RangeEffect> * good_down_speed_range; //范围减少移动
	DList<RangeEffect> * good_up_hp_range; //范围增加hp
	DList<RangeEffect> * good_up_mp_range; //范围增加mp

	DList<RangeEffect> * good_up_puzzle_range;  //范围增加混乱
	DList<RangeEffect> * good_up_poison_range;  //范围增加中毒
	DList<RangeEffect> * good_up_palsy_range; //范围增加麻痹
	DList<RangeEffect> * good_up_stun_range; //范围增加封杀
    DList<RangeEffect> * good_up_blind_range; //范围增加失明
    DList<RangeEffect> * good_up_obfuscation_range; //范围增加昏迷
	DList<RangeEffect> * good_down_puzzle_range;  //范围减少混乱
	DList<RangeEffect> * good_down_poison_range;  //范围减少中毒
	DList<RangeEffect> * good_down_palsy_range; //范围减少麻痹
	DList<RangeEffect> * good_down_stun_range; //范围减少封杀
    DList<RangeEffect> * good_down_blind_range; //范围减少失明
    DList<RangeEffect> * good_down_obfuscation_range; //范围减少昏迷
	DList<RangeEffect> * good_add_move_consume_range; //范围增加敌军地形移动消耗
	DList<RangeEffect> * good_down_action_range; //范围牵制行动
	DList<RangeEffect> * reduce_skill_hurt_buff; //降低法功光环
	DList<RangeEffect> * add_skill_hurt_buff; //增强法功光环
	DList<RangeEffect> * good_up_powint_range; //范围增强攻击精神力
	DList<RangeEffect> * good_shield_range; //庇护光环
	DList<RangeEffect> * good_can_control_tower; //范围提供召唤物移动力
	DList<RangeEffect> * good_add_tower_ability; //范围增强召唤物属性
	DList<RangeEffect> * good_add_land_ability; //范围增强召唤物属性
	DList<RangeEffect> * good_being_attack_add_mp; //范围内友军受到攻击增加mp
    
	int good_intensify_back_hit;//强化反击
	int good_intensify_deadlyattack;//强化暴击
	int good_mp_attack;//mp攻击

	int good_pursuit_attack_num;//追击攻击次数
	int good_skill_pursuit_attack_num;//追击攻击次数
	int pursuit_skill_lv;

    int good_invisible; //隐身
	int good_killed_hurt; //致死攻击
	int good_attack_must; //攻击必中
	int good_weak_attack; //死亡标记
	int good_battle_relive; //战场复活
	map<int, int> mElitePlus;//精英加成
//////////////////////////////////////////////////////////////////////////
	int good_magic_no_land;//法术无视地形	法术无视地形效果施放	
	int good_magic_no_air;//法术无视天气 法术无视天气效果施放
	int good_magic_twice;//法术连击（伤害系数）	1%~900%	伤害系数	主动使用法术时造成两次效果
	int good_magic_deadly;//法术暴击（伤害系数）	1%~900%	伤害系数	主动使用法术时造成暴击效果
	int good_magic_follow;//法术追击	(1~999)&(1%~900%)	“法术Id”&“法术的伤害系数”	攻击范围内的敌军受到物理伤害时，主动追加一次法术攻击	
	int good_magic_back;//法术反击	(1~999)&(1%~900%)	“法术Id”&“法术的伤害系数”	受到攻击后，自动追加一次特定法术的还击
	int good_alone_up_power;//单独增益(百分比)	(1~999)&(1%~900%)	“属性id”&“加成数值系数”	身边无队友时某属性增加	
	int good_loseblood_up_power;//损血增益(百分比)	(1~999)&(1%~900%)	“属性id”&“加成数值系数”	根据损失的血量比例增加的五维数值	
	int good_assist_land_power;//辅助地形效果	(1~999)&(1%~900%)	“地形id”&“加成的数值比例”	改变某种地形的适应能力
	int good_assist_land_speed;//辅助地形移动损耗	(1~999)&(1~999)	“地形id”&“移动力损耗”	改变某种地形的移动能力
	short good_effects[200];//物品特技数组[0]：同人印/[1]圣者攻击/[2]破坏攻击/[3]破坏精神/[4]破坏防御/[5]破坏暴发/[6]破坏士气/[7]破坏移动//[8]辅助法术命中
	//[9]辅助法术格挡/[10]掠夺攻击/[11]防御异常属性攻击/[12]防御异常状态攻击/[13]突击攻击/[14]昏迷攻击/[15]克制兵种输出/[16]克制兵种防御/[17]辅助法术输出
	//[18]辅助法术防御//[19]学会指定法术、[20]mp辅助防御（百分比）/[21]随机属性攻击/[22]法术追击/[23]辅助地形移动损耗/[24]神魔附体/[25]辅助地形效果/[26]单独增益、[27]损血增益/[28]--------
	//[29]致盲攻击//[30]每回合恢复固定mp//[31]范围属性影响的范围值//[32]法术必中//[33]踏浪人物水上漂//[34] 骑马攻击使用dic_jobclass兵种进行克制，并允许一次指定多个。
	//[35]砍人一刀秒//[36]无限移动，自动回归//[37]自动吃武力果 //[38]法术必防//[39]反弹异常攻击//[40]冲锋攻击//[41]无极攻击//[42]仇恨攻击//[43]唯我独尊
	//[44]攻击时降低对方武器或者装备等级//[45]使命中者降低自身装备经验//[46]攻击范围内所有的人攻击一次//[47]自动使用道具消耗库存（只要被击中）//[48]每回合增加武器和防具经验
	//[49]濒临死亡//[50]生身取义
	SSCHAR state_blind;//失明 被攻击必中 不可攻击
	short history_occupation_id;//初始兵种 
	short history_image_fight_id;	
	SSCHAR state_obfuscation;//昏迷，无法行动，被攻击必中，并且解除状态
	int image_id; //人物R形象
	int image_change_occu_id; //转职后的形象 初始化为-1 在其有值时 获取人物战场形象 始终返回它
	char force_save; //1表示强制保存此人物数据，在改变形象，人物内存退出的时候使用
	short limit_actor_move;//人物移动是否受到限制,如受到限制保存关卡数
	short limit_actor_move_x1;//人物移动受到限制时的范围坐标
	short limit_actor_move_y1;//人物移动受到限制时的范围坐标
	short limit_actor_move_x2;//人物移动受到限制时的范围坐标
	short limit_actor_move_y2;//人物移动受到限制时的范围坐标
	short isAssistArmy; //标识敌军是否是援军部队， 0--非援军  1--是援军；
	DShortArray set_hero_seid;//改变人物seid时，新的seid数组
	DArray<UUCHAR> used_skill_ids;//每关使用过的法术 下标对应法术id
	int good_auto_use_good_loss; //自动使用道具消耗库存

	int is_leave; //是否是离开状态，执行53指令后为1
	SSCHAR state_weaked; //武将的死亡标记状态
	int weaked_num; //被虚弱后受到伤害的加成系数
	int is_used_relive; //是否使用过战场复活
	int bust_style;//半身像表情
	DShortArray AcquisitionSeid;//额外习得特技
	int iTwiceAction; //标识二次行动是否使用过
	int iActionNum; //无极攻击剩余重置次数
	int buff_hate_attack;//仇恨攻击buff叠加次数
	int controlled_by_enemy;//标记是否被敌军控制
	int controlled_by_enemy_round;
	int controlled_by_enemy_addAbi;
	int border_on_death;//濒死状态
	int free_allocation;//自由分配点数
	DShortArray ConsumeSeid;//消耗品习得特技
	int enable_set_occupation_id;//是否可以修改兵种
	int actor_only_move;//仅能移动
	int blood_shield;//鲜血护盾
	int light_refle;//闪电反射
	int vital_spirit;//免受伤害
	int hp_mp_defense_number;//hp转化为mp
	int m_iShieldNum; //庇护次数

	bool siege_state;
	int stradd_power;//强化增加的武力
	int stradd_defence;
	int stradd_intellegence;
	int stradd_explosion;
	int stradd_fortune;
	short paint_special_effect[2]; //画出特效
	DShortArray actor_fit_id; //合体技
	map<int, int> mFashion;//时装[0]:开关[1]男女[2]衣服[3]武器前置编号[4]武器动作
	map<int, int> mInitFashion;//初始时装[0]:开关[1]男女[2]衣服[3]武器前置编号[4]武器动作
	int animation_index;
	int m_iShareTimes; //警戒使用次数
	int m_iExternalEquip;
	int m_iRestSpeed; //行动结束剩下的移动力
	map<int, int> mSkillUseNumLimt;//每场战斗技能使用次数限制
	map<int, int> mSeidAddGain;//seid附加的收益数值
	map<int, int> mSkillBeUseNum;//被施法的技能次数
	
	SSCHAR state_reduce_skill_hurt_range;//降低法伤光环
	SSCHAR state_SkillHurtAndRebound;//增加法功和反弹伤害
	SSCHAR state_add_skill_hurt_range;//增加法伤光环
	SSCHAR state_adddef_redpow;//降攻加防
	SSCHAR state_sual_link;//灵魂链接

	int m_iControllerId;//控制者id
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	void init(int i);
	float getGrownValue(int iAttrType, bool bIsTotal = true); //获取属性的成长值，是否包含修炼成长
	int GetGrownAbility(int iLevel, int iAbilityType);
	void InitAbility();
	void InitFiveBaseAbility();//重新计算人物五维 改变兵种时使用
	void InitFiveAllAbility();//重新计算人物属性
	void InitHPAndMPAbility();//重新计算人物hpmp
	void AddElitePlus();//增加精英加成
	void addBaseAbility(map<int, int> mAbility, int iType = 1); //增加的基础属性1、增加百分比；2、增加数值
	void updateBaseAbility(); //刷新基础属性
	void InitoneBaseAbility(int iIndex);//重新计算一种能力0.攻击力1.精神力2.防御力3.爆发力4.士气
	void UpdateSkills(int iMaxLevel = 0);
	void SetDefaultGood(int good_type, bool isGoodSet = false);
	void SetNullGood();
	void UpdataGoodEffect(int iOpenTerrainSuit=1);
	void AddGoodAttr(HistoryGoods* ptrGoods, int iAddVal);
	bool HasEnemyAside(int x,int y);
	int GenerateDefaultMove(int x1,int y1,int x2,int y2, int super_move = 0);
	int MapisMoveable(int x,int y);
	void clearGoodEffect();
	bool isAttackArea(int iX, int iY, bool bIsBack = false); //判断是否在武将的攻击范围内 bIsBack 是否是反击
	bool isSkillBackEnable(int iX, int iY, SkillAttr* ptrSkill); //判断法术是否能触发反击
	int inAttackArea_PK(int x, int y);
	int getAttackAreaId();
	void recoverState();
	unsigned short xx()
	{
		return x*SCENE_TILE_WIDTH;
	};
	unsigned short yy()
	{
		return y*SCENE_TILE_WIDTH;
	};
    int inArea(int x1,int y1,int x2,int y2);
	int addRealAbility(int abilityCase, int ability);
	int getRealAbility(int abilityCase, bool includeState = false,int real_x =-1,int real_y = -1,bool enable_fits = true); //获取真实的属性值
	int getMinAbilityId(); //获取五围属性中最低项，返回属性ID
	int getBasicsMAXAbility();//获得五维中最强的基础属性（人物基础属性加装备增加的）
	void AddSpecialEffect(int seid, int baseLevel = 1, bool bIsStreng = false); //bIsStreng标识是否是强化数据
	int SeparateState(int iAreaId = -1);//判读是不是单独状态
	HistoryActorPtr RangeOfEnemy(unsigned short actor_id);//计算范围内会攻击的敌人，近战，在攻击范围，没有行动过
	int GetActorOccuType(); //获取武将的兵种在列表中Index，返回值为从0开始，方便做下标用
	int GetActorInitialJob(); //获取武将的初始职业，返回值相当于 history_occupation_id
	int getActorJobType(); //获取武将职业大类
    int getTotalRangeEffect(int type, bool up, int dst_x, int dst_y);
	void GetSuitEffect();//获得套装效果
	int RangeOfNumbers();//计算武将九宫范围内有几人
	void delete_controlled_by_enemy();//从我方武将列表去除被控制的武将
	int getEnemyRangeEffect(int range_type, bool up);//获得敌军的减益光环效果
	int setGoodAddLevel();
	HistoryActorPtr RangeOfHPMinEnemy(int actorId);
	int GetDoubleSeid(int fits, bool is_effect = true, int real_x = -1, int real_y = -1);
    class StateRangeEffects 
	{
	public:
		StateRangeEffects()
		{
			blind = 0;
			puzzle = 0;
			poison = 0;
			palsy = 0;
			stun = 0;
			obfuscation = 0;
		}
    public:
        int blind;
        int puzzle;
        int poison;
        int palsy;
        int stun;
        int obfuscation;
    };
    void accumulateTotalStateRangeEffects(StateRangeEffects *state, bool up, int dst_x, int dst_y);
	void InitAbilityForRevive(); // 复活后初始化武将属性
	char* getActorSImgPath(const char* suffix, int iRelation = -1);//获得武将S形象路径
	void setStrAddGoodsEffect();//设置强化效果
	void setAnimationIndex();
	char* getAnimationName();//获得动编名
	DString getAnimationPlayId(int iAnimId,int iType, bool& iFlip);//获得动编playid名
	int getOccuPowerType();//1:物理 2:法术
	float getFiveAbilityBuffEffect(int state, int iBuffType);
	int GetActorSpeed(int iType = 1);//1:全部移动力2:buff增加的移动力
	void needAssault(HistoryActorPtr temp_cur_actor, int &AssX, int &AssY);
	int needAssault(int iMoveX, int iMoveY, HistoryActorPtr ptrActor);
	int needAssault(int iMoveX, int iMoveY, int iCurX, int iCurY, HistoryActorPtr ptrActor);
	int CanDrag(int iMoveX, int iMoveY, HistoryActorPtr ptrActor,bool bisPoint = false);
	int GetRangeBuffEffect(int iBuffType, bool bIsFavour,bool bIsAdd = true);
	int GetRange965FiveAbilityEffect(int iAbilityCase, int iAbilityNum);
	int getRangeBuffActorId(int iBuffType, bool bIsFavour); //获取BUFF的武将ID
	std::map<int,int> getRangeBuffActorAndNum(int iBuffType, bool bIsFavour); //获取BUFF的武将ID和Num
	int GetActorJobImgId();
	int getMoveSpeedUsed(int iPosX, int iPosY, int iMoveFlag); //获取武将移动力损耗
	void updateActorBuff(int iBuffId, int iBuffState, bool bIsAttr, ActionGain* ptrGain = NULL, HistoryActor* pCurActor = NULL);
	void updateActorState(int iBuffId); //更新武将BUFF状态
	void updateBuffDisplay(Layer* ptrLayer);
	void updateBuffPosition(bool bVisible);
	void clearActorBuff();
	int getLandAbilityGain(int iLandId); //计算当前地形对能力加成系数
	bool isBackAttack(HistoryActor* ptrActor); //是否是背击
	int getAttackedType(HistoryActor* ptrActor); //判断被攻击的类型，正面、侧面、背面，返回减伤系数
	int getActionDirect(int iDestX, int iDestY); //获取行动后朝向

	int getTotalAbility(int iTypeId);

	int GetBasepower();
	int GetBaseDefence();
	int GetBaseIntellegence();
	int GetBaseExplosion();
	int GetBaseFortune();
	int GetBaseSpeed();
	int GetBaseHp();
	int GetBaseMp();

	void SetBasepower(int iSet = -1,int iAdd = 0);
	void SetBaseDefence(int iSet = -1, int iAdd = 0);
	void SetBaseIntellegence(int iSet = -1, int iAdd = 0);
	void SetBaseExplosion(int iSet = -1, int iAdd = 0);
	void SetBaseFortune(int iSet = -1, int iAdd = 0);
	void SetBaseSpeed(int iSet = -1, int iAdd = 0);
	void SetBaseHp(int iSet = -1, int iAdd = 0);
	void SetBaseMp(int iSet = -1, int iAdd = 0);

	int GetCurpower();
	int GetCurDefence();
	int GetCurIntellegence();
	int GetCurExplosion();
	int GetCurFortune();
	int GetCurHp();
	int GetCurMp();
	int GetActorLv();
	int GetActorOldLv();

	void SetCurpower(int iSet = -1, int iAdd = 0);
	void SetCurDefence(int iSet = -1, int iAdd = 0);
	void SetCurIntellegence(int iSet = -1, int iAdd = 0);
	void SetCurExplosion(int iSet = -1, int iAdd = 0);
	void SetCurFortune(int iSet = -1, int iAdd = 0);
	void SetCurHp(int iSet = -1, int iAdd = 0);
	void SetCurMp(int iSet = -1, int iAdd = 0);
	void SetActorLv(int iSet = -1, int iAdd = 0);
	void SetActorOldLv(int iSet = -1, int iAdd = 0);

	int GetGoodsAddPower();
	int GetGoodsAddDefence();
	int GetGoodsAddIntellegence();
	int GetGoodsAddExplosion();
	int GetGoodsAddFortune();
	int GetGoodsAddSpeed();
	int GetGoodsAddHp();
	int GetGoodsAddMp();

	void SetGoodsAddPower(int iSet = -1, int iAdd = 0);
	void SetGoodsAddDefence(int iSet = -1, int iAdd = 0);
	void SetGoodsAddIntellegence(int iSet = -1, int iAdd = 0);
	void SetGoodsAddExplosion(int iSet = -1, int iAdd = 0);
	void SetGoodsAddFortune(int iSet = -1, int iAdd = 0);
	void SetGoodsAddSpeed(int iSet = -1, int iAdd = 0);
	void SetGoodsAddHp(int iSet = -1, int iAdd = 0);
	void SetGoodsAddMp(int iSet = -1, int iAdd = 0);

	bool SelfSkillLevelIsNull(int iSkill);
	int GetSelfSkillLevel(int iSkill);
	void SetSelfSkillLevel(int iSkill, int iSet = -1, int iAdd = 0);
	void ClearSelfSkillLevel();
	map<int,int> GetAllSelfSkillLevel();

	bool CurSkillLevelIsNull(int iSkill);
	int GetCurSkillLevel(int iSkill);
	void SetCurSkillLevel(int iSkill, int iSet = -1, int iAdd = 0);
	void ClearCurSkillLevel();
	map<int, map<int, int> > GetCurSkillLvList();

	map<int, int> GetNetSuitData();//全部触发的套装效果

	void addSpecialEffect(int iSeidId);
	void clearSpecialEffect();
	bool isHadSpecial(int iTypeId);
	int getSpecialInt(int iTypeId);
	map<int, int> getSpecialMap(int iTypeId);

	void SetFashionData();//设置时装
	map<int, int> GetFashionSeid();//时装附带seid

	bool isSameCamp(HistoryActor* ptrActor); //判断两武将是否为同一阵营
	map<int, HistoryActor*> getAroundActor(); //获取周围九宫格内武将集合
	map<int, HistoryActor*> getAroundActor(int iAttackId, bool bSameCamp); //获取周边相关的武将集合，用于追击攻击或警戒
	HistoryActor* getActorByType(int iType); //获取指定特性对应的武将
	int getHitbackStep(int iCurX, int iCurY, int iHitX, int iHitY); //获取冲击击退的步数
	int getDistance(HistoryActor* ptrActor); //获取两个武将之间的距离，用于冲击击退伤害计算
	void SetSoulBox(int i);//是否魂匣
	int GetSoulBox();
	int GetSeidIsOpen(int iSeid);//获取特技是否开启
	void initCopySkill(); //初始化武将复制技能
	void clearCopySkill();
	void updateCopySkill(int iSkillId);
	void updateRoundState(); //每回合刷新武将状态
	float getPracGrown(int iAttrType); //修炼的成长值
	void setPracGrown(int iAttrType, float fValue, bool bIsAdd = false); //增加或设定，默认设定
	void CopyActorPracGrown(HistoryActor* ptrActor);
	void GetSeidAddSkill();
	void updateStoneSuit(); //更新灵石套装特性
	int getInductNum();//获得武将自带seid引导次数
	void setAddAbilityGoods(map<int,int> mGoodsData);
	void setAddAbilityGoods(int iGoodsId,int iCount);
	map<int, int> getDicHeroSeid(bool bOlnyShow = false);
	map<int, map<int, int> > getExclusiveGemEffect();
	void updateFashion(Sprite* spImage, Sprite* spWeapon, int iZOrder = 0, int iDirection = DIRECT_DW); //更新武将时装形象
private:
	void addExtraGrowEffect(int iType, int iNum);
	int GetExtraGrow(int iPower);
	void SetExtraGrow(int iPower, int iSet = -1, int iAdd = 0);

    int getTotalRangeEffectByList(DList<RangeEffect> * list, int dst_x, int dst_y);
    DList<RangeEffect> * getRangeEffectListByType(int type, bool up);
	void SetFashionAddAbility();
	void setSpecialAddAbility(); //计算特技对成长的加成
	void setTowerAddAbility();//计算召唤物加成
	int getFmStoneIndex(map<int, int> mStoneId); //获取指定组合可以激活的灵石套装ID，不存在则返回0
	int getFmStoneIndex(int iStoneId);
	void setAddAbilityGoodsEffect();
public:
	map<int, ActorBuff*> m_mActorBuff;
	Armature* m_pArmSkill; //技能动画
	map<int, CopySkill*> m_mCopySkill;
	map<int, int> m_mSeidAddSkill;//特技习得技能
	map<int, int> m_mStoneSeid; //灵石套装
	map<int, int> m_mPVPLandSeid; //PVP地形加成
private:
	Sprite* m_spBuff;
	Armature* m_armBuff;

	int base_power;						//武力
	int base_defence;					//统帅
	int base_intellegence;				//智力
	int base_explosion;						//敏捷（命中率）
	int base_fortune;						//运气（爆发率）
	int base_speed;

	int power;						//攻击力
	int defence;					//防御力
	int intellegence;				//精神力
	int explosion;						//爆发力（命中率、格档率、连击率）
	int fortune;						//士气（暴击率法术命中率）

	int hp;
	int mp;
	int cur_hp;
	int cur_mp;
	int level;
	int old_level;

	int good_add_power;						//攻击力
	int good_add_defence;					//防御力
	int good_add_intellegence;				//精神力
	int good_add_explosion;						//爆发力（命中率）
	int good_add_fortune;						//士气（爆发率）
	int good_add_hp;							//辅助HP
	int good_add_mp;							//辅助MP
	int good_add_speed;						//辅助移动力
	int m_iSoulBox;							//是否魂匣

	map<int, int> m_EncryptKey;
	map<int, map<int, int> > m_mSpecialEffect;
	map<int, map<int, int> > mSkillItselfLevel;
	map<int, map<int, int> > skill_level;
	map<int, map<int, int> > mExtraGrow;
	map<int, map<int, int> > m_mPracGrown; //获取修炼的成长值
	map<int, int> mFashionAddAbility;

	map<int, int> m_mScriptValue;  //72指令修改属性数值
	map<int, int> m_mScriptPercent;  //72指令修改属性百分比
	map<int, int> m_mAddAbilityGoods;//永久增加属性消耗品
	map<int, map<int, int> > m_mExclusiveGemEffect;

	CC_SYNTHESIZE(bool, m_bIsCopy, IsCopySkill);
};
#endif