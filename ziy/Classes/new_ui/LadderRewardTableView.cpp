#include "LadderRewardTableView.h"
#include "Defines.h"
#include "GoodsSprite.h"

bool LadderRewardTableView::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2::ZERO);
	this->addChild(black_layer);

	Sprite* bg = ReadSpriteName("new_ui/img_global/global_ph_bg3.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	Sprite* infobgSp = ReadSpriteName("new_ui/img_global/mail_bg5.png");
	infobgSp->setPosition(Vec2(bg->getContentSize().width/2,345));
	bg->addChild(infobgSp);

	Label* infoLab = LabelSystemFont(LL("ggzjgzsm"),18);
	infoLab->setPosition(Vec2(infobgSp->getContentSize().width/2, infobgSp->getContentSize().height/2));
	infobgSp->addChild(infoLab);

    //创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 300 * 300
    tableView = TableView::create(this, Size(360,270));
    //设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    //设置位置
    tableView->setPosition(Vec2((bg->getContentSize().width - 360)/2,48));
    //设置代理对象
    tableView->setDelegate(this);
    // 填充顺序
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    //添加tableview到当前layer
	bg->addChild(tableView);
    //加载tableview
    //tableView->reloadData();

	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(427, 373));
	bg->addChild(ptrMenu);

	Sprite* closeMenuNSp = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeMenuSSp = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeMenuNSp, closeMenuSSp, CC_CALLBACK_1(LadderRewardTableView::clickCloseMenu, this));
	ptrMenu->addChild(close_menu);
    
    return true;
}


void LadderRewardTableView::clickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

LadderRewardTableView::LadderRewardTableView()
{
	
}

LadderRewardTableView::~LadderRewardTableView()
{

}

void LadderRewardTableView::scrollViewDidScroll(ScrollView* view)
{

}

void LadderRewardTableView::scrollViewDidZoom(ScrollView* view)
{

}

void LadderRewardTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

void LadderRewardTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void LadderRewardTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size LadderRewardTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(360,690);
}

TableViewCell* LadderRewardTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();

    if (!cell) {
		cell = LadderRewardCell::create();
    }
    return cell;
}

ssize_t LadderRewardTableView::numberOfCellsInTableView(TableView *table)
{
	return 1;
}

/////////////////////////////////////////////////////////////

bool LadderRewardCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}
	int m_iCellWidth = 360;
	int m_iCellHeight = 690;

	int iPointX = 10;
	int iPointY = m_iCellHeight;
	Label* guize1 = LabelSystemFont(LL("guoguanzhanjiang1"),16);
	guize1->setAnchorPoint(Vec2(0,1));
	guize1->setPosition(Vec2(iPointX, iPointY));
	guize1->setDimensions(340, 0);
	this->addChild(guize1);

	iPointY -= guize1->getContentSize().height+2;

	Label* guize2 = LabelSystemFont(LL("guoguanzhanjiang2"), 16);
	guize2->setAnchorPoint(Vec2(0, 1));
	guize2->setPosition(Vec2(iPointX, iPointY));
	guize2->setDimensions(340, 0);
	this->addChild(guize2);

	iPointY -= guize2->getContentSize().height+2;

	Label* guize3 = LabelSystemFont(LL("guoguanzhanjiang3"), 16);
	guize3->setAnchorPoint(Vec2(0, 1));
	guize3->setPosition(Vec2(iPointX, iPointY));
	guize3->setDimensions(340, 0);
	this->addChild(guize3);

	iPointY -= guize3->getContentSize().height+6;

	for (map<int, NetStalkerReward*>::iterator eIt = trData->net_stalker_reward.begin(); eIt != trData->net_stalker_reward.end(); eIt++)
	{
		Label* rankingSp = LabelSystemFont(eIt->second->name.c_str(),16);
		rankingSp->setColor(Color3B::YELLOW);
		rankingSp->setPosition(Vec2(iPointX, iPointY));
		rankingSp->setAnchorPoint(Vec2(0,1));
		this->addChild(rankingSp);

		iPointY -= rankingSp->getContentSize().height+4;

		int iGoodsBgWidth = 0;
		int iGoodsBgHeight = 0;
		int iGoodsItemIndex = 0;

		for (size_t i = 0; i < eIt->second->itemid.size(); i++)
		{
			int iEGoodsId = eIt->second->itemid[i][0];
			int iECount = eIt->second->itemid[i][1];

			if(iEGoodsId == 0 || iECount == 0)
				continue;

			GoodsSprite* goodsSp = GoodsSprite::create(iEGoodsId, iECount);

			iGoodsBgWidth = goodsSp->getContentSize().width;
			iGoodsBgHeight = goodsSp->getContentSize().height;

			goodsSp->setAnchorPoint(Vec2(0, 1));
			goodsSp->setPosition(Vec2(iPointX + (iGoodsBgWidth + 8)*(iGoodsItemIndex % 6), iPointY - iGoodsItemIndex / 6 * (iGoodsBgHeight + 8)));
			this->addChild(goodsSp);

			iGoodsItemIndex++;
		}

		iPointY -= ceil(iGoodsItemIndex / 6.0) * iGoodsBgHeight + 8;
	}

	return true;
}
LadderRewardCell::LadderRewardCell()
{

}
