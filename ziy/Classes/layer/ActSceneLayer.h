//
//  ActSceneLayer.h
//  TRGame
//
//  Created by Jimmy on 16/06/17.
//
//

#ifndef _ACTSCENELAYER_H_
#define _ACTSCENELAYER_H_

#include "Defines.h"

class ActSceneLayer : public BTLayer
{
public:
	static ActSceneLayer* create(int iWarId);
	virtual bool init(int iWarId);
	ActSceneLayer();
	~ActSceneLayer();
	void onExit();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	virtual void ChildCloseLayer(int iNomal);
	virtual void SetPopLayerElement(Sprite* ptrBg);
	virtual void RunFunction(int iNomal);
	virtual void SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType);
private:
	void initControl();

	void callBackClose(Ref *sender);
	void callBackSend(Ref *sender);
	int GetAttackNum(int iWarId);
private:
	Label* m_labTimes;
	int m_iWarId;
	int m_iCurTimes;
};

#endif //_ACTSCENELAYER_H_