#include "NetLevelInfoTableView.h"
#include "Defines.h"

NetLevelInfoTableView* NetLevelInfoTableView::create(map<int, map<int, int> > f_elem)
{
	NetLevelInfoTableView *pRet = new NetLevelInfoTableView();
	pRet->elem_list = f_elem;
	if (pRet && pRet->init())
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

bool NetLevelInfoTableView::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	mCellNum = elem_list.size();
	if (mCellNum < 5) 
		mCellNum = 5;
    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(320,100));
//GHB	tableView->setTouchPriority(LAYER_SECOND-1);
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    //设置位置
    tableView->setPosition(Vec2(247,146));
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

NetLevelInfoTableView::NetLevelInfoTableView():BTLayer(false)
{

}

NetLevelInfoTableView::~NetLevelInfoTableView()
{

}

void NetLevelInfoTableView::scrollViewDidScroll(ScrollView* view)
{

}

void NetLevelInfoTableView::scrollViewDidZoom(ScrollView* view)
{

}

void NetLevelInfoTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void NetLevelInfoTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void NetLevelInfoTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size NetLevelInfoTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(64,100);
}

TableViewCell* NetLevelInfoTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	int i_actor_id = -1;
	int i_num = 0;
	int i_lv = 0;
	if (idx < (int)elem_list.size())
	{
		i_actor_id = elem_list[idx][0] -1;
		i_lv = i_num = elem_list[idx][1];
		i_num = elem_list[idx][2];
	}
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
		cell = NetLevelInfoCell::create(i_actor_id, i_num, i_lv);
    }
    else
    {
		NetLevelInfoCell* c = (NetLevelInfoCell*)cell;
		c->UpDateCell(i_actor_id, i_num, i_lv);
    }
    return cell;
}

ssize_t NetLevelInfoTableView::numberOfCellsInTableView(TableView *table)
{
	return mCellNum;
}




NetLevelInfoCell* NetLevelInfoCell::create(int iActorId, int iNum, int iLv)
{
	NetLevelInfoCell* pRet = new NetLevelInfoCell();
	pRet->actor_id = iActorId;
	pRet->mNum = iNum;
	pRet->mLv = iLv;
	if (pRet&&pRet->init())
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
bool NetLevelInfoCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int m_wight = 64;

	bg = ReadSpriteName("new_ui/battle/battle_bg2.png");
	bg->setAnchorPoint(Vec2(0,0.5));
	bg->setPosition(Vec2((m_wight - bg->getContentSize().width) / 2, bg->getContentSize().height+6));
	this->addChild(bg);

	int point_x = bg->getContentSize().width / 2;
	int point_y = bg->getContentSize().height / 2-1;

	actor_img = ReadSpriteName("new_ui/battle/battle_48.png");
	actor_img->setAnchorPoint(Vec2(0.5, 0));
	actor_img->setPosition(Vec2(point_x, point_y));
	bg->addChild(actor_img);

	lv = LabelSystemFont("", 13);
	lv->setPosition(Vec2(point_x, point_y + actor_img->getContentSize().height+6));
	lv->setColor(Color3B(102, 0, 0));
	bg->addChild(lv);

	num = LabelSystemFont("", 16);
	num->setPosition(Vec2(point_x+13, point_y));
	num->setAnchorPoint(Vec2(0, 0));
	num->setColor(Color3B(102, 0, 0));
	bg->addChild(num);

	name = LabelSystemFont("", 14);
	name->setPosition(Vec2(point_x, -1));
	name->setAnchorPoint(Vec2(0.5, 1));
	name->setColor(Color3B(102, 0, 0));
	bg->addChild(name);
	UpDateCell(actor_id, mNum, mLv);
	return true;
}
NetLevelInfoCell::NetLevelInfoCell()
{
}
void NetLevelInfoCell::onExit()
{
	TableViewCell::onExit();
}
bool NetLevelInfoCell::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}
void NetLevelInfoCell::UpDateCell(int iActorId, int iNum, int iLv)
{
	mLv = iLv;
	actor_id = iActorId;
	mNum = iNum;

	if (actor_id > -1)
	{
		actor_img->setVisible(true);
		lv->setVisible(true);
		num->setVisible(true);
		name->setVisible(true);

		removeSpriteGray(bg);

		HistoryActorPtr ptrActor = &history->actors[actor_id];
		actor_img->setTexture(ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov",2)));
		actor_img->setTextureRect(Rect(0, 0, 48, 48));
		__String* lvS = __String::createWithFormat("%s%d",LL("dengji"),mLv);
		lv->setString(lvS->getCString());
		__String* numS = __String::createWithFormat("x%d", mNum);
		num->setString(numS->getCString());
		name->setString(trData->actors[actor_id].getActorName());
	}
	else
	{
		addSpriteGray(bg);
		actor_img->setVisible(false);
		lv->setVisible(false);
		num->setVisible(false);
		name->setVisible(false);
	}
}