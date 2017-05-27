#include "ClanLogTableView.h"
#include "Defines.h"

#define LOG_CELL_WIDTH 504
#define LOG_CELL_HEIGHT 30

void ClanLogElem::LoadJson(Json::Value vContent)
{
	string contentS = "";
	if (vContent["content"].isString())
	{
		contentS = vContent["content"].asString();
	}

	DString strData = contentS.c_str();
	DString strCur = "";
	int i = 0, index1 = -1, index2 = -1;
	bool addQ = false;
	do
	{
		index1 = index2 + 1;
		index2 = strData->indexOf("Q", index1);
		addQ = false;

		if (index2 != -1)
		{
			strCur = strData->substring(index1, index2);
			addQ = true;
		}
		else
		{
			strCur = strData->substring(index1, contentS.length());
		}

		if (strCur != "")
		{
			mContentlist[i++] = strCur->getChar();
		}

		if (addQ)
		{
			mContentlist[i++] = "Q";
		}

	} while (index2 != -1);

	if (!vContent["info"].isNull())
	{
		Json::Value promptInfo = vContent["info"];

		for (Json::Value::iterator it = promptInfo.begin(); it != promptInfo.end(); it++)
		{
			string pInfoKey = it.key().asCString();

			if (promptInfo[pInfoKey].isString())
				mInfoElem[pInfoKey] = promptInfo[pInfoKey].asCString();
			else if (promptInfo[pInfoKey].isInt())
				mInfoElem[pInfoKey] = IntToString(promptInfo[pInfoKey].asInt());
		}
	}


}

ClanLogTableView* ClanLogTableView::create(Json::Value vLogData)
{
	ClanLogTableView *pRet = new ClanLogTableView();
	if (pRet && pRet->init(vLogData))
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

bool ClanLogTableView::init(Json::Value vLogData)
{
    if ( !Layer::init() )
    {
        return false;
    }
	getLogLost(vLogData);
	this->cell_num = mLogList.size();
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(LOG_CELL_WIDTH,9* LOG_CELL_HEIGHT));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(53,54));
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

void ClanLogTableView::getLogLost(Json::Value vLogData)
{
	for (UInt i = 0; i < vLogData.size(); i++)
	{
		mLogList[i] = new ClanLogElem();
		mLogList[i]->LoadJson(vLogData[i]);
	}

	cell_num = mLogList.size();
}

ClanLogTableView::ClanLogTableView()
{
	cell_num = 0;
}

ClanLogTableView::~ClanLogTableView()
{
	for (size_t i = 0; i < mLogList.size(); i++)
	{
		delete mLogList[i];
	}
}

void ClanLogTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ClanLogTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ClanLogTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ClanLogTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClanLogTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClanLogTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(LOG_CELL_WIDTH, LOG_CELL_HEIGHT);
}

TableViewCell* ClanLogTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ClanLogCell::create(mLogList[idx]);
    }
    else
    {
		ClanLogCell* c = (ClanLogCell*)cell;
		c->UpDateCell(mLogList[idx]);
    }
    return cell;
}

ssize_t ClanLogTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ClanLogCell* ClanLogCell::create(ClanLogElem* ptrLogElem)
{
	ClanLogCell* pRet = new ClanLogCell();
	if (pRet&&pRet->init(ptrLogElem))
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

bool ClanLogCell::init(ClanLogElem* ptrLogElem)
{
	if (!TableViewCell::init())
	{
		return false;
	}

	mLogBg = ReadSpriteName("new_ui/gh/gh_bg4.png");
	mLogBg->setPosition(Vec2(LOG_CELL_WIDTH/2,LOG_CELL_HEIGHT/2));
	this->addChild(mLogBg);

	addString(ptrLogElem);
	return true;
}
ClanLogCell::ClanLogCell()
{

}

void ClanLogCell::UpDateCell(ClanLogElem* ptrLogElem)
{
	addString(ptrLogElem);
}

void ClanLogCell::addString(ClanLogElem* ptrLogElem)
{
	mLogBg->removeAllChildren();

	int elemIndex = 1;
	Label* ptrSLabel = NULL;
	float StartPX = 10;
	for (size_t j = 0; j < ptrLogElem->mContentlist.size(); j++)
	{
	 	if (ptrLogElem->mContentlist[j] == "Q")
	 	{
	 		string sKey = StringUtils::format("Q%d", elemIndex);
	 		if (ptrLogElem->mInfoElem.count(sKey))
	 		{
	 			ptrSLabel = LabelSystemFont(ptrLogElem->mInfoElem[sKey].c_str(), 16);
	 			ptrSLabel->setColor(Color3B::YELLOW);
	 			ptrSLabel->setAnchorPoint(Vec2(0, 0.5f));
	 			ptrSLabel->setPosition(Vec2(StartPX, mLogBg->getContentSize().height/2));
	 			mLogBg->addChild(ptrSLabel);
				StartPX += ptrSLabel->getContentSize().width + 2;
	 		}
	 		elemIndex++;
	 	}
	 	else
	 	{
	 		ptrSLabel = LabelSystemFont(ptrLogElem->mContentlist[j].c_str(), 16);
	 		ptrSLabel->setAnchorPoint(Vec2(0, 0.5f));
	 		ptrSLabel->setPosition(Vec2(StartPX, mLogBg->getContentSize().height / 2));
			mLogBg->addChild(ptrSLabel);
	 		StartPX += ptrSLabel->getContentSize().width + 2;
	 	}
	}
}