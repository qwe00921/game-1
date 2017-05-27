//
//  TreasureLayer.h
//  TRGame
//
//  Created by Jimmy on 16/3/30.
//
//

#ifndef _TREASURELAYER_H_
#define  _TREASURELAYER_H_

#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TreasureElem
{
	CC_SYNTHESIZE(int, m_iGoodId, GoodID);//物品ID
	CC_SYNTHESIZE(Sprite*, m_spIcon, Icon);//物品图标
	CC_SYNTHESIZE(Label*, m_labName, Name);//物品名称
};

class TreasureLayer : public BTLayer
{
public:
	CREATE_FUNC(TreasureLayer);
	bool init();
	TreasureLayer();
	~TreasureLayer();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void callBackItemClass(Ref* sender);
	void callBackLastView(Ref* sender);
	void callBackNextView(Ref* sender);
	void callBackClose(Ref* sender);
private:
	void initControl();
	void initGoodsMap(); //初始化图鉴集合
	void updateCurGoods(int iTypeId);
	void updateBtnState(int iIndex);
	void updateGoodsView();
	int getTreasureStore(); //计算库存已有宝物
private:
	map<int, int> m_mapGoods;
	map<int, int> m_mCurGoods;
	map<int, Sprite*> m_pBtnSelect;
	map<int, TreasureElem*> m_mTreasureElem;
	map<int, MenuItemSprite*> m_mpMenuItem;

	Menu* m_pBtnType;
	Menu* m_pBtnMenu;
	Label* m_labStore;
	Label* m_labPage;

	int m_iOldIndex;
	int m_iCurIndex;
	int m_iMaxIndex;
};

class TreasureInfoLayer : public BTLayer
{
public:
	static TreasureInfoLayer* create(int iGoodId);
	bool init(int iGoodId);
	TreasureInfoLayer();
	~TreasureInfoLayer();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
private:
	HistoryGoodsPtr m_pHisGoods;
};

#endif // _TREASURELAYER_H_