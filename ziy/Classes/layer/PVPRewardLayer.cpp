//
//  PVPRewardLayer.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/22.
//
//

#include "PVPRewardLayer.h"
#include "GoodsSprite.h"

#define REWARD_W 570
#define REWARD_H 310

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPRewardCell::PVPRewardCell()
{

}

PVPRewardCell::~PVPRewardCell()
{

}

PVPRewardCell* PVPRewardCell::create()
{
	PVPRewardCell* pRet = new PVPRewardCell();
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PVPRewardCell::init()
{
	if (!TableViewCell::init())
	{
		return false;
	}

	initControl();
	return true;
}

void PVPRewardCell::UpDateCell(int iIndex)
{

}

void PVPRewardCell::initControl()
{
	int iPointX = 5;
	int iPointY = 5;
	Layer* ptrLayer = Layer::create();

	//规则标题
	Label* labRank = LabelSystemFont(LL("pvprewardtitle"), 16);
	labRank->setAnchorPoint(Vec2(0.0f, 1.0f));
	labRank->setPosition(Vec2(iPointX, -iPointY));
	labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
	ptrLayer->addChild(labRank);
	iPointY += labRank->getContentSize().height + 5;

	//规则明细
	for (UInt i = 0; ; i++)
	{
		string strInfo = StringUtils::format("pvprewardinfo%d", i + 1);
		char* chInfo = LL(strInfo.c_str(), false, true);
		if (chInfo)
		{
			Label* labInfo = LabelSystemFont(chInfo, 16);
			labInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
			labInfo->setPosition(Vec2(iPointX, -iPointY));
			labInfo->setDimensions(REWARD_W - 10, 0);
			labInfo->enableOutline(Color4B(158, 116, 48, 255), 1);
			ptrLayer->addChild(labInfo);
			iPointY += labInfo->getContentSize().height + 5;
		}
		else break;
	}
	iPointY += 10;

	//奖励
	for (map<int, PVPReward*>::iterator it = trData->m_mPVPReward.begin(); it != trData->m_mPVPReward.end(); it++)
	{
		PVPReward* ptrReward = it->second;

		//名次
		Label* labRank = LabelSystemFont(ptrReward->strName, 16);
		labRank->setAnchorPoint(Vec2(0.0f, 1.0f));
		labRank->setPosition(Vec2(iPointX, -iPointY));
		labRank->setColor(Color3B::YELLOW);
		labRank->enableOutline(Color4B(158, 116, 48, 255), 1);
		ptrLayer->addChild(labRank);
		iPointY += labRank->getContentSize().height + 5;

		//礼包
		int iIndex = 0;
		int iGoodsSize = 50;
		for (UInt i = 0; i < ptrReward->mReward.size(); i++, iIndex++)
		{
			map<int, int> mGoods = ptrReward->mReward[i];
			int iGoodsId = mGoods[0];
			int iCount = mGoods[1];

			GoodsSprite* spGoods = GoodsSprite::create(iGoodsId, iCount);
			spGoods->setPosition(Vec2(iPointX + iGoodsSize / 2 + 56 * iIndex, -iPointY - iGoodsSize / 2));
			ptrLayer->addChild(spGoods);
		}
		iPointY += iGoodsSize + 5;
	}
	m_iHeight = iPointY;
	ptrLayer->setPosition(Vec2(0, iPointY));
	this->addChild(ptrLayer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPRewardTableView::PVPRewardTableView()
{

}

PVPRewardTableView::~PVPRewardTableView()
{

}

PVPRewardTableView* PVPRewardTableView::create()
{
	PVPRewardTableView* pRet = new PVPRewardTableView();
	if (pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PVPRewardTableView::init()
{
	if (!Layer::init())
	{
		return false;
	}

	PVPRewardCell* ptrCell = PVPRewardCell::create();
	m_iCellHeight = ptrCell->getCellHeight();

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 570 * 310
	tableView = TableView::create(this, Size(REWARD_W, REWARD_H));
	//设置tableview为水平方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	tableView->setPosition(Vec2(size.width / 2 - REWARD_W / 2, size.height / 2 - REWARD_H / 2 - 15));
	//设置代理对象
	tableView->setDelegate(this);
	// 填充顺序
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(tableView);

	return true;
}

void PVPRewardTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{

}

Size PVPRewardTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(REWARD_W, m_iCellHeight);
}

TableViewCell* PVPRewardTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	if (!cell)
	{
		cell = PVPRewardCell::create();
	}
	else
	{
		PVPRewardCell* c = (PVPRewardCell*)cell;
		c->UpDateCell(idx);
	}
	return cell;
}

ssize_t PVPRewardTableView::numberOfCellsInTableView(TableView *table)
{
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPRewardLayer::PVPRewardLayer()
{

}

PVPRewardLayer::~PVPRewardLayer()
{

}

PVPRewardLayer* PVPRewardLayer::create()
{
	PVPRewardLayer* pRet = new PVPRewardLayer();
	if(pRet&&pRet->init())
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

bool PVPRewardLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	return true;
}

void PVPRewardLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景1
	auto spBG1 = ReadSpriteName("new_ui/pvp/pvp_bg5.png");
	spBG1->setPosition(point);
	this->addChild(spBG1);

	//规则奖励
	auto tvReward = PVPRewardTableView::create();
	this->addChild(tvReward);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPRewardLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(290, 165));
	ptrMenu->addChild(btnClose);
}

void PVPRewardLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}