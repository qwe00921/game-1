#ifndef __TRGAME_XMTWAR_LAYER__
#define  __TRGAME_XMTWAR_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class XmtWarElem
{
	CC_SYNTHESIZE(Armature*,eArmature,EARMATURE);
	CC_SYNTHESIZE(Sprite*,eBg,EBG);
	CC_SYNTHESIZE(Sprite*, eSelect, ESELECT);
	CC_SYNTHESIZE(int,eWarId,EWARID);
	CC_SYNTHESIZE(int,eIsPass,EISPASS);
	CC_SYNTHESIZE(int,eIsEnable,EISENABLE);
	CC_SYNTHESIZE(Sprite*,eChest,ECHEST);
	CC_SYNTHESIZE(int,eStarNum,ESTARNUM);
};

class XmtWarElemLayer :public BTLayer
{
public:
	static XmtWarElemLayer* create(int iCurWar = -1, int iCurStage = 0,EventDelegate* ptrEventDelegate = NULL);
	bool init(int iCurWar, int iCurStage, EventDelegate* ptrEventDelegate);
	XmtWarElemLayer();
	~XmtWarElemLayer();

	void GetCurStage(int iCurWar);
	void GetElemData(int iCurWar);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void ChildCloseLayer(int iNomal);
	void SetPopLayerElement(Sprite* bgSp);

	void SendNetDate(int iMapId, int iStar);
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void setNetDate(Json::Value jsonValue);
	void showPrompt(float f);
	void RunFunction(int iNomal);
private:
	int m_iWarId;
	DSize mSize;
	map<int, XmtWarElem*> mShowList;
	Node* xmtNode;
	int m_iStage;
	int m_iSignWarId;
	int m_iXmtOffsetY;
	int m_iSelectIndex;
	int m_iClickIndex;
	int m_iFloorHeight;
	int m_iClickChestIndex;
	int mPromptTxtIndex;
	map<int, string> mPromptTxt;
	int mReceiveChestIndex;
};

class XmtWarLayer :public BTLayer, public ScrollViewDelegate
{
public:
	static XmtWarLayer* create(int iCurWar = -1, int iCurStage = 0);
	bool init(int iCurWar, int iCurStage);
	XmtWarLayer();
	~XmtWarLayer();
	void clickMenuBack(Ref* sender);
	void RunFunction(int iNomal);

private:
	int m_iOffsetY;
};

///////////////////////////////////////

class XmtWarCell : public TableViewCell
{
public:
	CREATE_FUNC(XmtWarCell);
	bool init();
	void updateCell(int iStage, int iEnable);
public:
	Sprite* m_spStage;
};

class XmtWarTableView : public BTLayer, TableViewDataSource, TableViewDelegate
{
public:
	CREATE_FUNC(XmtWarTableView);
	virtual bool init();
	XmtWarTableView();
	~XmtWarTableView();

	// 当滑动tableview时触发该方法 参数为当前的tableview对象
	virtual void scrollViewDidScroll(ScrollView* view);
	// 当cell被点击时调用该方法 参数为当前的tableview对象与被点击的cell对象
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	// 设置tableview的Cell大小
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
	// 获取编号为idx的cell
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	// 设置tableview中cell的个数
	virtual ssize_t numberOfCellsInTableView(TableView *table);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void callBackClose(Ref *sender);
private:
	void showDispatch();
	void updateArrow();
	void GetAllStage();
private:
	TableView* m_pTableView;
	map<int, map<int,int> > m_mStage;
	int m_iClickIndex;
	Sprite* m_spLArrow;
	Sprite* m_spRArrow;
};

#endif //(__TRGAME_XMTWAR_LAYER__)