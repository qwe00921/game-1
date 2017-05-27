#ifndef __TRGAME_CLAN_HOME_PAGE_LAYER__
#define  __TRGAME_CLAN_HOME_PAGE_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "ClanApplyListLayer.h"
#include "ClansmanAdmTableView.h"
#include "ClanLogTableView.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClanHomePageLayer :public BTLayer
{
public:
	static ClanHomePageLayer* create(Json::Value vDataValue);
	bool init(Json::Value vDataValue);
	ClanHomePageLayer();
	~ClanHomePageLayer();

	void GetData(Json::Value vDataValue);

	void clickBackMenu(Ref* sender);
	void clickTuiChuMenu(Ref* sender);
	void clickRightMenu(Ref* sender);
	void clickFunctionMenu(Ref* sender);
	void clickSetZongZhiMenu(Ref* sender);

	void SetPopLayerElement(Sprite* ptrBg);

	//退出解散氏族回调
	void RunFunction(int iNomal);
	//退出至主界面回调
	void ChildCloseLayer(int iNomal);
	//活跃任务刷新回调
	void updateShow(int iNomal);
private:
	void SendClansmanNetData();
	void SendClanInfoData();
	void SendLeaveNetData();
	void SendDisbandmentNetData();
	void SendActiveTaskNetData();
	void SendClanLogNetData();
	void SendArmyFuBenNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	void UpdataLeftShow(Json::Value vDataValue);
	void updateRTDate(std::string nomalS);


	ClanListElem mClanData;
	MenuItemSprite* mSelectMenu;
	Sprite* mCenterBg;
	Sprite* mbg;
	Sprite* mBgInfo;

	ClansmanAdmTableView* mClansmanTB;
	ClanLogTableView* mClanLogTB;

	string m_GuildIDS;
	string m_sSelfUid;
	int m_iSelfJob;

	Label* mElderNameLab;
	Label* mClanId;
	Label* mPeoNum;
	Label* mActiveLab;
	Label* mMilLab;
	Label* mZzLab;
	Sprite* mShizuNTSP;
	Sprite* mShizuSTSP;
	Menu* mSetZongZhi;
};
#endif //(__TRGAME_CLAN_HOME_PAGE_LAYER__)