//
//  GoodsSprite.h
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#ifndef _GOODSSPRITE_H_
#define  _GOODSSPRITE_H_

#include "Defines.h"

class GoodsSprite : public BTSprite
{
public:
	static GoodsSprite* create(int iGoodsId, int iCount = 1, float fScale = 0.8f);
	virtual bool init(int iGoodsId, int iCount, float fScale);

	static GoodsSprite* create(HistoryGoods* ptrGoods, float fScale = 1.0f);
	virtual bool init(HistoryGoods* ptrGoods, float fScale);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void setSupDrag(bool bIsDrag);
	void setCallback(const ccMenuCallback& callback);
	void updateSprite(HistoryGoods* ptrGoods);
	void updateSprite(int iGoodsId,int iCount = 1);
	void AddGoodsLayer();
private:
	void initControl();
private:
	int m_iGoodsId;
	int m_iCount;
	HistoryGoods* m_ptrGoods;
	float m_fScale;

	Sprite* m_spGoodsBG;
	Sprite* m_spImg;
	Sprite* m_spFrame;
	Sprite* m_spStar;
	Label* m_labNum;

	bool m_bIsClick;
	bool m_bIsDrag;

	Vec2 m_posTouch;

	ccMenuCallback _callback;
};


#endif // _GOODSSPRITE_H_