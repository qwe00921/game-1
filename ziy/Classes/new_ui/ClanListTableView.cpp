#include "ClanListTableView.h"
#include "Defines.h"

#define CELL_WIDTH 500
#define CELL_HEIGHT 44

ClanListTableView* ClanListTableView::create(map<int, ClanListElem*>* ptrElemList, EventDelegate* ptrDelegate)
{
	ClanListTableView *pRet = new ClanListTableView();
	if (pRet && pRet->init(ptrElemList, ptrDelegate))
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

bool ClanListTableView::init(map<int, ClanListElem*>* ptrElemList, EventDelegate* ptrDelegate)
{
    if ( !Layer::init() )
    {
        return false;
    }
	mElemDataList = ptrElemList;
	mDelegate = ptrDelegate;
	m_iSelectDataIndex = -1;

	for (size_t i = 0; i < ptrElemList->size(); i++)
		mClanIdIndex[ptrElemList->at(i)->getClanId()] = i;

	this->cell_num = mElemDataList->size();

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(CELL_WIDTH,5* CELL_HEIGHT));

    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(55,82));
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

void ClanListTableView::UpdataElemData(map<int, ClanListElem*>* ptrElemList,int iType)
{
	Vec2 vOffset = tableView->getContentOffset();
	mClanIdIndex.clear();

// 	if(m_iSelectDataIndex > -1 && m_iSelectDataIndex < mElemDataList->size())
// 		mElemDataList->at(m_iSelectDataIndex)->setIsSelect(0);

	mElemDataList = ptrElemList;
	for (size_t i = 0; i < ptrElemList->size(); i++)
		mClanIdIndex[ptrElemList->at(i)->getClanId()] = i;

	m_iSelectDataIndex = -1;
	this->cell_num = mElemDataList->size();

	tableView->reloadData();

	if (iType == 1)
		tableView->setContentOffset(vOffset-Vec2(0, CELL_HEIGHT));
}

void ClanListTableView::RunFunction(int iNomal)
{
	if (m_iSelectDataIndex > -1)
	{
		ClanListCell* ptrTCellData = (ClanListCell*)tableView->cellAtIndex(m_iSelectDataIndex);

		if (ptrTCellData)
		{
			ptrTCellData->SetSelect(false);
		}

		mElemDataList->at(m_iSelectDataIndex)->setIsSelect(0);
	}

	m_iSelectDataIndex = mClanIdIndex[iNomal];

	if (!mElemDataList->at(mClanIdIndex[iNomal])->getIsApply())
		ChessApplication::getInstance()->AddFunctionLayer(ApplyAddClanLayer::create(mElemDataList->at(mClanIdIndex[iNomal]), mDelegate));
}

void ClanListTableView::updateRTDate(int iClanId)
{
	if (mClanIdIndex.count(iClanId))
	{
		ClanListCell* ptrTCellData = (ClanListCell*)tableView->cellAtIndex(mClanIdIndex[iClanId]);
		if (ptrTCellData)
		{
			ptrTCellData->UpDateCell(ptrTCellData->GetElemData());
		}
	}
}

ClanListTableView::ClanListTableView()
{
	cell_num = 1;
}

ClanListTableView::~ClanListTableView()
{

}

void ClanListTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ClanListTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ClanListTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ClanListTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClanListTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClanListTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(CELL_WIDTH,CELL_HEIGHT);
}

TableViewCell* ClanListTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ClanListCell::create(mElemDataList->at(idx),this);
    }
    else
    {
		ClanListCell* c = (ClanListCell*)cell;
		c->UpDateCell(mElemDataList->at(idx));
    }
    return cell;
}

ssize_t ClanListTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ClanListCell* ClanListCell::create(ClanListElem* ptrElem, EventDelegate* ptrDelegate)
{
	ClanListCell* pRet = new ClanListCell();
	if (pRet&&pRet->init(ptrElem,ptrDelegate))
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

bool ClanListCell::init(ClanListElem* ptrElem, EventDelegate* ptrDelegate)
{
	if (!TableViewCell::init())
	{
		return false;
	}

	mDelegate = ptrDelegate;

	if (ptrElem->getIsSelect())
		m_bIsSelect = true;
	else
		m_bIsSelect = false;

	mDataElem = ptrElem;

	if(m_bIsSelect)
		mbg = ReadSpriteName("new_ui/gh/gh_box_bg5.png");
	else
		mbg = ReadSpriteName("new_ui/gh/gh_box_bg4.png");

	mbg->setPosition(Vec2(CELL_WIDTH/2, CELL_HEIGHT /2));
	this->addChild(mbg);

	int iBgHeight = mbg->getContentSize().height;

	mClanName = LabelSystemFont(ptrElem->getClanName().c_str(),16);
	mClanName->setPosition(Vec2(32, iBgHeight/2));
	mClanName->setAnchorPoint(Vec2(0,0.5));
	mClanName->setColor(Color3B(102,51,0));
	mbg->addChild(mClanName);

	mClanLv = LabelSystemFont(IntToString(ptrElem->getClanLv()), 16);
	mClanLv->setPosition(Vec2(172, iBgHeight / 2));
	mClanLv->setColor(Color3B(102, 51, 0));
	mbg->addChild(mClanLv);

	mElderName = LabelSystemFont(ptrElem->getElderName().c_str(), 16);
	mElderName->setPosition(Vec2(223, iBgHeight / 2));
	mElderName->setAnchorPoint(Vec2(0, 0.5));
	mElderName->setColor(Color3B(153, 0, 0));
	mbg->addChild(mElderName);

	mHeroNum = LabelSystemFont(__String::createWithFormat("%d/%d", ptrElem->getHeroNum(), ptrElem->getHeroMaxNum())->getCString(), 16);
	mHeroNum->setPosition(Vec2(442, iBgHeight / 2));
	mHeroNum->setAnchorPoint(Vec2(1, 0.5));
	mHeroNum->setColor(Color3B(102, 51, 0));
	mbg->addChild(mHeroNum);

	mIsApply = ReadSpriteName("new_ui/gh/gh_txt1.png");
	mIsApply->setPosition(Vec2(mbg->getContentSize().width - 5, iBgHeight / 2));
	mIsApply->setAnchorPoint(Vec2(1,0.5));
	mbg->addChild(mIsApply);

	if (!ptrElem->getIsApply())
		mIsApply->setVisible(false);

	initTouches();

	return true;
}

ClanListCell::ClanListCell()
{

}

void ClanListCell::UpDateCell(ClanListElem* ptrElem)
{
	mClanName->setString(ptrElem->getClanName().c_str());
	mClanLv->setString(IntToString(ptrElem->getClanLv()));
	mElderName->setString(ptrElem->getElderName().c_str());
	mHeroNum->setString(__String::createWithFormat("%d/%d", ptrElem->getHeroNum(), ptrElem->getHeroMaxNum())->getCString());

	mDataElem = ptrElem;

	if (mDataElem->getIsSelect())
	{
		mbg->setTexture(ReadSpriteTexture("new_ui/gh/gh_box_bg5.png"));
		m_bIsSelect = true;
	}
	else
	{
		mbg->setTexture(ReadSpriteTexture("new_ui/gh/gh_box_bg4.png"));
		m_bIsSelect = false;
	}

	if (!ptrElem->getIsApply())
		mIsApply->setVisible(false);
	else
		mIsApply->setVisible(true);
}

void ClanListCell::SetSelect(bool bIsSelect)
{
	if (bIsSelect && !m_bIsSelect)
	{
		mbg->setTexture(ReadSpriteTexture("new_ui/gh/gh_box_bg5.png"));
		mDataElem->setIsSelect(1);
	}
	else if (!bIsSelect && m_bIsSelect)
	{
		mbg->setTexture(ReadSpriteTexture("new_ui/gh/gh_box_bg4.png"));
		mDataElem->setIsSelect(0);
	}

	m_bIsSelect = bIsSelect;
}

void ClanListCell::initTouches()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event) {
		m_bIsClickSelect = false;
		Rect bgBox = mbg->getBoundingBox();
		if (Rect(0, 0, bgBox.size.width, bgBox.size.height).containsPoint(mbg->convertToNodeSpace(touch->getLocation())))
		{
			m_bIsClickSelect = true;
		}
		return true;
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event) {
		
		if (m_bIsClickSelect)
		{
			float fOffsetY = touch->getLocation().y - touch->getStartLocation().y;

			if (fOffsetY<1 && fOffsetY>-1)
			{
				if (!mDataElem->getIsSelect())
				{
					SetSelect(true);

					if (mDelegate)
						mDelegate->RunFunction(mDataElem->getClanId());
				}
			}

		}
	};

	listener->setSwallowTouches(false);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

int ClanListCell::GetClanIndex()
{
	return mDataElem->getEIndex();
}

ClanListElem* ClanListCell::GetElemData()
{
	return mDataElem;
}