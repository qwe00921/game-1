#include "Common.h"
#include "main.h"
#include "MapElem.h"
#include "StatConfig.h"
#include "ActionLayer.h"
#include "RedeScreen.h"
#include "BattleScreen.h"
#include "MapGenerate.h"
#include "Defines.h"
#include "PopLayer.h"
#include "LogoLayer.h"
#include "editor-support/cocostudio/CCArmature.h"
#include "GuideLayer.h"
#include "BattleLadder.h"
#include "ActivityInfo.h"
#include "ScriptExtendTest.h"
#include "ShortcutFunctionLayer.h"
#include "WinConditionTool.h"

using namespace cocostudio;

History::History()
{
    curCityId=0;
	actors = NULL;
	goods = NULL;
	elem_list = NULL;
	map_array = NULL;
	curmap_index = -1;
	map_width = 0;
	map_height = 0;
	imgAtc = NULL;
	imgMov = NULL;
	imgSpc = NULL;
	imgHead = NULL;
	imgSmallFace = NULL;
	imgLandSwamp = NULL;
	imgLandForest = NULL;
	imgLandGrass = NULL;
	imgLandSnow = NULL;
	imgLandRiver_ahead[0] = NULL;
	imgLandRiver_ahead[1] = NULL;
	imgLandRiver_back[0] = NULL;
	imgLandRiver_back[1] = NULL;
	imgLandRiver_left[0] = NULL;
	imgLandRiver_left[1] = NULL;
	imgRiver_ahead[0] = NULL;
	imgRiver_ahead[1] = NULL;
	imgRiver_back[0] = NULL;
	imgRiver_back[1] = NULL;
	imgRiver_left[0] = NULL;
	imgRiver_left[1] = NULL;
	imgBG = NULL;
	imgShield[8] = NULL;
	imgDoubleSeid[14] = NULL;
	m_iRunAction = 0;
	for (int i = 0; i<20; i++)
	{
		m_bGuide[i] = false;
	}
	m_layHome = NULL;
	m_layTitleBar = NULL;
	SetCurRound(0);
	SetMaxRound(0);
	m_iPatchVersion = 0;
	m_pPlayerInfo = nullptr;
	m_pLoginInfo = nullptr;
	m_iMailNum = 0;
	m_iRecruitTotle = 0;
}

History::~History()
{
	FreeMapArray();
	for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE_IMAGE(imgShield[i]);
	}
	for (int i = 0; i < 14; i++)
	{
		SAFE_DELETE_IMAGE(imgDoubleSeid[i]);
	}
}

int History::getBattleActorImageId(short actor_id)
{
	int index = actors[actor_id].history_image_fight_id;
	int index2;
	int occu_level = actors[actor_id].occupation_id - actors[actor_id].history_occupation_id;

	if(index<400)
	{
		index2 = index + (index - 1) * 7;
	}
	else
	{
		index2 = index + 2000 * (occu_level+1);
	}
	return index2;
}



void History::LoadBattleActorImage(short actor_id)
{
	LoadBattleActorImageEx(imgMov, actor_id, "mov");
	LoadBattleActorImageEx(imgAtc, actor_id, "atk");
	LoadBattleActorImageEx(imgSpc, actor_id, "spc");

	int index = actors[actor_id].image_id;

	index = history->actors[actor_id].face_id + 1;
	if(index > 0 && !imgHead[index])
	{
         imgHead[index] = CreateShrinkImageFile((char*)ChessSprintf((char*)"%s%d%s", "img/Face_s/", index, ".png"), 0, 0, 220, 220, 140, 140);
    }
}

void History::LoadBattleActorImageEx(TRImagePtr*& imgPtr, short actor_id, char* filename)
{
	int index = actors[actor_id].history_image_fight_id;
	int index2 = getBattleActorImageId(actor_id);
	int occu_level = actors[actor_id].occupation_id - actors[actor_id].history_occupation_id;

	if(index < 400)
	{
		for(int i = 0; i < 7; i++)
		{
			if(!imgPtr[index2 + i])
			{
				//imgPtr[index2 + i] = CreateImage(ChessSprintf("%s%d_%d_%s", "img/UnitS/", index, i + 1, filename));
                imgPtr[index2 + i] = CreateImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, i + 1, filename));
                if(!imgPtr[index2 + i])
				{
					bool isenable = true;
					for(int j = i - 1; j >= 0; j--)
					{
                        imgPtr[index2 + i] = CreateImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, j + 1, filename));
						if(imgPtr[index2 + i])
						{
							isenable = false;
							break;
						}
					}
					if(isenable)
					{
						for(int j = i + 1; j < 7; j++)
						{
                            imgPtr[index2 + i] = CreateImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, j + 1, filename));
                            if(imgPtr[index2 + i])
								break;
						}
					}
				}
			}
		}
	}
	else
	{
		if(!imgPtr[index2])
		{
			for(int i = 0; occu_level + 1 - i > 0; i++)
			{
				
                imgPtr[index2] = CreateImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, 1 + occu_level - i, filename));
                if(imgPtr[index2])
					break;
			}
			if(!imgPtr[index2])
			{
                imgPtr[index2] = CreateImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index - 400, 1, filename));
            }
		}
	}
}

int History::AddBattleOwn(short actor_id)
{
	int i,j;
	HistoryActorPtr historyActor;
	for(i=0;i<ownArmyNum;i++)
	{
		if(actor_id == ownArmy[i])
		{
			for(j = 0;j<battleOwnNum;j++)
			{
				if(actor_id == battleOwn[j]->actor_id)
					return 0;
			}
			historyActor = &actors[actor_id];
			battleOwn[battleOwnNum] = historyActor;
			battleOwnNum++;
			return 1;
		}
	}
	return 0;
}
int History::GetArmyAveLevel()
{
	if (ownArmyNum > 0)
	{
		int iSumLevel = 0;
		for(int i = 0; i < ownArmyNum; i++)
		{
			iSumLevel += actors[ownArmy[i]].GetActorLv();
		}
		return iSumLevel / ownArmyNum;
	}
	return 1;
}
int History::GetArmyAveLevelDq()
{
	int i;
	int total_level = 0;
	int baseLevel[MAX_OWN_ARMY_NUM], j, temp;
	if (ownArmyNum <= 0)
		return 1;
	if (3 >= ownArmyNum)
	{
		for (i = 0; i < ownArmyNum; i++)
		{
			total_level += actors[ownArmy[i]].GetActorLv();
		}
		return total_level / ownArmyNum;
	}
	for (i = 0; i < ownArmyNum; i++)
	{
		baseLevel[i] = actors[ownArmy[i]].GetActorLv();
	}
	for (i = 0; i < 3; i++)
	{
		for (j = i + 1; j < ownArmyNum; j++)
		{
			if (baseLevel[i] < baseLevel[j])
			{
				temp = baseLevel[i];
				baseLevel[i] = baseLevel[j];
				baseLevel[j] = temp;
			}
		}
		total_level += baseLevel[i];
	}
	int iRet = total_level / 3;
	int iRet1 = 0;
	if (baseLevel[0] > baseLevel[1] + baseLevel[2])
		iRet1 = baseLevel[0] - 7;
	if (iRet1 > iRet)
		iRet = iRet1;
	return iRet;
}
void History::ArmyCome(short actor_id, short level_add, bool memory_join)
{
	int iLevel = 0;
	iLevel = GetArmyAveLevelDq();

	//武将加入等级计算
	iLevel += level_add;
	if(iLevel <= 0)
	{
		iLevel = 1;
	}

	DealActorJoin(actor_id, iLevel,memory_join);
}

void History::ArmyComeReturnToLeave(short actor_id)
{
	for(int i = 0; i < ownArmyNum; i++)
	{
		if(actor_id == ownArmy[i])
			return;
	}

	LoadBattleActorImage(actor_id);
	ownArmy[ownArmyNum++] = actor_id;
}

void History::ArmyLeave(short actor_id,int left_goods)
{
	int i;
	int find = 0;
	for (i = 0; i < ownArmyNum; i++)
	{
		if (actor_id == ownArmy[i])
		{
			find = 1;

			if (left_goods){

				if (actors[actor_id].assist_good.isNormalGood())
				{
					HistoryGoodsPtr good = new HistoryGoods();
					good->setGood(&actors[actor_id].assist_good);
					history->goods->addElement(good);
					actors[actor_id].assist_good.good_id = -1;
				}
				if (actors[actor_id].weapon.isNormalGood())
				{
					HistoryGoodsPtr good = new HistoryGoods();
					good->setGood(&actors[actor_id].weapon);
					history->goods->addElement(good);
					actors[actor_id].weapon.good_id = -1;
				}

				if (actors[actor_id].coat.isNormalGood())
				{
					HistoryGoodsPtr good = new HistoryGoods();
					good->setGood(&actors[actor_id].coat);
					history->goods->addElement(good);
					actors[actor_id].coat.good_id = -1;
				}
				if (actors[actor_id].fashion.isNormalGood())
				{
					HistoryGoodsPtr good = new HistoryGoods();
					good->setGood(&actors[actor_id].fashion);
					history->goods->addElement(good);
					actors[actor_id].fashion.good_id = -1;
				}

				if (actors[actor_id].wap_fashion.isNormalGood())
				{
					HistoryGoodsPtr good = new HistoryGoods();
					good->setGood(&actors[actor_id].wap_fashion);
					history->goods->addElement(good);
					actors[actor_id].wap_fashion.good_id = -1;
				}
			}
		}

		if (find)
		{
			ownArmy[i] = ownArmy[i + 1];
		}
	}
	if (find)
	{
		ownArmyNum--;
	}
	
}
void History::clearSceneData()
{
	scr_x = 0;
	scr_y = 0;
	battleFriendNum = 0;
	battleEnemyNum = 0;
	curActionSide = 0;
	enemy_skipper_index = -1;
	own_skipper_index = -1;
	updateWeather(0);
	m_iAirType = 0;
	level_add = 0;
	cur_actor = NULL;
	BG_index = -1;
	max_amy_num = 0;
	bg_music_index = -1;
	m_bAutoRun = false;
	m_bTakeTurns = false;

	for (int i = 0; i < MAX_OWN_ARMY_NUM; i++)
	{
		out_amy[i] = -1;
		in_amy[i] = -1;
	}
	memset(event_name,0,30);
	memset(scene_name,0,30);
	memset(win_condition,0,500);
	SetCurRound(0);
	SetMaxRound(0);
	ScriptConfigOwnForce::scene_flag = -1;
	ScriptExtendTest122::m_mOldAttr.clear();
}

void History::clearAllData()
{
	SAFE_DELETE_ARRAY(actors);
	SAFE_DELETE(elem_list);
	SAFE_DELETE(goods);
	int i;
	if (imgAtc)
	{
		for (i = 0; i < MAX_BATTLE_IMAGE_NUM;i++)
		{
			SAFE_DELETE_IMAGE(imgAtc[i]);
		}
		SAFE_DELETE_ARRAY(imgAtc);
	}
	if (imgMov)
	{
		for (i = 0; i < MAX_BATTLE_IMAGE_NUM; i++)
		{
			SAFE_DELETE_IMAGE(imgMov[i]);
		}
		SAFE_DELETE_ARRAY(imgMov);
	}
	if (imgSpc)
	{
		for (i = 0; i < MAX_BATTLE_IMAGE_NUM; i++)
		{
			SAFE_DELETE_IMAGE(imgSpc[i]);
		}
		SAFE_DELETE_ARRAY(imgSpc);
	}
	if (imgHead)
	{
		for (i = 0; i < MAX_HEAD_NUM; i++)
		{
			SAFE_DELETE_IMAGE(imgHead[i]);
		}
		SAFE_DELETE_ARRAY(imgHead);
	}
	if(imgSmallFace)
	{
		for(i = 0; i < MAX_HEAD_NUM; i++)
		{
			SAFE_DELETE_IMAGE(imgSmallFace[i]);
		}
		SAFE_DELETE_ARRAY(imgSmallFace);
	}
}
void History::InitNoHisData()
{
	clearAllData();
	imgAtc = new TRImagePtr[MAX_BATTLE_IMAGE_NUM];
	imgMov = new TRImagePtr[MAX_BATTLE_IMAGE_NUM];
	imgSpc = new TRImagePtr[MAX_BATTLE_IMAGE_NUM];
	imgHead = new TRImagePtr[MAX_HEAD_NUM];
	imgSmallFace = new TRImagePtr[MAX_HEAD_NUM];
	memset(imgAtc,0,sizeof(TRImagePtr)*MAX_BATTLE_IMAGE_NUM);
	memset(imgMov,0,sizeof(TRImagePtr)*MAX_BATTLE_IMAGE_NUM);
	memset(imgSpc,0,sizeof(TRImagePtr)*MAX_BATTLE_IMAGE_NUM);
	memset(imgHead,0,sizeof(TRImagePtr)*MAX_HEAD_NUM);
	memset(imgSmallFace, 0, sizeof(TRImagePtr)*MAX_HEAD_NUM);

	map<int, GoodsItem*>::iterator itGoods;
	for(itGoods = trData->m_mGoodsItem.begin(); itGoods != trData->m_mGoodsItem.end(); itGoods++)
	{
		GoodsItem* ptrGoods = itGoods->second;
		if (!m_mImgGoods[ptrGoods->iGoodsId])
		{
			m_mImgGoods[ptrGoods->iGoodsId] = CreateScaleImage(ChessSprintf("img/Items/%d.png", ptrGoods->iImgId), 0.60f, 0);
		}
	}

	if(!imgLandSwamp)
	{
		imgLandSwamp = CreateGlobalImage("Mark/15");
		imgLandForest = CreateGlobalImage("Mark/16");
		imgLandGrass = CreateGlobalImage("Mark/17");
		imgLandSnow = CreateGlobalImage("Mark/18");
		imgLandRiver_ahead[0] = CreateLocalImage("Mark/35");
		imgLandRiver_ahead[1] = CreateLocalImage("Mark/36");
		imgLandRiver_back[0] = CreateLocalImage("Mark/37");
		imgLandRiver_back[1] = CreateLocalImage("Mark/38");
		imgLandRiver_left[0] = CreateLocalImage("Mark/39");
		imgLandRiver_left[1] = CreateLocalImage("Mark/40");
		imgRiver_ahead[0] = CreateLocalImage("Mark/29");
		imgRiver_ahead[1] = CreateLocalImage("Mark/30");
		imgRiver_back[0] = CreateLocalImage("Mark/31");
		imgRiver_back[1] = CreateLocalImage("Mark/32");
		imgRiver_left[0] = CreateLocalImage("Mark/33");
		imgRiver_left[1] = CreateLocalImage("Mark/34");
	}
	for (int j = 0; j < 8; j++)
	{
		imgShield[j] = CreateGlobalImageFile(ChessSprintf("%s%d%s", "Effect/BloodShield/", j+1, ".png"));
	}
	for (int j = 0; j < 14; j++)
	{
		imgDoubleSeid[j] = CreateGlobalImageFile(ChessSprintf("%s%d%s", "Effect/coop100/100_", j, ".png"));
	}

	SAFE_DELETE_IMAGE(imgBG);
	RedeActorNum = 0;
	memset(ownArmy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	memset(RedeActor, 0, MAX_OWN_ARMY_NUM*sizeof(HistoryActorPtr));
	memset(battleOwn, 0, MAX_OWN_ARMY_NUM*sizeof(HistoryActorPtr));
	memset(out_amy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	memset(in_amy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	memset(battleFriend, 0, modConst->NEW_MAX_FRIEND_ARMY_NUM*sizeof(HistoryActorPtr));
	memset(battleEnemy, 0, modConst->NEW_MAX_ENEMY_ARMY_NUM*sizeof(HistoryActorPtr));
	m_mConsuNum.clear();
	m_mConsuNetId.clear();
	base_level = 0;
}
void History::loadUserData()
{
	InitNoHisData();
	int i;
	actors = new HistoryActor[trData->actor_num];
	for(i = 0;i<trData->actor_num;i++)
	{
		actors[i].init(i);
	}
	m_mConsuNum.clear();
	m_mConsuNetId.clear();
	money = 0;
	stone = 0;
	extend_value = 0;
	ownArmyNum = 0;
	for (i = 0; i<MAX_OWN_ARMY_NUM; i++)
	{
		ownArmy[i] = -1;
	}
	memset(var, 0, MAX_CHAR_VAR_NUM * sizeof(var[0]));
	memset(var2, 0, MAX_INT_VAR_NUM * sizeof(var2[0]));
	memset(var3, 0, 4096 * sizeof(var3[0]));
	game_scene = 0;
	game_state = -1;
	initilized = 0;
	memset(battle_name,0,30);
	clearSceneData();
	elem_list = new DList<MapElem>(true);
	goods = new DList<HistoryGoods>(true);
	r_script_index = -1;
	isFreeMove = 0;
	readscreen_bust_one = -1;
	readscreen_bust_two = -1;
	readscreen_bust_paint = 0;
	battlescreen_paint_bust_offset = 0;
	is_paint_bust = 0;
	Actor1AttackActor2.resize(1000);
	Actor1AttackActor2.memset(0);
	bEnble5e = false;
	iExtraAddMaxRound = 0;
	m_iJade = 0;
	m_iJadeSeal = 0;

	mTitleLayer = NULL;
	is_battle_win = false;
	mArmyLevel = 1;
	m_layTitleBar = NULL;
	m_iBattleType = BATTLE_NORMAL;
	m_iCurWarId = 0;
	mScriptValidate = -1;
	mRomanceOpenData.clear();
	m_m12ScriptCon.clear();
	for (int i = 0; i<20; i++)
	{
		m_bGuide[i] = false;
	}
	m_iActivityEntry = 1;
	m_iOpenId = 0;
	m_iVIP = 0;
	m_iVIPGift = 0;
	m_iBattleStage = 0;
	BattleLadder::getInstance()->ClearAllData();
	m_mActScene.clear();
	m_bAutoRun = false;
	m_bTakeTurns = false;
	m_iMailNum = 0;
}
void History::loadInitialData()
{
	ownArmy = new short[MAX_OWN_ARMY_NUM]; //保存人物id
	memset(ownArmy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	RedeActorNum = 0;
	RedeActor = new HistoryActorPtr[MAX_OWN_ARMY_NUM];
	memset(RedeActor, 0, MAX_OWN_ARMY_NUM*sizeof(HistoryActorPtr));
	battleOwn = new HistoryActorPtr[MAX_OWN_ARMY_NUM];
	memset(battleOwn, 0, MAX_OWN_ARMY_NUM*sizeof(HistoryActorPtr));
	out_amy = new short[MAX_OWN_ARMY_NUM];
	memset(out_amy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	in_amy = new short[MAX_OWN_ARMY_NUM];
	memset(in_amy, 0, MAX_OWN_ARMY_NUM*sizeof(short));
	battleFriend = new HistoryActorPtr[modConst->NEW_MAX_FRIEND_ARMY_NUM];
	memset(battleFriend, 0, modConst->NEW_MAX_FRIEND_ARMY_NUM*sizeof(HistoryActorPtr));
	battleEnemy = new HistoryActorPtr[modConst->NEW_MAX_ENEMY_ARMY_NUM];
	memset(battleEnemy, 0, modConst->NEW_MAX_ENEMY_ARMY_NUM*sizeof(HistoryActorPtr));

	m_pLoginInfo = new LoginInfo();
	m_pLoginInfo->loadLogin();

	m_pPlayerInfo = new PlayerInfo();

	mMapDateInfo = new MapDateInfo();

	mChipSoulDate = new ChipSoulDate();

	mArmyFuBenBattle = new ArmyFuBenBattle();

	if(ChessFile::isDirectoryExist(getRespath("mapInfo")))
	{
		MapGenerate::GenerateMapJson();
	}

	int airArray[30] = {0, 0, 1, 1, 3, 3, 0, 0, 0, 0, 1, 3, 0, 2, 2, 1, 1, 3, 0, 3, 3, 2, 1, 1, 4, 4, 2, 2, 4, 3}; //天气类型初始化数组0-晴1-阴2-雨3-风4-雪
	for(int i = 0; i < 30; i++)
	{
		m_iAirArray[i] = airArray[i];
	}
}

HistoryActorPtr History::FindActor(short actor_id)
{
	if(game_state==GAME_STATE_BATTLE)
	{
		return FindBattleActor(actor_id);
	}
	else if(game_state==GAME_STATE_REDE)
	{
		return FindRedeActor(actor_id);
	}
	else if(actor_id>=0&&actor_id<=trData->actor_num)
	{
		return &actors[actor_id];
	}
	else
		return NULL;
}
// ///////////////////////////////////////////////////////////////////////////////////////
// 得到战场上是否存在某一角色
HistoryActorPtr History::FindRedeActor(short actor_id)
{
	if(game_state==GAME_STATE_REDE)
	{
		for(int i = 0;i<RedeActorNum;i++)
		{
			if (RedeActor[i]->actor_id == actor_id) // && battleOwn[i]->hide != 1
			{
				return RedeActor[i];
			}
		}
	}
	return NULL;
}
HistoryActorPtr History::FindBattleActor(short actor_id)
{
	int i;
	for (i = 0; i < battleOwnNum; i++) {
		if (battleOwn[i]->actor_id == actor_id) {
			return battleOwn[i];
		}
	}
	for (i = 0; i < battleFriendNum; i++) {
		if (battleFriend[i]->actor_id == actor_id) {
			return battleFriend[i];
		}
	}
	for (i = 0; i < battleEnemyNum; i++) {
		if (battleEnemy[i]->actor_id == actor_id) {
			return battleEnemy[i];
		}
	}
	return NULL;
}



HistoryActorPtr History::FindOppoActorPosition(HistoryActorPtr curActor,int x,int y)
{
	HistoryActorPtr battleActor = NULL;
	int i;
	if(curActor->relation == 2)
	{
		for(i = 0;i<battleOwnNum;i++)
		{
			if(battleOwn[i]->x == x&&battleOwn[i]->y == y && battleOwn[i]->hide !=1)
			{
				battleActor = battleOwn[i];
				break;
			}
		}
		if(!battleActor)
		{
			for(i = 0;i<battleFriendNum;i++)
			{
				if(battleFriend[i]->x == x&&battleFriend[i]->y == y && battleFriend[i]->hide !=1)
				{
					battleActor = battleFriend[i];
					break;
				}
			}
		}
	}
	else
	{
		for(i = 0;i<battleEnemyNum;i++)
		{
			if(battleEnemy[i]->x == x&&battleEnemy[i]->y == y&&battleEnemy[i]->hide !=1)
			{
				battleActor = battleEnemy[i];
				break;
			}
		}
	}
	return battleActor;
}

HistoryActorPtr History::FindBattleRelationActor(HistoryActorPtr curActor, int iActorId)
{
	HistoryActorPtr battleActor = NULL;
	int i;

	if (iActorId < 0)
		return battleActor;

	if (curActor->relation == 2)
	{
		for (i = 0; i < battleEnemyNum; i++)
		{
			if (battleEnemy[i]->actor_id == iActorId&&battleEnemy[i]->hide != 1)
			{
				battleActor = battleEnemy[i];
				break;
			}
		}
	}
	else
	{
		for (i = 0; i < battleOwnNum; i++)
		{
			if (battleOwn[i]->actor_id == iActorId && battleOwn[i]->hide != 1)
			{
				battleActor = battleOwn[i];
				break;
			}
		}
		if (!battleActor)
		{
			for (i = 0; i < battleFriendNum; i++)
			{
				if (battleFriend[i]->actor_id == iActorId && battleFriend[i]->hide != 1)
				{
					battleActor = battleFriend[i];
					break;
				}
			}
		}
	}
	return battleActor;
}

HistoryActorPtr History::FindNearestOppoActor(HistoryActorPtr curActor)
{
	HistoryActorPtr battleActor = NULL;
	int most_near_distance = 0xffff;
	int i;
	if(curActor->relation == 2)
	{
		for(i = 0;i<battleOwnNum;i++)
		{
			if((abs(battleOwn[i]->x - curActor->x) + abs(battleOwn[i]->y - curActor->y)<most_near_distance)
				&& battleOwn[i]->hide !=1)
			{
				most_near_distance = abs(battleOwn[i]->x - curActor->x) + abs(battleOwn[i]->y - curActor->y);
				battleActor = battleOwn[i];
			}
		}
		if(!battleActor)
		{
			for(i = 0;i<battleFriendNum;i++)
			{
				if((abs(battleFriend[i]->x - curActor->x) + abs(battleFriend[i]->y - curActor->y)<most_near_distance) 
					&& battleFriend[i]->hide !=1)
				{
					most_near_distance = abs(battleFriend[i]->x - curActor->x) + abs(battleFriend[i]->y - curActor->y);
					battleActor = battleFriend[i];
				}
			}
		}
	}
	else
	{
		for(i = 0;i<battleEnemyNum;i++)
		{
			if((abs(battleEnemy[i]->x - curActor->x) + abs(battleEnemy[i]->y - curActor->y)<most_near_distance)
				&& battleEnemy[i]->hide !=1)
			{
				most_near_distance = abs(battleEnemy[i]->x - curActor->x) + abs(battleEnemy[i]->y - curActor->y);
				battleActor = battleEnemy[i];
			}
		}
	}
	return battleActor;
}
HistoryActorPtr History::FindOwnActorPosition(HistoryActorPtr curActor,int x,int y)
{
	HistoryActorPtr battleActor = NULL;
	int i;
	if(curActor->relation == 2)
	{
		for(i = 0;i<battleEnemyNum;i++)
		{
			if(battleEnemy[i]->x == x&&battleEnemy[i]->y == y&& battleEnemy[i]->hide!=1)
			{
				battleActor = battleEnemy[i];
				break;
			}
		}
	}
	else
	{
		for(i = 0;i<battleOwnNum;i++)
		{
			if(battleOwn[i]->x == x&&battleOwn[i]->y == y&& battleOwn[i]->hide!=1)
			{
				battleActor = battleOwn[i];
				break;
			}
		}
		if(!battleActor)
		{
			for(i = 0;i<battleFriendNum;i++)
			{
				if(battleFriend[i]->x == x&&battleFriend[i]->y == y&& battleFriend[i]->hide!=1)
				{
					battleActor = battleFriend[i];
					break;
				}
			}
		}
	}
	return battleActor;
}
HistoryActorPtr History::FindActorPosition(int x,int y)
{
	HistoryActorPtr battleActor = NULL;
	int i;
	for(i = 0;i<battleOwnNum;i++)
	{
		if(battleOwn[i]->x == x&&battleOwn[i]->y == y && battleOwn[i]->hide!=1)
		{
			battleActor = battleOwn[i];
			break;
		}
	}
	if(!battleActor)
	{
		for(i = 0;i<battleFriendNum;i++)
		{
			if(battleFriend[i]->x == x&&battleFriend[i]->y == y&& battleFriend[i]->hide!=1)
			{
				battleActor = battleFriend[i];
				break;
			}
		}
	}
	if(!battleActor)
	{
		for(i = 0;i<battleEnemyNum;i++)
		{
			if(battleEnemy[i]->x == x&&battleEnemy[i]->y == y&& battleEnemy[i]->hide!=1)
			{
				battleActor = battleEnemy[i];
				break;
			}
		}
	}
	return battleActor;
}

int History::getBaseLevel(short armNum)
{
	int i;
	int total_level = 0;
	int baseLevel[MAX_OWN_ARMY_NUM], j, temp;
	if (armNum >= battleOwnNum)
	{
		for (i = 0; i < battleOwnNum; i++)
		{
			total_level += battleOwn[i]->GetActorLv();
		}
		return total_level / battleOwnNum;
	}
	for (i = 0; i < battleOwnNum; i++)
	{
		baseLevel[i] = battleOwn[i]->GetActorLv();
	}
	for (i = 0; i < armNum; i++)
	{
		for (j = i + 1; j < battleOwnNum; j++)
		{
			if (baseLevel[i] < baseLevel[j])
			{
				temp = baseLevel[i];
				baseLevel[i] = baseLevel[j];
				baseLevel[j] = temp;
			}
		}
		total_level += baseLevel[i];
	}
	return total_level / armNum;
}

int History::getBaseLevelSpecial()
{
	int i;
	int total_level = 0;
	int baseLevel[MAX_OWN_ARMY_NUM], j, temp;
	if (3 >= battleOwnNum)
	{
		for (i = 0; i < battleOwnNum; i++)
		{
			total_level += battleOwn[i]->GetActorLv();
		}
		return total_level / battleOwnNum;
	}
	for (i = 0; i < battleOwnNum; i++)
	{
		baseLevel[i] = battleOwn[i]->GetActorLv();
	}
	for (i = 0; i < 3; i++)
	{
		for (j = i + 1; j < battleOwnNum; j++)
		{
			if (baseLevel[i] < baseLevel[j])
			{
				temp = baseLevel[i];
				baseLevel[i] = baseLevel[j];
				baseLevel[j] = temp;
			}
		}
		total_level += baseLevel[i];
	}
	int iRet = total_level / 3;
	int iRet1 = 0;
	if (baseLevel[0]>baseLevel[1] + baseLevel[2])
		iRet1 = baseLevel[0] - 7;
	if (iRet1 > iRet)
		iRet = iRet1;
	return iRet;
}

//战场基本等级计算，我军等级平均值。
void History::CalcBaseLevel()
{
	int total_level = 0;
    if(history->var2[4050] == 1){ //4050用开控制是否采用指定等级模式，而不是自动计算等级模式
        base_level = 1 + history->var2[4051]; // 如果是指定等级模式，则数值来自4051 + 1
        return;
    }
    else if (battleOwnNum>4)//最高等级的前四个
	{
		base_level = getBaseLevel(4);
	}
	else if (history->var2[5012] > 0 && battleOwnNum > history->var2[5012])
	{
		base_level = getBaseLevel(history->var2[5012]);
	}
	else
	{
		base_level = getBaseLevelSpecial();//敌军等级改为出战人物等级最高三人的平均等级
	}
}
void History::SpecialCalcBaseLevel()
{
	int i;
	int total_level = 0;
	if (ownArmyNum > 3)//最高等级的前3个
	{
		int baseLevel[MAX_OWN_ARMY_NUM], j, temp;
		for (i = 0; i < ownArmyNum; i++)
		{
			baseLevel[i] = actors[history->ownArmy[i]].GetActorLv();
		}
		for (i = 0; i < 3; i++)
		{
			for (j = i + 1; j < ownArmyNum; j++)
			{
				if (baseLevel[i] < baseLevel[j])
				{
					temp = baseLevel[i];
					baseLevel[i] = baseLevel[j];
					baseLevel[j] = temp;
				}
			}
			total_level += baseLevel[i];
		}
		base_level = total_level / 3;
	}
	else
	{
		if (ownArmyNum > 0)
		{
			for (i = 0; i < ownArmyNum; i++)
			{
				total_level += actors[history->ownArmy[i]].GetActorLv();
			}
			base_level = total_level / ownArmyNum;
		}
	}
	if (base_level>0)
	{
		CalcUserGoodLevel();
	}
}
void History::CalcUserGoodLevel()
{

}
void History::CenterActor(HistoryActorPtr battleactor)
{
	CenterPos(battleactor->x, battleactor->y);
}
void History::CenterPos(int x,int y)
{
	scr_x = x*SCENE_TILE_WIDTH + SCENE_TILE_WIDTH / 2 - screen_size_x / 2;
	scr_y = y*SCENE_TILE_WIDTH + SCENE_TILE_WIDTH / 2 - screen_size_y / 2;
// 	if (scr_x<0)
// 		scr_x = 0;
// 	if (scr_y<0)
// 		scr_y = 0;
	if (scr_x + screen_size_x>map_width*SCENE_TILE_WIDTH)
		scr_x = map_width*SCENE_TILE_WIDTH - screen_size_x;
	if (scr_y + screen_size_y>map_height*SCENE_TILE_WIDTH)
		scr_y = map_height*SCENE_TILE_WIDTH - screen_size_y;

	if (scr_x < 0)
		scr_x = 0;
	if (scr_y < 0)
		scr_y = 0;
}

void History::CheckPosActor(HistoryActorPtr battleactor)
{
	CheckPos(battleactor->x, battleactor->y);
}
void History::CheckPos(int x, int y)
{
	if (x*SCENE_TILE_WIDTH + 2 * SCENE_TILE_WIDTH - scr_x> screen_size_x)
		scr_x = x*SCENE_TILE_WIDTH + 2 * SCENE_TILE_WIDTH - screen_size_x;

	if (x*SCENE_TILE_WIDTH - SCENE_TILE_WIDTH - scr_x < 0)
		scr_x = x*SCENE_TILE_WIDTH - SCENE_TILE_WIDTH;

	if (y*SCENE_TILE_WIDTH + 2 * SCENE_TILE_WIDTH - scr_y> screen_size_y)
		scr_y = y*SCENE_TILE_WIDTH + 2 * SCENE_TILE_WIDTH - screen_size_y;

	if (y*SCENE_TILE_WIDTH - SCENE_TILE_WIDTH - scr_y <0)
		scr_y = y*SCENE_TILE_WIDTH - SCENE_TILE_WIDTH;

	
	if (scr_x + screen_size_x>map_width*SCENE_TILE_WIDTH)
		scr_x = map_width*SCENE_TILE_WIDTH - screen_size_x;
	if (scr_y + screen_size_y>map_height*SCENE_TILE_WIDTH)
		scr_y = map_height*SCENE_TILE_WIDTH - screen_size_y;

	if (scr_x < 0)
		scr_x = 0;
	if (scr_y < 0)
		scr_y = 0;
	if (history->m_iRunAction)
		((ActionLayer*)(ChessApplication::getInstance()->GetActionLayer()))->SetActionPosition();
}


int History::getActiveOwnNum()
{
	int num = 0;
	for (int i = 0; i < battleOwnNum; i++)
	{
		if (battleOwn[i]->hide != 1 && !battleOwn[i]->action_finished && !battleOwn[i]->state_puzzle&&!battleOwn[i]->state_obfuscation)
		{
			num++;
		}
	}
	return num;
}
int History::getBattleOwnNum()
{
	int num = 0;
	for (int i = 0; i < battleOwnNum; i++)
	{
		if (battleOwn[i]->hide != 1)
		{
			num++;
		}
	}
	return num;
}
int History::getBattleEnemyNum()
{
	int num = 0;
	for(int i = 0; i < battleEnemyNum; i++)
	{
		if(battleEnemy[i]->hide != 1)
		{
			if(battleEnemy[i]->isAssistArmy == 0)
			{
				num++;
			}
		}
	}
	return num;
}
int History::getBattleAssistNum()
{
	int num = 0;
	for(int i = 0; i < battleEnemyNum; i++)
	{
		if(battleEnemy[i]->hide != 1)
		{
			if(battleEnemy[i]->isAssistArmy)
			{
				num++;
			}
		}
	}
	return num;
}

int History::getBattleOwnNum(int region, int x1, int y1, int x2, int y2)
{
	int num = 0;
	for(int i = 0; i < battleOwnNum; i++)
	{
		if(battleOwn[i]->hide != 1 && (!region || battleOwn[i]->x >= x1 && battleOwn[i]->x <= x2 && battleOwn[i]->y >= y1 && battleOwn[i]->y <= y2))
		{
			num++;
		}
	}
	return num;
}
int History::getBattleFriendNum(int region, int x1, int y1, int x2, int y2)
{
	int num = 0;
	for (int i = 0; i < battleFriendNum; i++)
	{
		if(battleFriend[i]->hide != 1 && (!region || battleFriend[i]->x >= x1 && battleFriend[i]->x <= x2 && battleFriend[i]->y >= y1 && battleFriend[i]->y <= y2))
		{
			num++;
		}
	}
	return num;
}
int History::getBattleEnemyNum(int region, int x1, int y1, int x2, int y2)
{
	int num = 0;
	for(int i = 0; i < battleEnemyNum; i++)
	{
		if(battleEnemy[i]->hide != 1 && (!region || battleEnemy[i]->x >= x1 && battleEnemy[i]->x <= x2 && battleEnemy[i]->y >= y1 && battleEnemy[i]->y <= y2))
		{
			if(battleEnemy[i]->isAssistArmy == 0)
			{
				num++;
			}
		}
	}
	return num;
}
int History::getBattleAssistNum(int region, int x1, int y1, int x2, int y2)
{
	int num = 0;
	for(int i = 0; i < battleEnemyNum; i++)
	{
		if(battleEnemy[i]->hide != 1 && (!region || battleEnemy[i]->x >= x1 && battleEnemy[i]->x <= x2 && battleEnemy[i]->y >= y1 && battleEnemy[i]->y <= y2))
		{
			if(battleEnemy[i]->isAssistArmy)
			{
				num++;
			}
		}
	}
	return num;
}

int History::getActorOwnIndex(short actor_id)
{
	int i;
	if(game_state == GAME_STATE_REDE)
	{
		for (i = 0;i<ownArmyNum;i++){
			if(ownArmy[i] == actor_id){
				return i;
			}
		}
	}
	else
	{
		for (i = 0; i < battleOwnNum; i++) {
			if (battleOwn[i]->actor_id == actor_id && battleOwn[i]->hide != 1) {
				return i;
			}
		}
	}
	return -1;
}
int History::getActorFriendIndex(short actor_id)
{
	int i;
	for (i = 0; i < battleFriendNum; i++) {
		if (battleFriend[i]->actor_id == actor_id
			&& battleFriend[i]->hide != 1) {
			return i;
		}
	}
	return -1;
}
int History::getActorEnemyIndex(short actor_id)
{
	int i;
	for (i = 0; i < battleEnemyNum; i++) {
		if (battleEnemy[i]->actor_id == actor_id && battleEnemy[i]->hide != 1) {
			return i;
		}
	}
	return -1;
}
HistoryActorPtr History::getNextOwn(HistoryActorPtr cur_actor)
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorOwnIndex(cur_actor->actor_id);
	if(game_state == GAME_STATE_REDE)
	{
		if(index==-1 || (index == history->ownArmyNum - 1))
		{
			return &history->actors[history->ownArmy[0]];
		}
		else
		{
			return &history->actors[history->ownArmy[index+1]];
		}
	}
	else
	{
		if(index==-1)
		{
			for (i = 0; i < battleOwnNum; i++) {
				if (battleOwn[i]->hide != 1 ) {
					return battleOwn[i];
				}
			}
		}
		else
		{
			for(i = index+1;i<battleOwnNum;i++)
			{
				if (battleOwn[i]->hide != 1 ) {
					return battleOwn[i];
				}
			}
			for(i = 0;i<index+1;i++)
			{
				if (battleOwn[i]->hide != 1 ) {
					return battleOwn[i];
				}
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getNextEnemy(HistoryActorPtr cur_actor)
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorEnemyIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleEnemyNum; i++) {
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
	}
	else
	{
		for(i = index+1;i<battleEnemyNum;i++)
		{
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
		for(i = 0;i<index+1;i++)
		{
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getNextFriend(HistoryActorPtr cur_actor) 
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorFriendIndex(cur_actor->actor_id);
	{
		if(index==-1)
		{
			for (i = 0; i < battleFriendNum; i++) {
				if (battleFriend[i]->hide != 1) {
					return battleFriend[i];
				}
			}
		}
		else
		{
			for(i = index+1;i<battleFriendNum;i++)
			{
				if (battleFriend[i]->hide != 1) {
					return battleFriend[i];
				}
			}
			for(i = 0;i<index+1;i++)
			{
				if (battleFriend[i]->hide != 1) {
					return battleFriend[i];
				}
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getPreOwn(HistoryActorPtr cur_actor)
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorOwnIndex(cur_actor->actor_id);
	if (game_state == GAME_STATE_REDE)
	{
		if (index == -1 || (index == 0))
		{
			return &history->actors[history->ownArmy[history->ownArmyNum - 1]];
		}
		else
		{
			return &history->actors[history->ownArmy[--index]];
		}
	}
	else
	{
		if (index == -1)
		{
			for (i = 0; i < battleOwnNum; i++) {
				if (battleOwn[i]->hide != 1) {
					return battleOwn[i];
				}
			}
		}
		else
		{
			for (i = index - 1; i>=0; i--)
			{
				if (battleOwn[i]->hide != 1) {
					return battleOwn[i];
				}
			}
			for (i = battleOwnNum - 1; i > index; i--)
			{
				if (battleOwn[i]->hide != 1) {
					return battleOwn[i];
				}
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getPreEnemy(HistoryActorPtr cur_actor)
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorEnemyIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleEnemyNum; i++) {
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
	}
	else
	{
		for(i = index-1;i>=0;i--)
		{
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
		for(i = battleEnemyNum-1;i>index;i--)
		{
			if (battleEnemy[i]->hide != 1) {
				return battleEnemy[i];
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getPreFriend(HistoryActorPtr cur_actor) 
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorFriendIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleFriendNum; i++) {
			if (battleFriend[i]->hide != 1) {
				return battleFriend[i];
			}
		}
	}
	else
	{
		for(i = index-1;i>=0;i--)
		{
			if (battleFriend[i]->hide != 1) {
				return battleFriend[i];
			}
		}
		for(i = battleFriendNum-1;i>index;i--)
		{
			if (battleFriend[i]->hide != 1) {
				return battleFriend[i];
			}
		}
	}
	return NULL;
}
HistoryActorPtr History::getNextActiveOwn()
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorOwnIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleOwnNum; i++) {
			if (battleOwn[i]->hide != 1 && !battleOwn[i]->action_finished&&!battleOwn[i]->state_puzzle&&!battleOwn[i]->state_obfuscation) {
				return battleOwn[i];
			}
		}
	}
	else
	{
		for(i = index+1;i<battleOwnNum;i++)
		{
			if (battleOwn[i]->hide != 1 && !battleOwn[i]->action_finished&&!battleOwn[i]->state_puzzle&&!battleOwn[i]->state_obfuscation) {
				return battleOwn[i];
			}
		}
		for(i = 0;i<index+1;i++)
		{
			if (battleOwn[i]->hide != 1 && !battleOwn[i]->action_finished&&!battleOwn[i]->state_puzzle&&!battleOwn[i]->state_obfuscation) {
				return battleOwn[i];
			}
		}
	}
	return NULL;
}
//获取到行动的部队
HistoryActorPtr History::getNextActiveEnemy()
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorEnemyIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleEnemyNum; i++) {
			if (battleEnemy[i]->hide != 1 && !battleEnemy[i]->action_finished&&!battleEnemy[i]->state_puzzle&&!battleEnemy[i]->state_obfuscation) {
				return battleEnemy[i];
			}
		}
	}
	else
	{
		for(i = index+1;i<battleEnemyNum;i++)
		{
			if (battleEnemy[i]->hide != 1 && !battleEnemy[i]->action_finished&&!battleEnemy[i]->state_puzzle&&!battleEnemy[i]->state_obfuscation) {
				return battleEnemy[i];
			}
		}
		for(i = 0;i<index+1;i++)
		{
			if (battleEnemy[i]->hide != 1 && !battleEnemy[i]->action_finished&&!battleEnemy[i]->state_puzzle&&!battleEnemy[i]->state_obfuscation) {
				return battleEnemy[i];
			}
		}
	}
	return NULL;
}

HistoryActorPtr History::getNextActiveFriend() 
{
	int i = 0,index = -1;
	if(cur_actor!=NULL)
		index = getActorFriendIndex(cur_actor->actor_id);
	if(index==-1)
	{
		for (i = 0; i < battleFriendNum; i++) {
			if (battleFriend[i]->hide != 1 && !battleFriend[i]->action_finished&& !battleFriend[i]->state_puzzle&& !battleFriend[i]->state_obfuscation) {
				return battleFriend[i];
			}
		}
	}
	else
	{
		for(i = index+1;i<battleFriendNum;i++)
		{
			if (battleFriend[i]->hide != 1 && !battleFriend[i]->action_finished&& !battleFriend[i]->state_puzzle&& !battleFriend[i]->state_obfuscation) {
				return battleFriend[i];
			}
		}
		for(i = 0;i<index+1;i++)
		{
			if (battleFriend[i]->hide != 1 && !battleFriend[i]->action_finished&& !battleFriend[i]->state_puzzle&& !battleFriend[i]->state_obfuscation) {
				return battleFriend[i];
			}
		}
	}
	return NULL;
}
HistoryActorPtr History::getNextDeadActor()
{
	HistoryActorPtr battleActor = NULL;
	int i;
	for(i = 0;i<battleOwnNum;i++)
	{
		if (battleOwn[i]->GetCurHp() <= 0 && battleOwn[i]->hide != 1)
		{
			battleActor = battleOwn[i];
			break;
		}
	}
	if(!battleActor)
	{
		for(i = 0;i<battleFriendNum;i++)
		{
			if (battleFriend[i]->GetCurHp() <= 0 && battleFriend[i]->hide != 1)
			{
				battleActor = battleFriend[i];
				break;
			}
		}
	}
	if(!battleActor)
	{
		for(i = 0;i<battleEnemyNum;i++)
		{
			if (battleEnemy[i]->GetCurHp() <= 0 && battleEnemy[i]->hide != 1)
			{
				battleActor = battleEnemy[i];
				break;
			}
		}
	}
	return battleActor;
}
bool  History::HasElem(int x, int y, short occupation_id)
{
	int i;
	MapElemPtr elem;
	for(i = 0;i<elem_list->size();i++)
	{
		elem = (MapElemPtr)elem_list->elementAt(i);
		
		if (elem->x == x&&elem->y == y)
		{
			int ret = 255;
			if (elem->land_id >= 0 && elem->land_id < trData->land_num)
			{
				ret = trData->occupation_landmove_info[occupation_id][elem->land_id];
			}
			if (ret == 255)
			{
				return true;
			}
		}	
	}
	return false;
}


int  History::getBarrierMoveSpend(int occupation_id,int x, int y)
{
	int i;
	MapElemPtr elem;
	for (i = 0; i < elem_list->size(); i++)
	{
		elem = (MapElemPtr)elem_list->elementAt(i);

		if(elem->x == x&&elem->y == y&&trData->occupation_landmove_info[occupation_id][elem->land_id] != 255)
		{
			return trData->occupation_landmove_info[occupation_id][elem->land_id];
		}
	}
	return 0;
}

TRImagePtr History::GetFaceImage(unsigned short actor_id, bool bSmall)
{
	int	face_index = actors[actor_id].face_id;
	//face_index = face_index>0?face_index+8:face_index+1;
	if(!actors[actor_id].mFashion[0]||actor_id)
	{
		face_index += 1;
		if(imgHead[face_index] == NULL)
			LoadBattleActorImage(actor_id);
	}
	else
	{
		string img_path = ChessSprintf("%d_%d_%d", face_index + 1, actors[actor_id].mFashion[1], actors[actor_id].mFashion[2]);
		if(mFashionFace.find(img_path) != mFashionFace.end())
		{
			return mFashionFace[img_path];
		}
		else
		{
			
            mFashionFace[img_path] = CreateShrinkImageFile((char*)ChessSprintf((char*)"%s%s%s", "img/Face_s/", img_path.c_str(), ".png"), 0, 0, 220, 220, 140, 140);
            return mFashionFace[img_path];
		}
	}
	if (bSmall)
	{
		return imgSmallFace[face_index];
	}
	return imgHead[face_index];
}
TRImagePtr History::GetGoodImage(unsigned short good_id)
{
	return m_mImgGoods[good_id];
}

void History::GenerateMapArray(char* data,int index)
{
	int point = 0x118 + index * 12;
	int i,j;
	point =  (data[point] & 0xff) << 24 | (data[point+1] & 0xff) << 16
		| (data[point+2] & 0xff) << 8| (data[point+3] & 0xff);
	map_width = data[point++]/3;
	map_height = data[point++]/3;
	map_array = new UUCHAR*[map_height];
	for (i = 0; i < map_height; i++)
	{
		map_array[i] = new UUCHAR[map_width];
	}
	for(i = 0;i<map_height;i++)
	{
		for(j = 0;j<map_width;j++)
		{
			map_array[i][j] = data[point++];
		}
	}
}
void History::GenerateMapArray(int index)
{
	if (index < 0)
	{
		index = 0;
	}

	if (index != curmap_index)
	{
		MapInfo* ptrMap = MapGenerate::getMapInfo(index);
		if(ptrMap)
		{
			FreeMapArray();
			map_width = ptrMap->m_iWidth;
			map_height = ptrMap->m_iHeight;
			map_array = new UUCHAR*[map_height];
			for(int i = 0; i < map_height; i++)
			{
				map_array[i] = new UUCHAR[map_width];
			}

			for(int i = 0; i < map_height; i++)
			{
				for(int j = 0; j < map_width; j++)
				{
					map_array[i][j] = ptrMap->m_mTerrain[i][j];
				}
			}
			curmap_index = index;
			return;
		}

 		char* game_data = ChessReadFile("Hexzmap.e5");
 		if (game_data != NULL)
 		{
 			FreeMapArray();
 			GenerateMapArray(game_data, index);
 			curmap_index = index;
 			ChessFree(game_data);
 		}
	}
}
void History::FreeMapArray()
{
	if (map_array)
	{
		for (int i = 0; i < map_height; i++)
		{
			if (map_array[i])
			{
				delete[] map_array[i];
				map_array[i] = NULL;
			}
		}
		delete[] map_array;
		map_array = NULL;
	}
}

//随机 air_type = 0
//晴天 34%    阴天 33%    小雨 23%   大雨 10%   雪天 0%

//晴好 air_type = 1
//晴天 70%    阴天 20%    小雨 10%   大雨 0%    雪天 0%

//阴雨 air_type = 2
//晴天 10%    阴天 25%    小雨 40%    大雨 25%    雪天 0%

//小雪 air_type = 3
//晴天 15%    阴天 35%    小雨 0%    大雨 0%    雪天 50%

//大雪 air_type = 4
//晴天 5%    阴天 25%    小雨 0%    大雨 0%    雪天 70%
void History::changeAir(int air_index)
{
	int iWeatherId = AIR_TYPE_SUN;
	if (air_index < 0)
	{
		int iWeather = GetCurWeather();
		if (iWeather > 0)
		{
			iWeatherId = iWeather;
			return;
		}
		else if (!iWeather&&(history->mRealWeather.size() || history->mWeather.size()))
		{
			int ran = rand() % 100;

			if (ran < 20)
				iWeatherId = AIR_TYPE_SUN;
			else if (ran < 40)
				iWeatherId = AIR_TYPE_CLOUDY;
			else if (ran < 60)
				iWeatherId = AIR_TYPE_RAIN;
			else if (ran < 80)
				iWeatherId = AIR_TYPE_WIND;
			else
				iWeatherId = AIR_TYPE_SNOW;
			return;
		}
	}

	int ran = rand()%100;
	if(ran<50)
		return;
	if(air_index<0)
	{
		switch(m_iAirType) {
		case 0: // 随机
			{
				ran = rand()%100;
				if(ran<34)
					iWeatherId = AIR_TYPE_SUN;
				else if(ran<67)
					iWeatherId = AIR_TYPE_CLOUDY;
				else if(ran<90)
					iWeatherId = AIR_TYPE_RAIN;
				else if(ran<100)
					iWeatherId = AIR_TYPE_WIND;
				else
					iWeatherId = AIR_TYPE_SNOW;
			}
			break;
		case 1: //晴好
			{
				ran = rand()%100;
				if(ran<70)
					iWeatherId = AIR_TYPE_SUN;
				else if(ran<90)
					iWeatherId = AIR_TYPE_CLOUDY;
				else if(ran<100)
					iWeatherId = AIR_TYPE_RAIN;
				else if(ran<110)
					iWeatherId = AIR_TYPE_WIND;
				else
					iWeatherId = AIR_TYPE_SNOW;
			}
			break;
		case 2: //阴雨
			{
				ran = rand()%100;
				if(ran<10)
					iWeatherId = AIR_TYPE_SUN;
				else if(ran<35)
					iWeatherId = AIR_TYPE_CLOUDY;
				else if(ran<75)
					iWeatherId = AIR_TYPE_RAIN;
				else if(ran<100)
					iWeatherId = AIR_TYPE_WIND;
				else
					iWeatherId = AIR_TYPE_SNOW;
			}
			break;
		case 3: //小雪
			{
				ran = rand()%100;
				if(ran<15)
					iWeatherId = AIR_TYPE_SUN;
				else if(ran<50)
					iWeatherId = AIR_TYPE_CLOUDY;
				else if(ran<50)
					iWeatherId = AIR_TYPE_RAIN;
				else if(ran<50)
					iWeatherId = AIR_TYPE_WIND;
				else
					iWeatherId = AIR_TYPE_SNOW;
			}
			break;
		case 4: //大雪
			{
				ran = rand()%100;
				if(ran<5)
					iWeatherId = AIR_TYPE_SUN;
				else if(ran<30)
					iWeatherId = AIR_TYPE_CLOUDY;
				else if(ran<30)
					iWeatherId = AIR_TYPE_RAIN;
				else if(ran<30)
					iWeatherId = AIR_TYPE_WIND;
				else
					iWeatherId = AIR_TYPE_SNOW;
			}
			break;
		}
	}
	else
	{
		iWeatherId = air_index + 1;
	}
	updateWeather(iWeatherId);
}

int History::GetCurWeather()
{
	int iRet = 0;
	int iCurRound = GetCurRound();
	if (iCurRound < 1) iCurRound = 1;

	if (history->mRealWeather.size())
	{
		int iMinRound = history->mRealWeather[0];
		for (map<int, int>::iterator eIt = history->mRealWeather.begin()++; eIt != history->mRealWeather.end(); eIt++)
		{
			if (iCurRound >= iMinRound&&iCurRound<iMinRound+eIt->first)
			{
				iRet = eIt->second;
				return iRet;
			}
			else
			{
				iMinRound += eIt->first;
			}
		}
		history->mRealWeather.clear();
	}

	if (history->mWeather.size())
	{
		map<int, int>::iterator iIt = history->mWeather.begin();

		if (iCurRound < iIt->first)
			return iRet;

		for (map<int, int>::iterator eIt = history->mWeather.begin(); eIt != history->mWeather.end(); eIt++)
		{
			int iFirstNum = eIt->first;
			int iSecend = 0;
			bool bEnableBreak = false;
			iIt++;
			if (iIt != history->mWeather.end())
			{
				iSecend = iIt->first;
			}
			else
			{
				bEnableBreak = true;
				iSecend = GetMaxRound()+1;
			}

			if (iCurRound >= iFirstNum&&iCurRound < iSecend)
			{
				iRet = eIt->second;
				break;
			}

			if (bEnableBreak) break;
		}
	}
	return iRet;
}

void History::FindNearestBlackPos(HistoryActorPtr curActor, unsigned short& x, unsigned short& y) {

	HistoryActorPtr actor = history->FindActorPosition(x, y);
	if (actor == NULL&&actor != curActor || history->game_state != GAME_STATE_BATTLE)
		return;

	AStarNodePtr  firstNode, openNodes, lastNode;
	AStarNodePtr curNode = new AStarNode();

	char *tiletag = (char*)ChessAlloc(history->map_width*history->map_height);
	memset(tiletag, 0, history->map_width*history->map_height);

	DPoint direction_point[8];
	{
		direction_point[0].x = 1;
		direction_point[0].y = 0;
		direction_point[1].x = -1;
		direction_point[1].y = 0;
		direction_point[2].x = 0;
		direction_point[2].y = -1;
		direction_point[3].x = 0;
		direction_point[3].y = 1;
		direction_point[4].x = 1;
		direction_point[4].y = 1;
		direction_point[5].x = -1;
		direction_point[5].y = -1;
		direction_point[6].x = 1;
		direction_point[6].y = -1;
		direction_point[7].x = -1;
		direction_point[7].y = 1;
	}
	curNode->x = x;
	curNode->y = y;
	tiletag[curNode->y * history->map_width + curNode->x] = 1;
	firstNode = lastNode = openNodes = curNode;

	while (openNodes)
	{
		for (int index = 0; index < 8; index++)
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
			tiletag[curNode->y * history->map_width + curNode->x] = 1;
			lastNode->NextNode = curNode;
			lastNode = curNode;
			if (history->FindActorPosition(curNode->x, curNode->y) == NULL&&curActor->MapisMoveable(curNode->x,curNode->y))
			{
				x = curNode->x;
				y = curNode->y;
				while (firstNode)
				{
					curNode = firstNode;
					firstNode = firstNode->NextNode;
					SAFE_DELETE(curNode);
				}
				ChessFree(tiletag);
				return;
			}
		}
		openNodes = openNodes->NextNode;
	}
}

void History::FindNearestBlackPosInit(HistoryActorPtr curActor, unsigned short& x, unsigned short& y)
{
	HistoryActorPtr actor = history->FindActorPosition(x, y);
	if (actor == NULL&&actor != curActor)
		return;
	if (actor == curActor)
	{
		return;
	}
	AStarNodePtr  firstNode, openNodes, lastNode;
	AStarNodePtr curNode = new AStarNode();

	char *tiletag = (char*)ChessAlloc(history->map_width*history->map_height);
	memset(tiletag, 0, history->map_width*history->map_height);

	DPoint direction_point[8];
	{
		direction_point[0].x = 1;
		direction_point[0].y = 0;
		direction_point[1].x = -1;
		direction_point[1].y = 0;
		direction_point[2].x = 0;
		direction_point[2].y = -1;
		direction_point[3].x = 0;
		direction_point[3].y = 1;
		direction_point[4].x = 1;
		direction_point[4].y = 1;
		direction_point[5].x = -1;
		direction_point[5].y = -1;
		direction_point[6].x = 1;
		direction_point[6].y = -1;
		direction_point[7].x = -1;
		direction_point[7].y = 1;
	}
	curNode->x = x;
	curNode->y = y;
	tiletag[curNode->y * history->map_width + curNode->x] = 1;
	firstNode = lastNode = openNodes = curNode;

	while (openNodes)
	{
		for (int index = 0; index < 8; index++)
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
			tiletag[curNode->y * history->map_width + curNode->x] = 1;
			lastNode->NextNode = curNode;
			lastNode = curNode;
			if (history->FindActorPosition(curNode->x, curNode->y) == NULL&&curActor->MapisMoveable(curNode->x, curNode->y))
			{
				x = curNode->x;
				y = curNode->y;
				while (firstNode)
				{
					curNode = firstNode;
					firstNode = firstNode->NextNode;
					SAFE_DELETE(curNode);
				}
				ChessFree(tiletag);
				return;
			}
		}
		openNodes = openNodes->NextNode;
	}
}

int History::GetArmyMaxLevel()
{
	int iMax = 1;
	for(int i = 0; i < ownArmyNum; i++)
	{
		if (actors[ownArmy[i]].GetActorLv() > iMax)
		{
			iMax = actors[ownArmy[i]].GetActorLv();
		}
	}
	return iMax;
}

int History::getBustHImg(int actor_id,int xory)
{
	int face_id = actors[actor_id].face_id;
	if (xory == 1)
	{
		for (int i = 0; i < trData->bustHImg_num;i++)
		if (trData->bustHImg[i].face_id == face_id + 1)
			return trData->bustHImg[i].src_x;
		return 120;
	}
	else if (xory == 2)
	{
		for (int i = 0; i < trData->bustHImg_num; i++)
		if (trData->bustHImg[i].face_id == face_id + 1)
			return trData->bustHImg[i].src_y;
		return 88;
	}
	return 0;
}

void History::DealActorJoin(int iActorId, int iLevel,bool memory_join)
{
	for(int i = 0; i < ownArmyNum; i++)
	{
		if(iActorId == ownArmy[i])
			return;
	}

	if (iLevel <= 0)
	{
		iLevel = GetArmyAveLevelDq();
	}

	HistoryActorPtr ptrActor = &actors[iActorId];
	ptrActor->SetActorLv(iLevel);
	if (memory_join || (ptrActor->enable_set_occupation_id&&ptrActor->occupation_id>-1)){}
	else
		ptrActor->occupation_id = trData->actors[iActorId].occupation_id;
	ptrActor->history_occupation_id = ptrActor->GetActorInitialJob();
	ptrActor->relation = ACTOR_RELATION_OWN;
	ptrActor->InitAbility();
	ptrActor->SetDefaultGood(GOOD_TYPE_ALL);
	//内存加入时取消原来的辅助物品
	ptrActor->fashion.init(-1, 1);
	ptrActor->wap_fashion.init(-1, 1);
	ptrActor->assist_good.init(-1, 1);
	if (ptrActor->history_image_fight_id < 400)
	{
		ptrActor->history_image_fight_id = trData->occupation_image_id[ptrActor->occupation_id];
	}
	LoadBattleActorImage(iActorId);

	ownArmy[ownArmyNum++] = iActorId;
}

void History::NewActorJoinOwn(ActorJoinOwnDatePtr ptrAJOD, bool bEnableJoin)
{
	int iActorId = ptrAJOD->getActorId()-1;

	if (iActorId < 0) return;
	HistoryActorPtr ptrActor = &actors[iActorId];

	if (ptrAJOD->getExp()>0)
		ptrActor->exp = ptrAJOD->getExp();
	else
		ptrActor->exp = 0;

	if (ptrAJOD->getLv() > 0)
		ptrActor->SetActorLv(ptrAJOD->getLv());
	else
		ptrActor->SetActorLv(1);

	if (ptrAJOD->getJobID() > 0)
		ptrActor->occupation_id = ptrAJOD->getJobID() -1;
	else
		ptrActor->occupation_id = trData->actors[iActorId].occupation_id;

	if (ptrAJOD->getQuality() > 0)
		ptrActor->actor_quality = ptrAJOD->getQuality();
	else
		ptrActor->actor_quality = 1;

	if (ptrAJOD->getGrade() > 0)
		ptrActor->actor_grade = ptrAJOD->getGrade();
	else
		ptrActor->actor_grade = 1;

	ptrActor->history_occupation_id = ptrActor->GetActorInitialJob();
	ptrActor->relation = ACTOR_RELATION_OWN;

	ptrActor->SetSoulBox(ptrAJOD->getSoulBox());

	for (UInt i = 0; i < ptrAJOD->m_mPractice.size(); i++)
	{
		ptrActor->setPracGrown(i, (float)ptrAJOD->m_mPractice[i]);
	}

	ptrActor->InitAbility();
	LoadBattleActorImage(iActorId);
	ptrActor->weapon.good_id = -1;
	ptrActor->coat.good_id = -1;
	ptrActor->assist_good.good_id = -1;
	ptrActor->fashion.good_id = -1;
	ptrActor->wap_fashion.good_id = -1;

	int fashion1 = ptrAJOD->getFashion1();
	int fashion2 = ptrAJOD->getFashion2();
	int iGender = ptrAJOD->getGender();
	if (iActorId > 0 && trData->actors[iActorId].iGender) //非主角性别读字典表
	{
		iGender = trData->actors[iActorId].iGender;
	}
	ptrActor->mInitFashion.clear();
	if (fashion1 > 0 && fashion2 > 0)
	{
		ptrActor->mFashion[0] = 1;
		ptrActor->mFashion[1] = iGender;
		ptrActor->mFashion[2] = fashion1;
		ptrActor->mFashion[3] = 100;
		ptrActor->mFashion[4] = fashion2;

		ptrActor->mInitFashion[0] = 1;
		ptrActor->mInitFashion[1] = iGender;
		ptrActor->mInitFashion[2] = fashion1;
		ptrActor->mInitFashion[3] = 100;
		ptrActor->mInitFashion[4] = fashion2;
	}

	ptrActor->setAddAbilityGoods(ptrAJOD->mSpecialItem);

	Json::Value jsonGoods = ptrAJOD->getGoodsValue();
	if (jsonGoods.size())
	{
		for (int i = 0; i < (int)jsonGoods.size(); i++)
		{
		 	GoodsElem tempGoogsItem;
		 	tempGoogsItem.loadJson(jsonGoods[i]);
		 	HistoryGoods hisGoods(&tempGoogsItem);
		 	if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
		 	{
		 		ptrActor->weapon.setGood(&hisGoods);
		 	}
		 	else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
		 	{
		 		ptrActor->coat.setGood(&hisGoods);
		 	}
		 	else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
		 	{
		 		ptrActor->assist_good.setGood(&hisGoods);
		 	}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
			{
				ptrActor->fashion.setGood(&hisGoods);
			}
			else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
			{
				ptrActor->wap_fashion.setGood(&hisGoods);
			}
		}
	}

	ptrActor->UpdataGoodEffect();
	if (ptrActor->history_image_fight_id < 400)
	{
		ptrActor->history_image_fight_id = trData->occupation_image_id[ptrActor->occupation_id];
	}
	for (map<int, int>::iterator it = ptrAJOD->mSkillLv.begin(); it != ptrAJOD->mSkillLv.end(); it++)
	{
		ptrActor->SetSelfSkillLevel(it->first, it->second);
	}
	ptrActor->UpdateSkills();

	if (bEnableJoin)
	{
		ownArmy[ownArmyNum++] = iActorId;
	}
}

int History::getGoodsNum(int iGoodId)
{
	int ret = 0;
	if (!trData->m_mGoodsItem[iGoodId]->bIsConsume)
	{
		for (int i = 0; i < goods->size(); i++)
		{
			if (goods->elementAt(i)->good_id == iGoodId)
				ret++;
		}

		for (int i = 0; i < history->ownArmyNum; i++)
		{
			if (history->actors[history->ownArmy[i]].weapon.good_id == iGoodId)
				ret++;
			if (history->actors[history->ownArmy[i]].coat.good_id == iGoodId)
				ret++;
			if (history->actors[history->ownArmy[i]].assist_good.good_id == iGoodId)
				ret++;
			if (history->actors[history->ownArmy[i]].fashion.good_id == iGoodId)
				ret++;
			if (history->actors[history->ownArmy[i]].wap_fashion.good_id == iGoodId)
				ret++;
		}
	}
	else
	{
		ret += history->m_mConsuNum[iGoodId];
	}
	return ret;
}

std::map<int, HistoryGoods*> History::GetAllTreasure(int iModel /*= MODEL_STRENG*/)
{
	map<int, HistoryGoods*> mGoodsMap;
	map<int, HistoryGoods*> mTempMap;

	int iTotalGoods = 0;
	HistoryGoods* ptrGoods = NULL;
	for(int i = 0; i < history->ownArmyNum; i++)
	{
		ptrGoods = &history->actors[history->ownArmy[i]].weapon;
		if(ptrGoods->isNormalGood())
		{
			ptrGoods->equipment_actorId = history->ownArmy[i];
			mTempMap[iTotalGoods++] = ptrGoods;
		}
		ptrGoods = &history->actors[history->ownArmy[i]].coat;
		if(ptrGoods->isNormalGood())
		{
			ptrGoods->equipment_actorId = history->ownArmy[i];
			mTempMap[iTotalGoods++] = ptrGoods;
		}
		ptrGoods = &history->actors[history->ownArmy[i]].assist_good;
		if(ptrGoods->isNormalGood())
		{
			ptrGoods->equipment_actorId = history->ownArmy[i];
			mTempMap[iTotalGoods++] = ptrGoods;
		}
	}

	for(int i = 0; i < history->goods->size(); i++)
	{
		ptrGoods = history->goods->elementAt(i);
		if (!ptrGoods->isFashion())
		{
			ptrGoods->equipment_actorId = -1;
			mTempMap[iTotalGoods++] = ptrGoods;
		}
	}

	for(UInt i = 0, iIndex = 0; i < mTempMap.size(); i++)
	{
		ptrGoods = mTempMap[i];
		if (iModel == MODEL_ENCHANT && ptrGoods->isFmEnable())
		{
			mGoodsMap[iIndex++] = ptrGoods;
		}
		else if (ptrGoods->isForgeEnable())
		{
			if(iModel == MODEL_MAX || (iModel == MODEL_STRENG && ptrGoods->iQuality != ptrGoods->getMaxStar()) || (iModel == MODEL_RECAST && ptrGoods->GetStrengEffect().size()))
			{
				mGoodsMap[iIndex++] = ptrGoods;
			}
		}
	}
	return mGoodsMap;
}

TRImagePtr History::GetActorScaleImage(short actor_id, char* filename, float fScale)
{
	int index = actors[actor_id].history_image_fight_id;
	int index2 = getBattleActorImageId(actor_id);
	int occu_level = actors[actor_id].occupation_id - actors[actor_id].history_occupation_id;
	TRImagePtr pImgActor = NULL;

	if(index < 400)
	{
		for(int i = 0; i < 7; i++)
		{
			if(!pImgActor)
			{
                pImgActor = CreateShrinkImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, i + 1, filename), 1 / fScale, 1);
                if(!pImgActor)
				{
					bool isenable = true;
					for(int j = i - 1; j >= 0; j--)
					{
						
                        pImgActor = CreateShrinkImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, j + 1, filename), 1 / fScale, 1);
                        if(pImgActor)
						{
							isenable = false;
							break;
						}
					}
					if(isenable)
					{
						for(int j = i + 1; j < 7; j++)
						{
							
                            pImgActor = CreateShrinkImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, j + 1, filename), 1 / fScale, 1);
                            if(pImgActor)
								break;
						}
					}
				}
			}
		}
	}
	else
	{
		if(!pImgActor)
		{
			for(int i = 0; occu_level + 1 - i > 0; i++)
			{
				
               pImgActor = CreateShrinkImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index, 1 + occu_level - i, filename), 1 / fScale, 1);
                if(pImgActor)
					break;
			}
			if(!pImgActor)
			{
				
                pImgActor = CreateShrinkImageFile(ChessSprintf("%s%d_%d_%s.png", "img/UnitS/", index - 400, 1, filename), 1 / fScale, 1);
            }
		}
	}
	return pImgActor;
}

int History::GetFashionImgIndex(HistoryActorPtr ptrActor)
{
	int iIndex =(ptrActor->mFashion[1] - 1)*3;
	if (!imgFashion.count(ptrActor->mFashion[2])|| !imgFashionArms.count(ptrActor->mFashion[4]))
		LoadFashionSImg(ptrActor);
	return iIndex;
}

int History::CalculateActorLv(int iExp)
{
	int iLevel = 0;
	for (map<int, NetHeroLvUpExpPtr>::iterator it = trData->net_hero_exp.begin(); it != trData->net_hero_exp.end(); it++)
	{
		if (iExp < it->second->mTotalExp)
		{
			iLevel = it->first-1;
			break;
		}
	}
	return iLevel;
}

int History::CalculateLvUpExp(int iExp)
{
	int need_exp_total = -1;
	for (map<int, NetHeroLvUpExpPtr>::iterator it = trData->net_hero_exp.begin(); it != trData->net_hero_exp.end(); it++)
	{
		if (iExp < it->second->mTotalExp)
		{
			need_exp_total = it->second->mTotalExp;
			break;
		}
	}
	return need_exp_total;
}

void History::initNetDate()
{
	money = m_pPlayerInfo->getGold();
	stone = m_pPlayerInfo->getImprovedStone();
	m_iJade = m_pPlayerInfo->getJade();
	m_iVIP = m_pPlayerInfo->getVIP();
	m_iVIPGift = m_pPlayerInfo->getVIPGift();
	m_iJadeSeal = m_pPlayerInfo->getJadeSeal();

	game_state = GAME_STATE_REDE;

	initNetHeroList();
	initNetGoodsList();
	initNetConsumeList();
	initNetChipList();

	if (m_pPlayerInfo->m_MapTreasure.size() > 0)
	{
		history->mMapDateInfo->setMapTreasure(m_pPlayerInfo->m_MapTreasure);
	}
	else
	{
		map<int, map<int, int> > mMapdate;
		history->mMapDateInfo->setMapTreasure(mMapdate);
	}

	m_mScriptCode.clear();
	m_mScriptCode = m_pPlayerInfo->m_mScriptCode;
}

void History::initNetHeroList()
{
	ownArmyNum = 0;
	for (int i = 0; i < MAX_OWN_ARMY_NUM; i++)
	{
		ownArmy[i] = -1;
	}
	for (map<int, HeroElem*>::iterator it = m_pPlayerInfo->m_mHero.begin(); it != m_pPlayerInfo->m_mHero.end(); it++)
	{
		initNetHeroElem(it->second);
	}
}
void History::initNetGoodsList()
{
	goods->removeAllElements();
	for (map<int, GoodsElem*>::iterator it = m_pPlayerInfo->m_mGoods.begin(); it != m_pPlayerInfo->m_mGoods.end(); it++)
	{
		GoodsElem* goodsElem = it->second;
		int iGoodsId = goodsElem->getGoodID();
		if (trData->m_mGoodsItem.count(iGoodsId) > 0)
		{
			int iHeroId = goodsElem->getHeroID();
			if (iHeroId > 0)
			{
				HistoryActorPtr ptrActor = &actors[iHeroId - 1];
				HistoryGoods hisGoods(goodsElem);
				if (!history->ActorEnableEquipGoods(&hisGoods, iHeroId))
					return;
				if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
				{
					ptrActor->weapon.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
				{
					ptrActor->coat.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
				{
					ptrActor->assist_good.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
				{
					ptrActor->fashion.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
				{
					ptrActor->wap_fashion.setGood(&hisGoods);
				}
				ptrActor->UpdataGoodEffect();
			}
			else
			{
				goods->addElement(new HistoryGoods(goodsElem));
			}
		}
	}
}
void History::initNetConsumeList()
{
	m_mConsuNum.clear();
	m_mConsuNetId.clear();
	for (map<int, ConsumeElem*>::iterator it = m_pPlayerInfo->m_mConsume.begin(); it != m_pPlayerInfo->m_mConsume.end(); it++)
	{
		m_mConsuNum[it->second->getGoodID()] = it->second->getCount();
		m_mConsuNetId[it->second->getGoodID()] = it->second->getNetID();
	}

	for (map<int, ConsumeElem*>::iterator it = m_pPlayerInfo->m_mGifts.begin(); it != m_pPlayerInfo->m_mGifts.end(); it++)
	{
		m_mConsuNum[it->second->getGoodID()] = it->second->getCount();
		m_mConsuNetId[it->second->getGoodID()] = it->second->getNetID();
	}

	for (map<int, ConsumeElem*>::iterator it = m_pPlayerInfo->m_mScriptGoods.begin(); it != m_pPlayerInfo->m_mScriptGoods.end(); it++)
	{
		m_mConsuNum[it->second->getGoodID()] = it->second->getCount();
		m_mConsuNetId[it->second->getGoodID()] = it->second->getNetID();
	}
}
void History::initNetChipList()
{
	mChipSoulDate->ClearChipList();
	for (map<int, ConsumeElem*>::iterator it = m_pPlayerInfo->m_mHeroSoul.begin(); it != m_pPlayerInfo->m_mHeroSoul.end(); it++)
	{
		ConsumeElem* ptrSoul = it->second;
		mChipSoulDate->AddActorSoulNumByGoodsId(ptrSoul->getGoodID(), ptrSoul->getCount(), ptrSoul->getNetID());
	}

	for (map<int, ConsumeElem*>::iterator it = m_pPlayerInfo->m_mEquipChip.begin(); it != m_pPlayerInfo->m_mEquipChip.end(); it++)
	{
		ConsumeElem* ptrChip = it->second;
		mChipSoulDate->AddEqSoulNumByGoodsId(ptrChip->getGoodID(), ptrChip->getCount(), ptrChip->getNetID());
	}
}
void History::initNetHeroElem(HeroElem* ptrHData, bool bEnableJoin)
{
	ActorJoinOwnDate NetActorDate;
	NetActorDate.setActorId(ptrHData->getHeroID());
	NetActorDate.setExp(ptrHData->getEXP());
	NetActorDate.setLv(ptrHData->getLV());
	NetActorDate.setJobID(ptrHData->getJobID());
	NetActorDate.setFashion1(ptrHData->getFashion1());
	NetActorDate.setFashion2(ptrHData->getFashion2());
	NetActorDate.setQuality(ptrHData->getQuality());
	NetActorDate.setGrade(ptrHData->getGrade());
	NetActorDate.setGender(ptrHData->getGender());
	NetActorDate.setSoulBox(ptrHData->geteSoulBox());
	NetActorDate.mSkillLv = ptrHData->mSkillLv;
	NetActorDate.m_mPractice = ptrHData->m_mPractice;
	NetActorDate.mSpecialItem = ptrHData->mSpecialItem;
	NewActorJoinOwn(&NetActorDate, bEnableJoin);
}



void History::parseErrorMsg(Json::Value val, string strTag)
{
	if(val["code"].isInt())
	{
		if (val["code"].asInt() == 124) //连接超时，重新连接
		{
			PopLayer* ptrPop = PopLayer::create(POP_REQ_AGAIN, NULL);
			ptrPop->setTag(strTag);
			Director::getInstance()->getRunningScene()->addChild(ptrPop, 98);
			return;
		}

		if(val["error"].isString())
		{
			string strNotice = val["error"].asString();
			if(val["code"].isInt())
			{
				int iCode = val["code"].asInt();
				if(trData->m_mErrorMSG.count(iCode))
				{
					strNotice = trData->m_mErrorMSG[iCode];
				}
			}
			ITools::getInstance()->createGlobalPrompt(strNotice, false);
		}
		else
		{
			Json::Value errVal = val["error"];
			if(errVal["error"].isString())
			{
				if(errVal["isOpen"].isInt() && errVal["isOpen"].asInt())
				{
					ChessViewTool::getInstance()->setScreenDefaultSize();
					Chess::getInstance()->removeAllScreen();
					ChessApplication::getInstance()->removeAllFunctionLayer();
					BTLayer* ptrLogo = NULL;
					ptrLogo = LogoLayer::create(true);
					ChessApplication::getInstance()->AddFunctionLayer(ptrLogo);
					PopLayer* ptrPop = PopLayer::create(POP_FWQ_WH, ptrLogo);
					ptrPop->setTitle(errVal["error"].asCString());
					Director::getInstance()->getRunningScene()->addChild(ptrPop, 98);
				}
				else
				{
					ITools::getInstance()->createGlobalPrompt(errVal["error"].asString(), false);
				}
			}
			else
			{
				Json::Value error = errVal["error"];
				int iCode = -1;
				string errorMsg = "";
				if(error["code"].isInt())
				{
					iCode = error["code"].asInt();
				}
				if(error["message"].isString())
				{
					errorMsg = error["message"].asString();
				}

				if(iCode != -1)
				{
					string strNotice = "";
					if (trData->m_mErrorMSG.count(iCode))
					{
						strNotice = trData->m_mErrorMSG[iCode];
					}
					else
					{
						strNotice = StringUtils::format("code=%d, %s", iCode, errorMsg.c_str());
					}
					ITools::getInstance()->createGlobalPrompt(strNotice, false);
				}
			}
		}
	}
}

bool History::ReduceNetGoods(map<int, int> mGoodsList, bool bOnlyTest/* = false*/)
{
	bool bEnableReduce = true;
	for (map<int, int>::iterator mIt = mGoodsList.begin(); mIt != mGoodsList.end(); mIt++)
	{
		int iGoodsId = mIt->first;
		int iCount = mIt->second;

		if ((iGoodsId == 10000 && history->m_iJade < iCount) || (iGoodsId == 10001 && history->money < iCount) 
			||(iGoodsId == 10002 && history->stone < iCount) || (iGoodsId == 10003 && history->m_iJadeSeal < iCount) )
		{
			bEnableReduce = false;
			break;
		}
		if (iGoodsId >= 10000 && iGoodsId<=10004)
			continue;

		switch (trData->m_mGoodsItem[iGoodsId]->iItId)
		{
		case 997:
		case 998:
		case 999:
		{
			if (history->m_mConsuNum.count(iGoodsId) && history->m_mConsuNum[iGoodsId]< iCount)
				bEnableReduce = false;;
		}
		break;
		}

		if (!bEnableReduce)
			break;
	}

	if (!bEnableReduce)
		return false;

	if (bOnlyTest)
		return bEnableReduce;

	for (map<int, int>::iterator mIt = mGoodsList.begin(); mIt != mGoodsList.end(); mIt++)
	{
		int iGoodsId = mIt->first;
		int iCount = mIt->second;

		if (iGoodsId == 10000)
			history->m_iJade -= iCount;
		else if (iGoodsId == 10001)
			history->money -= iCount;
		else if (iGoodsId == 10002)
			history->stone -= iCount;
		else if (iGoodsId == 10003)
			history->m_iJadeSeal -= iCount;


		if (iGoodsId >= 10000 && iGoodsId <= 10004)
			continue;

		switch (trData->m_mGoodsItem[iGoodsId]->iItId)
		{
		case 997:
		case 998:
		case 999:
		{
			history->m_mConsuNum[iGoodsId] -= iCount;
		}
		break;
		}
	}

	return true;
}

Sprite* History::getGoodsSprite(int iGoodsId)
{
	return  ReadSpriteNormal(history->GetGoodsImagePath(iGoodsId));
}

Texture2D* History::getGoodsTexture(int iGoodsId)
{
	return GetSpriteTexture(history->GetGoodsImagePath(iGoodsId));
}

void History::updateGuide()
{
	if (modConst->m_bGuideSwitch == true)
	{
		auto _ID = history->m_iCurWarId;
		auto _a = _ID - 94;
		if (_a <= FIRSET4_GUIDE &&_a >= FIRSET_GUIDE)
		{
			if (history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(_a)) == 0  )
			{

				GuideLayer::Factory(_a)->updateGuide();

			}
		}
	}
	
	
}
void History::resetGuide(int GuideType)
{
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
	{
		GuideLayer::reset_guide = true;
	}
	
	////Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.1), CC_CALLBACK_1(GuideLayer::setreset_guide)));
	////Director::getInstance()->getRunningScene()->scheduleOnce(schedule_selector(GuideLayer::setreset_guide));
	//DelayTime* ptrDelayTime = DelayTime::create(0.1);
	//CallFuncN* ptrCallFN = CallFuncN::create(CC_CALLBACK_1(GuideLayer::setreset_guide, this));
	//Sequence* ptrSequence = Sequence::create(ptrDelayTime, ptrCallFN, NULL);
	//Director::getInstance()->getRunningScene()->runAction(ptrSequence);
}
void History::updateGuide(int GuideType)
{
	if (modConst->m_bGuideSwitch == true)
	{
		int __BeforeGuide = 0;
		if (GuideType<ZHUJIEMIAN6_GUIDE)
		{
			__BeforeGuide = history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(GuideType - EMIL_GUIDE));
		}
		int __BeforOneGuide = 0;
		/*if ( GuideType == GOTOPubDrink_GUIDE)
		{
			__BeforOneGuide = history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(GuideType - 2));
		}
		else {*/
		if (GuideType>EMIL_GUIDE) {
			__BeforOneGuide = history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(GuideType - 1));
		}
		//}
		
		int __NowGuide = history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(GuideType));
		if (GuideType >= 1 && GuideType != 5 ) {
			if (GuideType > EMIL_GUIDE )
			{
				if (GuideType >= HELP2_GUIDE&&GuideType <= HELP4_GUIDE)
				{
					GuideLayer::Factory(GuideType)->updateGuide();
					return;
				}

				if ( __NowGuide == 0 && __BeforOneGuide == 1 && history->mMapDateInfo->isPassed(94 + GuideType - EMIL_GUIDE))
				{
					GuideLayer::Factory(GuideType)->updateGuide();
					return;
				}

				
			}
			else if (GuideType == EMIL_GUIDE && __NowGuide == 0 && __BeforeGuide == 1 && history->mMapDateInfo->isPassed(96))
			{
				GuideLayer::Factory(GuideType)->updateGuide();
			}

		}
		else if(GuideType == 0)
		{
			GuideLayer::Factory(GuideType)->updateGuide();
		}
	}
}



void History::popOpenNotice(int iWarId)
{
	if (trData->m_mNoticeOpen.count(iWarId))
	{
		if(iWarId != 1 || ownArmyNum == 4) //ID == 1代表开启酒馆
		{
			m_iOpenId = iWarId;
		}
	}
}

void History::SetCurRound(int iSet/* = -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		curRound = iSet^iEncryptKey;
	}
	else
	{
		curRound = (GetCurRound() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[KEY_CURROUND] = iEncryptKey;
}
int History::GetCurRound()
{
	return curRound^m_EncryptKey[KEY_CURROUND];
}
void History::SetMaxRound(int iSet /*= -1*/, int iAdd /*= 0*/)
{
	int iEncryptKey = rand();
	if (iSet >= 0)
	{
		maxRound = iSet^iEncryptKey;
	}
	else
	{
		maxRound = (GetMaxRound() + iAdd) ^ iEncryptKey;
	}
	m_EncryptKey[KEY_MAXROUND] = iEncryptKey;
}
int History::GetMaxRound()
{
	return maxRound^m_EncryptKey[KEY_MAXROUND];
}

string History::packGoodsAdd(int iGoodsId, int iNetId, int iCount, bool bAddConsume /*= true*/)
{
	if(iGoodsId >= 10000 && iGoodsId <= 10004) //金币、勾玉、强化石不做处理
	{
		return "";
	}

	HistoryGoods hisGoods(iGoodsId);
	if(hisGoods.isConsume() || hisGoods.isPreWarConsume() || hisGoods.isPlotGoods())
	{
		if(!hisGoods.isConsume() || bAddConsume)
		{
			m_mConsuNum[iGoodsId] += iCount;
			m_mConsuNetId[iGoodsId] = iNetId;

			if(m_mConsuNum[iGoodsId] > GOODS_MAX_STORE)
			{
				m_mConsuNum[iGoodsId] = GOODS_MAX_STORE;
			}
		}
	}
	else if(hisGoods.isSoul())
	{
		mChipSoulDate->AddActorSoulNumByGoodsId(iGoodsId, iCount, iNetId);
	}
	else if(hisGoods.isPiece())
	{
		mChipSoulDate->AddEqSoulNumByGoodsId(iGoodsId, iCount, iNetId);
	}
	else if (hisGoods.isHero()) 
	{

	}
	else
	{
		HistoryGoods* ptrGoods = new HistoryGoods(iGoodsId);
		ptrGoods->iNewGoods = true;
		ptrGoods->setNetID(iNetId);
		goods->addElement(ptrGoods);
	}

	return StringUtils::format(LL("huodess"), hisGoods.GetGoodsName(iCount));
}

void History::packGoodsReduce(int iGoodsId, int iNetId, int iCount)
{
	HistoryGoods hisGoods(iGoodsId);
	if(hisGoods.isConsume() || hisGoods.isPreWarConsume() || hisGoods.isPlotGoods())
	{
		m_mConsuNum[iGoodsId] -= iCount;
	}
	else if(hisGoods.isSoul())
	{
		mChipSoulDate->ReduceActorSoulNumByGoodsId(iGoodsId, iCount);
	}
	else if(hisGoods.isPiece())
	{
		mChipSoulDate->ReduceEqSoulNumByGoodsId(iGoodsId, iCount);
	}
	else
	{
		for(int i = 0; i < goods->size(); i++)
		{
			if(goods->elementAt(i)->getNetID() == iNetId)
			{
				goods->removeElementAt(i);
				break;
			}
		}
	}
}

map<int, string> History::onHttpGoodsAdd(Json::Value valRes, bool bIsNotice /*= true*/, bool bIsSweep /*= false*/)
{
	map<int, string> mStrNotice;
	int iIndex = 0;

	if (bIsNotice)
	{
		if(valRes["gold"].isInt())
		{
			int iAddGold = valRes["gold"].asInt() - money;
			money += iAddGold;
			if(iAddGold > 0)
			{
				mStrNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("jinbi"), iAddGold);
			}
		}

		if(valRes["jade"].isInt())
		{
			int iAddJade = valRes["jade"].asInt() - m_iJade;
			m_iJade += iAddJade;
			if(iAddJade > 0)
			{
				mStrNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("gouyu"), iAddJade);
			}
		}

		if(valRes["improvedStone"].isInt())
		{
			int iAddStone = valRes["improvedStone"].asInt() - stone;
			stone += iAddStone;
			if(iAddStone > 0)
			{
				mStrNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("qianghuashi"), iAddStone);
			}
		}

		if(valRes["jadeSeal"].isInt())
		{
			int iAddLong = valRes["jadeSeal"].asInt() - m_iJadeSeal;
			m_iJadeSeal += iAddLong;
			if(iAddLong > 0)
			{
				mStrNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("longbi"), iAddLong);
			}
		}
	}
	else
	{
		ParseInt(money, valRes["gold"]);
		ParseInt(m_iJade, valRes["jade"]);
		ParseInt(stone, valRes["improvedStone"]);

	}

	ParseInt(m_iJadeSeal, valRes["jadeSeal"]);	//玉玺

	//英雄增加经验
	map<int, int> mActorLvUp;
	map<int, int> mNetScriptHeroList;
	map<int, int> ScriptActorElem = WinConditionTool::getInstance()->GetScriptActorList();
	if (!valRes["hero"].isNull())
	{
		Json::Value JsonHero = valRes["hero"];
		if (JsonHero.isArray())
		{
			for (UInt i = 0; i < JsonHero.size(); i++)
			{
				int iHeroId = JsonHero[i]["heroID"].asInt();
				if (!ScriptActorElem.count(iHeroId))
				{
					int iLv = JsonHero[i]["lv"].asInt();
					int iEXP = JsonHero[i]["EXP"].asInt();

					HistoryActorPtr ptrEActor = &history->actors[iHeroId - 1];

					int iActorOldQuality = ptrEActor->actor_old_quality;
					if (iActorOldQuality > 0)
					{
						ptrEActor->actor_quality = iActorOldQuality;
						ptrEActor->actor_old_quality = 0;
					}

					int iCurLevel = ptrEActor->GetActorLv();
					int iOldLevel = ptrEActor->GetActorOldLv();
					if (iOldLevel > 0)
						iCurLevel = iOldLevel;
					ptrEActor->SetActorLv(iLv);
					ptrEActor->exp = iEXP;

					bool bEnableRef = true;
					if (iCurLevel < iLv)
					{
						ptrEActor->InitFiveAllAbility();
						mActorLvUp[iHeroId] = 1;
						if (iHeroId == 1)
						{
							HeroElem* mainHero = history->m_pPlayerInfo->getMainHero();
							if (mainHero) {
								mainHero->setLV(iLv);
							}
							StatConfig::submitExtendData(LogoLayer::getDataForSDKAnalys("levelChange",iLv).data());
						}
						ptrEActor->UpdataGoodEffect();
						bEnableRef = false;
					}
					if (bEnableRef&&iOldLevel > 0)
					{
						ptrEActor->InitFiveAllAbility();
						ptrEActor->UpdataGoodEffect();
					}
					ptrEActor->UpdateSkills();
					ptrEActor->SetActorOldLv(0);
				}
				else
				{
					int iOwnListIndex = ScriptActorElem[iHeroId];
					for (int j = iOwnListIndex; j < history->ownArmyNum - 1; j++)
					{
						history->ownArmy[j] = history->ownArmy[j + 1];
					}
					history->ownArmyNum--;

					HistoryActorPtr CurActor = &history->actors[iHeroId - 1];
					CurActor->init(iHeroId - 1);
					HeroElem InitHeroE;
					InitHeroE.loadJson(JsonHero[i]);
					history->initNetHeroElem(&InitHeroE);
					//history->popOpenNotice(1);
					if (history->actors[iHeroId - 1].GetActorLv() > 1)
					{
						mActorLvUp[iHeroId] = 1;
					}
					mNetScriptHeroList[iHeroId] = 1;
					history->actors[iHeroId - 1].SetActorOldLv(0);
					history->actors[iHeroId - 1].actor_old_quality = 0;
				}
			}
		}
	}

	Json::Value valGoods = valRes["goods"];
	if(valGoods.isArray())
	{
		for(UInt i = 0; i < valGoods.size(); i++)
		{
			if(bIsSweep) //扫荡数组多了一层
			{
				if (valGoods[i].isArray())
				{
					for(UInt j = 0; j < valGoods[i].size(); j++)
					{
						ConsumeElem consuElem;
						consuElem.loadJson(valGoods[i][j]);
						packGoodsAdd(consuElem.getGoodID(), consuElem.getNetID(), consuElem.getCount());
					}
				}
			}
			else
			{
				ConsumeElem consuElem;
				consuElem.loadJson(valGoods[i]);
				string strNotice = packGoodsAdd(consuElem.getGoodID(), consuElem.getNetID(), consuElem.getCount());
				if(bIsNotice && strNotice != "")
				{
					mStrNotice[iIndex++] = strNotice;
				}
			}
		}
	}

	return mStrNotice;
}

string History::GetFashionGoodsPath(int iGoodsId,HistoryActorPtr ptrActor)
{
	GoodsItem* ptrGItem = trData->m_mGoodsItem[iGoodsId];
	string imgPath = "";

	if (ptrGItem->m_iGoodType == GOOD_TYPE_WAP_FASHING)
	{
		imgPath = StringUtils::format("img/Items/%d.png", ptrGItem->iImgId);
	}
	else if (ptrGItem->m_iGoodType == GOOD_TYPE_FASHING)
	{
		int iGender = 1;
		if (!ptrActor)
		{
			if (ptrGItem->mHid.size() && ptrGItem->mHid.begin()->first && ptrGItem->mHid.begin()->first <= trData->actor_num)
			{
				if (ptrGItem->mHid.count(1))
				{
					iGender = history->m_pPlayerInfo->getMainHero()->getGender();
				}
				else
				{
					iGender = trData->actors[ptrGItem->mHid.begin()->first - 1].iGender;
				}
			}
		}
		else
		{
			iGender = trData->actors[ptrActor->actor_id].iGender;
			if (ptrActor->mFashion[0] == 1)
			{
				iGender = ptrActor->mFashion[1];
			}
		}
		if (iGender <= 0) iGender = 1;
		imgPath = StringUtils::format("img/Items/%d_%d.png", ptrGItem->iImgId, iGender);
	}

	return imgPath;
}

string History::GetGoodsImagePath(int iGoodsId)
{
	string imgPath = "";
	GoodsItem* ptrGItem = trData->m_mGoodsItem[iGoodsId];
	if (ptrGItem->m_iGoodType == GOOD_TYPE_WAP_FASHING || ptrGItem->m_iGoodType == GOOD_TYPE_FASHING)
	{
		return GetFashionGoodsPath(iGoodsId);
	}
	else
	{
		imgPath = StringUtils::format("img/Items/%d.png", ptrGItem->iImgId);
	}
	return imgPath;
}

void History::LoadFashionSImg(HistoryActorPtr ptrActor)
{
	if (ptrActor->mFashion[0])
	{
		int iFashionIndex = ptrActor->mFashion[2];
		if (!imgFashion.count(iFashionIndex))
		{
			imgFashion[iFashionIndex][S_IMG_MOV] = CreateImageFile(ChessSprintf("img/Nunits/1_%d_mov.png", iFashionIndex));
			imgFashion[iFashionIndex][S_IMG_ATK] = CreateImageFile(ChessSprintf("img/Nunits/1_%d_atk.png", iFashionIndex));
			imgFashion[iFashionIndex][S_IMG_SPC] = CreateImageFile(ChessSprintf("img/Nunits/1_%d_spc.png", iFashionIndex));

			imgFashion[iFashionIndex][3 + S_IMG_MOV] = CreateImageFile(ChessSprintf("img/Nunits/2_%d_mov.png", iFashionIndex));
			imgFashion[iFashionIndex][3 + S_IMG_ATK] = CreateImageFile(ChessSprintf("img/Nunits/2_%d_atk.png", iFashionIndex));
			imgFashion[iFashionIndex][3 + S_IMG_SPC] = CreateImageFile(ChessSprintf("img/Nunits/2_%d_spc.png", iFashionIndex));
		}

		int iWapFashionIndex = ptrActor->mFashion[4];
		if (!imgFashionArms.count(iWapFashionIndex))
		{
			imgFashionArms[iWapFashionIndex][S_IMG_MOV] = CreateImageFile(ChessSprintf("img/Nunits/100_%d_mov.png", iWapFashionIndex));
			imgFashionArms[iWapFashionIndex][S_IMG_ATK] = CreateImageFile(ChessSprintf("img/Nunits/100_%d_atk.png", iWapFashionIndex));
			imgFashionArms[iWapFashionIndex][S_IMG_SPC] = CreateImageFile(ChessSprintf("img/Nunits/100_%d_spc.png", iWapFashionIndex));
		}
	}

}

//iActorId已经减1
void History::SetNetExternalEquip(int iActorId, int iEquipId, int iType)
{
	NetExternalEquipPtr ptrNetExtEqu = NULL;
	HistoryActorPtr ptrActor = NULL;

	if (iActorId >= 0)
		ptrActor = &history->actors[iActorId];
	else
		return;

	if(iType == 0 && trData->net_external_equip.count(iEquipId))
		ptrNetExtEqu = trData->net_external_equip[iEquipId];
	else if (iType == 1 && trData->net_elite_equip.count(iEquipId))
	{
		int ieActorLv = ptrActor->GetActorLv();
		for (map<int, NetExternalEquipPtr>::iterator eIt = trData->net_elite_equip[iEquipId].begin(); eIt != trData->net_elite_equip[iEquipId].end(); eIt++)
		{
			if ((int)eIt->second->level_area.size() >= 2)
			{
				int iMinNum = eIt->second->level_area[0];
				int iMaxNum = eIt->second->level_area[1];
				if (iMinNum > iMaxNum)
				{
					iMinNum = eIt->second->level_area[1];
					iMaxNum = eIt->second->level_area[0];
				}
				if (ieActorLv >= iMinNum&&ieActorLv <= iMaxNum)
				{
					ptrNetExtEqu = eIt->second;
					break;
				}

			}
		}
	}
	else if (iType == 2 && trData->net_automatic_equip.count(ptrActor->occupation_id+1))
	{
		int ieActorLv = ptrActor->GetActorLv();
		if (history->mNet72EquipLevel.count(0) && history->mNet72EquipLevel[0] > 0)
			ieActorLv = history->mNet72EquipLevel[0];
		for (map<int, NetExternalEquipPtr>::iterator eIt = trData->net_automatic_equip[ptrActor->occupation_id + 1].begin(); eIt != trData->net_automatic_equip[ptrActor->occupation_id + 1].end(); eIt++)
		{
			if ((int)eIt->second->level_area.size() >= 2)
			{
				int iMinNum = eIt->second->level_area[0];
				int iMaxNum = eIt->second->level_area[1];
				if (iMinNum > iMaxNum)
				{
					iMinNum = eIt->second->level_area[1];
					iMaxNum = eIt->second->level_area[0];
				}
				if (ieActorLv >= iMinNum&&ieActorLv <= iMaxNum)
				{
					ptrNetExtEqu = eIt->second;
					break;
				}

			}
		}
	}

	if (ptrNetExtEqu)
	{
		bool bInitActorAbility = false;

		if (ptrNetExtEqu->boss.size() > 1)
		{
			ptrActor->mElitePlus.clear();
			ptrActor->mElitePlus = ptrNetExtEqu->boss;
			if (ptrActor->mElitePlus[1] <= -100)  ptrActor->mElitePlus[1] = -99;
			if (ptrNetExtEqu->boss[0] == 1 || ptrNetExtEqu->boss[0] == 3)
			{
				history->m_mBossActor[ptrActor->actor_id + 1] = 1;
			}
			bInitActorAbility = true;
		}

		if (ptrNetExtEqu->quality > 0)
		{
			ptrActor->actor_quality = ptrNetExtEqu->quality;
			bInitActorAbility = true;
		}
		else if (iType == 2|| iType == 1)
		{
			ptrActor->actor_quality = ceill(1.0*ptrActor->GetActorLv()/LEVELGROWTH);
			bInitActorAbility = true;
		}

		if (bInitActorAbility||history->mEnemyElitePlus.size())
			ptrActor->InitFiveAllAbility();

		if (ptrNetExtEqu->weapon[0] > 0)
		{
			ptrActor->weapon.init(ptrNetExtEqu->weapon[0], 1);
			ptrActor->weapon.GetOrSetQuality(ptrNetExtEqu->weapon[1], false);
			for (int i = 2; i < 5; i++)
			{
				int streffid = ptrNetExtEqu->weapon[i];
				if (streffid > 0)
				{
					ptrActor->weapon.AddStrengEffect(streffid);
				}
			}
		}
		if (ptrNetExtEqu->armour[0] > 0)
		{
			ptrActor->coat.init(ptrNetExtEqu->armour[0], 1);
			ptrActor->coat.GetOrSetQuality(ptrNetExtEqu->armour[1], false);
			for (int i = 2; i < 5; i++)
			{
				int streffid = ptrNetExtEqu->armour[i];
				if (streffid > 0)
				{
					ptrActor->coat.AddStrengEffect(streffid);
				}
			}
		}
		if (ptrNetExtEqu->assist[0] > 0)
		{
			ptrActor->assist_good.init(ptrNetExtEqu->assist[0], 1);
			ptrActor->assist_good.GetOrSetQuality(ptrNetExtEqu->assist[1], false);
			for (int i = 2; i < 5; i++)
			{
				int streffid = ptrNetExtEqu->assist[i];
				if (streffid > 0)
				{
					ptrActor->assist_good.AddStrengEffect(streffid);
				}
			}
		}
		ptrActor->UpdataGoodEffect();

		int iSkillId = 0, iSkillLvMax = ptrNetExtEqu->skill_lv;

		if (iSkillLvMax < 0)
		{
			iSkillLvMax += ptrActor->GetActorLv();

			if (iSkillLvMax <= 0) iSkillLvMax = 1;
		}

		if (iSkillLvMax > 0)
		{
			int iActorSkillIndex = trData->actors[ptrActor->actor_id].skill;
			if (iActorSkillIndex > 0)
			{
				NetHeroSkillPtr ptrNHSP = trData->net_hero_skill[iActorSkillIndex];
				map<int, map<int, int> > mSkillLevel = ptrActor->GetCurSkillLvList();
				for (map<int, map<int, int> >::iterator it = mSkillLevel.begin(); it != mSkillLevel.end(); it++)
				{
					iSkillId = it->first;
					int iRealMaxLv = iSkillLvMax > ptrActor->GetActorLv() ? ptrActor->GetActorLv() : iSkillLvMax;
					int iRealSkillLv = iRealMaxLv - ptrNHSP->skill[iSkillId] + 1;
					if (iRealSkillLv > 0)
					{
						ptrActor->SetCurSkillLevel(iSkillId, iRealSkillLv);
						ptrActor->SetSelfSkillLevel(iSkillId, iRealSkillLv);
						ptrActor->m_iExternalEquip = 1;
					}
				}
			}
		}
	}

}

void History::updateWeather(int iWeatherId)
{
	if (m_iCurAir != iWeatherId)
	{
		m_iCurAir = iWeatherId;
		//BattleScreen* ptrScreen = (BattleScreen*)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
		//if (ptrScreen)
		//{
		//	ptrScreen->updateWeather();
		//}
	}
}

string History::getRestoreNotice(Json::Value jsonVal)
{
	string strNotice = "";
	Json::Value jsonGoods = jsonVal["goods"];
	if (!jsonGoods.isNull() && jsonGoods.isArray()) //礼包
	{
		for(UInt i = 0; i < jsonGoods.size(); i++)
		{
			ConsumeElem consuElem;
			consuElem.loadJson(jsonGoods[i]);
			HistoryGoods hisGoods(consuElem.getGoodID());
			strNotice += hisGoods.GetGoodsName(consuElem.getCount());

			if(i < jsonGoods.size() - 1)
			{
				strNotice += LL("dunhao");
			}
		}
	}
	else
	{
		if(jsonVal["jade"].isInt())
		{
			int iAddJade = jsonVal["jade"].asInt() - m_iJade;
			if(iAddJade > 0)
			{
				strNotice += StringUtils::format("%sx%d%s", LL("gouyu"), iAddJade, LL("dunhao"));
			}
		}

		if(jsonVal["jadeSeal"].isInt())
		{
			int iAddJadeSeal = jsonVal["jadeSeal"].asInt() - m_iJadeSeal;
			if(iAddJadeSeal > 0)
			{
				strNotice += StringUtils::format("%sx%d", LL("yuxi"), iAddJadeSeal);
			}
		}
	}
	return strNotice;
}

void History::setRecruitTotle(int iNum)
{
	m_iRecruitTotle = iNum;
}
int History::getRecruitTotle()
{
	return m_iRecruitTotle;
}

void History::setRatioList(int iActorId, int iRatio)
{
	m_mRatioList[iActorId] = iRatio;
}
int History::getRatioList(int iActorId)
{
	if (m_mRatioList.count(iActorId))
		return m_mRatioList[iActorId];
	else
		return 0;
}

void History::clearRatioList()
{
	m_mRatioList.clear();
}

map<int, int> History::getRatioListMap()
{
	return m_mRatioList;
}

bool History::ActorEnableEquipGoods(HistoryGoodsPtr ptrGoods, int iActorId)
{
	bool bRet = false;
	int iGoodsType = ptrGoods->GetGoodsType();
	if (iGoodsType == GOOD_TYPE_FASHING || iGoodsType == GOOD_TYPE_WAP_FASHING)
	{
		if (trData->m_mGoodsItem[ptrGoods->good_id]->mHid.count(iActorId))
			bRet = true;
	}
	else if (iGoodsType == GOOD_TYPE_WEAPON || iGoodsType == GOOD_TYPE_COAT || iGoodsType == GOOD_TYPE_ASSIST)
	{
		if (!(trData->occupation_good_info[history->actors[iActorId - 1].occupation_id][ptrGoods->getItId()] == 0 || !ptrGoods->EnableEquip(iActorId)))
			bRet = true;
	}

	if (!bRet)
	{
		Director::getInstance()->getRunningScene()->addChild(ShortcutFunctionLayer::create(NULL, SHORTCUT_TYPE_JSYX, 0, 0));
	}

	return bRet;
}
