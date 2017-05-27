#include "ClanActiveTaskTableView.h"
#include "Defines.h"
#include "ArmorListLayer.h"
#include "EquipmentUp.h"
#include "PubDrinkLayer.h"
#include "GoodsShopLayer.h"
#include "GoldExchangeLayer.h"
#include "MergeLayer.h"
#include "BackpackLayer.h"
#include "DestroySixCountriesLayer.h"
#include "HeroicAmbitionLayer.h"

#define TASK_CELL_WIDTH 540
#define TASK_CELL_HEIGHT 96

void ClanActiveTaskElem::LoadJsonValue(Json::Value vElemJson,int EiIndex)
{
	ParseInt(iId, vElemJson["id"]);
	ParseInt(iType, vElemJson["type"]);
	ParseString(sTypeName, vElemJson["typeName"]);
	ParseInt(iWarid, vElemJson["warid"]);
	ParseInt(iNum, vElemJson["num"]);
	ParseInt(iActive, vElemJson["active"]);
	ParseInt(iStatus, vElemJson["status"]);
	ParseInt(iFinishNum, vElemJson["finishNum"]);
	iIndex = EiIndex;

	Json::Value vGoods = vElemJson["goods"];
	if (vGoods.isArray())
	{
		for (UInt i = 0; i < vGoods.size(); i++)
		{
			if (vGoods[i]["goodID"].isInt() && vGoods[i]["count"].isInt())
			{
				mGoods[i][0] = vGoods[i]["goodID"].asInt();
				mGoods[i][1] = vGoods[i]["count"].asInt();
			}
		}
	}
}

////////////////

ClanActiveTaskTableView* ClanActiveTaskTableView::create(Json::Value vLogData, EventDelegate* ptrDelegate)
{
	ClanActiveTaskTableView *pRet = new ClanActiveTaskTableView();
	if (pRet && pRet->init(vLogData,ptrDelegate))
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

bool ClanActiveTaskTableView::init(Json::Value vLogData, EventDelegate* ptrDelegate)
{
    if ( !Layer::init() )
    {
        return false;
    }
	getActiveData(vLogData);
	this->cell_num = mActiveData.size();
	mDelegate = ptrDelegate;
	bEnableUpdataFatherShow = false;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg6.png");
	bg->setPosition(Vec2(size.width/2,size.height/2 - 10));
	this->addChild(bg);

	Sprite* bgInfo = ReadSpriteName("new_ui/gh/gh_tibox1.png");
	bgInfo->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height - 5));
	bg->addChild(bgInfo);

	Label* infoLab = LabelSystemFont(LL("mtkwcsghyrw"),16);
	infoLab->setPosition(Vec2(bg->getContentSize().width/2,25));
	infoLab->enableOutline(Color4B(158, 116, 48, 255), 1);
	bg->addChild(infoLab);

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(TASK_CELL_WIDTH,3* TASK_CELL_HEIGHT));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(30,36));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    bg->addChild(tableView);
    //加载tableview
    //tableView->reloadData();

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(580, 358));
	bg->addChild(mCloseMenu, 2);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClanActiveTaskTableView::clickCloseMenu, this));
	mCloseMenu->addChild(close_menu);
    
    return true;
}

void ClanActiveTaskTableView::getActiveData(Json::Value vLogData)
{
	int iIsVip = history->m_pPlayerInfo->getVIP();
	Json::Value vTasksJson = vLogData["tasks"];
	for (UInt i = 0; i < vTasksJson.size(); i++)
	{
		mActiveData[i] = new ClanActiveTaskElem();
		mActiveData[i]->LoadJsonValue(vTasksJson[i],i);
		if (iIsVip)
			mActiveData[i]->setEiActive(mActiveData[i]->getEiActive() * 6 / 5);
	}
}

void ClanActiveTaskTableView::updateRTDate(int iElemIndex,int iType)
{
	m_iLingQuIndex = iElemIndex;
	if (iType == 1) //领取
	{
		SendLingQuNetData();
	}
	else if(iType == 2) //前往
	{
		ClanTaskJump(iElemIndex);
		//ITools::getInstance()->createGlobalPrompt("YOU SHANG JIAO , BU SONG!",false);
	}
}

void ClanActiveTaskTableView::ClanTaskJump(int iIndex)
{
	int iType = mActiveData[iIndex]->getEiType();
	int iWarId = mActiveData[iIndex]->getEiWarid();

	bool bEnableRemove = true;

	switch (iType)
	{
	case 1://武将列表
		ChessApplication::getInstance()->AddFunctionLayer(ArmorListLayer::create());
		break;
	case 2://锻造
	case 3:
		if (history->GetAllTreasure(MODEL_MAX).size())
		{
			ChessApplication::getInstance()->AddFunctionLayer(EquipmentUp::create());
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("mybwkyqh"), false);
			bEnableRemove = false;
		}
		break;
	case 4://酒馆
	case 5:
		if (history->ownArmyNum > 3)
		{
			ChessApplication::getInstance()->AddFunctionLayer(PubDrinkLayer::create());
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("pubdrinkopen"), false);
			bEnableRemove = false;
		}
		break;
	case 6://商店
		ChessApplication::getInstance()->AddFunctionLayer(GoodsShopLayer::create());
		break;
	case 7://兑换金币
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(history->m_layTitleBar));
		break;
	case 8://合成
		ChessApplication::getInstance()->AddFunctionLayer(MergeLayer::create());
		break;
	case 9://背包
		ChessApplication::getInstance()->AddFunctionLayer(BackpackLayer::create());
		break;
	case 99://战天下或者演义堂
	{
		if (trData->m_mNetWarInfo.count(iWarId))
		{
			int iBattleType = trData->m_mNetWarInfo[iWarId]->iType;

			if (iBattleType == BATTLE_SIXWAR)
			{
				ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create(iWarId, trData->m_mNetWarInfo[iWarId]->iModel));
				if (history->m_layHome)
					history->m_layHome->removeFromParent();
			}
			else if (iBattleType == BATTLE_HEROIC)
			{
				ChessApplication::getInstance()->AddFunctionLayer(HeroicAmbitionLayer::create(iWarId));
				if (history->m_layHome)
					history->m_layHome->removeFromParent();
			}
		}
	}
		break;
	default:
		bEnableRemove = false;
		break;
	}

	if (mDelegate&&bEnableRemove)
	{
		mDelegate->ChildCloseLayer(0);
		this->removeFromParent();
	}
}

void ClanActiveTaskTableView::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (bEnableUpdataFatherShow&&mDelegate)
		mDelegate->updateShow(0);

	this->removeFromParent();
}

ClanActiveTaskTableView::ClanActiveTaskTableView()
{
	cell_num = 0;
}

ClanActiveTaskTableView::~ClanActiveTaskTableView()
{
	for (size_t i = 0; i < mActiveData.size(); i++)
	{
		delete mActiveData[i];
	}
}

void ClanActiveTaskTableView::SendLingQuNetData()
{
	int iTaskId = mActiveData[m_iLingQuIndex]->getEiId();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("taskID=%d&gameToken=%s", iTaskId ,GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"taskID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iTaskId ,GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getActiveTaskGood",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ClanActiveTaskTableView::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ClanActiveTaskTableView::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
		if (value["result"]["jade"].isInt() && value["result"]["gold"].isInt() && value["result"]["improvedStone"].isInt())
		{
			mActiveData[m_iLingQuIndex]->setEiStatus(2);
			mPromptTxt.clear();
			map<int,string> tempPromptTxt = history->onHttpGoodsAdd(value["result"]);
			mPromptTxtIndex = 0;

			if (mActiveData[m_iLingQuIndex]->getEiActive() > 0)
			{
				string huoyueS = LL("huoyue");
				huoyueS.erase(huoyueS.end() - 1);
				mPromptTxt[0] = StringUtils::format(LL("huodessdd"),huoyueS.c_str(),mActiveData[m_iLingQuIndex]->getEiActive());

				for (UInt i = 0; i < tempPromptTxt.size(); i++)
				{
					mPromptTxt[i + 1] = tempPromptTxt[i];
				}
			}
			else
			{
				mPromptTxt = tempPromptTxt;
			}

			this->schedule(schedule_selector(ClanActiveTaskTableView::showPrompt), 0.8f, mPromptTxt.size() - 1, 0);
			ClanActiveTaskCell* ptrClanActiveTaskCell = (ClanActiveTaskCell*)tableView->cellAtIndex(m_iLingQuIndex);
			if (ptrClanActiveTaskCell)
				ptrClanActiveTaskCell->UpDateCell(mActiveData[m_iLingQuIndex]);

			bEnableUpdataFatherShow = true;
		}
	}
}

void ClanActiveTaskTableView::showPrompt(float f)
{
	ITools::getInstance()->createGlobalPrompt(mPromptTxt[mPromptTxtIndex++], true);
}

void ClanActiveTaskTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ClanActiveTaskTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ClanActiveTaskTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ClanActiveTaskTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClanActiveTaskTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClanActiveTaskTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(TASK_CELL_WIDTH, TASK_CELL_HEIGHT);
}

TableViewCell* ClanActiveTaskTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ClanActiveTaskCell::create(mActiveData[idx],this);
    }
    else
    {
		ClanActiveTaskCell* c = (ClanActiveTaskCell*)cell;
		c->UpDateCell(mActiveData[idx]);
    }
    return cell;
}

ssize_t ClanActiveTaskTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ClanActiveTaskCell* ClanActiveTaskCell::create(ClanActiveTaskElem* ptrATE, EventDelegate* ptrDelegate)
{
	ClanActiveTaskCell* pRet = new ClanActiveTaskCell();
	if (pRet&&pRet->init(ptrATE,ptrDelegate))
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

bool ClanActiveTaskCell::init(ClanActiveTaskElem* ptrATE, EventDelegate* ptrDelegate)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	mClanActiveTaskElem = ptrATE;
	mDelegate = ptrDelegate;
	NetActiveTask* ptrNetActiveTask = trData->net_active_task[ptrATE->getEiId()];

	Sprite* bg = ReadSpriteName("new_ui/gh/gh_bg7.png");
	bg->setPosition(Vec2(TASK_CELL_WIDTH/2, TASK_CELL_HEIGHT/2));
	this->addChild(bg);

	float fBgHeight = bg->getContentSize().height;

	int iStartX = 48;
	Sprite* imgBg = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBg->setPosition(Vec2(iStartX,fBgHeight/2));
	bg->addChild(imgBg);

	mImg = ReadSpriteName(__String::createWithFormat("new_ui/gh/gh_hyrw%d.png", ptrNetActiveTask->img)->getCString());
	mImg->setPosition(Vec2(imgBg->getContentSize().width/2, imgBg->getContentSize().height/2));
	imgBg->addChild(mImg);

	iStartX += imgBg->getContentSize().width / 2 + 4;

	mActiveName = LabelSystemFont(__String::createWithFormat("%s  (%d/%d)", ptrNetActiveTask->nameS.c_str(), ptrATE->getEiFinishNum(), ptrNetActiveTask->condition)->getCString(),18);
	mActiveName->setPosition(Vec2(iStartX, fBgHeight / 2 + 20));
	mActiveName->setAnchorPoint(Vec2(0, 0.5));
	mActiveName->setColor(Color3B(102, 0, 0));
	bg->addChild(mActiveName);
	
	mGoodsLab = LabelSystemFont(GetGoodsString().c_str(),16);
	mGoodsLab->setPosition(Vec2(iStartX, fBgHeight/2));
	mGoodsLab->setAnchorPoint(Vec2(0,0.5));
	mGoodsLab->setColor(Color3B::YELLOW);
	bg->addChild(mGoodsLab);

	mActiveInfo = LabelSystemFont(ptrNetActiveTask->explain.c_str(),16);
	mActiveInfo->setPosition(Vec2(iStartX, fBgHeight / 2 - 20));
	mActiveInfo->setAnchorPoint(Vec2(0, 0.5));
	mActiveInfo->setColor(Color3B(102, 51, 0));
	bg->addChild(mActiveInfo);

	mYilingquSp = ReadSpriteName("new_ui/battle/battle_txt1.png");
	mYilingquSp->setPosition(Vec2(bg->getContentSize().width - 70,fBgHeight / 2));
	bg->addChild(mYilingquSp);

// 	mWeiWanCheng = ReadSpriteName("new_ui/battle/battle_txt2.png");
// 	mWeiWanCheng->setPosition(mYilingquSp->getPosition());
// 	bg->addChild(mWeiWanCheng);

	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(mYilingquSp->getPosition());
	bg->addChild(ptrMenu);

	Sprite* menuNSp = ReadSpriteName("new_ui/img_global/global_ban3.png");
	Sprite* menuSSp = ReadSpriteName("new_ui/img_global/global_ban4.png");

	Sprite* menuNTSp = ReadSpriteName("new_ui/img_text/mail_txt1.png");
	Sprite* menuSTSp = ReadSpriteName("new_ui/img_text/mail_txt1.png");

	menuNTSp->setPosition(Vec2(menuNSp->getContentSize().width / 2, menuNSp->getContentSize().height / 2));
	menuSTSp->setPosition(Vec2(menuSSp->getContentSize().width / 2, menuSSp->getContentSize().height / 2));
	menuNSp->addChild(menuNTSp);
	menuSSp->addChild(menuSTSp);

	mLingQuMI = MenuItemSprite::create(menuNSp, menuSSp,CC_CALLBACK_1(ClanActiveTaskCell::clickLingQuMenu,this));
	ptrMenu->addChild(mLingQuMI);

	Sprite* menuNSp2 = ReadSpriteName("new_ui/img_global/global_ban23.png");
	Sprite* menuSSp2 = ReadSpriteName("new_ui/img_global/global_ban24.png");

	Sprite* menuNTSp2 = ReadSpriteName("new_ui/img_text/gh_bant9.png");
	Sprite* menuSTSp2 = ReadSpriteName("new_ui/img_text/gh_bant10.png");

	menuNTSp2->setPosition(Vec2(menuNSp2->getContentSize().width / 2, menuNSp2->getContentSize().height / 2));
	menuSTSp2->setPosition(Vec2(menuSSp2->getContentSize().width / 2, menuSSp2->getContentSize().height / 2));
	menuNSp2->addChild(menuNTSp2);
	menuSSp2->addChild(menuSTSp2);

	mQianWangMI = MenuItemSprite::create(menuNSp2, menuSSp2, CC_CALLBACK_1(ClanActiveTaskCell::clickQianWangMenu, this));
	ptrMenu->addChild(mQianWangMI);

	if (ptrATE->getEiStatus() == 0)
	{
		mYilingquSp->setVisible(false);
		mLingQuMI->setVisible(false);
	}
	else if (ptrATE->getEiStatus() == 1)
	{
		mYilingquSp->setVisible(false);
		mQianWangMI->setVisible(false);
	}
	else if(ptrATE->getEiStatus() == 2)
	{
		mQianWangMI->setVisible(false);
		mLingQuMI->setVisible(false);
	}
	else
	{
		mQianWangMI->setVisible(false);
		mLingQuMI->setVisible(false);
		mYilingquSp->setVisible(false);
	}

	return true;
}

void ClanActiveTaskCell::clickLingQuMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mDelegate)
		mDelegate->updateRTDate(mClanActiveTaskElem->getEiIndex(),1);
}

void ClanActiveTaskCell::clickQianWangMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (mDelegate)
		mDelegate->updateRTDate(mClanActiveTaskElem->getEiIndex(),2);
}

string ClanActiveTaskCell::GetGoodsString()
{
	string sRet = "";

	if (mClanActiveTaskElem->getEiActive() > 0)
	{
		string huoyueS = LL("huoyue");
		huoyueS.erase(huoyueS.end()-1);
		sRet = StringUtils::format("%sx%d",huoyueS.c_str(), mClanActiveTaskElem->getEiActive());
	}

	for (size_t i = 0;i < mClanActiveTaskElem->mGoods.size(); i++)
	{
		if (sRet != "")
		{
			sRet += LL("dunhao");
		}
		sRet += StringUtils::format("%sx%d", trData->m_mGoodsItem[mClanActiveTaskElem->mGoods[i][0]]->strName.c_str(), mClanActiveTaskElem->mGoods[i][1]);
	}
	return sRet;
}

ClanActiveTaskCell::ClanActiveTaskCell()
{

}

void ClanActiveTaskCell::UpDateCell(ClanActiveTaskElem* ptrATE)
{
	mClanActiveTaskElem = ptrATE;

	NetActiveTask* ptrNetActiveTask = trData->net_active_task[ptrATE->getEiId()];

	mImg->setTexture(ReadSpriteTexture(__String::createWithFormat("new_ui/gh/gh_hyrw%d.png", ptrNetActiveTask->img)->getCString()));

	mActiveName->setString(__String::createWithFormat("%s  (%d/%d)", ptrNetActiveTask->nameS.c_str(), ptrATE->getEiFinishNum(), ptrNetActiveTask->condition)->getCString());

	mGoodsLab->setString(GetGoodsString().c_str());

	mActiveInfo->setString(ptrNetActiveTask->explain.c_str());

	if (ptrATE->getEiStatus() == 0)
	{
		mYilingquSp->setVisible(false);
		mLingQuMI->setVisible(false);
		mQianWangMI->setVisible(true);
	}
	else if (ptrATE->getEiStatus() == 1)
	{
		mYilingquSp->setVisible(false);
		mQianWangMI->setVisible(false);
		mLingQuMI->setVisible(true);
	}
	else if (ptrATE->getEiStatus() == 2)
	{
		mQianWangMI->setVisible(false);
		mLingQuMI->setVisible(false);
		mYilingquSp->setVisible(true);
	}
	else
	{
		mQianWangMI->setVisible(false);
		mLingQuMI->setVisible(false);
		mYilingquSp->setVisible(false);
	}
}