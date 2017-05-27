#include "Common.h"
#include "Scripts.h"


int ScriptBattleActorAction::HandleScript()
{

	if(frame == 0&&delay1)
	{
		need_frame = delay1 + 1;
	}
	if(frame == delay1)
	{
		BattleActorPtr actor = NULL;
		if (actor_id == 1027)//我方当前人物
		{
			actor = BattleControler::getBattleActor(history->cur_actor->actor_id);

		}
		else if (actor_id < -1)//变量武将
		{
			actor = BattleControler::getBattleActor(history->var2[(int)abs(actor_id) - 2]);
		}
		else
		{
		    actor = BattleControler::getBattleActor(actor_id);
		}

		if (actor && actor->hisActor->is_leave == 0 && actor->hisActor->GetCurHp() > 0 && actor->hisActor->hide != 1)//角色不隐藏
		{
			need_frame = delay1 + actor->setState(action) + delay2;
			if(m_iShareId > -1 && BattleControler::getBattleActor(m_iShareId))
			{
				actor = BattleControler::getBattleActor(m_iShareId);
			}

			if(hurt_hp)
				actor->hurt_hp = hurt_hp;
			if(hurt_mp)
				actor->hurt_mp = hurt_mp;
			if (hurt_money)
				actor->hurt_money = hurt_money;

			actor->m_bIsDeally = m_bIsDeally;
		}
		if(need_frame ==0)
			need_frame = 1;
	}
	return need_frame - frame++;
}

int ScriptBattleActorAction::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	action = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay1 = bytesToShort(SceneData, index)*TIMER_DELAY_FRAME;
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	delay2 = bytesToShort(SceneData, index)*TIMER_DELAY_FRAME;
	index += SHORT_NUM_LEN;
	return index;
}

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
