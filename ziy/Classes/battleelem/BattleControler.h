#ifndef _BattleControler_H_
#define _BattleControler_H_
enum{
	BATTLE_STATE_RUN_SCRIPT= 0,
	BATTLE_STATE_WAIT_COMMAND,
	BATTLE_STATE_SHOW_MOVE_REGION,
	BATTLE_STATE_ACTOR_MOVE,
	BATTLE_STATE_WAIT_ACTION,
	BATTLE_STATE_DO_ACTION,
	BATTLE_STATE_ACTOR_LEAVE,
	BATTLE_STATE_SHOW_ATTACK_REGION,
	BATTLE_STATE_SHOW_ACTION_GAIN,
//	BATTLE_STATE_SHOW_ACTION_RESULT_OPPO,
//	BATTLE_STATE_SHOW_ACTION_RESULT_OWN,
	BATTLE_STATE_INIT,
	BATTLE_STATE_SHOW_BATTLE_START,
	BATTLE_STATE_ENDING_WIN,
	BATTLE_STATE_ENDING_LOSE,
	BATTLE_STATE_SHOW_START_OWN,
	BATTLE_STATE_SHOW_START_ENEMY,
	BATTLE_STATE_ACTOR_RECOVER,
	BATTLE_STATE_SPECIAL_OCCU_RECOVER,
	BATTLE_STATE_PK,
	BATTLE_STATE_BATTLE_END,
	BATTLE_STATE_SHOW_ATTACK_MOVE_REGION,
	BATTLE_STATE_SHOW_TAKE_TURNS,
	BATTLE_STATE_EXIT,
	BATTLE_STATE_SHOW_DISPATCH_MOVE_REGION,
	BATTLE_STATE_SHOW_DISPATCH_MOVE
};
class BattleControler
{
public:	
	BattleControler();
	~BattleControler();
	void onInit();
	void onDestroy();
	void Ontimer();
	void RoundStartFriend();
    void RoundStartFriendNew();
	void CancelMoveRegion();
	void ShowMoveRegion();
	void MoveActor(int i);	
	void DispatchActorMove(int i);
	void SelectAction(int action);
	void ActorDoAction();
	void SelectSkill(SkillAttrPtr skill);
	void SelectGood(GoodsItem* good);
	void EnterBattle();
	ActionSingleGainPtr getSingleGain(int x, int y, int setMost);
	void setActorLastXAndY(int lastX, int lastY);
public:
	static BattleActorPtr getBattleActor(int actor_id);
	static void ClearGain();
private:
	void GenerateActorList();
	void RoundStartOwn();
	void RoundStartEnemy();
	void CancelMoveActor();
	void needAssault(HistoryActorPtr cur_actor, HistoryActorPtr oppo_actor, int &x, int &y);
	void needAssault(HistoryActorPtr cur_actor, HistoryActorPtr oppo_actor);
	int ActorAssault(HistoryActorPtr cur_actor, HistoryActorPtr oppo_actor, int &x, int &y,int step = 1,bool bOnlyPoint = false);
    bool canSkillBack(HistoryActorPtr oppo_actor);
    void processSkillBack();
	void Resetbust_style();
	void SetActionfinish(); //武将行动是否结束，用于处理二次移动，无限移动，无极攻击
	int GetActorChangeWeatherId();
	void startNextRound();
	void autoRunAction();
	void changeActionSide();
public:
	BattleScreenPtr scr;
	int battle_state;
	int need_frame;
	BattleAI ai;
	ScriptHandler handler;	
	SkillAttrPtr curSkill;
	GoodsItem* curGood;	
	bool isShowTitleBG;   //是否显示战斗标题背景页
	DList<BattleActor>* actor_list;
	int last_x;
	int last_y;
	int m_iLastDir; //移动前朝向
public:
	static int auto_run;
private:	
	int need_run_script;
	int need_round_add;
	int next_round_flag;
	ChessPtr chess;
private:
	bool m_userOp;//用户操作时不要自动选择角色
};

#endif