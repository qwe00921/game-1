//
//  TalkLayer.h
//  TRGame
//
//  Created by Jimmy on 16/07/19.
//
//

#ifndef _TalkLayer_H_
#define _TalkLayer_H_

#include "Defines.h"


class TalkLabel
{
	CC_SYNTHESIZE(int, m_iActorId, ActorId);
	CC_SYNTHESIZE(int, m_iTotalPage, TotalPage);
	CC_SYNTHESIZE(string, m_strTitle, Title);
	CC_SYNTHESIZE(string, m_strContent, Content);
};

class ActorTalkCell : public Sprite
{
public:
	CREATE_FUNC(ActorTalkCell);
	virtual bool init();
	void updateSprite(TalkLabel* ptrLabel, int iPage);
private:
	void initControl();
private:
	Sprite* m_spCellBG;
	Sprite* m_spFace;
	Label* m_labTitle;
	Label* m_labText;
	ClippingNode* m_clipNode;

	int m_iDirection; //人物头像的左右位置, 0左边, 1右边
};

class LabelCell : public Sprite
{
public:
	CREATE_FUNC(LabelCell);
	virtual bool init();
	void updateSprite(TalkLabel* ptrLabel, int iPage);
private:
	void initControl();
private:
	Label* m_labText;
	ClippingNode* m_clipNode;
};

class TalkLayer : public BTLayer
{
public:
	CREATE_FUNC(TalkLayer);
	virtual bool init();
	TalkLayer();
	~TalkLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void updateLayer(string strContent);
	void updateLayer(string strContent, int iActorId);
	void showNextLabel();
private:
	void initTalkLabel();
	void initControl();
	void updateNextLayer(float fInterval);
	void autoNextLayer(float fInterval);
	void clearTalkLabel();
private:
	string m_strContent;
	ActorTalkCell* m_ActorTalk;

	bool m_lock;
	int m_iIndex;
	int m_iPage;
	bool m_bIsPress;
	bool m_bIsClick;
	map<int, TalkLabel*> m_mTalkLabel;
};

#endif //_TalkLayer_H_