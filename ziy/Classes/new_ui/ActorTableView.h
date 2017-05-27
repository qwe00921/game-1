#ifndef __TRGame_Actor_TableView__
#define __TRGame_Actor_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ActorTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ActorTableView* create(HistoryActorPtr Actor,int iCellNum);
	virtual bool init(HistoryActorPtr Actor, int iCellNum);

	ActorTableView();
	~ActorTableView();
    
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

	void UpdateTableView(HistoryActorPtr ptrActor);
    
    TableView* tableView;
	int cell_num;
	HistoryActorPtr curActor;
    
    map<int,int> getIds(int idx);
private:
    void initTouches();
};

class ShowActorInfo
{
	CC_SYNTHESIZE(Label*, m_lab,LAB);//属性
	CC_SYNTHESIZE(Label*, m_pow, POW);//自身能力
	CC_SYNTHESIZE(Label*, m_buff, BUFF);//buff
	CC_SYNTHESIZE(Label*, m_grown, GROWN);//成长
};

class ActorInfoCell :public TableViewCell
{
public:
	static ActorInfoCell* create(HistoryActorPtr Actor);
	bool init(HistoryActorPtr Actor);
	void PropertyElem(ShowActorInfo* ptrShow, const char* ptrPro, int iBase, int iGrown, int iDebuff, int x, int y);
	void PropertyElemGrow(ShowActorInfo* ptrShow, const char* ptrPro, int iBase, int x, int y);
	ActorInfoCell();
	~ActorInfoCell(){}
	void UpDateCell(HistoryActor* Actor);
private:
	void callBackEnchant(Ref *sender);
private:
	HistoryActor* m_ptrActor;
	map<int, ShowActorInfo*> elem;
	Label* introduction;
	MenuItemSprite* m_btnEnchant;
};

class EnchantInforLayer : public Layer
{
public:
	static EnchantInforLayer* create(HistoryActor* ptrActor);
	virtual bool init(HistoryActor* ptrActor);
private:
	void initControl();

	void callBackClose(Ref *sender);
private:
	HistoryActor* m_ptrActor;
};

#endif /* __TRGame_Actor_TableView__ */
