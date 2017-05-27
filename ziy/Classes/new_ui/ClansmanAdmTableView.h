#ifndef __TRGame_ClansmanAdm_TableView__
#define __TRGame_ClansmanAdm_TableView__

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
	CELL_TYPE_CLANSMAN,
	CELL_TYPE_APPLY
};

class ClansmanElem
{
public:
	ClansmanElem()
	{
		eName = "";
		sLastLogin = "";
		iJob = 0;
		iAllCP = 0;
		iIsVip = 0;
		iTodayActive = 0;
		iIndex = -1;
		sUid = "";
		iAllActive = 0;
	}
	void LoadJson(Json::Value vData);
	CC_SYNTHESIZE(int, iIndex, EiIndex)
	CC_SYNTHESIZE(string, eName, EsName);
	CC_SYNTHESIZE(int, iJob, EiJob);
	CC_SYNTHESIZE(int, iAllCP, EiAllCp);
	CC_SYNTHESIZE(int, iIsVip, EiIsVip);
	CC_SYNTHESIZE(string, sLastLogin, EsLastLogin);
	CC_SYNTHESIZE(int, iTodayActive, EiTodayActive);
	CC_SYNTHESIZE(int, iAllActive, EiAllActive);
	CC_SYNTHESIZE(int,iGender,EiGender)
	CC_SYNTHESIZE(int, iFashion1, EFashion1);
	CC_SYNTHESIZE(int, iFashion2, EFashion2);
	CC_SYNTHESIZE(int, iFashion3, EFashion3);
	CC_SYNTHESIZE(int, iFashion4, EFashion4);
	CC_SYNTHESIZE(string, sUid, EsUid);
};

class ClansmanAdmTableView : public BTLayer,TableViewDataSource,TableViewDelegate
{
public:
	static ClansmanAdmTableView* create(string GuildIDS,string SelfUidS,EventDelegate* ptrDelegate);
	virtual bool init(string GuildIDS,string SelfUidS, EventDelegate* ptrDelegate);

	ClansmanAdmTableView();
	~ClansmanAdmTableView();
    
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
    
	void UpdataTbV(Json::Value vListData,int iType);

    TableView* tableView;
	int cell_num;

private:
	void updataMapList(Json::Value vListData);
	void clickFunctionMenu(Ref* sender);

	void SendClansmanNetData();
	void SendApplyNetData();
	void SendApplyApprovalNetData(string userID,int iStatus);
	void SendGuildJobNetData(int  iJob);
	void SendDeleteManNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	void updateRTDate(int iNomal, int iType);
	void callBackListClick(int iNomal);

	char* GetJobName(int iJobId);

	map<int, ClansmanElem*> mClansManList;
	map<int, ClansmanElem*> mApplyManList;
	ClansmanElem* mSelfClanData;

	int m_iCellType;
	map<int, MenuItemSprite*> mMenuItem;
	string m_sGuildID;
	string m_sSelfUid;
	Sprite* mClansmanInfo;

	int iApprovalIntoIndex;
	int m_iSetGuildJob;

	EventDelegate* mDelegate;
};

class ClansmanAdmCell :public TableViewCell,public EventDelegate
{
public:
	static ClansmanAdmCell* create(ClansmanElem* ptrClansElem,EventDelegate* ptrDelegate,int iType, int iSelfJob);
	bool init(ClansmanElem* ptrClansElem, EventDelegate* ptrDelegate, int iType, int iSelfJob);
	ClansmanAdmCell();
	~ClansmanAdmCell();
	void UpDateCell(ClansmanElem* ptrClansElem, int iType,int iSelfJob);

	void SetPopLayerElement(Sprite* bg);
	void RunFunction(int iNomal);
private:
	void initTouches();
	char* GetJobName(int iJobId);

	EventDelegate* mDelegate;
	int m_iOldType;
	int m_iCurType;

	Sprite* mAdmini;
	Sprite* mSure;
	Sprite* mCancle;
	Sprite* mbg;
	Sprite* spEquip;
	Sprite* spHero;
	Label* mNameLab;
	Label* mJobNameLab;
	Label* mAllCp;
	Sprite* mIsVip;
	Label* mLastLogin;
	Label* mActive;

	ClansmanElem* m_pClansElem;
	EventListenerTouchOneByOne* m_ptrLisTouch;
	//1为管理，2为拒绝，3为同意
	int m_iClickIndex;
	int m_iSelfJob;
};


////////////////设置权限

class ClansmanAdminiLayer:public BTLayer
{
public:
	static ClansmanAdminiLayer* create(ClansmanElem* ptrClansMan,EventDelegate* ptrDelegate);
	bool init(ClansmanElem* ptrClansMan,EventDelegate* ptrDelegate);

	bool onTouchBegan(Touch *touch, Event *unused_event);

	ClansmanAdminiLayer();
	~ClansmanAdminiLayer();

private:
	char* GetJobName(int iJobId);
	void clickItemMenu(Ref* sender);
	void clickSureMenu(Ref* sender);
	void clickCloseMenu(Ref* sender);


	EventDelegate* mDelegate;
	MenuItemSprite* mSelectMenu;
	int m_iSelectJobId;
};


#endif /* __TRGame_ClansmanAdm_TableView__ */
