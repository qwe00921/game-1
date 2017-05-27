//
//  ActivityLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#include "ActivityLayer.h"
#include "ActivityInfoLayer.h"
#include "ActivityInfo.h"
#include "Net_HomePage.h"

#define ACT_VIEW_W 200
#define ACT_VIEW_H 290
#define ACT_CELL_W 200
#define ACT_CELL_H 78

//ActivityCell////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ActivityCell::ms_iSelect = 0;

bool ActivityCell::init()
{
	if(!TableViewCell::init())
	{
		return false;
	}

	initControl();
	return true;
}

void ActivityCell::initControl()
{
	Vec2 point = Vec2(ACT_CELL_W / 2, ACT_CELL_H / 2);

	//背景
	m_spBGN = ReadSpriteName("new_ui/activity/activity_bg3.png");
	m_spBGN->setPosition(point);
	this->addChild(m_spBGN);

	//活动ICON
	m_spIcon = ReadSpriteName("new_ui/activity/a_icon100.png");
	m_spIcon->setPosition(Vec2(36, 34));
	m_spBGN->addChild(m_spIcon);

	//标题
	m_labTitle = LabelSystemFont("", 16);
	m_labTitle->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTitle->setPosition(Vec2(66, 48));
	m_labTitle->setColor(Color3B(102, 0, 0));
	m_spBGN->addChild(m_labTitle);

	//类型
	m_labMark = LabelSystemFont("", 14);
	m_labMark->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labMark->setPosition(Vec2(66, 24));
	m_labMark->setColor(Color3B(51, 51, 51));
	m_spBGN->addChild(m_labMark);

	//选中框
	m_spSelect = ReadSpriteName("new_ui/activity/activity_bg4.png");
	m_spSelect->setPosition(Vec2(point.x-13, point.y - 2));
	m_spBGN->addChild(m_spSelect);
}

void ActivityCell::updateCell(int iActId)
{
	ActivityElem* ptrActElem = BTActivity::getInstance()->m_mActivity[iActId];
	string strBGPath = ms_iSelect == iActId ? "new_ui/activity/activity_bg2.png" : "new_ui/activity/activity_bg3.png";
	__String* strIcon = __String::createWithFormat("new_ui/activity/a_icon%d.png", ptrActElem->getImgId());
	m_spBGN->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strBGPath.c_str())));
	m_spIcon->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strIcon->getCString())));
	m_labTitle->setString(ptrActElem->getTitle().c_str());
	m_labMark->setString(ptrActElem->getMark().c_str());
	m_spSelect->setVisible(ms_iSelect == iActId);
}

//ActivityTableView////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ActivityTableView* ActivityTableView::create(EventDelegate* ptrDelegate)
{
	ActivityTableView* pRet = new ActivityTableView();
	if(pRet&&pRet->init(ptrDelegate))
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

bool ActivityTableView::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_iCellNum = BTActivity::getInstance()->m_mActivity.size();

	int iActIndex = 0;
	for(map<int, ActivityElem*>::iterator it = BTActivity::getInstance()->m_mActivity.begin(); it != BTActivity::getInstance()->m_mActivity.end(); it++, iActIndex++)
	{
		m_mActId[iActIndex] = it->first;
	}

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 200 * 320
	m_pTableView = TableView::create(this, Size(ACT_VIEW_W, ACT_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - ACT_VIEW_W / 2 - 180, size.height / 2 - ACT_VIEW_H / 2 - 4));
	//设置代理对象
	m_pTableView->setDelegate(this);
//GHB	m_pTableView->setTouchPriority(LAYER_BOTTOM);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	//箭头上
	m_spArrowUp = ReadSpriteName("new_ui/activity/activity_jt1.png");
	m_spArrowUp->setPosition(Vec2(size.width / 2 - 180, size.height / 2 + ACT_VIEW_H / 2 - 3));
	this->addChild(m_spArrowUp);

	//箭头下
	m_spArrowDown = ReadSpriteName("new_ui/activity/activity_jt2.png");
	m_spArrowDown->setPosition(Vec2(size.width / 2 - 180, size.height / 2 - ACT_VIEW_H / 2 - 5));
	this->addChild(m_spArrowDown);

	updateArrowShow();
	return true;
}

ActivityTableView::ActivityTableView()
{
	m_spArrowUp = NULL;
	m_spArrowDown = NULL;
	m_iCellNum = 0;
}

ActivityTableView::~ActivityTableView()
{

}

void ActivityTableView::scrollViewDidScroll(ScrollView* view)
{
	updateArrowShow();
}

void ActivityTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ActivityTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	ActivityCell* ptrCell = (ActivityCell*)cell;
	ActivityCell::ms_iSelect = m_mActId[ptrCell->getIdx()];
	ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
	CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(ActivityTableView::showActivityInfo, this));
	ptrCell->m_spBGN->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
}

void ActivityTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ActivityTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ActivityTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(ACT_CELL_W, ACT_CELL_H);
}

TableViewCell* ActivityTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	ActivityCell* cell = NULL;
	if(ptrCell)
	{
		cell = (ActivityCell*)ptrCell;
	}
	else
	{
		cell = ActivityCell::create();
	}
	cell->updateCell(m_mActId[idx]);
	return cell;
}

ssize_t ActivityTableView::numberOfCellsInTableView(TableView *table)
{
	return m_iCellNum;
}

void ActivityTableView::showActivityInfo()
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	Vec2 point = m_pTableView->getContentOffset();
	m_pTableView->reloadData();
	m_pTableView->setContentOffset(point);
	m_pDelegate->callBackListClick(ActivityCell::ms_iSelect);
}

void ActivityTableView::updateArrowShow()
{
	if(m_spArrowUp)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spArrowUp->setVisible(point.y - ACT_VIEW_H + m_iCellNum * ACT_CELL_H > 0);
		m_spArrowDown->setVisible(point.y < 0);
	}
}

//ActivityLayer////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ActivityLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	BTActivity::getInstance()->updateActByType(ACT_LEVEL);
	BTActivity::getInstance()->updateActByType(ACT_EXCHENGE);
	initControl();

	return true;
}

ActivityLayer::ActivityLayer()
{
	ActivityCell::ms_iSelect = BTActivity::getInstance()->m_mActivity.begin()->first;
}

ActivityLayer::~ActivityLayer()
{
	ActivityCell::ms_iSelect = 0;
}

void ActivityLayer::onEnter()
{
	Layer::onEnter();
}

void ActivityLayer::onExit()
{
	Layer::onExit();
}

bool ActivityLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void ActivityLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void ActivityLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void ActivityLayer::initCustomEvents() {
	/*
	auto _listener = EventListenerCustom::create("GAME_LAYER_CLOSE", [=](EventCustom* event) {
		std::string str("BuyJadeLayer");
		const char* buf = static_cast<const char*>(event->getUserData());
		if (str.compare(buf) == 0) {
			//this->runAction(RemoveSelf::create());
		}
		
	});

	getEventDispatcher()->addEventListenerWithFixedPriority(_listener, 1);
	*/
}

void ActivityLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/activity/activity_bg1.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	ActivityTableView* ptrActTab = ActivityTableView::create(this);
	this->addChild(ptrActTab);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(ActivityLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(297, 169));
	ptrMenu->addChild(btnClose);

	m_layActInfo = ActivityInfoLayer::create(this);
	this->addChild(m_layActInfo);
}

void ActivityLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	HongDian::getInstence()->autoUpdate();
	this->removeFromParent();
}

void ActivityLayer::callBackListClick(int iIndex)
{
	m_layActInfo->updateActLayer(iIndex);
}
