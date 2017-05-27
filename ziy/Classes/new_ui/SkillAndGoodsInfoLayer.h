#ifndef __TRGAME_SKILL_AND_GOODS_INFO_LAYER__
#define  __TRGAME_SKILL_AND_GOODS_INFO_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SkillInfoLayer:public BTLayer
{
public:
	static SkillInfoLayer* create(int iSkillId, int iSkillType, HistoryActorPtr ptrActor, EventDelegate* ptrDelegate=NULL);
	bool init(int iSkillId, int iSkillType, HistoryActorPtr ptrActor, EventDelegate* ptrDelegate);
	SkillInfoLayer();
	~SkillInfoLayer(){}
	void onExit();
	void onEnter();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void ClickLevelUpMenu(Ref* sender);
	void ClickLevelUpMenu10LV(Ref* sender);
	void SendNetDate(int times);
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	map<int, float> GetHurtNum();

	int mSkillId;
	bool bRemoveThis;
	HistoryActorPtr curActor;
	Menu* upMenu;
	MenuItemSprite* upM_Sp;
	Label* level_num;
	Label* mptxt;
	Label* gold_num;
	Label* hurt_text;
	Sprite* bg;
	EventDelegate* mDelegate;
};

enum
{
	EQ_TYPE_NOMAL = 100,
	EQ_TYPE_EQUIP = 110,
	EQ_TYPE_UPLOAD = 120,
	EQ_TYPE_BUY = 130,
};

class GoodsInfoLayer :public BTLayer
{
public:
	static GoodsInfoLayer* create(HistoryGoodsPtr ptrGoods,HistoryActorPtr ptrActor,int iFNType = EQ_TYPE_NOMAL, EventDelegate* ptrED = NULL, int iGoldIndex = -1);
	bool init(HistoryGoodsPtr ptrGoods, HistoryActorPtr ptrActor);
	GoodsInfoLayer();
	~GoodsInfoLayer(){}
	void onExit();
	void onEnter();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void clickButMenu(Ref* sender);
	void clickStrengMenu(Ref* sender);
	void clickSuitMenu(Ref* sender);
	void setBlackLayerVisible(bool bVisible);
	void setBgPoint(int iX, int iY, float iAx, float iAy);
	void setEnableRemove(bool bEnable);

	int mFNType;
	int mGoldIndex;
	EventDelegate* m_EventDelegate;
	Menu* FTM;
	Sprite* bg;
	LayerColor* black;
	HistoryGoodsPtr mGoods;

	int click_remove_layer;
	bool mEnableRemove;
private:
	HistoryActor* m_ptrActor;
};


#endif //(__TRGAME_SKILL_AND_GOODS_INFO_LAYER__)