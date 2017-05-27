//
//  DailySceneLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/08/18.
//
//

#include "DailySelectLayer.h"
#include "WolfCityLayer.h"
#include "GoodsShopLayer.h"
#include "XmtWarLayer.h"

#define DAILY_VIEW_W 600
#define DAILY_VIEW_H 310
#define DAILY_CELL_W 200
#define DAILY_CELL_H 310

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DailySceneCell::init()
{
	if(!TableViewCell::init())
	{
		return false;
	}

	m_spScene = NULL;

	initControl();
	return true;
}

void DailySceneCell::updateCell(DailyDate* ptrDaily)
{
	int iImgId = trData->m_mDailyWar[ptrDaily->getMapId()]->iImgId;
	string strPath = StringUtils::format("new_ui/daily/daily_gk%d.png", iImgId);

	if (m_spScene)
	{
		m_spScene->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
	}
	else
	{
		m_spScene = ReadSpriteName(strPath.c_str());
		m_spScene->setPosition(Vec2(DAILY_CELL_W / 2, DAILY_CELL_H / 2));
		this->addChild(m_spScene);
	}

	if(ptrDaily->getIsOpen() == false) //未开启加灰
	{
		addSpriteGray(m_spScene);
	}
	else
	{
		removeSpriteGray(m_spScene);
	}
}

void DailySceneCell::initControl()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DailySceneTableView* DailySceneTableView::create(map<int, DailyDate*> mDaily)
{
	DailySceneTableView* pRet = new DailySceneTableView();
	if(pRet&&pRet->init(mDaily))
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

bool DailySceneTableView::init(map<int, DailyDate*> mDaily)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iClickIndex = -1;
	m_spLArrow = NULL;
	m_spRArrow = NULL;
	m_mDaily = mDaily;

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

	updateArrow();
	return true;
}

DailySceneTableView::DailySceneTableView()
{

}

DailySceneTableView::~DailySceneTableView()
{

}

void DailySceneTableView::scrollViewDidScroll(ScrollView* view)
{
	updateArrow();
}

void DailySceneTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if (m_iClickIndex < 0)
	{
		DailySceneCell* ptrCell = (DailySceneCell*)cell;
		m_iClickIndex = ptrCell->getIdx();
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(DailySceneTableView::showSelectLayer, this));
		ptrCell->m_spScene->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
}

Size DailySceneTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(DAILY_CELL_W, DAILY_CELL_H);
}

TableViewCell* DailySceneTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	DailySceneCell* cell = NULL;
	if(ptrCell)
	{
		cell = (DailySceneCell*)ptrCell;
	}
	else
	{
		cell = DailySceneCell::create();
	}
	cell->updateCell(m_mDaily[idx]);
	return cell;
}

ssize_t DailySceneTableView::numberOfCellsInTableView(TableView *table)
{
	return m_mDaily.size();
}

void DailySceneTableView::showSelectLayer()
{
	if (m_iClickIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		if(m_mDaily[m_iClickIndex]->getIsOpen())
		{
			if (m_mDaily[m_iClickIndex]->getMapId() == MAPID_SCENE_WOLF_CITY)
			{
				ChessApplication::getInstance()->AddFunctionLayer(WolfCityLayer::create());
			}
			else if (m_mDaily[m_iClickIndex]->getMapId() == MAPID_SCENE_IMAGE)
			{
				if(history->mMapDateInfo->GetBattleImageData()->GetAllBattleData().size()|| history->mMapDateInfo->GetBattleImageData()->GetStageNum() == 1)
					ChessApplication::getInstance()->AddFunctionLayer(XmtWarLayer::create());
				else
					ChessApplication::getInstance()->AddFunctionLayer(XmtWarTableView::create());
			}
			else
			{
				ChessApplication::getInstance()->AddFunctionLayer(DailySelectLayer::create(m_mDaily[m_iClickIndex]));
			}
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("dailynotopen"), false);
		}
		m_iClickIndex = -1;
	}
}

void DailySceneTableView::updateArrow()
{
	if(m_spLArrow)
	{
		Vec2 point = m_pTableView->getContentOffset();
		m_spLArrow->setVisible(point.x < 0);
		m_spRArrow->setVisible(point.x - DAILY_VIEW_W + m_mDaily.size() * DAILY_CELL_W > 0);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DailySceneLayer::DailySceneLayer()
{

}

DailySceneLayer::~DailySceneLayer()
{
	for(UInt i = 0; i < m_mDaily.size(); i++)
	{
		SAFE_DELETE(m_mDaily[i]);
	}
}

DailySceneLayer* DailySceneLayer::create(int iIsGLCBattled)
{
	DailySceneLayer* pRet = new DailySceneLayer();
	if (pRet&&pRet->init(iIsGLCBattled))
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

bool DailySceneLayer::init(int iIsGLCBattled)
{
	if(!Layer::init())
	{
		return false;
	}
	//高狼城战斗结束特殊处理
	m_iIsGLC = iIsGLCBattled;
	initControl();
	initDailyDate();
	onHttpRequest();

	return true;
}

bool DailySceneLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void DailySceneLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void DailySceneLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void DailySceneLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu, 1);

	//返回
	Sprite* spCloseN = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/battle/flippage3.png");
	spCloseS->setScale(fScale);
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(DailySceneLayer::callBackClose, this));
	spCloseS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spCloseS->setPosition(Vec2(spCloseN->getContentSize().width / 2, spCloseN->getContentSize().height / 2));
	btnClose->setPosition(size.width - 32, size.height - 32);
	ptrMenu->addChild(btnClose);

	//兑换
	Sprite* spExchN = ReadSpriteName("new_ui/img_global/global_ph_icon6.png");
	Sprite* spExchS = ReadSpriteName("new_ui/img_global/global_ph_icon6.png");
	spExchS->setScale(fScale);
	MenuItemSprite* btnExch = MenuItemSprite::create(spExchN, spExchS, CC_CALLBACK_1(DailySceneLayer::callBackExch, this));
	spExchS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spExchS->setPosition(Vec2(spExchN->getContentSize().width / 2, spExchN->getContentSize().height / 2));
	btnExch->setPosition(size.width - 32, size.height - 92);
	ptrMenu->addChild(btnExch);
}

void DailySceneLayer::initDailyDate()
{
	for(map<int, DailyWar*>::iterator it = trData->m_mDailyWar.begin(); it != trData->m_mDailyWar.end(); it++)
	{
		DailyWar* ptrWar = it->second;
		DailyDate* ptrDaily = new DailyDate();
		ptrDaily->setMapId(ptrWar->iMapId);
		ptrDaily->setWarId(ptrWar->mWarId[0]);
		ptrDaily->setTimes(0);
		ptrDaily->setIsOpen(false);
		m_mDaily[ptrWar->iId - 1] = ptrDaily;
	}
}

string DailySceneLayer::getWarIdData()
{
	Json::Value valWarList(arrayValue);
	for(UInt i = 0; i < m_mDaily.size(); i++)
	{
		Json::Int valWarId = m_mDaily[i]->getWarId();
		valWarList.append(valWarId);
	}

	string strRes;
	FastWriter fastWriter;
	strRes = fastWriter.write(valWarList);
	strRes.erase(strRes.end() - 1); //去掉末尾的'\n'
	return strRes;
}

void DailySceneLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void DailySceneLayer::onHttpRequest()
{
	string strWarId = getWarIdData();
	string strToken = history->m_pLoginInfo->getGameToken();
	string strData = StringUtils::format("gameToken=%s&warIDs=%s", strToken.c_str(), strWarId.c_str());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData);
	string data = StringUtils::format("{\"gameToken\":\"%s\",\"warIDs\":%s,\"validateCode\":\"%s\"}", strToken.c_str(), strWarId.c_str(), validateCode.c_str());
	CCLOG("%s", data.c_str());

	ITools::getInstance()->connectingNetwork("isOpenGroup",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(DailySceneLayer::onHttpRequestCompleted),
		data,
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void DailySceneLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string reseData(buffer->begin(), buffer->end());
	CCLOG("%s...", reseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(reseData, value) && value.size() > 0)
	{
		onDailySucceed(value["result"]);
	}
}

bool DailySceneLayer::GetBattleState(int iMapId)
{
	bool iRet = false;
	if (iMapId == MAPID_SCENE_WOLF_CITY&&history->ownArmyNum >= 8)
		iRet = true;
	if (iMapId == MAPID_SCENE_IMAGE)
	{
		int i60LvActor = 0;
		for (int i = 0; i < history->ownArmyNum; i++)
		{
			if (history->actors[history->ownArmy[i]].GetActorLv() >= 60)
			{
				i60LvActor++;
			}

			if (i60LvActor >= 6)
			{
				iRet = true;
				break;
			}
		}
	}

	return iRet;
}

bool DailySceneLayer::onDailySucceed(Json::Value valRes)
{
	Json::Value arrData = valRes["arr"];
	if(arrData.isArray())
	{
		for(UInt i = 0; i < arrData.size(); i++)
		{
			int iWarId = arrData[i]["warID"].asInt();
			int iTimes = arrData[i]["num"].asInt();
			bool bIsOpen = arrData[i]["isOpen"].asBool();

			for(UInt j = 0; j < m_mDaily.size(); j++)
			{
				if(m_mDaily[j]->getWarId() == iWarId)
				{
					int iMaxTimes = trData->m_mDailyWar[m_mDaily[j]->getMapId()]->iMaxTimes;
					m_mDaily[j]->setTimes(iMaxTimes - iTimes);
					m_mDaily[j]->setIsOpen(bIsOpen);
					break;
				}
			}
		}

		for (UInt j = 0; j < m_mDaily.size(); j++)
		{
			//高狼城本地判断是否开启
			if (m_mDaily[j]->getMapId() == MAPID_SCENE_WOLF_CITY || m_mDaily[j]->getMapId() == MAPID_SCENE_IMAGE)
			{
				m_mDaily[j]->setIsOpen(GetBattleState(m_mDaily[j]->getMapId()));
			}
		}

		map<int, DailyDate*> mTempData;
		int iIndex = 0;
		for (UInt i = 0; i < m_mDaily.size(); i++)
		{
			if (m_mDaily[i]->getIsOpen())
			{
				mTempData[iIndex++] = m_mDaily[i];
			}
		}
		for(UInt i = 0; i < m_mDaily.size(); i++)
		{
			if(!m_mDaily[i]->getIsOpen())
			{
				mTempData[iIndex++] = m_mDaily[i];
			}
		}
		m_mDaily = mTempData;

		auto tvDaily = DailySceneTableView::create(m_mDaily);
		this->addChild(tvDaily);

		//加载高狼城数据
		history->mMapDateInfo->GetBattleWolfData()->LoadJsonValue(valRes);
		//加载心魔塔数据
		history->mMapDateInfo->GetBattleImageData()->LoadJsonValue(valRes);

		if (m_iIsGLC == 1)
		{
			ChessApplication::getInstance()->AddFunctionLayer(WolfCityLayer::create());
			m_iIsGLC = 0;
		}
		else if (m_iIsGLC == 2)
		{
			ChessApplication::getInstance()->AddFunctionLayer(XmtWarLayer::create(-1,history->m_iBattleStage));
			m_iIsGLC = 0;
		}
	}

	return true;
}

void DailySceneLayer::callBackExch(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoodsExchLayer::create());
}
