//
//  MailLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/07.
//
//


#include "MailLayer.h"
#include "MailInfoLayer.h"
#include "GuideLayer.h"
#include "Net_HomePage.h"

#define MAIL_VIEW_W 275
#define MAIL_VIEW_H 285
#define MAIL_CELL_W 275
#define MAIL_CELL_H 82

//MailCell////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MailCell::init()
{
	if(!TableViewCell::init())
	{
		return false;
	}

	initControl();
	return true;
}

void MailCell::initControl()
{
	Vec2 point = Vec2(MAIL_CELL_W / 2, MAIL_CELL_H / 2);
	//背景
	m_spBGN = ReadSpriteName("new_ui/img_global/mail_bg2.png");
	m_spBGN->setPosition(point);
	this->addChild(m_spBGN);

	//邮件ICON
	m_spIcon = ReadSpriteName("new_ui/img_global/icon_mail1.png");
	m_spIcon->setPosition(Vec2(42, 40));
	m_spBGN->addChild(m_spIcon);

	//标题
	m_labTitle = LabelSystemFont(LL("systemmail"), 14);
	m_labTitle->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTitle->setPosition(Vec2(75, 61));
	m_labTitle->setColor(Color3B(102, 0, 0));
	m_spBGN->addChild(m_labTitle);

	//类型
	m_labType = LabelSystemFont(LL("systemmail"), 13);
	m_labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labType->setPosition(Vec2(75, 40));
	m_labType->setColor(Color3B(255, 102, 0));
	m_spBGN->addChild(m_labType);

	//时间
	m_labTime = LabelSystemFont("2016-00-00", 13);
	m_labTime->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labTime->setPosition(Vec2(75, 22));
	m_labTime->setColor(Color3B(255, 102, 0));
	m_spBGN->addChild(m_labTime);
}

void MailCell::updateCell(MailElem* ptrMail)
{
	string strBGPath = "new_ui/img_global/mail_bg2.png";
	string strIconPath = "new_ui/img_global/icon_mail1.png";
	Color3B ccTitle = Color3B(102, 0, 0);
	Color3B ccTime = Color3B(255, 102, 0);

	if (ptrMail->getIsRead())
	{
		strBGPath = "new_ui/img_global/mail_bg3.png";
		strIconPath = "new_ui/img_global/icon_mail2.png";
		ccTitle = Color3B::GRAY;
		ccTime = Color3B::GRAY;
	}

	m_spBGN->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strBGPath.c_str())));
	m_spIcon->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strIconPath.c_str())));
	m_labTitle->setString(ptrMail->getTitle().c_str());
	m_labTitle->setColor(ccTitle);
	m_labType->setColor(ccTime);
	m_labTime->setString(ptrMail->getTime().c_str());
	m_labTime->setColor(ccTime);
}

//MailTableView////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MailTableView* MailTableView::create(map<int, MailElem*> mMailMap, EventDelegate* ptrDelegate)
{
	MailTableView* pRet = new MailTableView();
	if(pRet&&pRet->init(mMailMap, ptrDelegate))
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

bool MailTableView::init(map<int, MailElem*> mMailMap, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_mMailMap = mMailMap;
	m_iClickIndex = -1;

	//创建一个talbleview 将datasource设置为当前的类的对象 tableview的显示区域大小为 275 * 296
	m_pTableView = TableView::create(this, Size(MAIL_VIEW_W, MAIL_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - MAIL_VIEW_W / 2, size.height / 2 - MAIL_VIEW_H / 2 - 11));
	//设置代理对象
	m_pTableView->setDelegate(this);
//GHB	m_pTableView->setTouchPriority(LAYER_BOTTOM);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);
	return true;
}

MailTableView::MailTableView() : BTLayer(false)
{

}

MailTableView::~MailTableView()
{

}

void MailTableView::scrollViewDidScroll(ScrollView* view)
{

}

void MailTableView::scrollViewDidZoom(ScrollView* view)
{

}

void MailTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if (m_iClickIndex < 0)
	{
		MailCell* ptrCell = (MailCell*)cell;
		m_iClickIndex = ptrCell->getIdx();
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(MailTableView::showMailInfo, this));
		ptrCell->m_spBGN->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
	}
}

void MailTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void MailTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size MailTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(MAIL_CELL_W, MAIL_CELL_H);
}

TableViewCell* MailTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	MailCell* cell = NULL;
	if (ptrCell)
	{
		cell = (MailCell*)ptrCell;
		cell->updateCell(m_mMailMap[idx]);
	}
	else
	{
		cell = MailCell::create();
		cell->updateCell(m_mMailMap[idx]);
	}
	return cell;
}

ssize_t MailTableView::numberOfCellsInTableView(TableView *table)
{
	return m_mMailMap.size();
}

void MailTableView::showMailInfo()
{
	if(m_iClickIndex > -1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		this->getParent()->addChild(MailInfoLayer::create(m_mMailMap[m_iClickIndex], m_pDelegate));
		m_iClickIndex = -1;
	}
}

void MailTableView::updateMailShow(map<int, MailElem*> mMailMap)
{
	m_mMailMap = mMailMap;
	m_pTableView->reloadData();
}

//MailLayer////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MailLayer::MailLayer()
{

}

MailLayer::~MailLayer()
{
	for(UInt i = 0; i < m_mMailMap.size(); i++)
	{
		SAFE_DELETE(m_mMailMap[i]);
	}
}

MailLayer* MailLayer::create(EventDelegate* ptrDelegate)
{
	MailLayer* pRet = new MailLayer();
	if (pRet&&pRet->init(ptrDelegate))
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

bool MailLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrDel = ptrDelegate;
	m_homeClass = NULL;
	initControl();
	mailHttpRequest();
	return true;
}

void MailLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* spBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	spBlack->setPosition(Vec2(0, 0));
	this->addChild(spBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/mail_bg1.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(MailLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(165, 165-9));
	m_pBtnMenu->addChild(btnClose);
}

void MailLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	history->updateGuide(EMIL_GUIDE);
	if (m_homeClass != NULL)
	{
		m_homeClass->playAnimation("city_102", 2);
	}
	this->removeFromParent();
	HongDian::getInstence()->autoUpdate();
	
}

void MailLayer::updateShow(int iIndex)
{
	updateMailMap();
	m_tvMail->updateMailShow(m_mMailMap);
	m_ptrDel->updateState();
	
}

void MailLayer::mailHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("getAllEmail",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(MailLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void MailLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		onMailSucceed(value["result"]["emails"]);
	}
}

void MailLayer::updateMailMap()
{
	map<int, MailElem*> mMailMap = m_mMailMap;
	m_mMailMap.clear();

	UInt iIndex = 0;
	for(UInt i = 0; i < mMailMap.size(); i++)
	{
		if(!mMailMap[i]->getIsRead())
		{
			m_mMailMap[iIndex++] = mMailMap[i];
		}
	}

	for(UInt i = 0; i < mMailMap.size(); i++)
	{
		if(mMailMap[i]->getIsRead() && mMailMap[i]->getType() == MAIL_MSG)
		{
			m_mMailMap[iIndex++] = mMailMap[i];
		}
	}
}

bool MailLayer::onMailSucceed(Json::Value valRes)
{
	history->m_iMailNum = 0;
	for(UInt i = 0; i < valRes.size(); i++)
	{
		MailElem* ptrMail = new MailElem();
		ptrMail->setMailId(valRes[i]["id"].asString());
		ptrMail->setTitle(valRes[i]["title"].asString());
		ptrMail->setContent(valRes[i]["content"].asString());
		ptrMail->setSender(valRes[i]["sendName"].asString());

		DString strDate = valRes[i]["sendDate"].asCString();
		int iIndex = strDate->indexOf('T');
		strDate = strDate->substring(0, iIndex);
		ptrMail->setTime(strDate->getChar());

		int iIsRead = valRes[i]["statu"].asInt() == 1 ? 0 : 1;
		ptrMail->setIsRead(iIsRead);
		if (!iIsRead)
		{
			history->m_iMailNum++;
		}

		Json::Value valGoods = valRes[i]["goods"];
		if (valGoods.isNull() || !valGoods.size())
		{
			ptrMail->setType(MAIL_MSG);
		}
		else
		{
			ptrMail->setType(MAIL_GOODS);
			for(UInt j = 0; j < valGoods.size(); j++)
			{
				int iGoodsId = valGoods[j]["goodID"].asInt();
				int iCount = valGoods[j]["count"].asInt();
				ptrMail->m_mGoodsMap[iGoodsId] = iCount;
			}
		}
		m_mMailMap[i] = ptrMail;
	}

	updateMailMap();

	m_tvMail = MailTableView::create(m_mMailMap, this);
	this->addChild(m_tvMail);
	history->updateGuide(EMIL_GUIDE);
	return true;
}
