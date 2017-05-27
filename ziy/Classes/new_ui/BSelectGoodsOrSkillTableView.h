#ifndef __TRGame_BSELECTGOODSORSKILL_TableView__
#define __TRGame_BSELECTGOODSORSKILL_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
#include "BSelectGoodsOrSkillLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BSelectGoodsOrSkillTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static BSelectGoodsOrSkillTableView* create(map<int, GoodsOrSkillDatePtr>* ptrDate,EventDelegate* ptrDelegate);
	virtual bool init(map<int, GoodsOrSkillDatePtr>* ptrDate, EventDelegate* ptrDelegate);

	BSelectGoodsOrSkillTableView();
	~BSelectGoodsOrSkillTableView();
    
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
private:
    TableView* tableView;
	int mCellNum;
	map<int, GoodsOrSkillDatePtr> getDateElem(int iDx);
	EventDelegate* mDelegate;
	map<int, GoodsOrSkillDatePtr>* mElemDate;
};

class GoodsOrSkillInfo
{
	CC_SYNTHESIZE(Sprite*, m_bg, EBG);
	CC_SYNTHESIZE(Label*, m_name,ENAME);
	CC_SYNTHESIZE(Label*, m_effect, EEFFECT);
	CC_SYNTHESIZE(Sprite*, m_spCopy, CopyMark);
	CC_SYNTHESIZE(Sprite*, m_icon, EICON);
	CC_SYNTHESIZE(Sprite*, m_num, ENUM);
	CC_SYNTHESIZE(int, m_dataIndex, EDATAINDEX);
	CC_SYNTHESIZE(bool,m_enableclick,EENABLECLICK);
	CC_SYNTHESIZE(Sprite*,m_gray,EGRAY);
};

class GoodsOrSkillCell :public TableViewCell
{
public:
	static GoodsOrSkillCell* create(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate);
	bool init(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate);
	void onEnter();
	void onExit();
	void inittouches();
	GoodsOrSkillCell();
	~GoodsOrSkillCell();
	void UpDateCell(map<int, GoodsOrSkillDatePtr> ptrDate, EventDelegate* ptrDelegate);
private:
	map<int, GoodsOrSkillInfo*> mElemShow;
	EventDelegate* mDelegate;
	int mClickElemIndex;
};

#endif /* __TRGame_BSELECTGOODSORSKILL_TableView__ */
