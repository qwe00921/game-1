#include "Common.h"
#include "main.h"
#include "BattleAI.h"
#include "BattleAIExt.h"

ActionSingleGainPtr CalcGoodSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, GoodsItem* good, ActionGainPtr totalgain)
{
	ActionSingleGainPtr gain = new ActionSingleGain;
	gain->oppo_actor = oppo_actor;
	gain->probability = 100;
	gain->state_mustHit = 1;

	switch(good->iUeid) 
	{
	case 0://毒药 
		break;
 	case 1:// 恢复HP(整数)
	{
		gain->gain_hp = good->iPower;
		if (oppo_actor->GetCurHp() + gain->gain_hp > oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp())
		{
			gain->gain_hp = oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp() - oppo_actor->GetCurHp();
		}
	}
 		break;
	case 2:// 恢复MP(整数)
	{
		gain->gain_mp = good->iPower;
		if (oppo_actor->GetCurMp() + gain->gain_mp > oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp())
		{
			gain->gain_mp = oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp() - oppo_actor->GetCurMp();
		}
	}
		break;
	case 3:// 治疗混乱
		{
			gain->state_puzzle = 1;
		}
		break;
	case 4:// 治疗中毒
		{
			gain->state_poison = 1;
		}
		break;
	case 5:// 治疗麻痹
		{
			gain->state_palsy = 1;
		}
		break;
	case 6:// 治疗禁咒
		{
			gain->state_stun = 1;
		}
		break;
	case 7:// 治疗所有异常
		{
			gain->state_puzzle = 1;
			gain->state_poison = 1;
			gain->state_palsy = 1;
			gain->state_stun = 1;
			gain->state_blind = 1;
			gain->state_obfuscation = 1;
            gain->state_weaked = 1;
			gain->state_loseblood = 1;
		}
		break;
	case 8:// 武力上升(整数)
		{
			gain->state_power = 1;
		}
		break;
	case 9:// 智力上升(整数)
		{
			gain->state_intellegence = 1;
		}
		break;
	case 10:// 统帅上升(整数)
		{
			gain->state_defence = 1;
		}
		break;
	case 11:// 敏捷上升(整数)
		{
			gain->state_explosion = 1;
		}
		break;
	case 12:// 士气上升(整数)
		{
			gain->state_fortune = 1;
		}
		break;
	case 13:// 等级上升
		break;
	case 14:// 兵种上升
		break;
	case 15://治疗昏迷
		gain->state_obfuscation = 1;
		break;
	case 16://治疗失明
		gain->state_blind = 1;
		break;
	case 17://移动上升
		gain->state_speed = 1;
		break;
	case 18://攻击道具
		break;
    case 25://武力、灵力、体力、灵巧、运气均提高，且恢复所有的HP和MP
    {
        gain->state_power = 1;
        gain->state_intellegence = 1;
        gain->state_defence = 1;
        gain->state_explosion = 1;
        gain->state_fortune = 1;
		gain->gain_hp = oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp() - oppo_actor->GetCurHp();
		gain->gain_mp = oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp() - oppo_actor->GetCurMp();
    }
        break;
	case 28://改变兵种
		break;
	case 29://攻击力上升
		break;
	case 30://精神力上升
		break;
	case 31://防御力上升
		break;
	case 32://爆发力上升
		break;
	case 33://士气值上升
		break;
	case 34://被动消耗品
		break;
	case 36://锻造消耗品
		break;
	case 37://改变形象
		break;
	case 38://分配点消耗品
		break;
	case 39://学会特技
		break;
	case 40://学会法术
		break;
	case 41://购买武将，可重命名
		break;
	case 42://改名卡
		break;
	case 43://增加最大回合数
		break;
	case 44:
	{
		gain->state_power = 1;
	}
		break;
	case 45:
	{
		gain->state_intellegence = 1;
	}
		break;
	case 46:
	{
		gain->state_defence = 1;
	}
		break;
	case 47:
	{
		gain->state_explosion = 1;
	}
		break;
	case 48:
	{
		gain->state_fortune = 1;
	}
		break;
	}

	if (oppo_actor->isHadSpecial(TYPE723))
	{
		gain->gain_hp += gain->gain_hp * oppo_actor->getSpecialInt(TYPE723) / 100;
	}

	BattleControler::getBattleActor(oppo_actor->actor_id)->hp_gain = gain->gain_hp;
	BattleControler::getBattleActor(oppo_actor->actor_id)->mp_gain = gain->gain_mp;
	
	return gain;
}


ActionSingleGainPtr CalcAttackSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, ActionGainPtr totalgain, bool isVisible, int addprobability, int iPierceNum)
{
	// 计算攻击伤害 //地图属性需要加入
	ActionSingleGainPtr gain = new ActionSingleGain;

	bool bFirstAttack = totalgain->gains->size() ? false : true; //判定群伤是否是第一个目标
	int iCurLandsGain = cur_actor->getLandAbilityGain(history->map_array[totalgain->point.y][totalgain->point.x]);
	int iOppoLandsGain = oppo_actor->getLandAbilityGain(history->map_array[oppo_actor->y][oppo_actor->x]);

	if(cur_actor->good_effects[REDUCE_LAND] && iOppoLandsGain > cur_actor->good_effects[REDUCE_LAND])//攻击时固化敌军地形
	{
		iOppoLandsGain = cur_actor->good_effects[REDUCE_LAND];
	}

	int real_x = -1,real_y = -1;
	if (BattleControler::auto_run != 0)
	{
		real_x = totalgain->point.x;
		real_y = totalgain->point.y;
	}

	int iAddPow = 0;
	int iRealIntel = cur_actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE, true);
	if(cur_actor->good_effects[ADD_ATTACK_ELEM])
	{
		iAddPow = iRealIntel * cur_actor->good_effects[ADD_ATTACK_ELEM] / 100;
	}
	int cur_power = (cur_actor->getRealAbility(ACTOR_ABILITY_POWER, true, real_x, real_y) + iAddPow) * (iCurLandsGain - oppo_actor->good_effects[LGNORE_ABILITY]) / 100;
	int cur_intellegence = iRealIntel * iCurLandsGain / 100;
	int cur_explosion = cur_actor->getRealAbility(ACTOR_ABILITY_EXPLOSION, true) * iCurLandsGain / 100;

	int iAbilityId = ACTOR_ABILITY_DEFENCE;
	if (cur_actor->isHadSpecial(TYPE742)) //弱点攻击，视敌人五围最低一项为防御计算伤害
	{
		iAbilityId = oppo_actor->getMinAbilityId();
	}
	int iRduceDef = 0,iAddDef = 0;
	if (cur_actor->isHadSpecial(TYPE750))
		iRduceDef = cur_actor->getSpecialInt(TYPE750);
	if (oppo_actor->isHadSpecial(TYPE766))
	{
		int iOppoRealIntel = oppo_actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE, true);
		iAddDef = oppo_actor->getSpecialInt(TYPE766)*iOppoRealIntel / 100;
	}
	int oppo_defence = (oppo_actor->getRealAbility(iAbilityId, true)+ iAddDef) * (iOppoLandsGain - cur_actor->good_effects[LGNORE_ABILITY]- iRduceDef)/ 100;
	int oppo_explosion = oppo_actor->getRealAbility(ACTOR_ABILITY_EXPLOSION, true) * iOppoLandsGain / 100;

	gain->oppo_actor = oppo_actor;

	int iHurtPercent = 100; //伤害加成的系数，起始100%
	int base_probability = 0;

	OccupationType* pCurOccu = &trData->occu_types[cur_actor->GetActorOccuType()];
	OccupationType* pOppoOccu = &trData->occu_types[oppo_actor->GetActorOccuType()];
	// //////////////////////////////////////////////////////////////////////////////////
	// 兵种相克加成
	iHurtPercent += GetJobPKHartnum(pCurOccu->occupation_type_id, pOppoOccu->occupation_type_id, false) - 100;

	//降低远距离攻击伤害
	if(pCurOccu->job_type == 922)
	{
		iHurtPercent -= oppo_actor->good_reduce_far_hit;
	}

	int iOppoCid = pOppoOccu->occupation_type_id;
	if(iOppoCid == 7 || iOppoCid == 8 || iOppoCid == 9) //骑马攻击
	{
		iHurtPercent += cur_actor->getSpecialInt(TYPE028);
	}

	if(cur_actor->isHadSpecial(TYPE301)) //克制兵种输出
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE301);
		SeidType* ptrType = trData->m_mSeidType[TYPE301];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == oppo_actor->getActorJobType())
			{
				iHurtPercent += mSpecial[i + 1];
			}
		}
	}

	if(oppo_actor->isHadSpecial(TYPE302)) //克制兵种防御
	{
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE302);
		SeidType* ptrType = trData->m_mSeidType[TYPE302];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == cur_actor->getActorJobType())
			{
				iHurtPercent -= mSpecial[i + 1];
			}
		}
	}
	
	// 骑马攻击使用dic_jobclass兵种进行克制，并允许一次指定多个。
	if (cur_actor->good_effects[34])
	{
		int num = trData->m_mSeid[cur_actor->good_effects[34]]->special_effe_num;
		for (int i = 1; i < num; i++)
		{
			if (trData->m_mSeid[cur_actor->good_effects[34]]->special_effe[i] == oppo_actor->history_occupation_id)
			{
				iHurtPercent += trData->m_mSeid[cur_actor->good_effects[34]]->special_effe[0];
				break;
			}
		}
	}

	if (cur_actor->good_effects[ARBALESTER])
	{
		int iStepNum = (int)abs(cur_actor->x - oppo_actor->x) + (int)abs(cur_actor->y - oppo_actor->y);
		iHurtPercent += iStepNum*cur_actor->good_effects[ARBALESTER];
	}
	if (oppo_actor->good_effects[DECREASES_DAMA])
	{
		if ((int)abs(cur_actor->x - oppo_actor->x) < 2 && (int)abs(cur_actor->y - oppo_actor->y) < 2)
			iHurtPercent -= oppo_actor->good_effects[DECREASES_DAMA] / 2;
		else
			iHurtPercent -= oppo_actor->good_effects[DECREASES_DAMA];
	}

    if (cur_actor->good_mp_attack) { //MP类型的攻击，根据当前MP值，按百分比增加攻击力
		cur_power += cur_actor->good_mp_attack * cur_actor->GetBaseMp() / 100;
    }

	if (cur_actor->good_effects[1])
	{
		cur_power = cur_intellegence;
	}

	if (cur_actor->getOccuPowerType() == 2)
	{
		cur_power += cur_intellegence/10;
	}

	//TYPE 600, 破甲
	if(cur_actor->isHadSpecial(TYPE600) && oppo_actor->coat.isNormalGood())
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE600);
		SeidType* ptrType = trData->m_mSeidType[TYPE600];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == oppo_actor->coat.getItId())
			{
				iHurtPercent += mSpecial[i + 1];
			}
		}
	}

	//TYPE 700, 攻守兼备
	iHurtPercent += cur_actor->getSpecialInt(TYPE700);

	//TYPE701, 背击伤害加成
	if (cur_actor->isHadSpecial(TYPE701) && cur_actor->isBackAttack(oppo_actor))
	{
		iHurtPercent += cur_actor->getSpecialInt(TYPE701);
	}

	//TYPE705, 受到攻击会被击退
	if(oppo_actor->isHadSpecial(TYPE705)&&!cur_actor->good_effects[13]&&!cur_actor->good_effects[TWOORDER_SPIKE])
	{
		int iMoveX = cur_actor->x, iMoveY = cur_actor->y;
		if(BattleControler::auto_run)
		{
			iMoveX = totalgain->point.x;
			iMoveY = totalgain->point.y;
		}
		if(oppo_actor->needAssault(iMoveX, iMoveY, cur_actor))
		{
			gain->actor_knockback = 1;
			iHurtPercent -= oppo_actor->getSpecialInt(TYPE705);
		}
	}

	gain->gain_hp = -(cur_actor->GetActorLv() + 25 + (cur_power - oppo_defence/2) / 2) * iHurtPercent / 100;

	if (cur_actor->isHadSpecial(TYPE702)) //破灵特技，伤害加成
	{
		gain->gain_hp += -(oppo_defence * cur_actor->getSpecialInt(TYPE702) / 100);
	}
	
	if (gain->gain_hp >= 0)
		gain->gain_hp = -1;
	if (cur_actor->good_hit_fixharm)
	{
		if (cur_actor->weapon.isNormalGood())
			gain->gain_hp = -cur_actor->good_hit_fixharm*cur_actor->weapon.GetRealLevel() - cur_actor->good_hit_fixharm_begin;
	}

	int effect_addition_gain_hp = 0;
	if ((cur_actor->isHadSpecial(TYPE381)||cur_actor->state_emergency_command) && BattleActor::moveList != NULL) //冲锋攻击
	{
		int move_point = 0;
		if (BattleControler::auto_run == 0)
		{
			move_point = BattleActor::moveList->elementAt(BattleActor::move_index)->path->size();
		}
		else
		{
			MovePathPtr movePoint;
			int iMoveIndex = -1;
			for (int i = 0; i < BattleActor::moveList->size(); i++)
			{
				movePoint = BattleActor::moveList->elementAt(i);
				if (totalgain->point.x == movePoint->point.x&&totalgain->point.y == movePoint->point.y)
				{
					iMoveIndex = i; 
					break;
				}
			}
			if(iMoveIndex>-1)
				move_point = BattleActor::moveList->elementAt(iMoveIndex)->path->size();
		}

		int iPre = 0;
		if (cur_actor->state_emergency_command)
			iPre = cur_actor->m_mActorBuff[ACTOR_BUFF_EMERGENCYCOMMAND]->iPercent;
		else
			iPre = cur_actor->getSpecialInt(TYPE381);

		if(move_point > 1)
		{
			effect_addition_gain_hp += move_point * iPre * gain->gain_hp / 100;
		}
	}

	////疾风攻击
	if (cur_actor->good_effects[SPEED_ATTACK])
	{
		int temp_speed = 0;
		if (oppo_actor->state_speed)
		{
			temp_speed = oppo_actor->GetActorSpeed(2);
		}
		if ((trData->occupations[oppo_actor->occupation_id].base_speed + oppo_actor->GetGoodsAddSpeed() + temp_speed) < 14)
		{
			effect_addition_gain_hp += cur_actor->good_effects[SPEED_ATTACK] * (14 - trData->occupations[oppo_actor->occupation_id].base_speed - oppo_actor->GetGoodsAddSpeed() - temp_speed) * gain->gain_hp / 100;
		}
	}

	//仇恨攻击
	if (cur_actor->isHadSpecial(TYPE383) && cur_actor->buff_hate_attack && ((cur_actor->relation != 2 && history->curActionSide != 2) || (cur_actor->relation == 2 && history->curActionSide == 2)))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE383);
		effect_addition_gain_hp += mSpecial[1] * cur_actor->buff_hate_attack * gain->gain_hp / 100;
	}

	////激斗攻击
	if (cur_actor->good_effects[FIGHT_ATTACK] && cur_actor->weapon.isNormalGood())
	{
		effect_addition_gain_hp += -(cur_actor->good_effects[FIGHT_ATTACK] + cur_actor->weapon.GetRealLevel()) * cur_actor->RangeOfNumbers();
	}
	////火舞焰翔
	if (cur_actor->good_effects[HIT_INJURY] > 0)
	{
		effect_addition_gain_hp += cur_actor->good_effects[HIT_INJURY] * gain->gain_hp / 100;
	}
	if (oppo_actor->good_effects[HIT_INJURY] < 0)
	{
		effect_addition_gain_hp += oppo_actor->good_effects[HIT_INJURY] * gain->gain_hp / 100;
	}

	//闷击
	if (cur_actor->isHadSpecial(TYPE714) && iPierceNum == 1)
	{
		effect_addition_gain_hp += cur_actor->getSpecialInt(TYPE714) * gain->gain_hp / 100;
	}

	//TYPE716
	if (cur_actor->isHadSpecial(TYPE716))
	{
		map<int, int> mSeidData = cur_actor->getSpecialMap(TYPE716);
		int iPNum = (mSeidData[1] - oppo_actor->GetActorSpeed())*mSeidData[0];
		if (iPNum > 0)
			effect_addition_gain_hp += iPNum * gain->gain_hp / 100;
	}

	//726
	if (cur_actor->isHadSpecial(TYPE726) && cur_actor->mSeidAddGain[TYPE726] > 0)
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE726);
		effect_addition_gain_hp += gain->gain_hp * mSpecial[0] * cur_actor->mSeidAddGain[TYPE726] / 100;
	}

	//
	gain->gain_hp += effect_addition_gain_hp;
	// 计算命中率
	if (cur_explosion > oppo_explosion)
		base_probability = (cur_explosion * 10 / oppo_explosion + 90);
	else
		base_probability = (cur_explosion * 60 / oppo_explosion + 40);

	base_probability = base_probability *(100 + cur_actor->good_hit_probability - oppo_actor->good_total_defence - oppo_actor->good_hit_defence) / 100 - oppo_actor->getSpecialInt(TYPE753);
	addprobability += addprobability;

	if (BattleActor::moveList != NULL&&BattleActor::move_index<=0) //不移动加命中
	{
		base_probability += cur_actor->getSpecialInt(TYPE397);
	}

	if(pCurOccu->job_type == 922 && oppo_actor->good_hold_far_hit)
	{
		base_probability = 0;
	}

	if (base_probability > 100 || oppo_actor->state_puzzle || oppo_actor->state_blind || oppo_actor->state_obfuscation || cur_actor->isHadSpecial(TYPE751)) {
		base_probability = 100;
	}
	else if (cur_actor->state_blind)
	{
		gain->gain_hp = gain->gain_hp/2;
	}

	gain->probability = base_probability;

	//昏迷攻击
	if ((rand() % 100) < cur_actor->good_effects[14] && oppo_actor->state_obfuscation == 0) {
		if ((oppo_actor->good_effects[12] != 936) && (oppo_actor->good_effects[12] != 939) && (rand() % 100) >= oppo_actor->getSpecialInt(TYPE722))
		{
			gain->state_obfuscation = -1;
		}
	}
	else if (oppo_actor->state_obfuscation)//解除昏迷攻击
	{
		gain->state_obfuscation = 1;
	}
	//致盲攻击
	if ((rand() % 100) < cur_actor->getSpecialInt(TYPE150) && oppo_actor->state_blind == 0) {
		if ((oppo_actor->good_effects[12] != 931) && (oppo_actor->good_effects[12] != 939)) //策划表中属性id
		{
			gain->state_blind = -1;
		}
	}

	// UUCHAR good_puzzle_hit; //混乱攻击机率
	if ((rand() % 100)<cur_actor->good_puzzle_hit && oppo_actor->state_puzzle == 0) {
		if ((oppo_actor->good_effects[12] != 932) && (oppo_actor->good_effects[12] != 939))
		{
			gain->state_puzzle = -1;
		}
	}
	
	// UUCHAR good_poison_hit; //中毒攻击机率
	if ((rand() % 100) < cur_actor->good_poison_hit && oppo_actor->state_poison == 0) {
		if ((oppo_actor->good_effects[12] != 933) && (oppo_actor->good_effects[12] != 939))
		{
			gain->state_poison = -1;
		}
	}
	// UUCHAR good_palsy_hit; //麻痹攻击机率
	if ((rand() % 100) < cur_actor->good_palsy_hit && oppo_actor->state_palsy == 0) {
		if ((oppo_actor->good_effects[12] != 934) && (oppo_actor->good_effects[12] != 939))
		{
			gain->state_palsy = -1;
		}
	}
	// UUCHAR good_stun_hit; //封杀策略机率
	if ((rand() % 100) < cur_actor->good_stun_hit && oppo_actor->state_stun == 0) {
		if ((oppo_actor->good_effects[12] != 935) && (oppo_actor->good_effects[12] != 939))
		{
			gain->state_stun = -1;
		}
	}

	// 失血机率
	if((rand() % 100) < cur_actor->getSpecialInt(TYPE607))
	{
		if((oppo_actor->good_effects[12] != 935) && (oppo_actor->good_effects[12] != 939))
		{
			gain->state_loseblood = -1;
		}
	}

	if(oppo_actor->good_effects[11] != 959)
	{
		//破坏攻击
		if((rand() % 100) < cur_actor->good_effects[2] && oppo_actor->good_effects[11] != 951) //防御异常属性攻击
		{
			gain->state_power = -1;
		}
		//破坏精神
		if((rand() % 100) < cur_actor->good_effects[3] && oppo_actor->good_effects[11] != 952)
		{
			gain->state_intellegence = -1;
		}
		//破坏防御
		if((rand() % 100) < cur_actor->good_effects[4] && oppo_actor->good_effects[11] != 953)
		{
			gain->state_defence = -1;
		}
		//破坏爆发
		if((rand() % 100) < cur_actor->good_effects[5] && oppo_actor->good_effects[11] != 954)
		{
			gain->state_explosion = -1;
		}
		//破坏士气
		if((rand() % 100) < cur_actor->good_effects[6] && oppo_actor->good_effects[11] != 955)
		{
			gain->state_fortune = -1;
		}
		//破坏移动
		if((rand() % 100) < cur_actor->good_effects[7] && oppo_actor->good_effects[11] != 956)
		{
			gain->state_speed = -1;
		}
	}

	if(cur_actor->good_randbuff_hit && oppo_actor->good_effects[12] != 939) //随机状态攻击  可取值1、2、3、4.
	{
		int temp_num = cur_actor->good_randbuff_hit;
		if (temp_num>4)
			temp_num = 4;
		switch (rand()%temp_num+1)
		{
		case 4://封杀策略
			if (oppo_actor->good_effects[12] != 935)//防御异常状态攻击
			{
				gain->state_stun = -1;
			}
			break;
		case 3://麻痹攻击
			if (oppo_actor->good_effects[12] != 934)
			{
				gain->state_palsy = -1;
			}
			break;
		case 2://中毒攻击
			if (oppo_actor->good_effects[12] != 933)
			{
				gain->state_poison = -1;
			}
			break;
		case 1://混乱攻击
			if(oppo_actor->good_effects[12] != 932)
			{
				gain->state_puzzle = -1;
			}
			break;
		default:
			break;
		}
	}

	//随机属性攻击
	if (cur_actor->good_effects[21])
	{
		int temp_num = cur_actor->good_effects[21];
		int temp[5] = { 0 };
		int temp_type = 0,isenabled = 0;
		if (temp_num > 5)
			temp_num = 5;
		for (int i = 0; i < temp_num; i++)
		{
			temp_type = rand() % 5 + 1;
			isenabled = 1;
			for (int j = 0; j < i; j++)
			{
				if (temp_type == temp[j])
				{
					isenabled = 0;
					i--;
					break;
				}
			}

			if(isenabled && oppo_actor->good_effects[11] != 959)
			{
				temp[i] = temp_type;
				switch (temp_type)
				{
				case 1:
					if ((rand() % 100) < 20 && oppo_actor->good_effects[11] != 951)
					{
						gain->state_power = -1;
					}
					break;
				case 2:
					if ((rand() % 100) < 20 && oppo_actor->good_effects[11] != 952)
					{
						gain->state_intellegence = -1;
					}
					break;
				case 3:
					if ((rand() % 100) < 20 && oppo_actor->good_effects[11] != 953)
					{
						gain->state_defence = -1;
					}
					break;
				case 4:
					if ((rand() % 100) < 20 && oppo_actor->good_effects[11] != 954)
					{
						gain->state_explosion = -1;
					}
					break;
				case 5:
					if ((rand() % 100) < 20 && oppo_actor->good_effects[11] != 955)
					{
						gain->state_fortune = -1;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if (oppo_actor->isHadSpecial(TYPE764)&&oppo_actor->state_poison == 0)
	{
		gain->state_poison = -1;
	}

	if (cur_actor->good_weak_attack) //死亡标记
	{
		gain->state_weaked = -1;
		gain->oppo_actor->weaked_num = cur_actor->good_weak_attack;
	}

	if (cur_actor->good_vampire_attack&&gain->gain_hp<0) // 吸血攻击
	{
		totalgain->gain_hp += cur_actor->good_vampire_attack*(-gain->gain_hp) / 100;
	}

    if (cur_actor->good_suck_mp_attack&&gain->gain_hp<0) // 吸MP攻击
    {
        int mp = cur_actor->good_suck_mp_attack*(-gain->gain_hp) / 100;
		mp = mp < (oppo_actor->GetCurMp() + gain->gain_mp) ? mp : (oppo_actor->GetCurMp() + gain->gain_mp);
        totalgain->gain_mp += mp;
        gain->gain_mp -= mp;
    }

	//消耗敌军mp
	if (cur_actor->isHadSpecial(TYPE715))
	{
		gain->gain_mp -= cur_actor->getSpecialInt(TYPE715);
	}

	//TYPE732 灵质，普攻恢复自身X点MP
	if (cur_actor->isHadSpecial(TYPE732) && bFirstAttack)
	{
		totalgain->gain_mp += cur_actor->getSpecialInt(TYPE732);
	}

	if (cur_actor->good_attack_must) //攻击必中
	{
		gain->state_mustHit = 1;
	}
	if (cur_actor->good_effects[49]&&!oppo_actor->border_on_death)
	{
		gain->state_poison = -1;
		gain->border_on_death = -1;
		gain->gain_hp = -(oppo_actor->GetCurHp() - 1);
	}
	if (oppo_actor->border_on_death&&gain->gain_hp < 0)
		gain->gain_hp = 0;

	gain->gain_hp += gain->gain_hp * cur_actor->getSpecialInt(TYPE424) / 100;  //物理伤害加成

	if(oppo_actor->state_reduceSkill) //洞察，物伤增加10%
	{
		gain->gain_hp += gain->gain_hp * 10 / 100;
	}

	//727
	if (cur_actor->isHadSpecial(TYPE727))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE727);

		int iGoodsAddMp = cur_actor->GetGoodsAddMp();
		int iBaseMp = cur_actor->GetBaseMp();
		int iCurMp = cur_actor->GetCurMp();

		float fPre = (iGoodsAddMp + iBaseMp - iCurMp) / ((iGoodsAddMp + iBaseMp)*0.05);
		int iPre = mSpecial[1] - 20 + fPre;

		if (iPre > 0)
		{
			gain->gain_hp += iPre * gain->gain_hp / 100;
		}
	}

	//TYPE737, 增加自身移动力*X%的伤害
	if (cur_actor->isHadSpecial(TYPE737))
	{
		gain->gain_hp += gain->gain_hp * cur_actor->getTotalAbility(TOTAL_SPEED) * cur_actor->getSpecialInt(TYPE737) / 100;
	}

	//TYPE741, 转身攻击时，增加X伤害，未转身攻击时，增加Y伤害
	if (cur_actor->isHadSpecial(TYPE741))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE741);
		int iPecent = cur_actor->direction != cur_actor->getActionDirect(oppo_actor->x, oppo_actor->y) ? mSpecial[0] : mSpecial[1];
		gain->gain_hp += gain->gain_hp * iPecent / 100;
	}

	//TYPE707, MP抵挡一定HP
	if (oppo_actor->isHadSpecial(TYPE707))
	{
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE707);

		int iMpUsed = oppo_actor->getTotalAbility(TOTAL_MP) * mSpecial[0] / 100;
		int iHpTurn = oppo_actor->getTotalAbility(TOTAL_MP) * mSpecial[0] * mSpecial[1] / 10000;
		if (oppo_actor->GetCurMp() + gain->gain_mp >= iMpUsed && -gain->gain_hp >= iHpTurn)
		{
			gain->gain_mp += -iMpUsed;
			gain->gain_hp -= -iHpTurn;
		}
	}

	//TYPE710, 附加自身等级X百分比伤害
	if (cur_actor->isHadSpecial(TYPE710))
	{
		gain->gain_hp += -cur_actor->GetActorLv() * cur_actor->getSpecialInt(TYPE710) / 100;
	}

	if (oppo_actor->state_reduceHurt&&gain->gain_hp<0)
	{
		gain->gain_hp -= gain->gain_hp*oppo_actor->m_mActorBuff[ACTOR_BUFF_REDUCEHURT]->iPercent / 100;
	}

	//TYPE757 单独减伤
	if (oppo_actor->isHadSpecial(TYPE757) && oppo_actor->SeparateState(oppo_actor->getSpecialInt(TYPE757)))
	{
		map<int, int> mSeid = oppo_actor->getSpecialMap(TYPE757);
		gain->gain_hp -= gain->gain_hp * mSeid[1] / 100;
	}

	if (isVisible)
	{
		BattleControler::getBattleActor(oppo_actor->actor_id)->hp_gain = gain->gain_hp;
		BattleControler::getBattleActor(oppo_actor->actor_id)->mp_gain = gain->gain_mp;
		BattleControler::getBattleActor(oppo_actor->actor_id)->cur_probability = gain->probability;
	}
	return gain;
}

ActionSingleGainPtr CalcSkillSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, SkillAttrPtr skill, ActionGainPtr totalgain)
{
	ActionSingleGainPtr gain = new ActionSingleGain;

	int iCurLandId = history->map_array[totalgain->point.y][totalgain->point.x];
	int iOppoLandId = history->map_array[oppo_actor->y][oppo_actor->x];
	int iCurLandsGain = cur_actor->getLandAbilityGain(iCurLandId);
	int iOppoLandsGain = oppo_actor->getLandAbilityGain(iOppoLandId);

	int iAddIntellegence = 0;
	if (cur_actor->good_effects[ADD_ATTACK_ELEM])
	{
		int real_x = -1, real_y = -1;
		if (BattleControler::auto_run != 0)
		{
			real_x = totalgain->point.x;
			real_y = totalgain->point.y;
		}
		iAddIntellegence = cur_actor->getRealAbility(ACTOR_ABILITY_POWER, true,real_x,real_y)*cur_actor->good_effects[ADD_ATTACK_ELEM]/100;
	}

	int iIntellegence = 0;
	int oppo_defence = 0;
	if (skill->type == 1)
	{
		int iOppoAddDef = 0;
		if (oppo_actor->isHadSpecial(TYPE766))
		{
			iOppoAddDef = oppo_actor->getSpecialInt(TYPE766) * oppo_actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE, true) / 100;
		}
		oppo_defence = (oppo_actor->getRealAbility(ACTOR_ABILITY_DEFENCE, true)+ iOppoAddDef)* iOppoLandsGain / 100;
		iIntellegence = cur_actor->getRealAbility(ACTOR_ABILITY_POWER, true);
	}
	else
		iIntellegence = cur_actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE, true);

	int cur_intellegence = (iIntellegence + iAddIntellegence)* iCurLandsGain / 100;
	int cur_energe = (cur_actor->getRealAbility(ACTOR_ABILITY_FORTUNE, true))*iCurLandsGain / 100;

	int iOppoAddInt = 0;
	if (skill->type == 2 && oppo_actor->isHadSpecial(TYPE766))
	{
		iOppoAddInt = oppo_actor->getSpecialMap(TYPE766)[1] * oppo_actor->getRealAbility(ACTOR_ABILITY_DEFENCE, true) / 100;
	}
	int oppo_intellegence = (oppo_actor->getRealAbility(ACTOR_ABILITY_INTELLEGENCE, true)+ iOppoAddInt)* iOppoLandsGain / 100;
	int oppo_energe = (oppo_actor->getRealAbility(ACTOR_ABILITY_FORTUNE, true))* iOppoLandsGain / 100;

	int base_hp_hurt = 0;
	if (skill->type == 1)
		base_hp_hurt = ((cur_intellegence - (oppo_intellegence / 5 + oppo_defence * 3 / 10)) / 3 + cur_actor->GetActorLv() + 25);
	else
		base_hp_hurt = ((cur_intellegence - oppo_intellegence) / 3 + cur_actor->GetActorLv() + 25);
	
	gain->oppo_actor = oppo_actor;
	int auxiliary_spells_output = 0; //辅助法术输出加成百分比
	int auxiliary_spell_defense = 0;////辅助法术防御减伤百分比

	if(cur_actor->isHadSpecial(TYPE303)) //辅助法术输出
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE303);
		SeidType* ptrType = trData->m_mSeidType[TYPE303];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->magic_type)
			{
				auxiliary_spells_output += mSpecial[i + 1];
			}
		}
	}

	if (oppo_actor->good_effects[18]) //辅助法术防御
	{
		int temp_magic_type = trData->m_mSeid[oppo_actor->good_effects[18]]->special_effe[0];
		if ((temp_magic_type == 999) || (temp_magic_type == skill->magic_type))
		{
			auxiliary_spell_defense = trData->m_mSeid[oppo_actor->good_effects[18]]->special_effe[1];
		}
	}
	//地形辅助法术输出和防御
	for(int i = 0; i < trData->lands[iCurLandId].seid_num; i++)
	{
		Seid* seid = trData->m_mSeid[trData->lands[iCurLandId].seid[i]];
		if(seid->type == 303)
		{
			for(int j = 0; j < seid->special_effe_num; j += 2)
			{
				if(seid->special_effe[j] == 999 || seid->special_effe[j] == skill->magic_type)
				{
					auxiliary_spells_output += seid->special_effe[j + 1];
					break;
				}
			}
		}
	}
	for(int i = 0; i < trData->lands[iOppoLandId].seid_num; i++)
	{
		Seid* seid = trData->m_mSeid[trData->lands[iOppoLandId].seid[i]];
		if(seid->type == 304)
		{
			for(int j = 0; j < seid->special_effe_num; j += 2)
			{
				if(seid->special_effe[j] == 999 || seid->special_effe[j] == skill->magic_type)
				{
					auxiliary_spell_defense += seid->special_effe[j + 1];
					break;
				}
			}
		}
	}

	if (cur_actor->state_skillhurt) //法术伤害加成BUFF
	{
		auxiliary_spells_output += 0; //TODO
	}

	//TYPE 601, 法术伤害加成
	if(cur_actor->isHadSpecial(TYPE601)) //辅助法术输出
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE601);
		SeidType* ptrType = trData->m_mSeidType[TYPE601];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->type)
			{
				auxiliary_spells_output += mSpecial[i + 1];
			}
		}
	}

	//TYPE 700, 攻守兼备
	auxiliary_spells_output += cur_actor->getSpecialInt(TYPE700);

	// 降低策略损伤
	base_hp_hurt = -base_hp_hurt * (100 - oppo_actor->good_reduce_skill_hurt + auxiliary_spells_output - auxiliary_spell_defense) / 100;

	if (cur_actor->good_assist_fire != 0 && skill->magic_type == 900) {
		base_hp_hurt = (100 + cur_actor->good_assist_fire) * base_hp_hurt/ 100;
	}
	if (cur_actor->good_assist_wind != 0 && skill->magic_type == 903) {
		base_hp_hurt = (100 + cur_actor->good_assist_wind) * base_hp_hurt/ 100;
	}

	totalgain->need_mp = skill->GetMpConsume(cur_actor);

	if(base_hp_hurt >= 0)
	{
		base_hp_hurt = -1;
	}

	/////////新技能伤害计算，加持BUFF
	if (skill->power!=1000)
		gain->gain_hp = base_hp_hurt * skill->power / 100;
	else
		gain->gain_hp = -oppo_actor->GetCurHp() - 10;

	float iExtraGainHp = skill->GetExtraGainHp(cur_actor);
	if (skill->power)
		gain->gain_hp -= (iExtraGainHp + skill->hurt_num);

	float iExtraSeidGainHp = skill->GetExtraGainHp(cur_actor, 2) + skill->prop_num;

	float iExtraSeidGainPer = skill->GetExtraGainHp(cur_actor, 3) + skill->prop_per;

	switch (skill->seid)
	{
	case 1://损HP
		break;
	case 2://砂暴
		break;
	case 3://谍报
		gain->gain_mp = base_hp_hurt - iExtraSeidGainHp + base_hp_hurt*iExtraSeidGainPer/100;
		totalgain->gain_mp = -gain->gain_mp;
		break;
	case 4://降低敏捷
		gain->state_explosion = -1;
		break;
	case 5://降低士气
		gain->state_fortune = -1;
		break;
	case 6://降低能力，全能兵种924同时降低智力和攻击
	{
			   if (trData->occu_types[oppo_actor->GetActorOccuType()].job_type == 923)
			   {
				   gain->state_intellegence = -1;
			   }
               else if (trData->occu_types[oppo_actor->GetActorOccuType()].job_type == 924)
               {
                   gain->state_intellegence = -1;
                   gain->state_power = -1;
               }
			   else
			   {
				   gain->state_power = -1;
			   }

	}
		break;
	case 7://降低防御
		gain->state_defence = -1;
		break;
	case 8://混乱
		gain->state_puzzle = -1;
		break;
	case 9://中毒
		gain->state_poison = -1;
		break;
	case 10://麻痹
		gain->state_palsy = -1;
		break;
	case 11://封咒
		gain->state_stun = -1;
		break;
	case 12://八阵图
		break;
	case 13://加HP
		break;
	case 14://加MP(建言)
		gain->gain_mp = 24 * (iExtraSeidGainPer + 100) / 100 + iExtraSeidGainHp;
		break;
	case 15://恢复状态
	{
				if (oppo_actor->state_palsy != 0)
					gain->state_palsy = 1;
				if (oppo_actor->state_poison != 0)
					gain->state_poison = 1;
				if (oppo_actor->state_puzzle != 0)
					gain->state_puzzle = 1;
				if (oppo_actor->state_stun != 0)
					gain->state_stun = 1;
				if (oppo_actor->state_blind != 0)
					gain->state_blind = 1;
				if (oppo_actor->state_obfuscation != 0)
					gain->state_obfuscation = 1;
                if (oppo_actor->state_weaked != 0)
                    gain->state_weaked = 1;
				if(oppo_actor->state_loseblood != 0)
					gain->state_loseblood = 1;
	}
		break;
	case 16://增加移动力
		gain->state_speed = 1;
		break;
	case 17://增加敏捷
		gain->state_explosion = 1;
		break;
	case 18://增加士气
		gain->state_fortune = 1;
		break;
	case 19://增加能力，全能兵种924同时提高智力和攻击
	{
		if (oppo_actor->getOccuPowerType() == 2)
		{
			gain->state_intellegence = 1;
		}
		else
		{
			gain->state_power = 1;
		}
	}
		break;
	case 20://增加防御
		gain->state_defence = 1;
		break;
	case 21://再次行动
		gain->action_again = 1;
		break;
	case 22://气候变化
		break;
	case 23://气合，全能兵种924同时提高智力和攻击
	{
				if (oppo_actor->getOccuPowerType() == 2)
				{
					gain->state_intellegence = 1;
				}
				else
				{
					gain->state_power = 1;
				}
	}
		break;
	case 24://冥想
	{
		if (oppo_actor->GetCurMp() + gain->gain_mp*(100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp > oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp())
				{
					gain->gain_mp = oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp() - oppo_actor->GetCurMp() - gain->gain_mp* iExtraSeidGainPer / 100 - iExtraSeidGainHp;
					totalgain->need_hp = gain->gain_mp;
				}
				else
				{
					gain->gain_mp = 24 * (100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp;
					totalgain->need_hp = 24 * (100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp;
				}
	}
		break;
	case 25://霸气，924全能兵种提高能力的时候同时提高智力和攻击力
	{
				if (oppo_actor->getOccuPowerType() == 2)
				{
					gain->state_intellegence = 1;
				}
				else
				{
					gain->state_power = 1;
				}
				gain->state_defence = 1;
				gain->state_explosion = 1;
				gain->state_fortune = 1;
	}
		break;
	case 26://青龙
		break;
	case 27://玄武
		break;
	case 28://白虎
	{
		gain->gain_hp = 2000;
	}
		break;
	case 29://变雨
		gain->cCurAir = AIR_TYPE_RAIN;
		break;
	case 30://变晴
		gain->cCurAir = AIR_TYPE_SUN;
		break;
	case 31://变阴
		gain->cCurAir = AIR_TYPE_CLOUDY;
		break;
	case 32://盲
		gain->state_blind = -1;
		break;
	case 33://昏迷
		if ((rand() % 100) >= oppo_actor->getSpecialInt(TYPE722))
		{
			gain->state_obfuscation = -1;
		}
		break;
	case 34://自己加防御
		gain->state_defence = 1;
		break;
	case 35://自己加敏捷
		gain->state_explosion = 1;
		break;
	case 36://自己加士气
		gain->state_fortune = 1;
		break;
	case 37://自己加移动
		gain->state_speed = 1;
		break;
	case 38://自残
		break;
	case 39://连续移动
		gain->fast_move = 1;
		break;
	case 40://瞬间移动
		break;
	case 41://变兵种
		break;
	case 42://召唤助阵
		gain->actor_relive = 1;
		gain->actor_relive_id = skill->power-1;
		break;
	case 43://隐身
		break;
	case 44://地行
		break;
	case 45://变城防
		break;
	case 46://恢复HP和MP
		break;
	case 47://恢复所有状态
		break;
	case 48://经验加倍
		break;
	case 49://全部上升
		break;
	case 50://全部下降
    {
        //降低敏捷
        gain->state_explosion = -1;
        //降低士气
        gain->state_fortune = -1;
        //降低防御
        gain->state_defence = -1;
        //降低能力，如果是924全能兵种，同时降低智力和攻击
		if (oppo_actor->getOccuPowerType() == 2)
        {
            gain->state_intellegence = -1;
        }
        else
        {
            gain->state_power = -1;
        }
    }
		break;
	case 51://降低移动力
		gain->state_speed = -1;
		break;
	case 52://夷为平地
		break;
	case 53://挖地为坑
		break;
	case 54://指地成刚
		break;
	case 55://先天八卦
		break;
	case 56://后天八卦
		break;
	case 57://乾坤倒置
		break;
	case 58://斗转星移
		break;
	case 59://吸血
		totalgain->gain_hp = -gain->gain_hp*(100+iExtraSeidGainPer)/100 - iExtraSeidGainHp;
		break;
	case 60://加HP（小补）
		gain->gain_hp = (cur_intellegence *skill->power / 100) * (100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp;
		break;
	case 61://加HP（大补）
		gain->gain_hp = cur_intellegence / 2 + 70 + iExtraGainHp;
		break;
	case 62://加MP(献策)
		gain->gain_mp = 48 * (100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp;
		break;
	case 63://朱雀
		break;
	case 65://援队
		gain->gain_hp = cur_intellegence + 70;
		break;
	case 66:
		break;
    case 69:
        gain->state_weaked = -1;
        gain->oppo_actor->weaked_num = 80;
        break;
    case 70:
        gain->state_weaked = -1;
        gain->oppo_actor->weaked_num = 40;
        break;
	case 71://该seid连击已使用
		break;
    case 72: //回归，增加行动次数，恢复HP、MP。可全体使用
        gain->action_again = 1;
		gain->gain_hp = (oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp()) * 20 * 100 - oppo_actor->GetCurHp();
		gain->gain_mp = (oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp()) * 10 * 100 - oppo_actor->GetCurMp();
        gain->state_mustHit = 1;
        break;
    case 73: { //直接扣敌人MP，不加自己的的MP。如果没有MP了，就扣HP。可全体使用
		int gainMp = skill->power * cur_actor->GetActorLv() / 100 + iExtraGainHp;
				 int deltaMP = oppo_actor->GetCurMp() >= gainMp ? gainMp : oppo_actor->GetCurMp();
        gain->gain_mp = -deltaMP;
        int gainHP = gainMp - deltaMP;
		int deltaHP = oppo_actor->GetCurHp() >= gainHP ? gainHP : oppo_actor->GetCurHp();
        gain->gain_hp = -deltaHP;
		if (oppo_actor->GetCurMp() > 2)
            gain->gain_hp -= 1;
        break;
        }
	case 74://魅惑
	{
		if (cur_actor->relation == 0 && oppo_actor->relation == 2 && trData->actors[oppo_actor->actor_id].iHeroGrade < 2 &&
			cur_actor->GetCurSkillLevel(skill->skill_id + 1) >= oppo_actor->GetActorLv() &&
			oppo_actor->GetCurHp() < cur_actor->getTotalAbility(TOTAL_HP)*skill->power / 100)
		{
			gain->controlled_enemy_actorId = oppo_actor->actor_id + 1;
		}
		else
			gain->land_strict = 1;
		gain->gain_hp = 0;
	}
		break;
	case 75: // 变化天气为日月食
        break;
	case 76: //特点是此法术的伤害，类似固定伤害，不过要乘以放法术的人物的当前等级
        break;
    case 77: //恢复状态 + 白虎 的综合效果
    {
        if (oppo_actor->state_palsy != 0)
            gain->state_palsy = 1;
        if (oppo_actor->state_poison != 0)
            gain->state_poison = 1;
        if (oppo_actor->state_puzzle != 0)
            gain->state_puzzle = 1;
        if (oppo_actor->state_stun != 0)
            gain->state_stun = 1;
        if (oppo_actor->state_blind != 0)
            gain->state_blind = 1;
        if (oppo_actor->state_obfuscation != 0)
            gain->state_obfuscation = 1;
        if (oppo_actor->state_weaked != 0)
            gain->state_weaked = 1;
		if(oppo_actor->state_loseblood != 0)
			gain->state_loseblood = 1;
        gain->gain_hp = 2000;
    }
		break;
	case 101:
	{
		int iMoveX = cur_actor->x, iMoveY = cur_actor->y;
		if (BattleControler::auto_run)
		{
			iMoveX = totalgain->point.x;
			iMoveY = totalgain->point.y;
		}
		if (oppo_actor->needAssault(iMoveX, iMoveY, cur_actor))
		{
			gain->actor_knockback = 1;
		}
		else
		{
			gain->gain_hp += gain->gain_hp*iExtraSeidGainPer / 100;
		}
	}
		break;
	case 102:
		break;
	case 103://减少法伤光环
		gain->reduce_skill_hurt = 1;
		break;
	case 104://风天致盲
		if (history->m_iCurAir == AIR_TYPE_WIND)
			gain->state_blind = -1;
		break;
	case 105:
	{
				int iMoveX = cur_actor->x, iMoveY = cur_actor->y;
				if (BattleControler::auto_run)
				{
					iMoveX = totalgain->point.x;
					iMoveY = totalgain->point.y;
				}
				if (oppo_actor->needAssault(iMoveX, iMoveY, cur_actor))
				{
					gain->through_the_target = 1;
				}
				else
				{
					gain->gain_hp += gain->gain_hp*iExtraSeidGainPer / 100;
				}
	}
		break;
	case 108:
		gain->state_SkillHurtAndRebound = 1;
		break;
	case 110: //毒能阵法
		gain->add_skill_hurt = 1;
		break;
	case 111: //水之炼狱
		if (history->m_iCurAir == AIR_TYPE_SNOW)
			gain->state_speed = -1;
		break;
	case 112: //风灵精修
		gain->state_skillhurt = 1;
		break;
	case 113: //无畏冲锋
	{
				  int iMoveX = cur_actor->x, iMoveY = cur_actor->y;
				  if (BattleControler::auto_run)
				  {
					  iMoveX = totalgain->point.x;
					  iMoveY = totalgain->point.y;
				  }
				  if (oppo_actor->needAssault(iMoveX, iMoveY, cur_actor))
				  {
					  gain->new_assault = 1;
					  gain->gain_hp += gain->gain_hp*iExtraSeidGainPer / 100;
				  }
				  else
				  {
					  gain->gain_hp -= gain->gain_hp*iExtraSeidGainPer / 100;
				  }
	}
		break;
	case 114: //尖盾防御
		break;
	case 115: //盾击
		if((rand() % 100) >= oppo_actor->getSpecialInt(TYPE722))
		{
			gain->state_obfuscation = -1;
		}
		break;
	case 116://降攻减防
		gain->state_adddef_redpow = -1;
		break;
	case 118://灵魂链接
		gain->state_sual_link = 1;
		break;
	case 119://断魂斩
	{
		if (oppo_actor->state_loseblood)
			gain->gain_hp = gain->gain_hp * 3 / 2;
	}
		break;
	case 120://恢复术
	{
		gain->gain_hp = (cur_intellegence *skill->power / 100) * (100 + iExtraSeidGainPer) / 100 + iExtraSeidGainHp;
		gain->state_addhp = 1;
	}
	break;
	case 121://刀轮
		if (cur_actor == oppo_actor)
		{
			gain->gain_hp = 0;
		}
		break;
	case 122://洞察
		gain->state_reduceSkill = -1;
		break;
	case 123://风霜itid:50
	{
		if (oppo_actor->coat.isNormalGood() && oppo_actor->coat.getItId() == 50)
		{
			int iDefNum = oppo_actor->coat.GetGoodsPower();
			if (oppo_actor->coat.iQuality > 0)
			{
				iDefNum += oppo_actor->coat.getStarAddVal();
			}

			gain->gain_hp -= iDefNum*iExtraSeidGainPer / 100 + iExtraSeidGainHp;
		}
	}
		break;
	case 124://惨痛压制
		gain->state_reduceHurt = 1;
		break;
	case 125: //虎啸
		setLossBuff(gain, iExtraSeidGainPer);
		break;
	case 126://召唤塔
		gain->actor_tower_def = skill->power - 900;
		gain->gain_hp = 0;
		break;
	case 127://分解塔
		gain->actor_tower_def = 5;
		gain->gain_hp = 0;
		break;
	case 128://自爆
		gain->gain_hp = -cur_actor->getRealAbility(ACTOR_ABILITY_POWER, true)*iExtraSeidGainPer / 100 + iExtraSeidGainHp;
		break;
	case 129://拖到身边
	{
		int iMoveX = cur_actor->x, iMoveY = cur_actor->y;
		if (BattleControler::auto_run)
		{
			iMoveX = totalgain->point.x;
			iMoveY = totalgain->point.y;
		}
		if (oppo_actor->CanDrag(iMoveX, iMoveY, cur_actor))
		{
			gain->actor_can_drag = 1;
			gain->gain_hp += gain->gain_hp*iExtraSeidGainPer / 100;
		}
	}
	break;
	case 130://遣将
	{
		if (cur_actor->GetCurSkillLevel(skill->skill_id + 1) >= oppo_actor->GetActorLv()&&BattleControler::auto_run == 0)
		{
			gain->skill_dispatch_actor = 1;
			gain->gain_hp = 0;
		}
		else
			gain->land_strict = 1;
	}
		break;
	case 131://冲锋号令
	{
		gain->state_emergency_command = 1;
	}
	break;
	default:
		break;
	}

	//昏迷状态
	if (oppo_actor->state_obfuscation&&gain->state_obfuscation != -1&&oppo_actor->relation!=cur_actor->relation)//解除昏迷
	{
		gain->state_obfuscation = 1;
	}

	int harm_num = GetJobPKHartnum(skill->magic_type, trData->occu_types[oppo_actor->GetActorOccuType()].occupation_type_id, true);

	//TYPE 700, 攻守兼备
	if(gain->gain_hp > 0 || gain->gain_mp)
	{
		harm_num += cur_actor->getSpecialInt(TYPE700);
	}

	gain->gain_hp = gain->gain_hp * harm_num / 100;
	gain->gain_mp = gain->gain_mp * harm_num / 100;

	int iSpecialGainHp = 0;
	if ((cur_actor->relation != 2 && oppo_actor->relation == 2) || (cur_actor->relation == 2 && oppo_actor->relation != 2))
	{
		int iReduceSkillHurt = oppo_actor->GetRangeBuffEffect(RANGE_BUFF_REDUCE_SKILLHURT, true);
		iSpecialGainHp -= gain->gain_hp *iReduceSkillHurt/ 100;

		int iAddSkillHurt = oppo_actor->GetRangeBuffEffect(RANGE_BUFF_ADD_SKILLHURT, false);
		iSpecialGainHp += gain->gain_hp *iAddSkillHurt / 100;
	}
	if (cur_actor->state_SkillHurtAndRebound)
	{
		iSpecialGainHp += gain->gain_hp * cur_actor->m_mActorBuff[ACTOR_BUFF_FIRESHIELD]->iPercent / 100;
	}

	if (cur_actor->isHadSpecial(TYPE724) && cur_actor->mSeidAddGain[TYPE724] > 0 && gain->gain_hp < 0)
	{
		gain->mSeidAddGain[0] = cur_actor->mSeidAddGain[TYPE724];
		iSpecialGainHp -= cur_actor->mSeidAddGain[TYPE724];
	}

	if (cur_actor->isHadSpecial(TYPE726) && cur_actor->mSeidAddGain[TYPE726] > 0)
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE726);
		iSpecialGainHp += gain->gain_hp * mSpecial[0] * cur_actor->mSeidAddGain[TYPE726] / 100;
	}

	if (cur_actor->isHadSpecial(TYPE727))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE727);
		int iGoodsAddMp = cur_actor->GetGoodsAddMp();
		int iBaseMp = cur_actor->GetBaseMp();
		int iCurMp = cur_actor->GetCurMp();

		if (skill->type == 1)
		{
			float fPre = (iGoodsAddMp + iBaseMp - iCurMp) / ((iGoodsAddMp + iBaseMp)*0.05);
			int iPre = mSpecial[1] - 20 + fPre;

			if (iPre > 0)
			{
				gain->gain_hp += iPre * gain->gain_hp / 100;
			}
		}
		else
		{
			int iPre = mSpecial[0] - (iGoodsAddMp + iBaseMp - iCurMp) / ((iGoodsAddMp + iBaseMp)*0.025);

			if (iPre > 0)
			{
				iSpecialGainHp += iPre * gain->gain_hp / 100;
			}
		}
	}

	gain->gain_hp += iSpecialGainHp;
	//天气加成
	int iWeatherAddNum = -1;
	if (trData->skill_weather_addition.count(skill->magic_type) > 0 && trData->skill_weather_addition[skill->magic_type].count(history->m_iCurAir - 1) > 0)
	{
		iWeatherAddNum = trData->skill_weather_addition[skill->magic_type][history->m_iCurAir - 1];
	}

	//地形加成
	int iLandsAddNum = -1;
	if (trData->lands[history->map_array[oppo_actor->y][oppo_actor->x]].atk_up.count(skill->magic_type)>0)
	{
		iLandsAddNum = trData->lands[history->map_array[oppo_actor->y][oppo_actor->x]].atk_up[skill->magic_type];
	}

	//TYPE719 减少恶劣天气和加成有利天气对法术的影响
	int iAddAbility1 = 0, iAddAbility2 = 0;
	if (cur_actor->isHadSpecial(TYPE719))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE719);
		SeidType* ptrType = trData->m_mSeidType[TYPE719];
		for (UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if ((mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->magic_type))
			{
				if(mSpecial[i + 1]>iAddAbility1)
					iAddAbility1 = mSpecial[i + 1];
				if (mSpecial[i + 2]>0)
					iAddAbility2 += mSpecial[i + 2];
			}
		}
	}

	if (iAddAbility1)
	{
		if (iWeatherAddNum>-1&&iWeatherAddNum < 100 && iWeatherAddNum < iAddAbility1)
			iWeatherAddNum = iAddAbility1;
		if (iLandsAddNum>-1&&iLandsAddNum < 100 && iLandsAddNum < iAddAbility1)
			iLandsAddNum = iAddAbility1;
	}

	if (iAddAbility2)
	{
		if (iWeatherAddNum>-1 && iWeatherAddNum >= 100)
			iWeatherAddNum += iAddAbility2;
		if (iLandsAddNum>-1 && iLandsAddNum >= 100)
			iLandsAddNum += iAddAbility2;
	}

	if(iWeatherAddNum>0)
		gain->gain_hp = gain->gain_hp*iWeatherAddNum / 100;
	if(iLandsAddNum>0)
		gain->gain_hp = gain->gain_hp*iLandsAddNum / 100;

    /* 法术固定伤害，无视其他影响法术伤害值的原因，但不影响概率 */
    if (skill->seid == 67) 
	{
		gain->gain_hp = -skill->power;
    }

    /* 法术固定伤害 * 等级 */
    if (skill->seid == 76) 
	{
		gain->gain_hp = -skill->power * cur_actor->GetActorLv();
    }

	if (oppo_actor->state_reduce_skill_hurt_range) //法术减伤光环
	{
		gain->gain_hp -= gain->gain_hp * oppo_actor->m_mActorBuff[ACTOR_BUFF_HURTDW]->iPercent / 100;
	}

	if (oppo_actor->state_reduceSkill) //洞察，法术减伤
	{
		gain->gain_hp -= gain->gain_hp * oppo_actor->m_mActorBuff[ACTOR_BUFF_REDUCESKILL]->iPercent / 100;
	}

	//TYPE707, MP抵挡一定HP
	if(oppo_actor->isHadSpecial(TYPE707) && gain->gain_hp < 0)
	{
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE707);

		int iMpUsed = oppo_actor->getTotalAbility(TOTAL_MP) * mSpecial[0] / 100;
		int iHpTurn = oppo_actor->getTotalAbility(TOTAL_MP) * mSpecial[0] * mSpecial[1] / 10000;
		if(oppo_actor->GetCurMp() + gain->gain_mp >= iMpUsed && -gain->gain_hp >= iHpTurn)
		{
			gain->gain_mp += -iMpUsed;
			gain->gain_hp -= -iHpTurn;
		}
	}

	//TYPE708, 过量治疗恢复MP
	if (cur_actor->isHadSpecial(TYPE708) && gain->gain_hp > 0)
	{
		int iOverflow = oppo_actor->GetCurHp() + gain->gain_hp - oppo_actor->getTotalAbility(TOTAL_HP);
		if (iOverflow > 0)
		{
			//计算公式：MP恢复量 = 溢出血量 x MP总消耗量 x 恢复系数 / HP总恢复量
			int iMpAdd = iOverflow * cur_actor->getSpecialInt(TYPE708) * totalgain->need_mp / gain->gain_hp / 100;
			if (iMpAdd > totalgain->gain_mp) //全体加血恢复量取最大值
			{
				totalgain->gain_mp = iMpAdd;
			}
		}
	}

	if(oppo_actor->isHadSpecial(TYPE709) && gain->gain_hp < 0) //辅助法术防御, 减弱最终伤害
	{
		int iHurtNum = 100;
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE709);
		SeidType* ptrType = trData->m_mSeidType[TYPE709];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->type)
			{
				iHurtNum -= mSpecial[i + 1];
			}
		}

		if (iHurtNum <= 0)
		{
			iHurtNum = 1;
		}
		gain->gain_hp = gain->gain_hp * iHurtNum / 100;
	}

	//减免最终伤害
	if (oppo_actor->isHadSpecial(TYPE723))
	{
		int iSeidNum = oppo_actor->getSpecialInt(TYPE723);

		if (gain->gain_hp > 0)
			gain->gain_hp += gain->gain_hp*iSeidNum/100;
		else
			gain->gain_hp -= gain->gain_hp*iSeidNum/100;
	}

	if (oppo_actor->state_reduceHurt&&gain->gain_hp < 0)
	{
		gain->gain_hp -= gain->gain_hp*oppo_actor->m_mActorBuff[ACTOR_BUFF_REDUCEHURT]->iPercent / 100;
	}

	if (oppo_actor->isHadSpecial(TYPE724)&& gain->gain_hp < 0)
	{
		int iReduceNum = 0;
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE724);
		SeidType* ptrType = trData->m_mSeidType[TYPE724];
		for (UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if (mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->type)
			{
				iReduceNum += mSpecial[i + 1];
			}
		}

		gain->mSeidAddGain[1] = -gain->gain_hp*iReduceNum / 100;
		gain->gain_hp += gain->mSeidAddGain[1];
	}

	gain->probability = 100;

	//TYPE 606, 金刚，几率抵挡技能
	if (oppo_actor->isHadSpecial(TYPE606) && gain->gain_hp < 0)
	{
		map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE606);
		SeidType* ptrType = trData->m_mSeidType[TYPE606];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->type)
			{
				gain->probability -= mSpecial[i + 1];
			}
		}
	}

	if(cur_actor->good_vampire_attack && gain->gain_hp < 0) // 吸血攻击，技能也生效
	{
		totalgain->gain_hp += cur_actor->good_vampire_attack * (-gain->gain_hp) / 100;
	}

	//TYPE717, 技能所造成的有效治疗或伤害X%会治疗自己
	if (cur_actor->isHadSpecial(TYPE717))
	{
		int iValidHp = abs(gain->gain_hp);
		if (oppo_actor->GetCurHp() + gain->gain_hp - oppo_actor->getTotalAbility(TOTAL_HP) > 0)
		{
			iValidHp = oppo_actor->getTotalAbility(TOTAL_HP) - oppo_actor->GetCurHp();
		}
		else if (oppo_actor->GetCurHp() + gain->gain_hp < 0)
		{
			iValidHp = oppo_actor->GetCurHp();
		}

		totalgain->gain_hp += iValidHp * cur_actor->getSpecialInt(TYPE717) / 100;
	}

	if(gain->probability < 0)
	{
		gain->probability = 0;
	}

	if(skill->power == 1000 && oppo_actor->actor_id == history->enemy_skipper_index)//秒杀系技能
	{
		gain->enemy_skipper_strict = 1;
	}

	//TYPE744 抗击，减少物理普攻正面X、侧面Y、背面Z的伤害
	if(oppo_actor->isHadSpecial(TYPE744))
	{
		gain->gain_hp -= gain->gain_hp * oppo_actor->getAttackedType(cur_actor) / 100;
	}

	//TYPE747 提升单体技能X效果Y%
	if(cur_actor->isHadSpecial(TYPE747))
	{
		map<int, int> mSpecial = cur_actor->getSpecialMap(TYPE747);
		SeidType* ptrType = trData->m_mSeidType[TYPE747];
		for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
		{
			if(ptrType->iClass == skill->skill_id + 1 || mSpecial[i] == skill->skill_id + 1)
			{
				gain->gain_hp += gain->gain_hp * mSpecial[i + 1] / 100;
			}
		}
	}

	//TYPE757 单独减伤
	if (oppo_actor->isHadSpecial(TYPE757) && oppo_actor->SeparateState(oppo_actor->getSpecialInt(TYPE757)))
	{
		map<int, int> mSeid = oppo_actor->getSpecialMap(TYPE757);
		gain->gain_hp -= gain->gain_hp * mSeid[1] / 100;
	}

	//庇护是否触发，考虑TYPE724
	if (gain->probability && skill->effect_region_type == 0 && !oppo_actor->state_sual_link && gain->gain_hp < 0)
	{
		int iActorId = oppo_actor->getRangeBuffActorId(ACTOR_BUFF_SHIELD_RANGE, true);
		HistoryActor* ptrActor = history->FindBattleActor(iActorId);
		if (ptrActor && ptrActor != oppo_actor && ptrActor->m_iShieldNum)
		{
			int iSkillType = ptrActor->getSpecialMap(TYPE731)[3];
			SeidType* ptrType = trData->m_mSeidType[TYPE731];
			if(iSkillType == ptrType->iClass || iSkillType == skill->type)
			{
				gain->share_actor = ptrActor;
				if(gain->share_actor->isHadSpecial(TYPE724))
				{
					int iReduceNum = 0;
					map<int, int> mSpecial = gain->share_actor->getSpecialMap(TYPE724);
					SeidType* ptrType = trData->m_mSeidType[TYPE724];
					for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
					{
						if(mSpecial[i] == ptrType->iClass || mSpecial[i] == skill->type)
						{
							iReduceNum += mSpecial[i + 1];
						}
					}

					gain->mSeidAddGain[1] = -gain->gain_hp*iReduceNum / 100;
					gain->gain_hp += gain->mSeidAddGain[1];
				}
			}
		}
	}

	//限制
	if (skill->iActorUseNum && skill->iActorUseNum <= oppo_actor->mSkillBeUseNum[skill->skill_id])
	{
		gain->land_strict = 1;
	}

    BattleControler::getBattleActor(oppo_actor->actor_id)->hp_gain = gain->gain_hp;
    BattleControler::getBattleActor(oppo_actor->actor_id)->mp_gain = gain->gain_mp;
    BattleControler::getBattleActor(oppo_actor->actor_id)->cur_probability = gain->probability;
	return gain;
}

int GetJobPKHartnum(int occutype, int oppo_occutype, bool isSkill)
{
	if(!isSkill) //如果是技能伤害第一个参数为法术类型则不需要+1；
	{
		occutype += 1;
	}
	int hartnum1 = 100, hartnum2 = 100;
	for (int i = 0; i < trData->jobpk_num; i++)
	{
		if (oppo_occutype + 1 == trData->jobpk[i].cidpk)
		{
			if (occutype == trData->jobpk[i].cid)
			{
				hartnum1 = trData->jobpk[i].hart;
			}
			else if (999 == trData->jobpk[i].cid && isSkill)
			{
				hartnum2 = trData->jobpk[i].hart;
			}
		}
	}
	return hartnum1 * hartnum2 / 100;
}

void setLossBuff(ActionSingleGain* ptrGain, float fPercent)
{
	int arrBuff[] = {2, 3, 4, 5, 20, 21};
	int iBuffId = arrBuff[rand() % 6];

	if (rand() % 100 < (int)fPercent)
	{
		switch(iBuffId)
		{
		case 2:
			ptrGain->state_obfuscation = -1;
			break;
		case 3:
			ptrGain->state_stun = -1;
			break;
		case 4:
			ptrGain->state_weaked = -1;
			break;
		case 5:
			ptrGain->state_blind = -1;
			break;
		case 20:
			ptrGain->state_loseblood = -1;
			break;
		case 21:
			ptrGain->state_poison = -1;
			break;
		default:
			break;
		}
	}
}