//
//  BaseClass.h
//  TRGame
//
//  Created by Jimmy on 16/07/09.
//
//

#ifndef _BASECLASS_H_
#define _BASECLASS_H_

#include "cocos2d.h"
#include "Common.h"
#include "EventDelegate.h"
USING_NS_CC;
//////////////////////////////////用于cocostduio//////////////////////////////////////////////////
#define SG_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SG_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)

class BTSprite : public Sprite
{
public:
	BTSprite();
	virtual ~BTSprite();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	void setSwallow(bool bSwallow);
private:
	EventListener* m_ptrLisTouch;
};

class BTLayer : public Layer, public EventDelegate
{
public:
	BTLayer(bool bListener = true);
	virtual ~BTLayer();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void setSwallow(bool bSwallow);
private:
	EventListener* m_ptrLisTouch;
};

#endif //_BASECLASS_H_