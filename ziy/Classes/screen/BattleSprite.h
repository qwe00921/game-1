//
//  BattleSprite.h
//  TRGame
//
//  Created by Jimmy on 16/07/14.
//
//

#ifndef _BattleSprite_H_
#define _BattleSprite_H_

#include "Defines.h"

enum 
{
	LABEL_SKILL = 0, //技能标签
	LABEL_SHORT = 1, //战场警告，场景名等
	LABEL_LONG = 2, //人物对话，剧本独白等
	LABEL_MAX
};

class ActorInfoSprite : public Sprite
{
public:
	CREATE_FUNC(ActorInfoSprite);
	virtual bool init();
	void updateSprite(HistoryActor* ptrActor, LandformAttr* ptrLand);
	void updateBuff(HistoryActor* ptrActor);
private:
	void initControl();
private:
	Sprite* m_spBG;
	Sprite* m_spHpBar;
	Sprite* m_spMpBar;
	Label* m_labHp;
	Label* m_labMp;
	Label* m_labName;
	Label* m_labJobLv;
	Label* m_labRelation;
	Label* m_labLand;
	Label* m_labGain;

	map<int, string> m_mStrRelation;
	map<int, Color3B> m_mColor3B;
	map<int, Sprite*> m_mspBuff;
};

class ActionGainSprite : public Sprite
{
public:
	CREATE_FUNC(ActionGainSprite);
	virtual bool init();
	void updateSprite(HistoryActor* ptrActor, int iGainHp, int iGainMp);
private:
	void initControl();
	void updateSpriteBar(float fInterval);
private:
	Sprite* m_spBG;
	Sprite* m_spHpBar;
	Sprite* m_spMpBar;
	Label* m_labHp;
	Label* m_labMp;
	Label* m_labName;
	Label* m_labLv;

	HistoryActor* m_ptrActor;
	int m_iFrame;
	float m_fHpRate;
	float m_fMpRate;

	map<int, Color3B> m_mColor3B;
};

class LandInfoSprite : public Sprite
{
public:
	CREATE_FUNC(LandInfoSprite);
	virtual bool init();
	void updateSprite(int iPointX, int iPointY);
private:
	void initControl();
	void hideSprite(float fInterval);
private:
	Sprite* m_spBG;
	Sprite* m_spLand;
	Label* m_labLand;
	Label* m_labPostion;
	Label* m_labEffect;
};

class LabelSprite : public Sprite
{
public:
	CREATE_FUNC(LabelSprite);
	virtual bool init();
	void updateSprite(string strLabel, int iType, float iDelay = 0.0f);

	void setClickHide(bool bIsHide); //支持单击隐藏，默认支持
	bool isClickHide();
private:
	void initControl();
	void hideSprite(float fInterval);
private:
	bool m_bIsHide;
	Sprite* m_spShortL;
	Sprite* m_spShortR;

	map<int, Sprite*> m_mspBG;
	map<int, Label*> m_mNotice;
};

class GainNumSprite : public Sprite
{
public:
	static GainNumSprite* create(int iGainNum, BattleActor* ptrActor);
	virtual bool init(int iGainNum, BattleActor* ptrActor);
private:
	void initControl();
private:
	int m_iGainNum;
	BattleActor* m_ptrActor;
};

#endif //_BattleSprite_H_