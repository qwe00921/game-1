#ifndef __TRGAME_BSELECTGOODSORSKILL_LAYER__
#define  __TRGAME_BSELECTGOODSORSKILL_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocostudio;

#define BATTLE_TABLEVIEW_COLUMN 4

enum BSelectShowType
{
	BSELECT_SHOW_GOODS,//道具选择框
	BSELECT_SHOW_SKILL//技能选择框
};

class GoodsOrSkillDate
{
public:
	GoodsOrSkillDate()
	{
		m_iIndex = -1;
		m_iShowType = -1;
		m_iId = -1;
		m_iNum = 0;
		m_bEnableClick = true;
		m_iLevel = 0;
		m_bIsCopy = false;
	}
	CC_SYNTHESIZE(int, m_iIndex, EINDEX);
	CC_SYNTHESIZE(int, m_iShowType, ESHOWTYPE);
	CC_SYNTHESIZE(int, m_iId, EID);
	CC_SYNTHESIZE(int, m_iNum, ENUM);
	CC_SYNTHESIZE(bool, m_bEnableClick,EENABLECLICK);
	CC_SYNTHESIZE(int, m_iLevel, ELEVEL);
	CC_SYNTHESIZE(bool, m_bIsCopy, IsCopy);
};

typedef GoodsOrSkillDate* GoodsOrSkillDatePtr;

class BSelectGoodsOrSkillLayer :public BTLayer
{
public:
	static BSelectGoodsOrSkillLayer* create(BattleControlerPtr BCHandle,int iShowType);
	bool init(BattleControlerPtr BCHandle, int iShowType);
	BSelectGoodsOrSkillLayer();
	~BSelectGoodsOrSkillLayer();
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void GetGoodsDateList();
	void GetSkillDateList();
	void clickCloseLayer(Ref* sender);
	void RunFunction(int iNomal);
	void RemoveThisLayer(bool bOnlyRemove = true);

private:
	HistoryActorPtr curActor;
	BattleControlerPtr mHandle;
	int mShowType;
	map<int, GoodsOrSkillDatePtr> mElemDate;
	bool mClickRemoveThis;
	Sprite* mBg;
};
#endif //(__TRGAME_BSELECTGOODSORSKILL_LAYER__)