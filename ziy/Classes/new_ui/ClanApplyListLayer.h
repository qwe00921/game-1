#ifndef __TRGAME_CLANAPPLYLIST_LAYER__
#define  __TRGAME_CLANAPPLYLIST_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

#define CLAN_NAME_LENGTH 16
#define ZONGZHI_LENGTH 50
#define PAGE_ELEM_NUM 10

class ClanListElem
{
public:
	ClanListElem()
	{
		eIndex = -1;
		eClanId = 0;
		eIsSelect = 0;
		eIsApply = 0;
		eClanName = "";
		eClanLv = 1;
		eElderName = "";
		eHeroNum = 0;
		eHeroMaxNum = 0;
		eZongZhi = "";
		eActive = 0;
		eMilitary = 0;
		eMaxActive = 1;
	};
	void LoadJson(Json::Value vElemValue);
	CC_SYNTHESIZE(int, eIndex, EIndex);
	CC_SYNTHESIZE(int, eClanId, ClanId);
	CC_SYNTHESIZE(string,eClanName,ClanName);
	CC_SYNTHESIZE(int, eClanLv, ClanLv);
	CC_SYNTHESIZE(string, eElderName, ElderName);
	CC_SYNTHESIZE(int, eHeroNum, HeroNum);
	CC_SYNTHESIZE(int, eHeroMaxNum, HeroMaxNum);
	CC_SYNTHESIZE(int, eIsSelect, IsSelect);
	CC_SYNTHESIZE(string, eZongZhi, ZongZhi);
	CC_SYNTHESIZE(int, eIsApply, IsApply);
	//HomePage使用
	CC_SYNTHESIZE(int, eActive, EActive);
	CC_SYNTHESIZE(int, eMaxActive, EMaxActive);
	CC_SYNTHESIZE(int, eMilitary, EMilitary);
};

class ClanListTableView;
class ClanApplyListLayer :public BTLayer, public ui::EditBoxDelegate
{
public:
	static ClanApplyListLayer* create(Json::Value vHomePage);
	bool init(Json::Value vHomePage);
	ClanApplyListLayer();
	~ClanApplyListLayer();

	void updateRTDate(int iNomal);
	void ChildCloseLayer(int iNomal);

private:
	void editBoxReturn(ui::EditBox* editBox);
	void clickSouSuoMenu(Ref* sender);
	void clickCJSZMenu(Ref* sender);
	void clickCancleMenu(Ref* sender);
	void clickProMenu(Ref* sender);
	void clickNextMenu(Ref* sender);

	void getElemData(Json::Value vPageJson);
	void UpdataMenuState();
	void UpdataTBShow(int iType = 0, bool bIsInit = false);

	void SendSelectNetData();
	void SendPageNetData(bool bIsBack = false);
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	map<int, ClanListElem*> mAllClanElemList;
	map<int, ClanListElem*> mCurClanElemList;
	map<int, int> mAllClanIdIndex;
	ClanListTableView* mClanListTab;
	ui::EditBox* m_mEditBox;
	Menu* proMenu;
	Menu* nextMenu;

	int m_iCurPage;
	int m_iMaxPage;
};

/////////申请加入界面

class ApplyAddClanLayer:public BTLayer
{
public:
	static ApplyAddClanLayer* create(ClanListElem* ptrCLElem,EventDelegate* ptrDelegate);
	bool init(ClanListElem* ptrCLElem, EventDelegate* ptrDelegate);

	ApplyAddClanLayer();
	~ApplyAddClanLayer();

	void clickCancleMenu(Ref* sender);
	void clickSureMenu(Ref* sender);

	void SendNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);
public:
	EventDelegate* mDelegate;
	int m_iClanId;
	string m_ClanNameS;
};

/////////创建公会
enum
{
	TYPE_CREATE_CLAN,
	TYPE_SET_ZONGZHI,
};
class ApplyCreateClanLayer :public BTLayer, public ui::EditBoxDelegate
{
public:
	static ApplyCreateClanLayer* create(EventDelegate* ptrDelegate,int iType = TYPE_CREATE_CLAN,string clanNameS = "",string guildID = "");
	bool init(EventDelegate* ptrDelegate, int iType = TYPE_CREATE_CLAN, string clanNameS = "", string guildID = "");

	ApplyCreateClanLayer();
	~ApplyCreateClanLayer();

	void editBoxReturn(ui::EditBox* editBox);
	void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);
	void clickCancleMenu(Ref* sender);
	void clickSureMenu(Ref* sender);
	void SendNetData();

	void SendSetZongZhiData();
private:
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);

	EventDelegate* mDelegate;
	Label* ZongzhiLab;
	ui::EditBox* m_mZZEditBox;
	ui::EditBox* m_mNameEditBox;
	Label* ZongzhiTitle;

	int m_iType;
	string mGuildID;
};

#endif //(__TRGAME_CLANAPPLYLIST_LAYER__)