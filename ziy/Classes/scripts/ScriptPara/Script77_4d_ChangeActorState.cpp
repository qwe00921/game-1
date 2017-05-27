#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"
ScriptChangeActorState::ScriptChangeActorState(short actor_id,short unnormal_state)
{
	this->unnormal_state = unnormal_state;
	this->actor_id = actor_id;
	this->change_item = -2;
	this->change_type = -1;
	this->which_index = 0;
	this->iSkillMiss = 0;
	this->coat_exp_change = 0;
	this->weapon_exp_change = 0;
	this->hp_change = 0;
	this->mp_change = 0;
	this->show_change = 0;
	this->bIsScript = false;
	this->cmd = CMD_CHANGE_ACTOR_STATE;
}
// 0攻击力1防御力2精神力3爆发力4士气5移动力 
// 0下降1正常2上升;
ScriptChangeActorState::ScriptChangeActorState(short actor_id,int change_item,int change_type)
{
	this->change_item = change_item;
	this->change_type = change_type;
	this->which_index = 0;
	this->actor_id = actor_id;
	this->iSkillMiss = 0;
	this->coat_exp_change = 0;
	this->weapon_exp_change = 0;
	this->unnormal_state = 0;
	this->hp_change = 0;
	this->mp_change = 0;
	this->show_change = 0;
	this->bIsScript = false;
	this->cmd = CMD_CHANGE_ACTOR_STATE;
}
ScriptChangeActorState::ScriptChangeActorState(short actor_id, int hp_change, int mp_change, int iSkillMiss, short coat_exp_change, short weapon_exp_change, short unnormal_state)
{
	this->which_index = 0;
	this->actor_id = actor_id;
	this->iSkillMiss = iSkillMiss;
	this->coat_exp_change = coat_exp_change;
	this->weapon_exp_change = weapon_exp_change;
	this->unnormal_state = unnormal_state;
	this->hp_change = hp_change;
	this->mp_change = mp_change;
	this->show_change = 1;
	this->change_item = -2;
	this->bIsScript = false;
	this->cmd = CMD_CHANGE_ACTOR_STATE;
}
// 0我军，1友军，2敌军，3援军，4我军及友军，5敌军及援军，6所有部队
void ScriptChangeActorState::getActorList()
{
	int i,j;
	HistoryActorPtr actor = NULL; 
	for(i = left_top_y;i<=right_bottom_y;i++)
	{
		for(j = left_top_x;j<=right_bottom_x;j++)
		{
			actor = history->FindActorPosition(j,i);
			if(actor!=NULL)
			{
				switch(relation)
				{
				case 0:
					if(actor->relation == 0)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 1:
					if(actor->relation == 1)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 2:
					if(actor->relation == 2)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 3:
					break;
				case 4:
					if(actor->relation == 0||actor->relation == 1)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 5:
					if(actor->relation == 2)
					{
						actor_id_list[actor_num++] = actor->actor_id;
					}
					break;
				case 6:
					actor_id_list[actor_num++] = actor->actor_id;
					break;
				}
			}
		}
	}
}
int ScriptChangeActorState::HandleScript() {
	HistoryActorPtr actor = NULL;
	BattleScreenPtr scr = (BattleScreenPtr)chess->getScreenByID(SCREEN_BATTLE);
	if(frame == 0)
	{
		actor_index = 0;
		switch (which_index) {
		case 0: // 武将id;
			{
				actor_num = 1;
				actor_id_list[0] = actor_id;
			}
			break;
		case 1: // 出场顺序;
			{
				if (history->battleOwn[out_index] != NULL)
				{
					actor_num = 1;
					actor_id_list[0] = history->battleOwn[out_index]->actor_id;
				}
				else
				{
					actor_num = 0;
				}
		}
			break;
		case 2: // 区域
			{
				actor_num = 0;
				getActorList();
			}
			break;
		}
		need_frame = 7*actor_num-1;
		//TODO 区域没有人的情况
		if (actor_num == 0)
		{
			need_frame = 0;
		}
	}
	if(frame%7 ==0&&actor_num)
	{
		hp_change_replace = hp_change;
		mp_change_replace = mp_change;
		actor = &history->actors[actor_id_list[actor_index]];
		if (history->var2[4044] == 1)//特殊变量特殊处理
		{
			hp_change_replace = -(int)abs(hp_change_replace);
			mp_change_replace = -(int)abs(mp_change_replace);
		}
		if (actor->hide != 1 && !actor->is_leave)
		{
			if (scr != NULL&&show_change)
			{
				if (hp_change_replace || mp_change_replace)
				{
					if (hp_change_replace != 0 && hp_change_replace > actor->GetCurHp())
					{
						hp_change_replace = actor->GetCurHp();
					}
					if (-hp_change_replace + actor->GetCurHp() > actor->GetBaseHp() + actor->GetGoodsAddHp())
						hp_change_replace = -(actor->GetBaseHp() + actor->GetGoodsAddHp() - actor->GetCurHp());

					if (mp_change_replace != 0 && mp_change_replace > actor->GetCurMp())
					{
						mp_change_replace = actor->GetCurMp();
					}
					if (-mp_change_replace + actor->GetCurMp() > actor->GetBaseMp() + actor->GetGoodsAddMp())
						mp_change_replace = -(actor->GetBaseMp() + actor->GetGoodsAddMp() - actor->GetCurMp());
					history->CheckPosActor(actor);
					scr->m_spActGain->updateSprite(actor, -hp_change_replace, -mp_change_replace);
				}
				else if(iSkillMiss)
				{
					BattleControler::getBattleActor(actor->actor_id)->m_bSkillMiss = true;
				}
			}
			else
				frame++;
		}
		else
		{
			frame++;
		}

	}
	if(frame%7 == 1&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		if(actor->hide != 1 && !actor->is_leave)
		{
			if (hp_change_replace != 0)
			{
				if (-hp_change_replace + actor->GetCurHp() > actor->GetBaseHp() + actor->GetGoodsAddHp())
					hp_change_replace = -(actor->GetBaseHp() + actor->GetGoodsAddHp() - actor->GetCurHp());

				if (hp_change_replace >= actor->GetCurHp())
				{
					if (show_change)
					{
						actor->SetCurHp(0);
					}
					else 
					{
						if (actor->GetCurHp() > 0)
						{
							actor->SetCurHp(1);
						}
					}

				}
				else
				{
					actor->SetCurHp(-1, -hp_change_replace);
				}
				
			}
				
			if (mp_change_replace != 0)
			{
				if (-mp_change_replace + actor->GetCurMp() > actor->GetBaseMp() + actor->GetGoodsAddMp())
					mp_change_replace = -(actor->GetBaseMp() + actor->GetGoodsAddMp() - actor->GetCurMp());
				if (mp_change_replace >= actor->GetCurMp())
				{
					actor->SetCurMp(0);
				}
				else
				{
					actor->SetCurMp(-1, -mp_change_replace);
				}
			}
				
			int state_id = -1;

			int iBuffId = 0;
			int iBuffState = change_type - 1;
			switch (change_item) 
			{
			case ACTOR_ABILITY_POWER:
				iBuffId = ACTOR_BUFF_POWUP;
				break;
			case ACTOR_ABILITY_DEFENCE:
				iBuffId = ACTOR_BUFF_DEFUP;
				break;
			case ACTOR_ABILITY_INTELLEGENCE:
				iBuffId = ACTOR_BUFF_INTUP;
				break;
			case ACTOR_ABILITY_EXPLOSION:
				iBuffId = ACTOR_BUFF_EXPUP;
				break;
			case ACTOR_ABILITY_FORTUNE:
				iBuffId = ACTOR_BUFF_FORTUP;
				break;
			case ACTOR_ABILITY_SPEED:
				iBuffId = ACTOR_BUFF_SPEEDUP;
				break;
			default:
				break;
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, true);
				if(iBuffState < 0 && actor->m_mActorBuff.count(iBuffId + 1) && actor->m_mActorBuff[iBuffId + 1]->iCurRound) //原有上升BUFF则不+1
				{
					iBuffId += 1;
				}
				else if(iBuffState > 0 && !(actor->m_mActorBuff.count(iBuffId) && actor->m_mActorBuff[iBuffId]->iCurRound)) //原有下降BUFF则不+1
				{
					iBuffId += 1;
				}
				scr->addArmBuff(actor, iBuffId);
			}
			else
			{
				frame++;
			}
		}
		else
		{
			frame++;
		}
	}
	if(frame%7 == 2&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		int iBuffId = 0;
		int iBuffState = 0;
		if(unnormal_state != -1)
		{
			iBuffState = unnormal_state & UNNORMAL_STATE_CLEAR ? 1 : -1;
		}
		if(actor->hide != 1 && !actor->is_leave)
		{
			if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_PUZZLE))
			{
				iBuffId = ACTOR_BUFF_PUZZLE;
			}
			else if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_OBFUSCATION))
			{
				iBuffId = ACTOR_BUFF_OBFU;
			}
            else if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_BLIND))
            {
				iBuffId = ACTOR_BUFF_BLIND;
            }
			else if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_REDUCE_SKILLHURT))
			{
				iBuffId = ACTOR_BUFF_HURTDW;
				if(iBuffState > 0)
				{
					actor->reduce_skill_hurt_buff->removeAllElements();
				}
			}
			else if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_SKILLHURTANDREBOUND))
			{
				iBuffId = ACTOR_BUFF_FIRESHIELD;
			}
			else if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_ADD_SKILLHURT))
			{
				iBuffId = ACTOR_BUFF_HURTUP;
				if (iBuffState > 0)
				{
					actor->add_skill_hurt_buff->removeAllElements();
				}
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, false);
				scr->addArmBuff(actor, iBuffId);
			}
			else
			{
				frame++;
			}
		}
		else
		{
			frame++;
		}
	}
	if(frame%7 == 3&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		int iBuffId = 0;
		int iBuffState = 0;
		if(unnormal_state != -1)
		{
			iBuffState = unnormal_state & UNNORMAL_STATE_CLEAR ? 1 : -1;
		}
		if(actor->hide != 1 && !actor->is_leave)
		{
			if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_POISON))
			{
				iBuffId = ACTOR_BUFF_POISON;
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, false);
				scr->addArmBuff(actor, iBuffId);
			}
			else
			{
				frame++;
			}
		}
		else
		{
			frame++;
		}
	}
	if(frame%7 == 4&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		int iBuffId = 0;
		int iBuffState = 0;
		if(unnormal_state != -1)
		{
			iBuffState = unnormal_state & UNNORMAL_STATE_CLEAR ? 1 : -1;
		}
		if(actor->hide != 1 && !actor->is_leave)
		{
			if (unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_PALSY))
			{
				iBuffId = ACTOR_BUFF_PALSY;
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, false);
				scr->addArmBuff(actor, iBuffId);
			}
			else
			{
				frame++;
			}
		}
		else
		{
			frame++;
		}
	}
	if(frame%7 == 5&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		int iBuffId = 0;
		int iBuffState = 0;
		if(unnormal_state != -1)
		{
			iBuffState = unnormal_state & UNNORMAL_STATE_CLEAR ? 1 : -1;
		}
		if(actor->hide != 1 && !actor->is_leave)
		{
			if(unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_STUN))
			{
				iBuffId = ACTOR_BUFF_STUN;
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, false);
				scr->addArmBuff(actor, iBuffId);
			}
			else
			{
				frame++;
			}
		}
		else
		{
			frame++;
		}
	}
	if(frame%7 == 6&&actor_num)
	{
		actor = &history->actors[actor_id_list[actor_index]];
		int iBuffId = 0;
		int iBuffState = 0;
		if(unnormal_state != -1)
		{
			iBuffState = unnormal_state & UNNORMAL_STATE_CLEAR ? 1 : -1;
		}
		if(actor->hide != 1 && !actor->is_leave)
		{
			if(unnormal_state != -1 && (unnormal_state&UNNORMAL_STATE_WEAKED))
			{
				iBuffId = ACTOR_BUFF_WEAK;
			}

			if(iBuffId)
			{
				actor->updateActorBuff(iBuffId, iBuffState, false);
				scr->addArmBuff(actor, iBuffId);
			}
		}

		actor_index++;
	}

	return need_frame - frame++;
}

int ScriptChangeActorState::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_index =  bytesToShort(SceneData, index);//0 武将id 1 出场顺序 2 区域
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);//actor id
	index += SHORT_NUM_LEN;	
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_index =  bytesToInt(SceneData, index);//当which_index为出场顺序时的id序号
	index += INT_NUM_LEN;	
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	left_top_x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	left_top_y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	right_bottom_x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;	
	unkown7 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	right_bottom_y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown8 = bytesToShort(SceneData, index); 
	index += SHORT_NUM_LEN;
	relation =  bytesToShort(SceneData, index);// 0我军，1友军，2敌军，3援军，4我军及友军，5敌军及援军，6或者65535所有部队
	index += SHORT_NUM_LEN;	
	unkown9 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	change_item =  bytesToShort(SceneData, index);// 0 att攻击力 1 def防御力 2 spr精神力 3 cri爆发力 4 mor士气 5 speed移动力 6或者65535 无
    /* FIXME 矫正剧本传来的这个参数，因为目前无法修改剧本编辑器 */
    if(change_item == ACTOR_ABILITY_INTELLEGENCE)
        change_item = ACTOR_ABILITY_DEFENCE;
    else if (change_item == ACTOR_ABILITY_DEFENCE)
        change_item = ACTOR_ABILITY_INTELLEGENCE;

	index += SHORT_NUM_LEN;	
	unkown10 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	change_type =  bytesToShort(SceneData, index);//0 下降 1 正常 2 上升 65535 无
	index += SHORT_NUM_LEN;
	unkown11 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unnormal_state =  bytesToShort(SceneData, index);//2-30异常状态 130-158 取消对应异常状态 65535无
	//2-30 麻痹 封咒 麻痹+封咒 混乱 麻痹+混乱 封咒+混乱 麻痹+封咒+混乱 中毒 麻痹+中毒 封咒+中毒 麻痹+封咒+中毒 混乱+中毒 麻痹+混乱+中毒 封咒+混乱+中毒 麻痹+封咒+混乱+中毒
	//130-158 对应取消

	index += SHORT_NUM_LEN;
	unkown12 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	hp_change =  bytesToShort(SceneData, index);//hp
	index += INT_NUM_LEN;	
	unkown13 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	mp_change =  bytesToShort(SceneData, index);//mp
	index += INT_NUM_LEN;
	
	iSkillMiss = 0;
	coat_exp_change = 0;
	weapon_exp_change = 0;
	show_change = 0;
	return index;
}

