#include "ClanHomePageLayer.h"
#include "Defines.h"
#include "ClansmanAdmTableView.h"
#include "GetNetDateTool.h"
#include "PopLayer.h"
#include "ClanApplyListLayer.h"
#include "ClanActiveTaskTableView.h"
#include "ClanArmyBattleLayer.h"

ClanHomePageLayer* ClanHomePageLayer::create(Json::Value vDataValue)
{
	ClanHomePageLayer* pRet = new ClanHomePageLayer();
	if (pRet&&pRet->init(vDataValue))
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

bool ClanHomePageLayer::init(Json::Value vDataValue)
{
	if (!Layer::init())
	{
		return false;
	}
	mClansmanTB = NULL;
	mClanLogTB = NULL;

	GetData(vDataValue);

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg1.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);
	mbg = bg;

	mCenterBg = ReadSpriteName("new_ui/gh/gh_bg1.png");
	mCenterBg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(mCenterBg);
	mCenterBg->setOpacity(0);

	mBgInfo = ReadSpriteName("new_ui/gh/gh_til2.png");
	mBgInfo->setPosition(Vec2(bg->getContentSize().width / 2, 355));
	bg->addChild(mBgInfo);

	//50,48
	Sprite* leftSp = ReadSpriteName("new_ui/gh/gh_bg3.png");
	leftSp->setAnchorPoint(Vec2(0,0));
	leftSp->setPosition(Vec2(50,48));
	mCenterBg->addChild(leftSp);

	Label* clanName = LabelSystemFont(mClanData.getClanName().c_str(),20);
	clanName->setPosition(Vec2(leftSp->getContentSize().width/2,257));
	clanName->enableOutline(Color4B(51, 0, 0, 255), 1);
	leftSp->addChild(clanName);

	//族长
	Label* zuzhangLab = LabelSystemFont(__String::createWithFormat("%s:  ",LL("zuzhang"))->getCString(),16);
	zuzhangLab->setAnchorPoint(Vec2(0, 1));
	zuzhangLab->setPosition(Vec2(18,228));
	zuzhangLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(zuzhangLab);

	mElderNameLab = LabelSystemFont(mClanData.getElderName().c_str(),16);
	mElderNameLab->setAnchorPoint(Vec2(0, 1));
	mElderNameLab->setPosition(Vec2(22 + zuzhangLab->getContentSize().width, 228));
	mElderNameLab->setColor(Color3B::YELLOW);
	mElderNameLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mElderNameLab);

	//ID
	mClanId = LabelSystemFont(__String::createWithFormat("%s  %d", LL("idhao"), mClanData.getClanId())->getCString(),16);
	mClanId->setAnchorPoint(Vec2(0,1));
	mClanId->setPosition(Vec2(18,208));
	mClanId->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mClanId);

	//人数
	mPeoNum = LabelSystemFont(__String::createWithFormat("%s  %d/%d", LL("renshu"), mClanData.getHeroNum(), mClanData.getHeroMaxNum())->getCString(), 16);
	mPeoNum->setAnchorPoint(Vec2(0, 1));
	mPeoNum->setPosition(Vec2(18, 188));
	mPeoNum->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mPeoNum);

	//活跃
	mActiveLab = LabelSystemFont(__String::createWithFormat("%s  %d/%d", LL("huoyue"), mClanData.getEActive(), mClanData.getEMaxActive())->getCString(), 16);
	mActiveLab->setAnchorPoint(Vec2(0, 1));
	mActiveLab->setPosition(Vec2(18, 168));
	mActiveLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mActiveLab);

	//军费
	mMilLab = LabelSystemFont(__String::createWithFormat("%s  0/0", LL("junfei"))->getCString(), 16);
	mMilLab->setAnchorPoint(Vec2(0, 1));
	mMilLab->setPosition(Vec2(18, 148));
	mMilLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mMilLab);

	//宗旨
	mZzLab = LabelSystemFont(mClanData.getZongZhi().c_str(),16);
	mZzLab->setPosition(Vec2(18, 113));
	mZzLab->setAnchorPoint(Vec2(0,1));
	mZzLab->setDimensions(200, 0);
	mZzLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	leftSp->addChild(mZzLab);

	//退出氏族
	string shizuNomal,shizuSelect;
	if (m_iSelfJob == 1)
	{
		shizuNomal = "new_ui/img_text/gh_ban7.png";
		shizuSelect = "new_ui/img_text/gh_ban8.png";
	}
	else
	{
		shizuNomal = "new_ui/img_text/gh_ban9.png";
		shizuSelect = "new_ui/img_text/gh_ban10.png";
	}

	Sprite* shizuNSp = ReadSpriteName("new_ui/img_global/global_ban21.png");
	mShizuNTSP = ReadSpriteName(shizuNomal.c_str());
	mShizuNTSP->setPosition(Vec2(shizuNSp->getContentSize().width / 2, shizuNSp->getContentSize().height / 2));
	shizuNSp->addChild(mShizuNTSP);

	Sprite* shizuSSp = ReadSpriteName("new_ui/img_global/global_ban22.png");
	mShizuSTSP = ReadSpriteName(shizuSelect.c_str());
	mShizuSTSP->setPosition(Vec2(shizuSSp->getContentSize().width / 2, shizuSSp->getContentSize().height / 2));
	shizuSSp->addChild(mShizuSTSP);

	MenuItemSprite* btnshizu = MenuItemSprite::create(shizuNSp, shizuSSp, CC_CALLBACK_1(ClanHomePageLayer::clickTuiChuMenu, this));

	Menu* menushizu = Menu::create();
	menushizu->setPosition(Vec2(leftSp->getContentSize().width/2, 25));
	menushizu->addChild(btnshizu);
	leftSp->addChild(menushizu);

	//改宗旨200,216
	mSetZongZhi = Menu::create();
	mSetZongZhi->setPosition(Vec2(195,205));
	leftSp->addChild(mSetZongZhi);

	Sprite* setNSp = ReadSpriteName("new_ui/gh/gh_icon99.png");
	Sprite* setSSp = ReadSpriteName("new_ui/gh/gh_icon99.png");
	setSSp->setScale(0.9f);

	MenuItemSprite* setItemM = MenuItemSprite::create(setNSp, setSSp,CC_CALLBACK_1(ClanHomePageLayer::clickSetZongZhiMenu,this));
	mSetZongZhi->addChild(setItemM);
	setSSp->setAnchorPoint(Vec2(0.5,0.5));
	setSSp->setPosition(Vec2(setSSp->getContentSize().width/2, setSSp->getContentSize().height/2));

	if (m_iSelfJob > 2)
		mSetZongZhi->setVisible(false);


	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(584, 364));
	bg->addChild(mCloseMenu,2);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClanHomePageLayer::clickBackMenu, this));
	mCloseMenu->addChild(close_menu);


	//活跃任务
	Menu* rightMenu = Menu::create();
	rightMenu->setPosition(Vec2(0, 0));;
	mCenterBg->addChild(rightMenu);


	Sprite* SpNomal = NULL;
	Sprite* SpSelect = NULL;
	Sprite* SpDisable = NULL;
	string nameS = "";
	for (int i = 0; i < 7; i++)
	{
		nameS = StringUtils::format("new_ui/gh/gh_icon%d.png",i+1);
		SpNomal = ReadSpriteName(nameS.c_str());
		SpSelect = ReadSpriteName(nameS.c_str());
		SpDisable = ReadSpriteName(nameS.c_str());
		addSpriteGray(SpDisable);
		SpSelect->setScale(0.9f);
		MenuItemSprite* menuIt = MenuItemSprite::create(SpNomal, SpSelect, SpDisable, CC_CALLBACK_1(ClanHomePageLayer::clickRightMenu, this));
		menuIt->setPosition(Vec2(336 + (i % 3) * 88, 270 - (i / 3) * 88));
		menuIt->setTag(i + 1);
		rightMenu->addChild(menuIt);
		SpSelect->setAnchorPoint(Vec2(0.5f,0.5f));
		SpSelect->setPosition(Vec2(SpNomal->getContentSize().width/2,SpNomal->getContentSize().height/2));

		if (i > 1)
			menuIt->setEnabled(false);
	}

	Menu* funcMenu = Menu::create();
	funcMenu->setPosition(Vec2(0, 0));
	bg->addChild(funcMenu,2);

	for (int i = 0; i < 3; i++)
	{
		Sprite* FuncNSp = ReadSpriteName("new_ui/img_global/global_ban19.png");
		Sprite* FuncSSp = ReadSpriteName("new_ui/img_global/global_ban20.png");
		Sprite* FuncDsSp = ReadSpriteName("new_ui/img_global/global_ban20.png");

		Sprite* FuncNtSp = ReadSpriteName(__String::createWithFormat("new_ui/img_text/gh_bant%d.png",i*2+1)->getCString());
		FuncNtSp->setPosition(Vec2(FuncNSp->getContentSize().width / 2, FuncNSp->getContentSize().height / 2));
		Sprite* FuncStSp = ReadSpriteName(__String::createWithFormat("new_ui/img_text/gh_bant%d.png", i * 2 + 2)->getCString());
		FuncStSp->setPosition(Vec2(FuncSSp->getContentSize().width / 2, FuncSSp->getContentSize().height / 2));
		Sprite* FuncDtsSp = ReadSpriteName(__String::createWithFormat("new_ui/img_text/gh_bant%d.png", i * 2 + 2)->getCString());
		FuncDtsSp->setPosition(Vec2(FuncDsSp->getContentSize().width / 2, FuncDsSp->getContentSize().height / 2));

		FuncNSp->addChild(FuncNtSp);
		FuncSSp->addChild(FuncStSp);
		FuncDsSp->addChild(FuncDtsSp);

		MenuItemSprite* Func_menu = MenuItemSprite::create(FuncNSp, FuncSSp, FuncDsSp, CC_CALLBACK_1(ClanHomePageLayer::clickFunctionMenu, this));
		Func_menu->setPosition(Vec2(594, 280 - i*60));
		Func_menu->setTag(i + 1);
		funcMenu->addChild(Func_menu);
		if (i == 0)
		{
			Func_menu->setEnabled(false);
			mSelectMenu = Func_menu;
		}
	}


	return true;
}

void ClanHomePageLayer::GetData(Json::Value vDataValue)
{
	m_GuildIDS = "";
	m_sSelfUid = "";
	m_iSelfJob = 0;

	mClanData.LoadJson(vDataValue);

	if (vDataValue["id"].isString()) 
	{
		m_GuildIDS = vDataValue["id"].asString();
	}

	if (vDataValue["uid"].isString())
	{
		m_sSelfUid = vDataValue["uid"].asString();
	}

	if (vDataValue["job"].isInt())
	{
		m_iSelfJob = vDataValue["job"].asInt();
	}

}

void ClanHomePageLayer::UpdataLeftShow(Json::Value vDataValue)
{
	int iOldJob = m_iSelfJob;

	GetData(vDataValue);

	mElderNameLab->setString(mClanData.getElderName().c_str());
	//ID
	mClanId->setString(__String::createWithFormat("%s  %d", LL("idhao"), mClanData.getClanId())->getCString());
	//人数
	mPeoNum->setString(__String::createWithFormat("%s  %d/%d", LL("renshu"), mClanData.getHeroNum(), mClanData.getHeroMaxNum())->getCString());
	//活跃
	mActiveLab->setString(__String::createWithFormat("%s  %d/%d", LL("huoyue"), mClanData.getEActive(),mClanData.getEMaxActive())->getCString());
	//军费
	mMilLab->setString(__String::createWithFormat("%s  0/0", LL("junfei"))->getCString());
	//宗旨
	mZzLab->setString(mClanData.getZongZhi().c_str());

	if (iOldJob != m_iSelfJob&&(iOldJob == 1 || m_iSelfJob == 1))
	{
		if (m_iSelfJob == 1)
		{
			mShizuNTSP->setTexture(ReadSpriteTexture("new_ui/img_text/gh_ban7.png"));
			mShizuSTSP->setTexture(ReadSpriteTexture("new_ui/img_text/gh_ban8.png"));
		}
		else
		{
			mShizuNTSP->setTexture(ReadSpriteTexture("new_ui/img_text/gh_ban9.png"));
			mShizuSTSP->setTexture(ReadSpriteTexture("new_ui/img_text/gh_ban10.png"));
		}
	}

	if (m_iSelfJob > 2)
		mSetZongZhi->setVisible(false);
	else
		mSetZongZhi->setVisible(true);
}

void ClanHomePageLayer::updateRTDate(std::string nomalS)
{
	mZzLab->setString(nomalS.c_str());
	mClanData.setZongZhi(nomalS);
}

void ClanHomePageLayer::SetPopLayerElement(Sprite* ptrBg)
{
	Label* titleLab = NULL;
	if (m_iSelfJob == 1)
	{
		titleLab = LabelSystemFont(__String::createWithFormat(LL("sfjsssz"), mClanData.getClanName().c_str())->getCString(), 16);
	}
	else
	{
		titleLab = LabelSystemFont(__String::createWithFormat(LL("sftcssz"),mClanData.getClanName().c_str())->getCString(),16);
	}

	titleLab->setPosition(Vec2(ptrBg->getContentSize().width / 2, 132));
	ptrBg->addChild(titleLab);
}

void ClanHomePageLayer::RunFunction(int iNomal)
{
	if (m_iSelfJob == 1)
	{
		SendDisbandmentNetData();
	}
	else
	{
		SendLeaveNetData();
	}
}

void ClanHomePageLayer::ChildCloseLayer(int iNomal)
{
	this->removeFromParent();
}

void ClanHomePageLayer::updateShow(int iNomal)
{
	SendClanInfoData();
}

void ClanHomePageLayer::clickBackMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void ClanHomePageLayer::clickTuiChuMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_CLAN_DELETE, this));
}

void ClanHomePageLayer::clickRightMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iTag = ((MenuItemSprite*)sender)->getTag();

	if (iTag == 1)
	{
		//活跃任务
		SendActiveTaskNetData();
	}
	else if (iTag == 2)
	{
		//团队副本
		SendArmyFuBenNetData();
	}
	else if (iTag == 3)
	{
		//攻防战
	}
	else if (iTag == 4)
	{
		//贡献
	}
	else if (iTag == 5)
	{
		//兵营
	}
	else if (iTag == 6)
	{
		//城池站
	}
	else if (iTag == 7)
	{
		//军机处
	}

}

void ClanHomePageLayer::clickFunctionMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	mSelectMenu->setEnabled(true);

	mSelectMenu = (MenuItemSprite*)sender;

	mSelectMenu->setEnabled(false);

	if (mClansmanTB)
	{
		mClansmanTB->removeFromParent();
		mClansmanTB = NULL;
	}
	if (mClanLogTB)
	{
		mClanLogTB->removeFromParent();
		mClanLogTB = NULL;
	}

	int iTag = mSelectMenu->getTag();

	if (iTag == 1)
	{
		//氏族
		mCenterBg->setVisible(true);
		SendClanInfoData();

		mBgInfo->setTexture(ReadSpriteTexture("new_ui/gh/gh_til2.png"));
	}
	else if (iTag == 2)
	{
		//族人
		SendClansmanNetData();

		mBgInfo->setTexture(ReadSpriteTexture("new_ui/gh/gh_til3.png"));
	}
	else if (iTag == 3)
	{
		//日志
		SendClanLogNetData();

		mBgInfo->setTexture(ReadSpriteTexture("new_ui/gh/gh_til4.png"));
	}
}

void ClanHomePageLayer::clickSetZongZhiMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_iSelfJob > 2)
	{
		return;
	}
	ChessApplication::getInstance()->AddFunctionLayer(ApplyCreateClanLayer::create(this, TYPE_SET_ZONGZHI, mClanData.getClanName(), m_GuildIDS));
}

ClanHomePageLayer::ClanHomePageLayer()
{

}

ClanHomePageLayer::~ClanHomePageLayer()
{

}

void ClanHomePageLayer::SendClansmanNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&gameToken=%s", m_GuildIDS.c_str(), GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_GuildIDS.c_str(), GameTokenS.c_str(), validateCode.c_str());
	CCLOG("++++++  %s", data->getCString());
	ITools::getInstance()->connectingNetwork("selectAllGuildUsers",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendClansmanNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendClanInfoData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildByUser",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendClanInfoData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendLeaveNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("deleteUserGuildByUser",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendLeaveNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendDisbandmentNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&gameToken=%s", m_GuildIDS.c_str(),GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_GuildIDS.c_str(), GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("deleteGuild",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendDisbandmentNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendActiveTaskNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getGuildActiveTask",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendActiveTaskNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendClanLogNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildLogByGuildLog",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendClanLogNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::SendArmyFuBenNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getGuildDate",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanHomePageLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendArmyFuBenNetData");

	ITools::getInstance()->addWaitLayer();
}

void ClanHomePageLayer::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
				mCenterBg->setVisible(false);
				mClansmanTB = ClansmanAdmTableView::create(m_GuildIDS, m_sSelfUid,this);
				mbg->addChild(mClansmanTB);
				mClansmanTB->UpdataTbV(clanValue, CELL_TYPE_CLANSMAN);
			}
		}
		else if (strTag == "Post_SendClanInfoData")
		{
			if (clanValue["isTrue"].isInt() && clanValue["isTrue"].asInt())
			{
				UpdataLeftShow(clanValue);
			}
			else if (clanValue["isTrue"].isInt() && clanValue["isTrue"].asInt() == 0)
			{
				Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_DELETE_CLAN, this));
			}
		}
		else if (strTag == "SendLeaveNetData" || strTag == "SendDisbandmentNetData")
		{
			if (clanValue["isTrue"].isInt() && clanValue["isTrue"].asInt())
			{
				GetNetDateTool::getInstance()->SendClanInfoData();

				if (strTag == "SendLeaveNetData")
				{
					ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("ytcssz"), mClanData.getClanName().c_str())->getCString(), true);
				}
				else
				{
					ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("yjsssz"), mClanData.getClanName().c_str())->getCString(), true);
				}

			}

			this->removeFromParent();

		}
		else if (strTag == "SendActiveTaskNetData")
		{
			if(clanValue["tasks"].isArray())
				ChessApplication::getInstance()->AddFunctionLayer(ClanActiveTaskTableView::create(clanValue,this));
		}
		else if (strTag == "SendClanLogNetData")
		{
			if (clanValue["logArr"].isArray())
			{
				mCenterBg->setVisible(false);
				mClanLogTB = ClanLogTableView::create(clanValue["logArr"]);
				mbg->addChild(mClanLogTB);
			}
		}
		else if (strTag == "SendArmyFuBenNetData")
		{
			if (clanValue["guildWarArr"].isArray())
			{
				ChessApplication::getInstance()->AddFunctionLayer(ClanArmyBattleLayer::create(clanValue,this, m_iSelfJob,m_GuildIDS));
			}
		}
	}
}