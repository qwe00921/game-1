#include "ClanApplyListLayer.h"
#include "Defines.h"
#include "ClanListTableView.h"
#include "ClanHomePageLayer.h"
#include "GetNetDateTool.h"

void ClanListElem::LoadJson(Json::Value vElemValue)
{
	if (vElemValue["guildName"].isString())
		setClanName(vElemValue["guildName"].asString());

	if(vElemValue["num"].isInt())
		setClanId(vElemValue["num"].asInt());

	if (vElemValue["guildLV"].isInt())
		setClanLv(vElemValue["guildLV"].asInt());

	if (vElemValue["patriarch"].isString())
		setElderName(vElemValue["patriarch"].asString());

	if (vElemValue["peopleNum"].isInt())
		setHeroNum(vElemValue["peopleNum"].asInt());

	if (vElemValue["introduce"].isString())
		setZongZhi(vElemValue["introduce"].asString());

	if (vElemValue["maxPeopleNum"].isInt())
		setHeroMaxNum(vElemValue["maxPeopleNum"].asInt());

	if (vElemValue["active"].isInt())
		setEActive(vElemValue["active"].asInt());

	if (vElemValue["maxActive"].isInt())
		setEMaxActive(vElemValue["maxActive"].asInt());

	if (vElemValue["military"].isInt())
		setEMilitary(vElemValue["military"].asInt());
}

////////////////
ClanApplyListLayer* ClanApplyListLayer::create(Json::Value vHomePage)
{
	ClanApplyListLayer* pRet = new ClanApplyListLayer();
	if (pRet&&pRet->init(vHomePage))
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

bool ClanApplyListLayer::init(Json::Value vHomePage)
{
	if (!Layer::init())
	{
		return false;
	}

	DSize size = ChessViewTool::getInstance()->getFullSize();
	m_iCurPage = 0;
	getElemData(vHomePage);
	UpdataTBShow(0, true);

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg1.png");
	bg->setPosition(Vec2(size.width/2, size.height/2));
	this->addChild(bg);

	Sprite* bg_info = ReadSpriteName("new_ui/gh/gh_til1.png");
	bg_info->setPosition(Vec2(bg->getContentSize().width/2,355));
	bg->addChild(bg_info);

	//55,318
	Sprite* infoSp = ReadSpriteName("new_ui/gh/gh_txt2.png");
	infoSp->setPosition(Vec2(bg->getContentSize().width/2 - 12,314));
	bg->addChild(infoSp);

 	mClanListTab = ClanListTableView::create(&mCurClanElemList, this);
 	bg->addChild(mClanListTab);

	//52,54
	auto spUserBG = ui::Scale9Sprite::create(getRespath("new_ui/gh/gh_box_bg3.png", 0, 1));
	m_mEditBox = ui::EditBox::create(spUserBG->getContentSize(), spUserBG);
	m_mEditBox->setPosition(Vec2(52, 54));
	m_mEditBox->setAnchorPoint(Vec2(0,0.5));
	m_mEditBox->setFontSize(18);
	m_mEditBox->setPlaceholderFontSize(18);
	m_mEditBox->setPlaceHolder(LL("srszidhmc"));
	m_mEditBox->setPlaceholderFontColor(Color3B(204, 204, 204));
	m_mEditBox->setFontColor(Color3B::WHITE);
	m_mEditBox->setMaxLength(CLAN_NAME_LENGTH);
	m_mEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_mEditBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_mEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_mEditBox->setDelegate(this);
	bg->addChild(m_mEditBox);

	//搜索
	Sprite* sousuoNSp = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* sousuoNTSP = ReadSpriteName("new_ui/img_text/gh_ban5.png");
	sousuoNTSP->setPosition(Vec2(sousuoNSp->getContentSize().width / 2, sousuoNSp->getContentSize().height / 2));
	sousuoNSp->addChild(sousuoNTSP);

	Sprite* sousuoSSp = ReadSpriteName("new_ui/img_global/global_ban4.png");
	Sprite* sousuoSTSP = ReadSpriteName("new_ui/img_text/gh_ban6.png");
	sousuoSTSP->setPosition(Vec2(sousuoSSp->getContentSize().width/2, sousuoSSp->getContentSize().height/2));
	sousuoSSp->addChild(sousuoSTSP);

	MenuItemSprite* btnSousuo = MenuItemSprite::create(sousuoNSp, sousuoSSp, CC_CALLBACK_1(ClanApplyListLayer::clickSouSuoMenu, this));

	Menu* menuSousuo = Menu::create();
	menuSousuo->setPosition(Vec2(284, 54));
	menuSousuo->addChild(btnSousuo);
	bg->addChild(menuSousuo);


	//创建氏族
	Sprite* cjszNSp = ReadSpriteName("new_ui/img_global/global_ban21.png");
	Sprite* cjszNTSP = ReadSpriteName("new_ui/img_text/gh_ban1.png");
	cjszNTSP->setPosition(Vec2(cjszNSp->getContentSize().width / 2, cjszNSp->getContentSize().height / 2));
	cjszNSp->addChild(cjszNTSP);

	Sprite* cjszSSp = ReadSpriteName("new_ui/img_global/global_ban22.png");
	Sprite* cjszSTSP = ReadSpriteName("new_ui/img_text/gh_ban2.png");
	cjszSTSP->setPosition(Vec2(cjszSSp->getContentSize().width / 2, cjszSSp->getContentSize().height / 2));
	cjszSSp->addChild(cjszSTSP);

	MenuItemSprite* btnCjsz = MenuItemSprite::create(cjszNSp, cjszSSp, CC_CALLBACK_1(ClanApplyListLayer::clickCJSZMenu, this));

	Menu* menuCjsz = Menu::create();
	menuCjsz->setPosition(Vec2(497, 54));
	menuCjsz->addChild(btnCjsz);
	bg->addChild(menuCjsz);


	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(584, 364));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClanApplyListLayer::clickCancleMenu, this));
	mCloseMenu->addChild(close_menu);


	//左右箭头
	Sprite* ProNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* ProSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	ProSSp->setScale(0.9f);
	int iSwitchWidth = ProNSp->getContentSize().width;

	proMenu = Menu::create();
	proMenu->setPosition(Vec2(28, bg->getContentSize().height / 2));
	bg->addChild(proMenu);

	MenuItemSprite* pro_mItem = MenuItemSprite::create(ProNSp, ProSSp, CC_CALLBACK_1(ClanApplyListLayer::clickProMenu, this));
	proMenu->addChild(pro_mItem);
	ProSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	ProSSp->setPosition(Vec2(ProSSp->getContentSize().width / 2, ProSSp->getContentSize().height / 2));

	Sprite* NextNSp = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* NextSSp = ReadSpriteName("new_ui/img_global/switch1.png");
	NextNSp->setFlippedX(true);
	NextSSp->setFlippedX(true);
	NextSSp->setScale(0.9f);

	nextMenu = Menu::create();
	nextMenu->setPosition(Vec2(bg->getContentSize().width - 28, bg->getContentSize().height / 2));
	bg->addChild(nextMenu);

	MenuItemSprite* next_mItem = MenuItemSprite::create(NextNSp, NextSSp, CC_CALLBACK_1(ClanApplyListLayer::clickNextMenu, this));
	nextMenu->addChild(next_mItem);
	NextSSp->setAnchorPoint(Vec2(0.5f, 0.5f));
	NextSSp->setPosition(Vec2(NextSSp->getContentSize().width / 2, NextSSp->getContentSize().height / 2));

	UpdataMenuState();

	return true;
}

void ClanApplyListLayer::editBoxReturn(ui::EditBox* editBox)
{

}

void ClanApplyListLayer::UpdataMenuState()
{
	if (m_iCurPage == 0)
		proMenu->setVisible(false);
	else
		proMenu->setVisible(true);

	if (m_iCurPage == m_iMaxPage - 1)
		nextMenu->setVisible(false);
	else
		nextMenu->setVisible(true);
}

void ClanApplyListLayer::UpdataTBShow(int iType,bool bIsInit)
{
	if (!bIsInit)
	{
		if (mClanListTab->m_iSelectDataIndex > -1 && mClanListTab->m_iSelectDataIndex < (int)mCurClanElemList.size())
			mCurClanElemList[mClanListTab->m_iSelectDataIndex]->setIsSelect(0);
	}

	int iClanIndex = 0;
	mCurClanElemList.clear();

	for (size_t i = m_iCurPage * PAGE_ELEM_NUM; i < mAllClanElemList.size()&& iClanIndex < 10; i++)
	{
		mCurClanElemList[iClanIndex++] = mAllClanElemList[i];
	}

	if(!bIsInit)
		mClanListTab->UpdataElemData(&mCurClanElemList, iType);
}

void ClanApplyListLayer::clickSouSuoMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SendSelectNetData();
}

void ClanApplyListLayer::clickCJSZMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(ApplyCreateClanLayer::create(this));
}

void ClanApplyListLayer::clickCancleMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void ClanApplyListLayer::clickProMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (m_iCurPage > 0)
	{
		m_iCurPage--;
		UpdataTBShow();
		UpdataMenuState();
	}
}

void ClanApplyListLayer::clickNextMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iAllDataPage = ceil(mAllClanElemList.size() / (PAGE_ELEM_NUM*1.0));

	if (m_iCurPage < m_iMaxPage - 1)
	{
		m_iCurPage++;

		if (m_iCurPage > iAllDataPage - 1)
		{
			SendPageNetData();
		}
		else
		{
			UpdataTBShow();
			UpdataMenuState();
		}
	}
}

void ClanApplyListLayer::getElemData(Json::Value vPageJson)
{
	map<int, int> mApplyList;
	Json::Value applyValue = vPageJson["apply"];
	if (applyValue.isArray())
	{
		for (UInt i = 0; i < applyValue.size(); i++)
		{
			mApplyList[applyValue[i].asInt()] = 1;
		}
	}

	Json::Value guildList = vPageJson["guildList"];
	if (guildList.isArray())
	{
		ClanListElem* ptrClanListElem = NULL;

		int iElemIndex = 0,iElemStartIndex = mAllClanElemList.size(),iClanId = 0;
		for (UInt i = 0; i < guildList.size(); i++)
		{
			iElemIndex = i + iElemStartIndex;
			iClanId = 0;

			if (guildList[i]["num"].isInt())
			{
				iClanId = guildList[i]["num"].asInt();
			}
			else
				continue;

			if (mAllClanIdIndex.count(iClanId))
			{
				iElemStartIndex--;
				ptrClanListElem = mAllClanElemList[mAllClanIdIndex[iClanId]];
			}
			else
			{
				mAllClanElemList[iElemIndex] = new ClanListElem();
				mAllClanElemList[iElemIndex]->setEIndex(iElemIndex);

				ptrClanListElem = mAllClanElemList[iElemIndex];

				mAllClanIdIndex[iClanId] = iElemIndex;
			}

			ptrClanListElem->LoadJson(guildList[i]);

			if (mApplyList.count(iClanId))
				ptrClanListElem->setIsApply(1);
		}
	}

	m_iMaxPage = 1;
	if (vPageJson["guildCount"].isInt())
	{
		m_iMaxPage = ceil(vPageJson["guildCount"].asInt() / (PAGE_ELEM_NUM*1.0));
	}
}

void ClanApplyListLayer::SendSelectNetData()
{
	//TODO
	string clanIdS = m_mEditBox->getText();

	if (clanIdS == "")
		return;

	int iClanId = StringToInt(clanIdS.c_str());

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("num=%d&gameToken=%s", iClanId, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"num\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}",iClanId, GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildByNum",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanApplyListLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"POST_SEND_SELECT_DATA");

	ITools::getInstance()->addWaitLayer();
}

void ClanApplyListLayer::SendPageNetData(bool bIsBack)
{

	string urlTag = "POST_SEND_PAGE_DATA";
	if(bIsBack)
		urlTag = "POST_SEND_BACK_PAGE_DATA";

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("curPage=%d&pageSize=%d&gameToken=%s", m_iCurPage+1,PAGE_ELEM_NUM, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"curPage\":%d,\"pageSize\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iCurPage + 1, PAGE_ELEM_NUM, GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildList",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanApplyListLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		urlTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void ClanApplyListLayer::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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

		if (strTag == "POST_SEND_SELECT_DATA")
		{
			if (clanValue.isObject())
			{
				int iClanId = 0;
				if (clanValue["num"].isInt())
					iClanId = clanValue["num"].asInt();
				else
					return;

				bool bIsDelete = false;
				ClanListElem* ptrClanElemList = NULL;
				if (mAllClanIdIndex.count(iClanId))
				{
					ptrClanElemList = mAllClanElemList[mAllClanIdIndex[iClanId]];
				}
				else
				{
					int iCnum = mAllClanElemList.size();
					ptrClanElemList = new ClanListElem();
					ptrClanElemList->setEIndex(iCnum);
					bIsDelete = true;
				}

				ptrClanElemList->LoadJson(clanValue);

				int iClanDataNum = ceil(mAllClanElemList.size() / (PAGE_ELEM_NUM*1.0));

				if (iClanDataNum >= m_iMaxPage&&bIsDelete)
					SendPageNetData(true);

				ChessApplication::getInstance()->AddFunctionLayer(ApplyAddClanLayer::create(ptrClanElemList, this));

				if (bIsDelete)
					delete ptrClanElemList;
			}
		}
		else if (strTag == "POST_SEND_PAGE_DATA"||strTag == "POST_SEND_BACK_PAGE_DATA")
		{
			if (value["result"]["guildList"].isArray())
			{
				getElemData(clanValue);

				if (strTag == "POST_SEND_PAGE_DATA")
				{
					UpdataTBShow(0);
					UpdataMenuState();
				}
				else
				{
					if (mClanListTab->cell_num < PAGE_ELEM_NUM && m_iCurPage == m_iMaxPage - 1)
					{
						UpdataTBShow(1);
						UpdataMenuState();
					}
				}
			}
		}
	}
}

void ClanApplyListLayer::updateRTDate(int iNomal)
{
	if (mAllClanIdIndex.count(iNomal))
	{
		mAllClanElemList[mAllClanIdIndex[iNomal]]->setIsApply(1);

		mClanListTab->updateRTDate(iNomal);
	}
}

void ClanApplyListLayer::ChildCloseLayer(int iNomal)
{
	this->removeFromParent();
}

ClanApplyListLayer::ClanApplyListLayer()
{

}

ClanApplyListLayer::~ClanApplyListLayer()
{
	for (size_t i = 0; i < mAllClanElemList.size(); i++)
	{
		delete mAllClanElemList[i];
	}
}

//////////////

ApplyAddClanLayer* ApplyAddClanLayer::create(ClanListElem* ptrCLElem, EventDelegate* ptrDelegate)
{
	ApplyAddClanLayer* ptrRet = new ApplyAddClanLayer();
	if (ptrRet&&ptrRet->init(ptrCLElem, ptrDelegate))
	{
		ptrRet->autorelease();
		return ptrRet;
	}
	else
	{
		delete ptrRet;
		ptrRet = NULL;
		return NULL;
	}
}

bool ApplyAddClanLayer::init(ClanListElem* ptrCLElem, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;
	m_iClanId = ptrCLElem->getClanId();
	m_ClanNameS = ptrCLElem->getClanName();

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg2.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	Sprite* bgInfo = ReadSpriteName("new_ui/gh/gh_til10.png");
	bgInfo->setPosition(Vec2(bg->getContentSize().width / 2, 286+8));
	bg->addChild(bgInfo);

	//103,219
	Label* nameLab = LabelSystemFont(ptrCLElem->getClanName().c_str(),16);
	nameLab->setAnchorPoint(Vec2(0,0.5));
	nameLab->setPosition(Vec2(117,226));
	bg->addChild(nameLab);

	//207,226
	Label* lvLab = LabelSystemFont(__String::createWithFormat("%d%s",ptrCLElem->getClanLv(),LL("ji"))->getCString(), 16);
	lvLab->setAnchorPoint(Vec2(0,0.5));
	lvLab->setPosition(Vec2(207,226));
	bg->addChild(lvLab);

	Label* ZongZhiLab = LabelSystemFont(ptrCLElem->getZongZhi().c_str(),16);
	ZongZhiLab->setDimensions(278, 0);
	ZongZhiLab->setPosition(Vec2(117,206));
	ZongZhiLab->setAnchorPoint(Vec2(0,1));
	bg->addChild(ZongZhiLab);

	Menu* menuM = Menu::create();
	menuM->setPosition(Vec2(bg->getContentSize().width/2, 48));
	bg->addChild(menuM);
	//取消
	Sprite* cancleNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* cancleNTSP = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	cancleNTSP->setPosition(Vec2(cancleNSp->getContentSize().width / 2, cancleNSp->getContentSize().height / 2));
	cancleNSp->addChild(cancleNTSP);

	Sprite* cancleSSp = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* cancleSTSP = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	cancleSTSP->setPosition(Vec2(cancleSSp->getContentSize().width / 2, cancleSSp->getContentSize().height / 2));
	cancleSSp->addChild(cancleSTSP);

	MenuItemSprite* btncancle = MenuItemSprite::create(cancleNSp, cancleSSp, CC_CALLBACK_1(ApplyAddClanLayer::clickCancleMenu, this));
	btncancle->setPosition(Vec2(-90,0));
	menuM->addChild(btncancle);

	//申请
	Sprite* applyNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* applyNTSP = ReadSpriteName("new_ui/img_text/gh_ban3.png");
	applyNTSP->setPosition(Vec2(applyNSp->getContentSize().width / 2, applyNSp->getContentSize().height / 2));
	applyNSp->addChild(applyNTSP);

	Sprite* applySSp = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* applySTSP = ReadSpriteName("new_ui/img_text/gh_ban4.png");
	applySTSP->setPosition(Vec2(applySSp->getContentSize().width / 2, applySSp->getContentSize().height / 2));
	applySSp->addChild(applySTSP);

	MenuItemSprite* btnapply = MenuItemSprite::create(applyNSp, applySSp, CC_CALLBACK_1(ApplyAddClanLayer::clickSureMenu, this));
	btnapply->setPosition(Vec2(90, 0));
	menuM->addChild(btnapply);

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(430, 276));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ApplyAddClanLayer::clickCancleMenu, this));
	mCloseMenu->addChild(close_menu);

	

	return true;
}

void ApplyAddClanLayer::clickCancleMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void ApplyAddClanLayer::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	SendNetData();
}

void ApplyAddClanLayer::SendNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("num=%d&gameToken=%s", m_iClanId, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"num\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iClanId, GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("applyGuild",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ApplyAddClanLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ApplyAddClanLayer::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
		if (value["result"]["isTrue"].isInt()&& value["result"]["isTrue"].asInt())
		{
			if (mDelegate)
				mDelegate->updateRTDate(m_iClanId);

			ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("yxszsfcsq"), m_ClanNameS.c_str())->getCString(),true);

			this->removeFromParent();
		}
	}
}

ApplyAddClanLayer::ApplyAddClanLayer()
{

}

ApplyAddClanLayer::~ApplyAddClanLayer()
{

}


////////////////

ApplyCreateClanLayer* ApplyCreateClanLayer::create(EventDelegate* ptrDelegate, int iType, string clanNameS, string guildID)
{
	ApplyCreateClanLayer* pRet = new ApplyCreateClanLayer();
	if (pRet&&pRet->init(ptrDelegate,iType,clanNameS, guildID))
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

bool ApplyCreateClanLayer::init(EventDelegate* ptrDelegate, int iType, string clanNameS, string guildID)
{
	if (!Layer::init())
	{
		return false;
	}

	mDelegate = ptrDelegate;
	m_iType = iType;
	mGuildID = guildID;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0, 0, 0, 100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg2.png");
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	this->addChild(bg);

	if (iType == TYPE_CREATE_CLAN)
	{
		Sprite* bgInfo = ReadSpriteName("new_ui/gh/gh_til11.png");
		bgInfo->setPosition(Vec2(bg->getContentSize().width / 2, 286));
		bg->addChild(bgInfo);

		auto spCreateBG = ui::Scale9Sprite::create(getRespath("new_ui/gh/gh_box_bg3.png", 0, 1));
		m_mNameEditBox = ui::EditBox::create(spCreateBG->getContentSize(), spCreateBG);
		m_mNameEditBox->setPosition(Vec2(117, 226));
		m_mNameEditBox->setAnchorPoint(Vec2(0, 0.5));
		m_mNameEditBox->setFontSize(18);
		m_mNameEditBox->setPlaceholderFontSize(18);
		m_mNameEditBox->setPlaceHolder(LL("djsrszmc"));
		m_mNameEditBox->setPlaceholderFontColor(Color3B(204, 204, 204));
		m_mNameEditBox->setFontColor(Color3B::WHITE);
		m_mNameEditBox->setMaxLength(8);
		m_mNameEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_mNameEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
		m_mNameEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
		m_mNameEditBox->setDelegate(this);
		m_mNameEditBox->setTag(1);
		bg->addChild(m_mNameEditBox);
	}
	else
	{
		Sprite* bgInfo = ReadSpriteName("new_ui/gh/gh_til13.png");
		bgInfo->setPosition(Vec2(bg->getContentSize().width / 2, 286));
		bg->addChild(bgInfo);

		m_mNameEditBox = NULL;

		Label* clanNameLab = LabelSystemFont(clanNameS.c_str(),18);
		clanNameLab->setPosition(Vec2(117, 226));
		clanNameLab->setAnchorPoint(Vec2(0, 0.5));
		bg->addChild(clanNameLab);
	}

	Sprite* LabBgSp = ReadSpriteName("new_ui/gh/gh_box_bg2.png");
	LabBgSp->setPosition(Vec2(117, 206));
	LabBgSp->setAnchorPoint(Vec2(0, 1));
	bg->addChild(LabBgSp);

	ZongzhiTitle = Label::createWithSystemFont(LL("djsrszzz"),"Arial",16);
	ZongzhiTitle->setPosition(Vec2(119, 204));
	ZongzhiTitle->setAnchorPoint(Vec2(0,1));
	ZongzhiTitle->setColor(Color3B(204, 204, 204));
	bg->addChild(ZongzhiTitle);

	auto spZongzhiBG = ui::Scale9Sprite::create(getRespath("new_ui/gh/gh_box_bg2.png", 0, 1));
	spZongzhiBG->setOpacity(0);
	m_mZZEditBox = ui::EditBox::create(spZongzhiBG->getContentSize(), spZongzhiBG);
	m_mZZEditBox->setPosition(Vec2(119, 204));
	m_mZZEditBox->setAnchorPoint(Vec2(0, 1));
	m_mZZEditBox->setFontSize(16);
	m_mZZEditBox->setFontColor(Color4B(255,255,255,0));
	m_mZZEditBox->setMaxLength(ZONGZHI_LENGTH);
	m_mZZEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_mZZEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_mZZEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	m_mZZEditBox->setDelegate(this);
	m_mZZEditBox->setTag(2);
	bg->addChild(m_mZZEditBox);

	ZongzhiLab = Label::createWithSystemFont("", "Arial",16);
	ZongzhiLab->setAnchorPoint(Vec2(0, 1));
	ZongzhiLab->setPosition(Vec2(121, 204));
	ZongzhiLab->setDimensions(290, 90);
	ZongzhiLab->setHorizontalAlignment(TextHAlignment::LEFT);
	ZongzhiLab->setMaxLineWidth(290);
	bg->addChild(ZongzhiLab);


	if (iType == TYPE_CREATE_CLAN)
	{
		Sprite* jySp = ReadSpriteName("new_ui/img_global/hero_bg4.png");
		jySp->setPosition(Vec2(bg->getContentSize().width / 2, 105));
		bg->addChild(jySp);

		Sprite* gySp = ReadSpriteName("new_ui/img_global/icon_gy.png");
		gySp->setAnchorPoint(Vec2(0, 0.5));
		gySp->setPosition(Vec2(4, jySp->getContentSize().height / 2));
		jySp->addChild(gySp);

		Label* gyNumSp = LabelSystemFont("2000", 16);
		gyNumSp->setColor(Color3B::GREEN);
		gyNumSp->setPosition(Vec2(jySp->getContentSize().width / 2, jySp->getContentSize().height / 2));
		jySp->addChild(gyNumSp);
	}

	Menu* menuM = Menu::create();
	menuM->setPosition(Vec2(bg->getContentSize().width / 2, 48));
	bg->addChild(menuM);
	//取消
	Sprite* cancleNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* cancleNTSP = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	cancleNTSP->setPosition(Vec2(cancleNSp->getContentSize().width / 2, cancleNSp->getContentSize().height / 2));
	cancleNSp->addChild(cancleNTSP);

	Sprite* cancleSSp = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* cancleSTSP = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	cancleSTSP->setPosition(Vec2(cancleSSp->getContentSize().width / 2, cancleSSp->getContentSize().height / 2));
	cancleSSp->addChild(cancleSTSP);

	MenuItemSprite* btncancle = MenuItemSprite::create(cancleNSp, cancleSSp, CC_CALLBACK_1(ApplyCreateClanLayer::clickCancleMenu, this));
	btncancle->setPosition(Vec2(-90, 0));
	menuM->addChild(btncancle);

	//确定
	Sprite* applyNSp = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* applyNTSP = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	applyNTSP->setPosition(Vec2(applyNSp->getContentSize().width / 2, applyNSp->getContentSize().height / 2));
	applyNSp->addChild(applyNTSP);

	Sprite* applySSp = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* applySTSP = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	applySTSP->setPosition(Vec2(applySSp->getContentSize().width / 2, applySSp->getContentSize().height / 2));
	applySSp->addChild(applySTSP);

	MenuItemSprite* btnapply = MenuItemSprite::create(applyNSp, applySSp, CC_CALLBACK_1(ApplyCreateClanLayer::clickSureMenu, this));
	btnapply->setPosition(Vec2(90, 0));
	menuM->addChild(btnapply);

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(430, 276));
	bg->addChild(mCloseMenu);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ApplyCreateClanLayer::clickCancleMenu, this));
	mCloseMenu->addChild(close_menu);


	return true;
}

void ApplyCreateClanLayer::editBoxReturn(ui::EditBox* editBox)
{
	if (editBox->getTag() == 2)
	{
		string txtS = editBox->getText();
		if ("" == txtS)
		{
			m_mZZEditBox->setOpacity(255);
			ZongzhiTitle->setVisible(true);
		}
		else
		{
			m_mZZEditBox->setOpacity(0);
			ZongzhiTitle->setVisible(false);
		}
		ZongzhiLab->setString(txtS.c_str());
	}
}

void ApplyCreateClanLayer::editBoxTextChanged(ui::EditBox* editBox, const std::string& txtS)
{
	if (editBox->getTag() == 2)
	{
		if ("" == txtS)
		{
			m_mZZEditBox->setOpacity(255);
			ZongzhiTitle->setVisible(true);
		}
		else
		{
			m_mZZEditBox->setOpacity(0);
			ZongzhiTitle->setVisible(false);
		}
		ZongzhiLab->setString(txtS.c_str());
	}
}

void ApplyCreateClanLayer::clickCancleMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void ApplyCreateClanLayer::clickSureMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iType == TYPE_CREATE_CLAN)
		SendNetData();
	else
	{
		SendSetZongZhiData();
	}
}

ApplyCreateClanLayer::ApplyCreateClanLayer()
{

}

ApplyCreateClanLayer::~ApplyCreateClanLayer()
{

}

void ApplyCreateClanLayer::SendNetData()
{
	//TODO
	string guildNameS = m_mNameEditBox->getText();
	string introduceS = m_mZZEditBox->getText();

	if (guildNameS == "")
	{
		ITools::getInstance()->createGlobalPrompt(LL("mingchengbnk"), false);
		return;
	}

	if (guildNameS.length() < 2)
	{
		ITools::getInstance()->createGlobalPrompt(LL("mingchengcuowu"), false);
		return;
	}
	else if (guildNameS.length() > 15)
	{
		ITools::getInstance()->createGlobalPrompt(LL("mingchengguochang"), false);
		return;
	}

	DString guildNameDS = guildNameS.c_str();
	DString introduceDS = introduceS.c_str();
	if (guildNameDS->indexOf(' ') != -1 || introduceDS->indexOf(' ') != -1)
	{
		ITools::getInstance()->createGlobalPrompt(LL("feifazifu"), false);
		return;
	}

	for (UInt i = 0; i < trData->m_mNameForbid.size(); i++)
	{
		if (guildNameDS->indexOf(trData->m_mNameForbid[i].c_str()) != -1||
			(introduceS != ""&&introduceDS->indexOf(trData->m_mNameForbid[i].c_str()) != -1))
		{
			ITools::getInstance()->createGlobalPrompt(LL("feifazifu"), false);
			return;
		}
	}

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildName=%s&introduce=%s&gameToken=%s", guildNameS.c_str(), introduceS.c_str(), GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildName\":\"%s\",\"introduce\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", guildNameS.c_str(), introduceS.c_str(), GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("createGuild",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ApplyCreateClanLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendCreateData");

	ITools::getInstance()->addWaitLayer();
}

void ApplyCreateClanLayer::SendSetZongZhiData()
{
	string introduceS = m_mZZEditBox->getText();

	if (introduceS == "")
	{
		ITools::getInstance()->createGlobalPrompt(LL("zongzhibnk"), false);
		return;
	}

	DString introduceDS = introduceS.c_str();
	if (introduceDS->indexOf(' ') != -1)
	{
		ITools::getInstance()->createGlobalPrompt(LL("feifazifu"), false);
		return;
	}

	for (UInt i = 0; i < trData->m_mNameForbid.size(); i++)
	{
		if (introduceDS->indexOf(trData->m_mNameForbid[i].c_str()) != -1)
		{
			ITools::getInstance()->createGlobalPrompt(LL("feifazifu"), false);
			return;
		}
	}


	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("guildID=%s&introduce=%s&gameToken=%s", mGuildID.c_str(), introduceS.c_str(), GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"guildID\":\"%s\",\"introduce\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", mGuildID.c_str(), introduceS.c_str(), GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("setGuildNotice",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ApplyCreateClanLayer::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SendSetZzData");

	ITools::getInstance()->addWaitLayer();

}

void ApplyCreateClanLayer::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
		if (strTag == "SendCreateData")
		{
			if (value["result"].isObject() && value["result"]["num"].isInt()&& value["result"]["num"].asInt())
			{
				if (value["result"]["jade"].isInt())
					history->m_iJade = value["result"]["jade"].asInt();

				if (history->m_layTitleBar)
				{
					history->m_layTitleBar->updateShow(0);
				}

				if (mDelegate)
					mDelegate->ChildCloseLayer(0);
				GetNetDateTool::getInstance()->SendClanInfoData();

				this->removeFromParent();
			}
		}
		else if (strTag == "SendSetZzData")
		{
			if (value["result"].isObject()&& value["result"]["isTrue"].isInt() && value["result"]["isTrue"].asInt())
			{
				if (mDelegate)
					mDelegate->updateRTDate(m_mZZEditBox->getText());

				ITools::getInstance()->createGlobalPrompt(LL("xiugaichenggong"), true);

				this->removeFromParent();
			}
		}

	}
}