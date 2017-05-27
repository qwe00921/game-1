//
//  ActorPracticeLayer.h
//  TRGame
//
//  Created by Jimmy on 16/12/26.
//
//

#ifndef _ActorPracticeLayer_H_
#define  _ActorPracticeLayer_H_

#include "Defines.h"

class PracticeCell : public BTSprite, public EventDelegate
{
public:
	static PracticeCell* create(HistoryActor* ptrActor, int iAttrType, EventDelegate* ptrDel);
	virtual bool init(HistoryActor* ptrActor, int iAttrType, EventDelegate* ptrDel);
	virtual void SetPopLayerElement(Sprite* spBG);
	virtual void callBackConfirm();

	void updateCell();
private:
	void initControl();

	void onHttpRequest(int iReqType);
	void onHttpRequestCompleted(Node *sender, void *resultdata);
	void onStoreInitSucceed(Json::Value valRes);

	void callBackPractice(Ref *sender);
private:
	HistoryActor* m_ptrActor;
	int m_iAttrType;
	EventDelegate* m_ptrDel;
	int m_iGoodsId; //将魂碎片
	string m_strAttrKey;

	Label* m_labAttr;
	Label* m_labAttrVal;
	Label* m_labGrown1;
	Label* m_labGrown2;
	Label* m_labMax;
	MenuItemSprite* m_btnPractice;
};

class ActorPracticeLayer : public BTLayer
{
public:
	ActorPracticeLayer();
	~ActorPracticeLayer();
	static ActorPracticeLayer* create(HistoryActor* ptrActor, EventDelegate* ptrDel);
	virtual bool init(HistoryActor* ptrActor, EventDelegate* ptrDel);
	virtual void updateState();
private:
	void initControl();
	void updateSoul();

	void callBackClose(Ref *sender);
private:
	HistoryActor* m_ptrActor;
	EventDelegate* m_ptrDel;

	Label* m_labSoul;
	map<int, PracticeCell*> m_mPracCell;
};

#endif // _ActorPracticeLayer_H_