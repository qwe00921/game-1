#ifndef __TRGAME_EQUIPFASHION_LAYER__
#define  __TRGAME_EQUIPFASHION_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class FashionElemShow
{
	CC_SYNTHESIZE(Sprite*,eBg,eBG);
	CC_SYNTHESIZE(Sprite*,eItemImg,eITEMIMG);
	CC_SYNTHESIZE(Label*, eName, eNAME);
	CC_SYNTHESIZE(Label*,eEffect,eEFFECT);
	CC_SYNTHESIZE(Sprite*,eLock,eLock);
	CC_SYNTHESIZE(Sprite*,eSelected,eSELECTED);
	CC_SYNTHESIZE(int,eDataIndex,eDataIndex);
};

class FashionElemData
{
	CC_SYNTHESIZE(int,iGoodsId,eGOODSID);
	CC_SYNTHESIZE(int,iIsLock,eLOCK);
	CC_SYNTHESIZE(int,iSelectEd,eSELECTED);
	CC_SYNTHESIZE(HistoryGoodsPtr, mGoodsItem, eGOODSITEM);
};


class EquipFashionLayer :public BTLayer
{
public:
	static EquipFashionLayer* create(HistoryActorPtr ptrActor,EventDelegate* ptrDelegate);
	bool init(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate);
	EquipFashionLayer();
	~EquipFashionLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void ClickProMenu(Ref* sender);
	void ClickNextMenu(Ref* sender);
	void ClickCloseMenu(Ref* sender);

	void GetShowElem();
	void GetDataElem();
	void UpdataElemShow();
	void EquipFashion(int iDataIndex);
	void SendNetDate(int iZid, int iXid);
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void ActorSetEquip();

	void UpdataLeftShow();
	void EquipmentUpdataDataList(HistoryGoodsPtr EquipGoods = NULL, HistoryGoodsPtr UploadGoods = NULL);

private:
	EventDelegate* mDelegate;
	HistoryActorPtr curActor;

	map<int, FashionElemShow*> mShowElem;
	map<int, FashionElemData*> mDataList;

	Sprite* bg;
	Sprite* wapSp;
	Sprite* coatSp;
	Label* fashionEffect;

	int m_iMaxPage;
	int m_iCurPage;
	int m_iClickDataIndex;
	bool m_bEnableClick;
	bool m_bEnableUpDataFatherShow;

	map<int, Sprite*> mElemDot;
	Sprite* mCurDotSp;
	Sprite* actor_fashion;
	Sprite* actor_img;
	Sprite* img_bg;
};
#endif //(__TRGAME_EQUIPFASHION_LAYER__)