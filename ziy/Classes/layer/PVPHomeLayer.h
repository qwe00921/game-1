//
//  PVPHomeLayer.h
//  TRGame
//
//  Created by Jimmy on 17/03/21.
//
//

#ifndef _PVPHomeLayer_H_
#define  _PVPHomeLayer_H_

#include "Defines.h"
#include "PVPData.h"

class PVPActorSprite : public BTSprite
{
public:
	PVPActorSprite();
	~PVPActorSprite();
	static PVPActorSprite* create(bool bIsClicked = true);
	virtual bool init(bool bIsClicked);
	void updateSprite(PVPActorData* ptrActor);
private:
	void initControl();
	void callBackClick(Ref *sender);
private:
	PVPActorData* m_ptrActor;
	bool m_bIsClicked;
	Sprite* m_spImg;
	Sprite* m_spRank;
	Sprite* m_spVip;
	Label* m_labName;
};

class PVPHomeLayer : public BTLayer
{
public:
	PVPHomeLayer();
	~PVPHomeLayer();
	static PVPHomeLayer* create();
	virtual bool init();
private:
	void initControl();
	void updateDisplay();

	void onHttpRequest();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void callBackFunc(Ref *sender);
private:
	Sprite* m_spRank;
	Label* m_labTimes;
	map<int, PVPActorSprite*> m_mSpActor;
};

#endif // _PVPHomeLayer_H_