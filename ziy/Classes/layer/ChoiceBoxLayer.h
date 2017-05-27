//
//  ChoiceBoxLayer.h
//  TRGame
//
//  Created by Jimmy on 16/07/20.
//
//

#ifndef _ChoiceBoxLayer_H_
#define _ChoiceBoxLayer_H_

#include "Defines.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChoiceBoxLayer : public BTLayer
{
public:
	static ChoiceBoxLayer* create(int iActorId, string strContent);
	virtual bool init(int iActorId, string strContent);
	ChoiceBoxLayer();
	~ChoiceBoxLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initCondition();
	void initControl();
private:
	int m_iActorId;
	string m_strContent;
	int m_iClickIndex;
	Sprite* m_spLayBG;
	map<int, Sprite*> m_mspBG;
	map<int, Label*> m_mLabel;
	map<int, string> m_mCondition;
};

#endif //_ChoiceBoxLayer_H_