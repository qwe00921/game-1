#ifndef __TRGame_ArmorListTableView__
#define __TRGame_ArmorListTableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
#include "editor-support/cocostudio/CCArmature.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ArmorListDate
{
public:
	ArmorListDate()
	{
		actorId = 0;
		addOwnArmList = false;
		current_soul = 0;
		need_soul = 0;
		enable_recruit = false;
		bIsSend = false;
	}
public:
	short actorId;
	bool addOwnArmList;
	short current_soul;
	short need_soul;
	bool enable_recruit;
	bool bIsSend;
};

class ArmorListTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	static ArmorListTableView* create(EventDelegate* ptrDelegate);
	virtual bool init(EventDelegate* ptrDelegate);

	ArmorListTableView();
	~ArmorListTableView();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    /* 必须实现的函数 */
    //    // 当滑动tableview时触发该方法 参数为当前的tableview对象
    virtual void scrollViewDidScroll(ScrollView* view);
    //    // 当tableview被放大或缩小时触发该方法  参数为当前tableview对象
    virtual void scrollViewDidZoom(ScrollView* view);
    // 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    // 设置tableview的Cell大小
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    
    // 获取编号为idx的cell
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    // 设置tableview中cell的个数
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
	ArmorListDate* GetCellElem(int iIndex);
	void AddActorInfoLayer(int iIndex);
	void InitElemList();
	void SortElemList();
	void ChildCloseLayer(int iNomal);
	bool EnableShowChild();
	void RunFunction(int iNomal);
	void updateParentShow();
	void SetPopLayerElement(Sprite* ptrBg);
	void SendNetDate();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void UpdateArmorDate(Json::Value JsonValue);
	void updateShow(int iNomal);
	void updateMoneyShow();
	void RemoveAllParentLayer();

    TableView* tableView;
	int cell_num;
	map<int, ArmorListDate*> armor_list;
	bool show_child_layer;
	int click_index;
	EventDelegate* mDelegate;
};

class ArmorListInfo
{
	CC_SYNTHESIZE(Sprite*, mBg, BG);
	CC_SYNTHESIZE(Sprite*, mImg, IMG);
	CC_SYNTHESIZE(Sprite*, mFashion, FASHION);
	CC_SYNTHESIZE(Label*, mName, TNAME);
	CC_SYNTHESIZE(Label*, mLv, LV);
	CC_SYNTHESIZE(Sprite*, mQuality, QUALITY);
	CC_SYNTHESIZE(Sprite*, mSoul, SOUL);
	CC_SYNTHESIZE(Label*, mSoulNum, SOULNUM);
	CC_SYNTHESIZE(int, mActorId, ACTORID);
	CC_SYNTHESIZE(int, mIndex, MINDEX);
	CC_SYNTHESIZE(int, mIsOwn, MISOWN);
	CC_SYNTHESIZE(Sprite*, mRecruit, MRECRUIT);
	CC_SYNTHESIZE(Sprite*, mOccupation, OCCUPATION);
	CC_SYNTHESIZE(Sprite*,mSoulIcon,MSOULICON);
	CC_SYNTHESIZE(Sprite*, mCurSoul, MCURSOUL);
	CC_SYNTHESIZE(Sprite*, mspSend, SpSend); //出战遮罩
};

class ArmorListCell :public TableViewCell
{
public:
	static ArmorListCell* create(ArmorListTableView* ptrALTB,int iIndex);
	bool init(int iIndex);
	void onExit();
	void onEnter();
	ArmorListCell();
	~ArmorListCell(){}
	void UpDateCell(int iIndex);
public:
	map<int, ArmorListInfo*> elem;
	ArmorListTableView* mALTB;
	int click_elem_index;
private:
    void initTouches();
};

#endif /* __TRGame_ArmorListTableView__ */
