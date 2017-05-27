#ifndef _HISTORY_H_
#define _HISTORY_H_

#include "HistoryGoods.h"
#include "LoginRequest.h"
#include "PlayerInfo.h"
#include "MapDateInfo.h"
#include "ChipSoulDate.h"
#include "FunctionClass.h"
#include "TitleBarLayer.h"

#define MAX_OWN_ARMY_NUM 50
#define MAX_BATTLE_IMAGE_NUM 10000
#define MAX_BATTLE_BUST_IMAGE_NUM 50000
#define MAX_HEAD_NUM 2000
#define SKILL_IMG_NUM 31
#define EFFECT_AREA_NUM 30
#define HIT_AREA_NUM 50
#define MAX_FASHION_NUM 1000
#define GLOBAL_VAR_MAPID 99999

enum
{
	KEY_CURROUND,//当前回合数
	KEY_MAXROUND// 最大回合数
};

enum
{
	S_IMG_MOV = 0,//S形象.mov
	S_IMG_ATK = 1,//S形象.atk
	S_IMG_SPC = 2,//S形象.spc
};

using namespace std;

class History//战场信息，
{
public:
	History();
	~History();
	void loadInitialData(); //加载原始数据，仅开始游戏加载一次
	void loadUserData();	//加载用户数据，切换帐号重新加载

	void clearSceneData(); //清除战场数据，战斗结束调用
	void clearUserData(); //清除用户数据，切换帐号调用
	void clearAllData(); //清除所有数据，退出游戏调用
	void InitNoHisData();

	void ArmyCome(short actor_id, short level_add, bool memory_join=false);//memory_join表示是否是内存加入
	void ArmyLeave(short actor_id,int left_goods = 1);//left_goods 1:留下物品 0：带走物品
	int AddBattleOwn(short actor_id);
	void LoadBattleActorImage(short actor_id);
	void LoadBattleActorImageEx(TRImagePtr*& imgPtr, short actor_id, char* filename);
	void loadBuyActorImage(short actor_id);
	HistoryActorPtr FindActor(short actor_id);
	HistoryActorPtr FindRedeActor(short actor_id);
	HistoryActorPtr FindBattleActor(short actor_id);
	HistoryActorPtr FindActorPosition(int x,int y);
	HistoryActorPtr FindOwnActorPosition(HistoryActorPtr curActor,int x,int y);
	HistoryActorPtr FindOppoActorPosition(HistoryActorPtr curActor,int x,int y);
	HistoryActorPtr FindNearestOppoActor(HistoryActorPtr curActor);
	HistoryActorPtr FindBattleRelationActor(HistoryActorPtr curActor,int iActorId);
	void CalcBaseLevel();
	void SpecialCalcBaseLevel();
	void CalcUserGoodLevel();
	void CenterActor(HistoryActorPtr battleactor) ;
	int getActiveOwnNum();
	int getActorOwnIndex(short actor_id);
	int getActorFriendIndex(short actor_id);
	int getActorEnemyIndex(short actor_id);
	HistoryActorPtr getNextActiveEnemy();
	HistoryActorPtr getNextActiveFriend();
	HistoryActorPtr getNextActiveOwn();
	HistoryActorPtr getNextDeadActor();
	TRImagePtr GetFaceImage(unsigned short actor_id, bool bSmall = false);
	TRImagePtr GetGoodImage(unsigned short good_id);
	void CenterPos(int x,int y);
	void CheckPosActor(HistoryActorPtr battleactor);
	void CheckPos(int x, int y);
	void GenerateMapArray(int index);
	void GenerateMapArray(char* data,int index);
	void FreeMapArray();

	bool HasElem(int x, int y, short occupation_id);
	int getBattleOwnNum();
	int getBattleEnemyNum();
	int getBattleAssistNum();
	int getBattleOwnNum(int region, int x1, int y1, int x2, int y2);
	int getBattleFriendNum(int region, int x1, int y1, int x2, int y2);
	int getBattleEnemyNum(int region, int x1, int y1, int x2, int y2);
	int getBattleAssistNum(int region, int x1, int y1, int x2, int y2); //获取援军数量 
	HistoryActorPtr getNextOwn(HistoryActorPtr cur_actor);
	HistoryActorPtr getNextEnemy(HistoryActorPtr cur_actor);
	HistoryActorPtr getNextFriend(HistoryActorPtr cur_actor);
	HistoryActorPtr getPreOwn(HistoryActorPtr cur_actor);
	HistoryActorPtr getPreEnemy(HistoryActorPtr cur_actor);
	HistoryActorPtr getPreFriend(HistoryActorPtr cur_actor);
	void changeAir(int air_index);
	int GetCurWeather();
	void updateWeather(int iWeatherId);
	int getBattleActorImageId(short actor_id);
	void FindNearestBlackPos(HistoryActorPtr curActor, unsigned short& x, unsigned short& y);
	void FindNearestBlackPosInit(HistoryActorPtr curActor, unsigned short& x, unsigned short& y);
	void ArmyComeReturnToLeave(short actor_id);//武将内存加入 还原离开时的状态
	int getBarrierMoveSpend(int occupation_id, int x, int y); //获取障碍物消耗的移动力
	int GetArmyAveLevel();
	int GetArmyAveLevelDq();
	int GetArmyMaxLevel(); //获取我军队列的最大等级
	int getBustHImg(int actor_id,int xory);//获得半身像表情在半身像图上的位置1返回X，2返回Y
	void DealActorJoin(int iActorId, int iLevel, bool memory_join =false); //武将加入的相关处理
	int getGoodsNum(int iGoodId);//获得本方拥有该物品的数量
	std::map<int, HistoryGoods*> GetAllTreasure(int iModel = MODEL_STRENG);
	TRImagePtr GetActorScaleImage(short actor_id, char* filename, float fScale);
	int GetFashionImgIndex(HistoryActorPtr ptrActor);//获取时装,iType:1 移动/2 攻击/ 3 防御,iManId: 1 女/2 男
	void initNetDate();
	void initNetHeroList();
	void initNetGoodsList();
	void initNetConsumeList();
	void initNetChipList();
	void initNetHeroElem(HeroElem* ptrHData, bool bEnableJoin = true);
	int CalculateActorLv(int iExp);
	int CalculateLvUpExp(int iExp);
	void parseErrorMsg(Json::Value val, string strTag); //解析网络错误
	void NewActorJoinOwn(ActorJoinOwnDatePtr ptrAJOD,bool bEnableJoin = true);//获取武将加入信息
	bool ReduceNetGoods(map<int,int> mGoodsList,bool bOnlyTest = false);
	string packGoodsAdd(int iGoodsId, int iNetId, int iCount, bool bAddConsume = true);
	void packGoodsReduce(int iGoodsId, int iNetId, int iCount);
	string getRestoreNotice(Json::Value jsonVal); //订单恢复提醒
	map<int, string> onHttpGoodsAdd(Json::Value valRes, bool bIsNotice = true, bool bIsSweep = false); //礼包、扫荡等获得物品的处理
	//根据物品ID获取相应的精灵、纹理
	Sprite* getGoodsSprite(int iGoodsId);
	Texture2D* getGoodsTexture(int iGoodsId);
	int getBaseLevel(short armNum);
	int getBaseLevelSpecial();
	void updateGuide();
	void resetGuide(int GuideType);
	void updateGuide(int GuideType);
	void popOpenNotice(int iWarId); //弹出酒馆、关卡等开启提示
	void SetCurRound(int iSet = -1,int iAdd = 0);
	int GetCurRound();
	void SetMaxRound(int iSet = -1, int iAdd = 0);
	int GetMaxRound();
	string GetFashionGoodsPath(int iGoodsId, HistoryActorPtr ptrActor = NULL);
	string GetGoodsImagePath(int iGoodsId);
	void LoadFashionSImg(HistoryActorPtr ptrActor);
	void SetNetExternalEquip(int iActorId, int iEquipId, int iType = 0);//type 0：72指令-88自动配装 1：72指令-89精英自动配装 2：自动配装
	void setRecruitTotle(int iNum);
	int getRecruitTotle();
	void setRatioList(int iActorId,int iRatio);
	int getRatioList(int iActorId);
	void clearRatioList();
	map<int, int> getRatioListMap();
	bool ActorEnableEquipGoods(HistoryGoodsPtr ptrGoods,int iActorId);
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////全局数据，无需重置
	map<string, TRImagePtr> mFashionFace;
	TRImagePtr* imgMov;
	TRImagePtr* imgAtc;
	TRImagePtr* imgSpc;
	TRImagePtr* imgHead;//头像
	TRImagePtr* imgSmallFace;//小头像
	map<int, TRImagePtr> m_mImgGoods;
	TRImagePtr imgBG;
	TRImagePtr imgLandSwamp;
	TRImagePtr imgLandForest;
	TRImagePtr imgLandGrass;
	TRImagePtr imgLandSnow;
	TRImagePtr imgLandRiver_ahead[2];
	TRImagePtr imgLandRiver_back[2];
	TRImagePtr imgLandRiver_left[2];
	TRImagePtr imgRiver_ahead[2];
	TRImagePtr imgRiver_back[2];
	TRImagePtr imgRiver_left[2];
	TRImagePtr imgShield[8];
	TRImagePtr imgDoubleSeid[14];
	SSCHAR setting_flag[9];
	LoginInfo* m_pLoginInfo;
	int m_iAirArray[30];
	map<int, int> m_mActScene; //活动副本
	HistoryActorPtr actors;	//所有武将信息
	map<int, int> m_mBossActor;
	BTLayer* m_layHome; //主页
	int m_iPatchVersion; //补丁版本号
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////用户数据，切换帐号重置
	DList<HistoryGoods>* goods;
	map<int, int> m_mConsuNum; //消耗品库存
	map<int, int> m_mConsuNetId; //消耗品网络ID
	int money; 
	int stone;
	int m_iJade; //勾玉
	int m_iJadeSeal;//玉玺
	//int m_iLongCoin; //骨龙币
	int ownArmyNum;
	short* ownArmy; //保存人物id
	int game_scene; //关卡ID
	int game_state; //当前模式
	SSCHAR var[MAX_CHAR_VAR_NUM];
	short var2[MAX_INT_VAR_NUM];
	int* var3[4096];
	int bg_music_index;
	PlayerInfo* m_pPlayerInfo;
	MapDateInfo* mMapDateInfo;
	ChipSoulDate* mChipSoulDate;
	ArmyFuBenBattle* mArmyFuBenBattle;
	int mArmyLevel;
	int m_iBattleType; //战斗模式
	int m_iBattleStage;//战场难度
	int m_iCurWarId; //当前模式战场ID
    int curCityId;//当前城市id
	Layer* mTitleLayer;
	BTLayer* m_layTitleBar;	//标题栏（头像，金币，勾玉等）
	map<int, map<int,int> > mRomanceOpenData;//演义堂开启情况
	bool m_bGuide[20]; //新手引导开关
	int m_iActivityEntry; //活动副本入口 1、主城界面； 2、战天下界面
	int m_iOpenId; //开启新副本的Id
	//用户信息、原PlayerInfo类成员
	int m_iVIP; //VIP状态
	int m_iVIPGift; //VIP礼包状态
	int m_iMailNum; //未读邮件数
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////战斗数据，进战场重置
	int isFreeMove;
	char event_name[30];
	char scene_name[30];
	char battle_name[30];
	char win_condition[500];
	int RedeActorNum;
	HistoryActorPtr* RedeActor;
	int need_show_land;
	int initilized;
	int readscreen_bust_one;
	int readscreen_bust_two;
	int readscreen_bust_paint;
	int is_paint_bust;
	int battlescreen_paint_bust_offset;
	HistoryActorPtr cur_actor;
	int r_script_index;
	int extend_value;
	int battleNum;	//武将出征数量
	int battleOwnNum;
	HistoryActorPtr* battleOwn;
	int battleFriendNum;
	HistoryActorPtr* battleFriend;
	int battleEnemyNum;
	HistoryActorPtr* battleEnemy;
	short enemy_skipper_index; //敌方主将的角色id
	short own_skipper_index; //我方主将的角色id
	short is_victory_area;
	short victory_x1;
	short victory_y1;
	short victory_x2;
	short victory_y2;
	short m_iCurAir;
	short m_iAirType;
	short level_add;
	short base_level;
	int curActionSide; //此次动作的阵营
	DList<MapElem>* elem_list;
	int BG_index;
	UUCHAR** map_array;
	int curmap_index;
	int map_width;
	int map_height;
	int scr_x;
	int scr_y;
	int max_amy_num;
	short* out_amy;
	short* in_amy;
	bool m_bWaitCommand; //战场中只有等待命令时才可以存档
	int mScriptValidate;
	bool bEnble5e;
	DShortArray Actor1AttackActor2;
	int iExtraAddMaxRound;//额外增加的回合数
	int m_iRunAction;
	map<int, int> net_win_condition;
	bool is_battle_win;
	map<int, map<int, int> >m_mScriptCode;
	map<int, int> m_m12ScriptCon;

	map<int, int> mWeather;
	map<int, int> mRealWeather;
	map<int, map<int, TRImage*> > imgFashion;
	map<int, map<int, TRImage*> > imgFashionArms;
	map<int, int> mLadderEActor;
	map<int, int> mNet72EquipActor;
	map<int, map<int,int> > mEnemyElitePlus;
	map<int, int> mNet72EquipLevel;
	bool m_bAutoRun; //自动行动，即托管
	bool m_bTakeTurns; //轮流行动
	int m_iRecruitTotle;//可加入我方武将总数
	map<int,int> m_mRatioList;//副将加成id
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	map<int, int> m_EncryptKey;
	int maxRound;
	int curRound;//回合数从1开始
};

extern HistoryPtr history;

#endif //#ifndef _ACTION_H_