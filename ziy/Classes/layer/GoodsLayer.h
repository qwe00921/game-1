//
//  GoodsLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#ifndef _GOODSLAYER_H_
#define  _GOODSLAYER_H_

#include "Defines.h"

class GoodsLayer : public BTLayer
{
public:
	static GoodsLayer* create(int iGoodsId);
	bool init(int iGoodsId);

	static GoodsLayer* create(HistoryGoods* ptrGoods);
	bool init(HistoryGoods* ptrGoods);

	GoodsLayer();
	~GoodsLayer();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
private:
	void initControl();
	void callBackSuit(Ref* sender);
private:
	HistoryGoods* m_ptrGoods;
	Sprite* m_spLayBG;
};

#endif //_GOODSLAYER_H_