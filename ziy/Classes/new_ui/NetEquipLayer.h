#ifndef __TRGAME_NETEQUIPLAYERLAYER__
#define  __TRGAME_NETEQUIPLAYERLAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
#include "BoxSprite.h"
#include "SkillAndGoodsInfoLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class NetEquipLayer : public BTLayer
{
public:
	static NetEquipLayer* create(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate, int iEtype);
	bool init(HistoryActorPtr ptrActor, EventDelegate* ptrDelegate, int iEtype);
	void onExit();
	void onEnter();

	NetEquipLayer();
	~NetEquipLayer();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	bool getModels();
	void addLeftLayer();
	void updateShow(int index, int type);
	void ClickNextMenu(Ref* sender);
	void ClickPreMenu(Ref* sender);
	void ClickCloseMenu(Ref* sender);
	void upDateAllBox();
	void UpdateEquipment(int iFNType);
	void UpdateRightLayer(int iIndex);
	void SendNetDate(int iZid,int iXid);
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void ActorSetEquip();
	void RunFunction(int iNomal);

	map<int, boxModel*> mBoxDate;
	map<int, BoxSprite*> mBoxsList;
	map<int, HistoryGoods*> mGoodsDate;

	int mEtype;
	int maxPage;
	int curPage;
	int selectIndex;
	int mFunctionType;
	DSize mSize;
	HistoryActorPtr curActor;
	EventDelegate* mDelegate;
	GoodsInfoLayer* mGoodsInfo;

	Menu* page_M;
	Menu* close_M;
	Label* page_ttf;
};
#endif //(__TRGAME_NETEQUIPLAYERLAYER__)