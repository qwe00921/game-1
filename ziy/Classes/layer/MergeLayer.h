//
//  MergeLayer.h
//  TRGame
//
//  Created by Jimmy on 16/09/09.
//
//

#ifndef _MergeLayer_H_
#define _MergeLayer_H_

#include "Defines.h"
#include "GoodsSprite.h"

class MaterialSprite : public BTSprite
{
public:
	static MaterialSprite* create();
	virtual bool init();
	virtual bool onTouchBegan(cocos2d::CCTouch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::CCTouch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::CCTouch *touch, cocos2d::Event *unused_event);

	void updateSprite(int iGoodsId, int iCount);
	void updateSprite(HistoryGoods* ptrGoods);
	void setCallback(const ccMenuCallback& callback);
	void removeMaterial();
	bool isFinished();
	int getStoneNum();
	int getNetId();
private:
	void initControl();
private:
	HistoryGoods* m_ptrGoods;
	int m_iGoodsId;
	int m_iCount;
	Sprite* m_spGoodsBG;
	Sprite* m_spIcon;
	Sprite* m_spChip;
	Sprite* m_spPlus;
	Sprite* m_spFrame;
	Sprite* m_spStar;
	Label* m_labStore;
	Label* m_labUsed;

	bool m_bIsClick;
	bool m_bIsFinished;
	ccMenuCallback _callback;
};

class MergeTypeLayer : public BTLayer
{
public:
	static MergeTypeLayer* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);
	MergeTypeLayer();
	~MergeTypeLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void initGoodsMap();
	void updateDisplay();

	void callBackLast(Ref *sender);
	void callBackNext(Ref *sender);
private:
	EventDelegate* m_ptrDelegate;
	Sprite* m_spLayBG;
	int m_iCurPage;
	int m_iMaxPage;
	int m_iClickIndex;

	map<int, Rect> m_mRect;
	map<int, Sprite*> m_mGoodsMap;
	map<int, Sprite*> m_mSpPage;
};

class MergeSelectLayer : public BTLayer
{
public:
	MergeSelectLayer();
	~MergeSelectLayer();
	static MergeSelectLayer* create(int iGoodsId, MaterialSprite* spMaterial, EventDelegate* ptrDelegate);
	virtual bool init(int iGoodsId, MaterialSprite* spMaterial, EventDelegate* ptrDelegate);
	virtual void updateState();
private:
	void initControl();
	void initGoodsMap();
	void updateDisplay();

	void callBackLast(Ref *sender);
	void callBackNext(Ref *sender);
	void callBackSelect(Ref *sender);
	void callBackClose(Ref *sender);
private:
	int m_iGoodsId;
	MaterialSprite* m_spMaterial;
	EventDelegate* m_pDelegate;
	int m_iSelType;

	map<int, HistoryGoods*> m_mPtrGoods;
	map<int, Vec2> m_mPostion;
	map<int, GoodsSprite*> m_mSpGoods;
	map<int, Sprite*> m_mSpPage;

	int m_iCurPage;
	int m_iMaxPage;
	int m_iSelectIndex;
};

class MergeLayer : public BTLayer
{
public:
    CREATE_FUNC(MergeLayer);
    virtual bool init();
	MergeLayer();
	~MergeLayer();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void updateShow(int index);
	virtual void updateState();
private:
	void initControl();
	void initGoodsMap(int iType);
	void updateItemBox();
	void updateMaterial(int iGoodsId);
	string getNetIdData();

	void onHttpRequest();
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onHttpReqSucceed(Json::Value resVal);
	void playGoodsNotice(float fDelay);

	void callBackType(Ref *sender);
	void callBackLast(Ref *sender);
	void callBackNext(Ref *sender);
	void callBackMaterial(Ref *sender);
	void callBackGoods(Ref *sender);
	void callBackMerge(Ref *sender);
	void callBackClose(Ref *sender);
private:
	int m_iCurPage;
	int m_iMaxPage;
	int m_iClickIndex;
	int m_iGoodsId;

	Sprite* m_spSelect;
	Sprite* m_spGoods;
	Sprite* m_spGold;
	Label* m_labType;
	Label* m_labGoods;
	Label* m_labGold;
	Label* m_labPage;
	Label* m_labStone1;
	Label* m_labStone2;
	Label* m_labCount; //合成物品的数目

	map<int, Sprite*> m_mSpItemBG;
	map<int, Sprite*> m_mSpGoods;
	map<int, int> m_mGoodsId;
	map<int, MaterialSprite*> m_mSpMaterial;

	map<int, string> m_strNotice;
	int m_iNoticeIndex;
};

#endif //_MergeLayer_H_