//
//  PubBoxLayer.h
//  TRGame
//
//  Created by Jimmy on 16/11/01.
//
//

#ifndef _PubBoxLayer_H_
#define  _PubBoxLayer_H_

#include "Defines.h"

enum 
{
	REWARD_DRINK = 0,
	REWARD_BOX = 1
};

class SoulBoxInfo
{
public:
	SoulBoxInfo();
	~SoulBoxInfo();
	bool loadJson(Json::Value valJson);
private:
	CC_SYNTHESIZE(int, m_iHotId, HotID); //热门武将ID
	CC_SYNTHESIZE(int, m_iPrice, Price); //购买价格
	CC_SYNTHESIZE(string, m_strTime, Content); //时间描述
	map<int, int> m_mGoodsId; //每日热门
};

class PubBoxLayer : public BTLayer
{
public:
	PubBoxLayer();
	~PubBoxLayer();
	static PubBoxLayer* create(SoulBoxInfo* ptrSoul, EventDelegate* ptrDelegate);
	virtual bool init(SoulBoxInfo* ptrSoul, EventDelegate* ptrDelegate);
private:
	void initControl();

	void callBackBuy(Ref* sender);
	void callBackClose(Ref* sender);
private:
	SoulBoxInfo* m_ptrSoul;
	EventDelegate* m_pDelegate;
};

class PubRewardLayer : public BTLayer
{
public:
	PubRewardLayer();
	~PubRewardLayer();
	static PubRewardLayer* create(map<int, ConsumeElem*> mReward, int iType, EventDelegate* ptrDelegate, int iPrice = 0);
	virtual bool init(map<int, ConsumeElem*> mReward, int iType, EventDelegate* ptrDelegate, int iPrice);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();

	void callBackAgain(Ref* sender);
	void callBackConfirm(Ref* sender);
private:
	map<int, ConsumeElem*> m_mReward;
	int m_iType;
	EventDelegate* m_pDelegate;
	int m_iPrice;
};

#endif // _PubBoxLayer_H_