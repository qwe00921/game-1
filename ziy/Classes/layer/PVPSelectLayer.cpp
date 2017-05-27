//
//  PVPSceneLayer.cpp
//  TRGame
//
//  Created by zwy on 17/03/31.
//
//

#include "PVPSelectLayer.h"
#include "BattleLadder.h"
#include "PVPData.h"

#define DAILY_VIEW_W 600
#define DAILY_VIEW_H 310
#define DAILY_CELL_W 200
#define DAILY_CELL_H 310

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool PVPSelectCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}

	m_spScene = NULL;
	initControl();
	return true;
}

void PVPSelectCell::upDateCell(int iIndex)
{   
	string strPath = StringUtils::format("new_ui/pvp/pvp_tit%d.png", iIndex + 1);
	m_spScene->setTexture(ReadSpriteTexture(strPath.c_str()));
}
void PVPSelectCell::initControl()
{
	m_spScene = ReadSpriteName("new_ui/pvp/pvp_tit1.png");
	m_spScene->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_spScene->setPosition(Vec2(DAILY_CELL_W / 2, DAILY_CELL_H / 2));
	this->addChild(m_spScene);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPSelectTableView* PVPSelectTableView::create()
{
	PVPSelectTableView* pRet = new PVPSelectTableView();
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

bool PVPSelectTableView::init()
{
	if (!Layer::init())
	{
		return false;
	}
	m_iClickIndex = -1;
	m_spLArrow = NULL;
	m_spRArrow = NULL;

	m_iCellNum = trData->m_mPKMap.size();
	
	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 600 * 310
	m_pTableView = TableView::create(this, Size(DAILY_VIEW_W, DAILY_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - DAILY_VIEW_W / 2, size.height / 2 - DAILY_VIEW_H / 2));
	//设置代理对象
	m_pTableView->setDelegate(this);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	//箭头左
	m_spLArrow = ReadSpriteName("new_ui/img_global/turn_page.png");
	m_spLArrow->setPosition(Vec2(size.width / 2 - DAILY_VIEW_W / 2 - m_spLArrow->getContentSize().width / 2, size.height / 2));
	this->addChild(m_spLArrow);
	//箭头右
	m_spRArrow = ReadSpriteName("new_ui/img_global/turn_page.png");
	m_spRArrow->setFlippedX(true);
	m_spRArrow->setPosition(Vec2(size.width / 2 + DAILY_VIEW_W / 2 + m_spRArrow->getContentSize().width / 2, size.height / 2));
	this->addChild(m_spRArrow);

	upDateArrow();
	return true;
}

PVPSelectTableView::PVPSelectTableView()
{

}

PVPSelectTableView::~PVPSelectTableView()
{

}

void PVPSelectTableView::scrollViewDidScroll(ScrollView* view)
{
	upDateArrow();
}

void PVPSelectTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if (m_iClickIndex < 0)
	{
		PVPSelectCell* ptrCell = (PVPSelectCell*)cell;
		m_iClickIndex = ptrCell->getIdx();
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(PVPSelectTableView::showSelectLayer, this));
		ptrCell->m_spScene->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
	
}

Size PVPSelectTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(DAILY_CELL_W, DAILY_CELL_H);
}

TableViewCell* PVPSelectTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = PVPSelectCell::create();
	}
	else
	{
		PVPSelectCell* c = (PVPSelectCell*)cell;
		c->upDateCell(idx);
	}
	return cell;
}

ssize_t PVPSelectTableView::numberOfCellsInTableView(TableView *table)
{
	return m_iCellNum;
}

void PVPSelectTableView::showSelectLayer()
{
	if (m_iClickIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		if (m_iClickIndex == 0)
		{
			if (!trData->isSystemOpen(SystemOpenIDS::E_GUOGUANZHANJIANG, true))
			{
				//ITools::getInstance()->createGlobalPrompt(StringUtils::format(LL("openafterherolv"), 30), false);
			}
			else
			{
				BattleLadder::getInstance()->SendNetData();
			}
		}
		else if (m_iClickIndex == 1)
		{
			if (trData->isSystemOpen(SystemOpenIDS::E_YANWUCHANG, true))
			{
				PVPData::getInstance()->startPVP();
			}
			else
			{
				//ITools::getInstance()->createGlobalPrompt(LL("pvpentrycondition"), false);
			}
		}
		m_iClickIndex = -1;
	}
}

void PVPSelectTableView::upDateArrow()
{
	if (m_spLArrow)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spLArrow->setVisible(point.x < 0);
		m_spRArrow->setVisible(point.x - DAILY_VIEW_W + m_iCellNum * DAILY_CELL_W > 0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPSelectLayer::PVPSelectLayer()
{

}

PVPSelectLayer::~PVPSelectLayer()
{
	
}

PVPSelectLayer* PVPSelectLayer::create()
{
	PVPSelectLayer* pRet = new PVPSelectLayer();
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

bool PVPSelectLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initControl();
	return true;
}

void PVPSelectLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	PVPSelectTableView* ptrCell = PVPSelectTableView::create();
	this->addChild(ptrCell);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//返回
	Sprite* spCloseN = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/battle/flippage3.png");
	spCloseS->setScale(fScale);
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPSelectLayer::callBackClose, this));
	spCloseS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spCloseS->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
	btnClose->setPosition(size.width - 32, size.height - 32);
	ptrMenu->addChild(btnClose);
}

void PVPSelectLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}