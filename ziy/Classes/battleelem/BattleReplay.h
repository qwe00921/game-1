//
//  BattleReplay.h
//  TRGame
//
//  Created by Jimmy on 17/03/02.
//
//

#ifndef _BattleReplay_H_
#define _BattleReplay_H_

enum ReplayStatus
{
	REPLAY_STATUS_NONE,
	REPLAY_STATUS_SAVE,
    REPLAY_STATUS_READY_TO_SAVE,
    REPLAY_STATUS_READY_TO_REPLAY,
	REPLAY_STATUS_REPLAY
};

class ReplayActor : public Ref
{
public:
	ReplayActor();
	~ReplayActor();
	void initActor(HistoryActor* ptrActor);
	void setActor(); //替换内存数据为当前录像数据
	void resetActor(); //重置内存数据
	int getActorId();

	void readActor(DMemoryReader* reader);
	void saveActor(DMemoryWriter* writer);
private:
	int m_iActorId;
	int m_iBaseHP;
	int m_iAddHP;
	int m_iBaseMP;
	int m_iAddMP;
	int m_iLevel;

	int m_iInitBaseHP;
	int m_iInitAddHP;
	int m_iInitBaseMP;
	int m_iInitAddMP;
	int m_iInitLv;
};

class ReplayRound : public Ref
{
public:
	ReplayRound();
	~ReplayRound();
	void readRound(DMemoryReader* reader);
	void saveRound(DMemoryWriter* writer);

	CC_SYNTHESIZE(int, m_iCurAir, CurAir); //天气
};

class BattleReplay : public Ref
{
public:
	static BattleReplay* getInstance();
	~BattleReplay();
	bool loadReplay();
	void saveReplay();
	void startSaveReplay();
	void stopSaveReplay();
	void saveBattleActor(map<int, int> mActorId);
	map<int, int> getBattleActor();
	void saveBattleAI(BattleAI* ai);
	void saveActionGain(ActionGain* gain);
	void startReplay();
	BattleAI* getBattleAI(bool move = true);
	ActionGain* getActionGain(bool move = true);
	ReplayRound* getRepRound(int iRound);
	void saveMove();
	void saveOrDeleteMove();
private:
	BattleReplay();
	string getFileName();
private:
	static BattleReplay* m_pInstance;

	DMemoryWriter* m_dwBattleAI;
	DMemoryWriter* m_dwActionGain;
	string m_strVersion;
	map<int, ReplayActor*> m_mActor;
	map<int, BattleAI*> m_mBattleAI;
	map<int, ActionGain*> m_mActionGain;
	map<int, ReplayRound*> m_mRepRound;
	int m_iAIIndex;
	int m_iGainIndex;

	CC_SYNTHESIZE(int, m_iScriptIndex, ScriptIndex); //剧本索引
	CC_SYNTHESIZE(int, m_iStatus, CurStatus); //重播模式
};

#endif //_BattleReplay_H_