//
//  SysSetLayer.h
//  TRGame
//
//  Created by Jimmy on 16/05/31.
//
//

#ifndef _SYSSETLAYER_H_
#define _SYSSETLAYER_H_

#include "Defines.h"
#include "CheckBoxSprite.h"

class SysSetLayer : public BTLayer
{
public:
	CREATE_FUNC(SysSetLayer);
	SysSetLayer();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void callBackClick(Ref *sender);
	void callBackClose(Ref *sender);
private:
	map<int, CheckBoxSprite*> m_mCheckBox;
};

#endif //_SYSSETLAYER_H_
