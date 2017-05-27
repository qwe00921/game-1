#include "Common.h"
#include "Scripts.h"
#define CCC(A) history->var2[num] = A;break;
#define DDD(A) A = history->var2[num];break
void  ScriptValueIntValuation::valuation1()
{
	switch (attr_id)
	{
	case 0:  //R形象
		history->var2[num] = history->actors[actor_id_replace].image_id + 1;
		break;
	case 1:  //头像
		history->var2[num] = history->actors[actor_id_replace].face_id + 1;
		break;
	case 2:CCC(history->actors[actor_id_replace].GetCurpower());//攻击
	case 3:CCC(history->actors[actor_id_replace].GetCurDefence());//防御
	case 4:CCC(history->actors[actor_id_replace].GetCurIntellegence());//精神
	case 5:CCC(history->actors[actor_id_replace].GetCurExplosion());//爆发
	case 6:CCC(history->actors[actor_id_replace].GetCurFortune());//士气
	case 7:CCC(history->actors[actor_id_replace].GetBaseHp());//HP
	case 8:CCC(history->actors[actor_id_replace].GetBaseMp());//MP
	case 9:CCC(history->actors[actor_id_replace].GetBasepower());//武力
	case 10:CCC(history->actors[actor_id_replace].GetBaseDefence());//统帅
	case 11:CCC(history->actors[actor_id_replace].GetBaseIntellegence());//智力
	case 12:CCC(history->actors[actor_id_replace].GetBaseExplosion());//敏捷
	case 13:CCC(history->actors[actor_id_replace].GetBaseFortune());//运气
	case 14:CCC(history->actors[actor_id_replace].battleTimes);//出战场数
	case 15:CCC(history->actors[actor_id_replace].retreatTimes);//撤退场数
	case 16:CCC(history->actors[actor_id_replace].relation);//我军标识
	case 17:CCC(history->actors[actor_id_replace].occupation_id);//兵种ID
	case 18:CCC(history->actors[actor_id_replace].GetActorLv());//人物等级
	case 19:CCC(history->actors[actor_id_replace].exp);//人物经验值
	case 20:CCC(history->actors[actor_id_replace].weapon.good_id);//武器ID
	case 21:CCC(history->actors[actor_id_replace].weapon.level);//武器等级
	case 22:CCC(history->actors[actor_id_replace].weapon.exp);//武器经验值
	case 23:CCC(history->actors[actor_id_replace].coat.good_id);//防具ID
	case 24:CCC(history->actors[actor_id_replace].coat.level);//防具等级
	case 25:CCC(history->actors[actor_id_replace].coat.exp);//防具经验值
	case 26:CCC(history->actors[actor_id_replace].assist_good.good_id);//辅助道具ID
	case 28://战场编号
	{
		if (history->var2[4080] == 1)//对出场人物id特殊处理
		{
			if (actor_id_replace < history->battleOwnNum)
			{
				history->var2[num] = history->battleOwn[actor_id_replace]->actor_id;
			}
		}
		else
		{
			for (int i = 0; i < history->battleOwnNum; i++)
			{
				if (history->battleOwn[i]->actor_id == actor_id_replace)
				{
					history->var2[num] = i;
					break;
				}
			}
		}
		break;
	}
	case 29:CCC(history->actors[actor_id_replace].y);//战场纵坐标
	case 30:CCC(history->actors[actor_id_replace].x);//战场横坐标
	case 31:CCC(history->actors[actor_id_replace].action_finished);//战场行动标识
	case 32:CCC(history->actors[actor_id_replace].direction);//战场人物朝向
	case 33:CCC(history->actors[actor_id_replace].GetCurHp());//HPCur
	case 34:CCC(history->actors[actor_id_replace].GetCurMp());//MPCur
	case 35:CCC(history->actors[actor_id_replace].state_power);//战场人物攻击状态
	case 36:CCC(history->actors[actor_id_replace].state_defence);//战场人物防御状态
	case 37:CCC(history->actors[actor_id_replace].state_intellegence);//战场人物精神状态
	case 38:CCC(history->actors[actor_id_replace].state_explosion);//战场人物爆发状态
	case 39:CCC(history->actors[actor_id_replace].state_fortune);//战场人物士气状态
	case 40:CCC(history->actors[actor_id_replace].state_speed);//战场人物移动状态
		//case 41:CCC(history->actors[actor_id_replace]);//战场人物健康状态
	default:
		break;
	}
}

void  ScriptValueIntValuation::valuation2()
{
	HistoryActorPtr actor = &history->actors[actor_id_replace];

	switch (attr_id)
	{
	case 0:
		history->actors[actor_id_replace].image_id = history->var2[num] - 1;
		history->actors[actor_id_replace].force_save = 1;
		history->LoadBattleActorImage(actor_id_replace);
		break;
	case 1:
		history->actors[actor_id_replace].face_id = history->var2[num] - 1;
		history->LoadBattleActorImage(actor_id_replace);
		break;
	case 2:history->actors[actor_id_replace].SetCurpower(history->var2[num]); break;
	case 3:history->actors[actor_id_replace].SetCurDefence(history->var2[num]); break;
	case 4:history->actors[actor_id_replace].SetCurIntellegence(history->var2[num]); break;
	case 5:history->actors[actor_id_replace].SetCurExplosion(history->var2[num]); break;
	case 6:history->actors[actor_id_replace].SetCurFortune(history->var2[num]); break;
	case 7:history->actors[actor_id_replace].SetBaseHp(history->var2[num]); break;
	case 8:history->actors[actor_id_replace].SetBaseMp(history->var2[num]); break;
	case 9: history->actors[actor_id_replace].SetBasepower(history->var2[num]); break;
	case 10:history->actors[actor_id_replace].SetBaseDefence(history->var2[num]); break;
	case 11:history->actors[actor_id_replace].SetBaseIntellegence(history->var2[num]); break;
	case 12:history->actors[actor_id_replace].SetBaseExplosion(history->var2[num]); break;
	case 13:history->actors[actor_id_replace].SetBaseFortune(history->var2[num]); break;
	case 14:DDD(history->actors[actor_id_replace].battleTimes);
	case 15:DDD(history->actors[actor_id_replace].retreatTimes);
	case 16:DDD(history->actors[actor_id_replace].relation);
	case 17:
	{
		if (!actor->enable_set_occupation_id)
			actor->occupation_id = history->var2[num];
		actor->history_occupation_id = actor->GetActorInitialJob();
		if (history->var2[4081])//重新计算人物五维
		{
			actor->InitFiveBaseAbility();
		}
	}
		break;
	case 18:history->actors[actor_id_replace].SetActorLv(history->var2[num]); break;
	case 19:DDD(history->actors[actor_id_replace].exp);
	case 20:
	{
		actor->weapon.good_id = history->var2[num];
		actor->weapon.init(actor->weapon.good_id, actor->weapon.level);
		actor->UpdataGoodEffect();
	}
		break;
	case 21:
	{
		actor->weapon.level = history->var2[num];
		actor->weapon.init(actor->weapon.good_id, actor->weapon.level);
		actor->UpdataGoodEffect();
	}
		break;
	case 22:DDD(history->actors[actor_id_replace].weapon.exp);
	case 23:
	{
		actor->coat.good_id = history->var2[num];
		actor->coat.init(actor->coat.good_id, actor->coat.level);
		actor->UpdataGoodEffect();
	}
		break;
	case 24:
	{
		actor->coat.level = history->var2[num];
		actor->coat.init(actor->coat.good_id, actor->coat.level);
		actor->UpdataGoodEffect();
	}
		break;
	case 25:DDD(history->actors[actor_id_replace].coat.exp);
	case 26:
	{
		actor->assist_good.good_id = history->var2[num];
		actor->assist_good.init(actor->assist_good.good_id, 1);
		actor->UpdataGoodEffect();
	}
		break;
		//case 27:DDD(history->actors[actor_id_replace]);
		//case 28:DDD(history->actors[actor_id_replace]);
	case 29:DDD(history->actors[actor_id_replace].y);
	case 30:DDD(history->actors[actor_id_replace].x);
	case 31:DDD(history->actors[actor_id_replace].action_finished);
	case 32:
	{
			   //direction = 7代表自由控制友军
			   if (history->var2[num] == 7)
			   {
				   int i;
				   for (i = 0; i < history->battleFriendNum; i++) {
					   if (history->battleFriend[i]->actor_id == actor_id_replace)
					   {
						   history->battleOwn[history->battleOwnNum] = history->battleFriend[i];
						   history->battleOwn[history->battleOwnNum]->relation = 0;
						   history->battleOwn[history->battleOwnNum]->ai_type = 1;//修改为主动出击
						   history->battleOwnNum++;
						   for (int j = i; j < history->battleFriendNum - 1; j++)
						   {
							   history->battleFriend[j] = history->battleFriend[j + 1];
						   }
						   history->battleFriendNum--;
						   break;
					   }
				   }
			   }else if (history->var2[num] == 8)//代表变成友军
			   {
				   int i;
				   for (i = 0; i < history->battleOwnNum; i++) {
					   if (history->battleOwn[i]->actor_id == actor_id_replace)
					   {
						   history->battleFriend[history->battleFriendNum] = history->battleOwn[i];
						   history->battleFriend[history->battleFriendNum]->relation = ACTOR_RELATION_FRIEND;
						   history->battleFriend[history->battleFriendNum]->ai_type = 2;//修改坚守原地
						   history->battleFriendNum++;
						   for (int j = i; j < history->battleOwnNum - 1; j++)
						   {
							   history->battleOwn[j] = history->battleOwn[j + 1];
						   }
						   history->battleOwnNum--;
						   break;
					   }
				   }
			   }
			   else
			   {
				   history->actors[actor_id_replace].direction = history->var2[num];
			   }
			   break;
	}
	case 33:history->actors[actor_id_replace].SetCurHp(history->var2[num]); break;
	case 34:history->actors[actor_id_replace].SetCurMp(history->var2[num]); break;
	case 35:DDD(history->actors[actor_id_replace].state_power);
	case 36:DDD(history->actors[actor_id_replace].state_defence);
	case 37:DDD(history->actors[actor_id_replace].state_intellegence);
	case 38:DDD(history->actors[actor_id_replace].state_explosion);
	case 39:DDD(history->actors[actor_id_replace].state_fortune);
	case 40:DDD(history->actors[actor_id_replace].state_speed);
		//case 41:DDD(history->actors[actor_id_replace]);
	default:
		break;
	}
}
int ScriptValueIntValuation::HandleScript() {
	if (actor_id < -1)//变量武将
	{
		actor_id_replace = history->var2[(int)abs(actor_id) - 2];
	}
	else
	{
		actor_id_replace = actor_id;
	}
	if (actor_id == -1 && operation_type == 0 && attr_id == 16 && history->var2[4072]>10000)
	{
		int iGoodId = history->var2[4072] - 10001;
		history->var2[num] = history->getGoodsNum(iGoodId);
		return 0;
	}
	if (operation_type == 0)
	{
		valuation1();
	}
	else
	{
		valuation2();
	}
	return 0;
}

int ScriptValueIntValuation::initPara(char*SceneData, int index)
{
	unkown1 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	num = bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown2 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	operation_type = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown4 = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	attr_id = bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	return index;
}
