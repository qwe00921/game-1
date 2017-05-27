#include "Common.h"
#include "main.h"
#include "BattleAI.h"
#include "BattleAIExt.h"
#include "BattleScreen.h"
#include "WinConditionTool.h"
#include "BattleReplay.h"
#define UPATE_TOTAL_HP_HURT(totalGain, singalGain) {\
    totalGain->total_hp_hurt -= singalGain->gain_hp;\
    totalGain->gains->addElement(singalGain); \
}

MovePath::MovePath(const AStarNode* node)
{
	point.x = node->x;
	point.y = node->y;
	path = new DList<DPoint>(true);
	m_iUsedSpeed = node->step;
}

MovePath::~MovePath()
{
	SAFE_DELETE(path);
}

///////////////////////////////////////////////////////////////////////////////////////////

BattleAI::BattleAI()
{
	mostGain = nullptr;
	gainList = new DList<ActionGain>(false);
	curActor = nullptr;
	need_back_attack_twice = 0;//反击可以触发双击
	need_twice = 0;
	need_xianshou = 0;
	need_back_back = 0;
	need_induct = 0;
	induct_num = 0;//当前引导次数
	bravely_fight_num = 0;//当前奋战次数
	need_intensify_back_hit = 0;//强化反击百分比
	need_bravely_fight = 0;//奋战攻击
	need_pursuit_attack = 0;//追击攻击	
	need_assault_attack = 0;//突击攻击
	skill_need_twice = 0;  //法术连击
	skill_need_follow = 0; //法术追击
	skill_need_back = 0; //法术反击
	need_attact_all_enemy = 0;//攻击范围内所有人攻击一次
	need_attact_all_enemy_num = 0;//攻击范围内所有人攻击一次攻击到第几人
	skill_double_hit = 0;//法术技能连击
	need_siege = 0;
	is_xianshou = 0;//标记先手攻击
	bear_hand_attack = 0;//使自己后退一格，被攻击者同方向移动一格
	two_order_spike = 0;//二阶突刺
	new_assault = 0;//使自己和被攻击者都前进一格
	need_follow_probably = 0; // 几率追击的概率
	need_follow_hit_times = 0; // 几率追击N次攻击
	through_the_target = 0;//穿越攻击
	need_attack_sweep = 0;//横扫
	actor_can_drag = 0;//拖敌人到身前
	skill_dispatch_actor = 0;//遣将
	need_vampire_attack = 0;//攻击伤害
	attack_type = ATTACK_TYPE_NORMAL;
	skill_type = SKILL_TYPE_NORMAL; //技能特效
	skill_need_deally = 0; //法术暴击
	iActiveSkillId = -1; //主动使用技能
	memset(need_pursuit_attack_actor, 0, sizeof(need_pursuit_attack_actor));
	memset(skill_need_follow_actor, 0, sizeof(need_pursuit_attack_actor));
	this->skill_dispatch_actor = 0;
}

BattleAI::~BattleAI()
{
	SAFE_DELETE(mostGain);
	for (int i = gainList->size() - 1; i >= 0; i--)
	{
		ActionGainPtr curGain = gainList->removeElementAt(i);
		SAFE_DELETE(curGain);
	}
	SAFE_DELETE(gainList);
}

void BattleAI::readBattleAI(DMemoryReader* reader)
{
	int iActorId = -1;
	reader->tryReadInt(iActorId);
	if (iActorId != -1)
	{
		curActor = &history->actors[iActorId];
	}
	reader->tryReadInt(need_back_attack_twice);//反击可以触发双击
	reader->tryReadInt(need_twice);
	reader->tryReadInt(need_xianshou);
	reader->tryReadInt(need_back_back);
	reader->tryReadInt(need_induct);
	reader->tryReadInt(induct_num);//当前引导次数
	reader->tryReadInt(bravely_fight_num);//当前奋战次数
	reader->tryReadInt(need_intensify_back_hit);//强化反击百分比
	reader->tryReadInt(need_bravely_fight);//奋战攻击
	reader->tryReadInt(need_pursuit_attack);//追击攻击	
	reader->tryReadInt(need_assault_attack);//突击攻击
	reader->tryReadInt(skill_need_twice);  //法术连击
	reader->tryReadInt(skill_need_follow); //法术追击
	reader->tryReadInt(skill_need_back); //法术反击
	reader->tryReadInt(need_attact_all_enemy);//攻击范围内所有人攻击一次
	reader->tryReadInt(need_attact_all_enemy_num);//攻击范围内所有人攻击一次攻击到第几人
	reader->tryReadInt(skill_double_hit);//法术技能连击
	reader->tryReadInt(need_siege);
	reader->tryReadInt(is_xianshou);//标记先手攻击
	reader->tryReadInt(bear_hand_attack);//使自己后退一格，被攻击者同方向移动一格
	reader->tryReadInt(two_order_spike);//二阶突刺
	reader->tryReadInt(new_assault);//使自己和被攻击者都前进一格
	reader->tryReadInt(need_follow_probably); // 几率追击的概率
	reader->tryReadInt(need_follow_hit_times); // 几率追击N次攻击
	reader->tryReadInt(through_the_target);//穿越攻击
	reader->tryReadInt(need_attack_sweep);//横扫
	reader->tryReadInt(actor_can_drag);//拖敌人到身前
	reader->tryReadInt(skill_dispatch_actor);//遣将
	reader->tryReadInt(need_vampire_attack);//攻击伤害
	reader->tryReadInt(attack_type);
	reader->tryReadInt(skill_type); //技能特效
	reader->tryReadInt(skill_need_deally); //法术暴击
	reader->tryReadInt(iActiveSkillId); //主动使用技能
	int iPursuitSize = 0;
	reader->tryReadInt(iPursuitSize);//追击攻击	
	for (int i = 0; i < iPursuitSize; i++)
	{
		reader->tryReadInt(need_pursuit_attack_actor[i]);
	}
	int iSkillFollow = 0;
	reader->tryReadInt(iSkillFollow);//法术追击	
	for (int i = 0; i < iSkillFollow; i++)
	{
		reader->tryReadInt(skill_need_follow_actor[i]);
	}
	reader->tryReadMapInt(bravely_fight_oppo_actor);//奋战攻击的目标id集合
	reader->tryReadMapInt(skill_bravely_fight);//法术奋战 0:使能 1:已奋战人数 2:不能奋战的武将id
	reader->tryReadMapInt(need_spontaneous_detonation);//自爆
}

void BattleAI::saveBattleAI(DMemoryWriter* writer)
{
	writer->writeInt(history->cur_actor ? history->cur_actor->actor_id : -1);
	writer->writeInt(need_back_attack_twice);//反击可以触发双击
	writer->writeInt(need_twice);
	writer->writeInt(need_xianshou);
	writer->writeInt(need_back_back);
	writer->writeInt(need_induct);
	writer->writeInt(induct_num);//当前引导次数
	writer->writeInt(bravely_fight_num);//当前奋战次数
	writer->writeInt(need_intensify_back_hit);//强化反击百分比
	writer->writeInt(need_bravely_fight);//奋战攻击
	writer->writeInt(need_pursuit_attack);//追击攻击	
	writer->writeInt(need_assault_attack);//突击攻击
	writer->writeInt(skill_need_twice);  //法术连击
	writer->writeInt(skill_need_follow); //法术追击
	writer->writeInt(skill_need_back); //法术反击
	writer->writeInt(need_attact_all_enemy);//攻击范围内所有人攻击一次
	writer->writeInt(need_attact_all_enemy_num);//攻击范围内所有人攻击一次攻击到第几人
	writer->writeInt(skill_double_hit);//法术技能连击
	writer->writeInt(need_siege);
	writer->writeInt(is_xianshou);//标记先手攻击
	writer->writeInt(bear_hand_attack);//使自己后退一格，被攻击者同方向移动一格
	writer->writeInt(two_order_spike);//二阶突刺
	writer->writeInt(new_assault);//使自己和被攻击者都前进一格
	writer->writeInt(need_follow_probably); // 几率追击的概率
	writer->writeInt(need_follow_hit_times); // 几率追击N次攻击
	writer->writeInt(through_the_target);//穿越攻击
	writer->writeInt(need_attack_sweep);//横扫
	writer->writeInt(actor_can_drag);//拖敌人到身前
	writer->writeInt(skill_dispatch_actor);//遣将
	writer->writeInt(need_vampire_attack);//攻击伤害
	writer->writeInt(attack_type);
	writer->writeInt(skill_type); //技能特效
	writer->writeInt(skill_need_deally); //法术暴击
	writer->writeInt(iActiveSkillId); //主动使用技能
	int iPursuitSize = 10;
	writer->writeInt(iPursuitSize);
	for (int i = 0; i < iPursuitSize; i++)
	{
		writer->writeInt(need_pursuit_attack_actor[i]);
	}
	int iSkillFollow = 10;
	writer->writeInt(iSkillFollow);
	for (int i = 0; i < iSkillFollow; i++)
	{
		writer->writeInt(skill_need_follow_actor[i]);
	}
	writer->writeMapInt(bravely_fight_oppo_actor);//奋战攻击的目标id集合
	writer->writeMapInt(skill_bravely_fight);//法术奋战 0:使能 1:已奋战人数 2:不能奋战的武将id
	writer->writeMapInt(need_spontaneous_detonation);//自爆
}

void BattleAI::GenerateRealGain(){
	if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
	{
		mostGain = BattleReplay::getInstance()->getActionGain();
	}
	else
	{
		GenerateRealGainOld();
		if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_SAVE)
		{
			BattleReplay::getInstance()->saveActionGain(mostGain);
		}
	}
}
void BattleAI::GenerateRealGainOld() {
    if(mostGain && mostGain->skill) { //不产生伤害的特殊法术的处理
        SkillAttrPtr skill = mostGain->skill;
        //每关只能使用一次法术限制
        if (skill->skill_limit==11 || skill->skill_limit == 31) {
            curActor->used_skill_ids[skill->skill_id] = 1;
        }
        if(skill->seid == 68) //增加回合数
        {
            history->SetMaxRound(-1,12);
            mostGain->gain_mp -= mostGain->need_mp;
            return;
        }
    }
	if (curActor->good_effects[ATTACK_AFTER_MOVE] && history->cur_actor->actor_id == curActor->actor_id
		&&((BattleActor::moveList != NULL&& BattleActor::moveList->elementAt(0)->point.x == history->cur_actor->x&& BattleActor::moveList->elementAt(0)->point.y == history->cur_actor->y)||
		BattleActor::moveList==NULL)
		&&(!history->cur_actor->good_twice_action|| history->cur_actor->good_twice_action&&history->cur_actor->iTwiceAction))
		history->cur_actor->actor_only_move = 1;
    // 真实伤害计算
	auto _layer = Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") ;
	if (IsDeadlyAttack() && _layer == NULL)
	{
		attack_type |= ATTACK_TYPE_DEADLY;
		
			mostGain->deadly_attack = 1;
		
	}
	else
	{
		mostGain->deadly_attack = 0;
	}

	int real_total_hp_hurt = 0;  //需要反弹的总伤害
	for (int i = 0; i < mostGain->gains->size(); i++)
	{
		int ran = rand() % 5 - 2;//随机伤害浮动
		ActionSingleGainPtr curSingleGain = mostGain->gains->elementAt(i);
		//////借力打力技能
		if (curSingleGain->actor_leveraging_power)
		{
			//减伤
			if (curSingleGain->oppo_actor->isHadSpecial(TYPE764) && mostGain->good == NULL && curSingleGain->gain_hp < 0)
			{
				int iPre = curSingleGain->oppo_actor->getSpecialMap(TYPE764)[0];
				if (iPre >= 100) iPre = 100;
				curSingleGain->gain_hp -= curSingleGain->gain_hp * iPre / 100;
			}

			if (-curSingleGain->gain_hp > curSingleGain->oppo_actor->GetCurHp())
				curSingleGain->gain_hp = -curSingleGain->oppo_actor->GetCurHp();
			continue;
		}
		//////
		switch (attack_type & 0x1f)
		{
		case ATTACK_TYPE_NORMAL:
			break;
		case ATTACK_TYPE_BACK:
		case ATTACK_TYPE_BACK_BACK:
			curSingleGain->gain_hp = curSingleGain->gain_hp * 8 / 10;
			if (need_intensify_back_hit)//强化反击
			{
				curSingleGain->gain_hp += curSingleGain->gain_hp*need_intensify_back_hit/ 100;
				need_intensify_back_hit = 0;
			}
			if (curActor->light_refle)//强化反击
			{
				curSingleGain->gain_hp += curSingleGain->gain_hp*curActor->light_refle / 100;
				curActor->light_refle = 0;
			}

			if (curSingleGain->oppo_actor->isHadSpecial(TYPE728) && (
				(history->curActionSide == 2 && curSingleGain->oppo_actor->relation == 2)
								|| (history->curActionSide != 2 && curSingleGain->oppo_actor->relation != 2))
				)
			{
				curSingleGain->gain_hp -= curSingleGain->gain_hp*curSingleGain->oppo_actor->getSpecialInt(TYPE728) / 100;
			}

			break;
		case ATTACK_TYPE_TWICE:
			curSingleGain->gain_hp = curSingleGain->gain_hp * (80+ curActor->getSpecialInt(TYPE751)) / 100;
			if (curSingleGain->oppo_actor->good_hold_twice_hit|| curSingleGain->oppo_actor->isHadSpecial(TYPE751)){
				curSingleGain->probability = 0;//防御二次攻击
			}

			break;
		case ATTACK_TYPE_THIRD:
			curSingleGain->gain_hp = curSingleGain->gain_hp * 5 / 10;
			break;
		case ATTACK_TYPE_PURSUIT_ATTACK:
			curSingleGain->gain_hp = curSingleGain->gain_hp * curActor->getSpecialInt(TYPE114) / 100;
			break;
		case ATTACK_TYPE_ATTACK_SWEEP:
			curSingleGain->gain_hp = -curSingleGain->oppo_actor->GetCurHp() - 2;
			break;
		}
		if (attack_type&ATTACK_TYPE_DEADLY)
		{
			curSingleGain->gain_hp = curSingleGain->gain_hp * 15 / 10;
			if (curActor->good_intensify_deadlyattack)//强化暴击
			{
				curSingleGain->gain_hp += curSingleGain->gain_hp*curActor->good_intensify_deadlyattack / 100;
			}
			if (curSingleGain->oppo_actor->good_hold_deadliness_hit)
			{
				curSingleGain->probability = 0;//防御致命一击
			}

			if (curSingleGain->oppo_actor->isHadSpecial(TYPE706)) //弱化致命一击
			{
				curSingleGain->gain_hp -= curSingleGain->gain_hp * curSingleGain->oppo_actor->getSpecialInt(TYPE706) / 100;
			}
		}
		////伤害限制(优先级最高)
		if (curSingleGain->oppo_actor->isHadSpecial(TYPE421)  && mostGain->good == NULL) //最大伤害为生命值的%
		{
			int iGainMaxHp = curSingleGain->oppo_actor->getSpecialInt(TYPE421) * (curSingleGain->oppo_actor->GetBaseHp() + curSingleGain->oppo_actor->GetGoodsAddHp()) / 100;
			if (-curSingleGain->gain_hp>iGainMaxHp)
				curSingleGain->gain_hp = -iGainMaxHp;
		}

		////伤害限制(优先级最高)
		if (curSingleGain->oppo_actor->isHadSpecial(TYPE752) && mostGain->good == NULL && -curSingleGain->gain_hp > curSingleGain->oppo_actor->GetCurHp() * curSingleGain->oppo_actor->getSpecialInt(TYPE752) / 100) //最大伤害为生命值的%
		{
			int iGainReduceHp = curSingleGain->oppo_actor->getSpecialMap(TYPE752)[1] * curSingleGain->gain_hp / 100;
			curSingleGain->gain_hp -= iGainReduceHp;

			if (curSingleGain->gain_hp > 0)
				curSingleGain->gain_hp = 0;
		}

		switch (skill_type)
		{
		case SKILL_TYPE_NORMAL:
			break;
		case SKILL_TYPE_TWICE:
				curSingleGain->gain_hp = curSingleGain->gain_hp * curActor->good_magic_twice / 100;
			mostGain->need_mp = 0;
			break;
		case SKILL_TYPE_DEALLY:
			break;
		case SKILL_TYPE_FOLLOW:
			if (curActor->isHadSpecial(TYPE767))
			{
				curSingleGain->gain_hp = curSingleGain->gain_hp * curActor->getSpecialInt(TYPE767) / 100;
			}
			else
			{
				curSingleGain->gain_hp = curSingleGain->gain_hp * trData->m_mSeid[curActor->good_effects[22]]->special_effe[1] / 100;
			}
			break;
		case SKILL_TYPE_BACK:
			curSingleGain->gain_hp = curSingleGain->gain_hp * trData->m_mSeid[curActor->good_magic_back]->special_effe[1] / 100;
			break;
		case SKILL_TYPE_BRAVELY:
			mostGain->need_mp = 0;
		default:
			break;
		}

		if (skill_need_deally)
		{
			curSingleGain->gain_hp = curSingleGain->gain_hp * curActor->good_magic_deadly / 100;
			skill_need_deally = 0;
		}

		if (curSingleGain->gain_hp != 0 && mostGain->good == NULL) {
			if (curSingleGain->gain_hp < 0 && curSingleGain->gain_hp + ran >= 0)
				curSingleGain->gain_hp = -1;
			else
				curSingleGain->gain_hp += ran;
			if (mostGain->gain_hp != 0)
				mostGain->gain_hp -= ran;
		}

		if (curActor->good_hit_fixharm&&curSingleGain->gain_hp < 0 && (mostGain->good == NULL) && (mostGain->skill == NULL))//破甲攻击
		{
			if (curActor->weapon.isNormalGood())
			{
				curSingleGain->gain_hp = -curActor->good_hit_fixharm*curActor->weapon.GetRealLevel() - curActor->good_hit_fixharm_begin;
				if (attack_type&ATTACK_TYPE_DEADLY)
				{
					curSingleGain->gain_hp = curSingleGain->gain_hp * 15 / 10;
				}
				
			}
		}

		if (curActor->good_effects[13] && need_assault_attack == -1 && mostGain->good == NULL && mostGain->skill == NULL&&i == 0)//突击攻击击退被挡住时
		{
			curSingleGain->gain_hp += curSingleGain->gain_hp*(trData->m_mSeid[curActor->good_effects[13]]->special_effe[0]) / 100;
		}
		if (curActor->good_effects[BERA_HAND] && bear_hand_attack == -1 && mostGain->good == NULL && mostGain->skill == NULL&&i == 0)//熊手攻击后退被挡住时
		{
			curSingleGain->gain_hp += curSingleGain->gain_hp*trData->m_mSeid[curActor->good_effects[BERA_HAND]]->special_effe[0] / 100;
		}
		if (!curSingleGain->oppo_actor->isHadSpecial(TYPE743) && curActor->good_effects[TWOORDER_SPIKE] && two_order_spike > 0 && mostGain->good == NULL && mostGain->skill == NULL&&i == 0)//二阶突进
		{
			curSingleGain->gain_hp += curSingleGain->gain_hp*trData->m_mSeid[curActor->good_effects[TWOORDER_SPIKE]]->special_effe[0] * two_order_spike / 100;
		}
		if (curActor->good_effects[NEW_ASSAULT] && new_assault > 0 && mostGain->good == NULL && mostGain->skill == NULL&&i == 0)//二阶突进
		{
			curSingleGain->gain_hp += curSingleGain->gain_hp*curActor->good_effects[NEW_ASSAULT] / 100;
		}

		if((rand() % 100 < curActor->good_killed_hurt || curActor->good_effects[35]) && curSingleGain->gain_hp<0 && curSingleGain->oppo_actor->actor_id != curActor->actor_id)//一刀秒
		{
			curSingleGain->oppo_actor->SetCurHp(0);
		}
//////////////////////////////////////////////////////////////test//////////////////////////////////////////////////////////////////////////////////////
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		if (win32Const->yiDaoMiao)
		{
			for(int i = 0; i < history->battleEnemyNum; i++)
			{
				history->battleEnemy[i]->SetCurHp(0);
			}
		}
		if (win32Const->mpEmpty)
		{
			for (int i = 0; i < history->battleFriendNum; i++)
			{
				history->battleFriend[i]->SetCurMp(0);
			}
			for (int i = 0; i < history->battleOwnNum; i++)
			{
				history->battleOwn[i]->SetCurMp(0);
			}
			for (int i = 0; i < history->battleEnemyNum; i++)
			{
				history->battleEnemy[i]->SetCurMp(0);
			}
		}

		if(win32Const->iSingleSeckill && !curActor->relation && curSingleGain->oppo_actor->actor_id != curActor->actor_id)
		{
			curSingleGain->oppo_actor->SetCurHp(0);
		}

		if(win32Const->iNolimitMove && !curActor->relation)
		{
			curActor->good_effects[36] = 1;
		}

#endif
//////////////////////////////////////////////////////////////test//////////////////////////////////////////////////////////////////////////////////////

		if(rand() % 100 < curActor->getSpecialInt(TYPE372) && !mostGain->good && curSingleGain->gain_hp < 0&&mostGain->skill == NULL&&mostGain->good == NULL) //双倍伤害
		{
			curSingleGain->gain_hp *= 2;
		}
		if(!mostGain->good && curSingleGain->gain_hp < 0 && curSingleGain->oppo_actor->state_weaked) //被虚弱武将伤害加成计算
		{
			curSingleGain->gain_hp += curSingleGain->gain_hp * curSingleGain->oppo_actor->weaked_num / 100;
		}

		//mp辅助防御
		if (curSingleGain->oppo_actor->good_mp_hold_hurt != 0 && curSingleGain->oppo_actor->GetCurMp() != 0 && (mostGain->good == NULL) && (mostGain->skill == NULL)) {
			curSingleGain->gain_mp = curSingleGain->gain_hp;// oppo_actor->cur_mp<?oppo_actor->cur_mp:gain->gain_hp;//在后面再计算精确值 TODO mp辅助防御百分比计算
			curSingleGain->gain_hp = 0;
		}
		//mp辅助防御(百分数)
		int isenable_ran_gain_mp = 0;
		if (curSingleGain->oppo_actor->good_effects[20] && curSingleGain->oppo_actor->GetCurMp() > 0 && (mostGain->good == NULL) && (mostGain->skill == NULL))
		{
			int temp_mp = curSingleGain->oppo_actor->GetCurMp() + 100 * curSingleGain->gain_hp / curSingleGain->oppo_actor->good_effects[20];
			if (temp_mp >= 0)
			{
				curSingleGain->gain_mp = 100 * curSingleGain->gain_hp / curSingleGain->oppo_actor->good_effects[20];
				curSingleGain->gain_hp = 0;
			}
			else
			{
				curSingleGain->gain_mp = -curSingleGain->oppo_actor->GetCurMp();
				//curSingleGain->gain_hp = curSingleGain->gain_hp + curSingleGain->oppo_actor->cur_mp*curSingleGain->oppo_actor->good_effects[20] / 100;
				isenable_ran_gain_mp = 1;
				curSingleGain->gain_hp = 0;
			}
		}

		//灵魂链接伤害计算
		int iActorSualLingGainHp = 0;
		if (curSingleGain->oppo_actor->state_sual_link && curSingleGain->gain_hp < 0 && mostGain->good == NULL)
		{
			int iLinkActorId = curSingleGain->oppo_actor->m_mActorBuff[ACTOR_BUFF_SUALLINK]->iCasterActorId;
			int iLinkBuffPercent = curSingleGain->oppo_actor->m_mActorBuff[ACTOR_BUFF_SUALLINK]->iPercent;
			HistoryActorPtr LinkActor = history->FindBattleRelationActor(curSingleGain->oppo_actor, iLinkActorId - 1);
			if (LinkActor != NULL)
			{
				int gainHP = curSingleGain->gain_hp;
				curSingleGain->gain_hp -= iLinkBuffPercent * gainHP / 100;
				iActorSualLingGainHp = iLinkBuffPercent * gainHP / 100;
			}
		}

		//借力打力伤害计算
		int iActorTransferGainHp = 0;
		HistoryActorPtr TransferHpMinActor = NULL;
		if (curSingleGain->oppo_actor->good_effects[TRANSFER_HURT] && curSingleGain->gain_hp < 0
			&& mostGain->good == NULL && mostGain->skill == NULL)//借力打力
		{
			TransferHpMinActor = curSingleGain->oppo_actor->RangeOfHPMinEnemy(curActor->actor_id);
			if (TransferHpMinActor != NULL)
			{
				iActorTransferGainHp = curSingleGain->gain_hp;
				curSingleGain->gain_hp -= curSingleGain->oppo_actor->good_effects[TRANSFER_HURT] * iActorTransferGainHp / 100;
			}
		}

		//判断是否触发警戒
		if(curSingleGain->gain_hp + curSingleGain->oppo_actor->GetCurHp() <= 0)
		{
			map<int, HistoryActor*> mAroundActor = curSingleGain->oppo_actor->getAroundActor(1, true);
			for(UInt i = 0; i < mAroundActor.size(); i++)
			{
				if(mAroundActor[i]->good_effects[SHARE_DAMAGE])
				{
					Seid* ptrSeid = trData->m_mSeid[mAroundActor[i]->good_effects[SHARE_DAMAGE]];
					if(ptrSeid->special_effe[2] > mAroundActor[i]->m_iShareTimes)
					{
						curSingleGain->gain_hp = curSingleGain->gain_hp * ptrSeid->special_effe[0] / 100;
						curSingleGain->share_actor = mAroundActor[i];
						mAroundActor[i]->m_iShareTimes++;
						break;
					}
				}
			}
		}

		//减伤
		if (curSingleGain->oppo_actor->isHadSpecial(TYPE764) && mostGain->good == NULL && curSingleGain->gain_hp < 0)
		{
			int iPre = curSingleGain->oppo_actor->getSpecialMap(TYPE764)[0];
			if (iPre >= 100) iPre = 100;
			curSingleGain->gain_hp -= curSingleGain->gain_hp * iPre / 100;
		}

		if(curSingleGain->share_actor)
		{
			if(-curSingleGain->gain_hp > curSingleGain->share_actor->GetCurHp())
			{
				curSingleGain->gain_hp = -curSingleGain->share_actor->GetCurHp();
			}
		}
		else if(-curSingleGain->gain_hp > curSingleGain->oppo_actor->GetCurHp())
		{
			curSingleGain->gain_hp = -curSingleGain->oppo_actor->GetCurHp();
		}

		if (curSingleGain->gain_hp + curSingleGain->oppo_actor->GetCurHp() > curSingleGain->oppo_actor->GetBaseHp() + curSingleGain->oppo_actor->GetGoodsAddHp())
		{
			curSingleGain->gain_hp = curSingleGain->oppo_actor->GetBaseHp() + curSingleGain->oppo_actor->GetGoodsAddHp() - curSingleGain->oppo_actor->GetCurHp();
		}
		
		if (curActor->good_effects[LIFE_RIGHTEOUSNESS] && mostGain->good == NULL&&mostGain->skill == NULL&&curSingleGain->gain_hp<0)
		{
			SeidPtr ptr_seid = trData->m_mSeid[curActor->good_effects[LIFE_RIGHTEOUSNESS]];
			if (-curSingleGain->gain_hp < ptr_seid->special_effe[0])
			{
				if (curActor->GetCurMp() - ptr_seid->special_effe[1] >= 0)
					mostGain->gain_mp -= ptr_seid->special_effe[1];
				else
				{
					mostGain->gain_mp -= curActor->GetCurMp();
					mostGain->gain_hp -= (ptr_seid->special_effe[1] - curActor->GetCurMp());
				}
				mostGain->need_back = 0;
			}
			else
			{
				mostGain->gain_mp += ptr_seid->special_effe[2];
			}
		}
		if (isenable_ran_gain_mp)//去掉浮动mp变化
		{
			curSingleGain->gain_mp = -curSingleGain->oppo_actor->GetCurMp();
		}

		if (-curSingleGain->gain_mp > curSingleGain->oppo_actor->GetCurMp()) {
			curSingleGain->gain_mp = -curSingleGain->oppo_actor->GetCurMp();
			if (mostGain->gain_mp != 0)
				mostGain->gain_mp = -curSingleGain->gain_mp;
		}
		if (curSingleGain->gain_mp + curSingleGain->oppo_actor->GetCurMp()>curSingleGain->oppo_actor->GetBaseMp() + curSingleGain->oppo_actor->GetGoodsAddMp())
		{
			curSingleGain->gain_mp = curSingleGain->oppo_actor->GetBaseMp() + curSingleGain->oppo_actor->GetGoodsAddMp() - curSingleGain->oppo_actor->GetCurMp();
			if(mostGain->need_hp)
				mostGain->need_hp = curSingleGain->gain_mp;
		}

        if(curActor->state_blind) /* 攻击者失明的时候，命中概率，降低50% */
            curSingleGain->probability = curSingleGain->probability / 2;

		if(curSingleGain->state_mustHit || (rand() % 100 < curSingleGain->probability || curSingleGain->oppo_actor->state_puzzle || curSingleGain->oppo_actor->state_blind || curSingleGain->oppo_actor->state_obfuscation))
		{
			curSingleGain->probability = 100;
		}
		else
		{
			curSingleGain->probability = 0;
		}

		//天空盔
		if (mostGain->good == NULL && curSingleGain->oppo_actor->isHadSpecial(TYPE762) && curSingleGain->gain_hp < 0 &&
			curSingleGain->oppo_actor->GetCurHp() < (curSingleGain->oppo_actor->GetBaseHp() + curSingleGain->oppo_actor->GetGoodsAddHp())*35/100
			&& rand() % 100 < 50)
		{
			curSingleGain->probability = 0;
		}

		if (curSingleGain->probability == 0)
		{
			curSingleGain->gain_mp = 0;
			mostGain->gain_mp = 0;
			curSingleGain->gain_hp = 0;
			mostGain->gain_hp = 0;
		}

		//庇护次数计算
		if(curSingleGain->share_actor && curSingleGain->share_actor->m_iShieldNum > 0)
		{
			if (curSingleGain->probability)
			{
				curSingleGain->share_actor->m_iShieldNum--;
			}
			else
			{
				curSingleGain->share_actor = nullptr;
			}
		}

		//闪电反射
		if (curSingleGain->oppo_actor->light_refle)
			curSingleGain->oppo_actor->light_refle = 0;
		if (curSingleGain->probability == 0 && mostGain->good == NULL && mostGain->skill == NULL&&curSingleGain->oppo_actor->good_effects[LIGHT_REFLE])
		{
			curSingleGain->oppo_actor->light_refle = curSingleGain->oppo_actor->good_effects[LIGHT_REFLE];
		}
		//鲜血护盾
		if (curActor->good_effects[BLOOD_SHIELD] && i==0 && curSingleGain->gain_hp < 0 && curActor->relation == history->curActionSide&&mostGain->skill==NULL&&mostGain->good==NULL)
		{
			curActor->blood_shield += -curSingleGain->gain_hp*curActor->good_effects[BLOOD_SHIELD]/100;
		}
		if (curSingleGain->oppo_actor->blood_shield &&curSingleGain->gain_hp < 0 &&
			(curSingleGain->oppo_actor->relation == 2 && history->curActionSide != 2 || curSingleGain->oppo_actor->relation != 2 && history->curActionSide == 2))
		{
			if (curSingleGain->oppo_actor->blood_shield >= (int)abs(curSingleGain->gain_hp))
			{
				curSingleGain->oppo_actor->blood_shield += curSingleGain->gain_hp;
				curSingleGain->gain_hp = 0;
			}
			else
			{
				curSingleGain->gain_hp += curSingleGain->oppo_actor->blood_shield;
				curSingleGain->oppo_actor->blood_shield = 0;
			}
		}
		//免受伤害
		if (curSingleGain->oppo_actor->good_effects[VITAL_SPIRIT] && curSingleGain->probability&& curSingleGain->oppo_actor->good_effects[VITAL_SPIRIT] > curSingleGain->oppo_actor->vital_spirit&&curSingleGain->gain_hp<0)
		{
			curSingleGain->gain_hp = 0;
			if (curSingleGain->gain_mp<0)
				curSingleGain->gain_mp = 0;
			curSingleGain->oppo_actor->vital_spirit++;
		}

		if (!curActor->isHadSpecial(TYPE729))
		{
			real_total_hp_hurt -= (curSingleGain->gain_hp * curSingleGain->oppo_actor->good_hurt_back / 100);
		}

		//判断庇护光环
		if (curSingleGain->oppo_actor->isHadSpecial(TYPE731))
		{
			int iPecent = curSingleGain->oppo_actor->getSpecialMap(TYPE731)[0];
			if(curSingleGain->oppo_actor->GetCurHp() + curSingleGain->gain_hp >= curSingleGain->oppo_actor->getTotalAbility(TOTAL_HP) * iPecent / 100)
			{
				curSingleGain->state_shield_range = 1;
			}
			else
			{
				curSingleGain->state_shield_range = -1;
			}
		}


		if (mostGain->skill==NULL&&mostGain->good == NULL&&curSingleGain->oppo_actor->state_SkillHurtAndRebound)
		{
			real_total_hp_hurt -= (curSingleGain->gain_hp * curSingleGain->oppo_actor->m_mActorBuff[ACTOR_BUFF_FIRESHIELD]->iPercent / 100);
		}
		if(real_total_hp_hurt < 0)
		{
			real_total_hp_hurt = 0;
		}

		if (curSingleGain->probability == 0)
		{
			curSingleGain->state_power = 0;
			curSingleGain->state_defence = 0;
			curSingleGain->state_fortune = 0;
			curSingleGain->state_obfuscation = 0;
			curSingleGain->state_intellegence = 0;
			curSingleGain->state_speed = 0;
			curSingleGain->state_explosion = 0;
			curSingleGain->state_palsy = 0;
			curSingleGain->state_poison = 0;
			curSingleGain->state_puzzle = 0;
			curSingleGain->state_blind = 0;
			curSingleGain->state_stun = 0;
			curSingleGain->state_weaked = 0;
			curSingleGain->state_skillhurt = 0;
			curSingleGain->state_loseblood = 0;
			curSingleGain->state_addhp = 0;
			curSingleGain->state_reduceSkill = 0;
			curSingleGain->state_reduceHurt = 0;
			curSingleGain->state_emergency_command = 0;
		}

		if (curSingleGain->oppo_actor->good_effects[TRANSFER_HURT] && curSingleGain->gain_hp < 0&&TransferHpMinActor)//借力打力
		{
			ActionSingleGainPtr temp_gain = new ActionSingleGain;
			temp_gain->oppo_actor = TransferHpMinActor;
			temp_gain->gain_hp = curSingleGain->oppo_actor->good_effects[TRANSFER_HURT] * iActorTransferGainHp / 100;
			temp_gain->probability = 100;
			temp_gain->actor_leveraging_power = 1;
			mostGain->gains->addElement(temp_gain);
		}

		//灵魂链接触发效果
		if (curSingleGain->oppo_actor->state_sual_link && curSingleGain->gain_hp < 0 && iActorSualLingGainHp<0)
		{
			int iLinkActorId = curSingleGain->oppo_actor->m_mActorBuff[ACTOR_BUFF_SUALLINK]->iCasterActorId;
			HistoryActorPtr LinkActor = &history->actors[iLinkActorId - 1];
			if (LinkActor != NULL)
			{
				ActionSingleGainPtr temp_gain = new ActionSingleGain;
				temp_gain->oppo_actor = LinkActor;
				temp_gain->gain_hp = iActorSualLingGainHp;
				temp_gain->probability = 100;
				temp_gain->actor_leveraging_power = 1;
				mostGain->gains->addElement(temp_gain);
			}
		}

		//771
		if (curSingleGain->probability&&curSingleGain->gain_hp < 0 && (curActor->relation != 2 && curSingleGain->oppo_actor->relation == 2
			|| (curActor->relation == 2 && curSingleGain->oppo_actor->relation != 2)))
		{
			map<int,int>mTEff = curSingleGain->oppo_actor->getRangeBuffActorAndNum(RANGE_BE_ATTACK_ADD_MP, true);

			if (mTEff[0] > 0)
			{
				ActionSingleGainPtr temp_gain = new ActionSingleGain;
				temp_gain->oppo_actor = &history->actors[mTEff[0] - 1];
				temp_gain->gain_mp = mTEff[1];
				temp_gain->probability = 100;
				temp_gain->actor_leveraging_power = 1;
				mostGain->gains->addElement(temp_gain);

				if (curSingleGain->gain_mp + curSingleGain->oppo_actor->GetCurMp() > curSingleGain->oppo_actor->GetBaseMp() + curSingleGain->oppo_actor->GetGoodsAddMp())
				{
					curSingleGain->gain_mp = curSingleGain->oppo_actor->GetBaseMp() + curSingleGain->oppo_actor->GetGoodsAddMp() - curSingleGain->oppo_actor->GetCurMp();
				}
			}
		}

		////反弹异常攻击
		if (curSingleGain->oppo_actor->good_effects[39] && curSingleGain->gain_hp < 0
			&& mostGain->good == NULL && mostGain->skill == NULL)
		{
			BounceAbnormalAttack(mostGain, curActor, curSingleGain);
		}
		//仇恨攻击---新增命中率判断、格挡不计算在内
		if (curSingleGain->oppo_actor->isHadSpecial(TYPE383) && curSingleGain->probability  && (curSingleGain->oppo_actor->relation == 2 && curActor->relation != 2 || curSingleGain->oppo_actor->relation != 2 && curActor->relation == 2)
			&& (curSingleGain->oppo_actor->relation == 2 && history->curActionSide != 2 || curSingleGain->oppo_actor->relation != 2 && history->curActionSide == 2)
			&& mostGain->good == NULL && mostGain->skill == NULL)
		{
			HistoryActor* _oppoActor = curSingleGain->oppo_actor;
			if(_oppoActor->buff_hate_attack < _oppoActor->getSpecialInt(TYPE383) || _oppoActor->getSpecialInt(TYPE383) == 0)
			{
				_oppoActor->buff_hate_attack++;
			}
		}
		////火舞焰翔
		if (curActor->good_effects[HIT_INJURY]>0 && curSingleGain->probability > 0 && mostGain->good == NULL&&curSingleGain->gain_hp < 0)
			curActor->good_effects[HIT_INJURY] = -curActor->good_effects[HIT_INJURY];
		if (curSingleGain->oppo_actor->good_effects[HIT_INJURY] < 0 && curSingleGain->gain_hp < 0 && mostGain->good == NULL)
			curSingleGain->oppo_actor->good_effects[HIT_INJURY] = -curSingleGain->oppo_actor->good_effects[HIT_INJURY];
        /* 法术固定伤害，无视其他影响法术伤害值的原因，但不影响概率 */
        if (mostGain->skill != NULL && mostGain->skill->seid == 67) {
            curSingleGain->gain_hp = - mostGain->skill->power;
        }
        /* 法术固定伤害 * 等级 */
        if (mostGain->skill != NULL && mostGain->skill->seid == 76) {
            curSingleGain->gain_hp = - mostGain->skill->power * curActor->GetActorLv();
        }
		/////HP转化MP防御
		if (curSingleGain->oppo_actor->good_effects[HP_CHANGE_MP] && curSingleGain->gain_mp < 0 && curSingleGain->oppo_actor->GetCurMp() + curSingleGain->gain_mp <= 0 &&
			((history->curActionSide == ACTOR_RELATION_ENEMY && (curSingleGain->oppo_actor->relation == 0 || curSingleGain->oppo_actor->relation == 1)) ||
			(curSingleGain->oppo_actor->relation == 2 && (history->curActionSide == ACTOR_RELATION_OWN || history->curActionSide == ACTOR_RELATION_FRIEND)))
			&& curSingleGain->oppo_actor->hp_mp_defense_number < curSingleGain->oppo_actor->good_effects[HP_CHANGE_MP])
		{
			int mp_max = curSingleGain->oppo_actor->GetBaseMp() + curSingleGain->oppo_actor->GetGoodsAddMp();
			if (curSingleGain->oppo_actor->GetCurHp() + curSingleGain->gain_hp>mp_max)
			{
				curSingleGain->oppo_actor->hp_mp_defense_number++;
				ActionSingleGainPtr temp_gain = new ActionSingleGain;
				temp_gain->oppo_actor = curSingleGain->oppo_actor;
				temp_gain->gain_hp = -mp_max;
				temp_gain->gain_mp = 1;
				temp_gain->actor_leveraging_power = 1;
				mostGain->gains->addElement(temp_gain);
			}
		}

		//濒临死亡
		if (curSingleGain->border_on_death&&mostGain->skill==NULL&&mostGain->good==NULL)
		{
			curSingleGain->gain_hp = -(curSingleGain->oppo_actor->GetCurHp() - 1);
		}
		if (curSingleGain->oppo_actor->border_on_death&&curSingleGain->gain_hp<0)
		{
			curSingleGain->gain_hp = 0;
		}
		if (curSingleGain->oppo_actor->border_on_death&&curSingleGain->gain_hp > 0)
		{
			curSingleGain->state_poison = 1;
			curSingleGain->border_on_death = 1;
		}
		if (mostGain->skill && curSingleGain->mSeidAddGain[1] > 0)
		{
			if(curSingleGain->oppo_actor->isHadSpecial(TYPE724))
			{
				curSingleGain->oppo_actor->mSeidAddGain[TYPE724] = curSingleGain->mSeidAddGain[1];
			}
			else if(curSingleGain->share_actor && curSingleGain->share_actor->isHadSpecial(TYPE724))
			{
				curSingleGain->share_actor->mSeidAddGain[TYPE724] = curSingleGain->mSeidAddGain[1];
			}
		}

		if (mostGain->skill&&curActor->isHadSpecial(TYPE724) && curSingleGain->mSeidAddGain[0] > 0 && curActor->mSeidAddGain[TYPE724])
		{
			curActor->mSeidAddGain[TYPE724] = 0;
		}


		//杀敌后直接再杀一人
		if (curActor->isHadSpecial(TYPE725) && curSingleGain->oppo_actor->GetCurHp() + curSingleGain->gain_hp <= 0 && !mostGain->good && !mostGain->skill&&attack_type != ATTACK_TYPE_ATTACK_SWEEP)
		{
			need_attack_sweep = 1;
		}

		if (curSingleGain->oppo_actor->isHadSpecial(TYPE745) &&curSingleGain->oppo_actor->GetCurHp() + curSingleGain->gain_hp <= 0 && !mostGain->good /*&& (!mostGain->skill || mostGain->skill->seid != 128)*/)
		{
			need_spontaneous_detonation[curSingleGain->oppo_actor->actor_id+1] = 1;
		}
	}

	ActionSingleGainPtr curSingleGain = mostGain->gains->elementAt(0);
	if(curSingleGain != NULL)
	{
		if(curActor->good_induct_hit && !(attack_type&ATTACK_TYPE_BACK) && !(attack_type&ATTACK_TYPE_BACK_BACK))
		{
			HistoryActor* ptrActor = curSingleGain->oppo_actor;
			if(curSingleGain->share_actor)
			{
				ptrActor = curSingleGain->share_actor;
			}

			if(curSingleGain->gain_hp + ptrActor->GetCurHp() <= 0)
			{
				need_induct = 1;
			}
			else
			{
				need_induct = 0;
			}
		}
	}


	//伤害反弹
	if (real_total_hp_hurt)
	{
		mostGain->need_hp = real_total_hp_hurt;
		if (curActor->GetCurHp()>0 && mostGain->need_hp >= curActor->GetCurHp())
		{
			mostGain->need_hp = curActor->GetCurHp() - 1;
		}
	}
	mostGain->gain_hp -=mostGain->need_hp;
	if (curActor->border_on_death&&mostGain->gain_hp<0)
	{
		mostGain->gain_hp = 0;
	}
	else if (curActor->border_on_death&&mostGain->gain_hp>0)
	{
		curActor->border_on_death = 0;
		curActor->updateActorBuff(ACTOR_BUFF_POISON, 1, false);
	}

	if (mostGain->gain_hp > 0 && curActor->isHadSpecial(TYPE723))
	{
		int iSeidNum = curActor->getSpecialInt(TYPE723);
		mostGain->gain_hp += mostGain->gain_hp*iSeidNum/100;
	}

	if (curActor->isHadSpecial(TYPE745) && mostGain->gain_hp < 0 && mostGain->gain_hp + curActor->GetCurHp() <= 0 && !mostGain->good && (!mostGain->skill || mostGain->skill->seid != 128))
	{
		need_spontaneous_detonation[curActor->actor_id + 1] = 1;
	}

	if (mostGain->gain_hp + curActor->GetCurHp() >curActor->GetBaseHp() + curActor->GetGoodsAddHp())
		mostGain->gain_hp = curActor->GetBaseHp() + curActor->GetGoodsAddHp() - curActor->GetCurHp();
	mostGain->gain_mp -=mostGain->need_mp;
	if (mostGain->gain_mp + curActor->GetCurMp() >curActor->GetBaseMp() + curActor->GetGoodsAddMp())
		mostGain->gain_mp = curActor->GetBaseMp() + curActor->GetGoodsAddMp() - curActor->GetCurMp();

	attack_type = ATTACK_TYPE_NORMAL;
	skill_type = SKILL_TYPE_NORMAL;
}

void BattleAI::GenerateAttackType()
{
	ActionSingleGainPtr curSingleGain = mostGain->gains->elementAt(0);


	if (curSingleGain==NULL){
		return;
	}

	HistoryActorPtr oppo_actor = curSingleGain->oppo_actor;
//	OccupationAttrPtr curOccu = &data->occupations[oppo_actor->occupation_id];
	float cur_explosion = curActor->getRealAbility(ACTOR_ABILITY_EXPLOSION, true);
	float oppo_explosion = oppo_actor->getRealAbility(ACTOR_ABILITY_EXPLOSION, true);

	float rate = cur_explosion / oppo_explosion;
	float probably = 0;
	if(rate>=3)
	{
		probably = 100;
	}
	else if (rate >= 2)
	{
		probably = 80 * rate - 140;
	}
	else if (rate >= 1)
	{
		probably = 18 * rate - 16;
	}
	else
	{
		probably = 1;
	}

	////移动加双击率
	int move_add_twice = 0;
	if (curActor->good_effects[MOVE_ADDTWICE] && BattleActor::moveList != NULL&&BattleActor::move_index > 0)
	{
		move_add_twice += curActor->good_effects[MOVE_ADDTWICE];
	}
	probably += move_add_twice + curActor->good_effects[TWICEHITORDEADLY_RATE] + curActor->getSpecialInt(TYPE711);

	if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
	{
		if (BattleReplay::getInstance()->getBattleAI(false)->need_twice == 1)
		{
			need_twice = 1;
		}
	}
	else if(rand() % 100 < probably || curActor->good_hit_twice == 100)
	{
		need_twice = 1;
	}

	if(curActor->isHadSpecial(TYPE428)) //几率N攻击
	{
		map<int, int> mSpecial = curActor->getSpecialMap(TYPE428);
		need_follow_probably = mSpecial[0];
		if(need_follow_probably > rand() % 100)
		{
			need_follow_hit_times = mSpecial[1];
		}
	}

	if(curActor->good_hit_back_back&&mostGain->need_back)
		need_back_back = 1;

	if (oppo_actor->good_magic_back)
	{
		skill_need_back = 1;
	}

	if (curActor->good_effects[46])
	{
		need_attact_all_enemy = oppo_actor->actor_id + 1;
		need_attact_all_enemy_num = 0;
	}

	if(curSingleGain->actor_knockback == 1) //普攻被击退
	{
		need_assault_attack = 2;
	}

	if (curActor->isHadSpecial(TYPE109)&& !is_xianshou)
	{
		bravely_fight_oppo_actor.clear();
		int i;
		int fight_num = 0;//攻击范围内的敌人数量
		int attack_id = curActor->getAttackAreaId();
		HistoryActorPtr oppo_actor = NULL;
		int x, y;
		if (trData->hitArea[attack_id].length >= 0)
		{
			for (i = 0; i < trData->hitArea[attack_id].length; i++)
			{
				x = history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x;
				y = history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y;
				oppo_actor = history->FindOppoActorPosition(curActor, x, y);
				if (oppo_actor != NULL&&oppo_actor->GetCurHp() > 0&&!oppo_actor->hide) {
					fight_num++;
				}
			}
		}

		if (fight_num > 1) { //攻击范围内只有一个敌人时不触发奋战攻击
			need_bravely_fight = 1;
			bravely_fight_oppo_actor[curSingleGain->oppo_actor->actor_id + 1] = 1;
		}

	}
}
void BattleAI::GenerateSkillType(SkillAttrPtr skill)
{
	if (curActor->good_magic_twice&&mostGain->skill->power)
	{
		skill_need_twice = 1;
	}
	if (curActor->good_magic_deadly)
	{
		skill_need_deally = 1;
	}
	else
	{
		skill_need_deally = 0;
	}
}
void BattleAI::GenerateSkillType(ActionGainPtr ptrGain)
{
	if ((ptrGain->skill->seid == 101 || ptrGain->skill->seid == 105 ||ptrGain->skill->seid == 113 || ptrGain->skill->seid == 129|| ptrGain->skill->seid == 130)&& ptrGain->gains != NULL&&ptrGain->gains->size() != 0)
	{
		need_assault_attack = -1;
		for (int i = 0; i < ptrGain->gains->size(); i++)
		{
			ActionSingleGainPtr ptrCurSingleGain = (ActionSingleGainPtr)ptrGain->gains->elementAt(i);
			if (ptrCurSingleGain&&ptrCurSingleGain->probability)
			{
				if (ptrCurSingleGain->actor_knockback == 1)
				{
					need_assault_attack = 2;
					break;
				}
				else if (i == 0&&ptrCurSingleGain->through_the_target == 1)
				{
					through_the_target = 1;
					break;
				}
				else if (i == 0 && ptrCurSingleGain->new_assault == 1)
				{
					new_assault = 1;
					break;
				}
				else if (i == 0 && ptrCurSingleGain->actor_can_drag == 1)
				{
					actor_can_drag = 1;
				}
				else if( i == 0 && ptrCurSingleGain->skill_dispatch_actor)
				{
					skill_dispatch_actor = ptrCurSingleGain->oppo_actor->actor_id + 1;
				}
			}
		}
	}
}
void BattleAI::GenerateGain_AttackType(HistoryActorPtr actor,int x,int y,int attack_type)
{
	this->curActor = actor;
	this->attack_type = attack_type;
	int need_bac = mostGain->need_back;
//	int need_bac_bac = mostGain->need_back_back;
	ClearGain();
	mostGain = GetAttackGain(actor->x,actor->y,x,y);

	if (mostGain->gains->elementAt(0))
	{
		ActionSingleGain* ptrGain = mostGain->gains->elementAt(0);
		if(ptrGain->actor_knockback == 1) //普攻被击退
		{
			need_assault_attack = 2;
		}
	}

	if (attack_type != ATTACK_TYPE_BACK)
	{
		mostGain->need_back = need_bac;
	}
//	if(attack_type!=ATTACK_TYPE_BACK_BACK)
//		mostGain->need_back_back= need_bac_bac;
	GenerateRealGain();
}
void BattleAI::GenerateGain_SkillType(SkillAttrPtr curSkill, HistoryActorPtr actor, int x, int y, int skill_type)
{
	this->curActor = actor;
	this->skill_type = skill_type;
	ClearGain();
	mostGain = GetSkillGain(curSkill, actor->x, actor->y, x, y);

	GenerateRealGain();
}
void BattleAI::ClearGain()
{
	SAFE_DELETE(mostGain);
}
void BattleAI::ClearGainList()
{
	for(int i = gainList->size() - 1;i>=0;i--)
	{
		ActionGainPtr curGain = gainList->removeElementAt(i);
		SAFE_DELETE(curGain);
	}
	BattleControler::ClearGain();
	
}
void BattleAI::GenerateAttackGainList(int index)
{
	ClearGainList();
	int i,x,y;
	MovePathPtr movePoint = BattleActor::moveList->elementAt(index);
	//OccupationAttrPtr curOccu = &data->occupations[curActor->occupation_id];
	int attack_id = curActor->getAttackAreaId();
	HistoryActorPtr oppoActor;
	if(trData->hitArea[attack_id].length>=0)
	{
		for(i = 0;i<trData->hitArea[attack_id].length;i++)
		{
			x = movePoint->point.x + trData->hitArea[attack_id].pointlist[i].x;
			y = movePoint->point.y + trData->hitArea[attack_id].pointlist[i].y;
			oppoActor = history->FindOppoActorPosition(curActor,x,y);
			if(oppoActor && !oppoActor->good_invisible)
			{
				ActionGainPtr curGain = GetAttackGain(movePoint->point.x, movePoint->point.y, x, y);
				if (!oppoActor->state_puzzle && !oppoActor->state_blind&& !oppoActor->state_obfuscation && (curActor->good_hit_no_back != 100))
				{
					curGain->need_back = oppoActor->isAttackArea(curGain->point.x, curGain->point.y, true);
				}
				gainList->addElement(curGain);
			}
		}
	}
	else if(trData->hitArea[attack_id].length==-1)
	{
		BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		for(int i = 0; i < scr->pointNum; i++)
		{
			x = scr->pointList[i].x;
			y = scr->pointList[i].y;
			oppoActor = history->FindOppoActorPosition(curActor, x, y);
			if(oppoActor)
			{
				ActionGainPtr curGain = GetAttackGain(movePoint->point.x, movePoint->point.y, x, y);
				if(!oppoActor->state_puzzle && !oppoActor->state_blind&& !oppoActor->state_obfuscation && (curActor->good_hit_no_back != 100))
				{
					curGain->need_back = oppoActor->isAttackArea(curGain->point.x, curGain->point.y, true);
				}
				gainList->addElement(curGain);
			}
		}
	}
}

void BattleAI::GenerateGoodGainList(GoodsItem* good, int index)
{
	ClearGainList();
	MovePath* ptrMove = BattleActor::moveList->elementAt(index);
	map<int, DPoint> mHitPoint;
	int iLength = trData->hitArea[1].length;
	for (int i = 0; i < iLength; i++)
	{
		int x = ptrMove->point.x + trData->hitArea[1].pointlist[i].x;
		int y = ptrMove->point.y + trData->hitArea[1].pointlist[i].y;
		mHitPoint[i] = DPoint(x, y);
	}
	mHitPoint[iLength] = ptrMove->point;

	for (UInt i = 0; i < mHitPoint.size(); i++)
	{
		int x = mHitPoint[i].x;
		int y = mHitPoint[i].y;
		HistoryActorPtr ptrActor = history->FindOwnActorPosition(curActor, x, y);
		if(ptrActor)
		{
			ActionGainPtr curGain = new ActionGain();
			curGain->good = good;
			curGain->point.x = ptrMove->point.x;
			curGain->point.y = ptrMove->point.y;
			curGain->hitPoint.x = x;
			curGain->hitPoint.y = y;
			curGain->gains->addElement(CalcGoodSingleGain(curActor, ptrActor, good, curGain));
			gainList->addElement(curGain);
		}
	}
}

void BattleAI::GenerateSkillGainList(SkillAttrPtr skill, int index)
{
	ClearGainList();
	int i,x,y;
	MovePathPtr movePoint = BattleActor::moveList->elementAt(index);
//	OccupationAttrPtr curOccu = &data->occupations[curActor->occupation_id];
	HistoryActorPtr oppoActor;
	if(trData->hitArea[skill->use_region].length>=0)
	{
		for(i = 0;i<trData->hitArea[skill->use_region].length;i++)
		{
			x = movePoint->point.x + trData->hitArea[skill->use_region].pointlist[i].x;
			y = movePoint->point.y + trData->hitArea[skill->use_region].pointlist[i].y;
			if (skill->skill_object == 0){
				oppoActor = history->FindOppoActorPosition(curActor, x, y);
                if(oppoActor && oppoActor->good_invisible) /* 不能用法术攻击隐身人物 */
                    oppoActor = NULL;
			}
			else{
				oppoActor = history->FindOwnActorPosition(curActor, x, y);
			}
				
			if(skill->skill_object == 3 || skill->skill_object == 4 || skill->skill_object == 7 || skill->skill_object == 8)
			{
				if(x >= 0 && x < history->map_width && y >= 0 && y < history->map_height)
				{
					ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
					curGain->point.x = movePoint->point.x;
					curGain->point.y = movePoint->point.y;
					gainList->addElement(curGain);
				}
			}
			else if (oppoActor)
			{
				ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
				curGain->point.x = movePoint->point.x;
				curGain->point.y = movePoint->point.y;
				curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
				gainList->addElement(curGain);
			}
		}
		if (skill->skill_object != 0)
		{
			ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
			curGain->point.x = movePoint->point.x;
			curGain->point.y = movePoint->point.y;
			gainList->addElement(curGain);
		}
	}
	else if(trData->hitArea[skill->use_region].length==-1)
	{

		if (skill->skill_object == 3){
			ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
			curGain->point.x = movePoint->point.x;
			curGain->point.y = movePoint->point.y;
			gainList->addElement(curGain);

		}
		else if(skill->skill_object == 4) // 连续移动
		{
			BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
			for(int i = 0; i < scr->pointNum; i++)
			{
				x = scr->pointList[i].x;
				y = scr->pointList[i].y;
				ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
				curGain->point.x = movePoint->point.x;
				curGain->point.y = movePoint->point.y;
				gainList->addElement(curGain);
			}
		}
		else{

			//全屏法术处理
			int relation = 0;
			if ((skill->skill_object == 0 && curActor->relation == 2) ||
				(skill->skill_object != 0 && curActor->relation != 2))
			{
				relation = 1;
			}

			if (skill->type == 1 && curActor->relation != 2)
				relation = 0;
			else if(skill->type == 1 && curActor->relation == 2)
				relation = 1;

			if (relation)
			{
				for (i = 0; i < history->battleOwnNum; i++)
				{
					if (history->battleOwn[i]->hide != 1)
					{
						oppoActor = history->battleOwn[i];
						ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
						curGain->point.x = movePoint->point.x;
						curGain->point.y = movePoint->point.y;
						curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
						gainList->addElement(curGain);
					}
				}
				for (i = 0; i < history->battleFriendNum; i++)
				{
					if (history->battleFriend[i]->hide != 1)
					{
						oppoActor = history->battleFriend[i];
						ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
						curGain->point.x = movePoint->point.x;
						curGain->point.y = movePoint->point.y;
						curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
						gainList->addElement(curGain);
					}
				}
			}
			else
			{
				for (i = 0; i < history->battleEnemyNum; i++)
				{
					if (history->battleEnemy[i]->hide != 1)
					{
						oppoActor = history->battleEnemy[i];
						ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
						curGain->point.x = movePoint->point.x;
						curGain->point.y = movePoint->point.y;
						curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
						gainList->addElement(curGain);
					}
				}
			}
		}
	}
	else if(trData->hitArea[skill->use_region].length==-5)
	{
		//无目标法术
		ActionGainPtr curGain = new ActionGain();
		curGain->skill = skill;
		curGain->point.x = movePoint->point.x;
		curGain->point.y = movePoint->point.y;
		ActionSingleGainPtr curSingleGain = NULL;
		int relation = 0;
		if((skill->skill_object == 0&&curActor->relation == 2)||
			(skill->skill_object != 0&&curActor->relation != 2))
		{
			relation = 1;
		}

		if (skill->skill_object == 8)
		{
			curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
			curGain->point.x = movePoint->point.x;
			curGain->point.y = movePoint->point.y;
			if (curGain->gains->size() == 1 && curGain->gains->elementAt(0)->land_strict == 1)
			{
				SAFE_DELETE(curGain);
				return;
			}
		}
		else if (skill->type == 1)
		{
			curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
			if (!curGain->gains->size())
			{
				SAFE_DELETE(curGain);
				return;
			}
		}
		else if(relation)
		{
			if(skill->skill_object !=0)
			{
				oppoActor = curActor;
				curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
				curGain->gains->addElement(curSingleGain);
			}
			else
			{
				for(i = 0;i<history->battleOwnNum;i++)
				{
					if(history->battleOwn[i]->hide !=1)
					{
						oppoActor = history->battleOwn[i];
						curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
						if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
							curGain->actor_kill++;
						curGain->total_hp_hurt -= curSingleGain->gain_hp;
						curGain->gains->addElement(curSingleGain);
					}
				}
				for(i = 0;i<history->battleFriendNum;i++)
				{
					if(history->battleFriend[i]->hide !=1)
					{
						oppoActor = history->battleFriend[i];
						curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
						if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
							curGain->actor_kill++;
						curGain->total_hp_hurt -= curSingleGain->gain_hp;
						curGain->gains->addElement(curSingleGain);
					}
				}
			}
		}
		else
		{
			if(skill->skill_object !=0)
			{
				oppoActor = curActor;
				curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
				curGain->gains->addElement(curSingleGain);
			}
			else
			{
				for(i = 0;i<history->battleEnemyNum;i++)
				{
					if(history->battleEnemy[i]->hide !=1)
					{
						oppoActor = history->battleEnemy[i];
						curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
						if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
							curGain->actor_kill++;
						curGain->total_hp_hurt -= curSingleGain->gain_hp;
						curGain->gains->addElement(curSingleGain);
					}
				}
			}
		}
		gainList->addElement(curGain);
	}
	else if (trData->hitArea[skill->use_region].length == -9)
	{
		ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, curActor->x, curActor->y);
		curGain->point.x = movePoint->point.x;
		curGain->point.y = movePoint->point.y;
		if(curGain->gains->size() == 0)
		{
			ActionSingleGainPtr curSingleGain = new ActionSingleGain();
			curSingleGain->oppo_actor = curActor;
			curSingleGain->land_strict = 1;
			curGain->gains->addElement(curSingleGain);
		}
		gainList->addElement(curGain);
	}
}

void BattleAI::UpdateMostGain(ActionGainPtr mostGain,ActionGainPtr curGain,int index)
{
	bool trans = false;//是否把curGain赋给mostGain
	if (curActor->actor_only_move == 2)
		trans = false;
	else if(curGain->actor_kill>mostGain->actor_kill)
	{
		trans = true;
	}
	else if (mostGain->gains != NULL&&mostGain->gains->size() != 0 && mostGain->gains->elementAt(0)->oppo_actor != NULL&&
		mostGain->gains->elementAt(0)->oppo_actor->state_obfuscation&&curGain->gains!=NULL&&curGain->gains->size())
	{
		trans = true;
    }
    else if(mostGain->gains != NULL&&mostGain->gains->size() != 0 && mostGain->gains->elementAt(0)->oppo_actor != NULL&&
              !(mostGain->gains->elementAt(0)->oppo_actor->state_obfuscation) && curGain->gains!=NULL && curGain->gains->size()&& curGain->gains->elementAt(0)->oppo_actor != NULL &&
              curGain->gains->elementAt(0)->oppo_actor->state_obfuscation)
    {
        trans = false;
    }
	else if(curGain->actor_kill==mostGain->actor_kill)
	{
		if(curGain->next_round_gain_hp>mostGain->next_round_gain_hp
			&&curActor->GetCurHp()<(curActor->GetBaseHp() + curActor->GetGoodsAddHp()) / 10)
							trans = true;
		else
		{
			if (curGain->gains)//敌友军濒死
			{
				for (int i = 0; i < curGain->gains->size(); i++)
				{
					ActionSingleGainPtr gain = curGain->gains->elementAt(i);

					if (gain->oppo_actor && gain->oppo_actor->GetCurHp() < (gain->oppo_actor->GetBaseHp() + gain->oppo_actor->GetGoodsAddHp()) / 10 && gain->gain_hp != 0)
					{
						trans = true;
					}
				}
			}

			if (curGain->total_hp_hurt < 0)
			{
				if (curGain->gains)
				{
					curGain->total_hp_hurt = 0;
					for (int i = 0; i < curGain->gains->size(); i++)
					{
						//实际能获取到的加血
						ActionSingleGainPtr gain = curGain->gains->elementAt(i);
						if (gain->oppo_actor && gain->gain_hp > 0)
						{
							int gethp = gain->oppo_actor->GetBaseHp() + gain->oppo_actor->GetGoodsAddHp() - gain->oppo_actor->GetCurHp();
							if (gethp <= (int)abs(gain->gain_hp) && curGain->gains->size() == 1)
							{
								gain->gain_hp = 0;
							}
						}
						curGain->total_hp_hurt += (int)abs(gain->gain_hp);
					}

					if (curActor->GetActorOccuType() == 0) //将军类加血收益的效果计算弱化50%
					{
						curGain->total_hp_hurt /= 2;
					}
					curGain->cur_hp_hurt = curGain->total_hp_hurt;
				}
			}

            if(curGain->need_back<mostGain->need_back&&curGain->total_hp_hurt>0)
			{
				if (curActor->GetCurHp()<(curActor->GetBaseHp() + curActor->GetGoodsAddHp()) / 10)
				{
					trans = true;
				}
				if (mostGain->gains != NULL&&mostGain->gains->size() != 0 && mostGain->gains->elementAt(0)->oppo_actor != NULL)
				{
					//计算反击所受到的伤害
					HistoryActorPtr oppoActor = mostGain->gains->elementAt(0)->oppo_actor;
					ActionGainPtr obj_gain = GetAttackGain(oppoActor->x, oppoActor->y, curActor->x, curActor->y, oppoActor,false);
					//以输出伤害值减去受反击伤害值得到收益值，正数为大，负数为小，选择数值大的来行动。
					if (obj_gain == NULL)
					{
						if (mostGain->total_hp_hurt + mostGain->next_round_gain_hp <= curGain->total_hp_hurt + curGain->next_round_gain_hp)
						{
							trans = true;
						}
					}
					else if ((mostGain->total_hp_hurt - obj_gain->total_hp_hurt + mostGain->next_round_gain_hp) <= (curGain->total_hp_hurt + curGain->next_round_gain_hp))
					{
						trans = true;
					}
					SAFE_DELETE(obj_gain);
				}
				else
				{
					trans = true;
				}
				
			}
			else
			{
				
				if (curGain->total_hp_hurt>mostGain->total_hp_hurt)
				{
					trans = true;
				}
				else if (curGain->total_hp_hurt == mostGain->total_hp_hurt)
				{
					
					if (curGain->cur_hp_hurt>mostGain->cur_hp_hurt)
						trans = true;
					else
					{
						if(curGain->cur_hp_hurt==mostGain->cur_hp_hurt)
						{	
						}
					}
				}

				//可以攻击到对方的情况下，优先站在补血多的位置上
				if (curGain->next_round_gain_hp > mostGain->next_round_gain_hp&&curGain->total_hp_hurt>0){
					trans = true;
				}
				//以输出伤害值减去受反击伤害值得到收益值，正数为大，负数为小，选择数值大的来行动。
				if (curGain->need_back&&mostGain->gains != NULL&&mostGain->gains->size() != 0 && mostGain->gains->elementAt(0)->oppo_actor != NULL
					&&curGain->gains != NULL&&curGain->gains->size() != 0 && curGain->gains->elementAt(0)->oppo_actor != NULL)
				{
					//计算反击所受到的伤害
					ActionGainPtr obj_cur_gain = GetAttackGain(curGain->gains->elementAt(0)->oppo_actor->x, curGain->gains->elementAt(0)->oppo_actor->y, curActor->x, curActor->y, curGain->gains->elementAt(0)->oppo_actor,false);
					if (mostGain->need_back)
					{
						//计算反击所受到的伤害
						HistoryActorPtr oppoActor = mostGain->gains->elementAt(0)->oppo_actor;
						ActionGainPtr obj_most_gain = GetAttackGain(oppoActor->x, oppoActor->y, curActor->x, curActor->y, oppoActor, false);
						if(obj_most_gain&&obj_cur_gain)
						{
							if ((mostGain->total_hp_hurt - obj_most_gain->total_hp_hurt + mostGain->next_round_gain_hp) >= (curGain->total_hp_hurt - obj_cur_gain->total_hp_hurt + curGain->next_round_gain_hp))
							{
								trans = false;
							}
						}
						SAFE_DELETE(obj_most_gain);
					}
					else if (obj_cur_gain && ((mostGain->total_hp_hurt + mostGain->next_round_gain_hp) >= (curGain->total_hp_hurt - obj_cur_gain->total_hp_hurt + curGain->next_round_gain_hp)))
					{
						trans = false;
					}

					SAFE_DELETE(obj_cur_gain);
				}
			}
		}
	}


	if(trans)
	{
		SAFE_DELETE(mostGain->gains);
		mostGain->gains = curGain->gains;
		curGain->gains = NULL;
		mostGain->point.x = curGain->point.x;
		mostGain->point.y = curGain->point.y;
		mostGain->hitPoint.x = curGain->hitPoint.x;
		mostGain->hitPoint.y = curGain->hitPoint.y;
		mostGain->skill = curGain->skill;
		mostGain->actor_kill = curGain->actor_kill;
		mostGain->total_hp_hurt = curGain->total_hp_hurt;
		mostGain->cur_hp_hurt = curGain->cur_hp_hurt;
		mostGain->moveIndex = index;
		mostGain->need_back = curGain->need_back;
		mostGain->need_hp = curGain->need_hp;
		mostGain->need_mp = curGain->need_mp;
		mostGain->gain_hp = curGain->gain_hp;
		mostGain->gain_mp = curGain->gain_mp;
		mostGain->good = curGain->good;
		mostGain->need_back = curGain->need_back;
		mostGain->deadly_attack = curGain->deadly_attack;
		mostGain->next_round_gain_hp = curGain->next_round_gain_hp;
	}
	else
	{
		SAFE_DELETE(curGain->gains);
	}
}
int BattleAI::GetMoveAttr(int x, int y, short occupation_id) {
	HistoryActorPtr oppoActor = history->FindActorPosition(x, y);
	bool isTrue = (oppoActor != NULL && !oppoActor->good_invisible);
	if (isTrue) {
		if (oppoActor->relation == 2 && curActor->relation != 2)
			return MAP_NO_REACH;
		if (oppoActor->relation != 2 && curActor->relation == 2)
			return MAP_NO_REACH;
	}
	if (history->HasElem(x, y, occupation_id))
	{
		return MAP_NO_REACH;
	}
	if (curActor->HasEnemyAside(x,y))
		return MAP_CAN_REACH;
	return MAP_CAN_MOVE;
}

int GetShockMoveAttr(int x, int y, int relation, short occupation_id) {
	if (history->HasElem(x, y, occupation_id))
		return MAP_NO_REACH;
	HistoryActorPtr actor = history->FindActorPosition(x, y);
	if (actor) {
		switch (relation)
		{
		case 0:
		case 1:
		{
				  if (actor->relation == 2)
				  {
					  return MAP_NO_REACH;
				  }
		}
			break;
		case 2:
		{
				  if (actor->relation == 0 || actor->relation == 1)
				  {
					  return MAP_NO_REACH;
				  }
		}
			break;
		default:
			break;
		}
	}
	return MAP_CAN_MOVE;
}


void BattleAI::GenerateMoveList(HistoryActorPtr curActor) 
{
	BattleActor::clearMoveList();
	BattleActor::moveList = new DList<MovePath>(true);
	BattleActor::is_default_move = 0;
	this->curActor = curActor;
	DPoint direction_point[4];
	int index = 0;
	AStarNodePtr  firstNode,openNodes, lastNode;
	AStarNodePtr curNode = new AStarNode();
	int move_flag = 0;
	int extra_move_consume = curActor->getEnemyRangeEffect(959, true);
	int iActorSpeed = curActor->m_iRestSpeed ? curActor->m_iRestSpeed : curActor->GetActorSpeed();

	//机甲需要在召唤师控制范围内才能移动
	if (trData->occupations[curActor->occupation_id].is_robot == 1&&curActor->GetRangeBuffEffect(RANGE_CONTROL_TOWER, true) <= 0)
	{
		iActorSpeed = 0;
	}

	char *tiletag = (char*)ChessAlloc(history->map_width*history->map_height);
	memset(tiletag,0,history->map_width*history->map_height);
	{
		direction_point[0].x = 0;
		direction_point[0].y = -1;//上
		direction_point[1].x = 0;
		direction_point[1].y = 1; //下
		direction_point[2].x = 1;
		direction_point[2].y = 0; //右
		direction_point[3].x = -1;
		direction_point[3].y = 0; //左
	}
	curNode->x = curActor->x;
	curNode->y = curActor->y;
	curNode->step = 0;
	curNode->Parent = NULL;
	curNode->NextNode = NULL;
	tiletag[curNode->y * history->map_width + curNode->x] = 1;
	{
		//添加目的地和路径
		MovePathPtr movePoint = new MovePath(curNode);
		BattleActor::moveList->addElement(movePoint);
		if(curActor->state_palsy||curActor->ai_type == 2)
			return;
	}
	firstNode = lastNode = openNodes = curNode;
	while(openNodes)
	{
		for(index = 0;index<4;index++)
		{
			curNode = new AStarNode(); 
			curNode->x = openNodes->x+direction_point[index].x;
			curNode->y = openNodes->y+direction_point[index].y;

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
			if (curActor->limit_actor_move)
			{
				if (curActor->limit_actor_move == history->game_scene + 1)
				{
					if (!(curNode->x >= curActor->limit_actor_move_x1&&curNode->x <= curActor->limit_actor_move_x2&&curNode->y >= curActor->limit_actor_move_y1&&curNode->y <= curActor->limit_actor_move_y2))
					{
						continue;
					}
				}
				else
				{
					curActor->limit_actor_move = 0;
					curActor->limit_actor_move_x1 = 0;
					curActor->limit_actor_move_y1 = 0;
					curActor->limit_actor_move_x2 = 0;
					curActor->limit_actor_move_y2 = 0;
				}
			}
			move_flag = GetMoveAttr(curNode->x, curNode->y, curActor->occupation_id);
			int iSpeedUsed = curActor->getMoveSpeedUsed(curNode->x, curNode->y, move_flag);
			curNode->step = openNodes->step + iSpeedUsed;

			curNode->Parent = openNodes;
			curNode->NextNode = NULL;
			curNode->step += extra_move_consume;
			if (curNode->step > iActorSpeed)
			{
				move_flag = MAP_NO_REACH;
			}
			else if (curActor->good_shock_move || curActor->state_emergency_command)//添加赤兔突击移动效果
			{
				move_flag = GetShockMoveAttr(curNode->x, curNode->y, curActor->relation, curActor->occupation_id);
			}
			else if (curActor->good_passtrough_move)//添加穿越移动效果
            {
                move_flag = MAP_CAN_MOVE;
                if (history->HasElem(curNode->x, curNode->y, curActor->occupation_id))
                    move_flag = MAP_NO_REACH;
            } else
			{
				move_flag = GetMoveAttr(curNode->x, curNode->y, curActor->occupation_id);
			}
			if(move_flag == MAP_CAN_REACH||move_flag==MAP_CAN_MOVE)
			{
				//添加目的地和路径
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
				tiletag[curNode->y * history->map_width + curNode->x] = 1;
			}
			if(move_flag >= MAP_CAN_MOVE) //可前进,加入到Open队列
			{
				curNode->Parent = openNodes;
				lastNode = firstNode;
				while(lastNode)
				{
					if(lastNode->NextNode == NULL)
					{
						curNode->NextNode = lastNode->NextNode;
						lastNode->NextNode = curNode;
						break;
					}
					else if(lastNode->step<=curNode->step&&lastNode->NextNode->step>curNode->step)
					{
						curNode->NextNode = lastNode->NextNode;
						lastNode->NextNode = curNode;
						break;
					}
					lastNode = lastNode->NextNode;
				}
				//lastNode->NextNode = curNode;
				//lastNode = curNode;
			}
			if(move_flag <= MAP_CAN_REACH)
			{
				SAFE_DELETE(curNode);
			}
		}
		openNodes = openNodes->NextNode;
	}
	while(firstNode)
	{
		curNode = firstNode;
		firstNode = firstNode->NextNode;
		SAFE_DELETE(curNode);
	}
	ChessFree(tiletag);
}//这里有问题

//该函数单纯计算移动路劲，不要加其他特效等影响
void BattleAI::GenerateDispatchMoveList(HistoryActorPtr curActor)
{
	BattleActor::clearMoveList();
	BattleActor::moveList = new DList<MovePath>(true);
	BattleActor::is_default_move = 0;
	this->curActor = curActor;
	DPoint direction_point[4];
	int index = 0;
	AStarNodePtr  firstNode, openNodes, lastNode;
	AStarNodePtr curNode = new AStarNode();
	int move_flag = 0;
	int iActorSpeed = curActor->GetActorSpeed();

	char *tiletag = (char*)ChessAlloc(history->map_width*history->map_height);
	memset(tiletag, 0, history->map_width*history->map_height);
	{
		direction_point[0].x = 0;
		direction_point[0].y = -1;//上
		direction_point[1].x = 0;
		direction_point[1].y = 1; //下
		direction_point[2].x = 1;
		direction_point[2].y = 0; //右
		direction_point[3].x = -1;
		direction_point[3].y = 0; //左
	}
	curNode->x = curActor->x;
	curNode->y = curActor->y;
	curNode->step = 0;
	curNode->Parent = NULL;
	curNode->NextNode = NULL;
	tiletag[curNode->y * history->map_width + curNode->x] = 1;
	{
		//添加目的地和路径
		MovePathPtr movePoint = new MovePath(curNode);
		BattleActor::moveList->addElement(movePoint);
		if (curActor->ai_type == 2)
			return;
	}
	firstNode = lastNode = openNodes = curNode;
	while (openNodes)
	{
		for (index = 0; index < 4; index++)
		{
			curNode = new AStarNode();
			curNode->x = openNodes->x + direction_point[index].x;
			curNode->y = openNodes->y + direction_point[index].y;

			if (curNode->x < 0 || curNode->x >= history->map_width || curNode->y < 0 || curNode->y >= history->map_height)
			{
				SAFE_DELETE(curNode);
				continue;
			}
			if (tiletag[curNode->y * history->map_width + curNode->x])
			{
				SAFE_DELETE(curNode);
				continue;
			}

			move_flag = GetMoveAttr(curNode->x, curNode->y, curActor->occupation_id);
			int iSpeedUsed = curActor->getMoveSpeedUsed(curNode->x, curNode->y, move_flag);
			curNode->step = openNodes->step + iSpeedUsed;

			curNode->Parent = openNodes;
			curNode->NextNode = NULL;
			if (curNode->step > iActorSpeed)
			{
				move_flag = MAP_NO_REACH;
			}
			else
			{
				move_flag = GetMoveAttr(curNode->x, curNode->y, curActor->occupation_id);
			}
			if (move_flag == MAP_CAN_REACH || move_flag == MAP_CAN_MOVE)
			{
				//添加目的地和路径
				AStarNodePtr tmpNode = curNode;
				MovePathPtr movePoint = new MovePath(curNode);
				while (tmpNode->Parent)
				{
					DPointPtr tile = new DPoint();
					tile->x = tmpNode->x;
					tile->y = tmpNode->y;
					movePoint->path->insertElementAt(tile, 0);
					tmpNode = tmpNode->Parent;
				}
				BattleActor::moveList->addElement(movePoint);
				tiletag[curNode->y * history->map_width + curNode->x] = 1;
			}
			if (move_flag >= MAP_CAN_MOVE) //可前进,加入到Open队列
			{
				curNode->Parent = openNodes;
				lastNode = firstNode;
				while (lastNode)
				{
					if (lastNode->NextNode == NULL)
					{
						curNode->NextNode = lastNode->NextNode;
						lastNode->NextNode = curNode;
						break;
					}
					else if (lastNode->step <= curNode->step&&lastNode->NextNode->step > curNode->step)
					{
						curNode->NextNode = lastNode->NextNode;
						lastNode->NextNode = curNode;
						break;
					}
					lastNode = lastNode->NextNode;
				}
				//lastNode->NextNode = curNode;
				//lastNode = curNode;
			}
			if (move_flag <= MAP_CAN_REACH)
			{
				SAFE_DELETE(curNode);
			}
		}
		openNodes = openNodes->NextNode;
	}
	while (firstNode)
	{
		curNode = firstNode;
		firstNode = firstNode->NextNode;
		SAFE_DELETE(curNode);
	}
	ChessFree(tiletag);
}//这里有问题

//魔法技能获取
ActionGainPtr BattleAI::GetSkillGain(SkillAttrPtr skill, int cur_x, int cur_y,int x, int y) 
{
	ActionGainPtr totalGain = new ActionGain();
	int hit_x, hit_y;
	ActionSingleGainPtr curSingleGain;
	HistoryActorPtr oppoActor;
	int j;
	//totalGain->moveIndex = -1;
	totalGain->skill = skill;
	totalGain->point.x = cur_x;
	totalGain->point.y = cur_y;
	totalGain->hitPoint.x = x;
	totalGain->hitPoint.y = y;
	//if (data->effectArea[skill->effect_region_type].length == -2)//测试代码
	//	skill->effect_region_type = 3;//测试代码
	if (trData->effectArea[skill->effect_region_type].length>0) {
		for (j = 0; j < trData->effectArea[skill->effect_region_type].length; j++) {
			hit_x = x + trData->effectArea[skill->effect_region_type].pointlist[j].x;
			hit_y = y + trData->effectArea[skill->effect_region_type].pointlist[j].y;
            if( hit_x >= history->map_width || hit_x < 0 || hit_y >= history->map_height || hit_y < 0 )
                continue; /* 防止后续地图处理发生越界 */
			if (skill->skill_object == 0){
				oppoActor = history->FindOppoActorPosition(curActor,hit_x, hit_y);
			}
			else{
				oppoActor = history->FindOwnActorPosition(curActor,hit_x, hit_y);
			}

			if (skill->skill_object == 5&&oppoActor)//对队友释放
			{
				if (curActor->actor_id == oppoActor->actor_id)
					oppoActor = NULL;
			}

			if (skill->skill_object == 3){//对空白处使用
				
				if ((oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y)) || (oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y))){
					curSingleGain = new ActionSingleGain();
					
					curSingleGain->oppo_actor = oppoActor;
					
					curSingleGain->land_strict = 1;
				}
				else{
					if (skill->seid!=42){
						oppoActor = curActor;
					}
					else{
						oppoActor = history->FindActor(skill->power - 1);
					}

					curSingleGain = CalcSkillSingleGain(curActor, oppoActor,skill, totalGain);

					curSingleGain->actor_relive_px = hit_x;
					curSingleGain->actor_relive_py = hit_y;
				}
				if (j == 0)
				{
					totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
				}
                UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
			else if (skill->skill_object == 8) {//对空白处使用
				if (((oppoActor = history->FindActorPosition(hit_x, hit_y)) && trData->hitArea[skill->use_region].length!=-5) ||
					!GetTowerEnable(skill->skill_id, hit_x, hit_y, cur_x, cur_y)) {
					curSingleGain = new ActionSingleGain();

					curSingleGain->oppo_actor = oppoActor;

					curSingleGain->land_strict = 1;
				}
				else {
// 					if (trData->hitArea[skill->use_region].length == -5) {
// 						oppoActor = curActor;
// 					}
// 					else {
// 						oppoActor = history->FindActor(skill->power - 1);
// 					}
					oppoActor = curActor;

					curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);

					curSingleGain->actor_relive_px = hit_x;
					curSingleGain->actor_relive_py = hit_y;
				}
				if (j == 0)
				{
					totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
				}
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
			else if(skill->skill_object == 4) // 连续移动
			{
				curSingleGain = CalcSkillSingleGain(curActor, curActor, skill, totalGain);
				if(history->FindActorPosition(hit_x, hit_y) || !curActor->MapisMoveable(hit_x, hit_y))
				{
					curSingleGain->land_strict = 1;
				}
				curSingleGain->cur_point.x = hit_x;
				curSingleGain->cur_point.y = hit_y;

				totalGain->gains->addElement(curSingleGain);
			}
			else if (oppoActor != NULL)
			{
				curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, totalGain);
				if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
					totalGain->actor_kill++;

				if (j == 0) 
				{
					totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
				}
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
		}

	}
	else if(trData->effectArea[skill->effect_region_type].length == -2)
	{
		BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		for(int i = 0; i < scr->pointNum; i++)
		{
			hit_x = scr->pointList[i].x;
			hit_y = scr->pointList[i].y;
			if(skill->skill_object == 0)
			{
				oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y);
			}
			else
			{
				oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y);
			}

			if (skill->skill_object == 5 && oppoActor)//对队友释放
			{
				if (curActor->actor_id == oppoActor->actor_id)
					oppoActor = NULL;
			}

			if(oppoActor)
			{
				curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
				if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
					totalGain->actor_kill++;
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
		}
	}
	else {
		if(trData->effectArea[skill->effect_region_type].length == -4)
		{
			for (j = 0; j < 2; j++) {
				hit_x = x;
				hit_y = y;
				if(cur_x>x)
				{
					hit_x -=j;
				}
				else if(cur_x<x)
				{
					hit_x +=j;
				}
				if(cur_y>y)
				{
					hit_y -=j;
				}
				else if(cur_y<y)
				{
					hit_y +=j;
				}
				if (skill->skill_object == 0)
					oppoActor = history->FindOppoActorPosition(curActor,hit_x, hit_y);
				else
					oppoActor = history->FindOwnActorPosition(curActor,hit_x, hit_y);

				if (skill->skill_object == 5 && oppoActor)//对队友释放
				{
					if (curActor->actor_id == oppoActor->actor_id)
						oppoActor = NULL;
				}

				if (skill->skill_object == 3){//对空白处使用

					if ((oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y)) || (oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y))){
						curSingleGain = new ActionSingleGain();
						
						curSingleGain->oppo_actor = oppoActor;
						
						curSingleGain->land_strict = 1;
					}
					else{
						if (skill->seid != 42){
							oppoActor = curActor;
						}
						else{
							oppoActor = history->FindActor(skill->power - 1);
						}
						curSingleGain = CalcSkillSingleGain(curActor, oppoActor,skill, totalGain);
						curSingleGain->actor_relive_px = hit_x;
						curSingleGain->actor_relive_py = hit_y;
					}
					if (j == 0)
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
				}
				else if (oppoActor != NULL) 
				{
					curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, totalGain);
					if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
						totalGain->actor_kill++;
					if (j == 0) 
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
				}
			}
		}
		else if (trData->effectArea[skill->effect_region_type].length == -3 || trData->effectArea[skill->effect_region_type].length == -12)
		{
			int chuan_tou_num = 0;
			if (trData->effectArea[skill->effect_region_type].length == -3)
			{
				chuan_tou_num = 5;
			}
			else if (trData->effectArea[skill->effect_region_type].length == -12)
			{
				chuan_tou_num = 3;
			}
			for (j = 0; j < chuan_tou_num; j++) {
				hit_x = x;
				hit_y = y;
				if(cur_x>x)
				{
					hit_x -=j;
				}
				else if(cur_x<x)
				{
					hit_x +=j;
				}
				if(cur_y>y)
				{
					hit_y -=j;
				}
				else if(cur_y<y)
				{
					hit_y +=j;
				}
				if (skill->skill_object == 0)
					oppoActor = history->FindOppoActorPosition(curActor,hit_x, hit_y);
				else
					oppoActor = history->FindOwnActorPosition(curActor,hit_x, hit_y);

				if (skill->skill_object == 5 && oppoActor)//对队友释放
				{
					if (curActor->actor_id == oppoActor->actor_id)
						oppoActor = NULL;
				}

				if (skill->skill_object == 3){//对空白处使用

					if ((oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y)) || (oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y))){
						curSingleGain = new ActionSingleGain();
						
						curSingleGain->oppo_actor = oppoActor;
						
						curSingleGain->land_strict = 1;
					}
					else{
						if (skill->seid != 42){
							oppoActor = curActor;
						}
						else{
							oppoActor = history->FindActor(skill->power - 1);
						}
						curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
						curSingleGain->actor_relive_px = hit_x;
						curSingleGain->actor_relive_py = hit_y;
					}
					if (j == 0)
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);;
				}
				else if (oppoActor != NULL) 
				{
					curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, totalGain);
					if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
						totalGain->actor_kill++;
					if (j == 0) 
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
				}
			}
		}
		else if (trData->effectArea[skill->effect_region_type].length == -10 || trData->effectArea[skill->effect_region_type].length == -11)
		{
			int offset_x = cur_x - x;
			int offset_y = cur_y - y;
			int arrSize = 3;
			if (trData->effectArea[skill->effect_region_type].length == -11)
				arrSize = 4;
			std::map<int ,std::map<int, int> > hitPointM;
			hitPointM[0][0] = x;
			hitPointM[0][1] = y;
			if (!offset_x&& !offset_y)
				arrSize = 1;
			else if (!offset_x&& offset_y)
			{
				hitPointM[1][0] = x - 1;
				hitPointM[1][1] = y ;
				hitPointM[2][0] = x + 1;
				hitPointM[2][1] = y ;
				hitPointM[3][0] = x;
				hitPointM[3][1] = y - offset_y / abs(offset_y);
			}
			else 	if (offset_x&& !offset_y)
			{
				hitPointM[1][0] = x ;
				hitPointM[1][1] = y - 1;
				hitPointM[2][0] = x ;
				hitPointM[2][1] = y + 1;
				hitPointM[3][0] = x - offset_x / abs(offset_x);
				hitPointM[3][1] = y;
			}
			else 	if (offset_x&& offset_y)
			{
				hitPointM[1][0] = x - offset_x / abs(offset_x);
				hitPointM[1][1] = y + offset_y / abs(offset_y);
				hitPointM[2][0] = x + offset_x / abs(offset_x);
				hitPointM[2][1] = y - offset_y / abs(offset_y);
				hitPointM[3][0] = x - offset_x / abs(offset_x);
				hitPointM[3][1] = y - offset_y / abs(offset_y);
			}

			for (j = 0; j < arrSize; j++) {
				hit_x = hitPointM[j][0];
				hit_y = hitPointM[j][1];
				if (skill->skill_object == 0)
					oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y);
				else
					oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y);

				if (skill->skill_object == 5 && oppoActor)//对队友释放
				{
					if (curActor->actor_id == oppoActor->actor_id)
						oppoActor = NULL;
				}

				if (skill->skill_object == 3){//对空白处使用

					if ((oppoActor = history->FindOppoActorPosition(curActor, hit_x, hit_y)) || (oppoActor = history->FindOwnActorPosition(curActor, hit_x, hit_y))){
						curSingleGain = new ActionSingleGain();

						curSingleGain->oppo_actor = oppoActor;

						curSingleGain->land_strict = 1;
					}
					else{
						if (skill->seid != 42){
							oppoActor = curActor;
						}
						else{
							oppoActor = history->FindActor(skill->power - 1);
						}
						curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
						curSingleGain->actor_relive_px = hit_x;
						curSingleGain->actor_relive_py = hit_y;
					}
					if (j == 0)
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);;
				}
				else if (oppoActor != NULL)
				{
					curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
					if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
						totalGain->actor_kill++;
					if (j == 0)
					{
						totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					}
					UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
				}
			}
		}
		else if(trData->effectArea[skill->effect_region_type].length == -13)
		{
			oppoActor = nullptr;
			int iStep = curActor->getHitbackStep(cur_x, cur_y, x, y);
			if (iStep)
			{
				int iAdd = iStep / abs(iStep);

				if (cur_x == x)
				{
					for(int iY = cur_y; iY != cur_y + iStep; iY += iAdd)
					{
						HistoryActor* ptrActor = history->FindActorPosition(cur_x, iY + iAdd);
						if (ptrActor && !curActor->isSameCamp(ptrActor))
						{
							oppoActor = ptrActor;
							break;
						}
					}

					if (oppoActor)
					{
						curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
						curSingleGain->iHitbackId = oppoActor->actor_id;
						curSingleGain->oppoMovePos.x = cur_x;
						curSingleGain->oppoMovePos.y = cur_y + iStep + iAdd;
					}
					else
					{
						HistoryActor* ptrActor = history->FindActorPosition(x, y);
						if(ptrActor && !curActor->isSameCamp(ptrActor))
						{
							int iDistance = curActor->getDistance(ptrActor);
							if (iDistance - abs(iStep) == 1)
							{
								oppoActor = ptrActor;
								curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
							}
						}

						if (!oppoActor)
						{
							curSingleGain = CalcSkillSingleGain(curActor, curActor, skill, totalGain);
						}
					}
					curSingleGain->iIsHitback = 1;
					curSingleGain->curMovePos.x = cur_x;
					curSingleGain->curMovePos.y = cur_y + iStep;

					int iIndex = 0;
					for(int iY = cur_y; iY != cur_y + iStep; iY += iAdd)
					{
						DPoint point1;
						DPoint point2;

						point1.x = cur_x - 1;
						point1.y = iY + iAdd;
						curSingleGain->m_mEffectArea[iIndex++] = point1;

						point2.x = cur_x + 1;
						point2.y = iY + iAdd;
						curSingleGain->m_mEffectArea[iIndex++] = point2;
					}
				}
				else if (cur_y == y)
				{
					for(int iX = cur_x; iX != cur_x + iStep; iX += iAdd)
					{
						HistoryActor* ptrActor = history->FindActorPosition(iX + iAdd, cur_y);
						if(ptrActor && !curActor->isSameCamp(ptrActor))
						{
							oppoActor = ptrActor;
							break;
						}
					}

					if(oppoActor)
					{
						curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
						curSingleGain->iHitbackId = oppoActor->actor_id;
						curSingleGain->oppoMovePos.x = cur_x + iStep + iAdd;
						curSingleGain->oppoMovePos.y = cur_y;
					}
					else
					{
						HistoryActor* ptrActor = history->FindActorPosition(x, y);
						if(ptrActor && !curActor->isSameCamp(ptrActor))
						{
							int iDistance = curActor->getDistance(ptrActor);
							if(iDistance - abs(iStep) == 1)
							{
								oppoActor = ptrActor;
								curSingleGain = CalcSkillSingleGain(curActor, oppoActor, skill, totalGain);
							}
						}

						if(!oppoActor)
						{
							curSingleGain = CalcSkillSingleGain(curActor, curActor, skill, totalGain);
						}
					}
					curSingleGain->iIsHitback = 1;
					curSingleGain->curMovePos.x = cur_x + iStep;
					curSingleGain->curMovePos.y = cur_y;

					int iIndex = 0;
					for(int iX = cur_x; iX != cur_x + iStep; iX += iAdd)
					{
						DPoint point1;
						DPoint point2;

						point1.x = iX + iAdd;
						point1.y = cur_y - 1;
						curSingleGain->m_mEffectArea[iIndex++] = point1;

						point2.x = iX + iAdd;
						point2.y = cur_y + 1;
						curSingleGain->m_mEffectArea[iIndex++] = point2;
					}
				}
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
				
				int iHitNum = 0; //技能攻击人数
				map<int, ActionSingleGain*> mSingleGain;
				for (UInt i = 0; i < curSingleGain->m_mEffectArea.size(); i++)
				{
					DPoint point = curSingleGain->m_mEffectArea[i];
					HistoryActor* ptrActor = history->FindActorPosition(point.x, point.y);
					if (ptrActor && !curActor->isSameCamp(ptrActor))
					{
						mSingleGain[iHitNum++] = CalcSkillSingleGain(curActor, ptrActor, skill, totalGain);
					}
				}

				for (UInt i = 0; i < mSingleGain.size(); i++)
				{
					ActionSingleGain* ptrGain = mSingleGain[i];
					int iDistance = curActor->getDistance(ptrGain->oppo_actor);
					ptrGain->gain_hp = ptrGain->gain_hp * (40 + iDistance * 10) / 100;
					BattleControler::getBattleActor(ptrGain->oppo_actor->actor_id)->hp_gain = ptrGain->gain_hp;
					UPATE_TOTAL_HP_HURT(totalGain, ptrGain);
				}

				if(oppoActor)
				{
					DPoint point;
					int iSize = curSingleGain->m_mEffectArea.size();
					point.x = oppoActor->x;
					point.y = oppoActor->y;
					curSingleGain->m_mEffectArea[iSize] = point;
				}
			}
			else
			{
				curSingleGain = new ActionSingleGain();
				curSingleGain->oppo_actor = oppoActor;
				curSingleGain->land_strict = 1;
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
		}
	}

	return totalGain;
}

ActionGainPtr BattleAI::GetAttackGain(int cur_x, int cur_y, int x, int y, HistoryActorPtr cur_Actor, bool isVisible)
{
	ActionGainPtr totalGain = new ActionGain();
	int hit_x, hit_y;
	ActionSingleGainPtr curSingleGain = nullptr;
	HistoryActorPtr oppoActor = nullptr;
	if (cur_Actor == NULL)
	{
		cur_Actor = curActor;
	}
	int i, j;
	int attackArea_index = 0;
	totalGain->skill = 0;
	totalGain->hitPoint.x = x;
	totalGain->hitPoint.y = y;
	totalGain->point.x = cur_x;
	totalGain->point.y = cur_y;
	if(cur_Actor->good_hit_area != 0)
		attackArea_index = cur_Actor->good_hit_area;
	if(trData->effectArea[attackArea_index].length > 0)
	{
		int first_index = -1; //标记是否是第一个目标
		for(i = 0; i < trData->effectArea[attackArea_index].length; i++)
		{
			hit_x = x + trData->effectArea[attackArea_index].pointlist[i].x;
			hit_y = y + trData->effectArea[attackArea_index].pointlist[i].y;
			if(hit_x >= history->map_width || hit_x < 0 || hit_y >= history->map_height || hit_y < 0)
				continue; /* 防止后续地图处理发生越界 */
			oppoActor = history->FindOppoActorPosition(cur_Actor, hit_x, hit_y);
			if(oppoActor != NULL)
			{
				curSingleGain = CalcAttackSingleGain(cur_Actor, oppoActor, totalGain, isVisible);
				if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
					totalGain->actor_kill++;
				if(first_index < 0) //第一个目标100%伤害
				{
					totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
					first_index = i;
				}
				else //其余目标80%伤害
				{
					if((int)abs(curSingleGain->gain_hp) != 1)
					{
						curSingleGain->gain_hp = curSingleGain->gain_hp * 8 / 10;
						curSingleGain->gain_mp = curSingleGain->gain_mp * 8 / 10;
					}
				}
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}

			if (cur_Actor->isHadSpecial(TYPE721) && oppoActor &&  trData->effectArea[attackArea_index].length == 1)
			{
				HistoryActor* ptrActor = oppoActor->getActorByType(TYPE721);
				if(ptrActor)
				{
					ActionSingleGainPtr ptrGain = CalcAttackSingleGain(cur_Actor, ptrActor, totalGain, isVisible);
					ptrGain->gain_hp = ptrGain->gain_hp * cur_Actor->getSpecialInt(TYPE721) / 100;
					ptrGain->gain_mp = ptrGain->gain_mp * cur_Actor->getSpecialInt(TYPE721) / 100;
					BattleControler::getBattleActor(ptrActor->actor_id)->hp_gain = ptrGain->gain_hp;
					BattleControler::getBattleActor(ptrActor->actor_id)->mp_gain = ptrGain->gain_mp;
					UPATE_TOTAL_HP_HURT(totalGain, ptrGain);
				}
			}
		}
	}
	else
	{
		int chuan_tou_num = 0;
		if(trData->effectArea[attackArea_index].length == -4)
		{
			chuan_tou_num = 3;
		}
		else if(trData->effectArea[attackArea_index].length == -3)
		{
			chuan_tou_num = 6;
		}
		else if(trData->effectArea[attackArea_index].length == -6)
		{
			chuan_tou_num = 2;
		}
		else if(trData->effectArea[attackArea_index].length == -8)
		{
			chuan_tou_num = 4;
		}

		int iPierceNum = 0;
		if (cur_Actor->isHadSpecial(TYPE714))
		{
			for (j = 0; j < chuan_tou_num; j++)
			{
				hit_x = x;
				hit_y = y;
				if (cur_x > x)
				{
					hit_x -= j;
				}
				else if (cur_x < x)
				{
					hit_x += j;
				}
				if (cur_y > y)
				{
					hit_y -= j;
				}
				else if (cur_y < y)
				{
					hit_y += j;
				}
				oppoActor = history->FindOppoActorPosition(cur_Actor, hit_x, hit_y);
				if (oppoActor != NULL)
					iPierceNum++;
			}
		}

		for(j = 0; j < chuan_tou_num; j++)
		{
			hit_x = x;
			hit_y = y;
			if(cur_x>x)
			{
				hit_x -= j;
			}
			else if(cur_x<x)
			{
				hit_x += j;
			}
			if(cur_y>y)
			{
				hit_y -= j;
			}
			else if(cur_y < y)
			{
				hit_y += j;
			}
			oppoActor = history->FindOppoActorPosition(cur_Actor, hit_x, hit_y);
			if(oppoActor != NULL)
			{
				curSingleGain = CalcAttackSingleGain(cur_Actor, oppoActor, totalGain, isVisible,0,iPierceNum);
				if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
					totalGain->actor_kill++;
				if(j == 0)
				{
					totalGain->cur_hp_hurt = -curSingleGain->gain_hp;
				}
				UPATE_TOTAL_HP_HURT(totalGain, curSingleGain);
			}
		}
		// 方向性攻击处理
	}

	return totalGain;
}

// 移动规则：
// 计算移动范围,
// 尽量少移动，
// 对每个点判断是否有可以消灭的部队，
// 对每个点计算产生HP伤害的最大总值，
// 无反击
// 对于每一点.计算每个法术的Gain.计算物理攻击的gain.计算mostGain
// AI种类：被动出击、主动出击、坚守原地、攻击武将、到指定地点、跟随武将、逃至指定点。
ActionGainPtr BattleAI::GeneratePointMostGain(int index)
{
	int i,j,k,x,y;
    DArray<short> temp_skill;
    temp_skill.resize(MAX_SKILL_NUM);
    temp_skill.memset(0);
	ActionGainPtr retMostGain = new ActionGain();
	retMostGain->moveIndex = -1;
	if (BattleActor::moveList == NULL || BattleActor::moveList->size() <= index)
	{
		return retMostGain;
	}
	MovePathPtr movePoint = BattleActor::moveList->elementAt(index);
	//OccupationAttrPtr curOccu = &data->occupations[curActor->occupation_id];
	int attack_id = curActor->getAttackAreaId();
	HistoryActorPtr actor;
	HistoryActorPtr oppoActor;
	if (movePoint == NULL)
	{
		return retMostGain;
	}
	actor = history->FindActorPosition(movePoint->point.x,movePoint->point.y);

	if((!actor||(actor->actor_id == curActor->actor_id)))
	{
		//不是禁咒
		if(!curActor->state_stun)
		{
			for (i = 0; i<trData->skill_num; i++)
			{

				int temp_mp_consume = trData->skills[i].GetMpConsume(curActor);

				//释放法术
				if (!curActor->CurSkillLevelIsNull(i + 1) && curActor->GetCurMp() >= temp_mp_consume)
				{
					SkillAttrPtr skill = &trData->skills[i];
					if (curActor->relation == 2 && skill->boss_limit) continue;

					//天气限制
					if(!skill->BeingAbleToCastSpells(curActor)) continue;

					//复制技能
					if (curActor->m_mCopySkill.count(skill->skill_id + 1) > 0 && curActor->m_mCopySkill[skill->skill_id + 1]->iUsed) continue;

					if(trData->hitArea[skill->use_region].length>=0)
					{
						if (skill->skill_object != 0)
						{
							ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
							curGain->point.x = movePoint->point.x;
							curGain->point.y = movePoint->point.y;
							UpdateMostGain(retMostGain, curGain, index);
							SAFE_DELETE(curGain);
						}
						for(j = 0;j<trData->hitArea[skill->use_region].length;j++)
						{
							
							x = movePoint->point.x + trData->hitArea[skill->use_region].pointlist[j].x;
							y = movePoint->point.y + trData->hitArea[skill->use_region].pointlist[j].y;
							if(skill->skill_object == 0)//敌方对象
								oppoActor = history->FindOppoActorPosition(curActor,x,y);
							else//我方对象
								oppoActor = history->FindOwnActorPosition(curActor,x,y);
							if (oppoActor && (!oppoActor->good_invisible || skill->skill_object != 0))
							{
								ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
								curGain->point.x = movePoint->point.x;
								curGain->point.y = movePoint->point.y;
								curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
								UpdateMostGain(retMostGain, curGain, index);
								SAFE_DELETE(curGain);
							}
						}
						
					}
					else if(trData->hitArea[skill->use_region].length==-1)
					{

						if (skill->skill_object == 3)
						{
							ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
							curGain->point.x = movePoint->point.x;
							curGain->point.y = movePoint->point.y;
							UpdateMostGain(retMostGain, curGain, index);
							SAFE_DELETE(curGain);
						}
						else{
							//全屏法术处理
							int relation = 0;
							if ((skill->skill_object == 0 && curActor->relation == 2) ||
								(skill->skill_object != 0 && curActor->relation != 2))
							{
								relation = 1;
							}
							if (skill->type == 1 && curActor->relation != 2)
								relation = 0;
							else if (skill->type == 1 && curActor->relation == 2)
								relation = 1;
							//全屏法术分友军和敌军
							if (relation)
							{
								for (int j = 0; j < history->battleOwnNum; j++)
								{
									if (history->battleOwn[j]->hide != 1)
									{
										oppoActor = history->battleOwn[j];
										ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
										curGain->point.x = movePoint->point.x;
										curGain->point.y = movePoint->point.y;
										curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
										UpdateMostGain(retMostGain, curGain, index);
										SAFE_DELETE(curGain);
									}
								}
								for (int j = 0; j < history->battleFriendNum; j++)
								{
									if (history->battleFriend[j]->hide != 1)
									{
										oppoActor = history->battleFriend[j];
										ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
										curGain->point.x = movePoint->point.x;
										curGain->point.y = movePoint->point.y;
										curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
										UpdateMostGain(retMostGain, curGain, index);
										SAFE_DELETE(curGain);
									}
								}
							}
							else
							{
								for (int j = 0; j < history->battleEnemyNum; j++)
								{
									if (history->battleEnemy[j]->hide != 1)
									{
										oppoActor = history->battleEnemy[j];
										ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, oppoActor->x, oppoActor->y);
										curGain->point.x = movePoint->point.x;
										curGain->point.y = movePoint->point.y;
										curGain->need_back = oppoActor->isSkillBackEnable(curGain->point.x, curGain->point.y, skill);
										UpdateMostGain(retMostGain, curGain, index);
										SAFE_DELETE(curGain);
									}
								}
							}
						}
					}
					else
					{
						//无目标法术	
						ActionGainPtr curGain = new ActionGain();
						curGain->skill = skill;
						curGain->point.x = movePoint->point.x;
						curGain->point.y = movePoint->point.y;
						ActionSingleGainPtr curSingleGain = NULL;
						int relation = 0;
						if((skill->skill_object == 0&&curActor->relation == 2)||
							(skill->skill_object != 0&&curActor->relation != 2))
						{
							relation = 1;
						}

						if (skill->type == 1)
						{
							curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
						}
						else if(relation)
						{
							if(skill->skill_object !=0)
							{
								oppoActor = curActor;
								curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
                                UPATE_TOTAL_HP_HURT(curGain, curSingleGain);
							}
							else
							{
								for(k = 0;k<history->battleOwnNum;k++)
								{
									if(history->battleOwn[k]->hide !=1)
									{
										oppoActor = history->battleOwn[k];
										curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
										if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
											curGain->actor_kill++;
										UPATE_TOTAL_HP_HURT(curGain, curSingleGain);
									}
								}
								for(k = 0;k<history->battleFriendNum;k++)
								{
									if(history->battleFriend[k]->hide !=1)
									{
										oppoActor = history->battleFriend[k];
										curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
										if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
											curGain->actor_kill++;
										UPATE_TOTAL_HP_HURT(curGain, curSingleGain);
									}
								}
							}
						}
						else//敌军
						{
							if(skill->skill_object !=0)
							{
								oppoActor = curActor;
								curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
								UPATE_TOTAL_HP_HURT(curGain, curSingleGain);
							}
							else
							{
								for(k = 0;k<history->battleEnemyNum;k++)
								{
									if(history->battleEnemy[k]->hide !=1)
									{
										oppoActor = history->battleEnemy[k];
										curSingleGain = CalcSkillSingleGain(curActor,oppoActor, skill, curGain);
										if (-curSingleGain->gain_hp >= oppoActor->GetCurHp())
											curGain->actor_kill++;
										UPATE_TOTAL_HP_HURT(curGain, curSingleGain);
									}
								}
							}
						}
						UpdateMostGain(retMostGain,curGain,index);
						SAFE_DELETE(curGain);
					}
				}
				
			}
		}
		//攻击获取的数值
        if(!curActor->state_blind)
		if(trData->hitArea[attack_id].length>=0)
		{
			for(i = 0;i<trData->hitArea[attack_id].length;i++)
			{
				x = movePoint->point.x + trData->hitArea[attack_id].pointlist[i].x;
				y = movePoint->point.y + trData->hitArea[attack_id].pointlist[i].y;
				oppoActor = history->FindOppoActorPosition(curActor,x,y);
				if(oppoActor && !oppoActor->good_invisible)
				{
					ActionGainPtr curGain = GetAttackGain(movePoint->point.x, movePoint->point.y, x, y);
					if (!oppoActor->state_puzzle && !oppoActor->state_blind && !oppoActor->state_obfuscation && (curActor->good_hit_no_back != 100))
					{
						curGain->need_back = oppoActor->isAttackArea(curGain->point.x, curGain->point.y, true);
					}
					UpdateMostGain(retMostGain,curGain,index);
					SAFE_DELETE(curGain);
				}
			}
		}
		else if(trData->hitArea[attack_id].length == -1)
		{
			BattleScreenPtr scr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
			for(int i = 0; i < scr->pointNum; i++)
			{
				x = scr->pointList[i].x;
				y = scr->pointList[i].y;
				oppoActor = history->FindOppoActorPosition(curActor, x, y);
				if(oppoActor)
				{
					ActionGainPtr curGain = GetAttackGain(movePoint->point.x, movePoint->point.y, x, y);
					if(!oppoActor->state_puzzle && !oppoActor->state_blind && !oppoActor->state_obfuscation && (curActor->good_hit_no_back != 100))
					{
						curGain->need_back = oppoActor->isAttackArea(curGain->point.x, curGain->point.y, true);
					}
					UpdateMostGain(retMostGain, curGain, index);
					SAFE_DELETE(curGain);
				}
			}
		}
		if(retMostGain->moveIndex == -1)
		{
			retMostGain->point.x = movePoint->point.x;
			retMostGain->point.y = movePoint->point.y;
			int cur_land = history->map_array[movePoint->point.y][movePoint->point.x];
			switch(cur_land)
			{
			case 18: //城池	
				retMostGain->next_round_gain_hp = 20;
				break;
			case 19: //关隘
				retMostGain->next_round_gain_hp = 20;
				break;
			case 20: //鹿砦
				retMostGain->next_round_gain_hp = 25;
				break;
			case 21: //村庄
				retMostGain->next_round_gain_hp = 16;
				break;
			case 22: //兵营
				retMostGain->next_round_gain_hp = 20;
				break;
			}
		}
	}
	return retMostGain;
}




// 移动规则：
// 计算移动范围,
// 尽量少移动，
// 计算给自己加血有效治疗最大化
// AI种类：被动出击、主动出击、坚守原地、攻击武将、到指定地点、跟随武将、逃至指定点。
ActionGainPtr BattleAI::GeneratePointMostGainOnlyAddHp(int index)
{
	int i, j, x, y;
	ActionGainPtr retMostGain = new ActionGain();
	retMostGain->moveIndex = -1;
	MovePathPtr movePoint = BattleActor::moveList->elementAt(index);
	//OccupationAttrPtr curOccu = &data->occupations[curActor->occupation_id];
	int attack_id = curActor->getAttackAreaId();
	HistoryActorPtr actor;
	HistoryActorPtr oppoActor = NULL;
	actor = history->FindActorPosition(movePoint->point.x, movePoint->point.y);
	if ((!actor || (actor->actor_id == curActor->actor_id)))
	{
		//不是禁咒
		if (!curActor->state_stun)
		{
			for (i = 0; i < trData->skill_num; i++)
			{

				int temp_mp_consume = trData->skills[i].GetMpConsume(curActor);
				//释放法术
				if (!curActor->CurSkillLevelIsNull(i + 1) && curActor->GetCurMp() >= temp_mp_consume)
				{
					SkillAttrPtr skill = &trData->skills[i];

					//天气限制
					if (skill){
						if (!skill->BeingAbleToCastSpells(curActor))
							continue;
					}

					if (trData->hitArea[skill->use_region].length >= 0)
					{
						if (skill->skill_object != 0) //初始化retMostGain
						{
							ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, movePoint->point.x, movePoint->point.y);
							curGain->point.x = movePoint->point.x;
							curGain->point.y = movePoint->point.y;
							UpdateMostGain(retMostGain, curGain, index);							
							SAFE_DELETE(curGain);
						}
						for (j = 0; j < trData->hitArea[skill->use_region].length; j++)
						{

							x = movePoint->point.x + trData->hitArea[skill->use_region].pointlist[j].x;
							y = movePoint->point.y + trData->hitArea[skill->use_region].pointlist[j].y;
							//if (data->skills[i].skill_object == 0)//敌方对象
							//oppoActor = history->FindOppoActorPosition(curActor, x, y);
							//else
							if (trData->skills[i].skill_object != 0)//我方对象
								oppoActor = history->FindOwnActorPosition(curActor, x, y);

							if (oppoActor != NULL && oppoActor->actor_id == curActor->actor_id) //给自己释放增益魔法
							{
								ActionGainPtr curGain = GetSkillGain(skill, movePoint->point.x, movePoint->point.y, x, y);
								curGain->point.x = movePoint->point.x;
								curGain->point.y = movePoint->point.y;
								curGain->hitPoint.x = movePoint->point.x;
								curGain->hitPoint.y = movePoint->point.y;							
								UpdateMostGain(retMostGain, curGain, index);								
								SAFE_DELETE(curGain);
							}
						}

					}
					
				}
			}
		}
	}
	return retMostGain;
}



int BattleAI::GenerateMostNearPos(int x,int y)
{
	int i,ret = 0;
	int nearest_dis = 0xffff,cur_distance;
	DList<MovePath>* moveList = BattleActor::moveList;
	BattleActor::moveList = NULL;
	for(i = 0;i<moveList->size();i++)
	{
		MovePathPtr movePoint = (MovePathPtr)moveList->elementAt(i);
		if(history->FindActorPosition(movePoint->point.x,movePoint->point.y)!=NULL&&
			history->FindActorPosition(movePoint->point.x,movePoint->point.y)!=curActor)
			continue;
		cur_distance = curActor->GenerateDefaultMove(movePoint->point.x,movePoint->point.y,x,y);
		if(cur_distance<nearest_dis)
		{
			nearest_dis = cur_distance;
			ret = i;
		}
	}
	BattleActor::clearMoveList();
	BattleActor::moveList = moveList;
	return ret;
}
// AI种类：被动出击、主动出击、坚守原地、攻击武将、到指定地点、跟随武将、逃至指定点。

// AI种类：被动出击、主动出击、坚守原地、攻击武将、到指定地点、跟随武将、逃至指定点。
void BattleAI::GenerateMostNearPos()
{
	int i,j;
	int nearest_dis = 0xffff,one_point_most = 0xffff,cur_distance;
	DList<MovePath>* moveList = BattleActor::moveList;
	BattleActor::moveList = NULL;
	HistoryActorPtr oppo_actor; 
	HistoryActorPtr nearest_actor = NULL;
	if(curActor->relation == 2)
	{
		for (int k = 0; k < history->battleOwnNum; k++)
		{
			if (history->battleOwn[k]->hide != 1 && !history->battleOwn[k]->good_invisible)
			{
				nearest_actor = history->battleOwn[k];
				one_point_most = (int)abs(curActor->x - nearest_actor->x) + (int)abs(curActor->y - nearest_actor->y);
			}
		}
		for(j = 0;j<history->battleOwnNum;j++)
		{
			if(history->battleOwn[j]->hide!=1 && !history->battleOwn[j]->good_invisible)
			{
				oppo_actor = history->battleOwn[j];
				cur_distance = (int)abs(curActor->x - oppo_actor->x)+(int)abs(curActor->y - oppo_actor->y);
				//cur_distance = curActor->GenerateDefaultMove(movePoint->point.x,movePoint->point.y,
				//	history->battleOwn[j]->x,history->battleOwn[j]->y);
				if (cur_distance<one_point_most&&curActor->MapisMoveable(oppo_actor->x, oppo_actor->y))
				{
					one_point_most = cur_distance;
					nearest_actor = oppo_actor;
				}
			}
		}
		for(j = 0;j<history->battleFriendNum;j++)
		{
			if(history->battleFriend[j]->hide!=1 && !history->battleFriend[j]->good_invisible)
			{
				oppo_actor = history->battleFriend[j];
				cur_distance = (int)abs(curActor->x - oppo_actor->x)+(int)abs(curActor->y - oppo_actor->y);
				//cur_distance = curActor->GenerateDefaultMove(movePoint->point.x,movePoint->point.y,
				//	history->battleFriend[j]->x,history->battleFriend[j]->y);
				if (cur_distance<one_point_most&&curActor->MapisMoveable(oppo_actor->x, oppo_actor->y))
				{
					one_point_most = cur_distance;
					nearest_actor = oppo_actor;
				}
			}
		}
	}
	else
	{
		for (int k = 0; k < history->battleEnemyNum; k++)
		{
			if (history->battleEnemy[k]->hide != 1 && !history->battleEnemy[k]->good_invisible)
			{
				nearest_actor = history->battleEnemy[k];
				one_point_most = (int)abs(curActor->x - nearest_actor->x) + (int)abs(curActor->y - nearest_actor->y);
			}
		}
		for(j = 0;j<history->battleEnemyNum;j++)
		{
			if(history->battleEnemy[j]->hide!=1 && !history->battleEnemy[j]->good_invisible)
			{
				oppo_actor = history->battleEnemy[j];
				cur_distance = (int)abs(curActor->x - oppo_actor->x)+(int)abs(curActor->y - oppo_actor->y);
				//if(abs(movePoint->point.x - history->battleEnemy[j]->x)+abs(movePoint->point.y - history->battleEnemy[j]->y)
				//	>one_point_most)
				//	continue;
				//cur_distance = curActor->GenerateDefaultMove(movePoint->point.x,movePoint->point.y,
				//	history->battleEnemy[j]->x,history->battleEnemy[j]->y);
				if (cur_distance<one_point_most&&curActor->MapisMoveable(oppo_actor->x, oppo_actor->y))
				{
					one_point_most = cur_distance;
					nearest_actor = oppo_actor;
				}
			}
		}
	}

	if(nearest_actor == NULL) //找不到目的地
	{
		BattleActor::clearMoveList();
		return;
	}
	one_point_most = 0xffff;
	for(i = 0;i<moveList->size();i++)
	{
		MovePathPtr movePoint = (MovePathPtr)moveList->elementAt(i);
		if(history->FindActorPosition(movePoint->point.x,movePoint->point.y)!=NULL&&
			history->FindActorPosition(movePoint->point.x,movePoint->point.y)!=curActor)
			continue;
		if((int)abs(movePoint->point.x - nearest_actor->x)+(int)abs(movePoint->point.y - nearest_actor->y)>one_point_most)
			continue;
		one_point_most = curActor->GenerateDefaultMove(movePoint->point.x,movePoint->point.y,
			nearest_actor->x,nearest_actor->y);
		if(one_point_most<nearest_dis)
		{
			mostGain->moveIndex = i;
			mostGain->point.x = movePoint->point.x;
			mostGain->point.y = movePoint->point.y;
			mostGain->gains->removeAllElements();
			nearest_dis = one_point_most;
		}
	}
	BattleActor::clearMoveList();
	BattleActor::moveList = moveList;	
}
void BattleAI::GenerateMostGain()
{
	if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
		mostGain = BattleReplay::getInstance()->getActionGain(false);
	else
		GenerateMostGain_old();

	if (mostGain->skill)
	{
		int iSkillId = mostGain->skill->skill_id + 1;
		if(curActor->m_mCopySkill.count(iSkillId) > 0 && !curActor->m_mCopySkill[iSkillId]->iUsed)
		{
			curActor->setIsCopySkill(true);
		}
	}
}
void BattleAI::GenerateMostGain_old() 
{
	int i;
	ClearGain();
	mostGain = new ActionGain();
	mostGain->moveIndex = -1;
	switch (curActor->ai_type) {
	case 0: // 被动出击
		{
			for (i = 0; i < BattleActor::moveList->size(); i++) 
			{
				ActionGainPtr curGain = GeneratePointMostGain(i);
				if (curGain->moveIndex != -1)
				{
					UpdateMostGain(mostGain, curGain, i);
				}
				SAFE_DELETE(curGain);
			}
		}
		break;
	case 1:// 主动出击
		{
			for (i = 0; i < BattleActor::moveList->size(); i++) 
			{
				//主动出击敌方AI
				ActionGainPtr curGain = GeneratePointMostGain(i);
				//更新
				UpdateMostGain(mostGain,curGain,i);//生成最终的移动和攻击路径
				SAFE_DELETE(curGain);
			}
			if (mostGain->moveIndex == -1) {
				GenerateMostNearPos();
			}
		}
		break;
	case 2: // 坚守原地
		{
			ActionGainPtr curGain = GeneratePointMostGain(0);
			if (curGain->moveIndex != -1)
			{
				UpdateMostGain(mostGain, curGain, 0);
			}
			SAFE_DELETE(curGain);
		}
		break;
	case 3:// 攻击武将
		{
			HistoryActorPtr oppo_actor = history->FindActor(curActor->oppo_actor_id);
			if(oppo_actor == NULL||oppo_actor->hide==1)
			{
				curActor->oppo_actor_id = 0;
				curActor->ai_type = 1;
				for (i = 0; i < BattleActor::moveList->size(); i++) 
				{
					ActionGainPtr curGain = GeneratePointMostGain(i);
					UpdateMostGain(mostGain,curGain,i);
					SAFE_DELETE(curGain);
				}
				if (mostGain->moveIndex == -1) {
					GenerateMostNearPos();
				}
			}
			else
			{
				for (i = 0; i < BattleActor::moveList->size(); i++)
				{
					ActionGainPtr curGain = GeneratePointMostGain(i);
					if(curGain->moveIndex!=-1)
					{
						ActionSingleGainPtr single_gain = (ActionSingleGainPtr)curGain->gains->elementAt(0);

						if(single_gain!=NULL&&single_gain->oppo_actor!=NULL&&
							single_gain->oppo_actor->actor_id == curActor->oppo_actor_id)
						{
							SAFE_DELETE(mostGain);
							mostGain = curGain;
							break;
						}
						else
						{	
							UpdateMostGain(mostGain,curGain,i);
							SAFE_DELETE(curGain);
						}
					}
					else
					{
						SAFE_DELETE(curGain);
					}
				}
				if (mostGain->moveIndex == -1)
				{
					int index = GenerateMostNearPos(oppo_actor->x,oppo_actor->y);
					DList<MovePath>* moveList = BattleActor::moveList;
					MovePathPtr movePoint = moveList->elementAt(index);
					mostGain->moveIndex = index;
					mostGain->point.x = movePoint->point.x;
					mostGain->point.y = movePoint->point.y;
					mostGain->gains->removeAllElements();
				}
			}
		}
		break;
	case 4:// 到指定地点。路上会攻击。
		{
			for (i = 0; i < BattleActor::moveList->size(); i++) 
			{
				ActionGainPtr curGain = GeneratePointMostGain(i);
				UpdateMostGain(mostGain,curGain,i);
				SAFE_DELETE(curGain);
			}
			if (mostGain->moveIndex == -1) 
			{
				int index = GenerateMostNearPos(curActor->dest_x,curActor->dest_y);
				DList<MovePath>* moveList = BattleActor::moveList;
				MovePathPtr movePoint = moveList->elementAt(index);
				mostGain->moveIndex = index;
				mostGain->point.x = movePoint->point.x;
				mostGain->point.y = movePoint->point.y;
				mostGain->gains->removeAllElements();
			}
		}
		break;
	case 5:// 跟随武将
		{
			HistoryActorPtr oppo_actor = history->FindActor(curActor->oppo_actor_id);
			if(oppo_actor != NULL)
			{
				int index = GenerateMostNearPos(oppo_actor->x,oppo_actor->y);
				ActionGainPtr curGain = GeneratePointMostGain(index);
				UpdateMostGain(mostGain, curGain, index);
				if (mostGain->moveIndex == -1)
				{
					DList<MovePath>* moveList = BattleActor::moveList;
					MovePathPtr movePoint = moveList->elementAt(index);
					mostGain->moveIndex = index;
					mostGain->point.x = movePoint->point.x;
					mostGain->point.y = movePoint->point.y;
					mostGain->gains->removeAllElements();
				}
				SAFE_DELETE(curGain);
			}
			else
			{
				curActor->ai_type = 2;
				ActionGainPtr curGain = GeneratePointMostGain(0);
				if (curGain->moveIndex != -1)
				{
					UpdateMostGain(mostGain, curGain, 0);
				}
				SAFE_DELETE(curGain);
			}
		}
		break;
	case 6:// 逃到指定点。路上不会攻击
		{
			int index = GenerateMostNearPos(curActor->dest_x, curActor->dest_y);
			DList<MovePath>* moveList = BattleActor::moveList;
			MovePathPtr movePoint = moveList->elementAt(index);
			mostGain->moveIndex = index;
			mostGain->point.x = movePoint->point.x;
			mostGain->point.y = movePoint->point.y;
			mostGain->gains->removeAllElements();

			if (curActor->x == curActor->dest_x&&
				curActor->y == curActor->dest_y)
			{
				curActor->ai_type = 2;
				ActionGainPtr curGain = GeneratePointMostGain(0);
				if (curGain->moveIndex != -1)
				{
					UpdateMostGain(mostGain, curGain, 0);
				}
				SAFE_DELETE(curGain);
			}
 			else
 			{
 				//给自己回血
 
				if (curActor->GetCurHp() < curActor->GetBaseHp() + curActor->GetGoodsAddHp())
				{ 
					ActionGainPtr curGain = GeneratePointMostGainOnlyAddHp(index);
					if (curGain->moveIndex != -1)
					{
						UpdateMostGain(mostGain, curGain, index);
					}
					SAFE_DELETE(curGain);
 				}
 			}
		}
		break;

	}
}
bool BattleAI::IsDeadlyAttack()
{
	if(mostGain->skill!=NULL||mostGain->good!=NULL || history->m_bGuide[0]) //新手引导屏蔽爆击
		return false;
	ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)mostGain->gains->elementAt(0);
	if (curSingleGain==NULL){
		return false;
	}
	HistoryActorPtr oppo_actor = curSingleGain->oppo_actor;
	float cur_energe = curActor->getRealAbility(ACTOR_ABILITY_FORTUNE, true);
	float oppo_energe = oppo_actor->getRealAbility(ACTOR_ABILITY_FORTUNE, true);

	float rate = cur_energe / oppo_energe;
	float probably = 0;
	if(rate>=3)
	{
		probably = 100;
	}
	else if(rate>=2)
	{
		probably = 80 * rate - 140;
	}
	else if(rate>=1)
	{
		probably = 18 * rate - 16;
	}
	else
	{
		probably = 1;
	}
	probably += curActor->good_deadliness_hit + curActor->good_effects[TWICEHITORDEADLY_RATE] + curActor->getSpecialInt(TYPE712) + curActor->getSpecialMap(TYPE763)[2];
	if(rand()%100<probably)
		return true;
	else
		return false;
}
void BattleAI::BounceAbnormalAttack(ActionGainPtr mostGain, HistoryActorPtr actor, ActionSingleGainPtr curSingleGain)
{
	bool enable = false;
	ActionSingleGainPtr temp_gain = new ActionSingleGain;
	if (curSingleGain->oppo_actor->good_effects[39] == 2 || curSingleGain->oppo_actor->good_effects[39] == 3)
	{
		if (curSingleGain->state_power < 0)
		{
			temp_gain->state_power = -1;
			curSingleGain->state_power = 0;
			enable = true;
		}
		if (curSingleGain->state_intellegence < 0)
		{
			temp_gain->state_intellegence = -1;
			curSingleGain->state_intellegence = 0;
			enable = true;
		}
		if (curSingleGain->state_defence < 0)
		{
			temp_gain->state_defence = -1;
			curSingleGain->state_defence = 0;
			enable = true;
		}
		if (curSingleGain->state_explosion < 0)
		{
			temp_gain->state_explosion = -1;
			curSingleGain->state_explosion = 0;
			enable = true;
		}
		if (curSingleGain->state_fortune < 0)
		{
			temp_gain->state_fortune = -1;
			curSingleGain->state_fortune = 0;
			enable = true;
		}
		if (curSingleGain->state_speed < 0)
		{
			temp_gain->state_speed = -1;
			curSingleGain->state_speed = 0;
			enable = true;
		}
	}
	if (curSingleGain->oppo_actor->good_effects[39] == 1 || curSingleGain->oppo_actor->good_effects[39] == 3)
	{
		if (curSingleGain->state_puzzle < 0)
		{
			temp_gain->state_puzzle = -1;
			curSingleGain->state_puzzle = 0;
			enable = true;
		}
		if (curSingleGain->state_poison < 0)
		{
			temp_gain->state_poison = -1;
			curSingleGain->state_poison = 0;
			enable = true;
		}
		if (curSingleGain->state_palsy < 0)
		{
			temp_gain->state_palsy = -1;
			curSingleGain->state_palsy = 0;
			enable = true;
		}
		if (curSingleGain->state_stun < 0)
		{
			temp_gain->state_stun = -1;
			curSingleGain->state_stun = 0;
			enable = true;
		}
		if (curSingleGain->state_blind < 0)
		{
			temp_gain->state_blind = -1;
			curSingleGain->state_blind = 0;
			enable = true;
		}
		if (curSingleGain->state_obfuscation < 0)
		{
			temp_gain->state_obfuscation = -1;
			curSingleGain->state_obfuscation = 0;
			enable = true;
		}
		if (curSingleGain->state_weaked < 0)
		{
			temp_gain->state_weaked = -1;
			curSingleGain->state_weaked = 0;
			enable = true;
		}
		if(curSingleGain->state_skillhurt < 0)
		{
			temp_gain->state_skillhurt = -1;
			curSingleGain->state_skillhurt = 0;
			enable = true;
		}
		if(curSingleGain->state_loseblood < 0)
		{
			temp_gain->state_loseblood = -1;
			curSingleGain->state_loseblood = 0;
			enable = true;
		}
		if (curSingleGain->state_addhp < 0)
		{
			temp_gain->state_addhp = -1;
			curSingleGain->state_addhp = 0;
			enable = true;
		}
	}

	if (curSingleGain->gain_hp + curSingleGain->oppo_actor->GetCurHp() <= 0)
		enable = false;
	if (enable)
	{
		// 		ActionSingleGainPtr temp_gain = new ActionSingleGain;
		temp_gain->oppo_actor = actor;
		temp_gain->probability = 100;
		mostGain->gains->addElement(temp_gain);
	}
	else
	{
		SAFE_DELETE(temp_gain);
	}
}

bool BattleAI::GetTowerEnable(int iSkillId, int iX, int iY, int iCurX, int iCurY)
{
	SkillAttrPtr skill = &trData->skills[iSkillId];
	HistoryActorPtr ptrActor = &history->actors[skill->power - 1];

	if (history->HasElem(iX, iY, ptrActor->occupation_id) || trData->occupation_landmove_info[ptrActor->occupation_id][history->map_array[iY][iX]] == 255)
	{
		return false;
	}

	if (skill->power == 904&& skill->seid == 126)
	{
		HistoryActorPtr oppoActor;
		int x = 0, y = 0;
		map<int, int> tMap;

		for (int i = 0; i < trData->hitArea[skill->iControlAtkid - 1].length; i++)
		{
			x = iCurX + trData->hitArea[skill->iControlAtkid - 1].pointlist[i].x;
			y = iCurY + trData->hitArea[skill->iControlAtkid - 1].pointlist[i].y;

			oppoActor = history->FindOwnActorPosition(curActor, x, y);

			if (oppoActor && !oppoActor->hide && (oppoActor->actor_id == 901 || oppoActor->actor_id == 902))
			{
				tMap[oppoActor->actor_id] = 1;
			}
		}

		if (tMap[901] && tMap[902] && abs(history->actors[901].x - history->actors[902].x) < 2 && abs(history->actors[901].y - history->actors[902].y) < 2)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if (skill->power == 904 && skill->seid == 127)
	{
		HistoryActorPtr oppoActor;
		int x = 0, y = 0;
		map<int, int> tMap;
		for (int i = 0; i < trData->hitArea[skill->iControlAtkid - 1].length; i++)
		{
			x = iCurX + trData->hitArea[skill->iControlAtkid - 1].pointlist[i].x;
			y = iCurY + trData->hitArea[skill->iControlAtkid - 1].pointlist[i].y;

			oppoActor = history->FindOwnActorPosition(curActor, x, y);

			if (oppoActor && !oppoActor->hide && oppoActor->actor_id == 903)
			{
				tMap[oppoActor->actor_id] = 1;
			}
		}

		if (tMap[903])
		{
			HistoryActorPtr ptrTActor = &history->actors[903];
			HistoryActorPtr ptrT1Actor =history->FindActor(901);
			if (ptrT1Actor)
			{
				unsigned short iOffX = ptrTActor->x, iOffY = ptrTActor->y;
				history->FindNearestBlackPosInit(ptrT1Actor, iOffX, iOffY);

				if ((iOffX != ptrTActor->x || iOffY != ptrTActor->y) && abs(iOffX - ptrTActor->x) < 2 && abs(iOffY - ptrTActor->y) < 2)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActionSingleGain::ActionSingleGain()
{
	oppo_actor = nullptr;
	share_actor = nullptr;
	gain_hp = 0;
	gain_mp = 0;
	state_mustHit = 0;
	state_power = 0;
	state_intellegence = 0;
	state_defence = 0;
	state_explosion = 0;
	state_fortune = 0;
	state_puzzle = 0;
	state_blind = 0;
	state_obfuscation = 0;
	state_poison = 0;
	state_palsy = 0;
	state_stun = 0;
	state_speed = 0;
	state_weaked = 0;
	state_skillhurt = 0;
	state_adddef_redpow = 0;
	state_loseblood = 0;
	state_addhp = 0;
	state_sual_link = 0;
	state_reduceSkill = 0;
	state_reduceHurt = 0;
	state_SkillHurtAndRebound = 0;
	state_emergency_command = 0;
	probability = 0;
	action_again = 0;
	land_strict = 0;
	enemy_skipper_strict = 0;
	cCurAir = -1;
	actor_relive = 0;
	fast_move = 0;
	actor_tower_def = 0;
	cur_point.x = -1;
	cur_point.y = -1;
	actor_relive_id = 0;
	actor_relive_px = -1;
	actor_relive_py = -1;
	controlled_enemy_actorId = 0;
	border_on_death = 0;
	actor_leveraging_power = 0;
	actor_knockback = -1;
	reduce_skill_hurt = -1;
	add_skill_hurt = -1;
	state_shield_range = 0;
	through_the_target = -1;
	new_assault = 0;
	actor_can_drag = 0;
	skill_dispatch_actor = 0;
	iIsHitback = 0;
	iHitbackId = -1;
	curMovePos.x = 0;
	curMovePos.y = 0;
	oppoMovePos.x = 0;
	oppoMovePos.y = 0;
}

ActionSingleGain::~ActionSingleGain()
{
	oppo_actor = nullptr;
	share_actor = nullptr;
	m_mEffectArea.clear();
	mSeidAddGain.clear();
}

void ActionSingleGain::readSingleGain(DMemoryReader* reader)
{
	int iOppoId = -1, iShareId = -1;
	reader->tryReadInt(iOppoId);
	reader->tryReadInt(iShareId);
	if (iOppoId != -1)
	{
		oppo_actor = &history->actors[iOppoId];
	}
	if (iShareId != -1)
	{
		share_actor = &history->actors[iShareId];
	}
	reader->tryReadInt(gain_hp); //获得的HP,对敌人造成伤害时该值为负数，
	reader->tryReadInt(gain_mp);//获得的MP,对敌人造成伤害时该值为负数，
	reader->tryReadChar((char&)state_mustHit); //百分百命中
	reader->tryReadChar((char&)state_power);
	reader->tryReadChar((char&)state_intellegence);
	reader->tryReadChar((char&)state_defence);
	reader->tryReadChar((char&)state_explosion);//敏捷，爆发力
	reader->tryReadChar((char&)state_fortune);//士气
	reader->tryReadChar((char&)state_puzzle);//混乱
	reader->tryReadChar((char&)state_blind);//失明 被攻击必中 不可攻击
	reader->tryReadChar((char&)state_obfuscation);//昏迷，无法行动，被攻击必中，并且解除状态
	reader->tryReadChar((char&)state_poison);//中毒
	reader->tryReadChar((char&)state_palsy);//麻痹
	reader->tryReadChar((char&)state_stun);//禁咒
	reader->tryReadChar((char&)state_speed);
	reader->tryReadChar((char&)state_weaked); //被虚弱，受伤害增加
	reader->tryReadChar((char&)state_skillhurt); //法伤加成
	reader->tryReadChar((char&)state_adddef_redpow);
	reader->tryReadChar((char&)state_loseblood); //失血
	reader->tryReadChar((char&)state_addhp); //回血
	reader->tryReadChar((char&)state_sual_link);
	reader->tryReadChar((char&)state_reduceSkill); //洞察
	reader->tryReadChar((char&)state_reduceHurt); //惨痛压制
	reader->tryReadChar((char&)state_emergency_command);//冲锋号令
	reader->tryReadChar((char&)probability);//命中率
	reader->tryReadChar((char&)action_again);
	reader->tryReadChar((char&)land_strict);
	reader->tryReadChar((char&)enemy_skipper_strict);//主将限制
	reader->tryReadChar((char&)cCurAir);
	reader->tryReadChar((char&)actor_relive);
	reader->tryReadChar((char&)fast_move);
	reader->tryReadChar((char&)actor_tower_def);
	reader->tryReadChar((char&)state_SkillHurtAndRebound);//增加法术和伤害反弹
	reader->tryReadDPoint(cur_point);
	reader->tryReadInt(actor_relive_id);
	reader->tryReadInt(actor_relive_px);
	reader->tryReadInt(actor_relive_py);
	reader->tryReadInt(controlled_enemy_actorId);//控制的敌军id
	reader->tryReadInt(border_on_death);
	reader->tryReadInt(actor_leveraging_power);//标记是否是借力打力对象
	reader->tryReadInt(actor_knockback);//武将被击退
	reader->tryReadInt(reduce_skill_hurt);//减伤光环
	reader->tryReadInt(add_skill_hurt);//增伤光环
	reader->tryReadInt(state_shield_range);//庇护光环
	reader->tryReadInt(through_the_target);//穿越目标
	reader->tryReadInt(new_assault);//使自己和被攻击者前进一格
	reader->tryReadInt(actor_can_drag);
	reader->tryReadInt(skill_dispatch_actor);
	reader->tryReadInt(iIsHitback);
	reader->tryReadInt(iHitbackId);
	reader->tryReadDPoint(curMovePos);
	reader->tryReadDPoint(oppoMovePos);
	int iPointSize = 0;
	reader->tryReadInt(iPointSize);
	for (int i = 0; i < iPointSize; i++)
	{
		reader->tryReadDPoint(m_mEffectArea[i]);
	}
	reader->tryReadMapInt(mSeidAddGain);
}

void ActionSingleGain::saveSingleGain(DMemoryWriter* writer)
{
	writer->writeInt(oppo_actor ? oppo_actor->actor_id : -1);
	writer->writeInt(share_actor ? share_actor->actor_id : -1);
	writer->writeInt(gain_hp); //获得的HP,对敌人造成伤害时该值为负数，
	writer->writeInt(gain_mp);//获得的MP,对敌人造成伤害时该值为负数，
	writer->writeChar(state_mustHit); //百分百命中
	writer->writeChar(state_power);
	writer->writeChar(state_intellegence);
	writer->writeChar(state_defence);
	writer->writeChar(state_explosion);//敏捷，爆发力
	writer->writeChar(state_fortune);//士气
	writer->writeChar(state_puzzle);//混乱
	writer->writeChar(state_blind);//失明 被攻击必中 不可攻击
	writer->writeChar(state_obfuscation);//昏迷，无法行动，被攻击必中，并且解除状态
	writer->writeChar(state_poison);//中毒
	writer->writeChar(state_palsy);//麻痹
	writer->writeChar(state_stun);//禁咒
	writer->writeChar(state_speed);
	writer->writeChar(state_weaked); //被虚弱，受伤害增加
	writer->writeChar(state_skillhurt); //法伤加成
	writer->writeChar(state_adddef_redpow);
	writer->writeChar(state_loseblood); //失血
	writer->writeChar(state_addhp); //回血
	writer->writeChar(state_sual_link);
	writer->writeChar(state_reduceSkill); //洞察
	writer->writeChar(state_reduceHurt); //惨痛压制
	writer->writeChar(state_emergency_command); //冲锋号令
	writer->writeChar(probability);//命中率
	writer->writeChar(action_again);
	writer->writeChar(land_strict);
	writer->writeChar(enemy_skipper_strict);//主将限制
	writer->writeChar(cCurAir);
	writer->writeChar(actor_relive);
	writer->writeChar(fast_move);
	writer->writeChar(actor_tower_def);
	writer->writeChar(state_SkillHurtAndRebound);//增加法术和伤害反弹
	writer->writeDPoint(cur_point);
	writer->writeInt(actor_relive_id);
	writer->writeInt(actor_relive_px);
	writer->writeInt(actor_relive_py);
	writer->writeInt(controlled_enemy_actorId);//控制的敌军id
	writer->writeInt(border_on_death);
	writer->writeInt(actor_leveraging_power);//标记是否是借力打力对象
	writer->writeInt(actor_knockback);//武将被击退
	writer->writeInt(reduce_skill_hurt);//减伤光环
	writer->writeInt(add_skill_hurt);//增伤光环
	writer->writeInt(state_shield_range);//庇护光环
	writer->writeInt(through_the_target);//穿越目标
	writer->writeInt(new_assault);//使自己和被攻击者前进一格
	writer->writeInt(actor_can_drag);
	writer->writeInt(skill_dispatch_actor);
	writer->writeInt(iIsHitback);
	writer->writeInt(iHitbackId);
	writer->writeDPoint(curMovePos);
	writer->writeDPoint(oppoMovePos);
	int iPointSize = m_mEffectArea.size();
	writer->writeInt(iPointSize);
	for (int i = 0; i < iPointSize; i++)
	{
		writer->writeDPoint(m_mEffectArea[i]);
	}
	writer->writeMapInt(mSeidAddGain);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActionGain::ActionGain()
{
	moveIndex = 0;
	actor_kill = 0;
	cur_hp_hurt = 0;
	total_hp_hurt = 0;
	gain_mp = 0;
	gain_hp = 0;
	need_hp = 0;
	need_mp = 0;
	need_back = 0;
	deadly_attack = 0;
	next_round_gain_hp = 0;
	iRealKillNum = 0;
	iDontShowSkillName = 0;
	skill = nullptr;
	good = nullptr;
	gains = new DList<ActionSingleGain>(true);
}

ActionGain::~ActionGain()
{
	skill = nullptr;
	good = nullptr;
	SAFE_DELETE(gains);
}

void ActionGain::readGain(DMemoryReader* reader)
{
	reader->tryReadInt(moveIndex);
	reader->tryReadDPoint(point);
	reader->tryReadDPoint(hitPoint);
	reader->tryReadInt(actor_kill);//单次收益的杀敌数
	reader->tryReadInt(cur_hp_hurt);//对当前攻击目标的伤害量
	reader->tryReadInt(total_hp_hurt);//单次攻击多个目标时的总伤害量
	reader->tryReadInt(gain_mp);//获取mp
	reader->tryReadInt(gain_hp);//获取hp
	reader->tryReadInt(need_hp);//消耗hp
	reader->tryReadInt(need_mp);//消耗mp
	reader->tryReadInt(need_back);//反击
	reader->tryReadInt(deadly_attack);//致命一击
	reader->tryReadInt(next_round_gain_hp);//下回合回复的血量
	reader->tryReadInt(iRealKillNum); //实际击杀的人数
	reader->tryReadInt(iDontShowSkillName);//不显示技能名
	int iSkillId = -1;
	int iGoodsId = -1;
	reader->tryReadInt(iSkillId);
	reader->tryReadInt(iGoodsId);
	if (iSkillId != -1)
	{
		skill = &trData->skills[iSkillId];
	}
	if (iGoodsId != -1)
	{
		good = trData->m_mGoodsItem[iGoodsId];
	}
	int iGainSize = 0;
	reader->tryReadInt(iGainSize);
	for (int i = 0; i < iGainSize; i++)
	{
		ActionSingleGain* ptrGain = new ActionSingleGain();
		ptrGain->readSingleGain(reader);
		gains->addElement(ptrGain);
	}
}

void ActionGain::saveGain(DMemoryWriter* writer)
{
	writer->writeInt(moveIndex);
	writer->writeDPoint(point);
	writer->writeDPoint(hitPoint);
	writer->writeInt(actor_kill);//单次收益的杀敌数
	writer->writeInt(cur_hp_hurt);//对当前攻击目标的伤害量
	writer->writeInt(total_hp_hurt);//单次攻击多个目标时的总伤害量
	writer->writeInt(gain_mp);//获取mp
	writer->writeInt(gain_hp);//获取hp
	writer->writeInt(need_hp);//消耗hp
	writer->writeInt(need_mp);//消耗mp
	writer->writeInt(need_back);//反击
	writer->writeInt(deadly_attack);//致命一击
	writer->writeInt(next_round_gain_hp);//下回合回复的血量
	writer->writeInt(iRealKillNum); //实际击杀的人数
	writer->writeInt(iDontShowSkillName);//不显示技能名
	writer->writeInt(skill ? skill->skill_id : -1);
	writer->writeInt(good ? good->iGoodsId : -1);
	writer->writeInt(gains->size());
	for (int i = 0; i < gains->size(); i++)
	{
		ActionSingleGain* ptrGain = gains->elementAt(i);
		ptrGain->saveSingleGain(writer);
	}
}