#include "Common.h"
#include "Scripts.h"
#include "BattleAI.h"
#include "BattleActor.h"
#include "ScriptHandler.h"
#include "RedeScreen.h"
#include "StatConfig.h"
#include "BattleScreen.h"
#include "WinConditionTool.h"
#include "ScriptRunBuffAction.h"
#include "GuideLayer.h"

ScriptHandler::ScriptHandler()
{
	script_lists = new DList<DList<Script> >(true);
	script_index_lists = new DList<DVector<int> >(true);
	pre_battle_list = new DList<Script>(true);
	cur_list = NULL;
	cur_index_list = NULL;
	script_skipto_begin = false;
	is_script_skipto = false;
	m_iBGIndex = 0;
}

ScriptHandler::~ScriptHandler()
{
	SAFE_DELETE(script_lists);
	SAFE_DELETE(script_index_lists);
	SAFE_DELETE(pre_battle_list);
}


//TODO 每回合的状态增益效果
void ScriptHandler::ChangeActorState(HistoryActorPtr curActor)
{
	//每场战斗添加出场动画
	if (history->GetCurRound() == 1 && curActor->isHadSpecial(TYPE756))
	{
		cur_list->addElement(new ScriptRunAction(NULL, curActor, __String::createWithFormat("reward_%d", curActor->getSpecialInt(TYPE756))->getCString(), ACTOR_STATE_RUN_ACTION, 20));
		cur_list->addElement(new ScriptDelaySet(curActor->getSpecialMap(TYPE756)[1]));
	}

	//////////////////////////////////////////异常状态
	int land_recover_state = 0; //该地形是否有异常状态回复功能
	//if ((curActor->x == 52685) && (curActor->y == 52685)) {
	//	curActor->x = 0;
	//	curActor->y = 0;
	//	if (history->m_iCurWarId == 98) {//引导第三关卡第一次进入有个不改上的英雄
	//		curActor->hide = 1;
	//	}
	//	
	//	CCLOG("WTF");
	//}
	for (int i = 0; i < trData->lands[history->map_array[curActor->y][curActor->x]].seid_num; i++)
	{
		if (trData->m_mSeid[trData->lands[history->map_array[curActor->y][curActor->x]].seid[i]]->type == 242)
		{
			land_recover_state = 1;
			break;
		}
	}

	if(curActor->good_effects[HP_CHANGE_MP])
	{
		curActor->hp_mp_defense_number = 0;
	}

	if (curActor->border_on_death == 1 || curActor->border_on_death == 2) curActor->border_on_death++;
	else if (curActor->border_on_death == 3)
	{
		curActor->border_on_death = 0;
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, curActor->GetCurHp(), 0, 0, 0, 0, 0));
	}
	if (curActor->mSkillBeUseNum.size())
	{
		curActor->mSkillBeUseNum.clear();
	}
	curActor->good_pursuit_attack_num = 0;
	curActor->good_skill_pursuit_attack_num = 0;
	curActor->blood_shield = 0;

	for(map<int, ActorBuff*>::iterator it = curActor->m_mActorBuff.begin(); it != curActor->m_mActorBuff.end(); it++)
	{
		ActorBuff* pActBuff = it->second;
		BuffElem* ptrBuff = trData->m_mDataBuff[pActBuff->iBuffId];

		bool bIsRecover = false; //TYPE718, 免疫特定状态
		if (curActor->isHadSpecial(TYPE718))
		{
			map<int, int> mSpecial = curActor->getSpecialMap(TYPE718);
			SeidType* ptrType = trData->m_mSeidType[TYPE718];
			for(UInt i = 0; i < mSpecial.size(); i += ptrType->iEffNum)
			{
				if(mSpecial[i] == ptrType->iClass || mSpecial[i] == pActBuff->iBuffId)
				{
					bIsRecover = true;
					break;
				}
			}
		}

		if(pActBuff->iCurRound > 0)
		{
			if((curActor->good_recover_state || land_recover_state || bIsRecover) && ptrBuff->iEffectType == 0)
			{
				pActBuff->iCurRound = 0;
			}
			else
			{
				pActBuff->iCurRound--;
			}

			if (pActBuff->iCurRound == 0)
			{
				pActBuff->iArmIndex = 2;
			}
			else if(ptrBuff->iIsPlay == 1)
			{
				pActBuff->iArmIndex = 1;
			}
			curActor->updateActorState(pActBuff->iBuffId);
		}
	}

	//每回合自己自动加的状态
	curActor->updateActorBuff(ACTOR_BUFF_POWUP, curActor->good_add_power_round, true);
	curActor->updateActorBuff(ACTOR_BUFF_INTUP, curActor->good_add_intellegence_round, true);
	if(curActor->good_add_defence_round || curActor->good_effects[ROUND_DEF_UP] > rand() % 100)
	{
		curActor->updateActorBuff(ACTOR_BUFF_DEFUP, 1, true);
	}
	curActor->updateActorBuff(ACTOR_BUFF_EXPUP, curActor->good_add_explosion_round, true);
	curActor->updateActorBuff(ACTOR_BUFF_FORTUP, curActor->good_add_fortune_round, true);
	if(curActor->good_add_speed_round || curActor->good_effects[ROUND_SPEED_UP] > rand() % 100)
	{
		curActor->updateActorBuff(ACTOR_BUFF_SPEEDUP, 1, true);
	}

	for(map<int, ActorBuff*>::iterator it = curActor->m_mActorBuff.begin(); it != curActor->m_mActorBuff.end(); it++)
	{
		ActorBuff* pActBuff = it->second;
		if(pActBuff->iArmIndex)
		{
			cur_list->addElement(new ScriptRunBuffAction(curActor, pActBuff->iBuffId));
		}
	}

	//每回合范围添加、消除状态
    HistoryActor::StateRangeEffects stateRangeEffects;
    //CCLOG("hid:%d relation:%d hide:%d x:%d y:%d", curActor->actor_id, curActor->relation, curActor->hide, curActor->x, curActor->y);
    bool upEffect = curActor->relation == ACTOR_RELATION_OWN || curActor->relation == ACTOR_RELATION_FRIEND;
    for (int i = 0; i < history->battleEnemyNum; i++) {
        history->battleEnemy[i]->accumulateTotalStateRangeEffects(&stateRangeEffects, upEffect, curActor->x, curActor->y);
    }
    for (int i = 0; i < history->battleOwnNum; i++) {
        history->battleOwn[i]->accumulateTotalStateRangeEffects(&stateRangeEffects, !upEffect, curActor->x, curActor->y);
    }
    for (int i = 0; i < history->battleFriendNum; i++) {
        history->battleFriend[i]->accumulateTotalStateRangeEffects(&stateRangeEffects, !upEffect, curActor->x, curActor->y);
    }
    if(curActor->state_blind && stateRangeEffects.blind < 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_BLIND | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_blind && stateRangeEffects.blind > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_BLIND));
    }
    if(curActor->state_puzzle && stateRangeEffects.puzzle < 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_PUZZLE | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_puzzle && stateRangeEffects.puzzle > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_PUZZLE));
    }
    if(curActor->state_poison && stateRangeEffects.poison < 0&&!curActor->border_on_death) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_POISON | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_poison && stateRangeEffects.poison > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_POISON));
    }
    if(curActor->state_palsy && stateRangeEffects.palsy < 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_PALSY | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_palsy && stateRangeEffects.palsy > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_PALSY));
    }
    if(curActor->state_stun && stateRangeEffects.stun < 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_STUN | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_stun && stateRangeEffects.stun > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_STUN));
    }
    if(curActor->state_obfuscation && stateRangeEffects.obfuscation < 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_OBFUSCATION | UNNORMAL_STATE_CLEAR));
    } else if(!curActor->state_obfuscation && stateRangeEffects.obfuscation > 0) {
        cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_OBFUSCATION));
    }


	//每回合范围 +hp+mp
    int range_up_hp=0,range_up_mp=0;
    if (curActor->relation == ACTOR_RELATION_OWN || curActor->relation == ACTOR_RELATION_FRIEND) {//当前人物阵营 我 友 方
		for (int i = 0; i < history->battleOwnNum; i++) {
            range_up_hp += history->battleOwn[i]->getTotalRangeEffect(957, true, curActor->x, curActor->y);
            range_up_mp += history->battleOwn[i]->getTotalRangeEffect(958, true, curActor->x, curActor->y);
		}
		for (int i = 0; i < history->battleFriendNum; i++) {
            range_up_hp += history->battleFriend[i]->getTotalRangeEffect(957, true, curActor->x, curActor->y);
            range_up_mp += history->battleFriend[i]->getTotalRangeEffect(958, true, curActor->x, curActor->y);
		}
	}
	else { //敌方
		for (int i = 0; i < history->battleEnemyNum; i++) {
            range_up_hp += history->battleEnemy[i]->getTotalRangeEffect(957, true, curActor->x, curActor->y);
            range_up_mp += history->battleEnemy[i]->getTotalRangeEffect(958, true, curActor->x, curActor->y);
		}
	}
	int range_gain_hp = (curActor->GetBaseHp() + curActor->GetGoodsAddHp()) * range_up_hp / 100;
	if (curActor->GetCurHp() + range_gain_hp > curActor->GetBaseHp() + curActor->GetGoodsAddHp())
		range_gain_hp = curActor->GetBaseHp() + curActor->GetGoodsAddHp() - curActor->GetCurHp();
	int range_gain_mp = (curActor->GetBaseMp() + curActor->GetGoodsAddMp()) * range_up_mp / 100;
	if (curActor->GetCurMp() + range_gain_mp > curActor->GetBaseMp() + curActor->GetGoodsAddMp())
		range_gain_mp = curActor->GetBaseMp() + curActor->GetGoodsAddMp() - curActor->GetCurMp();
	if (curActor->border_on_death&&range_gain_hp)
	{
		curActor->border_on_death = 0;
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_POISON | UNNORMAL_STATE_CLEAR));
	}
    cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, -range_gain_hp, -range_gain_mp, 0, 0, 0, 0));



	///////////////////////////////////////////////////////////
	//
	int cur_land = history->map_array[curActor->y][curActor->x];
	int hurt_hp = 0;

	/////////////////////////////////////////////////
	//中毒减HP
	if (curActor->state_poison&&curActor->border_on_death == 0)
	{
		hurt_hp = (curActor->GetBaseHp() + curActor->GetGoodsAddHp())*(curActor->m_mActorBuff[ACTOR_BUFF_POISON]->iPercent) / 100;
		if (curActor->state_reduceHurt&&hurt_hp)
		{
			hurt_hp -= hurt_hp*curActor->m_mActorBuff[ACTOR_BUFF_REDUCEHURT]->iPercent / 100;
		}
	}
	if (curActor->GetCurHp() - hurt_hp <= 0)
		hurt_hp = curActor->GetCurHp() - 1;
	if(hurt_hp)
	{
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, hurt_hp, 0, 0, 0, 0, 0));
		cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_NORMAL, 0, 0, -hurt_hp, 0, false, -1));
	}

	//失血减HP
	hurt_hp = 0;
	if(curActor->state_loseblood && curActor->border_on_death == 0)
	{
		hurt_hp += curActor->getTotalAbility(TOTAL_HP)*(curActor->m_mActorBuff[ACTOR_BUFF_BLOOD]->iPercent) / 100;
		if (curActor->state_reduceHurt&&hurt_hp)
		{
			hurt_hp -= hurt_hp*curActor->m_mActorBuff[ACTOR_BUFF_REDUCEHURT]->iPercent / 100;
		}
		if(curActor->GetCurHp() - hurt_hp <= 0)
		{
			hurt_hp = curActor->GetCurHp() - 1;
		}
	}
	//回复加HP
	if (curActor->state_addhp)
	{
		hurt_hp -= curActor->getTotalAbility(TOTAL_HP)*(curActor->m_mActorBuff[ACTOR_BUFF_ADDHP]->iPercent) / 100;
		if (curActor->GetCurHp() - hurt_hp > curActor->getTotalAbility(TOTAL_HP))
		{
			hurt_hp = curActor->GetCurHp() - curActor->getTotalAbility(TOTAL_HP);
		}
	}
	//失血
	if (curActor->isHadSpecial(TYPE763))
	{
		hurt_hp += curActor->getTotalAbility(TOTAL_HP)*curActor->getSpecialInt(TYPE763) / 100;

		if (curActor->GetCurHp() - hurt_hp <= 0)
		{
			hurt_hp = curActor->GetCurHp() - 1;
		}
	}

	if(hurt_hp)
	{
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, hurt_hp, 0, 0, 0, 0, 0));
		cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_NORMAL, 0, 0, -hurt_hp, 0, false, -1));
	}

	///////////////////////////////////////////////////
	//其他状态
	int gain_hp = 0;
	int gain_mp = 0;

	for(int i = 0; i < trData->lands[cur_land].seid_num; i++)
	{
		Seid* seid = trData->m_mSeid[trData->lands[cur_land].seid[i]];
		switch(seid->type)
		{
		case 240: //每回合恢复HP
			gain_hp += (curActor->GetBaseHp() + curActor->GetGoodsAddHp()) * seid->special_effe[0] / 100;
			break;
		case 241: //每回合恢复MP
			gain_mp += (curActor->GetBaseMp() + curActor->GetGoodsAddMp()) * seid->special_effe[0] / 100;
			break;
		default:
			break;
		}
	}
	//这里还需要处理每回合获得EXP的情况,每回合恢复hp，每回合恢复mp
	if (curActor->good_recover_hp_round)
		gain_hp += (curActor->GetBaseHp() + curActor->GetGoodsAddHp()) * curActor->good_recover_hp_round / 100;
	if (curActor->good_recover_mp_round)
		gain_mp += (curActor->GetBaseMp() + curActor->GetGoodsAddMp()) * curActor->good_recover_mp_round / 100;

	gain_mp += curActor->getSpecialInt(TYPE361); //每回合恢复固定的MP

	if (gain_hp + curActor->GetCurHp() > (curActor->GetBaseHp() + curActor->GetGoodsAddHp()))
		gain_hp = (curActor->GetBaseHp() + curActor->GetGoodsAddHp()) - curActor->GetCurHp();
	if (gain_mp + curActor->GetCurMp() > (curActor->GetBaseMp() + curActor->GetGoodsAddMp()))
		gain_mp = (curActor->GetBaseMp() + curActor->GetGoodsAddMp()) - curActor->GetCurMp();

	if (curActor->border_on_death&&gain_hp)
	{
		curActor->border_on_death = 0;
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, UNNORMAL_STATE_POISON | UNNORMAL_STATE_CLEAR));
	}
	if(gain_hp > 0 || gain_mp > 0)
	{
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, -gain_hp, -gain_mp, 0, 0, 0, 0));
		cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_NORMAL, 0, 0, gain_hp, gain_mp, false, -1));
	}
}

void ScriptHandler::GenerateScriptListActorState(int relation, HistoryPtr history)
{
	int i;
	HistoryActorPtr curActor = NULL;
	script_index = 0;
	is_script_skipto = false;
	cur_list = new DList<Script>(true);
	int containment_action_num = 0;
	if (relation == ACTOR_RELATION_OWN)
	{
		for (i = 0; i < history->battleOwnNum; i++)
		{
			curActor = history->battleOwn[i];
			if (curActor->hide != 1)
			{
				ChangeActorState(curActor);
				int effect_value = curActor->getEnemyRangeEffect(960, false);
				if (effect_value && containment_action_num < effect_value && curActor->actor_id>=0 && curActor->actor_id < 1000 && curActor->action_finished != 1)
				{
					curActor->action_finished = 1;
					containment_action_num++;
				}
			}
		}
		for (i = 0; i < history->battleFriendNum; i++)
		{
			curActor = history->battleFriend[i];
			if (curActor->hide != 1)
			{
				ChangeActorState(curActor);
				int effect_value = curActor->getEnemyRangeEffect(960, false);
				if (effect_value && containment_action_num < effect_value && curActor->actor_id>=0 && curActor->actor_id < 1000 && curActor->action_finished != 1)
				{
					curActor->action_finished = 1;
					containment_action_num++;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < history->battleEnemyNum; i++)
		{
			curActor = history->battleEnemy[i];
			if (curActor->hide != 1)
			{
				ChangeActorState(curActor);
				int effect_value = curActor->getEnemyRangeEffect(960, false);
				if (effect_value && containment_action_num < effect_value && curActor->actor_id>=0 && curActor->actor_id < 1000 && curActor->action_finished != 1)
				{
					curActor->action_finished = 1;
					containment_action_num++;
				}
			}
		}
	}
	/////自方回合清楚对方buff
	if (relation == ACTOR_RELATION_OWN)
	{
		for (i = 0; i < history->battleEnemyNum; i++)
		{
			curActor = history->battleEnemy[i];
			if (curActor->hide != 1)
			{
				if (curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurMp() <= 0 && curActor->hp_mp_defense_number < curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurHp() >(curActor->GetBaseMp() + curActor->GetGoodsAddMp()))
				{
					curActor->hp_mp_defense_number++;
					cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, curActor->GetBaseMp() + curActor->GetGoodsAddMp(), -1, 0, 0, 0, 0));
				}
				if (curActor->buff_hate_attack)
					curActor->buff_hate_attack = 0;
				curActor->actor_only_move = 0;
				curActor->vital_spirit = 0;
			}
		}
	}
	else
	{
		for (i = 0; i < history->battleOwnNum; i++)
		{
			curActor = history->battleOwn[i];
			if (curActor->hide != 1)
			{
				if (curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurMp() <= 0 && curActor->hp_mp_defense_number < curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurHp() >(curActor->GetBaseMp() + curActor->GetGoodsAddMp()))
				{
					curActor->hp_mp_defense_number++;
					cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, curActor->GetBaseMp() + curActor->GetGoodsAddMp(), -1, 0, 0, 0, 0));
				}
				if (curActor->controlled_by_enemy)
					curActor->controlled_by_enemy_round++;
				if (curActor->controlled_by_enemy&&curActor->controlled_by_enemy_round >= curActor->controlled_by_enemy)//将被控制的敌军从我军列表去除
				{
					curActor->delete_controlled_by_enemy();
				}
				if (curActor->buff_hate_attack)
					curActor->buff_hate_attack = 0;
				curActor->actor_only_move = 0;
				curActor->vital_spirit = 0;
			}
		}
		for (i = 0; i < history->battleFriendNum; i++)
		{
			curActor = history->battleFriend[i];
			if (curActor->hide != 1)
			{
				if (curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurMp() <= 0 && curActor->hp_mp_defense_number < curActor->good_effects[HP_CHANGE_MP] && curActor->GetCurHp() >(curActor->GetBaseMp() + curActor->GetGoodsAddMp()))
				{
					curActor->hp_mp_defense_number++;
					cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, curActor->GetBaseMp() + curActor->GetGoodsAddMp(), -1, 0, 0, 0, 0));
				}
				if (curActor->buff_hate_attack)
					curActor->buff_hate_attack = 0;
				curActor->actor_only_move = 0;
				curActor->vital_spirit = 0;
			}
		}
	}
	/////
	cur_script = (ScriptPtr)cur_list->elementAt(0);
	//异常状态的恢复与运气有关
}

void ScriptHandler::GenerateScriptState(ActionGainPtr gain, HistoryActorPtr curActor)
{
	int i;
	ActionSingleGainPtr single_gain = NULL;
	HistoryActorPtr oppo_actor;

	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);

		oppo_actor = single_gain->oppo_actor;
		cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_NORMAL, 0, 0));
	}

	cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_NORMAL, 0, 0));
	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);
		oppo_actor = single_gain->oppo_actor;

		if (-single_gain->gain_hp >= oppo_actor->GetCurHp())
		{
			gain->iRealKillNum++;
		}

		int iSkillMiss = 0;
		if (gain->skill && single_gain->probability == 0) //技能MISS显示未命中标识
		{
			iSkillMiss = 1;
		}

		if(single_gain->gain_hp || single_gain->gain_mp || iSkillMiss)
		{
			int iActorId = oppo_actor->actor_id;
			if (single_gain->share_actor)
			{
				iActorId = single_gain->share_actor->actor_id;
			}
			cur_list->addElement(new ScriptChangeActorState(iActorId, -single_gain->gain_hp, -single_gain->gain_mp, iSkillMiss, 0, 0, 0));
		}
	}
	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);
		oppo_actor = single_gain->oppo_actor;

		oppo_actor->updateActorBuff(ACTOR_BUFF_PUZZLE, single_gain->state_puzzle, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_OBFU, single_gain->state_obfuscation, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_STUN, single_gain->state_stun, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_WEAK, single_gain->state_weaked, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_BLIND, single_gain->state_blind, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_PALSY, single_gain->state_palsy, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_BLOOD, single_gain->state_loseblood, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_POISON, single_gain->state_poison, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_ADDHP, single_gain->state_addhp, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_POWUP, single_gain->state_power, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_INTUP, single_gain->state_intellegence, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_DEFUP, single_gain->state_defence, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_EXPUP, single_gain->state_explosion, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_FORTUP, single_gain->state_fortune, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_SPEEDUP, single_gain->state_speed, true, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_FIRESHIELD, single_gain->state_SkillHurtAndRebound, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_ADDDEFREDPOW, single_gain->state_adddef_redpow, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_SUALLINK, single_gain->state_sual_link, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_REDUCESKILL, single_gain->state_reduceSkill, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_REDUCEHURT, single_gain->state_reduceHurt, false, gain, curActor);
		oppo_actor->updateActorBuff(ACTOR_BUFF_EMERGENCYCOMMAND, single_gain->state_emergency_command, false, gain, curActor);

		if(single_gain->add_skill_hurt > 0 && !oppo_actor->state_add_skill_hurt_range)
		{
			oppo_actor->updateActorBuff(ACTOR_BUFF_HURTUP, single_gain->add_skill_hurt, false, gain, curActor);
			oppo_actor->add_skill_hurt_buff->addElement(new HistoryActor::RangeEffect(2, oppo_actor->m_mActorBuff[ACTOR_BUFF_HURTUP]->iPercent, 100));
		}

		if(single_gain->reduce_skill_hurt > 0 && !oppo_actor->state_reduce_skill_hurt_range)
		{
			oppo_actor->updateActorBuff(ACTOR_BUFF_HURTDW, single_gain->reduce_skill_hurt, false, gain, curActor);
			oppo_actor->reduce_skill_hurt_buff->addElement(new HistoryActor::RangeEffect(2, oppo_actor->m_mActorBuff[ACTOR_BUFF_HURTDW]->iPercent, 100));
		}

		if(single_gain->state_shield_range)
		{
			if (single_gain->state_shield_range > 0 && oppo_actor->good_shield_range->size() == 0)
			{
				map<int, int> mSpecial = oppo_actor->getSpecialMap(TYPE731);
				oppo_actor->good_shield_range->addElement(new HistoryActor::RangeEffect(mSpecial[1], 20, 100));
				oppo_actor->m_iShieldNum = mSpecial[2];
			}
			else if (single_gain->state_shield_range < 0 && oppo_actor->good_shield_range->size())
			{
				oppo_actor->good_shield_range->removeAllElements();
				oppo_actor->m_iShieldNum = 0;
			}
		}
	
		for(map<int, ActorBuff*>::iterator it = oppo_actor->m_mActorBuff.begin(); it != oppo_actor->m_mActorBuff.end(); it++)
		{
			ActorBuff* pActBuff = it->second;
			if (pActBuff->iArmIndex)
			{
				cur_list->addElement(new ScriptRunBuffAction(oppo_actor, pActBuff->iBuffId));
			}
		}

		if (gain->skill != NULL&&single_gain->probability == 0 && single_gain->gain_hp == 0 && single_gain->land_strict == 0)
		{
			cur_list->addElement(new ScriptChangeActorState(oppo_actor->actor_id, UNNORMAL_STATE_MISS));
		}

		if (single_gain->border_on_death<0)
		{
			oppo_actor->border_on_death = 1;
		}
		else if (single_gain->border_on_death > 0)
			oppo_actor->border_on_death = 0;

		if (single_gain->cCurAir > -1)
		{
			history->updateWeather(single_gain->cCurAir + 1);
		}

		if (single_gain->action_again)
			oppo_actor->action_finished = 0;

		if (single_gain->controlled_enemy_actorId)
		{
			bool bEnable = false;
			for (int i = 0; i < history->battleEnemyNum; i++)
			{
				if (history->battleEnemy[i]->actor_id + 1 == single_gain->controlled_enemy_actorId&&history->battleEnemy[i]->GetCurHp()>0 && history->battleOwnNum<MAX_OWN_ARMY_NUM)
				{
					history->battleOwn[history->battleOwnNum] = history->battleEnemy[i];
					history->battleOwn[history->battleOwnNum]->relation = 0; 
					history->battleOwn[history->battleOwnNum]->ai_type = 1;
					history->battleOwn[history->battleOwnNum]->controlled_by_enemy = 1;
					history->battleOwn[history->battleOwnNum]->controlled_by_enemy_round = 0;

					if (gain->skill)
					{
						history->battleOwn[history->battleOwnNum]->controlled_by_enemy_addAbi = trData->skills[gain->skill->skill_id].prop_per + gain->skill->GetExtraGainHp(curActor, 3);
						history->battleOwn[history->battleOwnNum]->controlled_by_enemy = trData->skills[gain->skill->skill_id].seid_round;
					}
					else
						history->battleOwn[history->battleOwnNum]->controlled_by_enemy_addAbi = 0;

// 					if (gain->skill->seid == 74 && gain->skill->power)
// 						history->battleOwn[history->battleOwnNum]->controlled_by_enemy = gain->skill->power;
					history->battleOwnNum++;
					bEnable = true;
				}
				if (bEnable)
				{
					for (int j = i; j < history->battleEnemyNum-1; j++)
					{
						history->battleEnemy[j] = history->battleEnemy[j + 1];
					}
					history->battleEnemy[history->battleEnemyNum - 1] = NULL;
					history->battleEnemyNum--;
					break;
				}
			}
		}

		if (single_gain->actor_relive != 0)
		{
			HistoryActorPtr temp = history->FindActor(single_gain->actor_relive_id);
			temp->hide = 1;
			temp->is_leave = 1;
			//px py actor_id
			cur_list->addElement(new ScriptBattleActorRelive(single_gain->actor_relive_id, single_gain->actor_relive_px, single_gain->actor_relive_py));
		}

		if (single_gain->actor_tower_def)
		{
			if (single_gain->actor_tower_def > 0 && single_gain->actor_tower_def < 4)
			{
				cur_list->addElement(new ScriptBattleActorRelive(curActor, single_gain->actor_tower_def + 899, gain->hitPoint.x, gain->hitPoint.y, curActor->GetCurSkillLevel(gain->skill->skill_id+1)));
			}
			else if (single_gain->actor_tower_def == 4)
			{
				cur_list->addElement(new ScriptBattleActorRelive(curActor, single_gain->actor_tower_def + 899, gain->hitPoint.x, gain->hitPoint.y, curActor->GetCurSkillLevel(gain->skill->skill_id+1),2));
			}
			else if (single_gain->actor_tower_def == 5)
			{
				cur_list->addElement(new ScriptBattleActorRelive(curActor, 903, gain->hitPoint.x, gain->hitPoint.y, curActor->GetCurSkillLevel(gain->skill->skill_id+1), 3));
			}
		}

		if (single_gain->fast_move)
		{
			BattleActor::clearMoveList();
            single_gain->oppo_actor->GenerateDefaultMove(single_gain->oppo_actor->x, single_gain->oppo_actor->y,single_gain->cur_point.x, single_gain->cur_point.y, 1);
			cur_list->addElement(new ScriptActorMove(single_gain->oppo_actor->actor_id, single_gain->cur_point.x, single_gain->cur_point.y, -1));
		}

		if (curActor->isHadSpecial(TYPE726) && oppo_actor->GetCurHp() && oppo_actor->GetCurHp() + single_gain->gain_hp <= 0 && curActor->mSeidAddGain[TYPE726] < curActor->getSpecialMap(TYPE726)[1])
		{
			curActor->mSeidAddGain[TYPE726] += 1;
		}

		if (oppo_actor->good_hurt_back && !curActor->isHadSpecial(TYPE729) && oppo_actor->isHadSpecial(TYPE726) && curActor->GetCurHp() && gain->gain_hp+curActor->GetCurHp()<=0&& single_gain->oppo_actor->mSeidAddGain[TYPE726] < single_gain->oppo_actor->getSpecialMap(TYPE726)[1])
		{
			oppo_actor->mSeidAddGain[TYPE726] += 1;
		}
		if (curActor->isHadSpecial(TYPE760) && gain->skill == NULL&&gain->good == NULL&&curActor->mSeidAddGain[TYPE760] < curActor->getSpecialMap(TYPE760)[1])
		{
			curActor->mSeidAddGain[TYPE760] += 1;
		}

		HistoryActorPtr jjOppoActor = oppo_actor;
		if (single_gain->share_actor)
		{
			jjOppoActor = single_gain->share_actor;
		}
		if (trData->occupations[jjOppoActor->occupation_id].is_robot == 2 && jjOppoActor->relation == 0&& jjOppoActor->GetCurHp() && jjOppoActor->GetCurHp() + single_gain->gain_hp <= 0)
		{
			for (int i = 900; i < 904; i++)
			{
				if (!history->actors[i].hide)
				{
					cur_list->addElement(new ScriptChangeActorState(i,history->actors[i].GetCurHp(), 0, 0, 0, 0, 0));
				}
			}
		}

		if (gain->skill&&gain->skill->iActorUseNum)
		{
			oppo_actor->mSkillBeUseNum[gain->skill->skill_id]++;
		}
	}

	if (gain->skill&&(gain->skill->seid == 124 || gain->skill->iTimes))
	{
		curActor->mSkillUseNumLimt[gain->skill->skill_id] += 1;
	}

	//自爆
	if (gain->skill&&gain->skill->seid == 128)
	{
		gain->gain_hp -= curActor->GetCurHp();
	}

	if (trData->occupations[curActor->occupation_id].is_robot == 2 && curActor->relation == 0 && curActor->GetCurHp() && curActor->GetCurHp() + gain->gain_hp <= 0)
	{
		for (int i = 900; i < 904; i++)
		{
			if (!history->actors[i].hide)
			{
				cur_list->addElement(new ScriptChangeActorState(i, history->actors[i].GetCurHp(), 0, 0, 0, 0, 0));
			}
		}
	}

	if (gain->gain_hp || gain->gain_mp)
	{
		cur_list->addElement(new ScriptChangeActorState(curActor->actor_id, -gain->gain_hp, -gain->gain_mp, 0, 0, 0, 0));
	}

	WinConditionTool::getInstance()->BattleSetCondition(gain, curActor);
}
// 致命一击，连击，引导攻击，反击怎么做？ (连击、反击和致命一击可以做了。引导攻击需要另外做一次。)
void ScriptHandler::GenerateScriptAttack(ActionGainPtr gain, HistoryActorPtr curActor)
{
	int i;
	ActionSingleGainPtr single_gain = NULL;
	HistoryActorPtr oppo_actor;
	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);
		oppo_actor = single_gain->oppo_actor;
		if (i == 0)
		{
			if (curActor->actor_fit_id.size())
				GetDoubleSeid(curActor);
			if (curActor->paint_special_effect[0] == 1)
			{
				cur_list->addElement(new ScriptSoundSet(curActor->paint_special_effect[1]));
				cur_list->addElement(new ScriptDelaySet(38));
			}
			cur_list->addElement(new ScriptBattleActorTurn(curActor->actor_id, oppo_actor->actor_id, 0, 0));

			auto _layer = Director::getInstance()->getRunningScene()->getChildByName("GuideLayer");
			if (gain->deadly_attack && _layer==NULL)
			{
				if (history->m_iBattleType != BATTLE_PVP)
				{
					int ran = rand() % 100;
					
					if (strlen(trData->actors[curActor->actor_id].rouse_word) != 0 && strcmp(trData->actors[curActor->actor_id].rouse_word, "null") && ran < 50 )
						cur_list->addElement(new ScriptActorTalk(trData->actors[curActor->actor_id].rouse_word, curActor->actor_id));
					else
					{
						int ran = rand() % 100;

						if (strlen(trData->actors[curActor->actor_id].rouse_word) != 0 && strcmp(trData->actors[curActor->actor_id].rouse_word, "null") && ran < 50)
							cur_list->addElement(new ScriptActorTalk(trData->actors[curActor->actor_id].rouse_word, curActor->actor_id));
						else
						{

							int ran = rand() % 3;
							if (strlen(trData->kill_words[trData->killword[curActor->actor_id]][ran]) != 0)
							{
								cur_list->addElement(new ScriptActorTalk(trData->kill_words[trData->killword[curActor->actor_id]][ran], curActor->actor_id));
							}
						}
					}
				}
				cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_DEADLY_ATTACK_PREPARE, 0, 0));
			}
			else
			{
				cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_ATTACK_PREPARE_SOUND, 0, 0));
			}
			cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_NORMAL_ATTACK, 0, TIMER_ACTOR_ATTACK_FRAME * 2));
		}
		if (oppo_actor->GetCurHp() == 0)
		{
			cur_list->addElement(new ScriptSoundSet(SOUND_ATTACK));
			cur_list->addElement(new ScriptDelaySet(TIMER_ACTOR_HIT_DELAY));
			if (trData->occupations[curActor->occupation_id].effect != "")
			{
				cur_list->addElement(new ScriptRunAction(NULL, oppo_actor, trData->occupations[curActor->occupation_id].effect.c_str(), ACTOR_STATE_RUN_ACTION, 20));
			}
		}
		else if (single_gain->probability)
		{
			int iShareId = -1;
			if(single_gain->share_actor)
			{
				iShareId = single_gain->share_actor->actor_id;
			}

			if (gain->deadly_attack)
				cur_list->addElement(new ScriptSoundSet(SOUND_DEAD_ATTACK_HIT));
			else
				cur_list->addElement(new ScriptSoundSet(SOUND_ATTACK_HIT));
			cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_ATTACKED, 0, TIMER_ACTOR_HIT_DELAY, single_gain->gain_hp, single_gain->gain_mp, gain->deadly_attack ? true : false, iShareId));

			if (trData->occupations[curActor->occupation_id].effect != "")
			{
				cur_list->addElement(new ScriptRunAction(NULL, oppo_actor, trData->occupations[curActor->occupation_id].effect.c_str(), ACTOR_STATE_RUN_ACTION, 20));
			}
		}
		else
		{
			//修改防御时伤害为0 TODO 暂时只对血量进行处理
			if (gain->deadly_attack)
				cur_list->addElement(new ScriptSoundSet(SOUND_DEADLY_ATTACK_NO_HIT));
			else
				cur_list->addElement(new ScriptSoundSet(SOUND_ATTACK_NO_HIT));
			cur_list->addElement(new ScriptBattleActorTurn(oppo_actor->actor_id, curActor->actor_id, 0, 0));
			cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_ACTION_DEF, 0, TIMER_ACTOR_HIT_DELAY));
		}
	}

	GenerateScriptState(gain, curActor);
	GenerateScriptUseGood(gain);

}

//获得双人特技
void ScriptHandler::GetDoubleSeid(HistoryActorPtr curActor)
{
	int iSound = curActor->GetDoubleSeid(1, false);
	if (iSound > -1)
	{
		curActor->paint_special_effect[0] = 1;
		curActor->paint_special_effect[1] = iSound;
	}
}

void ScriptHandler::GenerateScriptUseGood(ActionGainPtr gain) //自动使用补血道具
{
	for (int i = 0; i < gain->gains->size(); i++)
	{
		ActionSingleGainPtr single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);

		if(single_gain->gain_hp >= 0) //格挡不吃豆
		{
			continue;
		}

		HistoryActorPtr oppo_actor = single_gain->oppo_actor;
		int good_id = -1;
		bool is_loss = false;

		if(oppo_actor->good_auto_use_good)
		{
			good_id = oppo_actor->good_auto_use_good;
			is_loss = false;
		}
		else if(oppo_actor->good_auto_use_good_loss)
		{
			good_id = oppo_actor->good_auto_use_good_loss;
			is_loss = true;
		}
		if (oppo_actor->good_effects[47])
		{
			good_id = oppo_actor->good_effects[47] - 1;
			is_loss = true;
		}

		if(good_id >= 0)
		{
			HistoryGoods hisGoods(good_id);
			int cur_hp = oppo_actor->GetCurHp() + single_gain->gain_hp; //收益后的HP
			int cur_mp = oppo_actor->GetCurMp() + single_gain->gain_mp; //收益后的MP
			int gain_hp = 0;
			int gain_mp = 0;
			switch(hisGoods.getUeId())
			{
				case 1:// 恢复HP(整数)
				{
					if(hisGoods.GetGoodsPower() == 65535)
						gain_hp = oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp() - oppo_actor->GetCurHp();
					else
						gain_hp = hisGoods.GetGoodsPower();

					if (oppo_actor->isHadSpecial(TYPE723))
					{
						int iSeidNum = oppo_actor->getSpecialInt(TYPE723);
						gain_hp += gain_hp*iSeidNum/100;
					}
				}
					break;
				case 2:// 恢复MP(整数)
				{
					if(hisGoods.GetGoodsPower() == 65535)
						gain_mp = oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp() - oppo_actor->GetCurMp();
					else
						gain_mp = hisGoods.GetGoodsPower();
				}
					break;
			}
			if ((gain_hp && cur_hp > 0 && cur_hp + gain_hp < oppo_actor->GetBaseHp() + oppo_actor->GetGoodsAddHp()) || (cur_hp > 0 && gain_mp && cur_mp + gain_mp < oppo_actor->GetBaseMp() + oppo_actor->GetGoodsAddMp()) || (cur_hp > 0 && gain_hp && oppo_actor->good_effects[47] && single_gain->gain_hp<0)) //武将未死亡&&补给后血量不溢出
			{
				if(is_loss && oppo_actor->relation == 0) //消耗库存&&我方阵营&&消耗品库存不为零
				{
					if(history->m_mConsuNum[good_id] > 0)
					{
						history->m_mConsuNum[good_id]--;
						WinConditionTool::getInstance()->AddBattleConsumes(good_id);
					}
					else //库存为零就跳过
					{
						continue;
					}
				}

				BattleActor::SetGoodImage(good_id);
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_GOOD_GIVE, 0, 0));
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_GOOD_ACCEPT, 0, 0));
				if (gain_hp || gain_mp)
				{
					cur_list->addElement(new ScriptChangeActorState(oppo_actor->actor_id, -gain_hp, -gain_mp, 0, 0, 0, 0));
				}
			}
		}
	}
}

void ScriptHandler::GenerateScriptSkill(ActionGainPtr gain, HistoryActorPtr curActor)
{
	int i;
	ActionSingleGainPtr single_gain = NULL;
	HistoryActorPtr oppo_actor;
	SkillAttrPtr skill = gain->skill;

	oppo_actor = ((ActionSingleGainPtr)gain->gains->elementAt(0))->oppo_actor;

	if (skill->full_anim)
	{
		cur_list->addElement(new ScriptCallMagic(skill->full_anim + 99, oppo_actor->x, oppo_actor->y));
	}

	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);
		oppo_actor = single_gain->oppo_actor;
		if (i == 0)
		{
			if (skill->skill_limit == 30)
			{
				BattleActor::SetGoodImage(0);
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_GOOD_GIVE, 0, 0));
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_GOOD_ACCEPT, 0, 0));
			}
			cur_list->addElement(new ScriptBattleActorTurn(curActor->actor_id, oppo_actor->actor_id, 0, TIMER_ACTOR_TURN_DELAY));
			cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_SKILL_PREPARE, 0, 0));
			if (skill->animation[0])
				cur_list->addElement(new ScriptRunAction(curActor, oppo_actor, skill->animation[0], skill->animation[2], ACTOR_STATE_RUN_ACTION, 20,0,true));
			if(!gain->iDontShowSkillName)
			{
				cur_list->addElement(new ScriptCommonInfo(skill->skill_name, LABEL_SKILL, skill->animation[0] ? true : false));
			}
			if (skill->animation[1])
			{
				HistoryActorPtr ptrAnimationActor = oppo_actor;
				if (/*trData->hitArea[skill->use_region].length == -1 ||*/trData->hitArea[skill->use_region].length == -5)
					ptrAnimationActor = curActor;
				cur_list->addElement(new ScriptRunAction(curActor, ptrAnimationActor, skill->animation[1], skill->animation[2], ACTOR_STATE_RUN_ACTION, 20,skill->skill_mp3));
			}

			if(single_gain->iIsHitback)
			{
				BattleActor::clearMoveList();

				if(single_gain->iHitbackId > -1)
				{
					cur_list->addElement(new ScriptActorMove(single_gain->iHitbackId, single_gain->oppoMovePos.x, single_gain->oppoMovePos.y, -1));
				}

				cur_list->addElement(new ScriptActorMove(curActor->actor_id, single_gain->curMovePos.x, single_gain->curMovePos.y, -1));
			}
		}
	}
	if (skill->delay1>0)
		cur_list->addElement(new ScriptDelaySet(skill->delay1));

	for (i = 0; i < gain->gains->size(); i++)
	{
		single_gain = (ActionSingleGainPtr)gain->gains->elementAt(i);
		if (single_gain->land_strict != 0)
			continue;
		oppo_actor = single_gain->oppo_actor;

		SSCHAR have_state = single_gain->state_power | single_gain->state_intellegence | single_gain->state_defence | single_gain->state_explosion | single_gain->state_fortune |
			single_gain->state_obfuscation | single_gain->state_blind | single_gain->state_puzzle | single_gain->state_poison | single_gain->state_palsy | single_gain->state_stun | single_gain->state_speed;

		if (have_state || (have_state == 0 && single_gain->probability))
		{
			int iShareId = -1;
			if(single_gain->share_actor)
			{
				iShareId = single_gain->share_actor->actor_id;
			}

			if (gain->skill->skill_object == 0)
			{
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_ATTACKED, 0, 0, single_gain->gain_hp, single_gain->gain_mp, false, iShareId));
			}
			else
			{
				cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_CELEBRATE_NO_SOUND, 0, 0, single_gain->gain_hp, single_gain->gain_mp, false, iShareId));
			}

			cur_list->addElement(new ScriptRunAction(NULL,oppo_actor, gain->skill->effect.c_str(), ACTOR_STATE_RUN_ACTION, 20));
		}
		else
		{   //修改防御时伤害为0 TODO 暂时只对血量进行处理
			single_gain->gain_hp = 0;
			single_gain->gain_mp = 0;
			cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_ACTION_DEF, 0, 0));
		}
	}

	if (skill->delay2>0)
		cur_list->addElement(new ScriptDelaySet(skill->delay2));

	GenerateScriptState(gain,curActor);
	GenerateScriptUseGood(gain);
}
void ScriptHandler::GenerateScriptGood(ActionGainPtr gain, HistoryActorPtr curActor)
{
	//	int power = 0;
	ActionSingleGainPtr single_gain = (ActionSingleGainPtr)gain->gains->elementAt(0);
	HistoryActorPtr oppo_actor = single_gain->oppo_actor;
	BattleActor::SetGoodImage(gain->good->iGoodsId);

	WinConditionTool::getInstance()->AddBattleConsumes(gain->good->iGoodsId);
	history->m_mConsuNum[gain->good->iGoodsId]--;
	cur_list->addElement(new ScriptBattleActorAction(curActor->actor_id, ACTOR_STATE_GOOD_GIVE, 0, 0));
	cur_list->addElement(new ScriptBattleActorAction(oppo_actor->actor_id, ACTOR_STATE_GOOD_ACCEPT, 0, 0));
	GenerateScriptState(gain, curActor);
}
void ScriptHandler::GenerateScriptListFromAI(const BattleAI& ai)
{
	ActionGainPtr gain = ai.mostGain;
	HistoryActorPtr curActor = ai.curActor;
	script_index = 0;
	is_script_skipto = false;
	cur_list = new DList<Script>(true);
	if (gain->good != NULL)
	{
		GenerateScriptGood(gain, curActor);
	}
	else if (gain->skill != NULL)
	{
		GenerateScriptSkill(gain, curActor);
	}
	else
	{
		GenerateScriptAttack(gain, curActor);
	}
	cur_script = (ScriptPtr)cur_list->elementAt(0);
}

void ScriptHandler::ClearScriptList()
{
	SAFE_DELETE(cur_list);
}
void ScriptHandler::SetScriptList(int index)
{
	script_index = 0;
	cur_list = script_lists->elementAt(index);
	cur_script = cur_list->elementAt(0);
	list_index = index;
}
bool ScriptHandler::RunRScript(RedeScreenPtr scr)
{
	if (scr->rede_state == REDE_STATE_ACTOR_MOVE)
	{
		if (cur_script->cmd != CMD_ACTOR_MOVE)
			return false;
	}
	if (cur_list != NULL)
	{
		while (scr->rede_state != REDE_STATE_ACTOR_MOVE || cur_script->cmd == CMD_ACTOR_MOVE)
		{
			int action_need_frame = cur_script->HandleRScript(scr);
			if (action_need_frame < 0)
			{
				if (cur_script->cmd != CMD_CASE)
				{
					script_skipto_begin = false;
				}
				TestFalse();
				action_need_frame = 0;
			}
			if (action_need_frame != 0)
				break;
			else
			{
				cur_script->frame = 0;
				cur_script->need_frame = 0;
				script_index++;
				if (script_index >= cur_list->size())
				{
					if (scr->rede_state == REDE_STATE_WAIT_COMMAND || ((list_index == script_lists->size() - 1) && !history->isFreeMove))
					{
						cur_script = (ScriptPtr)cur_list->elementAt(0);
						history->r_script_index = list_index;
						script_index = 0;
						return false;
					}
					else
					{
						if (history->isFreeMove)
						{
							scr->rede_state = REDE_STATE_WAIT_COMMAND;
						}
						else
						{
							++list_index;
						}
						cur_list = script_lists->elementAt(list_index);
						cur_script = cur_list->elementAt(0);
						history->r_script_index = list_index;
						script_index = 0;
						return true;
					}

				}
				else
				{
					cur_script = (ScriptPtr)cur_list->elementAt(script_index);
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool ScriptHandler::RunScript()
{
	if (cur_list != NULL)
	{
		while (1)
		{
			if (script_index >= cur_list->size())
				return false;
			int action_need_frame = cur_script->HandleScript();
			if (action_need_frame < 0)
			{
				if (cur_script->cmd != CMD_CASE)
				{
					script_skipto_begin = false;
				}
				cur_script->frame = 0;
				TestFalse();
				action_need_frame = 0;
			}
			if (action_need_frame != 0)
				break;
			else
			{
				cur_script->frame = 0;
				if (script_index >= cur_list->size() - 1)
					return false;
				cur_script = (ScriptPtr)cur_list->elementAt(++script_index);

			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


void ScriptHandler::TestFalse()
{
	int in_child = 1;
	while (in_child != 0)
	{
		cur_script = (ScriptPtr)cur_list->elementAt(script_index++);
		switch (cur_script->cmd)
		{
		case CMD_CHILD_ACTION:
			in_child++;
			break;
		case CMD_ACTION_END:
			in_child--;
			break;
		}
	}
	script_index--;
}

void ScriptHandler::LoadRFile(int index)
{
	char filename[40];
	if (index < 10)
	{
        sprintf(filename, "%s%d%s", "Scenes/R_0", index, ".eex_new");
		
	}
	else
	{
        sprintf(filename, "%s%d%s", "Scenes/R_", index, ".eex_new");
		
	}
	char* game_data = ChessReadFile(filename);
	if (game_data != NULL)
	{
		DecodeScenes(game_data);
		ChessFree(game_data);
		script_index = 0;
		list_index = 0;
		cur_list = script_lists->elementAt(list_index);
		cur_script = cur_list->elementAt(script_index);
		return;
	}
	cur_list = NULL;
	cur_script = NULL;
}

void ScriptHandler::LoadRFileName(const char* file_name)
{
	char filename[40];
    sprintf(filename, "%s%s%s", "Scenes/", file_name, ".eex_new");
	char* game_data = ChessReadFile(filename);
	if (game_data != NULL)
	{
		DecodeScenes(game_data);
		ChessFree(game_data);
		script_index = 0;
		list_index = 0;
		cur_list = script_lists->elementAt(list_index);
		cur_script = cur_list->elementAt(script_index);
		return;
	}
	cur_list = NULL;
	cur_script = NULL;
}

void ScriptHandler::LoadSFile(int index)
{
	char filename[40];
	char filename1[40];
	if (index < 10)
	{
        sprintf(filename, "%s%d%s", "Scenes/S_0", index, ".eex_new");
        sprintf(filename1, "%s%d%s", "S_0", index, ".eex_new");
		
	}
	else
	{
        sprintf(filename, "%s%d%s", "Scenes/S_", index, ".eex_new");
        sprintf(filename1, "%s%d%s", "S_", index, ".eex_new");
		
	}


	char* game_data = ChessReadFile(filename);

	StatConfig::onBeginMission(filename1);
	if (game_data != NULL)
	{
		DecodeScenes(game_data);
		ChessFree(game_data);
		return;
	}
	return;
}

void ScriptHandler::LoadSFileName(const char* file_name)
{
	char filename[40];
	char filename1[40];
    sprintf(filename, "%s%s%s", "Scenes/", file_name, ".eex_new");
	sprintf(filename1, "%s%s", file_name, ".eex_new");
	char* game_data = ChessReadFile(filename);

	StatConfig::onBeginMission(filename1);
	if (game_data != NULL)
	{
		DecodeScenes(game_data);
		ChessFree(game_data);
		return;
	}
	return;
}

void ScriptHandler::DecodeScenes(char* SceneData, bool bNormal)
{
	if(bNormal)
	{
		script_lists->removeAllElements();
	}
	else
	{
		pre_battle_list->removeAllElements();
	}

	int index = 0x0A;
	int suite_offset = bytesToInt(SceneData, index);
	int suite_num = (suite_offset - index) / INT_NUM_LEN;
	int subScript = -1;
	int iVar4071 = history->var2[4071];
	int iVar5004 = history->var2[5004];
	int iVar5006 = history->var2[5006];
	for(int i = 0; i < suite_num; i++)
	{
		subScript++;
		DList<Script>* curSuite;
		curSuite = new DList<Script>(true);

		index = 0x0A + i*INT_NUM_LEN;
		suite_offset = bytesToInt(SceneData, index);
		index = suite_offset;

		short section_num = bytesToShort(SceneData, index);
		index += SHORT_NUM_LEN;

		int* ptrsec_index = new int[section_num + 1];
		memset(ptrsec_index, 0, sizeof(ptrsec_index));

		int cursec_index = 0;
		for(int j = 0; j < section_num; j++)
		{
			subScript++;
			int finish_flag = 1;
			int in_child_section = 0;
			int in_child_section2 = 0;
			index += SHORT_NUM_LEN;
			do
			{
				short curCmd = bytesToShort(SceneData, index);
				index += SHORT_NUM_LEN;
				ScriptPtr curScript = NULL;
				subScript++;
				switch(curCmd)
				{
				case CMD_ACTION_END:{
					if(in_child_section2 != 0)
					{
						//							section_info_len = bytesToShort(SceneData, index);
						index += SHORT_NUM_LEN;
						curScript = NULL;
						in_child_section2 = 0;
						continue;
					}
					else
						finish_flag--;
					break;// = 0x0;//事件结束
				}
				case CMD_CHILD_ACTION:
				{
					in_child_section = 1;
					in_child_section2 = 0;
					finish_flag++;
					break;// = 0x1;//子事件设定
				}
				case CMD_CHILD_INFO_ACTION:
				{
					ScriptChildInfoPtr para = new ScriptChildInfo();
					if(in_child_section == 0)
						in_child_section2 = 1;
					curScript = para;
					break;// = 0x2;//子事件设定带信息
				}
				case CMD_ELSE:
				{
					ScriptElsePtr para = new ScriptElse();
					curScript = para;
					break;// = 0x3;//else
				}
				case CMD_REQUEST_TEST:
				{
					ScriptTestAskPtr para = new ScriptTestAsk();
					curScript = para;
					break;// = 0x4;//询问测试
				}
				case CMD_TEST_VALUE:
				{
					ScriptTestValuePtr para = new ScriptTestValue();
					curScript = para;
					break;// = 0x5;//变量测试
				}
				case CMD_FORCE_LIMIT:
				{
					ScriptForceLimitPtr para = new ScriptForceLimit();
					curScript = para;
					break;// = 0x6;//出场限制
				}
				case CMD_TEST_START_BATTLE:
				{
					ScriptTestStartBattlePtr para = new ScriptTestStartBattle();
					curScript = para;
					if(history->r_script_index == -1 && bNormal)
					{
						history->r_script_index = i;
					}
				}
					break;// = 0x7;//出战测试
				case CMD_MENU_VISIBLE:{
					ScriptMenuVisiblePtr para = new ScriptMenuVisible();
					curScript = para;
					break;// = 0x8;//菜单处理
				}
				case CMD_DELAY_SET:{
					ScriptDelaySetPtr para = new ScriptDelaySet();
					curScript = para;
					break;// = 0x9;//延时
				}
				case CMD_INIT_VALUE:{
					ScriptInitValuePtr para = new ScriptInitValue();
					curScript = para;
					break;// = 0xa;//初始化局部变量
				}
				case CMD_SET_VALUE:{
					ScriptSetValuePtr para = new ScriptSetValue();
					curScript = para;
					break;// = 0xb;//变量赋值
				}
				case CMD_c:{
					ScriptSectionEndPtr para = new ScriptSectionEnd();
					curScript = para;
					break;// = 0xc;//结束Section
				}
				case CMD_BATTLE_END:{
					ScriptBattleEndPtr para = new ScriptBattleEnd();
					curScript = para;
					break;// = 0xd;//结束Scene
				}
				case CMD_BATTLE_FAIL:{
					ScriptBattleFailPtr para = new ScriptBattleFail();
					curScript = para;
					break;// = 0xe;//战斗失败
				}
				case CMD_GAME_ENDING:{
					ScriptGameEndingPtr para = new ScriptGameEnding();
					curScript = para;
					break;// = 0xf;//结局设定
				}
				case CMD_10:{
					break;// = 0x10;//许子将指导
				}
				case CMD_SCENE_CHANGE:{
					ScriptSceneChangePtr para = new ScriptSceneChange();
					curScript = para;
					break;// = 0x11;//剧本跳转
				}
				case CMD_SHOW_CHOICE:{
					ScriptShowChoicePtr para = new ScriptShowChoice();
					curScript = para;
					break;// = 0x12;//选择框
				}
				case CMD_CASE:{
					ScriptCasePtr para = new ScriptCase();
					curScript = para;
					break;// = 0x13;//case
				}
				case CMD_ACTOR_TALK:{
					ScriptActorTalkPtr para = new ScriptActorTalk();
					curScript = para;
					break;// = 0x14;//对话
				}
				case CMD_ACTOR_TALK2:{
					ScriptActorTalk2Ptr para = new ScriptActorTalk2();
					curScript = para;
					break;// = 0x15;//对话2
				}
				case CMD_VICTORY_CONDITION:// = 0x19;//胜利条件
				case CMD_SCENE_NAME:// = 0x17;//场所名
				case CMD_EVENT_NAME:// = 0x18;//事件名称设定
				case CMD_69:
				case CMD_SHOW_VICTORY_CONDITION:// = 0x1a;//显示胜利条件
				case CMD_NORMAL_INFO:{
					ScriptCommonInfoPtr para = new ScriptCommonInfo(curCmd);
					curScript = para;
					break;// = 0x16;//信息
				}
				case CMD_LEAVE_MSG_SHOW_OR_NOT:{
					ScriptLeaveMsgShowOrNotPtr para = new ScriptLeaveMsgShowOrNot();
					curScript = para;
					break;// = 0x1b;//撤退信息是否显示设定
				}
				case CMD_DRAW:{
					ScriptDrawPtr para = new ScriptDraw();
					curScript = para;
					break;// = 0x1c;//绘图
				}
				case CMD_1d:{
					ScriptPaletteSetPtr para = new ScriptPaletteSet();
					curScript = para;
					break;// = 0x1d;//调色板设定
				}
				case CMD_1e:{
					break;// = 0x1e;//武将重绘
				}
				case CMD_CHANGE_VIEWPOINT:{
					ScriptChangeViewPointPtr para = new ScriptChangeViewPoint();
					curScript = para;
					break;// = 0x1f;//地图视点切换
				}
				case CMD_HEAD_STATE_SET:{
					ScriptHeadStateSetPtr para = new ScriptHeadStateSet();
					curScript = para;
					break;// = 0x20;//武将头像状态设置
				}
				case CMD_ADD_MAP_ELEM:{
					ScriptAddMapElemPtr para = new ScriptAddMapElem();
					curScript = para;
					break;// = 0x21;//战场物体添加
				}
				case CMD_MOVIE_SET:{
					ScriptMovieSetPtr para = new ScriptMovieSet();
					curScript = para;
					break;// = 0x22;//动画
				}
				case CMD_SOUND_SET:{
					ScriptSoundSetPtr para = new ScriptSoundSet();
					curScript = para;
					break;// = 0x23;//音效
				}
				case CMD_MUSIC_SET:{
					ScriptMusicSetPtr para = new ScriptMusicSet();
					curScript = para;
					break;// = 0x24;//CD音轨
				}
				case CMD_TEST_ACTOR_POS:{
					ScriptTestActorPosPtr para = new ScriptTestActorPos();
					curScript = para;
					break;// = 0x25;//武将进入地点测试
				}
				case CMD_TEST_ACTOR_AREA:{
					ScriptTestActorAreaPtr para = new ScriptTestActorArea();
					curScript = para;
					break;// = 0x26;//武将进入指定区域测试
				}
				case CMD_BG_SET:{
					ScriptBGSetPtr para = new ScriptBGSet();
					curScript = para;
					break;// = 0x27;//背景显示
				}
				case CMD_28:{
					ScriptFreeMovePtr para = new ScriptFreeMove();
					curScript = para;
					break;// = 0x28;//?三国地图显示？
				}
				case CMD_MAP_FACE_APPEAR:{
					ScriptMapFaceAppearPtr para = new ScriptMapFaceAppear();
					curScript = para;
					break;// = 0x29;//地图头像显示
				}
				case CMD_MAP_FACE_MOVE:{
					ScriptMapFaceMovePtr para = new ScriptMapFaceMove();
					curScript = para;
					break;// = 0x2a;//地图头像移动
				}
				case CMD_MAP_FACE_DISAPPEAR:{
					ScriptMapFaceDisAppearPtr para = new ScriptMapFaceDisAppear();
					curScript = para;
					break;// = 0x2b;//地图头像消失
				}
				case CMD_MAP_TELL_INFO:{
					ScriptMapTellInfoPtr para = new ScriptMapTellInfo();
					curScript = para;
					break;// = 0x2c;//地图文字显示
				}
				case CMD_TEST_ACTOR_CLICKED:{
					ScriptTestActorClickedPtr para = new ScriptTestActorClicked();
					curScript = para;
					break;// = 0x2d;//武将点击测试
				}
				case CMD_TEST_ACTOR_BORDER:{
					ScriptTestActorBorderPtr para = new ScriptTestActorBorder();
					curScript = para;
					break;// = 0x2e;//武将相邻测试
				}
				case CMD_CLEAR_ACTOR:{
					ScriptClearActorPtr para = new ScriptClearActor();
					curScript = para;
					break;// = 0x2f;//清除人物
				}
				case CMD_ACTOR_APPEAR:{
					ScriptActorAppearPtr para = new ScriptActorAppear();
					curScript = para;
					break;// = 0x30;//武将出现
				}
				case CMD_ACTOR_DISAPPEAR:{
					ScriptActorDisappearPtr para = new ScriptActorDisappear();
					curScript = para;
					break;// = 0x31;//武将消失
				}
				case CMD_ACTOR_MOVE:{
					ScriptActorMovePtr para = new ScriptActorMove();
					curScript = para;
					break;// = 0x32;//武将移动
				}
				case CMD_ACTOR_TURN:{
					ScriptActorTurnPtr para = new ScriptActorTurn();
					curScript = para;
					break;// = 0x33;//武将转向
				}
				case CMD_ACTOR_ACTION:{
					ScriptActorActionPtr para = new ScriptActorAction();
					curScript = para;
					break;// = 0x34;//武将动作
				}
				case CMD_ACTOR_CHANGE_IMAGE:{
					ScriptActorChangeImagePtr para = new ScriptActorChangeImage();
					curScript = para;
					break;// = 0x35;//武将形象改变
				}
				case CMD_TEST_ACTOR_STATE:{
					ScriptTestActorStatePtr para = new ScriptTestActorState();
					curScript = para;
					break;// = 0x36;//武将状态测试
				}
				case CMD_MONEY_SCENE_ROUTE_TEST:{
					ScriptMoneyTestPtr para = new ScriptMoneyTest();
					curScript = para;
					break;// = 0x37;//钱、剧本跳转、忠奸测试
				}
				case CMD_SET_ACTOR_ABILITY:{
					ScriptSetActorAbilityPtr para = new ScriptSetActorAbility();
					curScript = para;
					break;// = 0x38;//武将能力设定
				}
				case CMD_ACTOR_LEVEL_UP:{
					ScriptActorLevelUpPtr para = new ScriptActorLevelUp();
					curScript = para;
					break;// = 0x39;//武将等级提升
				}
				case CMD_MONEY_CHANGE:{
					ScriptMoneyChangePtr para = new ScriptMoneyChange();
					curScript = para;
					break;// = 0x3a;//钱、剧本跳转、忠奸设置
				}
				case CMD_ARMY_CHANGE:{
					ScriptArmyChangePtr para = new ScriptArmyChange();
					curScript = para;
					break;// = 0x3b;//武将加入和离开
				}
				case CMD_TEST_ARMY_COME:{
					ScriptTestArmyComePtr para = new ScriptTestArmyCome();
					curScript = para;
					break;// = 0x3c;//武将加入测试
				}
				case CMD_GET_GOOD:{
					ScriptGetGoodPtr para = new ScriptGetGood();
					curScript = para;
					break;// = 0x3d;//获得物品
				}
				case CMD_GOOD_SET:{
					ScriptGoodSetPtr para = new ScriptGoodSet();
					curScript = para;
					break;// = 0x3e;//加入装备设定
				}
				case CMD_TEST_ROUND:{
					ScriptTestRoundPtr para = new ScriptTestRound();
					curScript = para;
					break;// = 0x3f;//回合测试
				}
				case CMD_TEST_ACTION_SIDE:{
					ScriptTestActionSidePtr para = new ScriptTestActionSide();
					curScript = para;
					break;// = 0x40;//行动方测试
				}
				case CMD_TEST_ACTOR_NUM:{
					ScriptTestActorNumPtr para = new ScriptTestActorNum();
					curScript = para;
					break;// = 0x41;//战斗人数测试
				}
				case CMD_TEST_VICTORY:{
					ScriptTestVictoryPtr para = new ScriptTestVictory();
					curScript = para;
					break;// = 0x42;//战斗胜利测试
				}
				case CMD_TEST_FAIL:{
					ScriptTestFailPtr para = new ScriptTestFail();
					curScript = para;
					break;// = 0x43;//战斗失败测试
				}
				case CMD_BATTLE_INIT:{
					break;// = 0x44;//战斗初始化
				}
				case CMD_BATTLE_SET_VALUE:{
					ScriptSetBattleValuePtr para = new ScriptSetBattleValue();
					curScript = para;
					break;// = 0x45;//战场全局变量
				}
				case CMD_DISPATCH_FRIEND:{
					ScriptDispatchFriendPtr para = new ScriptDispatchFriend();
					curScript = para;
					break;// = 0x46;//友军出场设定
				}
				case CMD_DISPATCH_ENEMY:{
					ScriptDispatchEnemyPtr para = new ScriptDispatchEnemy();
					curScript = para;
					break;// = 0x47;//敌军出场设定
				}
				case CMD_SET_ENEMY_GOODS:{
					ScriptSetEnemyGoodsPtr para = new ScriptSetEnemyGoods();
					curScript = para;
					break;// = 0x48;//敌军装备设定
				}
				case CMD_49:{
					break;// = 0x49;//战斗结束
				}
				case CMD_CONFIG_OWN_FORCE:{
					ScriptConfigOwnForcePtr para = new ScriptConfigOwnForce();
					curScript = para;
					break;// = 0x4a;//我军出场强制设定
				}
				case CMD_DISPATCH_OWN:{
					ScriptDispatchOwnPtr para = new ScriptDispatchOwn();
					curScript = para;
					break;// = 0x4b;//我军出场设定
				}
				case CMD_SET_ACTOR_VISIBLE:{
					ScriptSetActorVisiblePtr para = new ScriptSetActorVisible();
					curScript = para;
					break;// = 0x4c;//隐藏武将出现
				}
				case CMD_CHANGE_ACTOR_STATE:{
					ScriptChangeActorStatePtr para = new ScriptChangeActorState();
					curScript = para;
					break;// = 0x4d;//武将状态变更
				}
				case CMD_CHANGE_ACTOR_AI:{
					ScriptChangeActorAIPtr para = new ScriptChangeActorAI();
					curScript = para;
					break;// = 0x4e;//武将方针变更
				}
				case CMD_BATTLE_ACTOR_TURN:{
					ScriptBattleActorTurnPtr para = new ScriptBattleActorTurn();
					curScript = para;
					break;// = 0x4f;//战场转向设置
				}
				case CMD_BATTLE_ACTOR_ACTION:{
					ScriptBattleActorActionPtr para = new ScriptBattleActorAction();
					curScript = para;
					break;// = 0x50;//战场动作设置
				}
				case CMD_BATTLE_FIGHT_END:{
					break;// = 0x51;//战场恢复行动权
				}
				case CMD_CHANGE_ACTOR_OCCU:{
					ScriptChangeActorOccuPtr para = new ScriptChangeActorOccu();
					curScript = para;
					break;// = 0x52;//兵种改变
				}
				case CMD_BATTLE_ACTOR_LEAVE:{
					ScriptBattleActorLeavePtr para = new ScriptBattleActorLeave();
					curScript = para;
					break;// = 0x53;//战场撤退
				}
				case CMD_BATTLE_LEAVE_COMFORM:{
					ScriptBattleLeaveComformPtr para = new ScriptBattleLeaveComform();
					curScript = para;
					break;// = 0x54;//战场撤退确认
				}
				case CMD_BATTLE_ACTOR_RELIVE:{
					ScriptBattleActorRelivePtr para = new ScriptBattleActorRelive();
					curScript = para;
					break;// = 0x55;//战场复活
				}
				case CMD_SET_AIR_TYPE:{
					ScriptSetAirTypePtr para = new ScriptSetAirType();
					curScript = para;
					break;// = 0x56;//天气类别设定
				}
				case CMD_SET_CUR_AIR:{
					ScriptSetCurAirPtr para = new ScriptSetCurAir();
					curScript = para;
					break;// = 0x57;//当前天气设定
				}
				case CMD_SET_BATTLE_BARRIER:{
					ScriptSetBattleBarrierPtr para = new ScriptSetBattleBarrier();
					curScript = para;
					break;// = 0x58;//战场障碍设定
				}
				case CMD_SET_BATTLE_BONUS:{
					ScriptSetBattleBonusPtr para = new ScriptSetBattleBonus();
					curScript = para;
					break;// = 0x59;//战利品设定
				}
				case CMD_BATTLE_START:{
					ScriptBattleStartPtr para = new ScriptBattleStart();
					curScript = para;
					break;// = 0x5a;//战场操作开始
				}
				case CMD_BATTLE_HIGHLIGHT_AREA:{
					ScriptBattleHighlightAreaPtr para = new ScriptBattleHighlightArea();
					curScript = para;
					break;// = 0x5b;//战场高亮区域
				}
				case CMD_BATTLE_HIGHLIGHT_ACTOR:{
					ScriptBattleHighlightActorPtr para = new ScriptBattleHighlightActor();
					curScript = para;
					break;// = 0x5c;//战场高亮人物
				}
				case CMD_SET_MAX_ROUND:{
					ScriptSetMaxRoundPtr para = new ScriptSetMaxRound();
					curScript = para;
					break;// = 0x5d;//回合上限设定
				}
				case CMD_TEST_ACTOR_NOT_SAME:{
					ScriptTestActorNotSamePtr para = new ScriptTestActorNotSame();
					curScript = para;
					break;// = 0x5e;//武将不同测试
				}
				case CMD_PK_END:{
					ScriptPKEndPtr para = new ScriptPKEnd();
					curScript = para;
					break;// = 0x5f;//单挑结束 
				}
				case CMD_PK_ACTOR_APPEAR:{
					ScriptPKActorAppearPtr para = new ScriptPKActorAppear();
					curScript = para;
					break;// = 0x60;//单挑武将出场
				}
				case CMD_PK_SHOW_K_O:{
					ScriptPKShowKOPtr para = new ScriptPKShowKO();
					curScript = para;
					break;// = 0x61;//单挑胜负显示
				}
				case CMD_PK_ACTOR_DIE:{
					ScriptPKActorDiePtr para = new ScriptPKActorDie();
					curScript = para;
					break;// = 0x62;//单挑阵亡
				}
				case CMD_PK_ACTOR_TALK:{
					ScriptPKActorTalkPtr para = new ScriptPKActorTalk();
					curScript = para;
					break;// = 0x63;//单挑对话
				}
				case CMD_PK_ACTION:{
					ScriptPKActorActionPtr para = new ScriptPKActorAction();
					curScript = para;
					break;// = 0x64;//单挑动作
				}
				case CMD_PK_ATTACK1:{
					ScriptPKActorAttack1Ptr para = new ScriptPKActorAttack1();
					curScript = para;
					break;// = 0x65;//单挑攻击1
				}
				case CMD_PK_ATTACK2:{
					ScriptPKActorAttack2Ptr para = new ScriptPKActorAttack2();
					curScript = para;
					break;// = 0x66;//单挑攻击2
				}
				case CMD_CHAPTER_NAME:{
					ScriptChapterNamePtr para = new ScriptChapterName();
					curScript = para;
					break;// = 0x67;//章名
				}
				case CMD_PK_START:{
					ScriptPKStartPtr para = new ScriptPKStart();
					curScript = para;
					break;// = 0x68;//单挑开始
				}

				case CMD_6a:{
					break;// = 0x6a;//
				}
				case CMD_CALL_MAGIC:{
					ScriptCallMagicPtr para = new ScriptCallMagic();
					curScript = para;
					break;// = 0x6b;//法术召唤
				}
				case CMD_ACTOR_ABILITY_COPY:{
					ScriptActorAbilityCopyPtr para = new ScriptActorAbilityCopy();
					curScript = para;
					break;// = 0x6c;//武将能力复制
				}
				case CMD_RELA_REVIVEORMOVE:{
					ScriptRelaReviveOrMovePtr para = new ScriptRelaReviveOrMove();
					curScript = para;
					break;// = 0x6d;//相对复活或移动
				}
				case CMD_TEST_PROBALITY:{
					ScriptTestProbalityPtr para = new ScriptTestProbality();
					curScript = para;
					break;// = 0x6e;//概率测试
				}
				case CMD_GOOD_THROW:{
					ScriptGoodThrowPtr para = new ScriptGoodThrow();
					curScript = para;
					break;// = 0x6f;//丢弃物品
				}
				case CMD_ABILITY_CHOOSE_COPY:{
					ScriptAbilityChooseCopyPtr para = new ScriptAbilityChooseCopy();
					curScript = para;
					break;// = 0x70;//能力选择复制
				}
				case CMD_ASK_EFFECTS:{
					ScriptAskEffectsPtr para = new ScriptAskEffects();
					curScript = para;
					break;// = 0x71;//特效请求
				}
				case CMD_MSG_TRANSMIT:{
					ScriptMsgTransmitPtr para = new ScriptMsgTransmit();
					curScript = para;
					break;// = 0x72;//信息传送
				}
				case CMD_ACTOR_PROPERTY:{
					ScriptActorPropertyPtr para = new ScriptActorProperty();
					curScript = para;
					break;// = 0x73;//人物五围和测试
				}
				case CMD_SWITCH_SAVEFILE:{
					ScriptSwitchSavefilePtr para = new ScriptSwitchSavefile();
					curScript = para;
					break;// = 0x74;//开/禁存档
				}
				case CMD_ASSIGN_SIMAGE:{
					ScriptAssignSImagePtr para = new ScriptAssignSImage();
					curScript = para;
					break;// = 0x75;//S特殊形象指定
				}
				case CMD_SKIP_UNCONDITION:{
					ScriptSkipUnconditionPtr para = new ScriptSkipUncondition();
					curScript = para;
					break;// = 0x76;//无条件跳转
				}
				case CMD_VALUE_OPERATION:{
					ScriptValueOperationPtr para = new ScriptValueOperation();
					curScript = para;
					break;// = 0x77;//变量运算
				}
				case CMD_VALUE_INT_VALUATION:{
					ScriptValueIntValuationPtr para = new ScriptValueIntValuation();
					curScript = para;
					break;// = 0x78;//整形变量赋值
				}
				case CMD_TEST_VALUEPLUS:{
					ScriptTestValuePlusPtr para = new ScriptTestValuePlus();
					curScript = para;
					break;// = 0x79;//加强版变量测试
				}
				case CMD_MAX:{
					break;// = 0x6e;
				}
				default:
				{
					break;
				}
				}
				if(curScript != NULL)
				{
					index = curScript->initPara(SceneData, index);
					if (bNormal)
					{
						if(CMD_PK_START == curScript->cmd)
						{
							ScriptPKStartPtr para = (ScriptPKStartPtr)curScript;
						}
					}
					else
					{
						if(curScript->cmd == CMD_BG_SET)
						{
							int iSIndex = ((ScriptBGSetPtr)curScript)->zhanchang_id;
							if (iVar4071 == 123)
							{
								iSIndex += 100;
							}
							m_iBGIndex = iSIndex + 1;
						}
						else if(curScript->cmd == CMD_VALUE_OPERATION)
						{
							ScriptValueOperationPtr ptrScript = (ScriptValueOperationPtr)curScript;
							if(ptrScript->value_type1 == 2 && ptrScript->operation_type == 2)
							{
								if (ptrScript->num1 == 4071)
								{
									iVar4071 = ptrScript->getNum2();
								}
								else if(ptrScript->num1 == 5004)
								{
									iVar5004 = ptrScript->getNum2();
								}
								else if(ptrScript->num1 == 5006)
								{
									iVar5006 = ptrScript->getNum2();
								}
							}
						}
						else if(curScript->cmd == CMD_DISPATCH_OWN)
						{
							bool bSecond = false; //是否存在两组我军位置设定
							ScriptDispatchOwnPtr ptrScript = (ScriptDispatchOwnPtr)curScript;
							if(iVar5004 == 1 && iVar5006 > 0)
							{
								if(iVar5006 > 2)
								{
									iVar5006 = 2;
								}
								ptrScript->out_index += iVar5006 * MAX_FORCE_LIMIT;
							}

							for(int i = 0; i < pre_battle_list->size(); i++)
							{
								ScriptDispatchOwnPtr para = (ScriptDispatchOwnPtr)pre_battle_list->elementAt(i);
								if(para->out_index == ptrScript->out_index)
								{
									bSecond = true;
									break;
								}
							}
							if(bSecond)
							{
								pre_battle_list->removeAllElements();
								SAFE_DELETE(curScript);
								SAFE_DELETE(curSuite);
								SAFE_DELETE_ARRAY(ptrsec_index);
								return;
							}
							else
							{
								pre_battle_list->addElement(curScript);
							}
						}
					}
				}
				else
				{
					curScript = new Script(curCmd);
				}
				if(in_child_section != 0 && curScript->cmd != CMD_CHILD_ACTION)
				{
					index += SHORT_NUM_LEN;
					in_child_section = 0;
				}
				if (bNormal)
				{
					curScript->setSubScript(subScript);
					curSuite->addElement(curScript);
				}
				else if(CMD_DISPATCH_OWN != curScript->cmd)
				{
					SAFE_DELETE(curScript);
				}
				cursec_index++;
			}while(finish_flag);
			ptrsec_index[j + 1] = cursec_index;
		}

		if (bNormal)
		{
			script_lists->addElement(curSuite);
		}
		else
		{
			SAFE_DELETE(curSuite);
		}
		SAFE_DELETE_ARRAY(ptrsec_index);
	}
	return;
}

void ScriptHandler::SkipToLastOrder()
{
	if (cur_list)
	{
		script_index = cur_list->size() - 1;
	}
}
void ScriptHandler::SkipScript(int index)
{
	if (cur_list)
	{
		for (int i = 0; i < cur_list->size(); i++)
		{
			if (cur_list->elementAt(i)->subScript == index)
			{
				script_index = i - 1;
				break;
			}
		}
	}
}

bool ScriptHandler::GetDispatchInfo(string SFileNameS)
{
	char filename[40];

	sprintf(filename, "%s%s%s", "Scenes/", SFileNameS.c_str(), ".eex_new");
	char* game_data = ChessReadFile(filename);

	if(game_data)
	{
		DecodeScenes(game_data, false);
		ChessFree(game_data);
	}
	return pre_battle_list->size() > 0 ? true : false;
}

void ScriptHandler::addScript(Script* ptrScript)
{
	cur_list->addElement(ptrScript);
}

void ScriptHandler::removeScript(Script* ptrScript)
{
	cur_list->removeElement(ptrScript);
}
