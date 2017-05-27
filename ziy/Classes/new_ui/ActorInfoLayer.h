#ifndef __TRGAME_ACTOR_INFO_LAYER__
#define  __TRGAME_ACTOR_INFO_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "editor-support/cocostudio/CCArmature.h"

#include "Common.h"
#include "main.h"

#include "ActorTableView.h"
#include "SkillTableView.h"

#include "ActorOccuInfoLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
enum
{
	READ_STATE_INFO,
	BATTLE_STATE_INFO
};

class ActorInfoLayer:public BTLayer
{
public:
	static ActorInfoLayer* create(HistoryActorPtr ptrActor, int iInfoState,EventDelegate* ptrDelegate=NULL);
	bool init(HistoryActorPtr Actor);
	ActorInfoLayer();
	~ActorInfoLayer(){}
	void onExit();
	void onEnter();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void clickCloseLayer(Ref* sender);
	void clickPowerMenu(Ref* sender);
	void clickSkillMenu(Ref* sender);
	void clickOccuMenu(Ref* sender);
	void clickListLeft(Ref* sender);
	void clickListRight(Ref* sender);
	void clickEquipmentMenu(Ref* sender);
	void clickCultivationMenu(Ref* sender);
	void clickPromoteMenu(Ref* sender);
	void clickNullMenu(Ref* sender);

	void UpdateActorDate(HistoryActorPtr ptrActor);

	void SetNextActor(bool is_next);
	void UpdateCurrentDate();
	void AddSkillInfoLayer(int iSkillId,int iSkillType);
	void AddGoodsInfoLayer();
	__String* GetActorCanUseGoods(int iGoodsType);
	bool isSkilltbv();
	void SetPopLayerElement(Sprite* ptrBg);
	void RunFunction(int iNomal);
	void updateShow(int index);
	void updateMoneyShow();
	void RemoveAllParentLayer();
	void updateParentShow();//神装界面回调

	void SendNetDate();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);

	void GetOwnActorList(HistoryActorPtr ptrCurActor);
	HistoryActorPtr GetSortActor(bool bIsNext);

public:
	int mInfoState;
	MenuItemSprite* select_menu;
	MenuItemSprite* close_menu;
	ActorTableView* power_tbv;
	SkillTableView* skill_tbv;
	ActorOccuInfoLayer* occu_info;
	Sprite* left_bg;
	Sprite* we_bg;
	Sprite* co_bg;
	Sprite* as_bg;

	Sprite* name_bg;
	Sprite* actor_face;
	Label* name;
	Sprite* bar_hp;
	Label* bar_hp_ttf;
	Sprite* bar_mp;
	Label* bar_mp_ttf;
	Sprite* bar_exp;
	Label* bar_exp_ttf;
	Sprite* relation;
	Sprite* actor_job;
	Sprite* we_icon;
	Sprite* spWeStone;
	Armature* we_arm;
	Sprite* we_quality;
	Sprite* we_add;
	Label* we_effect;
	Sprite* co_icon;
	Sprite* spCoStone;
	Armature* co_arm;
	Sprite* co_quality;
	Sprite* co_add;
	Label* co_effect;
	Sprite* as_icon;
	Sprite* spAsStone;
	Sprite* as_add;
	Label* as_effect;
	Label* debuff;
	Sprite* info1;

	Vec2 move_point;
	int actor_index;
	HistoryActorPtr curActor;
	bool bIsSkillTbv;
	int LongPress;
	EventDelegate* mDelegate;

	Menu* mMenuSZ;
	Menu* mMenuXL;
	Menu* mMenuJJ;
	Menu* mMenuPower;
	Menu* listLeftMenu;
	Menu* listRightMenu;
	Menu* mCloseMenu;

	map<int, int> mOwnActorList;
	int m_iOwnActorIndex;
};


#endif //(__TRGAME_ACTOR_INFO_LAYER__)