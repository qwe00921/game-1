//
//  BattleLayer.h
//  TRGame
//
//  Created by Jimmy on 16/07/14.
//
//

#ifndef _BattleLayer_H_
#define _BattleLayer_H_

#include "Defines.h"


class BattleLayer : public BTLayer
{
public:
	CREATE_FUNC(BattleLayer);
	virtual bool init();
	BattleLayer();
	~BattleLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void gameSpeedUp(float fDelay);
private:
	bool m_bSpeedUp;
};

#endif //_BattleLayer_H_