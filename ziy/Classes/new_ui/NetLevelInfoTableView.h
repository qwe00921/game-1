#ifndef __NETLEVELINFO_TABLEVIEW__
#define __NETLEVELINFO_TABLEVIEW__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class NetLevelInfoTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static NetLevelInfoTableView* create(map<int, map<int, int> > f_elem);
	virtual bool init();

	NetLevelInfoTableView();
	~NetLevelInfoTableView();
    
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

public:
	TableView* tableView;
	map<int, map<int, int> > elem_list;
	int mCellNum;
};

class NetLevelInfoCell :public TableViewCell
{
public:
	static NetLevelInfoCell* create(int iActorId, int iNum, int iLv);
	bool init();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	NetLevelInfoCell();
	~NetLevelInfoCell(){}
	void UpDateCell(int iActorId, int iNum, int iLv);
	void AddActorImgSprite(const char* suffix, int actor_id, Sprite* ptrBg);
private:
	int actor_id;
	int mNum;
	int mLv;
	Sprite* actor_img;
	Sprite* bg;
	Label* lv;
	Label* num;
	Label* name;
};

#endif /* __NETLEVELINFO_TABLEVIEW__ */
