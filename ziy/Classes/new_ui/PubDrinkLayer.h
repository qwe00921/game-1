//
//  PubDrinkLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/01.
//
//

#ifndef _PUBDRINKLAYER_H_
#define  _PUBDRINKLAYER_H_

#include "Defines.h"

class DrinkerItem
{
	CC_SYNTHESIZE(int, iKeyId, KeyId); //物品Key值
	CC_SYNTHESIZE(int, iHeroId, HeroId); //武将ID
	CC_SYNTHESIZE(int, iChipId, ChipId); //碎片ID
	CC_SYNTHESIZE(int, iIsSell, IsSell); //是否已卖
	CC_SYNTHESIZE(int, iPosIndex, PosIndex); //位置坐标索引
};

class PubDrinkLayer : public BTLayer
{
public:
	PubDrinkLayer();
	~PubDrinkLayer();
	CREATE_FUNC(PubDrinkLayer);
	virtual bool init();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateShow(int index);
	virtual void callBackConfirm();
private:
	void initControl();
	void updateHeroAction();
	map<int, int> getRandMap(int iSize);

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	bool onBoxInitSucceed(Json::Value valRes);
	bool onBoxBuySucceed(Json::Value valRes);
	bool parseJson(Json::Value valRes);

	void callBackRefresh(Ref* sender);
	void callBackClose(Ref* sender);
	void callBackNull(Ref *sender);
private:
	TitleBarLayer* m_layTitleBar;
	Sprite* m_spPubBG;
	Label* m_labRefTime;
	Armature* m_armBox;
	Menu* m_menuNull;

	map<int, DrinkerItem*> m_mDrinker;
	map<int, Sprite*> m_mSpHero;
	map<int, Label*> m_mLabName;

	int m_iRefresh; //手动刷新次数
	int m_iMaxRef; //最大刷新次数
	int m_iClickHero;
	int m_iBuyType; //刷新&购买魂匣
	int m_iBoxPrice; //魂匣价格
	bool m_bClickBox;
	int m_guideStep;
};

class PubPopupLayer : public BTLayer
{
public:
	PubPopupLayer();
	~PubPopupLayer();
	static PubPopupLayer* create(DrinkerItem* ptrDrinker, EventDelegate* ptrDelegate);
	virtual bool init(DrinkerItem* ptrDrinker, EventDelegate* ptrDelegate);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();

	void buyHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onBuySucceed(Json::Value valRes);

	void callBackClose(Ref *sender);
	void callBackBuy(Ref* sender);
private:
	DrinkerItem* m_ptrDrinker;
	EventDelegate* m_pDelegate;

	Menu* m_pMenu;
	int m_iBuyType;
};

#endif // _PUBDRINKLAYER_H_