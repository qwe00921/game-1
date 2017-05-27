//
//  NoticeActOpenLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/22.
//
//

#ifndef _NOTICEACTOPENLAYER_H_
#define _NOTICEACTOPENLAYER_H_

#include "Defines.h"

class NoticeActOpenLayer : public BTLayer
{
public:
	static NoticeActOpenLayer* create(map<int, int> mWarId);
	virtual bool init(map<int, int> mWarId);

	static NoticeActOpenLayer* create(int iWarId);
	virtual bool init(int iWarId);

	NoticeActOpenLayer();
	~NoticeActOpenLayer();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void updateNoticeLayer(int iWarId);
private:
	map<int, int> m_mWarId;
	int m_iCurIndex;

	Sprite* m_spLayBG;
	Sprite* m_spIcon;
	Label* m_labNotice1;
	Label* m_labNotice2;
};

#endif //_NOTICEACTOPENLAYER_H_