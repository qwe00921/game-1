#include "Common.h"
#include "main.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "MiniMap.h"
#include "button.h"
#include "BattleRoundStart.h"
#include "MapElem.h"
#include "StatConfig.h"
#include "DestroySixCountriesLayer.h"
#include "WinConditionTool.h"
#include "BattleEndLayer.h"
#include "HeroicAmbitionLayer.h"
#include "NoticeActOpenLayer.h"
#include "ShortcutFunctionLayer.h"
#include "BSelectGoodsOrSkillLayer.h"
#include "BattleReplay.h"
#include "RomanceDetailsLayer.h"
#include "Net_HomePage.h"
#include "DailySceneLayer.h"
#include "LadderLayer.h"
#include "BattleLadder.h"
#include "WolfCityLayer.h"
#include "GetNetDateTool.h"
#include "GreatBattleLayer.h"
#include "PVPData.h"

static BattleControler* sBattleCtrl = NULL;
int BattleControler::auto_run = 0;

void BattleControler::ClearGain()
{
	if (sBattleCtrl == NULL)
	{
		return;
	}
	for (int i = 0; i<sBattleCtrl->actor_list->size(); i++)
	{
		BattleActorPtr actor = sBattleCtrl->actor_list->elementAt(i);
		actor->hp_gain = 0;
		actor->mp_gain = 0;
		actor->cur_probability = -1;
	}
}
BattleActorPtr BattleControler::getBattleActor(int actor_id)
{
	if (sBattleCtrl == NULL)
	{
		return NULL;
	}
	for (int i = 0; i<sBattleCtrl->actor_list->size(); i++)
	{
		BattleActorPtr actor = sBattleCtrl->actor_list->elementAt(i);
		if(actor->hisActor->actor_id == actor_id)
		{
			return actor;
		}
	}
	return NULL;
}

BattleControler::BattleControler()
{
	chess = Chess::getInstance();
    actor_list = new DList<BattleActor>(true);
	history->is_victory_area = 0;
	string strSFile="";
    int map_index=-1;
    strSFile = trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene.rbegin()->second;
	if (history->m_iBattleType == BATTLE_PVP)
	{
		map_index = PVPData::getInstance()->getMapID();
	}
	else
	{
		map_index = trData->m_mNetWarInfo[history->m_iCurWarId]->iMapId;
	}
	handler.LoadSFileName(strSFile.c_str());
	strSFile.erase(0, 1);
	history->game_scene = StringToInt(strSFile.c_str());
	history->BG_index = map_index;
	history->GenerateMapArray(map_index - 1);

	SAFE_DELETE_IMAGE(history->imgBG);
    history->imgBG = CreateImage(ChessSprintf("img/HM/%d", history->BG_index), 1);
	
	strcpy(history->battle_name, getCurrentSenceTitle());

	if(chess->is_load == 0)
	{
		battle_state = BATTLE_STATE_INIT;	
		handler.SetScriptList(0);
		need_frame = 0;
	}
	else
	{
		battle_state = BATTLE_STATE_SHOW_BATTLE_START;
		need_frame = 201;
		//history->initilized = 1;
		//chess ->is_load = 0;
		//GenerateActorList();
		//ChessSetWindowSize(SCREEN_SIZE_BATTLE_MAX);
		//battle_state = BATTLE_STATE_WAIT_COMMAND;
	}
	curSkill = NULL;
	curGood = NULL;
	need_run_script = 0;
	next_round_flag = 1;
	need_round_add = 0;
	isShowTitleBG = false;
	m_iLastDir = 0;
	m_userOp = false;
}

BattleControler::~BattleControler()
{
	SAFE_DELETE(actor_list);
}

void BattleControler::onInit()
{
	sBattleCtrl = this;
}

void BattleControler::onDestroy()
{
	if (sBattleCtrl == this)
	{
		sBattleCtrl = NULL;
	}
}

void BattleControler::GenerateActorList()
{
	//TODO 生成战场对象的时候对xy进行重新计算
	int i;
	for (i = 0; i < history->battleOwnNum; i++) {
		history->FindNearestBlackPosInit(history->battleOwn[i], history->battleOwn[i]->x, history->battleOwn[i]->y);
		actor_list->addElement(new BattleActor(history->battleOwn[i]));
	}
	for (i = 0; i < history->battleFriendNum; i++) {
		history->FindNearestBlackPosInit(history->battleFriend[i], history->battleFriend[i]->x, history->battleFriend[i]->y);
		actor_list->addElement(new BattleActor(history->battleFriend[i]));
	}
	for (i = 0; i < history->battleEnemyNum; i++) {
		history->FindNearestBlackPosInit(history->battleEnemy[i], history->battleEnemy[i]->x, history->battleEnemy[i]->y);
		actor_list->addElement(new BattleActor(history->battleEnemy[i]));
	}
}
void BattleControler::RoundStartOwn()
{
	scr->bonus = new BattleRoundStart(0,CTRL_BONUS);
	scr->appendControl(scr->bonus);
	for(int i = 0; i < history->battleEnemyNum; i++)
	{
		history->battleEnemy[i]->updateRoundState();
	}
	next_round_flag = 0;

}

void BattleControler::RoundStartEnemy() 
{
	scr->bonus = new BattleRoundStart(1,CTRL_BONUS);
	scr->appendControl(scr->bonus);
	for(int i = 0;i<history->battleOwnNum;i++)
	{
		history->battleOwn[i]->updateRoundState();
	}
	for(int i = 0;i<history->battleFriendNum;i++)
	{
		history->battleFriend[i]->updateRoundState();
	}
}
void BattleControler::EnterBattle()
{
	if (history->mTitleLayer != NULL)
	{
		history->mTitleLayer->removeFromParent();
		history->mTitleLayer = NULL;
	}
	if (history->m_iBattleType == BATTLE_PVP)
	{
		history->BG_index = PVPData::getInstance()->getMapID();
	}
	else
	{
		history->BG_index = trData->m_mNetWarInfo[history->m_iCurWarId]->iMapId;
	}

	history->initilized = 1;
	history->game_state = GAME_STATE_BATTLE;
	ChessViewTool::getInstance()->setScreenFullSize();
	scr->initControl(); //初始化战斗界面控件
	if(chess->is_load == 0)
	{
		battle_state = BATTLE_STATE_INIT;
	}
	else
	{
		battle_state = BATTLE_STATE_SHOW_START_OWN;
		RoundStartOwn();
		if(history->bg_music_index > 0)
		{
			ChessPlayMp3(history->bg_music_index);
		}
		if(history->setting_flag[3])
		{
			scr->mini_map->changeState();
		}
	}
	isShowTitleBG = false;
	WinConditionTool::getInstance()->StartBattleSetCondition();

	for(int i = 0; i < history->battleOwnNum; i++)
	{
		history->battleOwn[i]->initCopySkill();
	}

	for(int i = 0; i < history->battleFriendNum; i++)
	{
		history->battleFriend[i]->initCopySkill();
	}

	for(int i = 0; i < history->battleEnemyNum; i++)
	{
		history->battleEnemy[i]->initCopySkill();
	}
}
void BattleControler::Ontimer()
{
	history->m_bWaitCommand = false;

	switch (battle_state) 
	{
	case BATTLE_STATE_INIT:
		if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_READY_TO_REPLAY)
		{
			BattleReplay::getInstance()->setCurStatus(REPLAY_STATUS_REPLAY);
			history->m_bAutoRun = true;
		}
		else if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_READY_TO_SAVE)
		{
			BattleReplay::getInstance()->startSaveReplay();
		}

		if (history->iExtraAddMaxRound)
			history->iExtraAddMaxRound = 0;

		if (!handler.RunScript()) 
		{
			if (history->m_iBattleType == BATTLE_PVP)
			{
				history->m_bAutoRun = true;
				history->m_bTakeTurns = true;
			}
			else
			{
				//自动配装
				for (int i = 0; i < history->battleEnemyNum; i++)
				{
					int iEneActorId = history->battleEnemy[i]->actor_id;
					if (!(iEneActorId >= LADDER_ACTOR_MIN_ID && iEneActorId < LADDER_ACTOR_MAX_ID) && !history->mNet72EquipActor.count(iEneActorId + 1))
						history->SetNetExternalEquip(history->battleEnemy[i]->actor_id, 0, 2);
				}

				for (int i = 0; i < history->battleFriendNum; i++)
				{
					int iFriActorId = history->battleFriend[i]->actor_id;
					if (!(iFriActorId >= LADDER_ACTOR_MIN_ID && iFriActorId < LADDER_ACTOR_MAX_ID) && !history->mNet72EquipActor.count(iFriActorId + 1))
						history->SetNetExternalEquip(history->battleFriend[i]->actor_id, 0, 2);
				}
			}

			history->SetCurRound(1);
			scr->ptrSceneTitle->UpdateRound();
			RoundStartOwn();
			battle_state = BATTLE_STATE_SHOW_START_OWN;
			if (history->setting_flag[3])
				scr->mini_map->changeState();
			if (!history->actors[0].hide) {
				HistoryActorPtr ptr = &history->actors[0];
				history->CenterActor(ptr);
			}
			else
			{
				for (int i = 0; i < history->battleOwnNum; i++)
				{
					if (!history->battleOwn[i]->hide)
					{
						history->CenterActor(history->battleOwn[i]);
						break;
					}
				}
			}
			if (history->m_iCurWarId != history->mScriptValidate)
			{
				ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(NULL, SHORTCUT_TYPE_JSYX, 0, 0));
			}
			if (BattleReplay::getInstance()->getCurStatus() != REPLAY_STATUS_READY_TO_REPLAY)
				StatConfig::onBeginMission(__String::createWithFormat("warId:%d", history->m_iCurWarId)->getCString());
		}
		break;
	case BATTLE_STATE_SHOW_BATTLE_START:
		{
			need_frame--;
			if(need_frame == 200)
			{
				//need_frame = 2;
				isShowTitleBG = true;
				ChessSetWindowSize(SCREEN_SIZE_REDE);
				playSound(SOUND_BATTLE_START);
				ChessString str;
				str.x = screen_size_x/2 - 18*strlen(getCurrentSenceTitle());
				str.y = screen_size_y/2;
				str.color = 0xffffffff;
				str.str = getCurrentSenceTitle();
				ChessAddString(&str);
			}
			else if(need_frame == 100)
			{
				GenerateActorList();
			}
			else  if(need_frame== 0)
			{
				EnterBattle();
			}
		}	
		break;
	case BATTLE_STATE_SHOW_START_OWN:

//        if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_READY_TO_REPLAY) {
//            BattleReplay::getInstance()->startReplay();
//            isTrustee = 1;
//        }

		if(chess ->is_load)
		{
			chess->is_load = 0;
			battle_state = BATTLE_STATE_WAIT_COMMAND;
		}
		else
		{
			battle_state = BATTLE_STATE_ACTOR_RECOVER;
			history->curActionSide = ACTOR_RELATION_OWN;
			handler.GenerateScriptListActorState(ACTOR_RELATION_OWN,history);
		}
		break;
	case BATTLE_STATE_SHOW_START_ENEMY:
			battle_state = BATTLE_STATE_ACTOR_RECOVER;			
			history->curActionSide = ACTOR_RELATION_ENEMY;
			handler.GenerateScriptListActorState(ACTOR_RELATION_ENEMY,history);
		break;
	case BATTLE_STATE_ACTOR_RECOVER:
		auto_run = 0;
		if (!handler.RunScript()) {
			auto_run = 1;
			battle_state = BATTLE_STATE_SPECIAL_OCCU_RECOVER;
			handler.ClearScriptList();
		}
		break;
	case BATTLE_STATE_SPECIAL_OCCU_RECOVER:
		auto_run = 0;
		if (!handler.RunScript()) {
			auto_run = 1;
			battle_state = BATTLE_STATE_RUN_SCRIPT;
			handler.ClearScriptList();
			handler.SetScriptList(1);
		}
		break;
	case BATTLE_STATE_RUN_SCRIPT:
		auto_run = 0;
		if (!handler.RunScript()) {
			auto_run = 1;
			if(need_run_script)
			{
				handler.SetScriptList(1);
				need_run_script = 0;
			}
			else if(need_round_add)
			{
				history->SetCurRound(-1,1);
				scr->ptrSceneTitle->UpdateRound();
				need_round_add = 0;

				if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
				{
					history->m_iCurAir = BattleReplay::getInstance()->getRepRound(history->GetCurRound())->getCurAir();
				}
				else
				{
					history->changeAir(GetActorChangeWeatherId());
					BattleReplay::getInstance()->getRepRound(history->GetCurRound())->setCurAir(history->m_iCurAir);
				}

				RoundStartOwn();
				battle_state = BATTLE_STATE_SHOW_START_OWN;
				need_frame = TIMER_SHOW_START_FRAME;
			}
			else
			{
				if (battle_state != BATTLE_STATE_ENDING_LOSE)
				{
					battle_state = BATTLE_STATE_ACTOR_LEAVE;
					history->cur_actor = NULL;
					need_frame = 0;
				}
			}
		}
		else
		{
			need_run_script = 1;
		}
		break;
	case BATTLE_STATE_PK:
		break;
	case BATTLE_STATE_SHOW_MOVE_REGION:
	case BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION:
		BattleActor::normal_frame++;
		if(auto_run)
		{
			if(need_frame-- == 10)
			{
				//生成行动。。。
				if(ai.mostGain->point.x == history->cur_actor->x&&ai.mostGain->point.y == history->cur_actor->y)
				{
					if(ai.mostGain->gains!=NULL && ai.mostGain->gains->size()>0)
					{
						battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
						need_frame = TIMER_SHOW_ATTACK_REGION;
					}
					else
					{ 
						SetActionfinish();
						
						curSkill = NULL;
						curGood = NULL;
						BattleActor::clearMoveList();
						battle_state = BATTLE_STATE_RUN_SCRIPT;
						handler.SetScriptList(1);
					}
					need_frame = 0;
				}
			}
			else if(need_frame-- == 0)
			{
				scr->autopointClicked((ai.mostGain->point.x*SCENE_TILE_WIDTH -history->scr_x),
					(ai.mostGain->point.y*SCENE_TILE_WIDTH - history->scr_y));
			}
		}
		break;
	case BATTLE_STATE_SHOW_ATTACK_MOVE_REGION:
		BattleActor::normal_frame++;
		break;
	case BATTLE_STATE_ACTOR_MOVE:
	case BATTLE_STATE_SHOW_DISPATCH_MOVE:
		if(need_frame-- == 0)
		{
			if(auto_run)
			{
				if (ai.mostGain->gains != NULL && ai.mostGain->gains->size()>0)
				{
					battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
					need_frame = TIMER_SHOW_ATTACK_REGION;
				}
				else
				{
                    SetActionfinish();
					curSkill = NULL;
					curGood = NULL;
					BattleActor::clearMoveList();
					changeActionSide();
					battle_state = BATTLE_STATE_RUN_SCRIPT;
					handler.SetScriptList(1);
				}
			}
			else
			{
				ai.ClearGainList();
				if (battle_state == BATTLE_STATE_SHOW_DISPATCH_MOVE)
				{
					battle_state = BATTLE_STATE_RUN_SCRIPT;
					handler.SetScriptList(1);
				}
				else if(history->cur_actor->actor_only_move == 2)
					SelectAction(3);
				else
					SelectAction(0);
			}
		}
		break;
	case BATTLE_STATE_WAIT_ACTION:
		BattleActor::normal_frame++;
		break;
	case BATTLE_STATE_SHOW_ATTACK_REGION:
		BattleActor::normal_frame++;
		if(auto_run)
		{
			if(need_frame-- == 0)
			{
				if (ai.mostGain->gains != NULL && ai.mostGain->gains->size()>0)
				{
					//ActionSingleGainPtr gain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
					//scr->pointerDragged((gain->oppo_actor->xx() - history->scr_x),
					//	(gain->oppo_actor->yy() - history->scr_y));
					battle_state = BATTLE_STATE_SHOW_ACTION_GAIN;
					need_frame = TIMER_SHOW_ATTACK_REGION;
				}
			}
		}
		break;
	case BATTLE_STATE_SHOW_ACTION_GAIN://获取到攻击效果
		if(auto_run)//敌方自动攻击
		{
			if(need_frame-- == 0)
			{
 				ActionSingleGainPtr gain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
					scr->autopointClicked((gain->oppo_actor->xx() - history->scr_x),
						(gain->oppo_actor->yy() - history->scr_y));
			}
		}
		break;
	case BATTLE_STATE_DO_ACTION:
	{
		if (!handler.RunScript()) {
		handler.ClearScriptList();
		history->bEnble5e = true;
        if(ai.skill_need_back && !(ai.need_twice || ai.need_follow_hit_times) && canSkillBack(ai.mostGain->gains->elementAt(0)->oppo_actor)) {  //法术反击
            processSkillBack();
        }
		else if((ai.need_twice || ai.need_follow_hit_times)&& Director::getInstance()->getRunningScene()->getChildByName("GuideLayer")==NULL)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);

			if(ai.need_xianshou || ai.need_siege)
			{
				ai.GenerateGain_AttackType(ai.curActor, curSingleGain->oppo_actor->x, curSingleGain->oppo_actor->y, ATTACK_TYPE_TWICE);
			}
			else
			{
				ai.GenerateGain_AttackType(history->cur_actor, curSingleGain->oppo_actor->x, curSingleGain->oppo_actor->y, ATTACK_TYPE_TWICE);
			}
			handler.GenerateScriptListFromAI(ai);

			if(ai.need_twice) //如果双击和几率追击攻击两特技共存，则双击第二次再判定几率追击攻击
			{
				ai.need_twice = 0;
			}
			else
			{
				if (ai.need_follow_probably > rand() % 100)
				{
					ai.need_follow_hit_times--;
				}
				else
				{
					ai.need_follow_hit_times = 0;
				}
			}
		}
		else if (ai.need_assault_attack)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (ai.need_assault_attack == 1 && curSingleGain != NULL && curSingleGain->probability)
			{
				int x = 0, y = 0;
				needAssault(ai.curActor, curSingleGain->oppo_actor, x, y);
				if (ai.curActor->x > curSingleGain->oppo_actor->x)
					curSingleGain->oppo_actor->direction = 1;
				else if (ai.curActor->x < curSingleGain->oppo_actor->x)
					curSingleGain->oppo_actor->direction = 3;
				else if (ai.curActor->y < curSingleGain->oppo_actor->y)
					curSingleGain->oppo_actor->direction = 0;
				else if (ai.curActor->y > curSingleGain->oppo_actor->y)
					curSingleGain->oppo_actor->direction = 2;
				curSingleGain->oppo_actor->x = x;
				curSingleGain->oppo_actor->y = y;
			}
			else if (ai.need_assault_attack==2)
			{
				for (int i = 0; i < ai.mostGain->gains->size(); i++)
				{
					ActionSingleGainPtr ptrCurSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(i);
					if (ptrCurSingleGain&&ptrCurSingleGain->probability&&ptrCurSingleGain->actor_knockback == 1)
					{
						int x = -1, y = -1;
						curSingleGain->oppo_actor->needAssault(ai.curActor, x, y);
						if (ai.curActor->x > curSingleGain->oppo_actor->x)
							curSingleGain->oppo_actor->direction = 1;
						else if (ai.curActor->x < curSingleGain->oppo_actor->x)
							curSingleGain->oppo_actor->direction = 3;
						else if (ai.curActor->y < curSingleGain->oppo_actor->y)
							curSingleGain->oppo_actor->direction = 0;
						else if (ai.curActor->y > curSingleGain->oppo_actor->y)
							curSingleGain->oppo_actor->direction = 2;
						if (x>-1&&y>-1&&!history->FindActorPosition(x, y))
						{
							curSingleGain->oppo_actor->x = x;
							curSingleGain->oppo_actor->y = y;
						}
					}
				}
			}
			ai.need_assault_attack = 0;
		}
		else if (ai.bear_hand_attack)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (ai.bear_hand_attack == 1 && curSingleGain != NULL)
			{
				int cur_x = 0, cur_y = 0, oppo_x = ai.curActor->x, oppo_y = ai.curActor->y;
				ActorAssault(curSingleGain->oppo_actor, ai.curActor, cur_x, cur_y);
				ai.curActor->x = cur_x;
				ai.curActor->y = cur_y;
				curSingleGain->oppo_actor->x = oppo_x;
				curSingleGain->oppo_actor->y = oppo_y;
			}
			ai.bear_hand_attack = 0;
		}
		else if (ai.two_order_spike)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain != NULL)
			{
				int x = -1, y = -1;
				ActorAssault(ai.curActor, curSingleGain->oppo_actor,x, y, ai.two_order_spike,true);
				if (x > -1 && y > -1 && !history->FindActorPosition(x, y))
				{
					curSingleGain->oppo_actor->x = x;
					curSingleGain->oppo_actor->y = y;
				}
			}
			ai.two_order_spike = 0;
		}
		else if (ai.through_the_target)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain != NULL)
			{
				int x = -1, y = -1;
				curSingleGain->oppo_actor->needAssault(ai.curActor, x, y);
				if (x>-1&&y>-1&&!history->FindActorPosition(x,y))
				{
					ai.curActor->x = x;
					ai.curActor->y = y;
				}
			}
			ai.through_the_target = 0;
		}
		else if (ai.new_assault)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain != NULL)
			{
				int cur_x = curSingleGain->oppo_actor->x, cur_y = curSingleGain->oppo_actor->y, oppo_x = -1, oppo_y = -1;
				if (ai.mostGain->skill == NULL)
				{
					ActorAssault(ai.curActor, curSingleGain->oppo_actor, oppo_x, oppo_y);

				}
				else
				{
					curSingleGain->oppo_actor->needAssault(ai.curActor, oppo_x, oppo_y);
				}
				if (oppo_x>-1&&oppo_y>-1&&!history->FindActorPosition(oppo_x,oppo_y))
				{
					curSingleGain->oppo_actor->x = oppo_x;
					curSingleGain->oppo_actor->y = oppo_y;

					ai.curActor->x = cur_x;
					ai.curActor->y = cur_y;
				}
			}
			ai.new_assault = 0;
		}
		else if (ai.actor_can_drag)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain != NULL)
			{
				int iOffsetX = ai.curActor->x - curSingleGain->oppo_actor->x, iOffsetY = ai.curActor->y - curSingleGain->oppo_actor->y;

				int oppo_x = curSingleGain->oppo_actor->x, oppo_y = curSingleGain->oppo_actor->y;
				if (iOffsetX != 0)
					oppo_x = ai.curActor->x - abs(iOffsetX) / iOffsetX;
				else if(iOffsetY != 0)
					oppo_y = ai.curActor->y - abs(iOffsetY) / iOffsetY;

				if (oppo_x > -1 && oppo_y > -1 && !history->FindActorPosition(oppo_x, oppo_y))
				{
					curSingleGain->oppo_actor->x = oppo_x;
					curSingleGain->oppo_actor->y = oppo_y;
				}
			}
			ai.actor_can_drag = 0;
		}
		else if (ai.need_siege)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain == NULL){
				ai.need_siege = 0;
				if (ai.curActor)
				{
					ai.curActor->siege_state = false;
				}
				break;
			}
			HistoryActorPtr siege_curActor = ai.curActor;
			HistoryActorPtr siege_oppo_actor = curSingleGain->oppo_actor;
			if (ai.is_xianshou == 1)
			{
				siege_curActor = curSingleGain->oppo_actor;
				siege_oppo_actor = ai.curActor;
			}
			if (!siege_curActor->good_effects[36])//无限移动自动回归
			{
				siege_curActor->action_finished = 1;
			}
			ClearGain();
			if (ai.need_siege == 1)//至多三人攻击，最后一个不是
			{
				ai.need_siege = 0;
			}
			else
			{
				if (siege_oppo_actor->GetCurHp() > 0)
				{
					HistoryActorPtr siegeActor = siege_oppo_actor->RangeOfEnemy(history->cur_actor->actor_id);
					if (siegeActor)
					{
						siegeActor->siege_state = true;
						if (ai.mostGain->need_back&&ai.is_xianshou == 1)
							ai.is_xianshou = 2;
						else if (!ai.mostGain->need_back&&ai.is_xianshou != 0)
							ai.is_xianshou = 0;
						ai.GenerateGain_AttackType(siegeActor, siege_oppo_actor->x, siege_oppo_actor->y,
							ATTACK_TYPE_NORMAL);
						handler.GenerateScriptListFromAI(ai);
						ai.need_siege--;
					}
					else
					{
						ai.need_siege = 0;
					}
				}
				else
				{
					ai.need_siege = 0;
				}
			}
		}
		else if (ai.need_attack_sweep)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			int attack_id = ai.curActor->getAttackAreaId();
			HistoryActorPtr oppo_actor = NULL,oppo_maxhp = NULL;
			int x, y;
			int iAddPre = ai.curActor->getSpecialInt(TYPE725);
			if (trData->hitArea[attack_id].length >= 0 && iAddPre>0)
			{
				for (int i = 0; i < trData->hitArea[attack_id].length; i++)
				{
					x = ai.curActor->x + trData->hitArea[attack_id].pointlist[i].x;
					y = ai.curActor->y + trData->hitArea[attack_id].pointlist[i].y;

					oppo_actor = history->FindOppoActorPosition(ai.curActor, x, y);

					if (oppo_actor && !oppo_actor->isHadSpecial(TYPE743) &&oppo_actor->GetCurHp() > 0&& ai.curActor->getRealAbility(ACTOR_ABILITY_POWER) > iAddPre*oppo_actor->getRealAbility(ACTOR_ABILITY_DEFENCE)/100)
					{
						if (oppo_maxhp == NULL|| oppo_maxhp->GetCurHp() < oppo_actor->GetCurHp())
							oppo_maxhp = oppo_actor;
					}
				}
			}

			if (oppo_maxhp&&history->cur_actor->state_stun == 0 && history->cur_actor->state_puzzle == 0 && history->cur_actor->state_obfuscation == 0)
			{
				ai.GenerateGain_AttackType(ai.curActor, oppo_maxhp->x, oppo_maxhp->y, ATTACK_TYPE_ATTACK_SWEEP);

				handler.GenerateScriptListFromAI(ai);
			}

			ai.need_attack_sweep = 0;
		}
		else if (ai.need_spontaneous_detonation.size())
		{
			int iActorId = 0;
			int iEnableClear = 1;
			for (map<int, int>::iterator eIt = ai.need_spontaneous_detonation.begin(); eIt != ai.need_spontaneous_detonation.end(); eIt++)
			{
				if (eIt->second == 1)
				{
					iActorId = eIt->first;
					iEnableClear = 0;
					break;
				}
			}

			if (iEnableClear)
			{
				ai.need_spontaneous_detonation.clear();
			}
			else
			{
				ai.need_spontaneous_detonation[iActorId] = 2;
				int iSkillId = history->actors[iActorId - 1].getSpecialInt(TYPE745);
				if (iSkillId > 0 && iSkillId <= trData->skill_num)
				{
					ai.GenerateGain_SkillType(&trData->skills[iSkillId-1], &history->actors[iActorId - 1], history->actors[iActorId - 1].x, history->actors[iActorId - 1].y,
						SKILL_TYPE_NORMAL);
					if (ai.mostGain&&ai.mostGain->gains->size())
					{
						if (ai.mostGain->gain_hp > 0)
						{
							ai.mostGain->gain_hp = 0;
						}
						handler.GenerateScriptListFromAI(ai);
					}
				}
			}
		}
		else if (ai.mostGain->need_back)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (!curSingleGain)
			{
				ai.mostGain->need_back = 0;
				break;
			}

			HistoryActorPtr back_curActor = curSingleGain->oppo_actor;
			HistoryActorPtr back_oppo_actor = ai.curActor;
			if (ai.is_xianshou  == 1)
			{
				ai.is_xianshou = 0;
			}
			else if (ai.is_xianshou == 2)
			{
				back_curActor = history->cur_actor;
				back_oppo_actor = curSingleGain->oppo_actor;
				ai.is_xianshou = 0;
			}

			if (back_oppo_actor->GetCurHp()>0 && back_curActor->isAttackArea(back_oppo_actor->x, back_oppo_actor->y, true))
			{
				if (back_curActor->state_puzzle == 0 && back_curActor->state_blind == 0 && back_curActor->state_obfuscation == 0)
				{
					if (back_curActor->GetCurHp() > 0)
					{
						if (back_curActor->good_intensify_back_hit)
						{
							ai.need_intensify_back_hit = back_curActor->good_intensify_back_hit;
						}

						if (back_curActor->isHadSpecial(TYPE753) && curSingleGain->probability <= 0)
						{
							ai.need_intensify_back_hit += back_curActor->getSpecialMap(TYPE753)[1];
						}

						if (back_curActor->good_effects[13])//反击时触发突击攻击
						{
							needAssault(back_curActor, back_oppo_actor);
						}
						else if (!back_oppo_actor->isHadSpecial(TYPE743) && back_curActor->good_effects[TWOORDER_SPIKE])
						{
							if ((int)abs(back_curActor->x - back_oppo_actor->x) < 2 && (int)abs(back_curActor->y - back_oppo_actor->y) < 2)
							{
								if (!(back_oppo_actor->getSpecialInt(TYPE703) || (rand() % 100) < back_oppo_actor->getSpecialInt(TYPE722)))
								{
									int iX = 0, iY = 0;
									if (ActorAssault(back_curActor, back_oppo_actor, iX, iY,1,true) && ActorAssault(back_curActor, back_oppo_actor, iX, iY, 2,true))
									{
										ai.two_order_spike = 2;
									}
									else if (ActorAssault(back_curActor, back_oppo_actor, iX, iY,1, true))
									{
										ai.two_order_spike = 1;
									}
								}
							}
						}

						ai.GenerateGain_AttackType(back_curActor, back_oppo_actor->x, back_oppo_actor->y, ATTACK_TYPE_BACK);
						handler.GenerateScriptListFromAI(ai);

						ActionSingleGainPtr assault_curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);

						if (assault_curSingleGain == NULL){
							ai.mostGain->need_back = 0;
							break;
						}
						if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
						{
							if (BattleReplay::getInstance()->getBattleAI(false)->need_back_attack_twice == 1)
							{
								ai.need_back_attack_twice = 1;
								ai.ClearGain();
								ai.mostGain = ai.GetAttackGain(ai.curActor->x, ai.curActor->y, history->cur_actor->x, history->cur_actor->y);
							}
						}
						else if(ai.curActor->good_back_attack_twice > rand() % 100 || ai.curActor->good_effects[HITBACK_DOUBLE] > rand() % 100)
						{
							ai.need_back_attack_twice = 1;
							ai.ClearGain();
							ai.mostGain = ai.GetAttackGain(ai.curActor->x, ai.curActor->y, history->cur_actor->x, history->cur_actor->y);
						}
					}
				}
			}
			ai.mostGain->need_back = 0;
			ai.skill_need_back = 0; //如果存在物理反击，则法术反击不触发
		}
		else if (ai.need_back_attack_twice)
		{
			if (history->cur_actor->GetCurHp() > 0){//反击时双击实现
				ai.GenerateGain_AttackType(ai.curActor, history->cur_actor->x, history->cur_actor->y, ATTACK_TYPE_TWICE);
				handler.GenerateScriptListFromAI(ai);
			}
			ai.need_back_attack_twice = 0;
		}
		else if (ai.need_back_back)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);

			if (!curSingleGain){
				ai.need_back_back = 0;
				return;
			}

			if (history->cur_actor->GetCurHp() > 0 && curSingleGain->oppo_actor->GetCurHp() > 0 && history->cur_actor->state_blind == 0 && history->cur_actor->state_obfuscation == 0 && history->cur_actor->state_puzzle == 0 && history->cur_actor->isAttackArea(ai.curActor->x, ai.curActor->y, true))//反击后反击必须在攻击范围中
			{

				ai.GenerateGain_AttackType(history->cur_actor, ai.curActor->x, ai.curActor->y,
					ATTACK_TYPE_BACK_BACK);
				handler.GenerateScriptListFromAI(ai);
			}
			ai.need_back_back = 0;
		}
		else if (ai.skill_bravely_fight[0] > 0)
		{
			if (history->cur_actor->state_blind || history->cur_actor->state_puzzle || history->cur_actor->state_obfuscation || !history->cur_actor->GetCurHp())
			{
				ai.skill_bravely_fight.clear();
				break;
			}

			int attack_id = 12;
			HistoryActorPtr oppo_actor = NULL;
			int x, y;
			int iCurIndex = -1;
			if (trData->hitArea[attack_id].length >= 0)
			{
				for (int i = ai.skill_bravely_fight[0] - 1; i<trData->hitArea[attack_id].length; i++)
				{
					x = history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x;
					y = history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0 && oppo_actor->actor_id != ai.skill_bravely_fight[2])
					{
						iCurIndex = i + 2;
						break;
					}
					else
						oppo_actor = NULL;
				}
			}
			if (oppo_actor != NULL)
			{
				ai.mostGain->skill = &trData->skills[ai.skill_bravely_fight[3]];
				ai.skill_bravely_fight[0] = iCurIndex;
				ai.skill_bravely_fight[1]++;
				ai.GenerateGain_SkillType(ai.mostGain->skill, history->cur_actor, oppo_actor->x, oppo_actor->y,
					SKILL_TYPE_BRAVELY);
				ai.mostGain->iDontShowSkillName = 1;
				handler.GenerateScriptListFromAI(ai);
			}

			if (!oppo_actor || ai.skill_bravely_fight[1] >= (ai.mostGain->skill->prop_num + ai.mostGain->skill->GetExtraGainHp(history->cur_actor, 2) - 1))//总攻击次数=一次普通攻击+奋战次数
			{
				ai.skill_bravely_fight.clear();
			}
		}
		else if (ai.need_bravely_fight)
		{
			if (history->cur_actor->state_blind || history->cur_actor->state_puzzle || history->cur_actor->state_obfuscation || !history->cur_actor->GetCurHp())
			{
				ai.need_bravely_fight = 0;
				ai.bravely_fight_oppo_actor.clear();
				break;
			}

			int attack_id = history->cur_actor->getAttackAreaId();
			HistoryActorPtr oppo_actor = NULL;
			int x, y;
			if (trData->hitArea[attack_id].length >= 0)
			{
				for (int i = 0; i<trData->hitArea[attack_id].length; i++)
				{
					x = history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x;
					y = history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0 && !ai.bravely_fight_oppo_actor.count(oppo_actor->actor_id+1))
						break;
					else
						oppo_actor = NULL;
				}
			}
			if (oppo_actor != NULL)
			{
				ai.bravely_fight_oppo_actor[oppo_actor->actor_id+1] = 1;
				ai.bravely_fight_num++;
				ai.GenerateGain_AttackType(history->cur_actor, x, y, ATTACK_TYPE_INDUCT);
				ai.mostGain->need_back = 1;
				if (history->cur_actor->good_hit_twice == 100)//二次攻击属性
				{
					ai.need_twice = 1;
				}
				handler.GenerateScriptListFromAI(ai);
			}

			if(!oppo_actor || history->cur_actor->getSpecialInt(TYPE109) <= ai.bravely_fight_num + 1)//总攻击次数=一次普通攻击+奋战次数
			{
				ai.need_bravely_fight = 0;
				ai.bravely_fight_oppo_actor.clear();
			}

		}
		else if (ai.need_induct)
		{
			int attack_id = history->cur_actor->getAttackAreaId();
			HistoryActorPtr oppo_actor = NULL;
			int x, y;
			//ai.need_induct = 0; //无限引导
			if (trData->hitArea[attack_id].length >= 0)
			{
				int i = 0;
				for(i = 0; i<trData->hitArea[attack_id].length; i++)
				{
					x = history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x;
					y = history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0)
						break;
					else
						oppo_actor = NULL;
				}
				if(i == trData->hitArea[attack_id].length&&!oppo_actor)
					ai.need_induct = 0;
			}
			else if(trData->hitArea[attack_id].length == -1)
			{
				int i = 0;
				for(i = 0; i < scr->pointNum; i++)
				{
					x = scr->pointList[i].x;
					y = scr->pointList[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0)
					{
						break;
					}
					else
					{
						oppo_actor = NULL;
					}
				}
				if(i == scr->pointNum && !oppo_actor)
					ai.need_induct = 0;
			}

			if (oppo_actor != NULL)
			{
				bool isNeedinduct = false;
                if(history->cur_actor->good_induct_hit > ai.induct_num){
                    isNeedinduct = true;
                }
				else if (history->cur_actor->weapon.isNormalGood() && history->cur_actor->weapon.getInductNum() > ai.induct_num)
				{
					isNeedinduct = true;
				}
				else if (history->cur_actor->weapon.isNormalGood() && history->cur_actor->weapon.getInductNum() <= ai.induct_num)
					ai.need_induct = 0;
				if(history->cur_actor->assist_good.isNormalGood() && (isNeedinduct == false) && history->cur_actor->assist_good.getInductNum() > ai.induct_num)
				{
					isNeedinduct = true;
				}
				else if(history->cur_actor->assist_good.isNormalGood() && (isNeedinduct == false) && history->cur_actor->assist_good.getInductNum() <= ai.induct_num)
					ai.need_induct = 0;
				if(history->cur_actor->coat.isNormalGood() && (isNeedinduct == false) && history->cur_actor->coat.getInductNum() > ai.induct_num)
				{
					isNeedinduct = true;
				}
				else if(history->cur_actor->coat.isNormalGood() && (isNeedinduct == false) && history->cur_actor->coat.getInductNum() <= ai.induct_num)
					ai.need_induct = 0;

				int iActorInductNum = 0;
				if (isNeedinduct == false && (iActorInductNum = history->cur_actor->getInductNum()))
				{
					if (iActorInductNum > ai.induct_num)
					{
						isNeedinduct = true;
					}
					else
					{
						ai.need_induct = 0;
					}
				}

				if (isNeedinduct)
				{
					ai.induct_num++;
					if (history->cur_actor->GetCurHp() > 0)
						ai.GenerateGain_AttackType(history->cur_actor, x, y, ATTACK_TYPE_INDUCT);
					ai.mostGain->need_back = 1;

					if (history->cur_actor->good_hit_twice == 100)//二次攻击属性
					{
						ai.need_twice = 1;
					}
					handler.GenerateScriptListFromAI(ai);
				}
			}
		}
		else if (ai.need_pursuit_attack)
		{
			HistoryActorPtr tmp_oppoactor = history->actors + ai.need_pursuit_attack_actor[0];
            HistoryActorPtr tmp_curactor = history->actors + ai.need_pursuit_attack_actor[ai.need_pursuit_attack];
			if (tmp_oppoactor->GetCurHp() > 0)
			{
				if (tmp_curactor->GetCurHp() > 0 && !tmp_curactor->is_leave && !tmp_curactor->hide)
				{
					ai.ClearGain();
					ai.mostGain = ai.GetAttackGain(tmp_curactor->x, tmp_curactor->y, tmp_oppoactor->x, tmp_oppoactor->y);
					ai.GenerateGain_AttackType(tmp_curactor, tmp_oppoactor->x, tmp_oppoactor->y, ATTACK_TYPE_PURSUIT_ATTACK);
					handler.GenerateScriptListFromAI(ai);
					tmp_curactor->good_pursuit_attack_num++;
				}
				ai.need_pursuit_attack--;
			}
			else
			{
				ai.need_pursuit_attack = 0;
			}
		}
		else if (ai.skill_need_twice) //法术连击
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
            //当连击发起者处于封禁、混乱、昏迷时，均不能连击
			if (curSingleGain->oppo_actor->GetCurHp() > 0 && history->cur_actor->state_stun == 0 && history->cur_actor->state_puzzle == 0 && history->cur_actor->state_obfuscation == 0)
			{
				ai.GenerateGain_SkillType(ai.mostGain->skill, history->cur_actor, curSingleGain->oppo_actor->x, curSingleGain->oppo_actor->y,
					SKILL_TYPE_TWICE);
				handler.GenerateScriptListFromAI(ai);
			}
			ai.skill_need_twice = 0;
		}
		else if (ai.skill_need_follow)
		{
			HistoryActorPtr tmp_oppoactor = history->actors + ai.skill_need_follow_actor[0];
			HistoryActorPtr tmp_curactor = NULL;
			SkillAttrPtr skill = NULL;
            tmp_curactor = history->actors + ai.skill_need_follow_actor[ai.skill_need_follow];

            int temp_skill_id = 0;

			if (tmp_curactor->isHadSpecial(TYPE767))
			{
				temp_skill_id = tmp_curactor->getSpecialMap(TYPE767)[1] - 1;
			}
			else
			{
				temp_skill_id = trData->m_mSeid[tmp_curactor->good_effects[22]]->special_effe[0] - 1;
			}

			if (tmp_oppoactor->GetCurHp() > 0)
			{
				if (temp_skill_id < trData->skill_num&&trData->skills[temp_skill_id].BeingAbleToCastSpells(tmp_curactor)
					&& tmp_curactor->GetCurMp() >= trData->skills[temp_skill_id].GetMpConsume(tmp_curactor) 
					&& tmp_curactor->state_stun == 0 && tmp_curactor->state_puzzle == 0 && tmp_curactor->state_obfuscation == 0)
				{
					if (tmp_curactor->isHadSpecial(TYPE767))
						tmp_curactor->pursuit_skill_lv = tmp_curactor->GetActorLv();
					skill = &trData->skills[temp_skill_id];
					ai.ClearGain();
					ai.mostGain = ai.GetSkillGain(skill, tmp_curactor->x, tmp_curactor->y, tmp_oppoactor->x, tmp_oppoactor->y);
					ai.GenerateGain_SkillType(skill, tmp_curactor, tmp_oppoactor->x, tmp_oppoactor->y, SKILL_TYPE_FOLLOW);
					handler.GenerateScriptListFromAI(ai);
					tmp_curactor->good_skill_pursuit_attack_num++;
					tmp_curactor->pursuit_skill_lv = 0;
				}

				ai.skill_need_follow--;
			}
			else{
				ai.skill_need_follow = 0;
			}

		}
		else if (ai.need_attact_all_enemy)
		{
			ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
			if (curSingleGain == NULL || history->cur_actor <= 0 || history->cur_actor->state_blind || history->cur_actor->state_puzzle || history->cur_actor->state_obfuscation){
				ai.need_attact_all_enemy = 0;
				ai.need_attact_all_enemy_num = 0;
				break;
			}
			int i = 0;
			int attack_id = history->cur_actor->getAttackAreaId();
			HistoryActorPtr oppo_actor = NULL;
			int x, y;
			//ai.need_induct = 0; //无限引导
			if (trData->hitArea[attack_id].length >= 0)
			{
				for (i = ai.need_attact_all_enemy_num; i<trData->hitArea[attack_id].length; i++)
				{
					x = history->cur_actor->x + trData->hitArea[attack_id].pointlist[i].x;
					y = history->cur_actor->y + trData->hitArea[attack_id].pointlist[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0 && oppo_actor->actor_id != ai.need_attact_all_enemy - 1)
					{
						ai.GenerateGain_AttackType(history->cur_actor, oppo_actor->x, oppo_actor->y,
							ATTACK_TYPE_NORMAL);
						handler.GenerateScriptListFromAI(ai);
                        if(history->cur_actor->good_hit_no_back != 100)
                            ai.mostGain->need_back = 1;
						if (history->cur_actor->good_hit_twice == 100)//二次攻击属性
							ai.need_twice = 1;
						ai.need_attact_all_enemy_num = i+1;
						break;
					}
				}
				if (i == trData->hitArea[attack_id].length)
				{
					ai.need_attact_all_enemy = 0;
					ai.need_attact_all_enemy_num = 0;
				}
			}
			else if (trData->hitArea[attack_id].length == -1)
			{
				for (i = ai.need_attact_all_enemy_num; i < scr->pointNum; i++)
				{
					x = scr->pointList[i].x;
					y = scr->pointList[i].y;
					oppo_actor = history->FindOppoActorPosition(history->cur_actor, x, y);
					if (oppo_actor != NULL&&oppo_actor->GetCurHp()>0 && oppo_actor->actor_id != ai.need_attact_all_enemy - 1)
					{
						ai.GenerateGain_AttackType(history->cur_actor, oppo_actor->x, oppo_actor->y,
							ATTACK_TYPE_NORMAL);
						handler.GenerateScriptListFromAI(ai);
						ai.need_attact_all_enemy_num = i+1;
                        if(history->cur_actor->good_hit_no_back != 100)
                            ai.mostGain->need_back = 1;
						break;
					}
				}
				if (i == scr->pointNum)
				{
					ai.need_attact_all_enemy = 0;
					ai.need_attact_all_enemy_num = 0;
				}
			}
			//ai.need_attact_all_enemy = 0;
		}
		else if (ai.skill_dispatch_actor)
		{
			SetActionfinish();
			ClearGain();
			curSkill = NULL;
			curGood = NULL;
			changeActionSide();

			battle_state = BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION;
			history->cur_actor = &history->actors[ai.skill_dispatch_actor - 1];
			ai.GenerateDispatchMoveList(history->cur_actor);
			ai.skill_dispatch_actor = 0;
		}
		else if (ai.skill_double_hit)
		{
			if (ai.skill_double_hit >= 2)
			{
				ClearGain();
				curSkill = NULL;
				curGood = NULL;
				ai.curActor = history->cur_actor;
				battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
				ai.GenerateAttackGainList(BattleActor::move_index);
			}
			else if (ai.skill_double_hit == 1 && ai.mostGain == NULL)
			{
				history->cur_actor->action_finished = 1;
				ClearGain();
				curSkill = NULL;
				curGood = NULL;
				BattleActor::clearMoveList();
				battle_state = BATTLE_STATE_RUN_SCRIPT;
				handler.SetScriptList(1);
			}
			ai.skill_double_hit--;
		}
		else
		{
			SetActionfinish();
			ClearGain();
			curSkill = NULL;
			curGood = NULL;
			BattleActor::clearMoveList();
			changeActionSide();
			battle_state = BATTLE_STATE_RUN_SCRIPT;
			handler.SetScriptList(1);
		}
	}
}
		break;
	case BATTLE_STATE_ACTOR_LEAVE://撤退
		if(need_frame-- == 0)
		{
			if(history->cur_actor==NULL)
			{
				history->cur_actor  = history->getNextDeadActor();
				if (history->cur_actor != NULL&&history->cur_actor->relation == 0 && history->m_iBattleType != BATTLE_PVP && strlen(trData->actors[history->cur_actor->actor_id].retreat_word)>0 && strcmp(trData->actors[history->cur_actor->actor_id].retreat_word, "null"))
				{
					history->cur_actor->retreatTimes++;
					scr->m_pTalkLayer->updateLayer(trData->actors[history->cur_actor->actor_id].retreat_word, history->cur_actor->actor_id);
					need_frame = 0;
					break;
				}
			}
			if(history->cur_actor!=NULL)
			{
				BattleActorPtr battle_actor = getBattleActor(history->cur_actor->actor_id);
				if(history->cur_actor->actor_id == history->own_skipper_index)
					need_frame = battle_actor->setState(ACTOR_STATE_LEAVE_DEAD) + 10;
				else
					need_frame = battle_actor->setState(ACTOR_STATE_LEAVE_NORMAL)+10;
				
				history->cur_actor = NULL;
				need_run_script = 1;
			}
			else if(need_run_script!=0)
			{
				battle_state = BATTLE_STATE_RUN_SCRIPT;
				handler.SetScriptList(1);
				need_run_script = 0;
			}
			else
			{
				battle_state = BATTLE_STATE_WAIT_COMMAND;
				need_frame = 1;
			}
		}
		break;
	case BATTLE_STATE_WAIT_COMMAND:	
		history->m_bWaitCommand = true;
		BattleActor::normal_frame++;

		if(history->m_bTakeTurns)
		{
			autoRunAction();
			break;
		}

		switch(history->curActionSide)
		{
		case ACTOR_RELATION_OWN:
			if(history->m_bAutoRun)
			{
				int enemy_num = history->getBattleEnemyNum() + history->getBattleAssistNum();
				if(!enemy_num) //敌军总数为0，取消托管
				{
					history->m_bAutoRun = false;
				}
			}
			auto_run = history->m_bAutoRun;
			if(auto_run)
			{
				if (history->cur_actor == NULL)
				{
					if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
					{
						BattleAI* ptrAI = BattleReplay::getInstance()->getBattleAI(false);
						if (ptrAI && ptrAI->curActor)
						{
							history->cur_actor = ptrAI->curActor;
						}
						else
						{
							RoundStartFriendNew();
						}
					}
					else
						history->cur_actor = history->getNextActiveOwn();
				}
				if(history->cur_actor!=NULL){
					ai.GenerateMoveList(history->cur_actor);
					ai.GenerateMostGain();
					if(ai.mostGain->moveIndex==-1 && ai.mostGain->gains->size() == 0)
					{
						SetActionfinish();
						BattleActor::clearMoveList();
						history->cur_actor = NULL;
						curSkill = NULL;
						curGood = NULL;
					}
					else
					{
						history->CheckPosActor(history->cur_actor);
						//scr->pointerDragged((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
						scr->autopointClicked((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
					}
				}
					
			}
			if(history->getBattleOwnNum() == 0)
			{
				//history->curActionSide = ACTOR_RELATION_FRIEND;
				//history->cur_actor = NULL;
				RoundStartFriend();
			}
			else if (history->getActiveOwnNum() == 0&&(next_round_flag == 0 || history->m_bAutoRun)) 
			{
				if (history->m_bAutoRun)
				{
					RoundStartFriend();
				}
				else
				{
					scr->nextRound();
				}
				next_round_flag = 1;
			}
			else if(auto_run==0)
			{
				if(need_frame == 1)
				{
					need_frame = 0;
					m_userOp = false;
				}
				else if ((!m_userOp) && need_frame <= 0 && history->m_iCurWarId >= LessGuideStatusWarID) {//引导后面的关卡需要每回合自动选择角色
					HistoryActorPtr cur_actor = history->getNextActiveOwn();
					if (cur_actor != NULL) {
						history->CheckPosActor(cur_actor);
						scr->autopointClicked((cur_actor->xx() - history->scr_x), (cur_actor->yy() - history->scr_y));
						m_userOp = true;
					}
				}
			}
			break;
		case ACTOR_RELATION_FRIEND:
			auto_run = 1;
			if(history->cur_actor == NULL)
				history->cur_actor = history->getNextActiveFriend();
			if(history->cur_actor!=NULL){
				ai.GenerateMoveList(history->cur_actor);
				ai.GenerateMostGain();
				curSkill = ai.mostGain->skill;
				if(ai.mostGain->moveIndex==-1)
				{
					SetActionfinish();
					BattleActor::clearMoveList();
					history->cur_actor = NULL;
				}
				else
				{
					history->CheckPosActor(history->cur_actor);
					//scr->pointerDragged((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
					scr->autopointClicked((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
				}
			}else {
				ai.need_siege = 0;
				if (ai.curActor)
				{
					ai.curActor->siege_state = false;
				}
				RoundStartEnemy();
				battle_state = BATTLE_STATE_SHOW_START_ENEMY;
				need_frame = TIMER_SHOW_START_FRAME;
			}
			break;
		case ACTOR_RELATION_ENEMY://敌军行动
			auto_run = 1;
			history->cur_actor = history->getNextActiveEnemy();
			if(history->cur_actor!=NULL)
			{
				ai.GenerateMoveList(history->cur_actor);//获取到移动路径
				ai.GenerateMostGain();//获取到最优攻击
				curSkill = ai.mostGain->skill;
				if (ai.mostGain->moveIndex == -1 || (ai.mostGain->moveIndex == 0 && (ai.mostGain->gains == NULL || ai.mostGain->gains->size()==0)))
				{
					SetActionfinish();
					BattleActor::clearMoveList();
					history->cur_actor = NULL;
				}
				else
				{
					//自动移动屏幕焦点
					history->CheckPosActor(history->cur_actor);
					//scr->pointerDragged((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
					scr->autopointClicked((history->cur_actor->xx()-history->scr_x),(history->cur_actor->yy() - history->scr_y));
				}
			}
			else 
			{	//敌军回合结束
				need_round_add = 1;//标示更新标题
				curSkill = NULL;
				curGood = NULL;
				BattleActor::clearMoveList();
				battle_state = BATTLE_STATE_RUN_SCRIPT;
				handler.SetScriptList(1);
				m_userOp = false;
			}
			break;
		}
		break;
	case BATTLE_STATE_ENDING_WIN:
		if (!handler.RunScript())
		{
			//去除召唤对象
			{
				map<int, HistoryActorPtr> mActorMap;
				int iMapIndex = 0;
				bool bEnableSet = false;
				for (int i = 0; i < history->battleOwnNum; i++)
				{
					if (history->battleOwn[i]->controlled_by_enemy)
					{
						history->battleOwn[i]->recoverState();
						history->battleOwn[i]->delete_controlled_by_enemy();
						i--;
					}
					else if (trData->occupations[history->battleOwn[i]->occupation_id].is_robot == 1)
					{
						bEnableSet = true;
						history->battleOwn[i]->recoverState();
					}
					else
					{
						mActorMap[iMapIndex++] = history->battleOwn[i];
					}
				}

				if (bEnableSet)
				{
					for (int i = 0; i < iMapIndex; i++)
					{
						history->battleOwn[i] = mActorMap[i];
					}

					history->battleOwnNum = iMapIndex;
				}
			}
			WinConditionTool::getInstance()->BattleEndSetCondition();
			if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
				history->mArmyFuBenBattle->SetEndBossGainHp(history->m_iCurWarId);

			history->is_battle_win = true;
			ChessApplication::getInstance()->AddFunctionLayer(BattleEndLayer::create());

			history->game_state = GAME_STATE_BATTLE_END;
			{
				int i;
				for (i = 0; i < history->battleOwnNum; i++)
				{
					history->battleOwn[i]->recoverState();
				}
				for (i = 0; i < history->ownArmyNum; i++)
				{
					(&history->actors[history->ownArmy[i]])->recoverState();
				}
				for (i = 0; i < history->battleFriendNum; i++)
				{
					history->battleFriend[i]->recoverState();
				}
				for (i = 0; i < history->battleEnemyNum; i++)
				{
					history->battleEnemy[i]->recoverState();
				}
			}
			history->clearSceneData();
			if (history->elem_list->size() != 0)
				history->elem_list->removeAllElements();
			battle_state = BATTLE_STATE_BATTLE_END;
			if (BattleReplay::getInstance()->getCurStatus() != REPLAY_STATUS_READY_TO_REPLAY) {
				if (history->is_battle_win)
					StatConfig::onCompletedMission(__String::createWithFormat("warId:%d", history->m_iCurWarId)->getCString());
				else
					StatConfig::onFailedMission(__String::createWithFormat("warId:%d", history->m_iCurWarId)->getCString(), "TODO");

			}

		}
		break;
	case BATTLE_STATE_ENDING_LOSE:
	case BATTLE_STATE_EXIT:
	{
		if (battle_state == BATTLE_STATE_ENDING_LOSE)
		{
			if (history->m_iCurWarId == 94 || history->m_iCurWarId == 95 )
			{

				history->m_bGuide[10] = true;
			}
			history->is_battle_win = true;
			ChessApplication::getInstance()->AddFunctionLayer(BattleEndLayer::create(true));
		}
		else
		{
			history->is_battle_win = false;
		}
		history->game_state = GAME_STATE_BATTLE_END;
		{
			int i;
			for (i = 0; i < history->battleOwnNum; i++)
			{
				history->battleOwn[i]->recoverState();
				if (history->battleOwn[i]->controlled_by_enemy)
					history->battleOwn[i--]->delete_controlled_by_enemy();
			}
			for (i = 0; i < history->ownArmyNum; i++)
			{
				(&history->actors[history->ownArmy[i]])->recoverState();
			}
			for (i = 0; i < history->battleFriendNum; i++)
			{
				history->battleFriend[i]->recoverState();
			}
			for (i = 0; i < history->battleEnemyNum; i++)
			{
				history->battleEnemy[i]->recoverState();
			}
		}
		history->clearSceneData();
		if (history->elem_list->size() != 0)
			history->elem_list->removeAllElements();
		battle_state = BATTLE_STATE_BATTLE_END;
		WinConditionTool::getInstance()->BattleFailedSetDate();//战斗失败退还物品等
		if (BattleReplay::getInstance()->getCurStatus() != REPLAY_STATUS_READY_TO_REPLAY) {
			if (history->is_battle_win)
				StatConfig::onCompletedMission(__String::createWithFormat("warId:%d", history->m_iCurWarId)->getCString());
			else
				StatConfig::onFailedMission(__String::createWithFormat("warId:%d", history->m_iCurWarId)->getCString(), "TODO");

		}

	}
		break;
	case BATTLE_STATE_BATTLE_END:
	{
		BattleReplay::getInstance()->stopSaveReplay();
		if (history->is_battle_win)
			break;
		{
			map<int, int> mUpdataEffectList;
			int iActirGqLimit = trData->m_mNetWarInfo[history->m_iCurWarId]->iGkLv;
			if (iActirGqLimit == 2 || iActirGqLimit == 3)
			{
				for (int i = 0; i < history->ownArmyNum; i++)
				{
					int iActorOldLevel = history->actors[history->ownArmy[i]].GetActorOldLv();
					if (iActorOldLevel > 0)
					{
						history->actors[history->ownArmy[i]].SetActorLv(iActorOldLevel);
						history->actors[history->ownArmy[i]].SetActorOldLv(0);
					}

					int iActorOldQuality = history->actors[history->ownArmy[i]].actor_old_quality;
					if (iActorOldQuality)
					{
						history->actors[history->ownArmy[i]].actor_quality = iActorOldQuality;
						history->actors[history->ownArmy[i]].actor_old_quality = 0;
					}

					if (iActorOldLevel || iActorOldQuality)
					{
						history->actors[history->ownArmy[i]].InitHPAndMPAbility();
						history->actors[history->ownArmy[i]].InitFiveBaseAbility();
						history->actors[history->ownArmy[i]].UpdataGoodEffect();
						history->actors[history->ownArmy[i]].UpdateSkills();
						mUpdataEffectList[history->ownArmy[i] + 1] = 1;
					}

				}
			}
			if (history->getRatioListMap().size() > 0)
			{
				map<int, int> mTRatioList = history->getRatioListMap();
				history->clearRatioList();

				for (map<int, int>::iterator eIt = mTRatioList.begin(); eIt != mTRatioList.end(); eIt++)
				{
					if (!mUpdataEffectList.count(eIt->first))
						history->actors[eIt->first - 1].UpdataGoodEffect();
				}
			}
		}

		SAFE_DELETE_IMAGE(history->imgBG);
		ChessSetWindowSize(SCREEN_SIZE_REDE);

		scr->resetWindow();
		chess->removeScreen(scr, 0);
		history->game_state = GAME_STATE_REDE;
		history->mScriptValidate = -1;
		if (history->m_iBattleType == BATTLE_HEROIC)
		{
			ChessApplication::getInstance()->AddFunctionLayer(HeroicAmbitionLayer::create());
		}
		else if (history->m_iBattleType == BATTLE_ROMANCE)
		{
			ChessApplication::getInstance()->AddFunctionLayer(RomanceDetailsLayer::create(trData->m_mNetWarInfo[history->m_iCurWarId]->iMapIndex));
		}
		else if (history->m_iBattleType == BATTLE_ACTSCENE)
		{
			if (history->m_iActivityEntry == 2)
			{
				ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
			}
			else
			{
				ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			}
		}
		else if (history->m_iBattleType == BATTLE_DAILY)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			ChessApplication::getInstance()->AddFunctionLayer(DailySceneLayer::create());
		}
		else if (history->m_iBattleType == BATTLE_LADDER)
		{
			history->mLadderEActor.clear();
			BattleLadder::getInstance()->ClearBattleActorName();
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			ChessApplication::getInstance()->AddFunctionLayer(LadderLayer::create());
		}
		else if (history->m_iBattleType == BATTLE_WOLFCITY)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			ChessApplication::getInstance()->AddFunctionLayer(DailySceneLayer::create(1));
		}
		else if (history->m_iBattleType == BATTLE_IMAGE)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			ChessApplication::getInstance()->AddFunctionLayer(DailySceneLayer::create(2));
		}
		else if (history->m_iBattleType == BATTLE_GREATBATTLE)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			ChessApplication::getInstance()->AddFunctionLayer(GreatBattleLayer::create());
		}
		else if (history->m_iBattleType == BATTLE_CLANARMYBATTLE)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			GetNetDateTool::getInstance()->SendClanInfoData();
		}
		else if (history->m_iBattleType == BATTLE_PVP)
		{
			ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			PVPData::getInstance()->startPVP();
		}
		else
		{
			if (history->m_iCurWarId < LessGuideStatusWarID && history->m_iCurWarId != 94 && history->m_iCurWarId != 95) {
				ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			}
			else if ((history->m_iCurWarId == 100 || history->m_iCurWarId == 101) && !history->mMapDateInfo->isPassed(102))
			{
				ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
			}
			else if (history->m_iCurWarId == 94)
			{
				if (history->mMapDateInfo->isPassed(95))
				{
					ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
				}
				else
				{
					if (history->m_bGuide[10] == false)
					{
						ITools::getInstance()->startFight(95, BATTLE_SIXWAR, 0);
					}
					else
					{
						history->m_bGuide[10] = false;
						ITools::getInstance()->startFight(94, BATTLE_SIXWAR, 0);
					}
				}




			}
			else if (history->m_iCurWarId == 95)
			{
				if (history->mMapDateInfo->isPassed(96))
				{
					ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
				}
				else
				{
					if (history->m_bGuide[10] == false)
					{
						ITools::getInstance()->startFight(96, BATTLE_SIXWAR, 0);
					}
					else
					{
						history->m_bGuide[10] = false;
						ITools::getInstance()->startFight(95, BATTLE_SIXWAR, 0);
					}
				}

			}
			else {
				ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create(-1, trData->m_mNetWarInfo[history->m_iCurWarId]->iModel));
			}
		}
		if (history->m_iOpenId)
		{
			ChessApplication::getInstance()->AddFunctionLayer(NoticeActOpenLayer::create(history->m_iOpenId));
			history->m_iOpenId = 0;
		}
	}
        break;
	case BATTLE_STATE_SHOW_TAKE_TURNS:
	{
		battle_state = BATTLE_STATE_ACTOR_RECOVER;
		handler.GenerateScriptListActorState(ACTOR_RELATION_OWN, history);
		handler.GenerateScriptListActorState(ACTOR_RELATION_ENEMY, history);
	}
	break;
	}
}
void BattleControler::ShowMoveRegion()
{
	if (!auto_run)
		battle_state = BATTLE_STATE_SHOW_ATTACK_MOVE_REGION;
	else
		battle_state = BATTLE_STATE_SHOW_MOVE_REGION;

	if(auto_run !=0)
	{
		need_frame = TIMER_SHOW_MOVE_REGION;
	}
	else
	{
		ai.GenerateMoveList(history->cur_actor);
	}
	if (battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION&&history->cur_actor->relation == 0 && !history->cur_actor->action_finished&&history->cur_actor->actor_only_move!=2)
	{
		ai.GenerateAttackGainList(BattleActor::move_index);
	}
}
void BattleControler::CancelMoveRegion()
{
	battle_state = BATTLE_STATE_WAIT_COMMAND;
	BattleActor::clearMoveList();
}
void BattleControler::MoveActor(int i)
{
	battle_state = BATTLE_STATE_ACTOR_MOVE;
	BattleActor::setMoveIndex(i);
	BattleActorPtr actor = getBattleActor(history->cur_actor->actor_id);
	need_frame = actor->setState(ACTOR_STATE_MOVE);
	last_x = history->cur_actor->x;
	last_y = history->cur_actor->y;
	m_iLastDir = history->cur_actor->direction;
}

void BattleControler::DispatchActorMove(int i)
{
	battle_state = BATTLE_STATE_SHOW_DISPATCH_MOVE;
	BattleActor::setMoveIndex(i);
	BattleActorPtr actor = getBattleActor(history->cur_actor->actor_id);
	need_frame = actor->setState(ACTOR_STATE_MOVE);
	last_x = history->cur_actor->x;
	last_y = history->cur_actor->y;
	m_iLastDir = history->cur_actor->direction;
}

void BattleControler::CancelMoveActor()
{
	history->cur_actor->x = last_x;
	history->cur_actor->y = last_y;
	history->cur_actor->direction = m_iLastDir;
	if (!auto_run)
		battle_state = BATTLE_STATE_SHOW_ATTACK_MOVE_REGION;
	else
		battle_state = BATTLE_STATE_SHOW_MOVE_REGION;

	ClearGain();
	if (battle_state == BATTLE_STATE_SHOW_ATTACK_MOVE_REGION&&history->cur_actor->actor_only_move != 2)
	{
		BattleActor::setMoveIndex(0);
		ai.GenerateAttackGainList(0);
	}
}
void BattleControler::SelectAction(int action)
{
	switch(action)
	{
	case 0:
		battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
		ai.need_siege = 0;
		if (ai.curActor)
		{
			ai.curActor->siege_state = false;
		}
		scr->ptrSelectAction->setVisible(true);
		ai.GenerateAttackGainList(BattleActor::move_index);
		
		break;
	case 1:
		ai.need_siege = 0;
		if (ai.curActor)
		{
			ai.curActor->siege_state = false;
		}
		scr->mFunctionLayer = BSelectGoodsOrSkillLayer::create(this, BSELECT_SHOW_SKILL);
		ChessApplication::getInstance()->AddFunctionLayer(scr->mFunctionLayer);
		break;
	case 2:
		ai.need_siege = 0;
		if (ai.curActor)
		{
			ai.curActor->siege_state = false;
		}
		scr->mFunctionLayer = BSelectGoodsOrSkillLayer::create(this, BSELECT_SHOW_GOODS);
		ChessApplication::getInstance()->AddFunctionLayer(scr->mFunctionLayer);
		break;
	case 3:
		ai.need_siege = 0;
		if (ai.curActor)
		{
			ai.curActor->siege_state = false;
		}
		if (ai.mostGain&&ai.mostGain->iRealKillNum > 0)
		{
			ai.mostGain->iRealKillNum = 0;
		}
		SetActionfinish();
		BattleActor::clearMoveList();
		battle_state = BATTLE_STATE_RUN_SCRIPT;
		handler.SetScriptList(1);
		break;
	case 4:
		ai.need_siege = 0;
		if (ai.curActor)
		{
			ai.curActor->siege_state = false;
		}
		CancelMoveActor();
		break;
	case 5:
		scr->mFunctionLayer = BSelectGoodsOrSkillLayer::create(this, BSELECT_SHOW_SKILL);
		ChessApplication::getInstance()->AddFunctionLayer(scr->mFunctionLayer);
		break;
	}
}
void BattleControler::SelectSkill(SkillAttrPtr skill)
{

	if(skill == NULL)
	{
		scr->ptrSelectAction->setVisible(true);
		curSkill = NULL;
	}
	else if(trData->hitArea[skill->use_region].length!=-5)
	{
		curSkill = skill;
		battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
		ai.GenerateSkillGainList(skill,BattleActor::move_index);
	}
	else
	{
		curSkill = skill;

		ai.GenerateSkillGainList(skill,BattleActor::move_index);
		if (ai.gainList->size() != 0)
		{
			ai.mostGain = ai.gainList->removeElementAt(0);
			ActorDoAction();
		}
		else
		{
			scr->ptrSelectAction->setVisible(true);
			curSkill = NULL;
			scr->m_spLabel->updateSprite(LL("cidiwufashiyong"), LABEL_SHORT, 0.8f);
			playSound(SOUND_MOUSE_CLICK_ERROR);
		}
	}
}
void BattleControler::SelectGood(GoodsItem* good)
{

	if(good == NULL)
	{
		scr->ptrSelectAction->setVisible(true);
		curGood = NULL;
	}
	else
	{
		curGood = good;
		battle_state = BATTLE_STATE_SHOW_ATTACK_REGION;
		ai.GenerateGoodGainList(good,BattleActor::move_index);
	}
}
ActionSingleGainPtr BattleControler::getSingleGain(int x,int y,int setMost)
{
	ActionSingleGainPtr curSingleGain = NULL;
	for(int i = 0;i<ai.gainList->size();i++)
	{
		ActionGainPtr curGain = ai.gainList->elementAt(i);
		if(curGain->hitPoint.x == x&&curGain->hitPoint.y == y)
		{
			if(setMost)
			{
				ai.mostGain = curGain;
				ai.gainList->removeElementAt(i);
			}
			curSingleGain = curGain->gains->elementAt(0);
		}
	}
	return curSingleGain;
}

void BattleControler::RoundStartFriend()
{
	history->curActionSide = ACTOR_RELATION_FRIEND;
	history->cur_actor = NULL;
	battle_state = BATTLE_STATE_RUN_SCRIPT;
	handler.SetScriptList(1);
}

void BattleControler::RoundStartFriendNew()
{
    if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_SAVE) {
        BattleReplay::getInstance()->saveBattleAI(&ai);
    }
    else if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
        BattleReplay::getInstance()->getBattleAI();

    RoundStartFriend();
}

void BattleControler::ActorDoAction()
{
	battle_state = BATTLE_STATE_DO_ACTION;
	ai.induct_num = 0;//初始化当前引导次数
	ai.bravely_fight_num = 0;//初始化当前奋战次数
	HistoryActorPtr temp = history->cur_actor;

	//HistoryActorPtr oppo_actor = ((ActionSingleGainPtr)ai.mostGain->gains->elementAt(0))->oppo_actor;

	HistoryActorPtr oppo_actor = NULL;

	DList<ActionSingleGain>* tempVector = ai.mostGain->gains;

	if (tempVector != NULL){
		oppo_actor = ((ActionSingleGainPtr)tempVector->elementAt(0))->oppo_actor;
	}

	if (ai.mostGain->need_back && oppo_actor != NULL && oppo_actor->good_xianshou == 100)//职业特性先手攻击 1 是否会被先手攻击的反击
	{

		ai.need_xianshou = 1;
		ai.is_xianshou = 1;

		ai.curActor = oppo_actor;

		ai.ClearGain();

		ai.mostGain = ai.GetAttackGain(ai.curActor->x, ai.curActor->y, temp->x, temp->y);
		ai.mostGain->need_back = 1;
		temp = ai.curActor;
		tempVector = ai.mostGain->gains;
		if (tempVector != NULL){
			oppo_actor = ((ActionSingleGainPtr)tempVector->elementAt(0))->oppo_actor;
		}
	}
	else{
		ai.need_xianshou = 0;//重置先手攻击
	}

	if(ai.mostGain->skill==NULL&&ai.mostGain->good == NULL)//不是放技能将进行攻击计算二次攻击和反击的反击可能性
	{
		ai.GenerateAttackType();//生成攻击类型（普通攻击、双击等）
	}
	else if (ai.mostGain->skill)//放技能时处理技能特效
	{
        ai.GenerateSkillType(ai.mostGain->skill);
		ai.GenerateSkillType(ai.mostGain);
		if (temp->getIsCopySkill())
		{
			ai.iActiveSkillId = ai.mostGain->skill->skill_id;
		}
    }

	if (ai.mostGain->skill != NULL&&ai.mostGain->skill->seid == 71)
		ai.skill_double_hit = 3;
	if (ai.mostGain->skill != NULL&&ai.mostGain->skill->seid == 102)
	{
		ai.skill_bravely_fight[0] = 1;
		ai.skill_bravely_fight[1] = 0;
		ai.skill_bravely_fight[2] = oppo_actor->actor_id;
		ai.skill_bravely_fight[3] = ai.mostGain->skill->skill_id;
	}
	int oppo_actor_pursuit_x = oppo_actor->x;
	int oppo_actor_pursuit_y = oppo_actor->y;

	if (temp->good_effects[13] && oppo_actor != NULL&&ai.mostGain->good == NULL&&ai.mostGain->skill == NULL)//突击攻击
	{
		needAssault(temp ,oppo_actor,oppo_actor_pursuit_x, oppo_actor_pursuit_y);//此处判断是否会被击退，oppo_actor_pursuit_x, oppo_actor_pursuit_y是被敌方所处位置坐标，用于追击攻击
	}
	else if (temp->good_effects[BERA_HAND] && oppo_actor != NULL&&ai.mostGain->good == NULL&&ai.mostGain->skill == NULL)
	{
		if ((int)abs(temp->x - oppo_actor->x) < 2 && (int)abs(temp->y - oppo_actor->y) < 2)
		{
			int iPursuit_x = -1;
			int iPursuit_y = -1;
			ai.bear_hand_attack = -1;
			if (ActorAssault(oppo_actor, temp, iPursuit_x, iPursuit_y))
			{
				oppo_actor_pursuit_x = iPursuit_x;
				oppo_actor_pursuit_y = iPursuit_y;
				ai.bear_hand_attack = 1;
			}
		}
	}
	else if (temp->good_effects[TWOORDER_SPIKE] && oppo_actor && !oppo_actor->isHadSpecial(TYPE743) && ai.mostGain->good == NULL&&ai.mostGain->skill == NULL)
	{
		if ((int)abs(temp->x - oppo_actor->x) < 2 && (int)abs(temp->y - oppo_actor->y) < 2)
		{
			if (!(oppo_actor->getSpecialInt(TYPE703) || (rand() % 100) < oppo_actor->getSpecialInt(TYPE722)))
			{
				if (ActorAssault(temp, oppo_actor, oppo_actor_pursuit_x, oppo_actor_pursuit_y,1,true) && ActorAssault(temp, oppo_actor, oppo_actor_pursuit_x, oppo_actor_pursuit_y, 2,true))
				{
					ai.two_order_spike = 2;
				}
				else if (ActorAssault(temp, oppo_actor, oppo_actor_pursuit_x, oppo_actor_pursuit_y,1,true))
				{
					ai.two_order_spike = 1;
				}
			}
		}
	}
	else if (temp->good_effects[NEW_ASSAULT] && oppo_actor != NULL&&ai.mostGain->good == NULL&&ai.mostGain->skill == NULL)
	{
		if ((int)abs(temp->x - oppo_actor->x) < 2 && (int)abs(temp->y - oppo_actor->y) < 2)
		{
			if (ActorAssault(temp, oppo_actor, oppo_actor_pursuit_x, oppo_actor_pursuit_y))
			{
				ai.new_assault = 1;
			}
		}
	}

	ai.GenerateRealGain();
	ai.gainList->removeElement(ai.mostGain);
	handler.GenerateScriptListFromAI(ai);
	ClearGain();

	//type755
	if (history->cur_actor->isHadSpecial(TYPE755) && history->cur_actor->mSeidAddGain[TYPE755] < 10 && ai.mostGain->good == NULL&&ai.mostGain->skill == NULL)
	{
		history->cur_actor->mSeidAddGain[TYPE755]++;
	}


	/////追击攻击（下面）
	if (oppo_actor != NULL && ((ActionSingleGainPtr)ai.mostGain->gains->elementAt(0))->gain_hp + oppo_actor->GetCurHp() > 0 &&
		ai.mostGain->good == NULL&&ai.mostGain->skill == NULL && !ai.need_pursuit_attack)
	{
		if (ai.need_assault_attack != 1)//不会被突击攻击击退时
		{
			oppo_actor_pursuit_x = oppo_actor->x;
			oppo_actor_pursuit_y = oppo_actor->y;
		}
		ai.need_pursuit_attack_actor[0] = oppo_actor->actor_id;
		int i, j, attack_id, x, y;
		switch (ai.curActor->relation)
		{
		case 0:
		case 1:
			for (i = 0; i < history->battleOwnNum; i++)
			{
				if (history->battleOwn[i]->isHadSpecial(TYPE114))
				{
					map<int, int> mSpecial = history->battleOwn[i]->getSpecialMap(TYPE114);
					if(history->battleOwn[i]->good_pursuit_attack_num >= mSpecial[1])
						continue;

					attack_id = history->battleOwn[i]->getAttackAreaId();
					if (trData->hitArea[attack_id].length >= 0)
					{
						for (j = 0; j < trData->hitArea[attack_id].length; j++)
						{
							x = history->battleOwn[i]->x + trData->hitArea[attack_id].pointlist[j].x;
							y = history->battleOwn[i]->y + trData->hitArea[attack_id].pointlist[j].y;
							if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y) && !history->battleOwn[i]->state_puzzle &&!history->battleOwn[i]->state_blind && !history->battleOwn[i]->state_obfuscation && (history->battleOwn[i]->actor_id != ai.curActor->actor_id))
							{
								ai.need_pursuit_attack++;
								if (ai.need_pursuit_attack < 9)
									ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleOwn[i]->actor_id;
							}
						}
					}
					else if(trData->hitArea[attack_id].length == -1)
					{
						if(!history->battleOwn[i]->state_puzzle &&!history->battleOwn[i]->state_blind && !history->battleOwn[i]->state_obfuscation && (history->battleOwn[i]->actor_id != ai.curActor->actor_id))
						{
							ai.need_pursuit_attack++;
							if(ai.need_pursuit_attack < 9)
								ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleOwn[i]->actor_id;
						}
					}
				}
			}
			for (i = 0; i < history->battleFriendNum; i++)
			{
				if(history->battleFriend[i]->isHadSpecial(TYPE114))
				{
					map<int, int> mSpecial = history->battleFriend[i]->getSpecialMap(TYPE114);
					if(history->battleFriend[i]->good_pursuit_attack_num >= mSpecial[1])
						continue;

					attack_id = history->battleFriend[i]->getAttackAreaId();
					if (trData->hitArea[attack_id].length >= 0)
					{
						for (j = 0; j < trData->hitArea[attack_id].length; j++)
						{
							x = history->battleFriend[i]->x + trData->hitArea[attack_id].pointlist[j].x;
							y = history->battleFriend[i]->y + trData->hitArea[attack_id].pointlist[j].y;
							if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y) && !history->battleFriend[i]->state_puzzle &&!history->battleFriend[i]->state_blind && !history->battleFriend[i]->state_obfuscation && (history->battleFriend[i]->actor_id != ai.curActor->actor_id))
							{
								ai.need_pursuit_attack++;
								if (ai.need_pursuit_attack < 9)
									ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleFriend[i]->actor_id;
							}
						}
					}
					else if (trData->hitArea[attack_id].length == -1)
					{
						if (!history->battleFriend[i]->state_puzzle &&!history->battleFriend[i]->state_blind && !history->battleFriend[i]->state_obfuscation && (history->battleFriend[i]->actor_id != ai.curActor->actor_id))
						{
							ai.need_pursuit_attack++;
							if (ai.need_pursuit_attack < 9)
								ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleFriend[i]->actor_id;
						}
					}
				}
			}
			break;
		case 2:
			for (i = 0; i < history->battleEnemyNum; i++)
			{
				if(history->battleEnemy[i]->isHadSpecial(TYPE114))
				{
					map<int, int> mSpecial = history->battleEnemy[i]->getSpecialMap(TYPE114);
					if(history->battleEnemy[i]->good_pursuit_attack_num >= mSpecial[1])
						continue;

					attack_id = history->battleEnemy[i]->getAttackAreaId();
					if (trData->hitArea[attack_id].length >= 0)
					{
						for (j = 0; j < trData->hitArea[attack_id].length; j++)
						{
							x = history->battleEnemy[i]->x + trData->hitArea[attack_id].pointlist[j].x;
							y = history->battleEnemy[i]->y + trData->hitArea[attack_id].pointlist[j].y;
							if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y) && !history->battleEnemy[i]->state_puzzle&&!history->battleEnemy[i]->state_blind&&
								!history->battleEnemy[i]->state_obfuscation && (history->battleEnemy[i]->actor_id != ai.curActor->actor_id))
							{
								ai.need_pursuit_attack++;
								if (ai.need_pursuit_attack < 9)
									ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleEnemy[i]->actor_id;
							}
						}
					}
					else if(trData->hitArea[attack_id].length == -1)
					{
						if(!history->battleEnemy[i]->state_puzzle&&!history->battleEnemy[i]->state_blind&&!history->battleEnemy[i]->state_obfuscation && (history->battleEnemy[i]->actor_id != ai.curActor->actor_id))
						{
							ai.need_pursuit_attack++;
							if(ai.need_pursuit_attack < 9)
								ai.need_pursuit_attack_actor[ai.need_pursuit_attack] = history->battleEnemy[i]->actor_id;
						}
					}
				}
			}
			break;

		}
	}

	//////////////追击攻击（上面）

	//////////////法术追击
	if (oppo_actor != NULL && ((ActionSingleGainPtr)ai.mostGain->gains->elementAt(0))->gain_hp + oppo_actor->GetCurHp() > 0 &&
		ai.mostGain->good == NULL/*&&ai.mostGain->skill == NULL */&& !ai.skill_need_follow)
	{
		//法术追击邂逅刀轮神技闪退BUG临时处理
		if (oppo_actor == history->cur_actor)
		{
			oppo_actor = nullptr;
			for (int i = 1; i < ai.mostGain->gains->size(); i++)
			{
				if (ai.mostGain->gains->elementAt(i))
				{
					HistoryActor* pTempActor = ai.mostGain->gains->elementAt(i)->oppo_actor;
					if (ai.mostGain->gains->elementAt(0)->gain_hp + pTempActor->GetCurHp() > 0)
					{
						oppo_actor = pTempActor;
						break;
					}
				}
			}
		}

		if (oppo_actor)
		{
			if (ai.need_assault_attack != 1)//不会被突击攻击击退时
			{
				oppo_actor_pursuit_x = oppo_actor->x;
				oppo_actor_pursuit_y = oppo_actor->y;
			}
			ai.skill_need_follow_actor[0] = oppo_actor->actor_id;
			int i, j, attack_id, x, y, temp_skill_id;
			switch (ai.curActor->relation)
			{
			case 0:
				for (i = 0; i < history->battleOwnNum; i++)
				{
					if (history->battleOwn[i]->hide != 1 && (history->battleOwn[i]->good_effects[22] && ai.mostGain->skill == NULL
						|| (history->battleOwn[i]->isHadSpecial(TYPE767) && (ai.mostGain->skill == NULL || ai.mostGain->skill->type <= 2))))
					{
						if (history->battleOwn[i]->isHadSpecial(TYPE767))
						{
							temp_skill_id = history->battleOwn[i]->getSpecialMap(TYPE767)[1] - 1;
							if (history->battleOwn[i]->good_skill_pursuit_attack_num >= history->battleOwn[i]->getSpecialMap(TYPE767)[2])
								continue;
						}
						else
						{
							temp_skill_id = trData->m_mSeid[history->battleOwn[i]->good_effects[22]]->special_effe[0] - 1;
						}
						attack_id = trData->skills[temp_skill_id].use_region;
						if (trData->hitArea[attack_id].length >= 0)
						{
							for (j = 0; j < trData->hitArea[attack_id].length; j++)
							{
								x = history->battleOwn[i]->x + trData->hitArea[attack_id].pointlist[j].x;
								y = history->battleOwn[i]->y + trData->hitArea[attack_id].pointlist[j].y;
								if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y) && !history->battleOwn[i]->state_puzzle &&
									!history->battleOwn[i]->state_obfuscation &&
									(history->battleOwn[i]->actor_id != ai.curActor->actor_id))
								{
									ai.skill_need_follow++;
									if (ai.skill_need_follow < 9)
										ai.skill_need_follow_actor[ai.skill_need_follow] = history->battleOwn[i]->actor_id;
								}
							}
						}
						else if (trData->hitArea[attack_id].length == -1) {
							if (!history->battleOwn[i]->state_puzzle && !history->battleOwn[i]->state_obfuscation && !history->battleOwn[i]->state_stun &&
								(history->battleOwn[i]->actor_id != ai.curActor->actor_id) && history->battleOwn[i]->GetCurHp() > 0)
							{
								ai.skill_need_follow++;
								if (ai.skill_need_follow < 9)
									ai.skill_need_follow_actor[ai.skill_need_follow] = history->battleOwn[i]->actor_id;
							}
						}
					}
				}
				break;
			case 2:
				for (i = 0; i < history->battleEnemyNum; i++)
				{
					if (history->battleEnemy[i]->hide != 1 && (history->battleEnemy[i]->good_effects[22] && ai.mostGain->skill == NULL
						|| (history->battleEnemy[i]->isHadSpecial(TYPE767) && (ai.mostGain->skill == NULL || ai.mostGain->skill->type <= 2))))
					{
						if (history->battleEnemy[i]->isHadSpecial(TYPE767))
						{
							temp_skill_id = history->battleEnemy[i]->getSpecialMap(TYPE767)[1] - 1;
							if (history->battleEnemy[i]->good_skill_pursuit_attack_num >= history->battleEnemy[i]->getSpecialMap(TYPE767)[2])
								continue;
						}
						else
						{
							temp_skill_id = trData->m_mSeid[history->battleEnemy[i]->good_effects[22]]->special_effe[0] - 1;
						}
						attack_id = trData->skills[temp_skill_id].use_region;
						if (trData->hitArea[attack_id].length >= 0)
						{
							for (j = 0; j < trData->hitArea[attack_id].length; j++)
							{
								x = history->battleEnemy[i]->x + trData->hitArea[attack_id].pointlist[j].x;
								y = history->battleEnemy[i]->y + trData->hitArea[attack_id].pointlist[j].y;
								if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y) && !history->battleEnemy[i]->state_puzzle &&
									!history->battleEnemy[i]->state_obfuscation && (history->battleEnemy[i]->actor_id != ai.curActor->actor_id))
								{
									ai.skill_need_follow++;
									if (ai.skill_need_follow < 9)
										ai.skill_need_follow_actor[ai.skill_need_follow] = history->battleEnemy[i]->actor_id;
								}
							}
						}
						else if (trData->hitArea[attack_id].length == -1) {
							if (!history->battleEnemy[i]->state_puzzle && !history->battleEnemy[i]->state_obfuscation && !history->battleEnemy[i]->state_stun &&
								(history->battleEnemy[i]->actor_id != ai.curActor->actor_id) && history->battleEnemy[i]->GetCurHp() > 0)
							{
								ai.skill_need_follow++;
								if (ai.skill_need_follow < 9)
									ai.skill_need_follow_actor[ai.skill_need_follow] = history->battleEnemy[i]->actor_id;
							}
						}
					}
				}
				break;
			}
		}
	}
    if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_SAVE) {
        BattleReplay::getInstance()->saveBattleAI(&ai);
    }
    else if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
        BattleReplay::getInstance()->getBattleAI();
}

void BattleControler::needAssault(HistoryActorPtr temp_cur_actor, HistoryActorPtr temp_oppo_actor, int &x, int &y)//判断是否会被击退
{
	Seid *pSeid = trData->m_mSeid[temp_cur_actor->good_effects[13]];
	if (pSeid->special_effe_num > 1)
	{
		for (int i = 1; i<pSeid->special_effe_num; i++)
		{
			if (pSeid->special_effe[i] - 1 == temp_oppo_actor->occupation_id)
				return;
		}
	}
	int temp_x = 0, temp_y = 0,move_flag = 0;
	temp_x = temp_oppo_actor->x;
	temp_y = temp_oppo_actor->y;//对象坐标
	if ((int)abs((temp_cur_actor->x - temp_x))>1)//攻击只击退一格
	{
		temp_x -= ((temp_cur_actor->x - temp_x) / (int)abs((temp_cur_actor->x - temp_x)));
	}
	else
	{
		temp_x -= (temp_cur_actor->x - temp_x);
	}
	if ((int)abs((temp_cur_actor->y - temp_y))>1)
	{
		temp_y -= ((temp_cur_actor->y - temp_y) / (int)abs((temp_cur_actor->y - temp_y)));
	}
	else
	{
		temp_y -= (temp_cur_actor->y - temp_y);
	}
	ai.need_assault_attack = -1;//默认被挡住
	if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{
	
		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if (oppoActor != NULL || history->HasElem(temp_x, temp_y, temp_cur_actor->occupation_id) || trData->occupation_landmove_info[temp_cur_actor->occupation_id][history->map_array[temp_y][temp_x]] == 255)
				move_flag = MAP_NO_REACH;
			else
				move_flag = MAP_CAN_MOVE;

			if (move_flag == MAP_CAN_MOVE)
			{
			ai.need_assault_attack = 1;//可以被击退
			x = temp_x;
			y = temp_y;
			}
	}
	
}
void BattleControler::needAssault(HistoryActorPtr temp_cur_actor, HistoryActorPtr temp_oppo_actor)//判断是否会被击退
{
	Seid *pSeid = trData->m_mSeid[temp_cur_actor->good_effects[13]];
	if (pSeid->special_effe_num > 1)
	{
		for (int i = 1; i<pSeid->special_effe_num; i++)
		{
			if (pSeid->special_effe[i] - 1 == temp_oppo_actor->occupation_id)
				return;
		}
	}
	int temp_x = 0, temp_y = 0, move_flag = 0;
	temp_x = temp_oppo_actor->x;
	temp_y = temp_oppo_actor->y;
	if ((int)abs((temp_cur_actor->x - temp_x))>1)//攻击只击退一格
	{
		temp_x -= ((temp_cur_actor->x - temp_x) / (int)abs((temp_cur_actor->x - temp_x)));
	}
	else
	{
		temp_x -= (temp_cur_actor->x - temp_x);
	}
	if ((int)abs((temp_cur_actor->y - temp_y))>1)
	{
		temp_y -= ((temp_cur_actor->y - temp_y) / (int)abs((temp_cur_actor->y - temp_y)));
	}
	else
	{
		temp_y -= (temp_cur_actor->y - temp_y);
	}
	ai.need_assault_attack = -1;//默认被挡住
	if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{

		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if (oppoActor != NULL || history->HasElem(temp_x, temp_y, temp_cur_actor->occupation_id) || trData->occupation_landmove_info[temp_cur_actor->occupation_id][history->map_array[temp_y][temp_x]] == 255)
			move_flag = MAP_NO_REACH;
		else
			move_flag = MAP_CAN_MOVE;

		if (move_flag == MAP_CAN_MOVE)
		{
			ai.need_assault_attack = 1;//可以被击退
		}
	}

}

int BattleControler::ActorAssault(HistoryActorPtr temp_cur_actor, HistoryActorPtr temp_oppo_actor, int &x, int &y, int step ,bool bOnlyPoint)
{
	if(!bOnlyPoint&&(temp_oppo_actor->getSpecialInt(TYPE703) || (rand() % 100) < temp_oppo_actor->getSpecialInt(TYPE722))) //被攻击不会后退
	{
		return 0;
	}

	int temp_x = 0, temp_y = 0, move_flag = 0;
	temp_x = temp_oppo_actor->x;
	temp_y = temp_oppo_actor->y;
	if ((int)abs(temp_cur_actor->x - temp_x) > 1)
	{
		temp_x -= ((temp_cur_actor->x - temp_x) / (int)abs(temp_cur_actor->x - temp_x))*step;
	}
	else
	{
		temp_x -= (temp_cur_actor->x - temp_x)*step;
	}
	if ((int)abs(temp_cur_actor->y - temp_y) > 1)
	{
		temp_y -= ((temp_cur_actor->y - temp_y) / (int)abs(temp_cur_actor->y - temp_y))*step;
	}
	else
	{
		temp_y -= (temp_cur_actor->y - temp_y)*step;
	}
	if ((temp_x >= 0 && temp_x < history->map_width) && (temp_y >= 0 && temp_y < history->map_height))
	{
		HistoryActorPtr oppoActor = history->FindActorPosition(temp_x, temp_y);
		if (oppoActor != NULL || history->HasElem(temp_x, temp_y, temp_cur_actor->occupation_id) || trData->occupation_landmove_info[temp_cur_actor->occupation_id][history->map_array[temp_y][temp_x]] == 255)
			move_flag = MAP_NO_REACH;
		else
			move_flag = MAP_CAN_MOVE;

		if (move_flag == MAP_CAN_MOVE)
		{
			x = temp_x;
			y = temp_y;
		}
	}
	return move_flag;
}

void BattleControler::processSkillBack() {
    ActionSingleGainPtr curSingleGain = (ActionSingleGainPtr)ai.mostGain->gains->elementAt(0);
    int skill_id = trData->m_mSeid[curSingleGain->oppo_actor->good_magic_back]->special_effe[0] - 1;
	HistoryActorPtr ptrPursuitActor = curSingleGain->oppo_actor;
	ptrPursuitActor->pursuit_skill_lv = curSingleGain->oppo_actor->GetActorLv();
    ai.GenerateGain_SkillType(&(trData->skills[skill_id]), curSingleGain->oppo_actor, history->cur_actor->x, history->cur_actor->y, SKILL_TYPE_BACK);
    handler.GenerateScriptListFromAI(ai);
	ptrPursuitActor->pursuit_skill_lv = 0;

}
bool BattleControler::canSkillBack(HistoryActorPtr oppo_actor)
{
    int skill_id = trData->m_mSeid[oppo_actor->good_magic_back]->special_effe[0] - 1;
    //检查反击是否在法术的攻击范围以内
    bool isSkillNeedBackInArea = false;
    int attack_id = trData->skills[skill_id].use_region;
    int oppo_actor_pursuit_x = history->cur_actor->x;
    int oppo_actor_pursuit_y = history->cur_actor->y;
    if (trData->hitArea[attack_id].length >= 0) {
        for (int j = 0; j < trData->hitArea[attack_id].length; j++) {
            int x = oppo_actor->x + trData->hitArea[attack_id].pointlist[j].x;
            int y = oppo_actor->y + trData->hitArea[attack_id].pointlist[j].y;
            if ((x == oppo_actor_pursuit_x) && (y == oppo_actor_pursuit_y)
                && (oppo_actor->actor_id != ai.curActor->actor_id)) {
                isSkillNeedBackInArea = true;
                break;
            } else {
                isSkillNeedBackInArea = false;
            }
        }
    } else if (trData->hitArea[attack_id].length == -1) {
        if (oppo_actor->actor_id != ai.curActor->actor_id) {
            isSkillNeedBackInArea = true;
        } else {
            isSkillNeedBackInArea = false;
        }
    }

	ai.skill_need_back = 0;
    //当mp不足、不在技能攻击范围内、天气不符合时，当反击发起者处于封禁、混乱、昏迷时，均不能反击
	if (isSkillNeedBackInArea && trData->skills[skill_id].BeingAbleToCastSpells(oppo_actor) && history->cur_actor->GetCurHp() > 0 && oppo_actor->GetCurHp() > 0 && history->cur_actor->state_stun == 0 && oppo_actor->GetCurMp() >= trData->skills[skill_id].GetMpConsume(oppo_actor) && oppo_actor->state_puzzle == 0 && oppo_actor->state_obfuscation == 0 && oppo_actor->state_stun == 0) {
        return true;
    }
    return false;
}
void BattleControler::Resetbust_style()
{
	for (int i = 0; i < trData->actor_num; i++)
		history->actors[i].bust_style = 0;
}

void BattleControler::SetActionfinish()
{
	BattleReplay::getInstance()->saveOrDeleteMove();

	HistoryActorPtr pDeadActor = history->getNextDeadActor();

	if(pDeadActor && pDeadActor->good_battle_relive && !pDeadActor->is_used_relive) //战场复活放在执行剧本前处理
	{
		ActorAttrPtr ptrActor = &trData->actors[pDeadActor->actor_id];
		if(pDeadActor->relation == 0 && history->m_iBattleType != BATTLE_PVP && strlen(ptrActor->retreat_word)>0 && strcmp(ptrActor->retreat_word, "null"))
		{
			scr->m_pTalkLayer->updateLayer(ptrActor->retreat_word, ptrActor->actor_id);
		}

		pDeadActor->recoverState();
		pDeadActor->initCopySkill();
		pDeadActor->is_used_relive = 1;
	}

	HistoryActorPtr pCurActor = history->cur_actor;
	if(!pCurActor->good_effects[36])
	{
		if(pCurActor->good_effects[41] && ai.mostGain && ai.mostGain->iRealKillNum)
		{
			if(pCurActor->iActionNum == -1)
			{
				pCurActor->iActionNum = pCurActor->good_effects[41];
			}
		}
		else
		{
			pCurActor->iActionNum = -1;
		}

		if(pCurActor->iActionNum > 0)
		{
			pCurActor->iActionNum--;
			if(pCurActor->good_twice_action)
			{
				pCurActor->iTwiceAction = 1;
			}
		}
		else if(pCurActor->good_twice_action && pCurActor->iTwiceAction == 0)
		{
			pCurActor->iTwiceAction = 1;
		}
 		else if (ai.mostGain&&ai.mostGain->skill&&ai.mostGain->skill->iNoCost&&pCurActor->iTwiceAction == 0 && BattleActor::move_index == 0)
 		{
 			pCurActor->iTwiceAction = 1;
 		}
		else if (pCurActor->actor_only_move == 1)
		{
			pCurActor->actor_only_move = 2;
		}
		else
		{
			if (pCurActor->isHadSpecial(TYPE738) && !BattleControler::auto_run) //随步，剩余移动力可再次移动
			{
				if (pCurActor->m_iRestSpeed == 0 && BattleActor::moveList)
				{
					int iUsedSpeed = 0;
					MovePath* ptrPath = BattleActor::moveList->elementAt(BattleActor::move_index);
					if(ptrPath)
					{
						iUsedSpeed = ptrPath->m_iUsedSpeed;
					}
					pCurActor->m_iRestSpeed = pCurActor->GetActorSpeed() - iUsedSpeed;
				}
				else
				{
					pCurActor->m_iRestSpeed = 0;
				}
			}

			if(pCurActor->m_iRestSpeed)
			{
				pCurActor->actor_only_move = 2;
			}
			else
			{
				pCurActor->action_finished = 1;
				pCurActor->actor_only_move = 0;
			}
		}
	}

	if (pCurActor->getIsCopySkill())
	{
		if (ai.mostGain && ai.iActiveSkillId != -1)
		{
			pCurActor->updateCopySkill(ai.iActiveSkillId + 1);
		}
		pCurActor->setIsCopySkill(false);
		ai.iActiveSkillId = -1;
	}
    if (battle_state != BATTLE_STATE_DO_ACTION) {
        if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_SAVE) {
            BattleReplay::getInstance()->saveBattleAI(&ai);
        }
        else if (BattleReplay::getInstance()->getCurStatus() == REPLAY_STATUS_REPLAY)
            BattleReplay::getInstance()->getBattleAI();
    }
}
int BattleControler::GetActorChangeWeatherId()
{
	for (int i = 0; i < history->battleOwnNum; i++)
	{
		HistoryActorPtr ptrActor = history->battleOwn[i];
		if (ptrActor->hide != 1)
		{
			int ret = ptrActor->good_effects[CHANGE_WEATHER];
			if (ret >= 1 && ret <= 5)
				return ret - 1;
		}
	}
	return -1;
}

void BattleControler::startNextRound()
{
	for(int i = 0; i < history->battleOwnNum; i++)
	{
		history->battleOwn[i]->updateRoundState();
	}

	for(int i = 0; i < history->battleFriendNum; i++)
	{
		history->battleFriend[i]->updateRoundState();
	}

	for(int i = 0; i < history->battleEnemyNum; i++)
	{
		history->battleEnemy[i]->updateRoundState();
	}
}

void BattleControler::autoRunAction()
{
	auto_run = history->m_bAutoRun;

	if (!history->getNextActiveOwn() && !history->getNextActiveFriend() && !history->getNextActiveEnemy())
	{
		need_round_add = 1;//标示更新标题
		curSkill = NULL;
		curGood = NULL;
		BattleActor::clearMoveList();
		startNextRound();
		battle_state = BATTLE_STATE_SHOW_TAKE_TURNS;
		return;
	}

	while(1)
	{
		switch(history->curActionSide)
		{
		case ACTOR_RELATION_OWN:
			history->cur_actor = history->getNextActiveOwn();
			break;
		case ACTOR_RELATION_FRIEND:
			history->cur_actor = history->getNextActiveFriend();
			break;
		case ACTOR_RELATION_ENEMY:
			history->cur_actor = history->getNextActiveEnemy();
			break;
		}

		if(history->cur_actor != NULL)
		{
			ai.GenerateMoveList(history->cur_actor);//获取到移动路径
			ai.GenerateMostGain();//获取到最优攻击
			curSkill = ai.mostGain->skill;
			if(ai.mostGain->moveIndex == -1 || (ai.mostGain->moveIndex == 0 && (ai.mostGain->gains == NULL || ai.mostGain->gains->size() == 0)))
			{
				SetActionfinish();
				BattleActor::clearMoveList();
				history->cur_actor = NULL;
			}
			else
			{
				history->CheckPosActor(history->cur_actor);//自动移动屏幕焦点
				scr->autopointClicked((history->cur_actor->xx() - history->scr_x), (history->cur_actor->yy() - history->scr_y));
				break;
			}
		}
		else
		{
			changeActionSide();
			break;
		}
	}
}

void BattleControler::changeActionSide()
{
	if(history->m_bTakeTurns)
	{
		switch(history->curActionSide)
		{
		case ACTOR_RELATION_OWN:
			history->curActionSide = ACTOR_RELATION_FRIEND;
			break;
		case ACTOR_RELATION_FRIEND:
			history->curActionSide = ACTOR_RELATION_ENEMY;
			break;
		case ACTOR_RELATION_ENEMY:
			history->curActionSide = ACTOR_RELATION_OWN;
			break;
		}
	}
}

void BattleControler::setActorLastXAndY(int lastX, int lastY)
{
	last_x = lastX;
	last_y = lastY;
}