//
//  TitleBarLayer.h
//  TRGame
//
//  Created by Jimmy on 16/04/29.
//
//

#ifndef _TITLEBARLAYER_H_
#define _TITLEBARLAYER_H_

#include "Defines.h"
#include "BaseClass.h"

enum
{
	TITLE_SHOW_JB_GY,//金币勾玉
	TITLE_SHOW_GY_YX//勾玉玉玺
};

class TitleBarLayer : public BTLayer
{
public:
	static TitleBarLayer* create(int iShowType = TITLE_SHOW_JB_GY);
	virtual bool init(int iShowType);
	TitleBarLayer();
	~TitleBarLayer();
	void onEnter();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void updateShow(int index);
	void updateMoneyShow();
	void UpdataFashionShow();
	void fireToJade();
private:
	void initControl();
	void callBackHero(Ref *sender);
	void callBackGold(Ref *sender);
	void callBackJade(Ref *sender);
	void callBackSeal(Ref *sender);
private:
	int m_iShowType;
	Menu* m_pBtnMenu;
	Label* m_labName;
	Label* m_labGold;
	Label* m_labJade;
	Label* m_labSeal;
	Sprite* m_spVIP;
	Sprite* m_spFashion;
	Sprite* m_spWapFashion;
};

#endif //_TITLEBARLAYER_H_