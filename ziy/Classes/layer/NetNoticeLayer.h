//
//  NetNoticelayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#ifndef _NETNOTICELAYER_H_
#define _NETNOTICELAYER_H_

#include "Defines.h"
#include "MailLayer.h"

class NetNoticelayer : public BTLayer
{
public:
	static NetNoticelayer* create();
	virtual bool init();
	NetNoticelayer();
	~NetNoticelayer();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
private:
	void initControl();
	void callBackClose(Ref *sender);
private:
	Menu* m_pBtnMenu;
	Label* m_labContent;

	Rect m_rcDraw;
	bool m_bSupDrag; //文字较长支持拖动
	bool m_bIsPress;
	float m_fOffsetY;
};

#endif //_NETNOTICELAYER_H_