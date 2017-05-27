//
//  BackpackLayer.h
//  TRGame
//
//  Created by Jimmy on 16/4/5.
//
//

#ifndef _BACKPACKLAYER_H_
#define  _BACKPACKLAYER_H_

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class BPGoodsSprite : public Sprite
{
public:
	static BPGoodsSprite* create();
	virtual bool init();
	void updateSprite(HistoryGoods* ptrGoods);
private:
	Sprite* m_spIcon;
	Sprite* m_spChip;
	Sprite* m_spFrame;
	Sprite* m_spStar;
	Label* m_labNum;
};

class PackGoods
{
	CC_SYNTHESIZE(HistoryGoodsPtr, m_ptrGoods, HisGood);//物品对象
	CC_SYNTHESIZE(Sprite*, m_bg, BG);//背景
	CC_SYNTHESIZE(BPGoodsSprite*, m_spGoods, SPGoods);//物品精灵
};

class BackpackLayer : public BTLayer
{
public:
	CREATE_FUNC(BackpackLayer);
	bool init();
	BackpackLayer();
	~BackpackLayer();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateShow(int index);

private:
	void initControl();
	void initGoodsMap();
	void updateGoodsMap();
	void updateCurGoods();
	void updateGoodsView();

	void callBackClose(Ref* sender);
	void callBackNext(Ref* sender);
	void callBackLast(Ref* sender);
	void callBackGold(Ref* sender);
	void callBackShowSwitch(Ref* sender);
private:
	map<int, map<int, HistoryGoods*> > m_mGoodsMap;
	map<int, HistoryGoods*> m_mCurGoods;
	map<int, PackGoods*> m_mPackGoods;
	map<int, MenuItemSprite*> m_mBtnType;
	Menu* m_pBtnMenu;
	Label* m_pLabMoney;
	Label* m_pLabPage;
	int m_iCurIndex;
	int m_iMaxIndex;
	int m_iCurType;
};

#endif // _BACKPACKLAYER_H_