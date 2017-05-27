//
//  GQRankLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/12.
//
//

#ifndef _GQRANKLAYER_H_
#define  _GQRANKLAYER_H_

#include "Defines.h"

class RankHero
{
	CC_SYNTHESIZE(int, m_iRankId, RankID); //名次
	CC_SYNTHESIZE(int, m_iIsExist, IsExist); //是否存在
	CC_SYNTHESIZE(int, m_iFashion1, Fashion1); //时装1
	CC_SYNTHESIZE(int, m_iFashion2, Fashion2); //时装2
	CC_SYNTHESIZE(int, m_iFashion3, Fashion3); //时装3
	CC_SYNTHESIZE(int, m_iFashion4, Fashion4); //时装4
	CC_SYNTHESIZE(int, m_iGender, Gender); //性别
	CC_SYNTHESIZE(int, m_iScore, Score); //得分
	CC_SYNTHESIZE(string, m_strName, Name); //姓名
};

class RankHeroSprite : public Sprite
{
public:
	static RankHeroSprite* create(RankHero* ptrRank);
	virtual bool init(RankHero* ptrRank);
};

class GQRankLayer : public BTLayer
{
public:
	static GQRankLayer* create(int iWarId);
	bool init(int iWarId);
	GQRankLayer();
	~GQRankLayer();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();

	void rankHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onRankSucceed(Json::Value valRes);
private:
	map<int, RankHero*> m_mRankHero;
	int m_iWarId;
};

#endif // _GQRANKLAYER_H_