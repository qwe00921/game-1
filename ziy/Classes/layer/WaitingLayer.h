//
//  WaitingLayer.h
//  TRGame
//
//  Created by Jimmy on 16/04/18.
//
//

#ifndef _WAITINGLAYER_H_
#define _WAITINGLAYER_H_

#include "Defines.h"

class WaitingLayer : public BTLayer
{
public:
	static WaitingLayer* create(bool bIsDelay = true);
	virtual bool init(bool bIsDelay);
	WaitingLayer();
	~WaitingLayer();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void callBackNull(Ref *sender);
	void addWaitLayer(float fDelay);
private:
	Menu* m_menuNull;
};

#endif //_WAITINGLAYER_H_