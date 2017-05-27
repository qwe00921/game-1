//
//  DailySelectLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/08/18.
//
//

#include "DailySelectLayer.h"
#include "DispatchLayer.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "BattleTitleLayer.h"
#include "ShortcutFunctionLayer.h"
#include "GetNetDateTool.h"
#include "SweepTableView.h"

#define DAILY_VIEW_W 408
#define DAILY_VIEW_H 100
#define DAILY_CELL_W 136
#define DAILY_CELL_H 100

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DailySelectCell::init()
{
	if(!TableViewCell::init())
	{
		return false;
	}

	initControl();
	return true;
}

void DailySelectCell::updateCell(int iWarId)
{
	//选项
	string strPath = StringUtils::format("new_ui/daily/daily_icon%d.png", trData->m_mNetWarInfo[iWarId]->iImgId);
	m_spSelect->setTexture(ReadSpriteTexture(strPath.c_str()));

	//已通关
	m_spPassed->setVisible(history->mMapDateInfo->isPassed(iWarId));
}

void DailySelectCell::initControl()
{
	//选项
	m_spSelect = ReadSpriteName("new_ui/daily/daily_icon1.png");
	m_spSelect->setPosition(Vec2(DAILY_CELL_W / 2, DAILY_CELL_H / 2));
	this->addChild(m_spSelect);

	//已通关
	m_spPassed = ReadSpriteName("new_ui/battle/icon_tuguan1.png");
	m_spPassed->setPosition(Vec2(m_spSelect->getContentSize().width / 2, m_spSelect->getContentSize().height - 15));
	m_spSelect->addChild(m_spPassed);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DailySelectTableView* DailySelectTableView::create(map<int, int> mWarId, EventDelegate* pDelegate)
{
	DailySelectTableView* pRet = new DailySelectTableView();
	if(pRet&&pRet->init(mWarId, pDelegate))
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

bool DailySelectTableView::init(map<int, int> mWarId, EventDelegate* pDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iClickIndex = -1;
	m_spLArrow = NULL;
	m_spRArrow = NULL;
	m_mWarId = mWarId;
	m_pDelegate = pDelegate;

	//创建一个talbleview 将datasource设置为当前的类的对象
	m_pTableView = TableView::create(this, Size(DAILY_VIEW_W, DAILY_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::HORIZONTAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - DAILY_VIEW_W / 2, size.height / 2 - DAILY_VIEW_H / 2 - 10));
	//设置代理对象
	m_pTableView->setDelegate(this);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	//箭头左
	m_spLArrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	m_spLArrow->setPosition(Vec2(size.width / 2 - DAILY_VIEW_W / 2 - m_spLArrow->getContentSize().width, size.height / 2 - 10));
	this->addChild(m_spLArrow);

	//箭头右
	m_spRArrow = ReadSpriteName("new_ui/battle/battle_arrow2.png");
	m_spRArrow->setFlippedX(true);
	m_spRArrow->setPosition(Vec2(size.width / 2 + DAILY_VIEW_W / 2 + m_spRArrow->getContentSize().width, size.height / 2 - 10));
	this->addChild(m_spRArrow);

	updateArrow();
	return true;
}

DailySelectTableView::DailySelectTableView()
{

}

DailySelectTableView::~DailySelectTableView()
{

}

void DailySelectTableView::scrollViewDidScroll(ScrollView* view)
{
	updateArrow();
}

void DailySelectTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if (m_iClickIndex < 0)
	{
		DailySelectCell* ptrCell = (DailySelectCell*)cell;
		m_iClickIndex = ptrCell->getIdx();
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(DailySelectTableView::showDispatch, this));
		ptrCell->m_spSelect->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
}

Size DailySelectTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(DAILY_CELL_W, DAILY_CELL_H);
}

TableViewCell* DailySelectTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	DailySelectCell* cell = NULL;
	if(ptrCell)
	{
		cell = (DailySelectCell*)ptrCell;
	}
	else
	{
		cell = DailySelectCell::create();
	}
	cell->updateCell(m_mWarId[idx]);
	return cell;
}

ssize_t DailySelectTableView::numberOfCellsInTableView(TableView *table)
{
	return m_mWarId.size();
}

void DailySelectTableView::showDispatch()
{
	if (m_iClickIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_pDelegate->callBackListClick(m_iClickIndex);
		m_iClickIndex = -1;
	}
}

void DailySelectTableView::updateArrow()
{
	if(m_spLArrow)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spLArrow->setVisible(point.x < 0);
		m_spRArrow->setVisible(point.x - DAILY_VIEW_W + m_mWarId.size() * DAILY_CELL_W > 0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DailySelectLayer* DailySelectLayer::create(DailyDate* ptrDaily)
{
	DailySelectLayer* pRet = new DailySelectLayer();
	if(pRet&&pRet->init(ptrDaily))
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

bool DailySelectLayer::init(DailyDate* ptrDaily)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrDaily = ptrDaily;
	m_iSweepId = -1;
	m_iSweepType = -1;

	initControl();
	updateDisplay();
	return true;
}

DailySelectLayer::DailySelectLayer()
{

}

DailySelectLayer::~DailySelectLayer()
{

}

void DailySelectLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	DailyWar* ptrDaily = trData->m_mDailyWar[m_ptrDaily->getMapId()];
	for (UInt i = 0; i < ptrDaily->mWarId.size(); i++)
	{
		int iWarId = ptrDaily->mWarId[i];
		if (history->mMapDateInfo->isPassed(iWarId) && iWarId > m_iSweepId)
		{
			m_iSweepId = iWarId;
		}
	}

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/daily/daily_bg1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	Sprite* spTitle = ReadSpriteName(StringUtils::format("new_ui/daily/daily_txt%d.png", ptrDaily->iImgId).c_str());
	spTitle->setPosition(point + Vec2(0, 124+13));
	this->addChild(spTitle);

	//描述
	Label* labInfo = LabelSystemFont(ptrDaily->strInfo, 16);
	labInfo->setPosition(point + Vec2(0, 80));
	labInfo->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(labInfo);

	//速战提示
	Label* labSweep = LabelSystemFont(LL("dailysweepnotice"), 16); 
	labSweep->setAnchorPoint(Vec2(0.0f, 0.5f));
	labSweep->setPosition(point + Vec2(-225, -96));
	labSweep->setColor(Color3B(255, 255, 51));
	labSweep->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(labSweep);

	//剩余次数
	m_labTimes = LabelSystemFont("", 16);
	m_labTimes->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTimes->setPosition(point + Vec2(60, -96));
	m_labTimes->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(m_labTimes);

	//TableView
	auto tvSelect = DailySelectTableView::create(ptrDaily->mWarId, this);
	this->addChild(tvSelect);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//速战
	Sprite* spSweepN = ReadSpriteName("new_ui/battle/battle_sd.png");
	Sprite* spSweepS = ReadSpriteName("new_ui/battle/battle_sd.png");
	spSweepS->setScale(fScale);
	Sprite* spSweepD = ReadSpriteName("new_ui/battle/battle_sd.png");
	addSpriteGray(spSweepD);
	m_btnSweep = MenuItemSprite::create(spSweepN, spSweepS, spSweepD, CC_CALLBACK_1(DailySelectLayer::callBackSweep, this));
	spSweepS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSweepS->setPosition(Vec2(spSweepS->getContentSize().width / 2, spSweepS->getContentSize().height / 2));
	m_btnSweep->setPosition(point + Vec2(215, -100));
	ptrMenu->addChild(m_btnSweep);

	//过关开启
	m_labOpen = LabelSystemFont(LL("guoguansaodang"), 16);
	m_labOpen->setPosition(Vec2(33, 45));
	m_labOpen->setColor(Color3B::YELLOW);
	m_labOpen->enableOutline(Color4B::GRAY, 1);
	spSweepD->addChild(m_labOpen);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(DailySelectLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(235, 123));
	ptrMenu->addChild(btnClose);
}

void DailySelectLayer::updateDisplay()
{
	//剩余次数
	DailyWar* ptrDaily = trData->m_mDailyWar[m_ptrDaily->getMapId()];
	string strTimes = StringUtils::format("%s%d/%d", LL("shengyucishu"), m_ptrDaily->getTimes(), ptrDaily->iMaxTimes);
	m_labTimes->setString(strTimes);

	//速战
	if(m_iSweepId < 0 || !m_ptrDaily->getTimes())
	{
		m_btnSweep->setEnabled(false);
	}
	else
	{
		m_btnSweep->setEnabled(true);
	}

	m_labOpen->setVisible(m_iSweepId < 0);
}

void DailySelectLayer::callBackSweep(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	int iSweepNum = m_ptrDaily->getTimes();
	int iGoodsNum = 0;
	if(history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if(iGoodsNum)
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_JSD, iSweepNum, iGoodsNum));
	else
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
}

void DailySelectLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void DailySelectLayer::callBackListClick(int iIndex)
{
	if(m_ptrDaily->getTimes())
	{
		history->m_iBattleType = BATTLE_DAILY;
		history->m_iCurWarId = trData->m_mDailyWar[m_ptrDaily->getMapId()]->mWarId[iIndex];
		history->m_iBattleStage = 0;
		ChessApplication::getInstance()->AddFunctionLayer(DispatchLayer::create(this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("dailytimeout"), false);
	}
}

void DailySelectLayer::ChildCloseLayer(int iNomal)
{
	Chess::getInstance()->removeAllScreen();
	ChessApplication::getInstance()->removeAllFunctionLayer();
	if(trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0][0] == 'R')
	{
		history->game_state = GAME_STATE_REDE;
		Chess::getInstance()->appendScreen(new RedeScreen());
	}
	else
	{
		history->game_state = GAME_STATE_BATTLE;
		history->mTitleLayer = BattleTitleLayer::create();
		ChessApplication::getInstance()->AddFunctionLayer(history->mTitleLayer);
		Chess::getInstance()->appendScreen(new BattleScreen());
	}
}

void DailySelectLayer::updateRTDate(int iNomal, int iType)
{
	m_iSweepType = iNomal;
	int iSweepNum = m_ptrDaily->getTimes();
	int iGoodsNum = 0;
	history->m_iCurWarId = m_iSweepId;
	if(history->m_mConsuNum.count(SWEEP_VOLUME_ID))
		iGoodsNum = history->m_mConsuNum[SWEEP_VOLUME_ID];
	if(iType == SHORTCUT_TYPE_JSD&&iNomal == 1 && iSweepNum > iGoodsNum)
	{
		ChessApplication::getInstance()->AddFunctionLayer(ShortcutFunctionLayer::create(this, SHORTCUT_TYPE_GYSD, iSweepNum, iGoodsNum));
	}
	else if(iType == SHORTCUT_TYPE_GYSD && ((iNomal == 1 && history->m_iJade < iSweepNum - iGoodsNum) || (iNomal == 0 && history->m_iJade < 1)))
	{
		ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
	}
	else
	{
		GetNetDateTool::getInstance()->SendSweepDate(m_iSweepId, iNomal + 1, this);
	}
}

void DailySelectLayer::SendNetDateResult(bool bIsSuccess, Json::Value jsonResult, int iType)
{
	if(iType == SEND_NET_SWEEP)
	{
		if(bIsSuccess == true)
		{
			int iBattleNum = m_iSweepType > 0 ? m_ptrDaily->getTimes() : 1;

			history->mMapDateInfo->AddHeroicBattleWinDate(m_iSweepId, iBattleNum);
			m_ptrDaily->setTimes(m_ptrDaily->getTimes() - iBattleNum);

			//减去消耗
			if(history->m_mConsuNum.count(SWEEP_VOLUME_ID) > 0)
			{
				if(history->m_mConsuNum[SWEEP_VOLUME_ID] >= iBattleNum)
					history->m_mConsuNum[SWEEP_VOLUME_ID] -= iBattleNum;
				else
					history->m_mConsuNum[SWEEP_VOLUME_ID] = 0;
			}

			//添加战利品
			history->onHttpGoodsAdd(jsonResult, false, true);
			if(jsonResult["goods"].isArray())
			{
				ChessApplication::getInstance()->AddFunctionLayer(SweepTableView::create(jsonResult["goods"], trData->m_mNetWarInfo[m_iSweepId]->iGold));
			}
			updateDisplay();
		}
	}
}
