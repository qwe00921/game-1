#include "TreasureBoxTableView.h"
#include "Defines.h"
#include "GoodsSprite.h"

TreasureBoxTableView* TreasureBoxTableView::create(int iMapId, EventDelegate* ptrDelegate,int iBattleType)
{
	TreasureBoxTableView *pRet = new TreasureBoxTableView();
	if (pRet && pRet->init(iMapId,ptrDelegate,iBattleType))
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

bool TreasureBoxTableView::init(int iMapId, EventDelegate* ptrDelegate, int iBattleType)
{
    if ( !Layer::init() )
    {
        return false;
    }
	mDelegate = ptrDelegate;
	mMapId = iMapId;
	mBattleType = iBattleType;
	GetElemList();
	mCellNum = mDataElemList.size();

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(410,104*3-10));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(54,31-1));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
    this->addChild(tableView);
    //加载tableview
    //tableView->reloadData();

    return true;
}

void TreasureBoxTableView::GetElemList()
{
	int iElemListIndex = 0;
	for (map<int, NetMapChestPtr>::iterator it = trData->net_map_chest[mMapId].begin(); it != trData->net_map_chest[mMapId].end(); it++)
	{
		mDataElemList[iElemListIndex++] = it->second;
	}
}

TreasureBoxTableView::TreasureBoxTableView():BTLayer(false)
{

}

TreasureBoxTableView::~TreasureBoxTableView()
{

}

void TreasureBoxTableView::scrollViewDidScroll(ScrollView* view)
{

}

void TreasureBoxTableView::scrollViewDidZoom(ScrollView* view)
{

}

void TreasureBoxTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void TreasureBoxTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void TreasureBoxTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size TreasureBoxTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(410, 104);
}

TableViewCell* TreasureBoxTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
		cell = TreasureBoxECell::create(mDataElemList[idx],mDelegate,mBattleType);
    }
    else
    {
		TreasureBoxECell* c = (TreasureBoxECell*)cell;
		c->UpDateCell(mDataElemList[idx]);
    }
    return cell;
}

ssize_t TreasureBoxTableView::numberOfCellsInTableView(TableView *table)
{
	return mCellNum;
}

//////////////////

TreasureBoxECell* TreasureBoxECell::create(NetMapChestPtr ptrNetMc, EventDelegate* ptrDelegate, int iBattleType)
{
	TreasureBoxECell* pRet = new TreasureBoxECell();
	if (pRet&&pRet->init(ptrNetMc,ptrDelegate,iBattleType))
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
bool TreasureBoxECell::init(NetMapChestPtr ptrNMC, EventDelegate* ptrDelegate,int iBattleType)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	mBattleType = iBattleType;
	mNetMapChest = ptrNMC;
	mDelegate = ptrDelegate;
	int iStar = ptrNMC->star;

	Sprite* elem_bg = ReadSpriteName("new_ui/battle/battle_sbg2_1.png");
	elem_bg->setPosition(Vec2(205, 52));
	this->addChild(elem_bg);

	Sprite* goodselem_bg = ReadSpriteName("new_ui/battle/battle_sbg2_1.png");
	goodselem_bg->setPosition(Vec2(205, 52));
	goodselem_bg->setOpacity(0);
	this->addChild(goodselem_bg);

	__String* hdkqCs = NULL;
	if (mBattleType == BATTLE_ROMANCE)
		hdkqCs = __String::createWithFormat(LL("tgdgkkq"), iStar);
	else
		hdkqCs = __String::createWithFormat(LL("hdbzdxjkkq"), iStar);

	Label* hdkq = LabelSystemFont(hdkqCs->getCString(), 16);
	hdkq->setAnchorPoint(Vec2(0, 1));
	hdkq->setColor(Color3B(102, 0, 0));
	hdkq->setPosition(Vec2(16, elem_bg->getContentSize().height - 8));
	elem_bg->addChild(hdkq);

	AddGoodsListSp(ptrNMC, goodselem_bg);

	//已领取
	Sprite* ylq = ReadSpriteName("new_ui/battle/battle_txt1.png");
	ylq->setPosition(Vec2(346, 44));
	elem_bg->addChild(ylq);
	ylq->setVisible(false);

	//未领取
	Sprite* wlq = ReadSpriteName("new_ui/battle/battle_txt2.png");
	wlq->setPosition(Vec2(346, 44));
	elem_bg->addChild(wlq);
	wlq->setVisible(false);

	//领取按钮
	Menu* receive_m = Menu::create();
	receive_m->setPosition(Vec2(346, 44));
	elem_bg->addChild(receive_m);

	Sprite* receive_N = ReadSpriteName("new_ui/img_global/global_ban11.png");
	Sprite* receive_S = ReadSpriteName("new_ui/img_global/global_ban12.png");
	int img_width = receive_N->getContentSize().width;
	int img_height = receive_N->getContentSize().height;
	Sprite* receive_Ntxt = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
	receive_Ntxt->setPosition(Vec2(img_width / 2, img_height / 2));
	receive_N->addChild(receive_Ntxt);
	Sprite* receive_Stxt = ReadSpriteName("new_ui/img_text/imgtxt4_6.png");
	receive_Stxt->setPosition(Vec2(img_width / 2, img_height / 2));
	receive_S->addChild(receive_Stxt);

	MenuItemSprite* receive_item = MenuItemSprite::create(receive_N, receive_S, CC_CALLBACK_1(TreasureBoxECell::ClickReceiveMenu, this));
	receive_m->addChild(receive_item);

	mElemBox.setBg(elem_bg);
	mElemBox.setGoodsBg(goodselem_bg);
	mElemBox.setAReceive(ylq);
	mElemBox.setNReceive(wlq);
	mElemBox.setReceive(receive_m);
	mElemBox.setHdkq(hdkq);

	UpdateShow();

	return true;
}

void TreasureBoxECell::AddGoodsListSp(NetMapChestPtr ptrNMC, Sprite* ptrElemBg)
{
	int item_num = 0;
	int iShowIndex = 0;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
			item_num = ptrNMC->jade;
		else if (i == 1)
			item_num = ptrNMC->gold;
		else
			item_num = ptrNMC->improvedStone;

		if (item_num == 0) continue;

		GoodsSprite* spItem = GoodsSprite::create(10000 + i, item_num);
		spItem->setPosition(Vec2(42 + 56 * iShowIndex, 44));
		ptrElemBg->addChild(spItem);
		iShowIndex++;
	}

	int iItemIndex = 0;
	for (map<int, int>::iterator ititem = ptrNMC->itemid.begin(); ititem != ptrNMC->itemid.end(); ititem++)
	{
		int iGoodsId = ititem->first;
		int iCount = ititem->second;
		if (!iGoodsId)  continue;

		GoodsSprite* spItem = GoodsSprite::create(iGoodsId, iCount);
		spItem->setPosition(Vec2(42 + 56 * (iShowIndex + iItemIndex), 44));
		ptrElemBg->addChild(spItem);
		iItemIndex++;
	}
}

TreasureBoxECell::TreasureBoxECell()
{

}
void TreasureBoxECell::UpDateCell(NetMapChestPtr ptrNMC)
{
	mNetMapChest = ptrNMC;

	__String* hdkqCs = NULL;
	if (mBattleType == BATTLE_ROMANCE)
		hdkqCs = __String::createWithFormat(LL("tgdgkkq"), ptrNMC->star);
	else
		hdkqCs = __String::createWithFormat(LL("hdbzdxjkkq"), ptrNMC->star);

	mElemBox.getHdkq()->setString(hdkqCs->getCString());
	mElemBox.getGoodsBg()->removeAllChildren();
	AddGoodsListSp(ptrNMC, mElemBox.getGoodsBg());
	UpdateShow();
}

void TreasureBoxECell::UpdateShow()
{
	int iMapTotalStar = 0;

	if (mBattleType == BATTLE_ROMANCE)
		iMapTotalStar = history->mMapDateInfo->getRomanceWinGq(mNetMapChest->mapid);
	else
		iMapTotalStar = history->mMapDateInfo->getMapTotalStar(mNetMapChest->mapid);

	map<int, int> MapDate = history->mMapDateInfo->getMapTreasure(mNetMapChest->mapid);

	int iStar = mNetMapChest->star;
	if (MapDate.count(iStar) > 0 && MapDate[iStar] > 0)
	{
		mElemBox.getAReceive()->setVisible(true);
		mElemBox.getNReceive()->setVisible(false);
		mElemBox.getBg()->setColor(Color3B(210, 210, 210));
		mElemBox.getReceive()->setVisible(false);
	}
	else if (iStar <= iMapTotalStar)
	{
		mElemBox.getAReceive()->setVisible(false);
		mElemBox.getNReceive()->setVisible(false);
		mElemBox.getBg()->setColor(Color3B::WHITE);
		mElemBox.getReceive()->setVisible(true);
	}
	else
	{
		mElemBox.getNReceive()->setVisible(true);
		mElemBox.getAReceive()->setVisible(false);
		mElemBox.getBg()->setColor(Color3B::WHITE);
		mElemBox.getReceive()->setVisible(false);
	}
	mElemBox.getReceive()->setEnabled(true);
}

void TreasureBoxECell::ClickReceiveMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	mElemBox.getReceive()->setEnabled(false);
	SendNetDate();
}

void TreasureBoxECell::SendNetDate()
{
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("mapID=%d&star=%d&gameToken=%s", mNetMapChest->mapid, mNetMapChest->star, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"mapID\":%d,\"star\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", mNetMapChest->mapid, mNetMapChest->star, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("openTreasures",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(TreasureBoxECell::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);
}

void TreasureBoxECell::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"].isObject())
		{
			setNetDate(value["result"]);
		}
	}
	ITools::getInstance()->removeWaitLayer();
}

void TreasureBoxECell::setNetDate(Json::Value jsonValue)
{
	map<int,std::string> mPromptTxt = history->onHttpGoodsAdd(jsonValue);

	history->mMapDateInfo->addMapTreasure(mNetMapChest->mapid, mNetMapChest->star, 1);
	UpdateShow();
	if (mDelegate)
	 	mDelegate->updateRTDate(mPromptTxt);
}

void TreasureBoxECell::onExit()
{
	TableViewCell::onExit();
}
void TreasureBoxECell::onEnter()
{
	TableViewCell::onEnter();
}