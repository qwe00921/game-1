#include "ClanTBHpLogTableView.h"
#include "Defines.h"

#define LOG_CELL_WIDTH 444
#define LOG_CELL_HEIGHT 30

void ClanTBHpLogElem::LoadJson(Json::Value vContent)
{
	ParseString(nameS, vContent["name"]);
	ParseInt(hurtS, vContent["hurt"]);
	ParseString(dateS, vContent["Date"]);
}

ClanTBHpLogTableView* ClanTBHpLogTableView::create(Json::Value vLogData, int iWarid)
{
	ClanTBHpLogTableView *pRet = new ClanTBHpLogTableView();
	if (pRet && pRet->init(vLogData,iWarid))
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

bool ClanTBHpLogTableView::init(Json::Value vLogData, int iWarid)
{
    if ( !Layer::init() )
    {
        return false;
    }
	m_iWarId = iWarid;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);

	Sprite* ptrbg = ReadSpriteName("new_ui/gh/gh_bg11.png");
	ptrbg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(ptrbg);


	getLogLost(vLogData);
	this->cell_num = mLogList.size();
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(LOG_CELL_WIDTH,9* LOG_CELL_HEIGHT));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2(37,54));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
	ptrbg->addChild(tableView);
    //加载tableview
    //tableView->reloadData();

	Menu* mCloseMenu = Menu::create();
	mCloseMenu->setPosition(Vec2(494, 350));
	ptrbg->addChild(mCloseMenu, 2);
	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ClanTBHpLogTableView::clickBackMenu, this));
	mCloseMenu->addChild(close_menu);
    
    return true;
}

void ClanTBHpLogTableView::getLogLost(Json::Value vLogData)
{
	int iIndex = vLogData.size() - 1;
	for (UInt i = 0; i < vLogData.size(); i++)
	{
		mLogList[iIndex] = new ClanTBHpLogElem();
		mLogList[iIndex]->LoadJson(vLogData[i]);
		iIndex--;
	}

	cell_num = mLogList.size();
}

void ClanTBHpLogTableView::clickBackMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

ClanTBHpLogTableView::ClanTBHpLogTableView()
{
	cell_num = 0;
}

ClanTBHpLogTableView::~ClanTBHpLogTableView()
{
	for (size_t i = 0; i < mLogList.size(); i++)
	{
		delete mLogList[i];
	}
}

void ClanTBHpLogTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ClanTBHpLogTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ClanTBHpLogTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void ClanTBHpLogTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ClanTBHpLogTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ClanTBHpLogTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(LOG_CELL_WIDTH, LOG_CELL_HEIGHT);
}

TableViewCell* ClanTBHpLogTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = ClanTBHpLogCell::create(mLogList[idx],m_iWarId);
    }
    else
    {
		ClanTBHpLogCell* c = (ClanTBHpLogCell*)cell;
		c->UpDateCell(mLogList[idx]);
    }
    return cell;
}

ssize_t ClanTBHpLogTableView::numberOfCellsInTableView(TableView *table)
{
	return cell_num;
}

ClanTBHpLogCell* ClanTBHpLogCell::create(ClanTBHpLogElem* ptrLogElem,int iWarId)
{
	ClanTBHpLogCell* pRet = new ClanTBHpLogCell();
	if (pRet&&pRet->init(ptrLogElem,iWarId))
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

bool ClanTBHpLogCell::init(ClanTBHpLogElem* ptrLogElem,int iWarId)
{
	if (!TableViewCell::init())
	{
		return false;
	}
	m_iWarId = iWarId;

	mLogBg = ReadSpriteName("new_ui/gh/gh_bg12.png");
	mLogBg->setPosition(Vec2(LOG_CELL_WIDTH/2,LOG_CELL_HEIGHT/2));
	this->addChild(mLogBg);

	addString(ptrLogElem);
	return true;
}
ClanTBHpLogCell::ClanTBHpLogCell()
{

}

void ClanTBHpLogCell::UpDateCell(ClanTBHpLogElem* ptrLogElem)
{
	addString(ptrLogElem);
}

void ClanTBHpLogCell::addString(ClanTBHpLogElem* ptrLogElem)
{
	mLogBg->removeAllChildren();

	int iStartX = 5, iStartY = mLogBg->getContentSize().height/2;
	Label* dateLab = LabelSystemFont(ptrLogElem->getEdateS().c_str(),16);
	dateLab->setPosition(Vec2(iStartX,iStartY));
	dateLab->setAnchorPoint(Vec2(0,0.5));
	//dateLab->setColor(Color3B::YELLOW);
	mLogBg->addChild(dateLab);
	iStartX += dateLab->getContentSize().width + 7;
	
	Label* nameLab = LabelSystemFont(ptrLogElem->getEnameS().c_str(), 16);
	nameLab->setColor(Color3B::YELLOW);
	nameLab->setAnchorPoint(Vec2(0,0.5));
	nameLab->setPosition(Vec2(iStartX,iStartY));
	mLogBg->addChild(nameLab);
	iStartX += nameLab->getContentSize().width + 7;

	string gainHpText = StringUtils::format(LL("dszcdsh"),trData->m_mNetWarInfo[m_iWarId]->strName.c_str(), ptrLogElem->getEhurtS());
	Label* gainHpLab = LabelSystemFont(gainHpText.c_str(),16);
	gainHpLab->setAnchorPoint(Vec2(0,0.5));
	gainHpLab->setPosition(Vec2(iStartX, iStartY));
	mLogBg->addChild(gainHpLab);
}