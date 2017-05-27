#ifndef __GREAT_BATTLE_LAYER__
#define  __GREAT_BATTLE_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "GoodsSprite.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocostudio;

class GreatBattleLayer :public BTLayer
{
public:
	CREATE_FUNC(GreatBattleLayer);
	bool init();
	GreatBattleLayer();
	~GreatBattleLayer();
	void clickCloseLayer(Ref* sender);
	void RunFunction(int iNomal);
	void ChildCloseLayer(int iNomal);
};


class GreatBattleSelectLayer :public BTLayer
{
public:
	static GreatBattleSelectLayer* create(int iId,EventDelegate* ptrDelegate);
	bool init(int iId,EventDelegate* ptrDelegate);

	GreatBattleSelectLayer();
	~GreatBattleSelectLayer();

	void clickProMenu(Ref* sender);
	void clickNextMenu(Ref* sender);
	void clickCloseMenu(Ref* sender);
	void clickDispatchMenu(Ref* sender);
private:
	void updataShow();
	Sprite* m_spBgTitle;
	Label* m_AllStageLab;
	Label* m_warInfoLab;
	MenuItemSprite* m_btnSend;
	Sprite* m_spBg;

	int m_iStage;
	int m_iMaxStage;
	int m_iBattleId;
	int m_iAllAttackNum;
	EventDelegate* mDelegate;
	map<int, GoodsSprite*> mGoodList;
	map<int, int> mAttackBattle;
};
#endif //(__GREAT_BATTLE_LAYER__)