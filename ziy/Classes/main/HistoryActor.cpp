#include "Common.h"
#include "main.h"
#include "StatConfig.h"

HistoryActor::HistoryActor()
{
	used_skill_ids.resize(MAX_SKILL_NUM);
	used_skill_ids.memset(0);
	good_add_move_consume_range = NULL;
	good_down_action_range = NULL;//范围牵制行动
	reduce_skill_hurt_buff = NULL;
	add_skill_hurt_buff = NULL;
	good_shield_range = nullptr;
	siege_state = false;
	m_spBuff = NULL;
	m_armBuff = NULL;
	m_pArmSkill = nullptr;
	m_bIsCopy = false;
	m_iShieldNum = 0;
	m_iRestSpeed = 0;

	SetBasepower(0);
	SetBaseDefence(0);
	SetBaseIntellegence(0);
	SetBaseExplosion(0);
	SetBaseFortune(0);
	SetBaseSpeed(0);
	SetBaseHp(0);
	SetBaseMp(0);

	SetCurpower(0);
	SetCurDefence(0);
	SetCurIntellegence(0);
	SetCurExplosion(0);
	SetCurFortune(0);
	SetCurHp(0);
	SetCurMp(0);
	SetActorLv(0);
	SetActorOldLv(0);

	SetGoodsAddPower(0);
	SetGoodsAddDefence(0);
	SetGoodsAddIntellegence(0);
	SetGoodsAddExplosion(0);
	SetGoodsAddFortune(0);
	SetGoodsAddSpeed(0);
	SetGoodsAddHp(0);
	SetGoodsAddMp(0);
}

void HistoryActor::clearGoodEffect()
{
	SetGoodsAddPower(0);					//武力
	SetGoodsAddDefence(0);					//防御力
	SetGoodsAddIntellegence(0);				//智力
	SetGoodsAddExplosion(0);					//敏捷（命中率）
	SetGoodsAddFortune(0);						//士气（爆发率）
	SetGoodsAddHp(0);						//辅助HP
	SetGoodsAddMp(0);							//辅助MP
	SetGoodsAddSpeed(0);				//辅助移动力
	good_recover_hp_round = 0; //每回合恢复Hp百分数
	good_recover_mp_round = 0; //每回合恢复Mp百分数
	
	good_recover_state = 0;	//每回合恢复状态	
	good_shock_move = 0; //突击移动
	good_rough_move = 0; //恶路移动
    good_passtrough_move = 0; //穿越移动
	good_puzzle_hit = 0;  //混乱攻击机率
	good_poison_hit = 0;  //中毒攻击机率
	good_palsy_hit = 0; //麻痹攻击机率
	good_stun_hit = 0; //封杀策略机率
	good_hit_probability = 0; //攻击命中(百分数)
	good_hit_back_back = 0; //反击后反击机率
	good_deadliness_hit = 0; //致命一击机率
	good_hit_distance = 0; //攻击距离类型
	good_hit_area = 0;     //攻击范围类型
	good_hit_no_back = 0; //无反击攻击
	good_induct_hit = 0; //引导攻击人数
	good_assist_fire = 0; //辅助火系攻击
	good_assist_wind = 0; //辅助风系攻击
	good_retrench_MP = 0; //节省MP
	good_call_myth = 0; //召唤四神
	good_skill_probability = 0; //辅助法术命中(百分数)
	good_hit_defence = 0;		//辅助攻击防御率(百分数)(对方命中率下降)
	good_skill_defence = 0;   //辅助法术防御率(百分数)(对方命中率下降)
	good_total_defence = 0;   //辅助全防御
	good_hold_far_hit = 0; //防御远距离攻击
	good_hold_deadliness_hit = 0; //防御致命一击
	good_hold_twice_hit = 0; //防御二次攻击
	good_reduce_skill_hurt = 0; //降低策略伤害
	good_mp_hold_hurt = 0; //mp辅助防御
	good_reduce_far_hit = 0; //降低远距离攻击伤害
	good_auto_use_good = 0; //自动使用道具
	good_hit_fixharm = 0; //破甲攻击
	good_hit_fixharm_begin = 0;
	good_randbuff_hit = 0; //随机状态攻击几率
	good_hit_twice = 0; //双击
	good_hurt_back = 0; //伤害反弹
	good_vampire_attack = 0;//攻击吸血
    good_suck_mp_attack = 0;//攻击吸MP
	good_init_ability = 0;//属性重新计算
	good_back_attack_twice = 0;//反击时可以触发双击
	good_xianshou = 0;
	good_add_power_round = 0;						//每回合加攻击力
	good_add_defence_round = 0;					//每回合加防御力
	good_add_intellegence_round = 0;				//每回合加精神力
	good_add_explosion_round = 0;						//每回合加爆发力（命中率）
	good_add_fortune_round = 0; //每回合加士气
	good_add_speed_round = 0; //每回合加移动
	good_twice_action = 0;

    good_up_power_range->removeAllElements();						//范围增加攻击力
	good_down_power_range->removeAllElements();				//范围减少攻击力
	good_up_defence_range->removeAllElements();					//范围增加防御力
	good_down_defence_range->removeAllElements();					//范围减少防御力
	good_up_intellegence_range->removeAllElements();				//范围增加精神力
	good_down_intellegence_range->removeAllElements();			//范围减少精神力
	good_up_explosion_range->removeAllElements();						//范围增加爆发力（命中率）
	good_down_explosion_range->removeAllElements();						//范围减少爆发力（命中率）
	good_up_fortune_range->removeAllElements(); //范围增加士气
	good_down_fortune_range->removeAllElements(); //范围减少士气
	good_up_speed_range->removeAllElements(); //范围增加移动
	good_down_speed_range->removeAllElements(); //范围减少移动
	good_up_hp_range->removeAllElements(); //范围增加hp
	good_up_mp_range->removeAllElements(); //范围增加mp

	good_up_puzzle_range->removeAllElements(); //范围增加混乱
	good_up_poison_range->removeAllElements(); //范围增加中毒
	good_up_palsy_range->removeAllElements(); //范围增加麻痹
	good_up_stun_range->removeAllElements(); //范围增加封杀
    good_up_blind_range->removeAllElements(); //范围减少失明
    good_up_obfuscation_range->removeAllElements(); //范围减少昏迷
	good_down_puzzle_range->removeAllElements();  //范围减少混乱
	good_down_poison_range->removeAllElements();  //范围减少中毒
	good_down_palsy_range->removeAllElements(); //范围减少麻痹
	good_down_stun_range->removeAllElements(); //范围减少封杀
    good_down_blind_range->removeAllElements(); //范围减少失明
    good_down_obfuscation_range->removeAllElements(); //范围减少昏迷
	if (good_add_move_consume_range)
		good_add_move_consume_range->removeAllElements();//范围增加敌军地形移动消耗
	if (good_down_action_range)
		good_down_action_range->removeAllElements();//范围牵制行动
	good_up_powint_range->removeAllElements();
	good_can_control_tower->removeAllElements();
	good_add_tower_ability->removeAllElements();
	good_add_land_ability->removeAllElements();
	good_being_attack_add_mp->removeAllElements();

	good_intensify_back_hit = 0;//强化反击
	good_intensify_deadlyattack = 0;//强化暴击
	good_mp_attack = 0;//mp攻击
	good_pursuit_attack_num = 0;//追击攻击次数
	good_skill_pursuit_attack_num = 0;
	pursuit_skill_lv = 0;
    good_invisible = 0;
	good_killed_hurt = 0;
	good_attack_must = 0;
	good_weak_attack = 0;
	good_battle_relive = 0;
	good_magic_no_land = 0;//法术无视地形	法术无视地形效果施放
	good_magic_no_air = 0;//法术无视天气 法术无视天气效果施放	
	good_magic_twice = 0;//法术连击（伤害系数）	1%~900%	伤害系数	主动使用法术时造成两次效果
	good_magic_deadly = 0;//法术暴击（伤害系数）	1%~900%	伤害系数	主动使用法术时造成暴击效果
	good_magic_follow = 0;//法术追击	(1~999)&(1%~900%)	“法术Id”&“法术的伤害系数”	攻击范围内的敌军受到物理伤害时，主动追加一次法术攻击	
	good_magic_back = 0;//法术反击	(1~999)&(1%~900%)	“法术Id”&“法术的伤害系数”	受到攻击后，自动追加一次特定法术的还击
	good_alone_up_power = 0;//单独增益(百分比)	(1~999)&(1%~900%)	“属性id”&“加成数值系数”	身边无队友时某属性增加	
	good_loseblood_up_power = 0;//损血增益(百分比)	(1~999)&(1%~900%)	“属性id”&“加成数值系数”	根据损失的血量比例增加的五维数值	
	good_assist_land_power = 0;//辅助地形效果	(1~999)&(1%~900%)	“地形id”&“加成的数值比例”	改变某种地形的适应能力
	good_assist_land_speed = 0;//辅助地形移动损耗	(1~999)&(1~999)	“地形id”&“移动力损耗”	改变某种地形的移动能力	
	memset(good_effects, 0, sizeof(good_effects));
	good_auto_use_good_loss = 0; //自动使用道具消耗库存
	mExtraGrow.clear();
	m_mStoneSeid.clear();
	clearSpecialEffect();
	m_mSeidAddSkill.clear();
	m_mExclusiveGemEffect.clear();
}
void HistoryActor::init(int i)
{
	m_iControllerId = 0;
	ActorAttrPtr actor = &trData->actors[i];
	actor_id = actor->actor_id;
	face_id = actor->face_id;
	occupation_id = actor->occupation_id;

	history_occupation_id = GetActorInitialJob();

	int iIsHalf = IS_HALF_BASEABILITY + 1;
	SetBasepower(actor->power / iIsHalf);
	SetBaseDefence(actor->defence / iIsHalf);
	SetBaseIntellegence(actor->intellegence / iIsHalf);
	SetBaseExplosion(actor->explosion / iIsHalf);
	SetBaseFortune(actor->fortune / iIsHalf);

	SetBaseSpeed(trData->occupations[occupation_id].base_speed);
	SetBaseHp(actor->hp);
	SetBaseMp(actor->mp);
	SetActorLv(actor->level);
	weapon.init(-1,1);
	coat.init(-1, 1);
	assist_good.init(-1, 1);
	fashion.init(-1,1);
	wap_fashion.init(-1, 1);
	exp = 0;
	ai_type = 1;
	relation = -1;
	if (actor->image_fight_id == 0) {
		actor->image_fight_id = trData->occupation_image_id[occupation_id];
	}
	history_image_fight_id = actor->image_fight_id;
	image_id = actor->image_id;
	image_change_occu_id = -1;
	actor_quality = actor->actor_quality;
	actor_old_quality = 0;
	actor_grade = actor->actor_grade;
	
    good_up_power_range = new DList<RangeEffect>(true);						//范围增加攻击力
    good_down_power_range = new DList<RangeEffect>(true);				//范围减少攻击力
    good_up_defence_range = new DList<RangeEffect>(true);					//范围增加防御力
    good_down_defence_range = new DList<RangeEffect>(true);					//范围减少防御力
    good_up_intellegence_range = new DList<RangeEffect>(true);				//范围增加精神力
    good_down_intellegence_range = new DList<RangeEffect>(true);			//范围减少精神力
    good_up_explosion_range = new DList<RangeEffect>(true);						//范围增加爆发力（命中率）
    good_down_explosion_range = new DList<RangeEffect>(true);						//范围减少爆发力（命中率）
    good_up_fortune_range = new DList<RangeEffect>(true);//范围增加士气
    good_down_fortune_range = new DList<RangeEffect>(true); //范围减少士气
    good_up_speed_range = new DList<RangeEffect>(true); //范围增加移动
    good_down_speed_range = new DList<RangeEffect>(true); //范围减少移动
    good_up_hp_range = new DList<RangeEffect>(true); //范围增加hp
    good_up_mp_range = new DList<RangeEffect>(true); //范围增加mp

    good_up_puzzle_range = new DList<RangeEffect>(true); //范围增加混乱
    good_up_poison_range = new DList<RangeEffect>(true); //范围增加中毒
    good_up_palsy_range = new DList<RangeEffect>(true); //范围增加麻痹
    good_up_stun_range = new DList<RangeEffect>(true); //范围增加封杀
    good_up_blind_range = new DList<RangeEffect>(true); //范围减少失明
    good_up_obfuscation_range = new DList<RangeEffect>(true); //范围减少昏迷
    good_down_puzzle_range = new DList<RangeEffect>(true);  //范围减少混乱
    good_down_poison_range = new DList<RangeEffect>(true);  //范围减少中毒
    good_down_palsy_range = new DList<RangeEffect>(true); //范围减少麻痹
    good_down_stun_range = new DList<RangeEffect>(true); //范围减少封杀
    good_down_blind_range = new DList<RangeEffect>(true); //范围减少失明
    good_down_obfuscation_range = new DList<RangeEffect>(true); //范围减少昏迷
	good_add_move_consume_range = new DList<RangeEffect>(true);//范围增加敌军地形移动消耗 
	good_down_action_range = new DList<RangeEffect>(true);//范围牵制行动
	if (!reduce_skill_hurt_buff)
		reduce_skill_hurt_buff = new DList<RangeEffect>(true);
	if (!add_skill_hurt_buff)
		add_skill_hurt_buff = new DList<RangeEffect>(true);
	good_up_powint_range = new DList<RangeEffect>(true);						//范围增加攻击力精神力
	good_shield_range = new DList<RangeEffect>(true);
	good_can_control_tower = new DList<RangeEffect>(true);
	good_add_tower_ability = new DList<RangeEffect>(true);
	good_add_land_ability = new DList<RangeEffect>(true);
	good_being_attack_add_mp = new DList<RangeEffect>(true);

	clearGoodEffect();
	mFashionAddAbility.clear();
	state_power = 0;
	state_intellegence = 0;
	state_defence = 0;
	state_explosion = 0;
	state_fortune = 0;
	state_puzzle = 0;
	state_poison = 0;
	state_palsy = 0;
	state_stun = 0;
	state_speed = 0;
	state_blind = 0;
	state_obfuscation = 0;
	state_skillhurt = 0;
	state_reduce_skill_hurt_range = 0;
	state_SkillHurtAndRebound = 0;
	state_add_skill_hurt_range = 0;
	state_adddef_redpow = 0;
	state_loseblood = 0;
	state_addhp = 0;
	state_sual_link = 0;
	state_reduceSkill = 0;
	state_reduceHurt = 0;
	state_emergency_command = 0;

	mFashion.clear();
	mInitFashion.clear();

	if (trData->net_hero_no_dress.count(actor_id + 1))
	{
		int fashion1 = trData->net_hero_no_dress[actor_id + 1]->fashion1;
		int fashion2 = trData->net_hero_no_dress[actor_id + 1]->fashion2;
		int iGender = trData->net_hero_no_dress[actor_id + 1]->gender;
		if (iGender < 1) iGender = trData->actors[actor_id].iGender;
		if (fashion1 > 0 && fashion2 > 0)
		{
			mFashion[0] = 1;
			mFashion[1] = iGender;
			mFashion[2] = fashion1;
			mFashion[3] = 100;
			mFashion[4] = fashion2;

			mInitFashion[0] = 1;
			mInitFashion[1] = iGender;
			mInitFashion[2] = fashion1;
			mInitFashion[3] = 100;
			mInitFashion[4] = fashion2;
		}
	}

	hide = 0;
	skill_level.clear();
	mSkillItselfLevel.clear();
	m_iExternalEquip = 0;
	stradd_power = 0;
	stradd_defence = 0;
	stradd_intellegence = 0;
	stradd_explosion = 0;
	stradd_fortune = 0;
	SetDefaultGood(GOOD_TYPE_ALL);
	SetCurHp(GetBaseHp() + GetGoodsAddHp());
	SetCurMp(GetBaseMp() + GetGoodsAddMp());
	battleTimes = 0;
	retreatTimes = 0;
	
	force_save = 0;
	isAssistArmy = 0;
	limit_actor_move = 0;
	limit_actor_move_x1 = 0;
	limit_actor_move_y1 = 0;
	limit_actor_move_x2 = 0;
	limit_actor_move_y2 = 0;
	set_hero_seid.resize(0);
	used_skill_ids.resize(MAX_SKILL_NUM);
	used_skill_ids.memset(0);
	is_leave = 0;
	state_weaked = 0;
	weaked_num = 0;
	is_used_relive = 0;
	bust_style = 0;
	AcquisitionSeid.resize(0);
	iTwiceAction = 0;
	iActionNum = -1;
	buff_hate_attack = 0;
	controlled_by_enemy = 0;
	controlled_by_enemy_round = 0;
	controlled_by_enemy_addAbi = 0;
	border_on_death = 0;
	free_allocation = 0;
	ConsumeSeid.resize(0);
	enable_set_occupation_id = 0;
	actor_only_move = 0;
	blood_shield = 0;
	light_refle = 0;
	vital_spirit = 0;
	hp_mp_defense_number = 0;
	paint_special_effect[0] = -1;
	paint_special_effect[1] = -1;
	actor_fit_id.resize(0);
	animation_index = -1;
	m_iShareTimes = 0;
	m_iSoulBox = 0;
	
	mElitePlus.clear();
	clearActorBuff();
	m_mPracGrown.clear();
	m_mStoneSeid.clear();
	m_mPVPLandSeid.clear();
}

float HistoryActor::getGrownValue(int iAttrType, bool bIsTotal /*= true*/)
{
	int iGIndex = trData->actors[actor_id].qid;
	float fAbility = 0;
	if(iAttrType == ACTOR_ABILITY_POWER)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->atk;
	}
	else if(iAttrType == ACTOR_ABILITY_DEFENCE)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->def;
	}
	else if(iAttrType == ACTOR_ABILITY_INTELLEGENCE)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->ints;
	}
	else if(iAttrType == ACTOR_ABILITY_EXPLOSION)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->burst;
	}
	else if(iAttrType == ACTOR_ABILITY_FORTUNE)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->fortune;
	}
	else if(iAttrType == ACTOR_ABILITY_HPUP)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->hp_up;
	}
	else if(iAttrType == ACTOR_ABILITY_MPUP)
	{
		fAbility = trData->net_hero_grown[iGIndex][actor_quality]->mp_up;
	}

	if (bIsTotal) //修炼成长
	{
		fAbility += getPracGrown(iAttrType);
	}
	return fAbility + GetExtraGrow(iAttrType) / 10.0f;
}

int HistoryActor::GetGrownAbility(int iLevel, int iAbilityType)
{
	return iLevel * getGrownValue(iAbilityType);
}

//重新计算人物五维
void HistoryActor::InitFiveBaseAbility()
{
	SetCurpower(GetBasepower() + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_POWER));
	SetCurDefence(GetBaseDefence() + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_DEFENCE));
	SetCurIntellegence(GetBaseIntellegence() + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_INTELLEGENCE));
	SetCurExplosion(GetBaseExplosion() + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_EXPLOSION));
	SetCurFortune(GetBaseFortune() + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_FORTUNE));
}

void HistoryActor::InitFiveAllAbility()
{
	InitHPAndMPAbility();
	InitFiveBaseAbility();
	AddElitePlus();
}
void HistoryActor::InitHPAndMPAbility()
{
	SetBaseHp(trData->actors[actor_id].hp + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_HPUP));
	SetBaseMp(trData->actors[actor_id].mp + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_MPUP));
	SetCurHp(GetBaseHp() + GetGoodsAddHp());
	SetCurMp(GetBaseMp() + GetGoodsAddMp());
}

void HistoryActor::AddElitePlus()
{
	map<int, int> mAbility;
	if(mElitePlus[0] != 0 || (history->mEnemyElitePlus.size() && relation == ACTOR_RELATION_ENEMY))
	{
		int iElitePow = 0, iEliteDef = 0, iEliteInt = 0, iEliteExp = 0, iEliteFor = 0, iEliteHp = 0, iEliteMp = 0;

		if (mElitePlus[0] == 3 || mElitePlus[0] == 4)
		{
			iEliteHp = mElitePlus[1];
			iEliteMp = mElitePlus[2];
			iElitePow = mElitePlus[3];
			iEliteDef = mElitePlus[4];
			iEliteInt = mElitePlus[5];
			iEliteExp = mElitePlus[6];
			iEliteFor = mElitePlus[7];
		}
		else if(mElitePlus[1])//1和2去除mp加成
		{
			iEliteHp = mElitePlus[1];
			iElitePow = mElitePlus[1];
			iEliteDef = mElitePlus[1];
			iEliteInt = mElitePlus[1];
			iEliteExp = mElitePlus[1];
			iEliteFor = mElitePlus[1];
		}

		if (history->mEnemyElitePlus.size() && (relation == ACTOR_RELATION_ENEMY || relation == ACTOR_RELATION_FRIEND) && !(actor_id >= LADDER_ACTOR_MIN_ID && actor_id < LADDER_ACTOR_MAX_ID))
		{
			map<int, int> EnemyElitePlusM;
			if (history->mEnemyElitePlus.count(actor_id + 1))
			{
				EnemyElitePlusM = history->mEnemyElitePlus[actor_id + 1];
			}
			else if (history->mEnemyElitePlus.count(0)&& relation == ACTOR_RELATION_ENEMY)
			{
				EnemyElitePlusM = history->mEnemyElitePlus[0];
			}

			if (EnemyElitePlusM.size())
			{
				iEliteHp += EnemyElitePlusM[1];
				iEliteMp += EnemyElitePlusM[2];
				iElitePow += EnemyElitePlusM[3];
				iEliteDef += EnemyElitePlusM[4];
				iEliteInt += EnemyElitePlusM[5];
				iEliteExp += EnemyElitePlusM[6];
				iEliteFor += EnemyElitePlusM[7];
			}
		}

		mAbility[SCRIPT_HP] = iEliteHp;
		mAbility[SCRIPT_MP] = iEliteMp;
		mAbility[SCRIPT_PW] = iElitePow;
		mAbility[SCRIPT_DEF] = iEliteDef;
		mAbility[SCRIPT_INT] = iEliteInt;
		mAbility[SCRIPT_EXP] = iEliteExp;
		mAbility[SCRIPT_FOR] = iEliteFor;
	}
	addBaseAbility(mAbility);
}

void HistoryActor::addBaseAbility(map<int, int> mAbility, int iType /*= 1*/)
{
	for(int i = 0; i < SCRIPT_MAX; i++)
	{
		if (iType == 1)
		{
			m_mScriptPercent[i] += mAbility[i];
		}
		else if (iType == 2)
		{
			m_mScriptValue[i] += mAbility[i];
		}
	}
	updateBaseAbility();
}

void HistoryActor::updateBaseAbility()
{
	int iReduceHp = getTotalAbility(TOTAL_HP) - GetCurHp();
	int iReduceMp = getTotalAbility(TOTAL_MP) - GetCurMp();

	InitHPAndMPAbility();
	InitFiveBaseAbility();

	SetCurpower(-1, GetCurpower() * m_mScriptPercent[SCRIPT_PW] / 100 + m_mScriptValue[SCRIPT_PW]);
	SetCurDefence(-1, GetCurDefence() * m_mScriptPercent[SCRIPT_DEF] / 100 + m_mScriptValue[SCRIPT_DEF]);
	SetCurIntellegence(-1, GetCurIntellegence() * m_mScriptPercent[SCRIPT_INT] / 100 + m_mScriptValue[SCRIPT_INT]);
	SetCurExplosion(-1, GetCurExplosion() * m_mScriptPercent[SCRIPT_EXP] / 100 + m_mScriptValue[SCRIPT_EXP]);
	SetCurFortune(-1, GetCurFortune() * m_mScriptPercent[SCRIPT_FOR] / 100 + m_mScriptValue[SCRIPT_FOR]);
	SetBaseHp(-1, GetBaseHp() * m_mScriptPercent[SCRIPT_HP] / 100 + m_mScriptValue[SCRIPT_HP]);
	SetCurHp(getTotalAbility(TOTAL_HP) - iReduceHp);
	SetBaseMp(-1, GetBaseMp() * m_mScriptPercent[SCRIPT_MP] / 100 + m_mScriptValue[SCRIPT_MP]);
	SetCurMp(getTotalAbility(TOTAL_MP) - iReduceMp);
}

//1.攻击力 2.防御力 3.精神力，4.爆发力，5士气
void HistoryActor::InitoneBaseAbility(int iIndex)
{
	int iGrownAbility = GetGrownAbility(GetActorLv(), iIndex);
	switch(iIndex)
	{
	case ACTOR_ABILITY_POWER:
		SetCurpower(GetBasepower() + iGrownAbility);
		break;
	case ACTOR_ABILITY_INTELLEGENCE:
		SetCurIntellegence(GetBaseIntellegence() + iGrownAbility);
		break;
	case ACTOR_ABILITY_DEFENCE:
		SetCurDefence(GetBaseDefence() + iGrownAbility);
		break;
	case ACTOR_ABILITY_EXPLOSION:
		SetCurExplosion(GetBaseExplosion() + iGrownAbility);
		break;
	case ACTOR_ABILITY_FORTUNE:
		SetCurFortune(GetBaseFortune() + iGrownAbility);
		break;
	case ACTOR_ABILITY_HPUP:
	{
		SetBaseHp(trData->actors[actor_id].hp + iGrownAbility);
		SetCurHp(GetBaseHp() + GetGoodsAddHp());
	}
		break;
	case ACTOR_ABILITY_MPUP:
	{
		SetBaseMp(trData->actors[actor_id].mp + iGrownAbility);
		SetCurMp(GetBaseMp() + GetGoodsAddMp());
	}
		break;
	default:
		break;
	}
}

void HistoryActor::InitAbility()
{
	int cval = 1;//职业可转职数
	for (int i = 0; i<trData->occupation_type_num;i++)
	{
		if (trData->occu_types[i].occupation_start == history_occupation_id)
		{
			cval = trData->occu_types[i].cval;
			break;
		}
	}

	OccupationAttrPtr occu = &trData->occupations[occupation_id];
	if(relation == ACTOR_RELATION_OWN)
	{
		InitFiveBaseAbility();
		SetBaseHp(trData->actors[actor_id].hp + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_HPUP));
		SetBaseMp(trData->actors[actor_id].mp + GetGrownAbility(GetActorLv(), ACTOR_ABILITY_MPUP));
		SetCurHp(GetBaseHp() + GetGoodsAddHp());
		SetCurMp(GetBaseMp() + GetGoodsAddMp());
	}
	else
	{
		SetCurpower(GetBasepower() + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_POWER));
		SetCurDefence(GetBaseDefence() + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_DEFENCE));
		SetCurIntellegence(GetBaseIntellegence() + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_INTELLEGENCE));
		SetCurExplosion(GetBaseExplosion() + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_EXPLOSION));
		SetCurFortune(GetBaseFortune() + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_FORTUNE));
		SetBaseHp(trData->actors[actor_id].hp + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_HPUP));
		SetBaseMp(trData->actors[actor_id].mp + GetGrownAbility(GetActorLv() - 1, ACTOR_ABILITY_MPUP));
		SetCurHp(GetBaseHp() + GetGoodsAddHp());
		SetCurMp(GetBaseMp() + GetGoodsAddMp());
	}
}

void HistoryActor::UpdateSkills(int iMaxLevel)
{
	int need_level,iSkillId = 0;
	SkillAttrPtr skill = NULL;
	skill_level.clear();
	int iSkillIndex = trData->actors[actor_id].skill;
	if (iSkillIndex> 0 && trData->net_hero_skill.find(iSkillIndex) != trData->net_hero_skill.end())
	{
		for (map<int, int>::iterator it = trData->net_hero_skill[iSkillIndex]->skill.begin(); it != trData->net_hero_skill[iSkillIndex]->skill.end(); it++)
		{
			iSkillId = it->first;
			need_level = it->second;
			if (need_level > 0 && need_level <= GetActorLv() && CurSkillLevelIsNull(iSkillId) == true)
			{
				if (!SelfSkillLevelIsNull(iSkillId))
					SetCurSkillLevel(iSkillId, GetSelfSkillLevel(iSkillId));
				else
					SetCurSkillLevel(iSkillId, 1);

				if (iMaxLevel > 0 && ((need_level - 1) + GetCurSkillLevel(iSkillId))>iMaxLevel)
				{
					SetCurSkillLevel(iSkillId, iMaxLevel - (need_level - 1));
				}
			}
		}
	}
}
void HistoryActor::SetDefaultGood(int good_type, bool isGoodSet)
{
	int offset = 1;
	char good_level =  1;
	
	if(good_type&GOOD_TYPE_WEAPON)
	{
		if(isGoodSet && weapon.isNormalGood() && relation == 0)
		{
			//3E指令设定装备 && 我军 && 已有装备----------满足以上条件则不变更装备
		}
		else
		{
			for(int i = 0; i < trData->iWeaponNum; i++)
			{
				int iGoodId = trData->ptrDefWeapon[i];
				HistoryGoods hisGoods(iGoodId);
				if(trData->occupation_good_info[occupation_id][hisGoods.getItId()])
				{
					weapon.init(iGoodId + offset, good_level);
					break;
				}
			}
		}
	}
	if(good_type&GOOD_TYPE_COAT)
	{
		if(isGoodSet && coat.isNormalGood() && relation == 0)
		{
			//3E指令设定装备 && 我军 && 已有装备----------满足以上条件则不变更装备
		}
		else
		{
			for(int i = 0; i < trData->iCoatNum; i++)
			{
				int iGoodId = trData->ptrDefCoat[i];
				HistoryGoods hisGoods(iGoodId);
				if(trData->occupation_good_info[occupation_id][hisGoods.getItId()])
				{
					coat.init(iGoodId + offset, good_level);
					break;
				}
			}
		}
	}
	UpdataGoodEffect();
}

void HistoryActor::SetNullGood()
{
	bool enable_update = false;
	if (weapon.good_id > 0)
	{
		weapon.init(-1, 1);
		enable_update = true;
	}
	if (coat.good_id > 0)
	{
		coat.init(-1, 1);
		enable_update = true;
	}
	if (assist_good.good_id > 0)
	{
		assist_good.init(-1, 1);
		enable_update = true;
	}
	if (fashion.good_id > 0)
	{
		fashion.init(-1, 1);
		enable_update = true;
	}
	if (wap_fashion.good_id > 0)
	{
		wap_fashion.init(-1, 1);
		enable_update = true;
	}
	if (enable_update)
		UpdataGoodEffect();
}

void HistoryActor::UpdataGoodEffect(int iOpenTerrainSuit)
{
	/////替换时装
	SetFashionData();

	clearGoodEffect();
	int iGoodsAddLevelSeid = setGoodAddLevel();
	if (weapon.isNormalGood())
	{
		weapon.setGoodAddLevel(iGoodsAddLevelSeid);
		AddGoodAttr(&weapon, weapon.GetGoodsPower());
		map<int, int> mSeid = weapon.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			AddSpecialEffect(mSeid[i]);
		}
	}

	if (coat.isNormalGood())
	{
		coat.setGoodAddLevel(iGoodsAddLevelSeid);
		AddGoodAttr(&coat, coat.GetGoodsPower());
		map<int, int> mSeid = coat.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			AddSpecialEffect(mSeid[i]);
		}
	}

	if (assist_good.isNormalGood())
	{
		map<int, int> mSeid = assist_good.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			AddSpecialEffect(mSeid[i]);
		}
	}
	//衣服时装
	if (fashion.isNormalGood())
	{
		map<int, int> mSeid = fashion.getSeid();
		for (UInt i = 0; i < mSeid.size(); i++)
		{
			AddSpecialEffect(mSeid[i]);
		}
	}
	//武器时装
	if (wap_fashion.isNormalGood())
	{
		map<int, int> mSeid = wap_fashion.getSeid();
		for (UInt i = 0; i < mSeid.size(); i++)
		{
			AddSpecialEffect(mSeid[i]);
		}
	}

	updateStoneSuit();
	for(UInt i = 0; i < m_mStoneSeid.size(); i++)
	{
		AddSpecialEffect(m_mStoneSeid[i]);
	}

	//PVP地形特技
	for (UInt i = 0; i < m_mPVPLandSeid.size(); i++)
	{
		AddSpecialEffect(m_mPVPLandSeid[i]);
	}

	//职业特技
	for (int i = 0; i < trData->occupations[this->occupation_id].seid_num; i++)
	{
		AddSpecialEffect(trData->occupations[this->occupation_id].seid[i]);
	}
	//人物特技

	if (this->actor_id < trData->actor_num)
	{
		map<int, int> mTHeroSeid = getDicHeroSeid();
		for (size_t i = 0; i < mTHeroSeid.size(); i++)
		{
			AddSpecialEffect(mTHeroSeid[i]);
		}
	}
	//指定习得特效
	for (int i = 0; i<this->AcquisitionSeid.size(); i++)
	{
		if (this->AcquisitionSeid[i] == 0) break;
		AddSpecialEffect(this->AcquisitionSeid[i]);
	}
	//指定习得特效
	for (int i = 0; i < this->ConsumeSeid.size(); i++)
	{
		if (this->ConsumeSeid[i] == 0) break;
		AddSpecialEffect(this->ConsumeSeid[i]);
	}

	int iRatioId = history->getRatioList(actor_id + 1);
	if (iRatioId&&trData->net_ratio_buff.count(iRatioId))
	{
		NetRatioBuff* ptrNetRatioBuff = trData->net_ratio_buff[iRatioId];
		for (size_t i = 0; i < ptrNetRatioBuff->seid.size(); i++)
		{
			AddSpecialEffect(ptrNetRatioBuff->seid[i]);
		}
	}

	//添加永久增加属性物品
	setAddAbilityGoodsEffect();

	GetSuitEffect();
	setStrAddGoodsEffect();//获得强化能力

	//成长加成
	setTowerAddAbility();
	setSpecialAddAbility();
	SetFashionAddAbility();

	//特技习得技能
	GetSeidAddSkill();

	//减少hp上限
	if (isHadSpecial(TYPE764))
	{
		int iPre = getSpecialMap(TYPE764)[1];
		if (iPre >= 100) iPre = 99;
		int iGainHp = -(GetBaseHp() + GetGoodsAddHp())*iPre / 100 + GetGoodsAddHp();
		SetGoodsAddHp(iGainHp);
	}

	if (GetCurHp() >(GetBaseHp() + GetGoodsAddHp()))//更换装备之后原来good_add_hp可能归0;
	{
		SetCurHp(GetBaseHp() + GetGoodsAddHp());
	}
	if (GetCurMp() > (GetBaseMp() + GetGoodsAddMp()))//更换装备之后原来good_add_mp可能归0;
	{
		SetCurMp(GetBaseMp() + GetGoodsAddMp());
	}
}

void HistoryActor::AddSpecialEffect(int seid, int baseLevel /*= 1*/, bool bIsStreng /*= false*/)
{
	if (seid == 0)
	{
		return;
	}

	if(bIsStreng)
	{
		StrengthenPropertyPtr ptrStreng = &trData->strengthen_property[seid];
		seid = ptrStreng->id + MAX_SEID_NUM;
	}

	Seid* ptrSeid = trData->m_mSeid[seid];

	if(ptrSeid->mOpenCondition[0]>0&&!GetSeidIsOpen(seid))
		return;

	switch(ptrSeid->type)
	{
	case 1:// 每回合恢复HP(百分数)
	case 240:
		good_recover_hp_round += ptrSeid->special_effe[0];
		break;
	case 2:// 每回合恢复MP(百分数)
	case 241:
		good_recover_mp_round += ptrSeid->special_effe[0];
		break;
	case 3:// 每回合恢复状态
	case 242:
		good_recover_state = 100;
		break;
	case 4:// 每回合获得Exp(整数)
	case 243:
		break;
	case 5:// 每回合获得武器Exp(整数)
	case 244:
		break;
	case 6:// 每回合获得道具Exp(整数)
	case 245:
		break;
	case 7:// 辅助攻击力(整数)
	case 208:
		SetGoodsAddPower(-1, ptrSeid->special_effe[0]);
		break;
	case 8:// 辅助精神力(整数)
	case 209:
		SetGoodsAddIntellegence(-1, ptrSeid->special_effe[0]);
		break;
	case 9:// 辅助防御力(整数)
	case 210:
		SetGoodsAddDefence(-1, ptrSeid->special_effe[0]);
		break;
	case 10:// 辅助爆发力(整数)
	case 211:
		SetGoodsAddExplosion(-1, ptrSeid->special_effe[0]);
		break;
	case 11:// 辅助运气(整数)
	case 212:
		SetGoodsAddFortune(-1, ptrSeid->special_effe[0]);
		break;
	case 12:// 辅助HP增加(整数)
	case 213:
	{
		SetGoodsAddHp(-1, ptrSeid->special_effe[0]);
		SetCurHp(-1, ptrSeid->special_effe[0]);
		if(GetCurHp() > (GetBaseHp() + GetGoodsAddHp()))
		{
			SetCurHp(GetBaseHp() + GetGoodsAddHp());
		}
	}
		break;
	case 13:// 辅助MP增加(整数)
	case 214:
	{
		SetGoodsAddMp(-1, ptrSeid->special_effe[0]);
		SetCurMp(-1, ptrSeid->special_effe[0]);
		if(GetCurMp() > (GetBaseMp() + GetGoodsAddMp()))
		{
			SetCurMp(GetBaseMp() + GetGoodsAddMp());
		}
	}
		break;
	case 14:// 辅助获得Exp(十分数)
		break;
	case 16:// 突击移动
	case 271:
		good_shock_move = 1;
		break;
	case 17:// 恶路移动
	case 272:
		good_rough_move = 1;
		break;
	case 18:// 混乱攻击 TYPE18~21为100%命中，如果需要命中率可配置用TYPE151~154
		good_puzzle_hit = 100;
		break;
	case 19:// 中毒攻击
		good_poison_hit = 100;
		break;
	case 20:// 麻痹攻击
		good_palsy_hit = 100;
		break;
	case 21:// 封杀策略
		good_stun_hit = 100;
		break;
	case 22:// 辅助攻击命中(百分数)
	case 219:
		good_hit_probability += ptrSeid->special_effe[0]; //攻击命中(百分数)
		break;
	case 23:// 反击后反击
	case 101:
		good_hit_back_back = 100; //反击后反击机率
		break;
	case 24:// 致命一击攻击
	case 106:
		good_deadliness_hit = 100; //致命一击机率
		break;
	case 25:// 攻击距离变更
	case 118:
		good_hit_distance = ptrSeid->special_effe[0];
		break;
	case 26:// 攻击作用范围变更
	case 119:
		good_hit_area = ptrSeid->special_effe[0];
		break;
	case 27:// 无反击攻击
	case 102:
		good_hit_no_back = 100; //无反击攻击
		break;
	case 29:// 引导攻击
	case 108://TODO 引导几人 
	{
		if(ptrSeid->special_effe[0] > good_induct_hit)
		{
			good_induct_hit = ptrSeid->special_effe[0];
		}
	}
		break;
	case 30:// 辅助火类策略(百分数)
		good_assist_fire += ptrSeid->special_effe[0];
		break;
	case 31:// 辅助风类策略(百分数)
		good_assist_wind += ptrSeid->special_effe[0];
		break;
	case 32:// 节约MP(百分数)
	case 135:
		good_retrench_MP += ptrSeid->special_effe[0];
		break;
	case 33:// 召唤攻击
		good_call_myth = 1; //召唤四神
		break;
	case 34:// 策略模仿
	case 273:
		break;
	case 35:// 辅助策略命中(百分数)
		good_skill_probability += ptrSeid->special_effe[0]; //辅助法术命中(百分数)
		break;
	case 36:// 辅助攻击防御(百分数)
	case 221:
		good_hit_defence += ptrSeid->special_effe[0];		//辅助攻击防御(百分数)
		break;
	case 37:// 辅助策略防御(百分数)
		good_skill_defence += ptrSeid->special_effe[0];   //辅助法术防御(百分数)
		break;
	case 38:// 辅助全防御
	case 223:
		good_total_defence += ptrSeid->special_effe[0];   //辅助全防御
		break;
	case 39: //防御远程攻击
	case 180:
		good_hold_far_hit = 100;
		break;
	case 40:// 防止致命一击
		good_hold_deadliness_hit = 100;
		break;
	case 41:// 防止二次攻击
		good_hold_twice_hit = 100;
		break;
	case 42:// 减轻策略损伤(百分数)
		good_reduce_skill_hurt += ptrSeid->special_effe[0];
		break;
	case 43:// mp辅助防御
		good_mp_hold_hurt = 1;
		break;
	case 44:// 减轻远距离损伤(百分数)
		good_reduce_far_hit += ptrSeid->special_effe[0];
		break;
	case 45:// 自动使用道具不消耗库存
		good_auto_use_good = ptrSeid->special_effe[0];
		break;
	case 274:// 自动使用道具消耗库存
		good_auto_use_good_loss = ptrSeid->special_effe[0];
		break;
	case 46:// 解除友军和援军所有异常状态，且每回合增加20hp（同人印）
		good_effects[0] = 1;
		break;
	case 47://攻击两次
	case 105:
		good_hit_twice = 100;
		break;
	case 48://先手攻击
	case 107:
		good_xianshou = 100;
		break;
	case 103://反击时可以触发双击
		good_back_attack_twice = 100;
		break;
	case 110://强化反击
		good_intensify_back_hit += ptrSeid->special_effe[0];
		break;
	case 111://强化暴击
		good_intensify_deadlyattack += ptrSeid->special_effe[0];
		break;
	case 112://吸血攻击
		good_vampire_attack += ptrSeid->special_effe[0];
		break;
	case 113://破甲攻击
	{
		good_hit_fixharm = ptrSeid->special_effe[0];
		if(ptrSeid->special_effe_num > 1)
		{
			good_hit_fixharm_begin = ptrSeid->special_effe[1];
		}
		else
		{
			good_hit_fixharm_begin = 0;
		}
	}
		break;
	case 115://mp攻击
		good_mp_attack += ptrSeid->special_effe[0];
		break;
	case 116://突击攻击
		good_effects[13] = seid;
		break;
	case 117://掠夺攻击
		break;
	case 131://法术无视天气
		good_magic_no_air = 1;
		break;
	case 132://法术无视地形
		good_magic_no_land = 1;
		break;
	case 133://法术连击
		good_magic_twice += ptrSeid->special_effe[0];
		break;
	case 134://法术暴击
		good_magic_deadly += ptrSeid->special_effe[0];
		break;
	case 151:// 混乱攻击（百分数）
		good_puzzle_hit += ptrSeid->special_effe[0];
		break;
	case 152:// 中毒攻击
		good_poison_hit += ptrSeid->special_effe[0];  //中毒攻击机率
		break;
	case 153:// 麻痹攻击
		good_palsy_hit += ptrSeid->special_effe[0]; //麻痹攻击机率
		break;
	case 154://禁咒攻击
		good_stun_hit += ptrSeid->special_effe[0];
		break;
	case 155://昏迷攻击
		good_effects[14] += ptrSeid->special_effe[0];
		break;
	case 156://随机状态攻击
		good_randbuff_hit += ptrSeid->special_effe[0];
		break;
	case 157://破坏攻击
		good_effects[2] += ptrSeid->special_effe[0];
		break;
	case 158://破坏精神
		good_effects[3] += ptrSeid->special_effe[0];
		break;
	case 159://破坏防御
		good_effects[4] += ptrSeid->special_effe[0];
		break;
	case 160://破坏爆发
		good_effects[5] += ptrSeid->special_effe[0];
		break;
	case 161://破坏士气
		good_effects[6] += ptrSeid->special_effe[0];
		break;
	case 162://破坏移动
		good_effects[7] += ptrSeid->special_effe[0];
		break;
	case 163://随机属性攻击
		good_effects[21] += ptrSeid->special_effe[0];
		break;
	case 181://防止致命一击
		good_hold_deadliness_hit = 100;
		break;
	case 182://防止二次攻击
		good_hold_twice_hit = 100;
		break;
	case 183://伤害反弹
		good_hurt_back += ptrSeid->special_effe[0];
		break;
	case 184://金钱防御
		break;
	case 185://mp辅助防御
		good_effects[20] += ptrSeid->special_effe[0];
		break;
	case 186://防御异常属性攻击
		good_effects[11] += ptrSeid->special_effe[0];
		break;
	case 187://防御异常状态攻击
		good_effects[12] += ptrSeid->special_effe[0];
		break;
	case 201://辅助攻击力
		SetGoodsAddPower(-1, GetCurpower() * ptrSeid->special_effe[0] / 100);
		break;
	case 202://辅助精神力
		SetGoodsAddIntellegence(-1, GetCurIntellegence() * ptrSeid->special_effe[0] / 100);
		break;
	case 203://辅助防御力
		SetGoodsAddDefence(-1, GetCurDefence() * ptrSeid->special_effe[0] / 100);
		break;
	case 204://辅助爆发力
		SetGoodsAddExplosion(-1,GetCurExplosion() * ptrSeid->special_effe[0] / 100);
		break;
	case 205://辅助运气
		SetGoodsAddFortune(-1,GetCurFortune() * ptrSeid->special_effe[0] / 100);
		break;
	case 206://辅助HP增加
	{
		int iBaseHp = GetBaseHp();
		SetGoodsAddHp(-1, iBaseHp*ptrSeid->special_effe[0] / 100);
		SetCurHp(-1, iBaseHp*ptrSeid->special_effe[0] / 100);
		if (GetCurHp() > (iBaseHp + GetGoodsAddHp()))
		{
			SetCurHp(iBaseHp + GetGoodsAddHp());
		}
	}
		break;
	case 207://辅助MP增加
	{
		int iBaseMp = GetBaseMp();
		SetGoodsAddMp(iBaseMp*ptrSeid->special_effe[0] / 100);
		SetCurMp(-1, iBaseMp*ptrSeid->special_effe[0] / 100);
		if (GetCurMp() > (iBaseMp + GetGoodsAddMp()))
		{
			SetCurMp(-1, iBaseMp + GetGoodsAddMp());
		}
	}
		break;
	case 215:
		break;
	case 216://辅助移动力
		SetGoodsAddSpeed(-1, ptrSeid->special_effe[0]);
		break;
	case 246://每回合自动加攻击
		good_add_power_round = 100;
		break;
	case 247://每回合自动加精神
		good_add_intellegence_round = 100;
		break;
	case 248://每回合自动加防御
		good_add_defence_round = 100;
		break;
	case 249://每回合自动加爆发
		good_add_explosion_round = 100;
		break;
	case 250://每回合自动加士气
		good_add_fortune_round = 100;
		break;
	case 251://每回合自动加移动
		good_add_speed_round = 100;
		break;
	case 270://回归
		good_twice_action = 1;
		break;
	case 275://神魔附体
		break;
	case 304://减轻法术伤害
		good_effects[18] = seid;
		break;
	case 306://会地风水火
		break;
	case 307://法术追击
		good_effects[22] = seid;
		break;
	case 308://法术反击
		good_magic_back = seid;
		break;
	case 311://辅助法术命中
		break;
	case 312://辅助法术格挡
		break;
	case 321://辅助地形移动损耗
		good_effects[23] = seid;
		break;
	case 351://范围属性增强
	{
		short* special_effe = ptrSeid->special_effe;
		switch(special_effe[0])
		{
		case 951:
			good_up_power_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 952:
			good_up_intellegence_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 953:
			good_up_defence_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 954:
			good_up_explosion_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 955:
			good_up_fortune_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 956:
			good_up_speed_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		case 959:
			good_up_power_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			good_up_intellegence_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			good_up_defence_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			good_up_explosion_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			good_up_fortune_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			good_up_speed_range->addElement(new RangeEffect(special_effe[2], special_effe[1], 100));
			break;
		default:
			break;
		}
	}
		break;
	case 352://范围属性减弱
	{
		short* special_effe = ptrSeid->special_effe;
		switch(special_effe[0])
		{
		case 951:
			good_down_power_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 952:
			good_down_intellegence_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 953:
			good_down_defence_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 954:
			good_down_explosion_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 955:
			good_down_fortune_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 956:
			good_down_speed_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		case 959:
			good_down_power_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			good_down_intellegence_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			good_down_defence_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			good_down_explosion_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			good_down_fortune_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			good_down_speed_range->addElement(new RangeEffect(special_effe[2], -special_effe[1], 100));
			break;
		default:
			break;
		}
	}
		break;
	case 353://范围状态效果
	{
		short* special_effe = ptrSeid->special_effe;
		switch(special_effe[0])
		{
		case 931:
			good_up_blind_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1])); //范围增加失明
			break;
		case 932:
			good_up_puzzle_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));  //范围增加混乱
			break;
		case 933:
			good_up_poison_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));  //范围增加中毒
			break;
		case 934:
			good_up_palsy_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1])); //范围增加麻痹
			break;
		case 935:
			good_up_stun_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1])); //范围增加封杀
			break;
		case 936:
			good_up_obfuscation_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1])); //范围增加昏迷
			break;
		case 939:
			good_up_blind_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			good_up_puzzle_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			good_up_poison_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			good_up_palsy_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			good_up_stun_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			good_up_obfuscation_range->addElement(new RangeEffect(special_effe[2], 1, special_effe[1]));
			break;
		default:
			break;
		}
	}
		break;
	case 354://范围回复状态
	{
		short* special_effe = ptrSeid->special_effe;
		switch(special_effe[0])
		{
		case 931:
			good_down_blind_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1])); //范围减少失明
			break;
		case 932:
			good_down_puzzle_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));  //范围减少混乱
			break;
		case 933:
			good_down_poison_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));  //范围减少中毒
			break;
		case 934:
			good_down_palsy_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1])); //范围减少麻痹
			break;
		case 935:
			good_down_stun_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1])); //范围减少封杀
			break;
		case 936:
			good_down_obfuscation_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1])); //范围减少昏迷
			break;
		case 939:
			good_down_blind_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			good_down_puzzle_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			good_down_poison_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			good_down_palsy_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			good_down_stun_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			good_down_obfuscation_range->addElement(new RangeEffect(special_effe[2], -1, special_effe[1]));
			break;
		default:
			break;
		}

	}
		break;
	case 355://范围回复HP
	{
		short* special_effe = ptrSeid->special_effe;
		good_up_hp_range->addElement(new RangeEffect(special_effe[1], special_effe[0], 100));
	}
		break;
	case 356://范围回复MP
	{
		short* special_effe = ptrSeid->special_effe;
		good_up_mp_range->addElement(new RangeEffect(special_effe[1], special_effe[0], 100));
	}
		break;
	case 357://属性重新计算
		good_init_ability = 100;
		break;
	case 359://精神防御
		SetGoodsAddDefence(-1, GetCurIntellegence() * ptrSeid->special_effe[0] / 100);
		break;
	case 360://圣者攻击
		good_effects[1] = ptrSeid->special_effe[0];
		break;
	case 362: //法术必中
		good_effects[32] = 1;
		break;
	case 363: //踏浪人物水上漂
	{
		//good_effects[25] = seid; //地形辅助特效
		good_effects[33] = 1;
	}
		break;
	case 365:// 辅助精神力(整数)每级增加
	{
		int iSeidAddNum = 0;
		if(baseLevel > 0)
		{
			if(baseLevel > ptrSeid->special_effe_num)
			{
				iSeidAddNum += ptrSeid->special_effe[ptrSeid->special_effe_num - 1];
			}
			else
			{
				iSeidAddNum += ptrSeid->special_effe[baseLevel - 1];
			}
		}
		else
		{
			iSeidAddNum += ptrSeid->special_effe[0];
		}
		SetGoodsAddIntellegence(-1, iSeidAddNum);
	}
		break;
	case 366:// 辅助防御力(整数)每级增加
	{
		int iSeidAddNum = 0;
		if(baseLevel > 0)
		{
			if(baseLevel > ptrSeid->special_effe_num)
			{
				iSeidAddNum += ptrSeid->special_effe[ptrSeid->special_effe_num - 1];
			}
			else
			{
				iSeidAddNum += ptrSeid->special_effe[baseLevel - 1];
			}
		}
		else
		{
			iSeidAddNum += ptrSeid->special_effe[0];
		}
		SetGoodsAddDefence(-1, iSeidAddNum);
	}
		break;
	case 367:// 骑马攻击使用dic_jobclass兵种进行克制，并允许一次指定多个。
		good_effects[34] = seid;
		break;
	case 368://一刀秒特技
		good_effects[35] = 1;
		break;
	case 369://无限移动，自动回归
		good_effects[36] = 1;
		break;
	case 370://自动吃武力果
		break;
	case 371://法术必防
		break;
	case 373: //致死攻击
		good_killed_hurt = ptrSeid->special_effe[0];
		break;
	case 374: //攻击必中
		good_attack_must = ptrSeid->special_effe[0];
		break;
	case 375: //死亡标记
		good_weak_attack = ptrSeid->special_effe[0];
		break;
	case 376: //战场复活
		good_battle_relive = ptrSeid->special_effe[0];
		break;
	case 378://隐身，敌人无法直接攻击，但可以溅射攻击
		good_invisible = 1;
		break;
	case 379://穿越移动，可以穿越我军、友军、敌军的任何人员
		good_passtrough_move = 1;
		break;
	case 380://反弹异常攻击
		good_effects[39] = ptrSeid->special_effe[0];
		break;
	case 382: //无极攻击
		good_effects[41] = ptrSeid->special_effe[0];
		break;
	case 384: //唯我独尊
		good_effects[43] = 100;
		break;
	case 386:// 吸MP攻击
		good_suck_mp_attack = ptrSeid->special_effe[0];
		break;
	case 387://攻击时降低对方武器或者装备等级
		break;
	case 388://使命中者降低自身装备经验
		break;
	case 389://攻击范围内所有的人攻击一次
		good_effects[46] = 1;
		break;
	case 390:// 自动使用道具消耗库存（只要被击中）
		good_effects[47] = ptrSeid->special_effe[0];
		break;
	case 391://每回合增加武器和防具经验
		good_effects[48] += ptrSeid->special_effe[0];
		break;
	case 394://改变兵种属性
		good_effects[CHANGE_OCCO_ATTRIBUTE] = ptrSeid->special_effe[0];
		break;
	case 395://濒临死亡
		good_effects[49] = 1;
		break;
	case 396://生身取义
		good_effects[LIFE_RIGHTEOUSNESS] = seid;
		break;
	case 398://移动加双击率
		good_effects[MOVE_ADDTWICE] += ptrSeid->special_effe[0];
		break;
	case 399://疾风攻击
		good_effects[SPEED_ATTACK] += ptrSeid->special_effe[0];;
		break;
	case 400://激斗攻击
		good_effects[FIGHT_ATTACK] += ptrSeid->special_effe[0];
		break;
	case 401://无视能力
		good_effects[LGNORE_ABILITY] += ptrSeid->special_effe[0];
		break;
	case 402://相邻敌人地形移动消耗+N
	{
		if(ptrSeid->special_effe_num > 1)
			good_add_move_consume_range->addElement(new RangeEffect(ptrSeid->special_effe[0], ptrSeid->special_effe[1], 100));
		else
			good_add_move_consume_range->addElement(new RangeEffect(2, ptrSeid->special_effe[0], 100));
	}
		break;
	case 403://固化地形
		good_effects[REDUCE_LAND] = ptrSeid->special_effe[0];
		break;
	case 404://若行动前未移动，行动后可移动
		good_effects[ATTACK_AFTER_MOVE] = 1;
		break;
	case 405://众志成城（九宫格五维提升光环）
	{
		int iAddAbility = ptrSeid->special_effe[0];
		good_up_power_range->addElement(new RangeEffect(2, iAddAbility, 100));
		good_up_intellegence_range->addElement(new RangeEffect(2, iAddAbility, 100));
		good_up_defence_range->addElement(new RangeEffect(2, iAddAbility, 100));
		good_up_explosion_range->addElement(new RangeEffect(2, iAddAbility, 100));
		good_up_fortune_range->addElement(new RangeEffect(2, iAddAbility, 100));
	}
		break;
	case 407://回合初强制改变天气
		good_effects[CHANGE_WEATHER] = ptrSeid->special_effe[0];
		break;
	case 408: //鲜血护盾
		good_effects[BLOOD_SHIELD] = ptrSeid->special_effe[0];
		break;
	case 409: //闪电反射
		good_effects[LIGHT_REFLE] = ptrSeid->special_effe[0];
		break;
	case 410: //免受伤害
		good_effects[VITAL_SPIRIT] = ptrSeid->special_effe[0];
		break;
	case 411://使自己和被攻击者都后退一格
		good_effects[BERA_HAND] = seid;
		break;
	case 412://二阶突刺
		good_effects[TWOORDER_SPIKE] = seid;
		break;
	case 413://使自己和被攻击者都前进一格
		good_effects[NEW_ASSAULT] = ptrSeid->special_effe[0];
		break;
	case 414://劲弩
		good_effects[ARBALESTER] = ptrSeid->special_effe[0];
		break;
	case 415://攻击时部分精神力转化为攻击力
		good_effects[ADD_ATTACK_ELEM] = ptrSeid->special_effe[0];
		break;
	case 416://转移伤害
		good_effects[TRANSFER_HURT] = ptrSeid->special_effe[0];
		break;
	case 417://本方回合面板攻击力上升X%，敌方行动回合防御力增加X%
		good_effects[ADD_ATKORDEF] = ptrSeid->special_effe[0];
		break;
	case 418://减轻不与自身相邻的敌军的攻击伤害X%，减轻近身伤害x/2%
		good_effects[DECREASES_DAMA] = ptrSeid->special_effe[0];
		break;
	case 419://当MP＝０时，消耗等同于MP上限的HP，并且令MP回复１
		good_effects[HP_CHANGE_MP] = ptrSeid->special_effe[0];
		break;
	case 420://范围牵制行动
		good_down_action_range->addElement(new RangeEffect(2, ptrSeid->special_effe[0], 100));
		break;
	case 422://增加双击和暴击率
		good_effects[TWICEHITORDEADLY_RATE] = ptrSeid->special_effe[0];
		break;
	case 423://击中敌人后，触发自身减伤X%，受到伤害后，触发自身增伤X%
		if(good_effects[HIT_INJURY] >= 0)
			good_effects[HIT_INJURY] = ptrSeid->special_effe[0];
		break;
	case 425: //反击几率双击
		good_effects[HITBACK_DOUBLE] = ptrSeid->special_effe[0];
		break;
	case 426: //每回合几率加移动
		good_effects[ROUND_SPEED_UP] = ptrSeid->special_effe[0];
		break;
	case 427: //每回合几率加防御
		good_effects[ROUND_DEF_UP] = ptrSeid->special_effe[0];
		break;
	case 603: //指定装备指定属性提升
	{
		int iItId = ptrSeid->special_effe[0];
		if(weapon.isNormalGood() && weapon.getItId() == iItId || coat.isNormalGood() && coat.getItId() == iItId)
		{
			int iAttrId = ptrSeid->special_effe[1];
			int iAddNum = ptrSeid->special_effe[2];
			switch(iAttrId)
			{
			case 951:
				SetGoodsAddPower(-1, GetCurpower() * iAddNum / 100);
				break;
			case 952:
				SetGoodsAddIntellegence(-1,GetCurIntellegence() * iAddNum / 100);
				break;
			case 953:
				SetGoodsAddDefence(-1,GetCurDefence() * iAddNum / 100);
				break;
			case 954:
				SetGoodsAddExplosion(-1,GetCurExplosion() * iAddNum / 100);
				break;
			case 955:
				SetGoodsAddFortune(-1,GetCurFortune() * iAddNum / 100);
				break;
			case 956:
				SetGoodsAddSpeed(-1,iAddNum);
				break;
			case 959:
			{
				SetGoodsAddPower(-1, GetCurpower() * iAddNum / 100);
				SetGoodsAddIntellegence(-1, GetCurIntellegence() * iAddNum / 100);
				SetGoodsAddDefence(-1, GetCurDefence() * iAddNum / 100);
				SetGoodsAddExplosion(-1, GetCurExplosion() * iAddNum / 100);
				SetGoodsAddFortune(-1, GetCurFortune() * iAddNum / 100);
				SetGoodsAddSpeed(-1, iAddNum);
			}
				break;
			default:
				break;
			}
		}
	}
		break;
	case 604: //智将属性提升
	{
		if (weapon.isNormalGood())
		{
			if(weapon.getItId() == 1) //装备剑
			{
				SetGoodsAddPower(-1, GetCurpower() * ptrSeid->special_effe[0] / 100);
			}
			else if (weapon.getItId() == 30) //装备扇
			{
				SetGoodsAddIntellegence(-1, GetCurIntellegence() * ptrSeid->special_effe[0] / 100);
			}
		}
	}
		break;
	case 605: //警戒，分担伤害
		good_effects[SHARE_DAMAGE] = seid;
		break;
	case 720:
		good_up_powint_range->addElement(new RangeEffect(ptrSeid->special_effe[0], ptrSeid->special_effe[1], 100));
		break;
	case 748:
		good_can_control_tower->addElement(new RangeEffect(ptrSeid->special_effe[0], 1, 100));
		break;
	case 749:
		good_add_tower_ability->addElement(new RangeEffect(ptrSeid->special_effe[0], seid, 100));
		break;
	case 758:
		SetGoodsAddPower(-1, GetCurpower() * ptrSeid->special_effe[0] / 100);
		SetGoodsAddIntellegence(-1, GetCurIntellegence() * ptrSeid->special_effe[0] / 100);
		SetGoodsAddDefence(-1, GetCurDefence() * ptrSeid->special_effe[0] / 100);
		SetGoodsAddExplosion(-1, GetCurExplosion() * ptrSeid->special_effe[0] / 100);
		SetGoodsAddFortune(-1, GetCurFortune() * ptrSeid->special_effe[0] / 100);
		break;
	case 765:
		good_add_land_ability->addElement(new RangeEffect(ptrSeid->special_effe[0], ptrSeid->special_effe[1], 100));
		break;
	case 768:
		SetGoodsAddDefence(-1, GetCurDefence() * ptrSeid->special_effe[0] / 100);
		break;
	case 769:
		SetGoodsAddHp(-1, GetBaseHp() * ptrSeid->special_effe[0] / 100);
		break;
	case 770:
	{
		int iJobType = trData->occu_types[GetActorOccuType()].job_type;
		if (iJobType == 923 || iJobType == 924)
		{
			SetGoodsAddIntellegence(-1, GetCurIntellegence() * ptrSeid->special_effe[0] / 100);
		}
		else
		{
			SetGoodsAddPower(-1, GetCurpower() * ptrSeid->special_effe[0] / 100);
		}
	}
		break;
	case 771:
		good_being_attack_add_mp->addElement(new RangeEffect(ptrSeid->special_effe[1], ptrSeid->special_effe[0], 100));
		break;
	case 800://成长
		addExtraGrowEffect(ptrSeid->special_effe[0], ptrSeid->special_effe[1]);
		break;
	case 1007: //辅助攻击力
		SetGoodsAddPower(-1,ptrSeid->special_effe[0]);
		break;
	case 1008: //辅助精神力
		SetGoodsAddIntellegence(-1,ptrSeid->special_effe[0]);
		break;
	case 1009: //辅助防御力
		SetGoodsAddDefence(-1, ptrSeid->special_effe[0]);
		break;
	case 1010: //辅助爆发力
		SetGoodsAddExplosion(-1, ptrSeid->special_effe[0]);
		break;
	case 1011: //辅助士气力
		SetGoodsAddFortune(-1, ptrSeid->special_effe[0]);
		break;
	default:
		addSpecialEffect(seid);
		break;
	}
	if(good_effects[13])
	{
		good_hit_back_back = 0;
		good_hit_twice = 0;
		good_back_attack_twice = 0;
		good_effects[HITBACK_DOUBLE] = 0;
	}
}

void HistoryActor::AddGoodAttr(HistoryGoods* ptrGoods, int iAddVal)
{
	int iType = ptrGoods->GetGoodsAbilityType();
	if(iType & GOODS_POWER) //攻击力
	{
		SetGoodsAddPower(-1, iAddVal);
	}
	if(iType & GOODS_INTEL) //精神力
	{
		SetGoodsAddIntellegence(-1, iAddVal);
	}
	if(iType & GOODS_DEFENCE) //防御力
	{
		SetGoodsAddDefence(-1, iAddVal);
	}
}
bool HistoryActor::HasEnemyAside(int x,int y)
{
	HistoryActorPtr historyActor;
	historyActor = history->FindActorPosition(x-1,y);
	bool isTrue = (historyActor != NULL && !historyActor->good_invisible);
	if (isTrue)
	{
		if(historyActor->relation == ACTOR_RELATION_ENEMY
			&&relation != ACTOR_RELATION_ENEMY)
			return true;
		if(historyActor->relation != ACTOR_RELATION_ENEMY
			&&relation == ACTOR_RELATION_ENEMY)
			return true;
	}
	historyActor = history->FindActorPosition(x,y-1);
	isTrue = (historyActor != NULL && !historyActor->good_invisible);
	if (isTrue)
	{
		if(historyActor->relation == 2&&relation != 2)
			return true;
		if(historyActor->relation != 2&&relation == 2)
			return true;
	}
	historyActor = history->FindActorPosition(x+1,y);
	isTrue = (historyActor != NULL && !historyActor->good_invisible);
	if (isTrue)
	{
		if(historyActor->relation == 2&&relation != 2)
			return true;
		if(historyActor->relation != 2&&relation == 2)
			return true;
	}
	historyActor = history->FindActorPosition(x,y+1);
	isTrue = (historyActor != NULL && !historyActor->good_invisible);
	if (isTrue)
	{
		if(historyActor->relation == 2&&relation != 2)
			return true;
		if(historyActor->relation != 2&&relation == 2)
			return true;
	}
	return false;
}



int HistoryActor::MapisMoveable(int x,int y)
{

    UUCHAR map_type = history->map_array[y][x];
	int ret = trData->occupation_landmove_info[occupation_id][map_type];
	if (ret == 255 || history->HasElem(x, y, occupation_id))
		return 0;
	return ret;
}
int HistoryActor::GenerateDefaultMove(int sx,int sy,int dx,int dy, int super_move)
{
	if (sx == dx&&sy == dy)
	{
		return 0;
	}
	BattleActor::clearMoveList();
	BattleActor::is_default_move = 1;
	BattleActor::moveList = new DList<MovePath>(true);
	int step = 0;
	int cur_step = 0;
	int ret = 0xffff;
	int direction[4][2];
	int index = 0;
	AStarNodePtr headNode, openNodes, firstNode;
	AStarNodePtr curNode;
	char *tiletag;
	curNode = new AStarNode();
	tiletag = (char*)ChessAlloc(history->map_width*history->map_height);
	memset(tiletag,0,history->map_width*history->map_height);
	curNode->x = sx;
	curNode->y = sy;
	curNode->Parent = NULL;
	curNode->NextNode = NULL;
	curNode->step = 0;
	headNode  = openNodes = curNode;
	/*if(abs(dy-sy)<abs(dx-sx))
	{
		direction[0][1] = 0;
		direction[3][1] = 0;
		direction[1][0] = 0;
		direction[2][0] = 0;
		direction[0][0] = dx>=sx?1:-1;
		direction[3][0] = dx>=sx?-1:1;
		direction[1][1] = dy>=sy?1:-1;
		direction[2][1] = dy>=sy?-1:1;
	}
	else
	{
		direction[0][0] = 0;
		direction[3][0] = 0;
		direction[1][1] = 0;
		direction[2][1] = 0;
		direction[0][1] = dy>=sy?1:-1;
		direction[3][1] = dy>=sy?-1:1;
		direction[1][0] = dx>=sx?1:-1;
		direction[2][0] = dx>=sx?-1:1;
	}*/
	{
		direction[0][0] = 1;
		direction[0][1] = 0; //右
		direction[1][0] = -1;
		direction[1][1] = 0; //左
		direction[2][0] = 0;
		direction[2][1] = -1;//上
		direction[3][0] = 0;
		direction[3][1] = 1; //下
	}
	while(openNodes&&ret==0xffff)
	{
		firstNode = openNodes;
		step = openNodes->step;
		for(index = 0;index<4;index++)
		{
			curNode = new AStarNode();
			curNode->x = firstNode->x+direction[index][0];
			curNode->y = firstNode->y+direction[index][1];
			if(curNode->x<0||curNode->x>=history->map_width||curNode->y<0||curNode->y>=history->map_height)
			{
				SAFE_DELETE(curNode);
				continue;
			}
			if(tiletag[curNode->y * history->map_width + curNode->x])
			{
				SAFE_DELETE(curNode);
				continue;
			}
			tiletag[curNode->y * history->map_width + curNode->x] = 1;
            if(super_move)
                cur_step = 1;
            else
                cur_step = MapisMoveable(curNode->x,curNode->y);
			if(cur_step)
			{
				curNode->step = step + cur_step;
				curNode->Parent = firstNode;
				openNodes = headNode;
				while(openNodes)
				{
					if(openNodes->NextNode == NULL)
					{
						curNode->NextNode = openNodes->NextNode;
						openNodes->NextNode = curNode;
						break;
					}
					else if(openNodes->step<=curNode->step&&openNodes->NextNode->step>curNode->step)
					{
						curNode->NextNode = openNodes->NextNode;
						openNodes->NextNode = curNode;
						break;
					}
					openNodes = openNodes->NextNode;
				}
			}
			else
			{
				SAFE_DELETE(curNode);
				continue;
			}
			if(curNode->x==dx&&curNode->y==dy)
			{
				AStarNodePtr tmpNode = curNode;
				MovePathPtr movePoint = new MovePath(curNode);
				while(tmpNode->Parent)
				{
					DPointPtr tile = new DPoint();
					tile->x = tmpNode->x;
					tile->y = tmpNode->y;
					movePoint->path->insertElementAt(tile,0);
					tmpNode= tmpNode->Parent;
				}
				BattleActor::moveList->addElement(movePoint);
				ret = curNode->step;
				break;
			}
		}
		openNodes = firstNode->NextNode;
	}
	curNode = headNode;
	while(curNode)
	{
		firstNode = curNode->NextNode;
		SAFE_DELETE(curNode);
		curNode = firstNode;
	}
	ChessFree(tiletag);
	return ret;
}
int HistoryActor::inArea(int x1,int y1,int x2,int y2)
{
	if((x-x1)*(x-x2)<=0&&(y-y1)*(y-y2)<=0)
		return 1;
	else
		return 0;
}

bool HistoryActor::isAttackArea(int iX, int iY, bool bIsBack /*= false*/)
{
	bool bIsArea = false;

	if (bIsBack && isHadSpecial(TYPE735)) //TYPE735 无视距离反击
	{
		bIsArea = true;
	}
	else
	{
		int iAttackId = getAttackAreaId();
		if(trData->hitArea[iAttackId].length >= 0)
		{
			for(int i = 0; i < trData->hitArea[iAttackId].length; i++)
			{
				int iCurX = this->x + trData->hitArea[iAttackId].pointlist[i].x;
				int iCurY = this->y + trData->hitArea[iAttackId].pointlist[i].y;
				if(iCurX == iX && iCurY == iY)
				{
					bIsArea = true;
					break;
				}
			}
		}
		else if(trData->hitArea[iAttackId].length == -1)
		{
			bIsArea = true;
		}
	}

	return bIsArea;
}

bool HistoryActor::isSkillBackEnable(int iX, int iY, SkillAttr* ptrSkill)
{
	bool bIsBack = false;
	if(isHadSpecial(TYPE736)) //支持技能反击
	{
		if (ptrSkill && ptrSkill->skill_object == 0)
		{
			int iSkillType = getSpecialInt(TYPE736);
			SeidType* ptrType = trData->m_mSeidType[TYPE736];
			if(iSkillType == ptrType->iClass || iSkillType == ptrSkill->type)
			{
				bIsBack = isAttackArea(iX, iY, true);
			}
		}
	}
	return bIsBack;
}

int HistoryActor::RangeOfNumbers()
{
	HistoryActorPtr temp_oppo_actor = NULL;
	int actor_num = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp_oppo_actor = history->FindActorPosition(x + i - 1, y + j - 1);
			if (temp_oppo_actor != NULL)
			{
				if (0 == i - 1 && 0 == j - 1)
				{
					continue;
				}
				actor_num++;
			}
		}
	}
	return actor_num;
}

int HistoryActor::inAttackArea_PK(int x, int y)
{
	int max_x = 0, min_x = 0, max_y = 0, min_y = 0, w_i, w_j, w_z;
	int attack_id = getAttackAreaId();
	if (trData->hitArea[attack_id].length >= 0)
	{
		for (w_i = 0; w_i < trData->hitArea[attack_id].length; w_i++){
			if (trData->hitArea[attack_id].pointlist[w_i].x>=max_x)
			{
				max_x = trData->hitArea[attack_id].pointlist[w_i].x;
			}
			if (trData->hitArea[attack_id].pointlist[w_i].x < min_x)
			{
				min_x = trData->hitArea[attack_id].pointlist[w_i].x;
			}
			if (trData->hitArea[attack_id].pointlist[w_i].y >= max_y)
			{
				max_y = trData->hitArea[attack_id].pointlist[w_i].y;
			}
			if (trData->hitArea[attack_id].pointlist[w_i].y < min_y)
			{
				min_y = trData->hitArea[attack_id].pointlist[w_i].y;
			}
		
		}
		for (w_j = min_x; w_j <= max_x; w_j++){
			for (w_z = min_y; w_z <= max_y; w_z++){
				int cur_x = this->x + w_j;
				int cur_y = this->y + w_z;
				if (cur_x == x && cur_y == y)
				{
					return 1;
				}
			}
		}
	}
	else if(trData->hitArea[attack_id].length == -1)
	{
		return 1;
	}
	return 0;
}

int  HistoryActor::getAttackAreaId()
{
	int attack_type;
	OccupationAttrPtr curOccu = &trData->occupations[occupation_id];
	attack_type = curOccu->attack_type_id;
	if(good_hit_distance)
	{
		attack_type = good_hit_distance - 1;
	}
	return attack_type;
}

void HistoryActor::recoverState()
{
	m_iControllerId = 0;
	state_power = 0;
	state_intellegence = 0;
	state_defence = 0;
	state_explosion = 0;
	state_fortune = 0;
	state_puzzle = 0;
	state_poison = 0;
	state_palsy = 0;
	state_stun = 0;
	state_speed = 0;
	state_blind = 0;
	state_obfuscation = 0;
    state_weaked = 0;
	state_skillhurt = 0;
	state_adddef_redpow = 0;
	state_loseblood = 0;
	state_addhp = 0;
	state_sual_link = 0;
	state_reduceSkill = 0;
	state_reduceHurt = 0;
	state_emergency_command = 0;

	action_finished = 0;
	hide = 0;
	SetCurHp(GetBaseHp() + GetGoodsAddHp());
	SetCurMp(GetBaseMp() + GetGoodsAddMp());
	is_leave = 0;
	weaked_num = 0;
	is_used_relive = 0;
	bust_style = 0;
	iTwiceAction = 0;
	iActionNum = -1;
	buff_hate_attack = 0;
	border_on_death = 0;
	actor_only_move = 0;
	good_pursuit_attack_num = 0;
	good_skill_pursuit_attack_num = 0;
	pursuit_skill_lv = 0;
	blood_shield = 0;
	vital_spirit = 0;
	hp_mp_defense_number = 0;
	actor_fit_id.resize(0);
	state_reduce_skill_hurt_range = 0;
	if (reduce_skill_hurt_buff)
		reduce_skill_hurt_buff->removeAllElements();
	state_SkillHurtAndRebound = 0;
	state_add_skill_hurt_range = 0;
	if (add_skill_hurt_buff)
		add_skill_hurt_buff->removeAllElements();
	good_shield_range->removeAllElements();
	mElitePlus.clear();
	m_iShareTimes = 0;
	m_iRestSpeed = 0;

	if (m_iExternalEquip)
	{
		m_iExternalEquip = 0;
		ClearSelfSkillLevel();
	}
	mSkillUseNumLimt.clear();
	mSeidAddGain.clear();
	clearActorBuff();
	clearCopySkill();
	m_mScriptValue.clear();
	m_mScriptPercent.clear();
	m_mPVPLandSeid.clear();

}

DList<HistoryActor::RangeEffect> * HistoryActor::getRangeEffectListByType(int type, bool up) {
    switch(type){
        case ACTOR_ABILITY_POWER:
            return up?good_up_power_range:good_down_power_range;
        case ACTOR_ABILITY_INTELLEGENCE:
            return up?good_up_intellegence_range:good_down_intellegence_range;
        case ACTOR_ABILITY_DEFENCE:
            return up?good_up_defence_range:good_down_defence_range;
        case ACTOR_ABILITY_EXPLOSION:
            return up?good_up_explosion_range:good_down_explosion_range;
        case ACTOR_ABILITY_FORTUNE:
            return up?good_up_fortune_range:good_down_fortune_range;
        case ACTOR_ABILITY_SPEED:
            return up?good_up_speed_range:good_down_speed_range;
        case 931:
            return up?good_up_blind_range:good_down_blind_range;
        case 932:
            return up?good_up_puzzle_range:good_down_puzzle_range;
        case 933:
            return up?good_up_poison_range:good_down_poison_range;
        case 934:
            return up?good_up_palsy_range:good_down_palsy_range;
        case 935:
            return up?good_up_stun_range:good_down_stun_range;
        case 936:
            return up?good_up_obfuscation_range:good_down_obfuscation_range;
        case 957:
            return good_up_hp_range;
        case 958:
            return good_up_mp_range;
		case 959:
			return good_add_move_consume_range;
		case 960:
			return good_down_action_range;
		case RANGE_BUFF_REDUCE_SKILLHURT:
			return reduce_skill_hurt_buff;
		case RANGE_BUFF_ADD_SKILLHURT:
			return add_skill_hurt_buff;
		case RANGE_BUFF_ADD_POWINT:
			return up ? good_up_powint_range : nullptr;
		case ACTOR_BUFF_SHIELD_RANGE:
			return good_shield_range;
		case RANGE_CONTROL_TOWER:
			return good_can_control_tower;
		case RANGE_ADD_TOWER_ABILITY:
			return good_add_tower_ability;
		case RANGE_ADD_LAND_ABILITY:
			return good_add_land_ability;
		case RANGE_BE_ATTACK_ADD_MP:
			return good_being_attack_add_mp;
        default:
            return NULL;
    }
}
int HistoryActor::getTotalRangeEffectByList(DList<RangeEffect> * list, int dst_x, int dst_y){
    int result = 0;
    for(int i=0; i<list->size(); i++) {
        RangeEffect * effect =  list->elementAt(i);
        if(trData->canRangeEffect(effect->area - 1, x, y, dst_x, dst_y)) {
            if(rand()%100 < effect->probability)
                result += effect->value;
        }
    }
    return result;
}
int HistoryActor::getTotalRangeEffect(int type, bool up, int dst_x, int dst_y) {
    if(this->hide)
        return 0;
    DList<RangeEffect> * list = getRangeEffectListByType(type, up);
    if(list != NULL) {
        return getTotalRangeEffectByList(list, dst_x, dst_y);
    } else {
        return 0;
    }
}
void HistoryActor::accumulateTotalStateRangeEffects(StateRangeEffects *state, bool up, int dst_x, int dst_y) {
    state->blind += getTotalRangeEffect(931, up, dst_x, dst_y);
    state->puzzle += getTotalRangeEffect(932, up, dst_x, dst_y);
    state->poison += getTotalRangeEffect(933, up, dst_x, dst_y);
    state->palsy += getTotalRangeEffect(934, up, dst_x, dst_y);
    state->stun += getTotalRangeEffect(935, up, dst_x, dst_y);
    state->obfuscation += getTotalRangeEffect(936, up, dst_x, dst_y);

}
int HistoryActor::addRealAbility(int abilityCase,int ability)
{
    int basePower = 0;
    for (int i = 0; i < history->battleEnemyNum; i++) {
        basePower += history->battleEnemy[i]->getTotalRangeEffect(abilityCase, relation == ACTOR_RELATION_ENEMY, x, y);
    }
    for (int i = 0; i < history->battleOwnNum; i++) {
        basePower += history->battleOwn[i]->getTotalRangeEffect(abilityCase, relation != ACTOR_RELATION_ENEMY, x, y);
    }
    for (int i = 0; i < history->battleFriendNum; i++) {
        basePower += history->battleFriend[i]->getTotalRangeEffect(abilityCase, relation != ACTOR_RELATION_ENEMY, x, y);
    }
    /* 最终能力，在 10% 到 1000% 之间 */
    if(basePower < -90)
        basePower = -90;
    if(basePower > 900)
        basePower = 900;
    return ability * basePower / 100;
}
int HistoryActor::getBasicsMAXAbility()
{
	int realAbility = 0;
	int ret = 0;
	if(GetCurDefence() > 60000) //武将能力未进行初始化或者存档数据有误
	{
		InitAbility();
	}

	for (int i = 0; i<5; i++)
	{
		if (0 == i)
		{
			{
				realAbility = GetCurpower() + GetGoodsAddPower();
			}
			if (realAbility > ret)
				ret = realAbility;
		}
		else if (1 == i)
		{
			{
				realAbility = GetCurDefence() + GetGoodsAddDefence();
			}
			if (realAbility > ret)
				ret = realAbility;
		}
		else if (2 == i)
		{
			{
				realAbility = GetCurIntellegence() + GetGoodsAddIntellegence();
			}
			if (realAbility > ret)
				ret = realAbility;
		}
		else if (3 == i)
		{
			{
				realAbility = GetCurExplosion() + GetGoodsAddExplosion();
			}
			if (realAbility > ret)
				ret = realAbility;
		}
		else if (4 == i)
		{
			{
				realAbility = GetCurFortune() + GetGoodsAddFortune();
			}
			if (realAbility > ret)
				ret = realAbility;
		}
	}
	return ret;
}
int HistoryActor::getRealAbility(int abilityCase, bool includeState, int real_x, int real_y,bool enable_fits)
{
	int realAbility = 0;
	if(GetCurDefence() > 60000) //武将能力未进行初始化或者存档数据有误
	{
		InitAbility();
	}

	switch (abilityCase)
	{
	case ACTOR_ABILITY_POWER:
		{
			realAbility = GetCurpower() + GetGoodsAddPower();
		}
		{
			int extra_realAbility = 0;
			if (good_effects[ADD_ATKORDEF] && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY &&history->curActionSide != ACTOR_RELATION_ENEMY)
				|| (relation == ACTOR_RELATION_ENEMY&&history->curActionSide == ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += good_effects[ADD_ATKORDEF] * realAbility / 100;
			}

			if (isHadSpecial(TYPE755) && mSeidAddGain[TYPE755])
			{
				extra_realAbility += getSpecialInt(TYPE755) * GetActorLv() * mSeidAddGain[TYPE755] / 100;
			}

			if (isHadSpecial(TYPE761) && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY &&history->curActionSide != ACTOR_RELATION_ENEMY)
					|| (relation == ACTOR_RELATION_ENEMY&&history->curActionSide == ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += getSpecialInt(TYPE761) * realAbility / 100;
			}

			if (isHadSpecial(TYPE763))
			{
				extra_realAbility += getSpecialMap(TYPE763)[1] * realAbility / 100;
			}
			if(controlled_by_enemy)
				extra_realAbility += controlled_by_enemy_addAbi * realAbility / 100;

			realAbility += extra_realAbility;
		}
		break;
	case ACTOR_ABILITY_DEFENCE:
		{
			realAbility = GetCurDefence() + GetGoodsAddDefence();
		}
		{
			int extra_realAbility = 0;
			if (good_effects[ADD_ATKORDEF] && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY&& history->curActionSide == ACTOR_RELATION_ENEMY)
				|| (relation == ACTOR_RELATION_ENEMY && history->curActionSide  != ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += good_effects[ADD_ATKORDEF] * realAbility / 100;
			}
			if (isHadSpecial(TYPE759))
			{
				extra_realAbility += getSpecialInt(TYPE759)*(GetCurIntellegence() + GetGoodsAddIntellegence()) / 100;
			}
			if (isHadSpecial(TYPE760) && mSeidAddGain[TYPE760])
			{
				extra_realAbility += getSpecialInt(TYPE760) * mSeidAddGain[TYPE760] * realAbility / 1000;
			}
			if (isHadSpecial(TYPE761) && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY &&history->curActionSide == ACTOR_RELATION_ENEMY)
					|| (relation == ACTOR_RELATION_ENEMY&&history->curActionSide != ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += getSpecialInt(TYPE761) * realAbility / 100;
			}


			realAbility += extra_realAbility;
		}
		break;
	case ACTOR_ABILITY_INTELLEGENCE:
		{
			realAbility = GetCurIntellegence() + GetGoodsAddIntellegence();

			int extra_realAbility = 0;
			if (isHadSpecial(TYPE761) && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY &&history->curActionSide != ACTOR_RELATION_ENEMY)
					|| (relation == ACTOR_RELATION_ENEMY&&history->curActionSide == ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += getSpecialInt(TYPE761) * realAbility / 100;
			}
			if (controlled_by_enemy)
				extra_realAbility += controlled_by_enemy_addAbi * realAbility / 100;

			realAbility += extra_realAbility;
		}
		break;
	case ACTOR_ABILITY_EXPLOSION:
		{
			realAbility = GetCurExplosion() + GetGoodsAddExplosion();

			int extra_realAbility = 0;
			if (isHadSpecial(TYPE761) && history->game_state == GAME_STATE_BATTLE &&
				((relation != ACTOR_RELATION_ENEMY &&history->curActionSide == ACTOR_RELATION_ENEMY)
					|| (relation == ACTOR_RELATION_ENEMY&&history->curActionSide != ACTOR_RELATION_ENEMY)))
			{
				extra_realAbility += getSpecialInt(TYPE761) * realAbility / 100;
			}
			realAbility += extra_realAbility;
		}
		break;
	case ACTOR_ABILITY_FORTUNE:
		{
			realAbility = GetCurFortune() + GetGoodsAddFortune();
		}
		break;
	default:
		break;
	}

	if (good_effects[43])
		realAbility = getBasicsMAXAbility();

	if (includeState){

		switch (abilityCase)
		{
		case ACTOR_ABILITY_POWER:
			realAbility = (100 + getFiveAbilityBuffEffect(state_power, ACTOR_BUFF_POWUP))* realAbility / 100;
			break;
		case ACTOR_ABILITY_INTELLEGENCE:
			realAbility = (100 + getFiveAbilityBuffEffect(state_intellegence, ACTOR_BUFF_INTUP))* realAbility / 100;
			break;
		case ACTOR_ABILITY_DEFENCE:
			realAbility = (100 + getFiveAbilityBuffEffect(state_defence, ACTOR_BUFF_DEFUP))* realAbility / 100;
			break;
		case ACTOR_ABILITY_EXPLOSION:
			realAbility = (100 + getFiveAbilityBuffEffect(state_explosion, ACTOR_BUFF_EXPUP))* realAbility / 100;
			break;
		case ACTOR_ABILITY_FORTUNE:
			realAbility = (100 + getFiveAbilityBuffEffect(state_fortune, ACTOR_BUFF_FORTUP))* realAbility / 100;
			break;
		default:
			break;
		}
	}

	if ((history->game_state == GAME_STATE_BATTLE || history->game_state == GAME_STATE_BATTLE_END)&& isHadSpecial(TYPE309) && SeparateState())//单独增益
	{
		map<int, int> mSpecial = getSpecialMap(TYPE309);
		for(UInt i = 0; i < mSpecial.size(); i += trData->m_mSeidType[TYPE309]->iEffNum)
		{
			switch(mSpecial[i])
			{
			case 951:
				if(abilityCase == ACTOR_ABILITY_POWER)
					realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			case 952:
				if(abilityCase == ACTOR_ABILITY_INTELLEGENCE)
					realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			case 953:
				if(abilityCase == ACTOR_ABILITY_DEFENCE)
					realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			case 954:
				if(abilityCase == ACTOR_ABILITY_EXPLOSION)
					realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			case 955:
				if(abilityCase == ACTOR_ABILITY_FORTUNE)
					realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			case 959:
				realAbility = (100 + mSpecial[i + 1])*realAbility / 100;
				break;
			default:
				break;
			}
		}
	}

	//TYPE740 激战，增加相邻部队数Y%的X属性
	if((history->game_state == GAME_STATE_BATTLE || history->game_state == GAME_STATE_BATTLE_END) && isHadSpecial(TYPE740))
	{
		int iAround = RangeOfNumbers();
		if (iAround)
		{
			map<int, int> mSpecial = getSpecialMap(TYPE740);
			for(UInt i = 0; i < mSpecial.size(); i += trData->m_mSeidType[TYPE740]->iEffNum)
			{
				bool isMatch = false;
				switch(mSpecial[i])
				{
				case 951:
					isMatch = abilityCase == ACTOR_ABILITY_POWER;
					break;
				case 952:
					isMatch = abilityCase == ACTOR_ABILITY_INTELLEGENCE;
					break;
				case 953:
					isMatch = abilityCase == ACTOR_ABILITY_DEFENCE;
					break;
				case 954:
					isMatch = abilityCase == ACTOR_ABILITY_EXPLOSION;
					break;
				case 955:
					isMatch = abilityCase == ACTOR_ABILITY_FORTUNE;
					break;
				case 959:
					isMatch = true;
					break;
				default:
					break;
				}
				if (isMatch)
				{
					realAbility = (100 + mSpecial[i + 1] * iAround) * realAbility / 100;
				}
			}
		}
	}

	int iCurHP = GetCurHp();
	if ((history->game_state == GAME_STATE_BATTLE || history->game_state == GAME_STATE_BATTLE_END) && isHadSpecial(TYPE310) && iCurHP > 0 && iCurHP < (GetBaseHp() + GetGoodsAddHp())) //损血增益
	{
		map<int, int> mSpecial = getSpecialMap(TYPE310);
		for(UInt i = 0; i < mSpecial.size(); i += trData->m_mSeidType[TYPE310]->iEffNum)
		{
			int iBaseHp = GetBaseHp();
			int iGoodsAddHp = GetGoodsAddHp();
			switch(mSpecial[i])
			{
			case 951:
				if(abilityCase == ACTOR_ABILITY_POWER)
					realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			case 952:
				if(abilityCase == ACTOR_ABILITY_INTELLEGENCE)
					realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			case 953:
				if(abilityCase == ACTOR_ABILITY_DEFENCE)
					realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			case 954:
				if(abilityCase == ACTOR_ABILITY_EXPLOSION)
					realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			case 955:
				if(abilityCase == ACTOR_ABILITY_FORTUNE)
					realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			case 959:
				realAbility = (100 + (iBaseHp + iGoodsAddHp - iCurHP) * 100 / (iBaseHp + iGoodsAddHp)*mSpecial[i + 1] / 100)*realAbility / 100;
				break;
			default:
				break;
			}
		}
	}
	if(history->game_state == GAME_STATE_BATTLE)
	{
		realAbility += addRealAbility(abilityCase, realAbility);//添加范围光环效果

		int iAddAbility = addRealAbility(RANGE_BUFF_ADD_POWINT, realAbility);
		int iJobType = getActorJobType();
		switch(abilityCase)
		{
		case ACTOR_ABILITY_POWER:
			if (iJobType == 921 || iJobType == 922 || iJobType == 924)
			{
				realAbility += iAddAbility;
			}
			break;
		case ACTOR_ABILITY_INTELLEGENCE:
			if(iJobType == 923 || iJobType == 924)
			{
				realAbility += iAddAbility;
			}
			break;
		default:
			break;
		}

		if (trData->occupations[occupation_id].is_robot == 1)
		{
			realAbility += GetRange965FiveAbilityEffect(abilityCase,realAbility);
		}
	}
	switch (abilityCase)
	{
	case ACTOR_ABILITY_POWER:
		if (history->game_state == GAME_STATE_BATTLE&&enable_fits)
		{
			int iFitsEffect = GetDoubleSeid(1, true, real_x, real_y);
			if (iFitsEffect > -1)
			{
				int OppoPow = history->actors[iFitsEffect - 1].getRealAbility(ACTOR_ABILITY_POWER, false, -1, -1, false);
				if (OppoPow > realAbility)
					realAbility = OppoPow;
			}

			if (state_adddef_redpow)
			{
				int iPro = m_mActorBuff[ACTOR_BUFF_ADDDEFREDPOW]->iPercentNum ? m_mActorBuff[ACTOR_BUFF_ADDDEFREDPOW]->iPercentNum : 2;
				realAbility -= realAbility*m_mActorBuff[ACTOR_BUFF_ADDDEFREDPOW]->iPercent / iPro / 100;
			}
		}
		break;
	case ACTOR_ABILITY_DEFENCE:
		if (history->game_state == GAME_STATE_BATTLE&&enable_fits)
		{
			int iFitsEffect = GetDoubleSeid(1, true, real_x, real_y);
			if (iFitsEffect > -1)
			{
				int OppoPow = history->actors[iFitsEffect - 1].getRealAbility(ACTOR_ABILITY_DEFENCE, false, -1, -1, false);
				if (OppoPow > realAbility)
					realAbility = OppoPow;
			}
			if (state_adddef_redpow)
			{
				realAbility += realAbility*m_mActorBuff[ACTOR_BUFF_ADDDEFREDPOW]->iPercent / 100;
			}
		}
		break;
	}
	return realAbility;
}

int HistoryActor::getMinAbilityId()
{
	int iAbilityId = ACTOR_ABILITY_POWER;
	int iMinAbility = getRealAbility(iAbilityId);
	for (int i = 1; i < 5; i++)
	{
		int iAbility = getRealAbility(i);
		if (iAbility < iMinAbility)
		{
			iMinAbility = iAbility;
			iAbilityId = i;
		}
	}
	return iAbilityId;
}

int HistoryActor::SeparateState(int iAreaId)
{
	HistoryActorPtr temp_oppo_actor = NULL;
	if (iAreaId < 0)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				temp_oppo_actor = history->FindActorPosition(x + i - 1, y + j - 1);
				if (temp_oppo_actor != NULL)
				{
					if (0 == i - 1 && 0 == j - 1)
					{
						continue;
					}
					if ((relation == 2) && (temp_oppo_actor->relation == 2))
					{
						return 0;
					}
					else if ((relation == 0 || relation == 1) && (temp_oppo_actor->relation == 0 || temp_oppo_actor->relation == 1))
					{
						return 0;
					}

				}
			}
		}
	}
	else
	{
		if (trData->hitArea[iAreaId - 1].length < 0)
		{
			return 0;
		}

		int iPointX = -1, iPointY = -1;
		for (int i = 0; i < trData->hitArea[iAreaId - 1].length; i++)
		{
			iPointX = x + trData->hitArea[iAreaId - 1].pointlist[i].x;
			iPointY = y + trData->hitArea[iAreaId - 1].pointlist[i].y;

			if(iPointX == x&&iPointY == y) continue;

			temp_oppo_actor = history->FindActorPosition(iPointX, iPointY);
			if (temp_oppo_actor != NULL)
			{
				if ((relation == 2) && (temp_oppo_actor->relation == 2))
				{
					return 0;
				}
				else if ((relation == 0 || relation == 1) && (temp_oppo_actor->relation == 0 || temp_oppo_actor->relation == 1))
				{
					return 0;
				}
			}
		}
	}

	return 1;
}
HistoryActorPtr HistoryActor::RangeOfEnemy(unsigned short actor_id)
{
	HistoryActorPtr actor_enemy = NULL;
	HistoryActorPtr oppo_actor = NULL;
	int x1, y1;
	//九宫格的攻击范围
	for (int i = 0; i<trData->hitArea[1].length; i++)
	{
		x1 = x + trData->hitArea[1].pointlist[i].x;
		y1 = y + trData->hitArea[1].pointlist[i].y;
		oppo_actor = history->FindActorPosition(x1, y1);
		if (oppo_actor != NULL&&oppo_actor->actor_id != actor_id && oppo_actor->GetCurHp()>0 && oppo_actor->action_finished == 0 &&(trData->occu_types[oppo_actor->GetActorOccuType()].job_type == 921 
			|| trData->occu_types[oppo_actor->GetActorOccuType()].job_type == 924)&& !oppo_actor->actor_only_move&& 
			(((relation == 0 || relation == 1) && oppo_actor->relation == 2) || (relation == 2 && (oppo_actor->relation == 0 || oppo_actor->relation == 1))) 
			&& oppo_actor->isAttackArea(this->x, this->y) &&!oppo_actor->state_puzzle){
			actor_enemy = oppo_actor;
		}
		if (actor_enemy)
		{
			break;
		}
	}
	return actor_enemy;
}
int HistoryActor::GetActorOccuType()
{
	int iOccupationId = 0;
	for(int i = 0; i < trData->occupation_type_num; i++)
	{
		if(occupation_id >= trData->occu_types[i].occupation_start && occupation_id < trData->occu_types[i].occupation_start + trData->occu_types[i].cval)
		{
			iOccupationId = i;
			break;
		}
	}
	return iOccupationId;
}

int HistoryActor::GetActorInitialJob()
{
	return trData->occu_types[GetActorOccuType()].occupation_start;
}

int HistoryActor::getActorJobType()
{
	return trData->occu_types[GetActorOccuType()].job_type;
}

void HistoryActor::GetSuitEffect()
{
	map<int,int>mSuitData = GetNetSuitData();
	NetSuitDataPtr ptrNetSuitData = NULL;

	for (map<int, int>::iterator mIt = mSuitData.begin(); mIt != mSuitData.end(); mIt++)
	{
		ptrNetSuitData = trData->net_suit_data[mIt->first];

		for (map<int, int>::iterator iIt = ptrNetSuitData->seid1.begin(); iIt != ptrNetSuitData->seid1.end(); iIt++)
		{
			if (iIt->first)
			{
				AddSpecialEffect(iIt->first);
			}
		}

		if (ptrNetSuitData->hid.count(actor_id + 1))
		{
			for (map<int, int>::iterator iIt = ptrNetSuitData->seid2.begin(); iIt != ptrNetSuitData->seid2.end(); iIt++)
			{
				if (iIt->first)
				{
					AddSpecialEffect(iIt->first);
				}
			}
		}
	}
}

void HistoryActor::delete_controlled_by_enemy()
{
	for (int j = 0; j < history->battleOwnNum; j++)
	{
		if (actor_id == history->battleOwn[j]->actor_id)
		{
			history->battleEnemy[history->battleEnemyNum] = history->battleOwn[j];
			history->battleEnemyNum++;
			for (int k = j; k < history->battleOwnNum - 1; k++)
			{
				history->battleOwn[k] = history->battleOwn[k + 1];
			}
			history->battleOwn[history->battleOwnNum - 1] = NULL;
			history->battleOwnNum--;
			break;
		}
	}
	controlled_by_enemy = 0;
	controlled_by_enemy_round = 0;
	controlled_by_enemy_addAbi = 0;
	relation = 2;
}

int HistoryActor::getEnemyRangeEffect(int range_type, bool up)
{
	int temp_cur_occu_land = 0;
	if (history->game_state != GAME_STATE_BATTLE)
		return temp_cur_occu_land;
	if (relation == ACTOR_RELATION_ENEMY) {//当前人物阵营 我 友 方
		for (int i = 0; i < history->battleOwnNum; i++) {
			int temp_up = history->battleOwn[i]->getTotalRangeEffect(range_type, up, x, y);
			if (temp_up>temp_cur_occu_land)
				temp_cur_occu_land = temp_up;
		}
		for (int i = 0; i < history->battleFriendNum; i++) {
			int temp_up = history->battleFriend[i]->getTotalRangeEffect(range_type, up, x, y);
			if (temp_up>temp_cur_occu_land)
				temp_cur_occu_land = temp_up;
		}
	}
	else { //敌方
		for (int i = 0; i < history->battleEnemyNum; i++) {
			int temp_up = history->battleEnemy[i]->getTotalRangeEffect(range_type, up, x, y);
			if (temp_up>temp_cur_occu_land)
				temp_cur_occu_land = temp_up;
		}
	}
	return temp_cur_occu_land;
}

void HistoryActor::InitAbilityForRevive()
{
	if(history->var2[5010] && relation != ACTOR_RELATION_OWN)
	{
		history->CalcBaseLevel();
		SetActorLv(history->base_level + history->level_add);
		if(GetActorLv()<1)
			SetActorLv(1);
		InitAbility();
		SetDefaultGood(GOOD_TYPE_ALL);
		fashion.good_id = -1;
		wap_fashion.good_id = -1;
		UpdateSkills();
		history->LoadBattleActorImage(actor_id);
	}
}

int HistoryActor::setGoodAddLevel()
{
	int iRet = 0;
	if (weapon.isNormalGood())
	{
		map<int, int> mSeid = weapon.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			if(trData->m_mSeid[mSeid[i]]->type == 406)
			{
				iRet = mSeid[i];
			}
		}
	}
	if (!iRet&&coat.isNormalGood())
	{
		map<int, int> mSeid = coat.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			if(trData->m_mSeid[mSeid[i]]->type == 406)
			{
				iRet = mSeid[i];
			}
		}
	}
	if (!iRet&&assist_good.isNormalGood())
	{
		map<int, int> mSeid = assist_good.getSeid();
		for(UInt i = 0; i < mSeid.size(); i++)
		{
			if(trData->m_mSeid[mSeid[i]]->type == 406)
			{
				iRet = mSeid[i];
			}
		}
	}
	return iRet;
}
HistoryActorPtr HistoryActor::RangeOfHPMinEnemy(int actorId)
{
	HistoryActorPtr actor_hp_min = NULL;
	int attack_id = getAttackAreaId();
	HistoryActorPtr oppo_actor = NULL;
	int x1, y1;
	if (trData->hitArea[attack_id].length >= 0)
	{
		for (int i = 0; i<trData->hitArea[attack_id].length; i++)
		{
			x1 = x + trData->hitArea[attack_id].pointlist[i].x;
			y1 = y + trData->hitArea[attack_id].pointlist[i].y;
			oppo_actor = history->FindActorPosition(x1, y1);
			if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0 && oppo_actor->actor_id != actorId
				&& (((relation == 0 || relation == 1) && oppo_actor->relation == 2) || (relation == 2 && (oppo_actor->relation == 0 || oppo_actor->relation == 1)))){
				if (actor_hp_min == NULL)
					actor_hp_min = oppo_actor;
				else if (oppo_actor->GetCurHp() < actor_hp_min->GetCurHp())
					actor_hp_min = oppo_actor;
			}
		}
	}
	return actor_hp_min;
}
char* HistoryActor::getActorSImgPath(const char* suffix, int iRelation)
{
	int actor_relation = relation;
	if (iRelation>-1)
		actor_relation = iRelation;
	int index = history_image_fight_id;
	int occu_level = occupation_id - history_occupation_id;
	int image_index = 0,img_offset = 0;
	if (history_image_fight_id <= 100)
	{
		if (image_change_occu_id<0)
		{
			if (controlled_by_enemy)
				image_index = 2;
			else
				image_index = actor_relation;
		}
		else if (image_change_occu_id>0 && image_change_occu_id <= 7)
		{
			image_index = image_change_occu_id - 1;
		}
	}

	if (index < 400)
	{
		img_offset = image_index;
	}
	else
	{
		if (occu_level <= trData->actor_turn[actor_id].turn - 1)
			img_offset = occu_level;
		else
			img_offset = trData->actor_turn[actor_id].turn - 1;
	}
	char* ptrPath = NULL;
	for (int j = 1 + img_offset; j > 0; j--)
	{
        ptrPath = ChessSprintf("img/UnitS/%d_%d_%s.png", index, j, suffix);
		unsigned long fileLen = 0;
		char* game_data = ChessReadFile(ptrPath, 0, 0, &fileLen);
		if (game_data != NULL)
		{
			ChessFree(game_data);
			break;
		}
	}
	return ptrPath;
}
void HistoryActor::setStrAddGoodsEffect()
{
	int weaAddAbility[6] = { 0 };
	int coatAddAbility[6] = { 0 };
	if (weapon.isNormalGood() && weapon.iQuality>0)
	{
		int* ptrA = weapon.getStrAddAbility();
		for (int i = 0; i < 6; i++)
			weaAddAbility[i] = ptrA[i];
		ptrA = weapon.getStrengEffectIndex();
		for (int j = 0; j<10; j++)
		{
			if (ptrA[j]>0)
				AddSpecialEffect(ptrA[j] - 1, 1, true);
			else
				break;
		}
		GoodsItem* ptrGoods = trData->m_mGoodsItem[weapon.good_id];
		StrengthenConsumePtr strCon = &trData->strengthen_consume[weapon.iQuality - 1];
		int value = ptrGoods->iPower;
		int strValue = weapon.getStarAddVal();
		AddGoodAttr(&weapon, strValue);
	}
	if (coat.isNormalGood() && coat.iQuality>0)
	{
		int* ptrA = coat.getStrAddAbility();
		for (int i = 0; i < 6; i++)
			coatAddAbility[i] = ptrA[i];
		ptrA = coat.getStrengEffectIndex();
		for (int j = 0; j<10; j++)
		{
			if (ptrA[j]>0)
				AddSpecialEffect(ptrA[j] - 1, 1, true);
			else
				break;
		}
		GoodsItem* ptrGoods = trData->m_mGoodsItem[coat.good_id];
		StrengthenConsumePtr strCon = &trData->strengthen_consume[coat.iQuality - 1];
		int value = ptrGoods->iPower;
		int strValue = coat.getStarAddVal();
		SetGoodsAddDefence(-1, strValue);
	}
	int iA = (weaAddAbility[A_POWER] + coatAddAbility[A_POWER] - stradd_power);
	if (iA)
	{
		SetBasepower(-1, iA);
		stradd_power = weaAddAbility[A_POWER] + coatAddAbility[A_POWER];
		InitoneBaseAbility(ACTOR_ABILITY_POWER);
	}

	int iB = (weaAddAbility[A_DEFENCE] + coatAddAbility[A_DEFENCE] - stradd_defence);
	if (iB)
	{
		SetBaseDefence(-1, iB);
		stradd_defence = weaAddAbility[A_DEFENCE] + coatAddAbility[A_DEFENCE];
		InitoneBaseAbility(ACTOR_ABILITY_DEFENCE);
	}
	int iC = (weaAddAbility[A_INTELLEGENCE] + coatAddAbility[A_INTELLEGENCE] - stradd_intellegence);
	if (iC)
	{
		SetBaseIntellegence(-1, iC);
		stradd_intellegence = weaAddAbility[A_INTELLEGENCE] + coatAddAbility[A_INTELLEGENCE];
		InitoneBaseAbility(ACTOR_ABILITY_INTELLEGENCE);
	}
	int iD = (weaAddAbility[A_EXPLOSION] + coatAddAbility[A_EXPLOSION] - stradd_explosion);
	if (iD)
	{
		SetBaseExplosion(-1, iD);
		stradd_explosion = weaAddAbility[A_EXPLOSION] + coatAddAbility[A_EXPLOSION];
		InitoneBaseAbility(ACTOR_ABILITY_EXPLOSION);
	}
	int iE = (weaAddAbility[A_FORTURN] + coatAddAbility[A_FORTURN] - stradd_fortune);
	if (iE)
	{
		SetBaseFortune(-1, iE);
		stradd_fortune = weaAddAbility[A_FORTURN] + coatAddAbility[A_FORTURN];
		InitoneBaseAbility(ACTOR_ABILITY_FORTUNE);
	}
	SetGoodsAddSpeed(-1, weaAddAbility[A_SPEED] + coatAddAbility[A_SPEED]);
}

void HistoryActor::SetFashionAddAbility()
{
	for (int i = ACTOR_ABILITY_POWER; i < ACTOR_ABILITY_MPUP + 1; i++)
	{
		if (i != ACTOR_ABILITY_SPEED) //屏蔽移动力
		{
			if (mFashionAddAbility[i] != GetExtraGrow(i))
			{
				mFashionAddAbility[i] = GetExtraGrow(i);
				InitoneBaseAbility(i);
			}
		}
	}
}

void HistoryActor::setTowerAddAbility()
{
	if (m_iControllerId <= 0)
	{
		return;
	}

	HistoryActor* ptrTActor = &history->actors[m_iControllerId - 1];
	//衣服时装
	if (ptrTActor->fashion.isNormalGood())
	{
		map<int, int> mSeid = ptrTActor->fashion.getSeid();
		for (UInt i = 0; i < mSeid.size(); i++)
		{
			if(mSeid[i] == 800)
				AddSpecialEffect(mSeid[i]);
		}
	}
	//武器时装
	if (ptrTActor->wap_fashion.isNormalGood())
	{
		map<int, int> mSeid = ptrTActor->wap_fashion.getSeid();
		for (UInt i = 0; i < mSeid.size(); i++)
		{
			if (mSeid[i] == 800)
				AddSpecialEffect(mSeid[i]);
		}
	}

	int iGIndex = trData->actors[m_iControllerId - 1].qid;
	SetExtraGrow(ACTOR_ABILITY_POWER, -1,trData->net_hero_grown[iGIndex][ptrTActor->actor_quality]->atk*10);
	SetExtraGrow(ACTOR_ABILITY_DEFENCE, -1, trData->net_hero_grown[iGIndex][ptrTActor->actor_quality]->def * 10);
	SetExtraGrow(ACTOR_ABILITY_INTELLEGENCE, -1, trData->net_hero_grown[iGIndex][ptrTActor->actor_quality]->ints * 10);
	SetExtraGrow(ACTOR_ABILITY_FORTUNE, -1, trData->net_hero_grown[iGIndex][ptrTActor->actor_quality]->fortune * 10);
	SetExtraGrow(ACTOR_ABILITY_EXPLOSION, -1, trData->net_hero_grown[iGIndex][ptrTActor->actor_quality]->burst * 10);
}

void HistoryActor::setAddAbilityGoodsEffect()
{
	int iGoodsId = 0;
	for (map<int, int>::iterator eIt = m_mAddAbilityGoods.begin(); eIt != m_mAddAbilityGoods.end(); eIt++)
	{
		iGoodsId = eIt->first;
		map<int, int> mSeid = trData->m_mGoodsItem[iGoodsId]->mSeid;
		for (int i = 0; i < eIt->second; i++)
		{
			for (size_t j = 0; j < mSeid.size(); j++)
			{
				AddSpecialEffect(mSeid[j]);
			}
		}
	}
}

void HistoryActor::setSpecialAddAbility()
{
	if (isHadSpecial(TYPE739)) //凤凰涅槃，五围中成长较低的X项增加Y点
	{
		//五围成长从小到大排序
		map<int, float> mTemp;
		for (int i = 0; i < 5; i++)
		{
			mTemp[i] = getGrownValue(i);
		}

		map<int, int> mSortIndex; //有序Map
		map<int, int> mSpecial = getSpecialMap(TYPE739);
		int iSortType = mSpecial[2]; //1、升序排列；2、降序排列
		int iIndex = 0;
		while(mTemp.size())
		{
			float fValue = mTemp.begin()->second;
			int iValIndex = mTemp.begin()->first;
			for (map<int, float>::iterator it = mTemp.begin(); it != mTemp.end(); it++)
			{
				if ((iSortType == 1 && it->second < fValue) || (iSortType == 2 && it->second > fValue))
				{
					fValue = it->second;
					iValIndex = it->first;
				}
			}
			mSortIndex[iIndex++] = iValIndex;
			mTemp.erase(iValIndex);
		}

		for (int i = 0; i < mSpecial[0] && i < (int)mSortIndex.size(); i++)
		{
			SetExtraGrow(mSortIndex[i], -1, mSpecial[1]);
		}
	}
}

int HistoryActor::getFmStoneIndex(map<int, int> mStoneId)
{
	int iStoneId = 0;
	for(map<int, FmStoneSuit*>::iterator it = trData->m_mFmStone.begin(); it != trData->m_mFmStone.end(); it++)
	{
		map<int, int> mNeedId = it->second->mNeedId;
		map<int, int> mTempId = mStoneId;
		if(mNeedId.size() == mTempId.size())
		{
			bool bAllready = true;
			for(UInt i = 0; i < mNeedId.size(); i++)
			{
				bool bExist = false;
				for(UInt j = 0; j < mTempId.size(); j++)
				{
					if(mTempId[j] == mNeedId[i])
					{
						mTempId[j] = 0;
						bExist = true;
						break;
					}
				}

				if(!bExist)
				{
					bAllready = false;
					break;
				}
			}

			if(bAllready)
			{
				iStoneId = it->first;
				break;
			}
		}
	}
	return iStoneId;
}

int HistoryActor::getFmStoneIndex(int iStoneId)
{
	map<int, int> mStoneId;
	mStoneId[0] = iStoneId;
	return getFmStoneIndex(mStoneId);
}

int HistoryActor::GetDoubleSeid(int fits, bool is_effect, int real_x, int real_y)
{
	int ret = -1,iX =x,iY = y;
	if (!actor_fit_id.size())
		return ret;
	if (real_x!=-1&&real_y!=-1)
	{
		iX = real_x;
		iY = real_y;
	}
	HistoryActorPtr ptrOppoActor = NULL;
	//复位
	for (int i = 0; i < actor_fit_id.size(); i++)
	{
		int fits_num = 0;
		int sound_id = 0;
		ActorFits* ptrFits = &trData->actor_fits[actor_fit_id[i]];
		if (ptrFits->fits!=fits)
			continue;
		for (int j = 0; j < ptrFits->hid_num; j++)
		{
			if (ptrFits->hid[j] != actor_id + 1 && !history->actors[ptrFits->hid[j] - 1].hide)
			{
				int offset_x = iX - history->actors[ptrFits->hid[j] - 1].x;
				int offset_y = iY - history->actors[ptrFits->hid[j] - 1].y;
				bool is_enable = true;
				for (int k = 0; k < trData->hitArea[ptrFits->atkid - 1].length; k++)
				{
					if (offset_x == trData->hitArea[ptrFits->atkid - 1].pointlist[k].x&&offset_y == trData->hitArea[ptrFits->atkid - 1].pointlist[k].y)
					{
						is_enable = false;
						fits_num++;
						break;
					}
				}
				if (is_enable)
					break;
			}
			else if (ptrFits->hid[j] == actor_id + 1)
			{
				sound_id = j;
			}
		}
		if (fits_num == ptrFits->hid_num - 1)
		{
			if (is_effect)
			{
				for (int j = 0; j < ptrFits->hid_num; j++)
				{
					if (ptrFits->hid[j] != actor_id + 1)
					{
						ret = ptrFits->hid[j];
					}
				}
			}
			else
				ret = ptrFits->sounds[sound_id];
			break;
		}
	}
	return ret;
}

void HistoryActor::setAnimationIndex()
{
	for (int i = 0; i < trData->animation_info_num; i++)
	{
		if (trData->animation_info[i].hid - 1 == actor_id&&mFashion[1] == trData->animation_info[i].gender)
		{
			animation_index = i;
			break;
		}
	}
}
char* HistoryActor::getAnimationName()
{
	if (animation_index > -1)
		return trData->animation_info[animation_index].name;
	else
	{
		setAnimationIndex();
		if (animation_index < 0)
			return NULL;
		return trData->animation_info[animation_index].name;
	}
}
DString HistoryActor::getAnimationPlayId(int iAnimId, int iType, bool& iFlip)
{
	//1_2&100_2_s2
	switch (iType)
	{
	case 1:
		return DString::format("%d_%d&%d_%d_s%d", mFashion[1], mFashion[2], mFashion[3], mFashion[4], iAnimId);
	case 2:
	{
			  int iDirection = 1;
			  if (direction == 0)
			  {
				  iDirection = 2;
			  }
			  else if (direction == 1)
			  {
				  iDirection = 3;
				  iFlip = true;
			  }
			  else if (direction == 2)
			  {
				  iDirection = 1;
			  }
			  else
			  {
				  iDirection = direction;
			  }
			  return DString::format("%d_%d&%d_%d_s%d_%d", mFashion[1], mFashion[2], mFashion[3], mFashion[4], iAnimId, iDirection).getChar();
	}
	case 3:
		break;
	}
	return DString("");
}

int HistoryActor::getOccuPowerType()
{
	int iJob1 = 0, iJob2 = 0, iJob3 = 0, iJob4 = 0;
	iJob1 = 1;
	iJob2 = 2;
	iJob3 = 9;
	iJob4 = 11;
	if (occupation_id == iJob1 || occupation_id == iJob2 || occupation_id == iJob3 || occupation_id == iJob4)
	{
		return 2;
	}
	else if (occupation_id == 10 && weapon.isNormalGood() && weapon.GetGoodsAbilityType() | GOODS_INTEL)
	{
		return 2;
	}
	return 1;
}

float HistoryActor::getFiveAbilityBuffEffect(int iState, int iBuffType)
{
	if (iState < 0)
		iBuffType += 1;
	if (!iState||!m_mActorBuff.count(iBuffType)) return 0;
	float iBuffNum = m_mActorBuff[iBuffType]->iPercent;
	return iBuffNum * iState;
}

int HistoryActor::GetActorSpeed(int iType)
{
	int  iRet = trData->occupations[occupation_id].base_speed + GetGoodsAddSpeed();
	int iBuffAdd =  state_speed * 2;
	if (state_speed>0&&m_mActorBuff.count(ACTOR_BUFF_SPEEDUP))
	{
		iBuffAdd = state_speed * iRet * m_mActorBuff[ACTOR_BUFF_SPEEDUP]->iPercent / 100;
	}
	else if (state_speed < 0 && m_mActorBuff.count(ACTOR_BUFF_SPEEDDW))
	{
		iBuffAdd = state_speed * iRet * m_mActorBuff[ACTOR_BUFF_SPEEDDW]->iPercent / 100;
	}

	if (iType == 1)
		return iRet + iBuffAdd;
	else
		return iBuffAdd;
}

void HistoryActor::needAssault(HistoryActorPtr ptrActor, int &AssX, int &AssY)
{
	int temp_x = x, temp_y = y, move_flag = 0, offsetX = 0, offsetY = 0;
	offsetX = ptrActor->x - x;
	offsetY = ptrActor->y - y;

	if ((offsetX&&offsetY&&abs(offsetX / offsetY) > 1))
		temp_x -= offsetX / abs(offsetX);
	else if (offsetX&&offsetY&&abs(offsetX / offsetY) < 1)
		temp_y -= offsetY / abs(offsetY);
	else if (offsetX&&offsetY&&abs(offsetX / offsetY) == 1)
	{
		temp_x -= offsetX / abs(offsetX);
		temp_y -= offsetY / abs(offsetY);
	}
	else if (!offsetX)
		temp_y -= offsetY / abs(offsetY);
	else if (!offsetY)
		temp_x -= offsetX / abs(offsetX);
	else
		return;

	if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{
		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if (oppoActor != NULL || history->HasElem(temp_x, temp_y, occupation_id) || trData->occupation_landmove_info[occupation_id][history->map_array[temp_y][temp_x]] == 255)
			move_flag = MAP_NO_REACH;
		else
			move_flag = MAP_CAN_MOVE;

		if (move_flag == MAP_CAN_MOVE)
		{
			AssX = temp_x;
			AssY = temp_y;
		}
	}
}
int HistoryActor::needAssault(int iMoveX, int iMoveY, HistoryActorPtr ptrActor)//判断是否会被击退
{
	if(getSpecialInt(TYPE703) || (rand() % 100) < getSpecialInt(TYPE722)) //被攻击不会后退
	{
		return 0;
	}

	int temp_x = x, temp_y = y, move_flag = 0, offsetX = 0, offsetY = 0;
	offsetX = iMoveX - x;
	offsetY = iMoveY - y;

	if (!offsetX&&!offsetY)
		return 0;
	else if ((offsetX&&offsetY&&abs(offsetX / offsetY) > 1))
		temp_x -= offsetX / abs(offsetX);
	else if (offsetX&&offsetY&&abs(offsetX / offsetY) < 1)
		temp_y -= offsetY / abs(offsetY);
	else if (offsetX&&offsetY&&abs(offsetX / offsetY) == 1)
	{
		temp_x -= offsetX / abs(offsetX);
		temp_y -= offsetY / abs(offsetY);
	}
	else if (!offsetX)
		temp_y -= offsetY / abs(offsetY);
	else if (!offsetY)
		temp_x -= offsetX / abs(offsetX);

	if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{
		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if (BattleControler::auto_run&&oppoActor&&oppoActor->actor_id == ptrActor->actor_id)
			oppoActor = NULL;
		if (oppoActor != NULL || history->HasElem(temp_x, temp_y, occupation_id) || trData->occupation_landmove_info[occupation_id][history->map_array[temp_y][temp_x]] == 255)
			move_flag = MAP_NO_REACH;
		else
			move_flag = MAP_CAN_MOVE;

		if (move_flag == MAP_CAN_MOVE)
		{
			return 1;
		}
	}
	return 0;
}

int HistoryActor::needAssault(int iMoveX, int iMoveY, int iCurX, int iCurY, HistoryActorPtr ptrActor)
{
	if(getSpecialInt(TYPE703) || (rand() % 100) < getSpecialInt(TYPE722)) //被攻击不会后退
	{
		return 0;
	}

	int temp_x = iCurX, temp_y = iCurY, move_flag = 0, offsetX = 0, offsetY = 0;
	offsetX = iMoveX - iCurX;
	offsetY = iMoveY - iCurY;

	if(!offsetX && !offsetY)
		return 0;
	else if((offsetX&&offsetY&&abs(offsetX / offsetY) > 1))
		temp_x -= offsetX / abs(offsetX);
	else if(offsetX&&offsetY&&abs(offsetX / offsetY) < 1)
		temp_y -= offsetY / abs(offsetY);
	else if(offsetX&&offsetY&&abs(offsetX / offsetY) == 1)
	{
		temp_x -= offsetX / abs(offsetX);
		temp_y -= offsetY / abs(offsetY);
	}
	else if(!offsetX)
		temp_y -= offsetY / abs(offsetY);
	else if(!offsetY)
		temp_x -= offsetX / abs(offsetX);

	if((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{
		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if(BattleControler::auto_run&&oppoActor&&oppoActor->actor_id == ptrActor->actor_id)
			oppoActor = NULL;
		if(oppoActor != NULL || history->HasElem(temp_x, temp_y, occupation_id) || trData->occupation_landmove_info[occupation_id][history->map_array[temp_y][temp_x]] == 255)
			move_flag = MAP_NO_REACH;
		else
			move_flag = MAP_CAN_MOVE;

		if(move_flag == MAP_CAN_MOVE)
		{
			return 1;
		}
	}
	return 0;
}

int HistoryActor::CanDrag(int iMoveX, int iMoveY, HistoryActorPtr ptrActor,bool bisPoint)
{
	if (!bisPoint&&(getSpecialInt(TYPE703) || (rand() % 100) < getSpecialInt(TYPE722))) //被攻击不会后退
	{
		return 0;
	}

	int temp_x = x, temp_y = y, move_flag = 0, offsetX = 0, offsetY = 0;
	offsetX = iMoveX - x;
	offsetY = iMoveY - y;

	if ((offsetX&&offsetY) || (!offsetX&&abs(offsetY) < 2) || (abs(offsetX) < 2 && !offsetY))
	{
		return 0;
	}

	int iIsPositive = 1,iWhileOffset = 0,iEnbale = 1;

	if (offsetX < 0 || offsetY < 0) iIsPositive = -1;

	do {
		iEnbale = 1;
		if (offsetX == 0)
		{
			if (iWhileOffset++ < abs(offsetY) - 1)
			{
				temp_y += iIsPositive;
			}
			else
			{
				iEnbale = 0;
			}
		}
		else if (offsetY == 0)
		{
			if (iWhileOffset++ < abs(offsetX) - 1)
			{
				temp_x += iIsPositive;
			}
			else
			{
				iEnbale = 0;
			}
		}
		else
		{
			iEnbale = 0;
		}

		if (iEnbale)
		{
			if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
			{
				HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
				if (BattleControler::auto_run&&oppoActor&&oppoActor->actor_id == ptrActor->actor_id)
					oppoActor = NULL;
				if (oppoActor != NULL || history->HasElem(temp_x, temp_y, occupation_id) || trData->occupation_landmove_info[occupation_id][history->map_array[temp_y][temp_x]] == 255)
					move_flag = MAP_NO_REACH;
				else
					move_flag = MAP_CAN_MOVE;

				if (move_flag == MAP_NO_REACH)
				{
					return 0;
				}
			}
		}

	} while (iEnbale);

	if (move_flag == MAP_CAN_MOVE)
		return 1;

	return 0;
}

int HistoryActor::GetRangeBuffEffect(int iBuffType, bool bIsFavour, bool bIsAdd)
{
	int iRet = 0,iBuffValue = 0;
	if ((bIsFavour&&relation != 2) || (!bIsFavour&&relation == 2))
	{
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			if (history->battleOwn[i]->hide != 1)
			{
				iBuffValue = history->battleOwn[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (bIsAdd)
				{
					iRet += iBuffValue;
				}
				else if(iRet == 0|| iBuffValue>iRet)
				{
					iRet = iBuffValue;
				}
			}
		}
		for (int i = 0; i < history->battleFriendNum; i++)
		{
			if (history->battleFriend[i]->hide != 1)
			{
				iBuffValue = history->battleFriend[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (bIsAdd)
				{
					iRet += iBuffValue;
				}
				else if (iRet == 0 || iBuffValue > iRet)
				{
					iRet = iBuffValue;
				}
			}
		}
	}
	else if ((!bIsFavour&&relation != 2) || (bIsFavour&&relation == 2))
	{
		for (int i = 0; i < history->battleEnemyNum; i++)
		{
			if (history->battleEnemy[i]->hide != 1)
			{
				iBuffValue = history->battleEnemy[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (bIsAdd)
				{
					iRet += iBuffValue;
				}
				else if (iRet == 0 || iBuffValue > iRet)
				{
					iRet = iBuffValue;
				}
			}
		}
	}

	return iRet;
}

int HistoryActor::GetRange965FiveAbilityEffect(int iAbilityCase,int iAbilityNum)
{
	int iRet = 0,iTempNum = 0;

	if (relation != 2)
	{
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			if (history->battleOwn[i]->hide != 1)
			{
				iTempNum = history->battleOwn[i]->getTotalRangeEffect(RANGE_ADD_TOWER_ABILITY, true, x, y);
				if (iTempNum>0&&trData->m_mSeid.count(iTempNum))
				{
					SeidPtr ptrSeid = trData->m_mSeid[iTempNum];
					if ((int)pow(2, iAbilityCase) & ptrSeid->special_effe[1])
					{
						iRet += ptrSeid->special_effe[2];
					}
				}
			}
		}
		for (int i = 0; i < history->battleFriendNum; i++)
		{
			if (history->battleFriend[i]->hide != 1)
			{
				iTempNum = history->battleFriend[i]->getTotalRangeEffect(RANGE_ADD_TOWER_ABILITY, true, x, y);
				if (iTempNum > 0 && trData->m_mSeid.count(iTempNum))
				{
					SeidPtr ptrSeid = trData->m_mSeid[iTempNum];
					if ((int)pow(2, iAbilityCase) & ptrSeid->special_effe[1])
					{
						iRet += ptrSeid->special_effe[2];
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < history->battleEnemyNum; i++)
		{
			if (history->battleEnemy[i]->hide != 1)
			{
				iTempNum = history->battleEnemy[i]->getTotalRangeEffect(RANGE_ADD_TOWER_ABILITY, true, x, y);
				if (iTempNum > 0 && trData->m_mSeid.count(iTempNum))
				{
					SeidPtr ptrSeid = trData->m_mSeid[iTempNum];
					if ((int)pow(2, iAbilityCase) & ptrSeid->special_effe[1])
					{
						iRet += ptrSeid->special_effe[2];
					}
				}
			}
		}
	}

	return iRet*iAbilityNum/100;
}

int HistoryActor::getRangeBuffActorId(int iBuffType, bool bIsFavour)
{
	if((bIsFavour&&relation != 2) || (!bIsFavour&&relation == 2))
	{
		for(int i = 0; i < history->battleOwnNum; i++)
		{
			if(history->battleOwn[i]->hide != 1)
			{
				if (history->battleOwn[i]->getTotalRangeEffect(iBuffType, true, x, y))
				{
					return history->battleOwn[i]->actor_id;
				}
			}
		}
		for(int i = 0; i < history->battleFriendNum; i++)
		{
			if(history->battleFriend[i]->hide != 1)
			{
				if(history->battleFriend[i]->getTotalRangeEffect(iBuffType, true, x, y))
				{
					return history->battleFriend[i]->actor_id;
				}
			}
		}
	}
	else if((!bIsFavour&&relation != 2) || (bIsFavour&&relation == 2))
	{
		for(int i = 0; i < history->battleEnemyNum; i++)
		{
			if(history->battleEnemy[i]->hide != 1)
			{
				if(history->battleEnemy[i]->getTotalRangeEffect(iBuffType, true, x, y))
				{
					return history->battleEnemy[i]->actor_id;
				}
			}
		}
	}

	return -1;
}

std::map<int, int> HistoryActor::getRangeBuffActorAndNum(int iBuffType, bool bIsFavour)
{
	map<int, int> mRet;
	int iTempNum = 0,iActorId = 0;
	if ((bIsFavour&&relation != 2) || (!bIsFavour&&relation == 2))
	{
		for (int i = 0; i < history->battleOwnNum; i++)
		{
			if (history->battleOwn[i]->hide != 1)
			{
				iTempNum = history->battleOwn[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (iTempNum>0)
				{
					iActorId = history->battleOwn[i]->actor_id+1;
					break;
				}
			}
		}
		for (int i = 0; i < history->battleFriendNum; i++)
		{
			if (history->battleFriend[i]->hide != 1)
			{
				iTempNum = history->battleFriend[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (iTempNum > 0)
				{
					iActorId = history->battleFriend[i]->actor_id+1;
					break;
				}
			}
		}
	}
	else if ((!bIsFavour&&relation != 2) || (bIsFavour&&relation == 2))
	{
		for (int i = 0; i < history->battleEnemyNum; i++)
		{
			if (history->battleEnemy[i]->hide != 1)
			{
				iTempNum = history->battleEnemy[i]->getTotalRangeEffect(iBuffType, true, x, y);
				if (iTempNum > 0)
				{
					iActorId = history->battleEnemy[i]->actor_id+1;
					break;
				}
			}
		}
	}
	mRet[0] = iActorId;
	mRet[1] = iTempNum;
	return mRet;
}

int HistoryActor::GetActorJobImgId()
{
	if (actor_id < trData->actor_num)
		return trData->actors[actor_id].job_img;

	return 1;
}

int HistoryActor::getMoveSpeedUsed(int iPosX, int iPosY, int iMoveFlag)
{
	int iLandIndex = history->map_array[iPosY][iPosX];
	int iSpeedUsed = trData->occupation_landmove_info[occupation_id][iLandIndex];
	int iSpeedBarrier = history->getBarrierMoveSpend(occupation_id, iPosX, iPosY);
	if (iSpeedUsed == 255 && iSpeedBarrier)
	{
		iSpeedUsed = iSpeedBarrier;
	}

	if(good_rough_move && (iMoveFlag == MAP_CAN_REACH || iMoveFlag == MAP_CAN_MOVE) && iSpeedUsed != 255) //的卢恶路移动
	{
		iSpeedUsed = 1;
	}
	else
	{
		if(good_effects[23]) //辅助地形移动
		{
			int iLandId = trData->m_mSeid[good_effects[23]]->special_effe[0];
			if(iLandId == 979 || trData->lands[iLandIndex].type == iLandId)
			{
				iSpeedUsed = trData->m_mSeid[good_effects[23]]->special_effe[1];
			}
		}

		if(isHadSpecial(TYPE602)) //指定地形移动能力提升
		{
			map<int, int> mSpecial = getSpecialMap(TYPE602);
			SeidType* ptrType = trData->m_mSeidType[TYPE602];
			for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
			{
				if(mSpecial[i] == ptrType->iClass || mSpecial[i] == trData->lands[iLandIndex].type)
				{
					iSpeedUsed -= mSpecial[i + 1];
				}
			}
		}
	}

	if(iSpeedUsed < 1)
	{
		iSpeedUsed = 1;
	}

	return iSpeedUsed;
}

void HistoryActor::updateActorBuff(int iBuffId, int iBuffState, bool bIsAttr, ActionGain* ptrGain /*= NULL*/, HistoryActor* pCurActor /*= NULL*/)
{
	if (isHadSpecial(TYPE743) && iBuffState < 0) //BOSS特效，免疫特定BUFF
	{
		map<int, int> mSpecial = getSpecialMap(TYPE743);
		for (UInt i = 0; i < mSpecial.size(); i++)
		{
			if (mSpecial[i] == iBuffId)
			{
				return;
			}
		}
	}

	if(iBuffState)
	{
		BuffElem* ptrBuff = trData->m_mDataBuff[iBuffId];
		int iRound = ptrBuff->iRound;
		float iPercent = ptrBuff->iPercent;
		float iPercentNum = 0;
		int iCasterActorId = 0;
		if(ptrGain && ptrGain->skill && ptrGain->skill->seid != 125) //125龙啸采用普通BUFF数据
		{
			iRound = ptrGain->skill->seid_round;
			iPercent = ptrGain->skill->prop_per + ptrGain->skill->GetExtraGainHp(pCurActor, 3);
			iPercentNum = ptrGain->skill->prop_num + ptrGain->skill->GetExtraGainHp(pCurActor, 2);
			iCasterActorId = ptrGain->skill->GetCasterActorId(pCurActor);
		}
		else if (ptrGain && ptrGain->good&&ptrGain->good->iUeid >= 44 && ptrGain->good->iUeid <= 48)//暂时写死，后面版本重做
		{
			iRound = 5;
			iPercent = 40;
		}

		if(bIsAttr)
		{
			if(iBuffState > 0)
			{
				int iCountDw = m_mActorBuff.count(iBuffId + 1);
				if(iCountDw && m_mActorBuff[iBuffId + 1]->iCurRound)
				{
					iBuffId++;
					m_mActorBuff[iBuffId]->iCurRound = 0;
					m_mActorBuff[iBuffId]->iArmIndex = 2;
				}
				else
				{
					ActorBuff* pActBuffUp = m_mActorBuff[iBuffId];
					if(!pActBuffUp)
					{
						pActBuffUp = new ActorBuff();
						pActBuffUp->iBuffId = iBuffId;
						pActBuffUp->iMaxRound = iRound;
						m_mActorBuff[iBuffId] = pActBuffUp;
					}
					pActBuffUp->iPercent = iPercent;
					pActBuffUp->iCurRound = iRound;
					pActBuffUp->iArmIndex = 1;
				}
			}
			else
			{
				int iCountUp = m_mActorBuff.count(iBuffId);
				if(iCountUp && m_mActorBuff[iBuffId]->iCurRound)
				{
					m_mActorBuff[iBuffId]->iCurRound = 0;
					m_mActorBuff[iBuffId]->iArmIndex = 2;
				}
				else
				{
					ActorBuff* pActBuffDw = m_mActorBuff[++iBuffId];
					if(!pActBuffDw)
					{
						pActBuffDw = new ActorBuff();
						pActBuffDw->iBuffId = iBuffId;
						pActBuffDw->iMaxRound = iRound;
						m_mActorBuff[iBuffId] = pActBuffDw;
					}
					pActBuffDw->iPercent = iPercent;
					pActBuffDw->iCurRound = iRound;
					pActBuffDw->iArmIndex = 1;
				}
			}
		}
		else
		{
			int iCount = m_mActorBuff.count(iBuffId);
			if(iCount)
			{
				ActorBuff* pActBuff = m_mActorBuff[iBuffId];
				if(ptrBuff->iEffectType || iBuffState < 0)
				{
					if(pActBuff->iCurRound == 0 || ptrBuff->iReset)
					{
						pActBuff->iCurRound = iRound;
						pActBuff->iPercent = iPercent;
						pActBuff->iPercentNum = iPercentNum;
						pActBuff->iCasterActorId = iCasterActorId;
						pActBuff->iArmIndex = 1;
					}
				}
				else if(iBuffState > 0)
				{
					pActBuff->iCurRound = 0;
					pActBuff->iArmIndex = 2;
				}
			}
			else
			{
				if(ptrBuff->iEffectType || iBuffState < 0)
				{
					ActorBuff* pActBuff = new ActorBuff();
					pActBuff->iBuffId = iBuffId;
					pActBuff->iMaxRound = iRound;
					pActBuff->iCurRound = iRound;
					pActBuff->iPercent = iPercent;
					pActBuff->iPercentNum = iPercentNum;
					pActBuff->iCasterActorId = iCasterActorId;
					m_mActorBuff[iBuffId] = pActBuff;
					pActBuff->iArmIndex = 1;
				}
			}
		}
		updateActorState(iBuffId);
	}
}

void HistoryActor::updateActorState(int iBuffId)
{
	if (m_mActorBuff.count(iBuffId))
	{
		int iState = m_mActorBuff[iBuffId]->iCurRound ? 1 : 0;
		switch(iBuffId)
		{
		case ACTOR_BUFF_PUZZLE:
			state_puzzle = iState;
			break;
		case ACTOR_BUFF_OBFU:
			state_obfuscation = iState;
			break;
		case ACTOR_BUFF_STUN:
			state_stun = iState;
			break;
		case ACTOR_BUFF_WEAK:
			state_weaked = iState;
			break;
		case ACTOR_BUFF_BLIND:
			state_blind = iState;
			break;
		case ACTOR_BUFF_PALSY:
			state_palsy = iState;
			break;
		case ACTOR_BUFF_BLOOD:
			state_loseblood = iState;
			break;
		case ACTOR_BUFF_ADDHP:
			state_addhp = iState;
			break;
		case ACTOR_BUFF_POISON:
			state_poison = iState;
			break;
		case ACTOR_BUFF_POWUP:
			state_power = iState;
			break;
		case ACTOR_BUFF_POWDW:
			state_power = -iState;
			break;
		case ACTOR_BUFF_INTUP:
			state_intellegence = iState;
			break;
		case ACTOR_BUFF_INTDW:
			state_intellegence = -iState;
			break;
		case ACTOR_BUFF_DEFUP:
			state_defence = iState;
			break;
		case ACTOR_BUFF_DEFDW:
			state_defence = -iState;
			break;
		case ACTOR_BUFF_EXPUP:
			state_explosion = iState;
			break;
		case ACTOR_BUFF_EXPDW:
			state_explosion = -iState;
			break;
		case ACTOR_BUFF_FORTUP:
			state_fortune = iState;
			break;
		case ACTOR_BUFF_FORTDW:
			state_fortune = -iState;
			break;
		case ACTOR_BUFF_SPEEDUP:
			state_speed = iState;
			break;
		case ACTOR_BUFF_SPEEDDW:
			state_speed = -iState;
			break;
		case ACTOR_BUFF_HURTUP:
			state_add_skill_hurt_range = iState;
			break;
		case ACTOR_BUFF_HURTDW:
			state_reduce_skill_hurt_range = iState;
			break;
		case ACTOR_BUFF_FIRESHIELD:
			state_SkillHurtAndRebound = iState;
			break;
		case ACTOR_BUFF_ADDDEFREDPOW:
			state_adddef_redpow = iState;
			break;
		case ACTOR_BUFF_SUALLINK:
			state_sual_link = iState;
			break;
		case ACTOR_BUFF_REDUCESKILL:
			state_reduceSkill = iState;
			break;
		case ACTOR_BUFF_REDUCEHURT:
			state_reduceHurt = iState;
			break;
		case ACTOR_BUFF_EMERGENCYCOMMAND:
			state_emergency_command = iState;
			break;
		default:
			break;
		}
	}
}

void HistoryActor::updateBuffDisplay(Layer* ptrLayer)
{
	string strPath = "";
	string strArmBuff = "";
	for(map<int, ActorBuff*>::iterator it = m_mActorBuff.begin(); it != m_mActorBuff.end(); it++)
	{
		if(it->second->iCurRound > 0)
		{
			BuffElem* ptrBuff = trData->m_mDataBuff[it->first];
			if(strPath == "")
			{
				int iImgId = ptrBuff->iImgId;
				strPath = StringUtils::format("new_ui/buff/%d.png", iImgId);
			}

			if(ptrBuff->iIsPlay == 2 && strArmBuff == "") //光环
			{
				strArmBuff = ptrBuff->strAnima1;
			}
		}
	}

	if (m_spBuff)
	{
		if(strPath != "")
		{
			m_spBuff->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
		}
		else
		{
			m_spBuff->removeFromParent();
			m_spBuff = nullptr;
		}
	}
	else
	{
		if(strPath != "")
		{
			m_spBuff = ReadSpriteName(strPath.c_str());
			m_spBuff->setScale(0.6f);
			ptrLayer->addChild(m_spBuff);
		}
	}

	if (m_armBuff)
	{
		if (m_armBuff->getName() != strArmBuff)
		{
			m_armBuff->removeFromParent();
			m_armBuff = NULL;
		}
	}

	if(!m_armBuff && strArmBuff != "")
	{
		string strArmPath = StringUtils::format("buff_effect/%s.ExportJson", strArmBuff.c_str());
		ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath(strArmPath.c_str(), 0, 1));
		m_armBuff = Armature::create(strArmBuff);
		m_armBuff->getAnimation()->play(strArmBuff);
		ptrLayer->addChild(m_armBuff);
	}
}

void HistoryActor::updateBuffPosition(bool bVisible)
{
	int iPosX = this->xx() - history->scr_x;
	int iPosY = this->yy() - history->scr_y;

	if(bVisible && (this->hide || this->is_leave))
	{
		bVisible = false;
	}

	if (m_spBuff)
	{
		m_spBuff->setPosition(Vec2(iPosX + m_spBuff->getContentSize().width * m_spBuff->getScale() / 2, screen_size_y - iPosY - m_spBuff->getContentSize().height * m_spBuff->getScale() / 2 - 5));
		m_spBuff->setVisible(bVisible);
	}

	if (m_armBuff)
	{
		m_armBuff->setPosition(Vec2(iPosX + SCENE_TILE_WIDTH / 2, screen_size_y - iPosY - SCENE_TILE_WIDTH / 2));
		m_armBuff->setVisible(bVisible);
	}
}

void HistoryActor::clearActorBuff()
{
	for(map<int, ActorBuff*>::iterator it = m_mActorBuff.begin(); it != m_mActorBuff.end(); it++)
	{
		it->second->iCurRound = 0;
		updateActorState(it->first);
		SAFE_DELETE(it->second);
	}
	m_mActorBuff.clear();
	if(m_spBuff)
	{
		m_spBuff->removeFromParent();
		m_spBuff = nullptr;
	}

	if(m_armBuff)
	{
		m_armBuff->removeFromParent();
		m_armBuff = nullptr;
	}
}

int HistoryActor::getLandAbilityGain(int iLandId)
{
	int iAddAbility = trData->occupation_land_info[occupation_id][iLandId];
	if(isHadSpecial(TYPE320)) //辅助地形效果
	{
		map<int, int> mSpecial = getSpecialMap(TYPE320);
		SeidType* ptrType = trData->m_mSeidType[TYPE320];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == trData->lands[iLandId].type)
			{
				iAddAbility += mSpecial[i + 1];
			}
		}
	}



	if (isHadSpecial(TYPE713)) //辅助地形效果
	{
		map<int, int> mSpecial = getSpecialMap(TYPE713);
		SeidType* ptrType = trData->m_mSeidType[TYPE713];
		for (UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if ((mSpecial[i] == ptrType->iClass || mSpecial[i] == trData->lands[iLandId].type)&& mSpecial[i + 1]>iAddAbility)
			{
				iAddAbility = mSpecial[i + 1];
			}
		}
	}

	int iRandAddAbility = GetRangeBuffEffect(RANGE_ADD_LAND_ABILITY,true,false);
	if (iRandAddAbility > iAddAbility)
		iAddAbility = iRandAddAbility;

	return iAddAbility;
}

bool HistoryActor::isBackAttack(HistoryActor* ptrActor)
{
	bool isBack = false;
	if (getSpecialInt(TYPE704))
	{
		isBack = true;
	}
	else
	{
		switch(ptrActor->direction)
		{
		case 0: //上
			if(y >= ptrActor->y)
			{
				isBack = true;
			}
			break;
		case 1: //右
			if(x <= ptrActor->x)
			{
				isBack = true;
			}
			break;
		case 2: //下
			if(y <= ptrActor->y)
			{
				isBack = true;
			}
			break;
		case 3: //左
			if(x >= ptrActor->x)
			{
				isBack = true;
			}
			break;
		default:
			break;
		}
	}
	return isBack;
}

int HistoryActor::getAttackedType(HistoryActor* ptrActor)
{
	int iPecent = 0;
	if (isHadSpecial(TYPE744))
	{
		int iType = 0; // 0--正面，1--侧面，2--背面
		if (x == ptrActor->x || y == ptrActor->y)
		{
			iType = 1;
		}
		else if(ptrActor->isBackAttack(this))
		{
			iType = 2;
		}
		iPecent = getSpecialMap(TYPE744)[iType];
	}
	return iPecent;
}

int HistoryActor::getActionDirect(int iDestX, int iDestY)
{
	int iDirection = direction;

	if (iDestX == x) //通过斜率取值范围划分四个象限来判断朝向
	{
		if (iDestY != y)
		{
			iDirection = iDestY < y ? 0 : 2;
		}
	}
	else
	{
		float fSlope = float(iDestY - y) / (iDestX - x);
		if (iDestY < y && (fSlope < -1.0f || fSlope > 1.0f))
		{
			iDirection = 0;
		}
		else if (iDestX > x && (fSlope >= -1.0f && fSlope <= 1.0f))
		{
			iDirection = 1;
		}
		else if (iDestY < y && (fSlope < -1.0f || fSlope > 1.0f))
		{
			iDirection = 2;
		}
		else if (iDestX < x && (fSlope >= -1.0f && fSlope <= 1.0f))
		{
			iDirection = 3;
		}
	}
	return iDirection;
}

int HistoryActor::getTotalAbility(int iTypeId)
{
	int iTotal = 0;
	switch(iTypeId)
	{
	case TOTAL_POWER:
		iTotal = GetBasepower() + GetGoodsAddPower();
		break;
	case TOTAL_INTELLEGENCE:
		iTotal = GetBaseIntellegence() + GetGoodsAddIntellegence();
		break;
	case TOTAL_DEFENCE:
		iTotal = GetBaseDefence() + GetGoodsAddDefence();
		break;
	case TOTAL_EXPLOSION:
		iTotal = GetBaseExplosion() + GetGoodsAddExplosion();
		break;
	case TOTAL_FORTUNE:
		iTotal = GetBaseFortune() + GetGoodsAddFortune();
		break;
	case TOTAL_SPEED:
		iTotal = GetBaseSpeed() + GetGoodsAddSpeed();
		break;
	case TOTAL_HP:
		iTotal = GetBaseHp() + GetGoodsAddHp();
		break;
	case TOTAL_MP:
		iTotal = GetBaseMp() + GetGoodsAddMp();
		break;
	default:
		break;
	}
	return iTotal;
}

int HistoryActor::GetBasepower()
{
	return base_power^m_EncryptKey[BASE_POWER];
}

void HistoryActor::SetBasepower(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_power = iSet^iEncryptKey;
	}
	else
	{
		base_power = (GetBasepower() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_POWER] = iEncryptKey;
}

int HistoryActor::GetBaseDefence()
{
	return base_defence^m_EncryptKey[BASE_DEFENCE];
}

void HistoryActor::SetBaseDefence(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_defence = iSet^iEncryptKey;
	}
	else
	{
		base_defence = (GetBaseDefence() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_DEFENCE] = iEncryptKey;
}

int HistoryActor::GetBaseIntellegence()
{
	return base_intellegence^m_EncryptKey[BASE_INTELLEGENCE];
}

void HistoryActor::SetBaseIntellegence(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_intellegence = iSet^iEncryptKey;
	}
	else
	{
		base_intellegence = (GetBaseIntellegence()+iAdd)^iEncryptKey;
	}
	m_EncryptKey[BASE_INTELLEGENCE] = iEncryptKey;
}

int HistoryActor::GetBaseExplosion()
{
	return base_explosion^m_EncryptKey[BASE_EXPLOSION];
}

void HistoryActor::SetBaseExplosion(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_explosion = iSet^iEncryptKey;
	}
	else
	{
		base_explosion = (GetBaseExplosion()+iAdd)^iEncryptKey;
	}
	m_EncryptKey[BASE_EXPLOSION] = iEncryptKey;
}

int HistoryActor::GetBaseFortune()
{
	return base_fortune^m_EncryptKey[BASE_FORTUNE];
}

void HistoryActor::SetBaseFortune(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_fortune = iSet^iEncryptKey;
	}
	else
	{
		base_fortune = (GetBaseFortune() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_FORTUNE] = iEncryptKey;
}

int HistoryActor::GetBaseSpeed()
{
	return base_speed^m_EncryptKey[BASE_SPEED];
}

void HistoryActor::SetBaseSpeed(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		base_speed = iSet^iEncryptKey;
	}
	else
	{
		base_speed = (GetBaseSpeed() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_SPEED] = iEncryptKey;
}

int HistoryActor::GetBaseHp()
{
	return hp^m_EncryptKey[BASE_HP];
}

void HistoryActor::SetBaseHp(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		hp = iSet^iEncryptKey;
	}
	else
	{
		hp = (GetBaseHp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_HP] = iEncryptKey;
}

int HistoryActor::GetBaseMp()
{
	return mp^m_EncryptKey[BASE_MP];
}

void HistoryActor::SetBaseMp(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		mp = iSet^iEncryptKey;
	}
	else
	{
		mp = (GetBaseMp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[BASE_MP] = iEncryptKey;
}

int HistoryActor::GetCurpower()
{
	return power^m_EncryptKey[CUR_POWER];
}

void HistoryActor::SetCurpower(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		power = iSet^iEncryptKey;
	}
	else
	{
		power = (GetCurpower() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_POWER] = iEncryptKey;
}

int HistoryActor::GetCurDefence()
{
	return defence^m_EncryptKey[CUR_DEFENCE];
}

void HistoryActor::SetCurDefence(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		defence = iSet^iEncryptKey;
	}
	else
	{
		defence = (GetCurDefence() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_DEFENCE] = iEncryptKey;
}

int HistoryActor::GetCurIntellegence()
{
	return intellegence^m_EncryptKey[CUR_INTELLEGENCE];
}

void HistoryActor::SetCurIntellegence(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		intellegence = iSet^iEncryptKey;
	}
	else
	{
		intellegence = (GetCurIntellegence() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_INTELLEGENCE] = iEncryptKey;
}

int HistoryActor::GetCurExplosion()
{
	return explosion^m_EncryptKey[CUR_EXPLOSION];
}

void HistoryActor::SetCurExplosion(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		explosion = iSet^iEncryptKey;
	}
	else
	{
		explosion = (GetCurExplosion() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_EXPLOSION] = iEncryptKey;
}

int HistoryActor::GetCurFortune()
{
	return fortune^m_EncryptKey[CUR_FORTUNE];
}

void HistoryActor::SetCurFortune(int iSet /*= 0*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		fortune = iSet^iEncryptKey;
	}
	else
	{
		fortune = (GetCurFortune() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_FORTUNE] = iEncryptKey;
}

int HistoryActor::GetCurHp()
{
	return cur_hp^m_EncryptKey[CUR_HP];
}

void HistoryActor::SetCurHp(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		cur_hp = iSet^iEncryptKey;
	}
	else
	{
		cur_hp = (GetCurHp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_HP] = iEncryptKey;
}

int HistoryActor::GetCurMp()
{
	return cur_mp^m_EncryptKey[CUR_MP];
}

void HistoryActor::SetCurMp(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		cur_mp = iSet^iEncryptKey;
	}
	else
	{
		cur_mp = (GetCurMp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[CUR_MP] = iEncryptKey;
}

int HistoryActor::GetActorLv()
{
	return level^m_EncryptKey[ACTOR_LEVEL];
}

void HistoryActor::SetActorLv(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		level = iSet^iEncryptKey;
	}
	else
	{
		level = (GetActorLv() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[ACTOR_LEVEL] = iEncryptKey;
}

int HistoryActor::GetActorOldLv()
{
	return old_level^m_EncryptKey[ACTOR_OLDLEVEL];
}

void HistoryActor::SetActorOldLv(int iSet, int iAdd)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		old_level = iSet^iEncryptKey;
	}
	else
	{
		old_level = (GetActorOldLv() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[ACTOR_OLDLEVEL] = iEncryptKey;
}

int HistoryActor::GetGoodsAddPower()
{
	return good_add_power^m_EncryptKey[GOODS_ADD_POWER];
}

void HistoryActor::SetGoodsAddPower(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_power = iSet^iEncryptKey;
	}
	else
	{
		good_add_power = (GetGoodsAddPower() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_POWER] = iEncryptKey;
}

int HistoryActor::GetGoodsAddDefence()
{
	return good_add_defence ^m_EncryptKey[GOODS_ADD_DEFENCE];
}

void HistoryActor::SetGoodsAddDefence(int iSet/* = -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_defence = iSet^iEncryptKey;
	}
	else
	{
		good_add_defence = (GetGoodsAddDefence() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_DEFENCE] = iEncryptKey;
}

int HistoryActor::GetGoodsAddIntellegence()
{
	return good_add_intellegence^m_EncryptKey[GOODS_ADD_INTELLEGENCE];
}

void HistoryActor::SetGoodsAddIntellegence(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_intellegence = iSet^iEncryptKey;
	}
	else
	{
		good_add_intellegence = (GetGoodsAddIntellegence() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_INTELLEGENCE] = iEncryptKey;
}

int HistoryActor::GetGoodsAddExplosion()
{
	return good_add_explosion^m_EncryptKey[GOODS_ADD_EXPLOSION];
}

void HistoryActor::SetGoodsAddExplosion(int iSet/* = -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_explosion = iSet^iEncryptKey;
	}
	else
	{
		good_add_explosion = (GetGoodsAddExplosion() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_EXPLOSION] = iEncryptKey;
}

int HistoryActor::GetGoodsAddFortune()
{
	return good_add_fortune^m_EncryptKey[GOODS_ADD_FORTUNE];
}

void HistoryActor::SetGoodsAddFortune(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_fortune = iSet^iEncryptKey;
	}
	else
	{
		good_add_fortune = (GetGoodsAddFortune() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_FORTUNE] = iEncryptKey;
}

int HistoryActor::GetGoodsAddSpeed()
{
	return good_add_speed^m_EncryptKey[GOODS_ADD_SPEED];
}

void HistoryActor::SetGoodsAddSpeed(int iSet/* = -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_speed = iSet^iEncryptKey;
	}
	else
	{
		good_add_speed = (GetGoodsAddSpeed() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_SPEED] = iEncryptKey;
}

int HistoryActor::GetGoodsAddHp()
{
	return good_add_hp^m_EncryptKey[GOODS_ADD_HP];
}

void HistoryActor::SetGoodsAddHp(int iSet /*= -1*/, int iAdd/* = 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_hp = iSet^iEncryptKey;
	}
	else
	{
		good_add_hp = (GetGoodsAddHp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_HP] = iEncryptKey;
}

int HistoryActor::GetGoodsAddMp()
{
	return good_add_mp^m_EncryptKey[GOODS_ADD_MP];
}

void HistoryActor::SetGoodsAddMp(int iSet /*= -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		good_add_mp = iSet^iEncryptKey;
	}
	else
	{
		good_add_mp = (GetGoodsAddMp() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[GOODS_ADD_MP] = iEncryptKey;
}

map<int, int> HistoryActor::GetNetSuitData()
{
	map<int,int> mRet;
	GoodsItem* ptrGoodsItem = NULL;
	map<int, int> mSuitId;
	map<int, int> mGoodsId;

	for (int i = 0; i < 5; i++)
	{
		ptrGoodsItem = NULL;
		if (i == 0&&weapon.isNormalGood())
			ptrGoodsItem = weapon.getGoodAttr();
		else if (i == 1 && coat.isNormalGood())
			ptrGoodsItem = coat.getGoodAttr();
		else if (i == 2 && assist_good.isNormalGood())
			ptrGoodsItem = assist_good.getGoodAttr();
		else if (i == 3 && fashion.isNormalGood())
			ptrGoodsItem = fashion.getGoodAttr();
		else if (i == 4 && wap_fashion.isNormalGood())
			ptrGoodsItem = wap_fashion.getGoodAttr();

		if (!ptrGoodsItem) continue;

		mGoodsId[ptrGoodsItem->iGoodsId] = 1;

		for (map<int, int>::iterator mIt = ptrGoodsItem->mSuitList.begin(); mIt != ptrGoodsItem->mSuitList.end(); mIt++)
		{
			mSuitId[mIt->first] = 1;
		}
	}

	NetSuitDataPtr ptrNetSuitData = NULL;
	bool bOpenSuitEffect = false;
	for (map<int, int>::iterator mIt = mSuitId.begin(); mIt != mSuitId.end(); mIt++)
	{
		bOpenSuitEffect = false;
		if (trData->net_suit_data.count(mIt->first))
		{
			ptrNetSuitData = trData->net_suit_data[mIt->first];

			for (map<int, int>::iterator iIt = ptrNetSuitData->item_id.begin(); iIt != ptrNetSuitData->item_id.end(); iIt++)
			{
				if (iIt->first&&!mGoodsId.count(iIt->first))
				{
					bOpenSuitEffect = false;
					break;
				}
				else
					bOpenSuitEffect = true;
			}
			if (bOpenSuitEffect)
				mRet[mIt->first] = 1;
		}
	}

	return mRet;
}

map<int, HistoryActor*> HistoryActor::getAroundActor(int iAttackId, bool bSameCamp)
{
	map<int, HistoryActor*> mAroundActor;
	map<int, HistoryActor*> mTempMap;
	int iIndex = 0;
	if(relation == ACTOR_RELATION_ENEMY && bSameCamp || relation != ACTOR_RELATION_ENEMY && !bSameCamp)
	{
		for(int i = 0; i < history->battleEnemyNum; i++)
		{
			mTempMap[iIndex++] = history->battleEnemy[i];
		}
	}
	else
	{
		for(int i = 0; i < history->battleOwnNum; i++)
		{
			mTempMap[iIndex++] = history->battleOwn[i];
		}

		for(int i = 0; i < history->battleFriendNum; i++)
		{
			mTempMap[iIndex++] = history->battleFriend[i];
		}
	}

	iIndex = 0;
	for(UInt i = 0; i < mTempMap.size(); i++)
	{
		if(mTempMap[i]->good_effects[SHARE_DAMAGE])
		{
			Seid* ptrSeid = trData->m_mSeid[mTempMap[i]->good_effects[SHARE_DAMAGE]];
			iAttackId = ptrSeid->special_effe[1];
		}

		if(trData->hitArea[iAttackId].length >= 0)
		{
			for(int j = 0; j < trData->hitArea[iAttackId].length; j++)
			{
				int iPosX = mTempMap[i]->x + trData->hitArea[iAttackId].pointlist[j].x;
				int iPosY = mTempMap[i]->y + trData->hitArea[iAttackId].pointlist[j].y;
				if(!mTempMap[i]->hide && x == iPosX && y == iPosY && mTempMap[i]->actor_id != actor_id)
				{
					mAroundActor[iIndex++] = mTempMap[i];
				}
			}
		}
		else if(trData->hitArea[iAttackId].length == -1)
		{
			if(!mTempMap[i]->hide && mTempMap[i]->actor_id != actor_id)
			{
				mAroundActor[iIndex++] = mTempMap[i];
			}
		}
	}
	return mAroundActor;
}

map<int, HistoryActor*> HistoryActor::getAroundActor()
{
	map<int, HistoryActor*> mAroundActor;
	int iIndex = 0;
	for(int i = -1; i < 2; i++)
	{
		for(int j = -1; j < 2; j++)
		{
			if(!(i == 0 && j == 0))
			{
				HistoryActor* ptrActor = history->FindActorPosition(x + i, y + j);
				if (ptrActor)
				{
					mAroundActor[iIndex++] = ptrActor;
				}
			}
		}
	}
	return mAroundActor;
}

HistoryActor* HistoryActor::getActorByType(int iType)
{
	HistoryActor* ptrActor = nullptr;
	switch(iType)
	{
	case TYPE721:
	{
		map<int, HistoryActor*> mActor = getAroundActor();
		for (UInt i = 0; i < mActor.size(); i++)
		{
			if (isSameCamp(mActor[i]))
			{
				if (!ptrActor || ptrActor->GetCurHp() > mActor[i]->GetCurHp())
				{
					ptrActor = mActor[i];
				}
			}
		}
	}
	break;
	default:
		break;
	}
	return ptrActor;
}

int HistoryActor::getHitbackStep(int iCurX, int iCurY, int iHitX, int iHitY)
{
	int iStep = 0;
	if (iCurX == iHitX && iCurY == iHitY)
	{
		iStep = 0;
	}
	else if (iCurX == iHitX)
	{
		int iOffsetY = iHitY - iCurY;
		int iAdd = iOffsetY / abs(iOffsetY);

		HistoryActor* ptrOppo = nullptr;
		for(int iY = iCurY; iY != iHitY; iY += iAdd)
		{
			if(ptrOppo)
			{
				if (ptrOppo->needAssault(iCurX, iY, iCurX, iY + iAdd, this))
				{
					iStep += iAdd;
				}
				else break;
			}
			else
			{
				HistoryActor* ptrActor = history->FindActorPosition(iCurX, iY + iAdd);
				if(ptrActor)
				{
					if(!isSameCamp(ptrActor) && ptrActor->needAssault(iCurX, iY, iCurX, iY + iAdd, this))
					{
						iStep += iAdd;
						ptrOppo = ptrActor;
					}
					else break;
				}
				else
				{
					if(MapisMoveable(iCurX, iY + iAdd))
					{
						iStep += iAdd;
					}
					else break;
				}
			}
		}
	}
	else if (iCurY == iHitY)
	{
		int iOffsetX = iHitX - iCurX;
		int iAdd = iOffsetX / abs(iOffsetX);

		HistoryActor* ptrOppo = nullptr;
		for(int iX = iCurX; iX != iHitX; iX += iAdd)
		{
			if(ptrOppo)
			{
				if(ptrOppo->needAssault(iX, iCurY, iX + iAdd, iCurY, this))
				{
					iStep += iAdd;
				}
				else break;
			}
			else
			{
				HistoryActor* ptrActor = history->FindActorPosition(iX + iAdd, iCurY);
				if(ptrActor)
				{
					if(!isSameCamp(ptrActor) && ptrActor->needAssault(iX, iCurY, iX + iAdd, iCurY, this))
					{
						iStep += iAdd;
						ptrOppo = ptrActor;
					}
					else break;
				}
				else
				{
					if(MapisMoveable(iX + iAdd, iCurY))
					{
						iStep += iAdd;
					}
					else break;
				}
			}
		}
	}
	return iStep;
}

int HistoryActor::getDistance(HistoryActor* ptrActor)
{
	int iDisX = abs(ptrActor->x - x);
	int iDisY = abs(ptrActor->y - y);
	return iDisX >= iDisY ? iDisX : iDisY;
}

void HistoryActor::addSpecialEffect(int iSeidId)
{
	Seid* ptrSeid = trData->m_mSeid[iSeidId];

	if (trData->m_mSeidType.count(ptrSeid->type))
	{
		map<int, int> mSpeEffect;
		for(int i = 0; i < ptrSeid->special_effe_num; i++)
		{
			mSpeEffect[i] = ptrSeid->special_effe[i];
		}

		SeidType* ptrType = trData->m_mSeidType[ptrSeid->type];
		int iTypeId = ptrType->iTypeId;
		int iAddIndex = ptrType->iAddIndex - 1;
		if(m_mSpecialEffect.count(iTypeId))
		{
			switch(ptrType->iAddType)
			{
			case 1: //纯叠加
				m_mSpecialEffect[iTypeId][iAddIndex] += mSpeEffect[iAddIndex];
				break;
			case 2: //大值覆盖小值
			{
				if(mSpeEffect[iAddIndex] > m_mSpecialEffect[iTypeId][iAddIndex])
				{
					m_mSpecialEffect[iTypeId][iAddIndex] = mSpeEffect[iAddIndex];
				}
			}
				break;
			case 3: //覆盖
				m_mSpecialEffect[iTypeId] = mSpeEffect;
				break;
			case 4: //1属性位对比2数值位叠加
			{
				bool bIsExist = false;
				for(UInt i = 0; i < m_mSpecialEffect[iTypeId].size(); i += ptrType->iEffNum)
				{
					if(m_mSpecialEffect[iTypeId][i] == mSpeEffect[0])
					{
						m_mSpecialEffect[iTypeId][i + 1] += mSpeEffect[1];
						bIsExist = true;
						break;
					}
				}

				if (!bIsExist)
				{
					UInt iIndex = m_mSpecialEffect[iTypeId].size();
					for(UInt i = 0; i < mSpeEffect.size(); i++)
					{
						m_mSpecialEffect[iTypeId][i + iIndex] = mSpeEffect[i];
					}
				}
			}
				break;
			case 5: //1属性位对比2数值位大盖小
			{
				bool bIsExist = false;
				for(UInt i = 0; i < m_mSpecialEffect[iTypeId].size(); i += ptrType->iEffNum)
				{
					if(m_mSpecialEffect[iTypeId][i] == mSpeEffect[0])
					{
						if(mSpeEffect[1] > m_mSpecialEffect[iTypeId][i + 1])
						{
							m_mSpecialEffect[iTypeId][i + 1] = mSpeEffect[1];
						}
						bIsExist = true;
						break;
					}
				}

				if(!bIsExist)
				{
					UInt iIndex = m_mSpecialEffect[iTypeId].size();
					for(UInt i = 0; i < mSpeEffect.size(); i++)
					{
						m_mSpecialEffect[iTypeId][i + iIndex] = mSpeEffect[i];
					}
				}
			}
				break;
			case 6: //交叉类1数值位叠加2数值位大盖小
			{
				m_mSpecialEffect[iTypeId][0] += mSpeEffect[0];

				if (mSpeEffect[1] > m_mSpecialEffect[iTypeId][1])
				{
					m_mSpecialEffect[iTypeId][1] = mSpeEffect[1];
				}
			}
			break;
			case 7: //所有位不覆盖、不叠加、分开统计
			{
				bool bIsExist = false;
				for(UInt i = 0; i < m_mSpecialEffect[iTypeId].size(); i += ptrType->iEffNum)
				{
					bool bSame = true;
					for (UInt j = 0; j < mSpeEffect.size(); j++)
					{
						if (m_mSpecialEffect[iTypeId][i + j] != mSpeEffect[j])
						{
							bSame = false;
							break;
						}
					}

					if (bSame)
					{
						bIsExist = true;
						break;
					}
				}

				if(!bIsExist)
				{
					UInt iIndex = m_mSpecialEffect[iTypeId].size();
					for(UInt i = 0; i < mSpeEffect.size(); i++)
					{
						m_mSpecialEffect[iTypeId][i + iIndex] = mSpeEffect[i];
					}
				}
			}
			break;
			case 8: //1属性位对比2数值位大盖小3属性为叠加
			{
				bool bIsExist = false;
				for (UInt i = 0; i < m_mSpecialEffect[iTypeId].size(); i += ptrType->iEffNum)
				{
					if (m_mSpecialEffect[iTypeId][i] == mSpeEffect[0])
					{
						if (mSpeEffect[1] > m_mSpecialEffect[iTypeId][i + 1])
						{
							m_mSpecialEffect[iTypeId][i + 1] = mSpeEffect[1];
						}
						m_mSpecialEffect[iTypeId][i + 2] += mSpeEffect[2];
						bIsExist = true;
						break;
					}
				}

				if (!bIsExist)
				{
					UInt iIndex = m_mSpecialEffect[iTypeId].size();
					for (UInt i = 0; i < mSpeEffect.size(); i++)
					{
						m_mSpecialEffect[iTypeId][i + iIndex] = mSpeEffect[i];
					}
				}
			}
			break;
			case 9: //所有位独自叠加
			{
				for (UInt i = 0; i < mSpeEffect.size(); i++)
				{
					m_mSpecialEffect[iTypeId][i] += mSpeEffect[i];
				}
			}
			break;
			default:
				break;
			}
		}
		else
		{
			m_mSpecialEffect[iTypeId] = mSpeEffect;
		}

		switch(ptrSeid->type)
		{
		case TYPE015:
			SetGoodsAddSpeed(getSpecialInt(TYPE015));
			break;
		default:
			break;
		}
	}
}

void HistoryActor::clearSpecialEffect()
{
	m_mSpecialEffect.clear();
}

bool HistoryActor::isHadSpecial(int iTypeId)
{
	bool isHad = false;
	if(m_mSpecialEffect.count(iTypeId))
	{
		isHad = true;
	}
	return isHad;
}

int HistoryActor::getSpecialInt(int iTypeId)
{
	int iSpecial = 0;
	if (isHadSpecial(iTypeId))
	{
		iSpecial = m_mSpecialEffect[iTypeId][0];
	}
	return iSpecial;
}

map<int, int> HistoryActor::getSpecialMap(int iTypeId)
{
	map<int, int> mSpecial;
	if(isHadSpecial(iTypeId))
	{
		mSpecial = m_mSpecialEffect[iTypeId];
	}
	return mSpecial;
}

bool HistoryActor::SelfSkillLevelIsNull(int iSkill)
{
	if (mSkillItselfLevel.count(iSkill) > 0)
		return false;
	else
		return true;
}

int HistoryActor::GetSelfSkillLevel(int iSkill)
{
	if (mSkillItselfLevel.count(iSkill) > 0)
		return mSkillItselfLevel[iSkill][0] ^ mSkillItselfLevel[iSkill][1];
	else
		return 0;
}
void HistoryActor::SetSelfSkillLevel(int iSkill, int iSet, int iAdd)
{
	int iEncryptKey = rand();
	if (mSkillItselfLevel.count(iSkill) > 0)
	{
		if (iSet >= 0)
		{
			mSkillItselfLevel[iSkill][0] = iSet^iEncryptKey;
		}
		else
		{
			mSkillItselfLevel[iSkill][0] = (GetSelfSkillLevel(iSkill) + iAdd) ^ iEncryptKey;
		}
	}
	else
	{
		if (iSet >= 0)
		{
			mSkillItselfLevel[iSkill][0] = iSet^iEncryptKey;
		}
		else
		{
			mSkillItselfLevel[iSkill][0] = iAdd ^ iEncryptKey;
		}
	}

	mSkillItselfLevel[iSkill][1] = iEncryptKey;
}

void HistoryActor::ClearSelfSkillLevel()
{
	mSkillItselfLevel.clear();
}

map<int, int> HistoryActor::GetAllSelfSkillLevel()
{
	map<int, int> mRet;
	for (map<int, map<int, int> >::iterator eIt = mSkillItselfLevel.begin(); eIt != mSkillItselfLevel.end(); eIt++)
	{
		mRet[eIt->first] = eIt->second[0] ^ eIt->second[1];
	}
	return mRet;
}

bool HistoryActor::CurSkillLevelIsNull(int iSkill)
{
	return skill_level.count(iSkill) > 0 || m_mCopySkill.count(iSkill) > 0 || m_mSeidAddSkill.count(iSkill) > 0 ? false : true;
}

int HistoryActor::GetCurSkillLevel(int iSkill)
{
	int iLevel = 0;
	if (m_mCopySkill.count(iSkill) > 0)
	{
		iLevel = m_mCopySkill[iSkill]->iLevel ^ m_mCopySkill[iSkill]->iEncryptKey;
		if (skill_level.count(iSkill) > 0 && m_mCopySkill[iSkill]->iUsed)
		{
			iLevel = skill_level[iSkill][0] ^ skill_level[iSkill][1];
		}
	}
	else if (m_mSeidAddSkill.count(iSkill))
	{
		iLevel = GetActorLv();
	}
	else if (skill_level.count(iSkill) > 0)
	{
		iLevel = skill_level[iSkill][0] ^ skill_level[iSkill][1];
	}
	return iLevel;
}
void HistoryActor::SetCurSkillLevel(int iSkill, int iSet, int iAdd)
{
	int iEncryptKey = rand();
	if (skill_level.count(iSkill) > 0)
	{
		if (iSet >= 0)
		{
			skill_level[iSkill][0] = iSet^iEncryptKey;
		}
		else
		{
			skill_level[iSkill][0] = (GetCurSkillLevel(iSkill) + iAdd) ^ iEncryptKey;
		}
	}
	else
	{
		if (iSet >= 0)
		{
			skill_level[iSkill][0] = iSet^iEncryptKey;
		}
		else
		{
			skill_level[iSkill][0] = iAdd ^ iEncryptKey;
		}
	}

	skill_level[iSkill][1] = iEncryptKey;
}

void HistoryActor::ClearCurSkillLevel()
{
	skill_level.clear();
}

map<int, map<int, int> > HistoryActor::GetCurSkillLvList()
{
	return skill_level;
}

void HistoryActor::addExtraGrowEffect(int iType, int iNum)//1:hp 2:mp 3:pow 4;int 5:def 6:exp 7:fortune
{
	if (iType == 1)
		SetExtraGrow(ACTOR_ABILITY_HPUP,-1,iNum);
	else if(iType == 2)
		SetExtraGrow(ACTOR_ABILITY_MPUP, -1, iNum);
	else if (iType == 3)
		SetExtraGrow(ACTOR_ABILITY_POWER, -1, iNum);
	else if (iType == 4)
		SetExtraGrow(ACTOR_ABILITY_INTELLEGENCE, -1, iNum);
	else if (iType == 5)
		SetExtraGrow(ACTOR_ABILITY_DEFENCE, -1, iNum);
	else if (iType == 6)
		SetExtraGrow(ACTOR_ABILITY_EXPLOSION, -1, iNum);
	else if (iType == 7)
		SetExtraGrow(ACTOR_ABILITY_FORTUNE, -1, iNum);
}

bool HistoryActor::isSameCamp(HistoryActor* ptrActor)
{
	return (relation == ACTOR_RELATION_ENEMY && ptrActor->relation == ACTOR_RELATION_ENEMY) || (relation != ACTOR_RELATION_ENEMY && ptrActor->relation != ACTOR_RELATION_ENEMY);
}

int HistoryActor::GetExtraGrow(int iPower)
{
	if (mExtraGrow.count(iPower) > 0)
		return mExtraGrow[iPower][0] ^ mExtraGrow[iPower][1];
	else
		return 0;
}

void HistoryActor::SetExtraGrow(int iPower, int iSet, int iAdd)
{
	int iEncryptKey = rand();
	if (mExtraGrow.count(iPower) > 0)
	{
		if (iSet >= 0)
		{
			mExtraGrow[iPower][0] = iSet^iEncryptKey;
		}
		else
		{
			mExtraGrow[iPower][0] = (GetExtraGrow(iPower) + iAdd) ^ iEncryptKey;
		}
	}
	else
	{
		if (iSet >= 0)
		{
			mExtraGrow[iPower][0] = iSet^iEncryptKey;
		}
		else
		{
			mExtraGrow[iPower][0] = iAdd ^ iEncryptKey;
		}
	}

	mExtraGrow[iPower][1] = iEncryptKey;
}

float HistoryActor::getPracGrown(int iAttrType)
{
	int iValue = 0;
	if (m_mPracGrown.count(iAttrType) > 0)
	{
		iValue = m_mPracGrown[iAttrType][0] ^ m_mPracGrown[iAttrType][1];
	}
	return iValue / 10.0f;
}

void HistoryActor::setPracGrown(int iAttrType, float fValue, bool bIsAdd /*= false*/)
{
	float fOldValue = getPracGrown(iAttrType);
	if (bIsAdd)
	{
		fValue += fOldValue;
	}

	if (m_mPracGrown.count(iAttrType) <= 0)
	{
		m_mPracGrown[iAttrType][1] = rand() % 1000;
	}
	m_mPracGrown[iAttrType][0] = (int)(fValue * 10) ^ m_mPracGrown[iAttrType][1];

	if(abs(fValue - fOldValue) > 0.01f) //数值变化重新计算属性
	{
		InitoneBaseAbility(iAttrType);
	}
}

void HistoryActor::CopyActorPracGrown(HistoryActor* ptrActor)
{
	this->m_mPracGrown.clear();
	this->m_mPracGrown = ptrActor->m_mPracGrown;
}

void HistoryActor::updateStoneSuit()
{
	m_mStoneSeid.clear();

	map<int, int> mStoneId;
	mStoneId[0] = weapon.isNormalGood() ? weapon.getStoneID() : 0;
	mStoneId[1] = coat.isNormalGood() ? coat.getStoneID() : 0;
	mStoneId[2] = assist_good.isNormalGood() ? assist_good.getStoneID() : 0;

	map<int, int> mSuitId;
	int iIndex = 0;
	int iStoneIndex = getFmStoneIndex(mStoneId);
	if (iStoneIndex) //三件套
	{
		mSuitId[iIndex++] = iStoneIndex;
	}
	else
	{
		map<int, int> mStoneId1;
		mStoneId1[0] = mStoneId[0];
		mStoneId1[1] = mStoneId[1];
		int iStoneIndex1 = getFmStoneIndex(mStoneId1);

		if (iStoneIndex1) //前两件
		{
			mSuitId[iIndex++] = iStoneIndex1;
		}
		else
		{
			mSuitId[iIndex++] = getFmStoneIndex(mStoneId[0]);
		}

		map<int, int> mStoneId2;
		mStoneId2[0] = mStoneId[1];
		mStoneId2[1] = mStoneId[2];
		int iStoneIndex2 = getFmStoneIndex(mStoneId2);

		if(iStoneIndex2) //后两件
		{
			mSuitId[iIndex++] = iStoneIndex2;
		}
		else
		{
			mSuitId[iIndex++] = getFmStoneIndex(mStoneId[2]);
		}

		if (!iStoneIndex1 && !iStoneIndex2) //单三件
		{
			mSuitId[iIndex++] = getFmStoneIndex(mStoneId[1]);
		}
	}

	iIndex = 0;
	for (UInt i = 0; i < mSuitId.size(); i++)
	{
		if (mSuitId[i])
		{
			FmStoneSuit* ptrSuit = trData->m_mFmStone[mSuitId[i]];
			for(UInt j = 0; j < ptrSuit->mNorSeid.size(); j++)
			{
				m_mStoneSeid[iIndex++] = ptrSuit->mNorSeid[j];
			}

			for(UInt j = 0; j < ptrSuit->mHeroId.size(); j++)
			{
				if(ptrSuit->mHeroId[j] == actor_id + 1)
				{
					int iTIndex = 0,iTGemSize = m_mExclusiveGemEffect[1].size();
					for(size_t k = 0; k < ptrSuit->mHeroDel.size(); k++)
					{
						if(ptrSuit->mHeroDel[k])
							m_mExclusiveGemEffect[0][ptrSuit->mHeroDel[k]] = iTGemSize + k;
					}

					for (size_t k = 0; k < ptrSuit->mHeroAdd.size(); k++)
					{
						if(ptrSuit->mHeroAdd[k])
							m_mExclusiveGemEffect[1][iTGemSize++] = ptrSuit->mHeroAdd[k];
					}

					iTGemSize = m_mExclusiveGemEffect[2].size();
					for (size_t k = 0; k < ptrSuit->mHeroExcl.size(); k++)
					{
						if(ptrSuit->mHeroExcl[k])
							m_mExclusiveGemEffect[2][iTGemSize++] = ptrSuit->mHeroExcl[k];
					}

					break;
				}
			}
		}
	}
}

void HistoryActor::SetFashionData()
{
	int iFashion1 = 0;
	int iFashion2 = 0;
	int iGender = mInitFashion[1];
	if (iGender < 1) iGender = trData->actors[actor_id].iGender;
	if (fashion.isNormalGood())
	{
		iFashion1 = fashion.good_id;
	}
	if (wap_fashion.isNormalGood())
	{
		iFashion2 = wap_fashion.good_id;
	}

	if (iFashion1 > 0 && iFashion2 > 0)
	{
		mFashion[0] = 1;
		mFashion[1] = iGender;
		mFashion[2] = iFashion1;
		mFashion[3] = 100;
		mFashion[4] = iFashion2;
	}
	else if (mInitFashion[0] == 1)
	{
		mFashion[0] = 1;
		if (iFashion1 > 0 && !iFashion2)
		{
			mFashion[1] = iGender;
			mFashion[2] = iFashion1;
			mFashion[3] = mInitFashion[3];
			mFashion[4] = mInitFashion[4];
		}
		else if (!iFashion1 && iFashion2 > 0)
		{
			mFashion[1] = mInitFashion[1];
			mFashion[2] = mInitFashion[2];
			mFashion[3] = 100;
			mFashion[4] = iFashion2;
		}
		else
		{
			mFashion[1] = mInitFashion[1];
			mFashion[2] = mInitFashion[2];
			mFashion[3] = mInitFashion[3];
			mFashion[4] = mInitFashion[4];
		}

	}
	else if(mFashion[0] == 1)
	{
		mFashion.clear();
	}
	if(mFashion[0])
		history->LoadFashionSImg(this);
}

map<int,int> HistoryActor::GetFashionSeid()
{
	map<int, int> mRet;
	GoodsItem* ptrGoodsItem = NULL;
	map<int, int> mSuitId;
	map<int, int> mGoodsId;
	int mRetIndex = 0;

	for (int i = 0; i < 2; i++)
	{
		ptrGoodsItem = NULL;
		if (i == 0 && fashion.isNormalGood())
			ptrGoodsItem = fashion.getGoodAttr();
		else if (i == 1 && wap_fashion.isNormalGood())
			ptrGoodsItem = wap_fashion.getGoodAttr();

		if (!ptrGoodsItem) continue;

		mGoodsId[ptrGoodsItem->iGoodsId] = 1;

		for (map<int, int>::iterator mIt = ptrGoodsItem->mSuitList.begin(); mIt != ptrGoodsItem->mSuitList.end(); mIt++)
		{
			mSuitId[mIt->first] = 1;
		}

		for (map<int, int>::iterator mIt = ptrGoodsItem->mSeid.begin(); mIt != ptrGoodsItem->mSeid.end(); mIt++)
		{
			if(trData->m_mSeid.count(mIt->second))
			{
				mRet[mRetIndex++] = mIt->second;
			}
		}
	}

	NetSuitDataPtr ptrNetSuitData = NULL;
	bool bOpenSuitEffect = false;
	for (map<int, int>::iterator mIt = mSuitId.begin(); mIt != mSuitId.end(); mIt++)
	{
		bOpenSuitEffect = false;
		if (trData->net_suit_data.count(mIt->first))
		{
			ptrNetSuitData = trData->net_suit_data[mIt->first];

			for (map<int, int>::iterator iIt = ptrNetSuitData->item_id.begin(); iIt != ptrNetSuitData->item_id.end(); iIt++)
			{
				if (iIt->first && !mGoodsId.count(iIt->first))
				{
					bOpenSuitEffect = false;
					break;
				}
				else
					bOpenSuitEffect = true;
			}
			if (bOpenSuitEffect)
			{
				for (map<int, int>::iterator mIt = ptrNetSuitData->seid1.begin(); mIt != ptrNetSuitData->seid1.end(); mIt++)
				{
					if(trData->m_mSeid.count(mIt->first))
					{
						mRet[mRetIndex++] = mIt->first;
					}
				}
				if (ptrNetSuitData->hid.count(actor_id + 1))
				{
					for (map<int, int>::iterator mIt = ptrNetSuitData->seid2.begin(); mIt != ptrNetSuitData->seid2.end(); mIt++)
					{
						if(trData->m_mSeid.count(mIt->first))
						{
							mRet[mRetIndex++] = mIt->first;
						}
					}
				}
			}
		}
	}

	return mRet;

}

void HistoryActor::SetSoulBox(int i)
{
	m_iSoulBox = i;
}

int HistoryActor::GetSoulBox()
{
	return m_iSoulBox;
}

int HistoryActor::GetSeidIsOpen(int iSeid)
{
	int iRet = 0;
	SeidPtr ptrSeid = trData->m_mSeid[iSeid];

	if (ptrSeid->mOpenCondition[0] == 1 && actor_quality >= ptrSeid->mOpenCondition[1])
	{
		iRet = 1;
	}

	return iRet;
}

void HistoryActor::initCopySkill()
{
	clearCopySkill();
	if (isHadSpecial(TYPE730))
	{
		int iSkillType = getSpecialInt(TYPE730);
		map<int, int> mActorId;
		int iIndex = 0;
		if(this->relation == ACTOR_RELATION_ENEMY)
		{
			for(int i = 0; i < history->battleOwnNum; i++)
			{
				mActorId[iIndex++] = history->battleOwn[i]->actor_id;
			}

			for(int i = 0; i < history->battleFriendNum; i++)
			{
				mActorId[iIndex++] = history->battleFriend[i]->actor_id;
			}
		}
		else
		{
			for(int i = 0; i < history->battleEnemyNum; i++)
			{
				mActorId[iIndex++] = history->battleEnemy[i]->actor_id;
			}
		}

		map<int, int> mSkillId;
		for(UInt i = 0; i < mActorId.size(); i++)
		{
			int iSkillId = trData->actors[mActorId[i]].skill;
			if(trData->net_hero_skill.count(iSkillId))
			{
				NetHeroSkill* ptrSkill = trData->net_hero_skill[iSkillId];
				for(map<int, int>::iterator it = ptrSkill->skill.begin(); it != ptrSkill->skill.end(); it++)
				{
					SeidType* ptrType = trData->m_mSeidType[TYPE730];
					SkillAttr* pCurSkill = &trData->skills[it->first - 1];
					if (!pCurSkill->iNoCopy)
					{
						if(iSkillType == ptrType->iClass || iSkillType == pCurSkill->type)
						{
							mSkillId[it->first] = it->second;
						}
					}
				}
			}
		}

		for (map<int, int>::iterator it = mSkillId.begin(); it != mSkillId.end(); it++)
		{
			int iSkillId = it->first;
			int iNeedLv = it->second;
			if (GetActorLv() >= iNeedLv)
			{
				CopySkill* ptrCSkill = new CopySkill();
				ptrCSkill->iSkillId = iSkillId;
				ptrCSkill->iEncryptKey = rand();
				int iLevel = GetActorLv() - iNeedLv + 1;
				ptrCSkill->iLevel = iLevel ^ ptrCSkill->iEncryptKey;
				ptrCSkill->iUsed = 0;
				m_mCopySkill[iSkillId] = ptrCSkill;
			}
		}
	}

	if(isHadSpecial(TYPE731))
	{
		map<int, int> mSpecial = getSpecialMap(TYPE731);
		good_shield_range->addElement(new HistoryActor::RangeEffect(mSpecial[1], 20, 100));
		m_iShieldNum = mSpecial[2];
	}
}

void HistoryActor::clearCopySkill()
{
	for (map<int, CopySkill*>::iterator it = m_mCopySkill.begin(); it != m_mCopySkill.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mCopySkill.clear();
}

void HistoryActor::updateCopySkill(int iSkillId)
{
	m_mCopySkill[iSkillId]->iUsed = 1;

	bool bAllUsed = true;
	for(map<int, CopySkill*>::iterator it = m_mCopySkill.begin(); it != m_mCopySkill.end(); it++)
	{
		CopySkill* ptrCSkill = it->second;
		if (ptrCSkill->iUsed == 0)
		{
			bAllUsed = false;
			break;
		}
	}

	if (bAllUsed) //全部用过之后则清零
	{
		for(map<int, CopySkill*>::iterator it = m_mCopySkill.begin(); it != m_mCopySkill.end(); it++)
		{
			it->second->iUsed = 0;
		}
	}
}

void HistoryActor::updateRoundState()
{
	style = ACTOR_STATE_NORMAL;
	action_finished = 0;
	iTwiceAction = 0;
	iActionNum = -1;
	m_iShareTimes = 0;
	m_iRestSpeed = 0;

	if(isHadSpecial(TYPE731)) //庇护光环数据刷新
	{
		map<int, int> mSpecial = getSpecialMap(TYPE731);
		if (GetCurHp() >= getTotalAbility(TOTAL_HP) * mSpecial[0] / 100)
		{
			if(good_shield_range->size() == 0)
			{
				good_shield_range->addElement(new HistoryActor::RangeEffect(mSpecial[1], 20, 100));
			}
			m_iShieldNum = mSpecial[2];
		}
		else
		{
			good_shield_range->removeAllElements();
			m_iShieldNum = 0;
		}
	}
}

void HistoryActor::GetSeidAddSkill()
{
	if (isHadSpecial(TYPE754))
	{
		map<int, int> mSpecial = getSpecialMap(TYPE754);

		SeidType* ptrType = trData->m_mSeidType[TYPE754];

		for (UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(CurSkillLevelIsNull(mSpecial[i]))
				m_mSeidAddSkill[mSpecial[i]] = 1;
		}
	}
}

int HistoryActor::getInductNum()
{
	int ret = 0;
	map<int, int> mTHeroSeid = getDicHeroSeid();
	for (size_t i = 0; i < mTHeroSeid.size(); i++)
	{
		int iSeid = mTHeroSeid[i];
		if (trData->m_mSeid[iSeid]->type == 108 || trData->m_mSeid[iSeid]->type == 29)
		{
			if (ret < trData->m_mSeid[iSeid]->special_effe[0])
			{
				ret = trData->m_mSeid[iSeid]->special_effe[0];
			}
		}
	}
	return ret;
}

void HistoryActor::setAddAbilityGoods(map<int, int> mmGoodsData)
{
	m_mAddAbilityGoods.clear();
	m_mAddAbilityGoods = mmGoodsData;
}
void HistoryActor::setAddAbilityGoods(int iGoodsId, int iCount)
{
	m_mAddAbilityGoods[iGoodsId] = iCount;
}

map<int, int> HistoryActor::getDicHeroSeid(bool bOlnyShow)
{
	map<int, int> mRet;

// 	if (this->set_hero_seid.size() > 0)
// 	{
// 		for (int i = 0; i < this->set_hero_seid.size(); i++)
// 		{
// 			
// 		}
// 	}
// 	else 
	int iTSeid = -1,iTIndex = 0;
	if (this->actor_id < trData->actor_num)
	{
		for (int i = 0; i < trData->actors[this->actor_id].seid_num; i++)
		{
			iTSeid = trData->actors[this->actor_id].seid[i];
			if (!m_mExclusiveGemEffect[0].count(iTSeid))
			{
				mRet[iTIndex++] = iTSeid;
			}
			else if(m_mExclusiveGemEffect[0][iTSeid] < (int)m_mExclusiveGemEffect[1].size())
			{
				mRet[iTIndex++] = m_mExclusiveGemEffect[1][m_mExclusiveGemEffect[0][iTSeid]];
			}
		}

		if (!bOlnyShow)
		{
			for (size_t i = 0; i < m_mExclusiveGemEffect[2].size(); i++)
			{
				mRet[iTIndex++] = m_mExclusiveGemEffect[2][i];
			}
		}
	}

	return mRet;
}

map<int, map<int, int> > HistoryActor::getExclusiveGemEffect()
{
	return m_mExclusiveGemEffect;
}

void HistoryActor::updateFashion(Sprite* spImage, Sprite* spWeapon, int iZOrder /*= 0*/, int iDirection /*= DIRECT_DW*/)
{
	string strImage = "";
	string strWeapon = "";

	if (mFashion[0] <= 0)
	{
		strImage = getActorSImgPath("mov", 0);
	}
	else
	{
		strImage = StringUtils::format("img/Nunits/%d_%d_mov.png", mFashion[1], mFashion[2]);
		strWeapon = StringUtils::format("img/Nunits/%d_%d_mov.png", mFashion[3], mFashion[4]);
	}

	int iIndex = 0;
	bool bIsFlipX = false;
	switch (iDirection)
	{
	case DIRECT_UP:
		iIndex = 7;
		break;
	case DIRECT_RT:
		iIndex = 8;
		bIsFlipX = true;
		break;
	case DIRECT_DW:
		iIndex = 6;
		break;
	case DIRECT_LT:
		iIndex = 8;
		break;
	default:
		break;
	}

	//形象
	Texture2D* pTexture1 = GetSpriteTexture(strImage);
	int iWidth1 = pTexture1->getContentSize().width;
	spImage->setTexture(pTexture1);
	spImage->setTextureRect(Rect(0, iIndex * iWidth1, iWidth1, iWidth1));

	//武器
	if (strWeapon == "")
	{
		spWeapon->setVisible(false);
	}
	else
	{
		spWeapon->setVisible(true);
		Texture2D* pTexture2 = GetSpriteTexture(strWeapon);
		int iWidth2 = pTexture2->getContentSize().width;
		spWeapon->setTexture(pTexture2);
		spWeapon->setTextureRect(Rect(0, iIndex * iWidth2, iWidth2, iWidth2));

		if (trData->net_hero_dress[mFashion[4]]->mMov[iIndex])
		{
			spImage->setLocalZOrder(iZOrder);
			spWeapon->setLocalZOrder(iZOrder + 1);
		}
		else
		{
			spImage->setLocalZOrder(iZOrder + 1);
			spWeapon->setLocalZOrder(iZOrder);
		}
	}
	spImage->setFlippedX(bIsFlipX);
	spWeapon->setFlippedX(bIsFlipX);
}
