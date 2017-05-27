#include "ClansmanAdmTableView.h"
#include "Defines.h"
#include "PopLayer.h"

#define CELL_WIDTH 490
#define CELL_HEIGHT 54

void ClansmanElem::LoadJson(Json::Value vData)
{
	ParseString(eName, vData["name"]);
	ParseInt(iJob, vData["job"]);
	ParseInt(iAllCP, vData["allCP"]);
	ParseInt(iIsVip, vData["isVip"]);
	ParseString(sLastLogin, vData["lastLogin"]);
	ParseInt(iTodayActive, vData["todayActive"]);
	ParseInt(iAllActive, vData["allActive"]);
	ParseInt(iGender, vData["gender"]);
	ParseInt(iFashion1, vData["Fashion1"]);
	ParseInt(iFashion2, vData["Fashion2"]);
	ParseInt(iFashion3, vData["Fashion3"]);
	ParseInt(iFashion4, vData["Fashion4"]);
	ParseString(sUid, vData["uid"]);
	iGender += 1;
}

///////////////

ClansmanAdmTableView* ClansmanAdmTableView::create(string GuildIDS,string SelfUidS, EventDelegate* ptrDelegate)
{
	ClansmanAdmTableView *pRet = new ClansmanAdmTableView();
	if (pRet && pRet->init(GuildIDS,SelfUidS,ptrDelegate))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ClansmanAdmTableView::init(string GuildIDS,string SelfUidS, EventDelegate* ptrDelegate)
{
    if ( !Layer::init() )
    {
        return false;
    }
	mDelegate = ptrDelegate;
	iApprovalIntoIndex = -1;
	m_iSetGuildJob = -1;
	m_sGuildID = GuildIDS;
	m_sSelfUid = SelfUidS;
	this->cell_num = 0;
	this->m_iCellType = CELL_TYPE_CLANSMAN;
	mSelfClanData = NULL;

	mClansmanInfo = ReadSpriteName("new_ui/gh/gh_txt3.png");
	mClansmanInfo->setPosition(Vec2(76, 304));
	mClansmanInfo->setAnchorPoint(Vec2(0, 0));
	this->addChild(mClansmanInfo);

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(CELL_WIDTH,4* CELL_HEIGHT));

    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(60,82));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();
 
	//申请列表按钮
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(490,54));
	this->addChild(ptrMenu);

	for (int i = 0; i < 2; i++)
	{
		Sprite* FuncNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
		Sprite* FuncSSp = ReadSpriteName("new_ui/img_global/global_ban2.png");

		Sprite* FuncNtSp = ReadSpriteName(__String::createWithFormat("new_ui/img_text/gh_ban%d.png",11+i*2)->getCString());
		FuncNtSp->setPosition(Vec2(FuncNSp->getContentSize().width / 2, FuncNSp->getContentSize().height / 2));
		Sprite* FuncStSp = ReadSpriteName(__String::createWithFormat("new_ui/img_text/gh_ban%d.png", 12+i*2)->getCString());
		FuncStSp->setPosition(Vec2(FuncSSp->getContentSize().width / 2, FuncSSp->getContentSize().height / 2));

		FuncNSp->addChild(FuncNtSp);
		FuncSSp->addChild(FuncStSp);

		MenuItemSprite* Func_menu = MenuItemSprite::create(FuncNSp, FuncSSp, CC_CALLBACK_1(ClansmanAdmTableView::clickFunctionMenu, this));
		Func_menu->setTag(i + 1);
		ptrMenu->addChild(Func_menu);
		mMenuItem[i+1] = Func_menu;
	}
	mMenuItem[2]->setVisible(false);

    return true;
}

void ClansmanAdmTableView::UpdataTbV(Json::Value vListData, int iType)
{
	this->m_iCellType = iType;

	if(iType == CELL_TYPE_CLANSMAN)
		mSelfClanData = NULL;

	updataMapList(vListData["peopleList"]);

	if (iType == CELL_TYPE_CLANSMAN && !mSelfClanData)
	{
		Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_DELETE_CLAN, mDelegate));
	}
	else
	{
		tableView->reloadData();
	}
}

void ClansmanAdmTableView::updataMapList(Json::Value vListData)
{
	map<int, ClansmanElem*> tListMap;

	if (m_iCellType == CELL_TYPE_CLANSMAN)
	{
		tListMap = mClansManList;
		mClansManList.clear();
	}
	else
	{
		tListMap = mApplyManList;
		mApplyManList.clear();
	}

	int iDataNum = vListData.size();
	int iCurListNum = tListMap.size();

	for (int i = 0; i < iDataNum; i++)
	{
		if (i >= iCurListNum)
		{
			tListMap[i] = new ClansmanElem();
		}

		tListMap[i]->LoadJson(vListData[i]);
		tListMap[i]->setEiIndex(i);

		if (m_iCellType == CELL_TYPE_CLANSMAN&&m_sSelfUid == tListMap[i]->getEsUid())
		{
			mSelfClanData = tListMap[i];
		}
	}

	if (iCurListNum > iDataNum)
	{
		for (int i = iDataNum; i < iCurListNum; i++)
			delete tListMap[i];
	}

	for (int i = 0; i < iDataNum; i++)
	{
		if (m_iCellType == CELL_TYPE_CLANSMAN)
			mClansManList[i] = tListMap[i];
		else
			mApplyManList[i] = tListMap[i];
	}

	this->cell_num = iDataNum;
}

void ClansmanAdmTableView::clickFunctionMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iTag = ((MenuItemSprite*)sender)->getTag();

	if (iTag == 1)
	{
		if (mSelfClanData&&mSelfClanData->getEiJob() < 3)
			SendApplyNetData();
		else
			ITools::getInstance()->createGlobalPrompt(LL("quanxianbuzu"), false);
		
	}
	else
	{
		SendClansmanNetData();
	}
}

ClansmanAdmTableView::ClansmanAdmTableView()
{
	cell_num = 1;
}

ClansmanAdmTableView::~ClansmanAdmTableView()
{
	for (size_t i = 0; i < mClansManList.size(); i++)
	{
		delete mClansManList[i];
	}
	for (size_t i = 0; i < mApplyManList.size(); i++)
	{
		delete mApplyManList[i];
	}
}

void ClansmanAdmTableView::scrollViewDidScroll(ScrollView* view)
{
	CCLOG("scrollViewDidScroll");
}

void ClansmanAdmTableView::scrollViewDidZoom(ScrollView* view)
{
	
}

void ClansmanAdmTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	CCLOG("tableCellTouched");
}

void ClansmanAdmTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClansmanAdmTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClansmanAdmTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(CELL_WIDTH,CELL_HEIGHT);
}

TableViewCell* ClansmanAdmTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

	ClansmanElem* ptrCellData = NULL;
	if (m_iCellType == CELL_TYPE_CLANSMAN)
	{
		ptrCellData = mClansManList[idx];
	}
	else
	{
		ptrCellData = mApplyManList[idx];
	}

	if (!cell) {
		cell = ClansmanAdmCell::create(ptrCellData, this ,m_iCellType,mSelfClanData->getEiJob());
	}
	else
	{
		ClansmanAdmCell* c = (ClansmanAdmCell*)cell;
		c->UpDateCell(ptrCellData, m_iCellType, mSelfClanData->getEiJob());
	}

    return cell;
}

ssize_t ClansmanAdmTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

void ClansmanAdmTableView::updateRTDate(int iIndex, int iType)
{
	iApprovalIntoIndex = iIndex;
	int iStatus = 0;
	if (iType == POP_CLAN_APPLY_SURE)
	{
		iStatus = 1;
	}
// 	else
// 	{
// 		iStatus = 0;
// 	}

	if(iType != 100)
		SendApplyApprovalNetData(mApplyManList[iIndex]->getEsUid(),iStatus);
	else
		ChessApplication::getInstance()->AddFunctionLayer(ClansmanAdminiLayer::create(mClansManList[iIndex], this));
}

void ClansmanAdmTableView::callBackListClick(int iJob)
{
	m_iSetGuildJob = iJob;

	if (iJob != 100)
		SendGuildJobNetData(iJob);
	else
		SendDeleteManNetData();
}

char* ClansmanAdmTableView::GetJobName(int iJobId)
{
	switch (iJobId)
	{
	case 1:
		return LL("zuzhang");
	case 2:
		return LL("fuzhuzhang");
	case 3:
		return LL("zhanglao");
	default:
		return LL("chengyuan");
	}
}

void ClansmanAdmTableView::SendClansmanNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&gameToken=%s", m_sGuildID.c_str(), GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_sGuildID.c_str(), GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("selectAllGuildUsers",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClansmanAdmTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendClansmanNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClansmanAdmTableView::SendApplyNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&gameToken=%s", m_sGuildID.c_str(), GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_sGuildID.c_str(), GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("selectAllGuildApply",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClansmanAdmTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendApplyNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClansmanAdmTableView::SendApplyApprovalNetData(string userID, int iStatus)
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&userID=%s&status=%d&gameToken=%s", m_sGuildID.c_str(), userID.c_str(),iStatus, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"userID\":\"%s\",\"status\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_sGuildID.c_str(), userID.c_str(), iStatus, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("ApprovalInto",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClansmanAdmTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendApplyApprovalNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClansmanAdmTableView::SendGuildJobNetData(int  iJob)
{
	if (!mClansManList.count(iApprovalIntoIndex))
		return;
	string userID = mClansManList[iApprovalIntoIndex]->getEsUid();

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&userID=%s&job=%d&gameToken=%s", m_sGuildID.c_str(), userID.c_str(),iJob, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"userID\":\"%s\",\"job\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_sGuildID.c_str(), userID.c_str(), iJob, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("setGuildJob",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClansmanAdmTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendGuildJobNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClansmanAdmTableView::SendDeleteManNetData()
{
	if (!mClansManList.count(iApprovalIntoIndex))
		return;
	string userID = mClansManList[iApprovalIntoIndex]->getEsUid();

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("userID=%s&gameToken=%s",userID.c_str(),GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"userID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", userID.c_str(),GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("ZZdeleteUserGuildByUser",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClansmanAdmTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendDeleteManNetData");

	ITools::getInstance()->addWaitLayer();
}


void ClansmanAdmTableView::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		Json::Value clanValue = value["result"];

		if (strTag == "SendClansmanNetData")
		{
			if (clanValue["peopleList"].isArray())
			{
				mMenuItem[2]->setVisible(false);
				mMenuItem[1]->setVisible(true);
				this->UpdataTbV(clanValue, CELL_TYPE_CLANSMAN);
				mClansmanInfo->setTexture(ReadSpriteTexture("new_ui/gh/gh_txt3.png"));
			}
		}
		else if (strTag == "SendApplyNetData")
		{
			if (clanValue["peopleList"].isArray())
			{
				mMenuItem[2]->setVisible(true);
				mMenuItem[1]->setVisible(false);
				this->UpdataTbV(clanValue, CELL_TYPE_APPLY);
				mClansmanInfo->setTexture(ReadSpriteTexture("new_ui/gh/gh_txt4.png"));
			}
		}
		else if (strTag == "SendApplyApprovalNetData"&&clanValue["isTrue"].isInt())
		{
			
			if (mApplyManList.count(iApprovalIntoIndex))
			{
				int iIsTrue = clanValue["isTrue"].asInt();
				string PeoplenameS = mApplyManList[iApprovalIntoIndex]->getEsName();
				if (iIsTrue)
				{
					ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("ssjrsz"),LL("tongyi"), PeoplenameS.c_str())->getCString(),true);
				}
				else
				{
					ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("ssjrsz"), LL("jujue"), PeoplenameS.c_str())->getCString(), false);
				}
			}

			iApprovalIntoIndex = -1;


			SendApplyNetData();
		}
		else if (strTag == "SendGuildJobNetData")
		{
			int iIsTrue = clanValue["isTrue"].asInt();
			if (mClansManList.count(iApprovalIntoIndex)&& iIsTrue)
			{
				//int iOldJob = mClansManList[iApprovalIntoIndex]->getEiJob();
				ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("sdzwbgws"), mClansManList[iApprovalIntoIndex]->getEsName().c_str(), GetJobName(m_iSetGuildJob))->getCString(), true);
				SendClansmanNetData();
			}
			else
			{
				ITools::getInstance()->createGlobalPrompt(LL("caozuoshibai"), true);
			}

			iApprovalIntoIndex = -1;
			m_iSetGuildJob = -1;
		}
		if (strTag == "SendDeleteManNetData")
		{
			if (clanValue["isTrue"].isInt() && clanValue["isTrue"].asInt())
			{
				ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("yjsqlsz"), mClansManList[iApprovalIntoIndex]->getEsName().c_str())->getCString(), true);
				SendClansmanNetData();
			}

			iApprovalIntoIndex = -1;
			m_iSetGuildJob = -1;
		}
	}
}
//////////////////////////////////////

ClansmanAdmCell* ClansmanAdmCell::create(ClansmanElem* ptrClansElem, EventDelegate* ptrDelegate,int iType,int iSelfJob)
{
	ClansmanAdmCell* pRet = new ClansmanAdmCell();
	if (pRet&&pRet->init(ptrClansElem,ptrDelegate,iType,iSelfJob))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ClansmanAdmCell::init(ClansmanElem* ptrClansElem, EventDelegate* ptrDelegate, int iType, int iSelfJob)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	m_iCurType = iType;
	m_iOldType = iType;
	m_pClansElem = ptrClansElem;
	mDelegate = ptrDelegate;
	m_iSelfJob = iSelfJob;

	mbg = ReadSpriteName("new_ui/gh/gh_box_bg6.png");
	mbg->setPosition(Vec2(CELL_WIDTH/2, CELL_HEIGHT /2));
	this->addChild(mbg);

	//形象
	int iBgHeight = mbg->getContentSize().height;

	int iFashion1 = ptrClansElem->getEFashion1();
	int iFashion2 = ptrClansElem->getEFashion2();

	if (ptrClansElem->getEFashion3() > 0)
		iFashion1 = ptrClansElem->getEFashion3();
	if (ptrClansElem->getEFashion4() > 0)
		iFashion2 = ptrClansElem->getEFashion4();

	int iGender = ptrClansElem->getEiGender();

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = trData->net_hero_dress[iFashion2]->mMov[6];

	spEquip = ReadSpriteNormal(strEquip->getCString());
	Size weaponSize = spEquip->getContentSize();

	spEquip->setPosition(Vec2(36, mbg->getContentSize().height / 2));
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	mbg->addChild(spEquip, iOrder);

	spHero = ReadSpriteNormal(strHero->getCString());
	Size heroSize = spHero->getContentSize();
	spHero->setPosition(spEquip->getPosition());
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));
	mbg->addChild(spHero, 1);

	//名字
	mNameLab = LabelSystemFont(ptrClansElem->getEsName().c_str(),16);
	mNameLab->setPosition(Vec2(63,45));
	mNameLab->setAnchorPoint(Vec2(0, 1));
	mNameLab->setColor(Color3B(102,0,0));
	mbg->addChild(mNameLab);

	//职位
	mJobNameLab = LabelSystemFont(GetJobName(ptrClansElem->getEiJob()), 16);
	mJobNameLab->setPosition(Vec2(66 + mNameLab->getContentSize().width,45));
	mJobNameLab->setAnchorPoint(Vec2(0,1));
	mJobNameLab->setColor(Color3B(204,102,0));
	mbg->addChild(mJobNameLab);

	//战力
	mAllCp = LabelSystemFont(__String::createWithFormat("%s  %d",LL("zhanli"), ptrClansElem->getEiAllCp())->getCString(),16);
	mAllCp->setPosition(Vec2(63,24));
	mAllCp->setAnchorPoint(Vec2(0,1));
	mAllCp->setColor(Color3B(102,51,0));
	mbg->addChild(mAllCp);

	//王族
	mIsVip = ReadSpriteName("new_ui/main/vip1.png");
	mIsVip->setPosition(Vec2(202, mbg->getContentSize().height / 2));
	mIsVip->setAnchorPoint(Vec2(0,0.5));
	mbg->addChild(mIsVip);
	if (!ptrClansElem->getEiIsVip())
		mIsVip->setVisible(false);

	//最后登录
	mLastLogin = LabelSystemFont(__String::createWithFormat("%s:%s", LL("zuihoudenglu"),ptrClansElem->getEsLastLogin().c_str())->getCString(),16);
	mLastLogin->setPosition(Vec2(280,45));
	mLastLogin->setColor(Color3B(102, 0, 0));
	mLastLogin->setAnchorPoint(Vec2(0,1));
	mbg->addChild(mLastLogin);

	//贡献
	mActive = LabelSystemFont(__String::createWithFormat("%s %s %s %d", LL("zonghuoyue"),IntToStringEx(ptrClansElem->getEiAllActive()) ,LL("jinri"),ptrClansElem->getEiTodayActive())->getCString(), 14);
	mActive->setPosition(Vec2(281, 24));
	mActive->setColor(Color3B(102, 51, 0));
	mActive->setAnchorPoint(Vec2(0, 1));
	mbg->addChild(mActive);

	//管理
	mAdmini = ReadSpriteName("new_ui/battle/battle_up.png");
	mAdmini->setPosition(Vec2(460,mbg->getContentSize().height/2));
	mbg->addChild(mAdmini);

	//同意
	mSure = ReadSpriteName("new_ui/battle/battle_up.png");
	mSure->setPosition(Vec2(452, mbg->getContentSize().height / 2));
	mbg->addChild(mSure);

	//取消
	mCancle = ReadSpriteName("new_ui/battle/battle_down.png");
	mCancle->setPosition(Vec2(395, mbg->getContentSize().height / 2));
	mbg->addChild(mCancle);


	if (iType == CELL_TYPE_CLANSMAN)
	{
		mSure->setVisible(false);
		mCancle->setVisible(false);

		if (ptrClansElem->getEiJob() == 1|| m_iSelfJob != 1)
			mAdmini->setVisible(false);
	}
	else
	{
		mJobNameLab->setVisible(false);
		mLastLogin->setVisible(false);
		mActive->setVisible(false);
		mAdmini->setVisible(false);
		mIsVip->setPosition(Vec2(170, mbg->getContentSize().height / 2));
	}


	initTouches();

	return true;
}

ClansmanAdmCell::ClansmanAdmCell()
{
	m_ptrLisTouch = NULL;
}

ClansmanAdmCell::~ClansmanAdmCell()
{
	if (m_ptrLisTouch)
	{
		_eventDispatcher->removeEventListener(m_ptrLisTouch);
	}
}

void ClansmanAdmCell::UpDateCell(ClansmanElem* ptrClansElem, int iType ,int iSelfJob)
{
	m_iSelfJob = iSelfJob;
	m_iCurType = iType;
	m_pClansElem = ptrClansElem;

	if (m_iCurType != m_iOldType)
	{
		if (iType == CELL_TYPE_CLANSMAN)
		{
			mSure->setVisible(false);
			mCancle->setVisible(false);

			mIsVip->setPosition(Vec2(202, mbg->getContentSize().height / 2));

			mJobNameLab->setVisible(true);
			mLastLogin->setVisible(true);
			mActive->setVisible(true);
			mAdmini->setVisible(true);
		}
		else
		{
			mJobNameLab->setVisible(false);
			mLastLogin->setVisible(false);
			mActive->setVisible(false);
			mAdmini->setVisible(false);
			mIsVip->setPosition(Vec2(170, mbg->getContentSize().height / 2));

			mSure->setVisible(true);
			mCancle->setVisible(true);
		}
	}


	int iFashion1 = ptrClansElem->getEFashion1();
	int iFashion2 = ptrClansElem->getEFashion2();

	if (ptrClansElem->getEFashion3() > 0)
		iFashion1 = ptrClansElem->getEFashion3();
	if (ptrClansElem->getEFashion4() > 0)
		iFashion2 = ptrClansElem->getEFashion4();

	int iGender = ptrClansElem->getEiGender();

	__String* strHero = __String::createWithFormat("img/Nunits/%d_%d_mov.png", iGender, iFashion1);
	__String* strEquip = __String::createWithFormat("img/Nunits/100_%d_mov.png", iFashion2);

	int iOrder = 0;
	if (trData->net_hero_dress[iFashion2]->mMov[6])
	{
		iOrder = 2;
	}

	Texture2D* ptrTexture2D = GetSpriteTexture(strEquip->getCString());
	Size weaponSize = ptrTexture2D->getContentSize();
	spEquip->setTexture(ptrTexture2D);
	spEquip->setTextureRect(Rect(0, weaponSize.width * 6, weaponSize.width, weaponSize.width));
	spEquip->setLocalZOrder(iOrder);

	ptrTexture2D = GetSpriteTexture(strHero->getCString());
	Size heroSize = ptrTexture2D->getContentSize();
	spHero->setTexture(ptrTexture2D);
	spHero->setTextureRect(Rect(0, heroSize.width * 6, heroSize.width, heroSize.width));

	mNameLab->setString(ptrClansElem->getEsName().c_str());

	mJobNameLab->setString(GetJobName(ptrClansElem->getEiJob()));
	mJobNameLab->setPosition(Vec2(66 + mNameLab->getContentSize().width, 45));

	mAllCp->setString(__String::createWithFormat("%s  %d", LL("zhanli"), ptrClansElem->getEiAllCp())->getCString());

	if (!ptrClansElem->getEiIsVip())
		mIsVip->setVisible(false);
	else
		mIsVip->setVisible(true);

	if (iType == CELL_TYPE_CLANSMAN)
	{
		mLastLogin->setString(__String::createWithFormat("%s:%s", LL("zuihoudenglu"), ptrClansElem->getEsLastLogin().c_str())->getCString());

		mActive->setString(__String::createWithFormat("%s %s %s %d", LL("zonghuoyue"), IntToStringEx(ptrClansElem->getEiAllActive()), LL("jinri"), ptrClansElem->getEiTodayActive())->getCString());

		if (ptrClansElem->getEiJob() == 1|| m_iSelfJob != 1)
		 	mAdmini->setVisible(false);
		else
		 	mAdmini->setVisible(true);
	}

	m_iOldType = iType;
}

char* ClansmanAdmCell::GetJobName(int iJobId)
{
	switch (iJobId)
	{
	case 1:
		return LL("zuzhang");
	case 2:
		return LL("fuzhuzhang");
	case 3:
		return LL("zhanglao");
	default:
		return LL("chengyuan");
	}
}

void ClansmanAdmCell::initTouches()
{
	m_ptrLisTouch = EventListenerTouchOneByOne::create();
	m_ptrLisTouch->onTouchBegan = [&](Touch* touch, Event* event) {
		m_iClickIndex = -1;
		if (m_iCurType == CELL_TYPE_CLANSMAN)
		{
			Rect adminiBox = mAdmini->getBoundingBox();
			if (m_pClansElem->getEiJob() != 1&& mAdmini->isVisible()&&Rect(0, 0, adminiBox.size.width, adminiBox.size.height).containsPoint(mAdmini->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickIndex = 1;
				mAdmini->runAction(ScaleTo::create(0.1f,0.9f));
			}
		}
		else
		{
			Rect cancleBox = mCancle->getBoundingBox();
			Rect sureBox = mSure->getBoundingBox();
			if (Rect(0, 0, cancleBox.size.width, cancleBox.size.height).containsPoint(mCancle->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickIndex = 2;
				mCancle->runAction(ScaleTo::create(0.1f, 0.9f));
			}
			else if(Rect(0, 0, sureBox.size.width, sureBox.size.height).containsPoint(mSure->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickIndex = 3;
				mSure->runAction(ScaleTo::create(0.1f, 0.9f));
			}
		}
		return true;
	};

	m_ptrLisTouch->onTouchMoved = [&](Touch* touch, Event* event) {
		if (m_iClickIndex > -1)
		{
			Sprite* TempSp = NULL;
			if (m_iClickIndex == 1)
			{
				TempSp = mAdmini;
			}
			else if (m_iClickIndex == 2)
			{
				TempSp = mCancle;
			}
			else
			{
				TempSp = mSure;
			}

			Rect tempBox = TempSp->getBoundingBox();
			if (!Rect(0, 0, tempBox.size.width, tempBox.size.height).containsPoint(TempSp->convertToNodeSpace(touch->getLocation())))
			{
				m_iClickIndex = -1;
				TempSp->runAction(ScaleTo::create(0.1f, 1));
			}
		}
	};

	m_ptrLisTouch->onTouchEnded = [&](Touch* touch, Event* event) {
		if (m_iClickIndex > -1)
		{
			Vec2 vOffset = touch->getLocation() - touch->getStartLocation();

			Sprite* TempSp = NULL;
			if (m_iClickIndex == 1)
			{
				TempSp = mAdmini;
			}
			else if (m_iClickIndex == 2)
			{
				TempSp = mCancle;
			}
			else
			{
				TempSp = mSure;
			}
			TempSp->runAction(ScaleTo::create(0.1f, 1));

			if (vOffset.x<2 && vOffset.x>-2 && vOffset.y<2 && vOffset.y>-2)
			{
				if (m_iClickIndex == 1)
				{
					mDelegate->updateRTDate(m_pClansElem->getEiIndex(), 100);
				}
				else if (m_iClickIndex == 2)
				{
					Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_CLAN_APPLY_CANCLE, this));
				}
				else if(m_iClickIndex == 3)
				{
					Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_CLAN_APPLY_SURE, this));
				}
			}
		}
	};

	m_ptrLisTouch->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_ptrLisTouch, this);
}

void ClansmanAdmCell::SetPopLayerElement(Sprite* bg)
{
	Label* infoLab = NULL;
	if (m_iClickIndex == 2)
	{
		infoLab = LabelSystemFont(__String::createWithFormat(LL("sfssdsq"),LL("jujue"), m_pClansElem->getEsName().c_str())->getCString(),16);
	}
	if (m_iClickIndex == 3)
	{
		infoLab = LabelSystemFont(__String::createWithFormat(LL("sfssdsq"), LL("tongyi"), m_pClansElem->getEsName().c_str())->getCString(), 16);
	}
	
	if (infoLab)
	{
		infoLab->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(infoLab);
	}
}

void ClansmanAdmCell::RunFunction(int iNomal)
{
	if (!mDelegate) return;
	if (iNomal == POP_CLAN_APPLY_CANCLE)
	{
		mDelegate->updateRTDate(m_pClansElem->getEiIndex(), POP_CLAN_APPLY_CANCLE);
	}
	else if (iNomal == POP_CLAN_APPLY_SURE)
	{
		mDelegate->updateRTDate(m_pClansElem->getEiIndex(), POP_CLAN_APPLY_SURE);
	}
}


///////////////////////////

ClansmanAdminiLayer* ClansmanAdminiLayer::create(ClansmanElem* ptrClansMan,EventDelegate* ptrDelegate)
{
	ClansmanAdminiLayer* pRet = new ClansmanAdminiLayer();
	if (pRet&&pRet->init(ptrClansMan,ptrDelegate))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool ClansmanAdminiLayer::init(ClansmanElem* ptrClansMan, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;
	m_iSelectJobId = 0;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg5.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	Label* name1S = LabelSystemFont(__String::createWithFormat("%s ",LL("duis"))->getCString(),16);
	name1S->setAnchorPoint(Vec2(0,0.5));
	name1S->enableOutline(Color4B(158, 116, 48, 255), 1);
	bg->addChild(name1S);

	Label* name2S = LabelSystemFont(ptrClansMan->getEsName().c_str(), 16);
	name2S->setAnchorPoint(Vec2(0, 0.5));
	name2S->enableOutline(Color4B(158, 116, 48, 255), 1);
	name2S->setColor(Color3B::GREEN);
	bg->addChild(name2S);

	Label* name3S = LabelSystemFont(__String::createWithFormat(" %s", LL("guanlizhong"))->getCString(), 16);
	name3S->setAnchorPoint(Vec2(0, 0.5));
	name3S->enableOutline(Color4B(158, 116, 48, 255), 1);
	bg->addChild(name3S);

	float fName1W = name1S->getContentSize().width;
	float fName2W = name2S->getContentSize().width;
	float fName3W = name3S->getContentSize().width;

	float fStartX = (bg->getContentSize().width - fName1W - fName2W - fName3W)/2;

	name1S->setPosition(Vec2(fStartX,310));
	name2S->setPosition(Vec2(fStartX + fName1W+2, 310));
	name3S->setPosition(Vec2(fStartX + fName2W + fName1W, 310));

	
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(60,274));
	bg->addChild(ptrMenu);

	int iPointYIndex = 0;
	for (int i = 0; i < 5; i++)
	{
		if(i+1 == ptrClansMan->getEiJob())
			continue;

		Sprite* mItemNomal = ReadSpriteName("new_ui/battle/box_no.png");
		Sprite* mItemSelect = ReadSpriteName("new_ui/battle/box_ok.png");
		Sprite* mItemDis = ReadSpriteName("new_ui/battle/box_ok.png");

		MenuItemSprite* mItem = MenuItemSprite::create(mItemNomal, mItemSelect, mItemDis, CC_CALLBACK_1(ClansmanAdminiLayer::clickItemMenu,this));
		mItem->setPosition(Vec2(0,-38 * iPointYIndex));
		mItem->setTag(i + 1);
		ptrMenu->addChild(mItem);

		Label* jobLab = LabelSystemFont(GetJobName(i+1),18);
		jobLab->setPosition(Vec2(90, 274-38 * iPointYIndex));
		jobLab->setAnchorPoint(Vec2(0,0.5));
		bg->addChild(jobLab);

		if (!iPointYIndex)
			mSelectMenu = mItem;

		if (i < 4)
			jobLab->setColor(Color3B(102, 0, 0));
		else
			jobLab->setColor(Color3B::RED);

		iPointYIndex++;
	}

	//确定
	Menu* sureMenu = Menu::create();
	sureMenu->setPosition(Vec2(0,0));
	bg->addChild(sureMenu);

	Sprite* FuncNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* FuncSSp = ReadSpriteName("new_ui/img_global/global_ban2.png");

	Sprite* FuncNtSp = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	FuncNtSp->setPosition(Vec2(FuncNSp->getContentSize().width / 2, FuncNSp->getContentSize().height / 2));
	Sprite* FuncStSp = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	FuncStSp->setPosition(Vec2(FuncSSp->getContentSize().width / 2, FuncSSp->getContentSize().height / 2));

	FuncNSp->addChild(FuncNtSp);
	FuncSSp->addChild(FuncStSp);

	MenuItemSprite* Func_menu = MenuItemSprite::create(FuncNSp, FuncSSp, CC_CALLBACK_1(ClansmanAdminiLayer::clickSureMenu, this));
	Func_menu->setPosition(Vec2(bg->getContentSize().width/2,66));
	sureMenu->addChild(Func_menu);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClansmanAdminiLayer::clickCloseMenu, this));
	close_menu->setPosition(Vec2(324,355));
	sureMenu->addChild(close_menu);

	return true;
}

bool ClansmanAdminiLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void ClansmanAdminiLayer::clickItemMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	mSelectMenu->setEnabled(true);

	mSelectMenu = (MenuItemSprite*)sender;

	mSelectMenu->setEnabled(false);

	int iTag = mSelectMenu->getTag();

	if (iTag == 5)
		m_iSelectJobId = 100;
	else
		m_iSelectJobId = iTag;
}

void ClansmanAdminiLayer::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_iSelectJobId < 1)
		ITools::getInstance()->createGlobalPrompt(LL("meiyouxuanze"),false);
	else if (mDelegate)
	{
		mDelegate->callBackListClick(m_iSelectJobId);
		this->removeFromParent();
	}

}

void ClansmanAdminiLayer::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();

}

char* ClansmanAdminiLayer::GetJobName(int iJobId)
{
	switch (iJobId)
	{
	case 1:
		return LL("zuzhang");
	case 2:
		return LL("fuzhuzhang");
	case 3:
		return LL("zhanglao");
	case 4:
		return LL("chengyuan");
	default:
		return LL("tichushizu");
	}
}

ClansmanAdminiLayer::ClansmanAdminiLayer()
{

}

ClansmanAdminiLayer::~ClansmanAdminiLayer()
{

}