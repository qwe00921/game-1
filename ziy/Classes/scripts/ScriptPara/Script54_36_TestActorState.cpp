#include "Common.h"
#include "Scripts.h"
// 0攻击力1防御力2精神力3爆发力4士气5HP6MP7当前HP 8当前MP 9 等级 10 武力 11统帅 12智力 13 敏捷 14运气 15 头像
void ScriptTestActorState::TestOneActorState(HistoryActorPtr actor)
{
	switch (which_state) {
	case 0:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurpower() >= num)
			|| (relation == 1 && actor->GetCurpower() < num)
			|| (relation == 2 && actor->GetCurpower() == num))) { //未加状态和物品
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 1:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurDefence() >= num)
			|| (relation == 1 && actor->GetCurDefence() < num)
			|| (relation == 2 && actor->GetCurDefence() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 2:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurIntellegence() >= num)
			|| (relation == 1 && actor->GetCurIntellegence() < num)
			|| (relation == 2 && actor->GetCurIntellegence() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 3:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurExplosion() >= num)
			|| (relation == 1 && actor->GetCurExplosion() < num)
			|| (relation == 2 && actor->GetCurExplosion() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 4:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurFortune() >= num)
			|| (relation == 1 && actor->GetCurFortune() < num)
			|| (relation == 2 && actor->GetCurFortune() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 5:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseHp() >= num) //此处为剧本判断用 只判断人物的血量 没有加装备加成
			|| (relation == 1 && actor->GetBaseHp() < num)
			|| (relation == 2 && actor->GetBaseHp() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 6:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseMp() >= num)
			|| (relation == 1 && actor->GetBaseMp() < num)
			|| (relation == 2 && actor->GetBaseMp() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 7:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurHp() >= num)
			|| (relation == 1 && actor->GetCurHp() < num&&actor->GetCurHp()>0)
			|| (relation == 2 && actor->GetCurHp() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 8:
		if (actor == NULL
			|| !((relation == 0 && actor->GetCurMp() >= num)
			|| (relation == 1 && actor->GetCurMp() < num&&actor->GetCurMp()>0)
			|| (relation == 2 && actor->GetCurMp() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 9:
		if (actor == NULL
			|| !((relation == 0 && actor->GetActorLv() >= num)
			|| (relation == 1 && actor->GetActorLv() < num)
			|| (relation == 2 && actor->GetActorLv() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 10:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBasepower() >= num)
			|| (relation == 1 && actor->GetBasepower() < num)
			|| (relation == 2 && actor->GetBasepower() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 11:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseDefence() >= num)
			|| (relation == 1 && actor->GetBaseDefence() < num)
			|| (relation == 2 && actor->GetBaseDefence() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 12:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseIntellegence() >= num)
			|| (relation == 1 && actor->GetBaseIntellegence() < num)
			|| (relation == 2 && actor->GetBaseIntellegence() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 13:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseExplosion() >= num)
			|| (relation == 1 && actor->GetBaseExplosion() < num)
			|| (relation == 2 && actor->GetBaseExplosion() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 14:
		if (actor == NULL
			|| !((relation == 0 && actor->GetBaseFortune() >= num)
			|| (relation == 1 && actor->GetBaseFortune() < num)
			|| (relation == 2 && actor->GetBaseFortune() == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	case 15:
		if (actor == NULL
			|| !((relation == 0 && actor->face_id >= num)
			|| (relation == 1 && actor->face_id < num)
			|| (relation == 2 && actor->face_id == num))) {
			test_result = false;
		}
		else
		{
			test_result = true;
		}
		break;
	}
}
int ScriptTestActorState::HandleScript() {
	if (actor_id < -1)//变量武将
	{
		actor_id_replace = history->var2[(int)abs(actor_id) - 2];
	}
	else
	{
		actor_id_replace = actor_id;
	}
	test_result = true;
	if (actor_id_replace == 1024)//任何部队
	{
		for (int i = 0; i < history->battleOwnNum; i++) {

			TestOneActorState(history->battleOwn[i]);
			if (test_result)
			{
				break;
			}
		}
		if (!test_result)
		{
			for (int i = 0; i < history->battleFriendNum; i++) {
				TestOneActorState(history->battleFriend[i]);
				if (test_result)
				{
					break;
				}
			}
			if (!test_result)
			{
				for (int i = 0; i < history->battleEnemyNum; i++) {
					TestOneActorState(history->battleEnemy[i]);
					if (test_result)
					{
						break;
					}
				}
			}
		}
	}
	else if (actor_id_replace == 1025)//我军或友军
	{
		for (int i = 0; i < history->battleOwnNum; i++) {

			TestOneActorState(history->battleOwn[i]);
			if (test_result)
			{
				break;
			}
		}
		if (!test_result)
		{
			for (int i = 0; i < history->battleFriendNum; i++) {
				TestOneActorState(history->battleFriend[i]);
				if (test_result)
				{
					break;
				}
			}
		}
	}
	else if (actor_id_replace == 1026)//敌军
	{
		for (int i = 0; i < history->battleEnemyNum; i++) {

			TestOneActorState(history->battleEnemy[i]);
			if (test_result)
			{
				break;
			}
		}
	}
	else if (actor_id_replace == 1027)//我军当前人物
	{
		TestOneActorState(history->cur_actor);
	}
	else if (actor_id_replace == -1)//无
	{
	}
	else//正常人物
	{
		HistoryActorPtr actor = NULL;
		actor = &history->actors[actor_id_replace];
		if(actor == NULL)
			return 0;
		TestOneActorState(actor);
	}

	if (!get_test_result()) {
		return -1;
	}
	return 0;
}

int ScriptTestActorState::initPara(char* SceneData, int index) {
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_state = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	relation = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
