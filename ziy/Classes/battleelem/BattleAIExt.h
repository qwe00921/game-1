#ifndef _BattleAIExt_H_
#define _BattleAIExt_H_

ActionSingleGainPtr CalcSkillSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, SkillAttrPtr skill, ActionGainPtr totalgain);
ActionSingleGainPtr CalcGoodSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, GoodsItem* good,ActionGainPtr totalgain);
ActionSingleGainPtr CalcAttackSingleGain(HistoryActor* cur_actor, HistoryActor* oppo_actor, ActionGainPtr totalgain, bool isVisible = true, int addprobability = 0,int iPierceNum = 0);
int GetJobPKHartnum(int occutype, int oppo_occutype, bool isSkill); //获取职业相克的伤害系数
void setLossBuff(ActionSingleGain* ptrGain, float fPercent); //计算损益BUFF

#endif