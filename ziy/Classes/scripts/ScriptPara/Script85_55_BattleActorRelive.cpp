#include "Common.h"
#include "Scripts.h"
#include "BattleScreen.h"

int ScriptBattleActorRelive::HandleScript()
{
	if (iIsTower)
	{
		if (iIsTower == 2)
		{
			if (!frame)
			{
				BattleActorPtr ptrActor = BattleControler::getBattleActor(901);
				if (ptrActor && ptrActor->hisActor != NULL && ptrActor->hisActor->hide != 1)
				{
					need_frame = ptrActor->setState(ACTOR_STATE_LEAVE_HIDE);
				}
				else
				{
					return 0;
				}
				ptrActor = BattleControler::getBattleActor(902);
				if (ptrActor && ptrActor->hisActor != NULL && ptrActor->hisActor->hide != 1)
				{
					need_frame = ptrActor->setState(ACTOR_STATE_LEAVE_HIDE);
				}
				else
				{
					return 0;
				}

				x = ptrActor->hisActor->x;
				y = ptrActor->hisActor->y;
			}
			else if (need_frame == frame)
			{
				AddTowerActor();
			}
		}
		else if (iIsTower == 3)
		{
			if (!frame)
			{
				BattleActorPtr ptrActor = BattleControler::getBattleActor(903);
				if (ptrActor && ptrActor->hisActor != NULL && ptrActor->hisActor->hide != 1)
				{
					need_frame = ptrActor->setState(ACTOR_STATE_LEAVE_HIDE);
				}
				else
				{
					return 0;
				}

				x = ptrActor->hisActor->x;
				y = ptrActor->hisActor->y;
			}
			else if (need_frame == frame)
			{
				DecomposeTowerActor();
			}
		}
		else
		{
			AddTowerActor();
		}
		return need_frame - frame++;
	}
	HistoryActorPtr actor = NULL;
	if(which_index <= 0)
	{
		if (history->var2[5007] == 1 && actor_id<-2 && history->var2[-actor_id - 2] != 5011)
			actor_id = history->var2[-actor_id - 2];
		if (actor_id > trData->actor_num || actor_id < 0)
		{
			return 0;
		}
		actor = &history->actors[actor_id];
	}
	else
	{
		actor = history->battleOwn[out_index];
	}
	if(actor!=NULL)
	{
		
		if (actor->hide == 1 || actor->style == ACTOR_STATE_LEAVE_DEAD || actor->style == ACTOR_STATE_LEAVE_NORMAL || actor->style == ACTOR_STATE_LEAVE_HIDE)
		{
			history->CenterActor(actor);
			actor->style = ACTOR_STATE_NORMAL;
			//history->FindActor(actor->actor_id)->style = ACTOR_STATE_NORMAL;
			
			actor->x = x;
			actor->y = y;
			history->FindNearestBlackPos(actor, actor->x, actor->y);
			actor->direction = direction;

			actor->InitAbility();
			actor->InitAbilityForRevive();
			int is_finished = actor->action_finished; //战场复活武将行动是否结束的状态不应该重置
			actor->recoverState();
			actor->action_finished = is_finished;
			history->CheckPosActor(actor);
		}
	}
	return 0;
}

int ScriptBattleActorRelive::initPara(char*SceneData,int index){
	unkown1 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	which_index =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown2 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	actor_id =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	unkown3 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	out_index =  bytesToInt(SceneData, index);
	index += INT_NUM_LEN;
	unkown4 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	x =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown5 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	y =  bytesToShort(SceneData, index);
	index += INT_NUM_LEN;
	unkown6 =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
	direction =  bytesToShort(SceneData, index);
	index += SHORT_NUM_LEN;
    actor_level = 0;
	return index;
}

void ScriptBattleActorRelive::AddTowerActor()
{
	if (actor_level <= 0)
		actor_level = 1;
	HistoryActorPtr actor = history->FindActor(actor_id);
	if (actor == NULL)
	{
		actor = &history->actors[actor_id];

		if (tCurActor->relation == 0)
		{
			history->battleOwn[history->battleOwnNum] = actor;
			history->battleOwnNum++;
		}
		else if (tCurActor->relation == 1)
		{
			history->battleFriend[history->battleOwnNum] = actor;
			history->battleFriendNum++;
		}
		else if (tCurActor->relation == 2)
		{
			history->battleEnemy[history->battleOwnNum] = actor;
			history->battleEnemyNum++;
		}

		BattleScreenPtr ptrBattleScr = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		ptrBattleScr->handler.actor_list->addElement(new BattleActor(actor));

		if (actor->history_image_fight_id < 400)
		{
			actor->history_image_fight_id = trData->occupation_image_id[actor->occupation_id];
		}
		history->LoadBattleActorImage(actor->actor_id);
	}
	else if(actor->relation != tCurActor->relation)
	{
		if (actor->relation == 0)
		{
			for (int i = 0; i < history->battleOwnNum; i++)
			{
				if (history->battleOwn[i]->actor_id == actor->actor_id)
				{
					for (int j = i; j < history->battleOwnNum - 1; j++)
					{
						history->battleOwn[j] = history->battleOwn[j + 1];
					}
					history->battleOwn[history->battleOwnNum - 1] = NULL;
					history->battleOwnNum--;
					break;
				}
			}
		}
		else if (tCurActor->relation == 1)
		{
			for (int i = 0; i < history->battleFriendNum; i++)
			{
				if (history->battleFriend[i]->actor_id == actor->actor_id)
				{
					for (int j = i; j < history->battleFriendNum - 1; j++)
					{
						history->battleFriend[j] = history->battleFriend[j + 1];
					}
					history->battleFriend[history->battleFriendNum - 1] = NULL;
					history->battleFriendNum--;
					break;
				}
			}
		}
		else if (tCurActor->relation == 2)
		{
			for (int i = 0; i < history->battleEnemyNum; i++)
			{
				if (history->battleEnemy[i]->actor_id == actor->actor_id)
				{
					for (int j = i; j < history->battleEnemyNum - 1; j++)
					{
						history->battleEnemy[j] = history->battleEnemy[j + 1];
					}
					history->battleEnemy[history->battleEnemyNum - 1] = NULL;
					history->battleEnemyNum--;
					break;
				}
			}
		}

		if (tCurActor->relation == 0)
		{
			history->battleOwn[history->battleOwnNum] = actor;
			history->battleOwnNum++;
		}
		else if (tCurActor->relation == 1)
		{
			history->battleFriend[history->battleOwnNum] = actor;
			history->battleFriendNum++;
		}
		else if (tCurActor->relation == 2)
		{
			history->battleEnemy[history->battleOwnNum] = actor;
			history->battleEnemyNum++;
		}
	}

	actor->style = ACTOR_STATE_NORMAL;

	actor->x = x;
	actor->y = y;

	actor->direction = direction;
	actor->relation = tCurActor->relation;
	actor->actor_quality = tCurActor->actor_quality;
	actor->CopyActorPracGrown(tCurActor);

	actor->SetActorLv(actor_level);
	actor->recoverState();
	actor->InitAbility();
	
	actor->action_finished = 0;
	actor->m_iControllerId = tCurActor->actor_id + 1;

	if (tCurActor->weapon.good_id > -1)
		actor->weapon.setGood(&tCurActor->weapon);
	else
		actor->weapon.good_id = -1;

	if (tCurActor->coat.good_id > -1)
		actor->coat.setGood(&tCurActor->coat);
	else
		actor->coat.good_id = -1;

	if (tCurActor->assist_good.good_id > -1)
		actor->assist_good.setGood(&tCurActor->assist_good);
	else
		actor->assist_good.good_id = -1;

// 	if (tCurActor->fashion.good_id > -1)
// 		actor->fashion.setGood(&tCurActor->fashion);
// 	else
// 		actor->fashion.good_id = -1;
// 
// 	if (tCurActor->wap_fashion.good_id > -1)
// 		actor->wap_fashion.setGood(&tCurActor->wap_fashion);
// 	else
// 		actor->wap_fashion.good_id = -1;

	actor->UpdataGoodEffect();
	actor->UpdateSkills(actor_level);

	if (iIsTower == 2)
	{
		int iCurHp = 0;
		int iCurMp = 0;

		BattleActorPtr ptrActor1 = BattleControler::getBattleActor(901);
		BattleActorPtr ptrActor2 = BattleControler::getBattleActor(902);

		int iCurNum1 = ptrActor1->hisActor->GetCurHp();
		int iCurNum2 = ptrActor2->hisActor->GetCurHp();
		int iActorHp = actor->GetBaseHp() + actor->GetGoodsAddHp();
		int iActorMp = actor->GetBaseMp() + actor->GetGoodsAddMp();
		if (iCurNum1 > iCurNum2)
		{
			if(iCurNum1<iActorHp)
				actor->SetCurHp(iCurNum1);
		}
		else
		{
			if (iCurNum2 < iActorHp)
				actor->SetCurHp(iCurNum2);
		}

		iCurNum1 = ptrActor1->hisActor->GetCurMp();
		iCurNum2 = ptrActor2->hisActor->GetCurMp();

		if (iCurNum1 > iCurNum2)
		{
			if (iCurNum1 < iActorHp)
				actor->SetCurMp(iCurNum1);
		}
		else
		{
			if (iCurNum2 < iActorHp)
				actor->SetCurMp(iCurNum2);
		}

		actor->action_finished = 1;
	}

	history->CheckPosActor(actor);
}

void ScriptBattleActorRelive::DecomposeTowerActor()
{
	if (actor_level <= 0)
		actor_level = 1;

	HistoryActorPtr actor1 = history->FindActor(901);
	HistoryActorPtr actor2 = history->FindActor(902);
	HistoryActorPtr actor3 = history->FindActor(903);

	actor1->style = ACTOR_STATE_NORMAL;
	actor2->style = ACTOR_STATE_NORMAL;

	actor2->x = x;
	actor2->y = y;

	actor1->x = tCurActor->x;
	actor1->y = tCurActor->y;

	actor1->direction = direction;
	actor2->direction = direction;
	actor1->relation = tCurActor->relation;
	actor2->relation = tCurActor->relation;
	actor1->actor_quality = tCurActor->actor_quality;
	actor2->actor_quality = tCurActor->actor_quality;
	actor1->SetActorLv(actor_level);
	actor2->SetActorLv(actor_level);
	actor1->CopyActorPracGrown(tCurActor);
	actor2->CopyActorPracGrown(tCurActor);

	actor1->InitAbility();
	actor1->recoverState();
	actor1->action_finished = actor3->action_finished;
	actor1->m_iControllerId = tCurActor->actor_id + 1;

	actor2->InitAbility();
	actor2->recoverState();
	actor2->action_finished = actor3->action_finished;
	actor2->m_iControllerId = tCurActor->actor_id + 1;

	unsigned short iOffX = x, iOffY = y;
	history->FindNearestBlackPosInit(actor1, iOffX, iOffY);
	actor1->x = iOffX;
	actor1->y = iOffY;

	if (tCurActor->weapon.good_id > -1)
	{
		actor1->weapon.setGood(&tCurActor->weapon);
		actor2->weapon.setGood(&tCurActor->weapon);
	}
	else
	{
		actor1->weapon.good_id = -1;
		actor2->weapon.good_id = -1;
	}

	if (tCurActor->coat.good_id > -1)
	{
		actor1->coat.setGood(&tCurActor->coat);
		actor2->coat.setGood(&tCurActor->coat);
	}
	else
	{
		actor1->coat.good_id = -1;
		actor2->coat.good_id = -1;
	}

	if (tCurActor->assist_good.good_id > -1)
	{
		actor1->assist_good.setGood(&tCurActor->assist_good);
		actor2->assist_good.setGood(&tCurActor->assist_good);
	}
	else
	{
		actor1->assist_good.good_id = -1;
		actor2->assist_good.good_id = -1;
	}

// 	if (tCurActor->fashion.good_id > -1)
// 	{
// 		actor1->fashion.setGood(&tCurActor->fashion);
// 		actor2->fashion.setGood(&tCurActor->fashion);
// 	}
// 	else
// 	{
// 		actor1->fashion.good_id = -1;
// 		actor2->fashion.good_id = -1;
// 	}
// 
// 	if (tCurActor->wap_fashion.good_id > -1)
// 	{
// 		actor1->wap_fashion.setGood(&tCurActor->wap_fashion);
// 		actor2->wap_fashion.setGood(&tCurActor->wap_fashion);
// 	}
// 	else
// 	{
// 		actor1->wap_fashion.good_id = -1;
// 		actor2->wap_fashion.good_id = -1;
// 	}

	actor1->UpdataGoodEffect();
	actor2->UpdataGoodEffect();
	actor1->UpdateSkills(actor_level);
	actor2->UpdateSkills(actor_level);

	int iCurHpPre = 100*actor3->GetCurHp()/(actor3->GetBaseHp()+actor3->GetGoodsAddHp());
	int iCurMpPre = 100 * actor3->GetCurMp() / (actor3->GetBaseMp() + actor3->GetGoodsAddMp());

	actor1->SetCurHp(iCurHpPre*(actor1->GetBaseHp() + actor1->GetGoodsAddHp()) / 100);
	actor1->SetCurMp(iCurMpPre*(actor1->GetBaseMp() + actor1->GetGoodsAddMp()) / 100);

	actor2->SetCurHp(iCurHpPre*(actor2->GetBaseHp() + actor2->GetGoodsAddHp()) / 100);
	actor2->SetCurMp(iCurMpPre*(actor2->GetBaseMp() + actor2->GetGoodsAddMp()) / 100);

	history->CheckPosActor(actor2);
}