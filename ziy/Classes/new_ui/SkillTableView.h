#ifndef __TRGame_Skill_TableView__
#define __TRGame_Skill_TableView__

#include <stdio.h>

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

 enum
 {
 	SKILL_ACTIVE = 1,//主动技能
 	SKILL_PASSIVE = 2,//被动技能
 	SKILL_SPECIAL = 3,//特技
 };

class SkillInfo
{
	CC_SYNTHESIZE(int,skill_id,SkillId);//技能ID
	CC_SYNTHESIZE(int, skill_type, SkillType);//类型
	CC_SYNTHESIZE(int, icon_id, IconId);//图标路径
	CC_SYNTHESIZE(string, type_path, TypePath);//法术类型图片路径
	CC_SYNTHESIZE(bool, is_gray, ISGRAY);//是否变灰
};

class SkillTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static SkillTableView* create(HistoryActorPtr Actor,Layer* parent);
	virtual bool init(HistoryActorPtr Actor,Layer* parent);

	SkillTableView();
	~SkillTableView();
    
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

	map<int, int> getIds(int idx);

	void addSkillElem(int index,int skillId,int SkillType,int iconId,bool is_gray);
	void setSkillArr();
	
    TableView* tableView;
	HistoryActorPtr curActor;
	map<int, SkillInfo*> skill_elem;
	Layer* ptrParent;
};

class ShowSkillInfo
{
	CC_SYNTHESIZE(Sprite*, m_bg, BG);//底纹
	CC_SYNTHESIZE(Sprite*, m_icon, ICON);//图片
	CC_SYNTHESIZE(Sprite*, m_type, TYPE);//释放类型
	CC_SYNTHESIZE(int, m_skillId, SKILLID);//法术ID
	CC_SYNTHESIZE(int, m_ispress, PRESS);//按下
	CC_SYNTHESIZE(int, m_caster, CASER);//释放类型
};

class SkillInfoCell :public TableViewCell
{
public:
	static SkillInfoCell* create(map<int, SkillInfo*> skill_elem, map<int, int> ids);
	bool init(map<int, int> ids);
	SkillInfoCell();
	~SkillInfoCell();
	void onExit();
	void onEnter();
	void UpDateCell(map<int, int> ids);
	Layer* upperLayer;
private:
	map<int, SkillInfo*> totleelem;
	map<int, ShowSkillInfo*> elem;
    void initTouches();
};

#endif /* __TRGame_Skill_TableView__ */
