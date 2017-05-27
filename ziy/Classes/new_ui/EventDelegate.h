//
//  EventDelegate.hpp
//  TRGame
//
//  Created by 袁文彬 on 16/2/29.
//
//

#ifndef EventDelegate_h
#define EventDelegate_h

#include <stdio.h>
#include "jsons.h"

#include "cocos2d.h"
USING_NS_CC;
class EventDelegate
{
public:
	virtual void updateState(){};
    virtual void updateShow(int index){};
    virtual void updateShow(int index,int type){};
    virtual void wield(int index){};
    virtual void closeLayer(Ref *sender){};
	virtual void UpdateEquipment(int iFNType){};
    virtual void createPop(int index){};
	virtual void addLayerChild(Layer* ptrLayer){};
	virtual void updateRTDate(int iNomal){};
	virtual void updateRTDate(int iNomal,int iType){};
	virtual void updateRTDate(std::string nomalS) {};
	virtual void RunFunction(int iNomal){};
	virtual void callBackListClick(int iIndex){};
	virtual void callBackConfirm(){};
	virtual void ChildCloseLayer(int iNomal){};
	virtual std::map<int, std::map<int, int> > GetElem(int iIndex);
	virtual int GetElemInt(int iIndex){ return 0; };
	virtual int GetOrSetDate(int iDate,bool is_get = true){ return 0; };
	virtual void SetPopLayerElement(Sprite* ptrBg){};
	virtual void updateParentShow(){};
	virtual void SelectPayRequest(int iPayType){};
	virtual void SendNetDateResult(bool bIsSuccess, Json::Value jsonResult,int iType){};
	virtual void updateMoneyShow(){};
	virtual void RemoveAllParentLayer(){};
	virtual void ResetBattleScreenState(){};
	virtual void previousItem(){};
	virtual void nextItem(){};
	virtual void updateRTDate(std::map<int, std::string> mString){};
};

#endif /* EventDelegate_hpp */
