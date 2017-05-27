//
//  ExpFruitLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/5/29.
//
//

#include "ExpFruitLayer.h"
#include "Defines.h"
#include "GoodsDealLayer.h"
#include "SkillAndGoodsInfoLayer.h"
#include "CocoMd5.h"
#include "PopLayer.h"

#define COLUMN_NUM 5

#define EXP_VIEW_W 530
#define EXP_VIEW_H 220
#define EXP_ITEM_W 106
#define EXP_ITEM_H 110

//ExpFruitSprite////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ExpFruitSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_bIsSelect = false;
	m_pAddHero = NULL;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ExpFruitSprite::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ExpFruitSprite::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ExpFruitSprite::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	initControl();
	return true;
}

void ExpFruitSprite::initControl()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));

	//Cell背景
	m_spCellBG = ReadSpriteName("new_ui/hero/herobox_bg1.png");
	this->addChild(m_spCellBG, 0);

	Size szBG = m_spCellBG->getContentSize();

	//职业
	m_spJob = ReadSpriteName("new_ui/hero/job1.png");
	m_spJob->setAnchorPoint(Vec2(0, 1));
	m_spJob->setPosition(Vec2(4, szBG.height - 5));
	m_spJob->setScale(0.65f);
	m_spCellBG->addChild(m_spJob, 6);

	//等级
	m_labLv = LabelSystemFont("", 16);
	m_labLv->setAnchorPoint(Vec2(1, 1));
	m_labLv->setPosition(Vec2(83, 83));
	m_labLv->setColor(Color3B::YELLOW);
	m_labLv->enableOutline(Color4B::BLACK, 1);
	m_spCellBG->addChild(m_labLv, 6);

	//时装武器
	m_spFashion1 = ReadSpriteName("new_ui/battle/battle_48.png");
	m_spFashion1->setPosition(Vec2(szBG.width / 2, 57));
	m_spCellBG->addChild(m_spFashion1, 1);

	//时装人物
	m_spFashion2 = ReadSpriteName("new_ui/battle/battle_48.png");
	m_spFashion2->setPosition(Vec2(szBG.width / 2, 57));
	m_spCellBG->addChild(m_spFashion2, 1);

	//武将名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setPosition(Vec2(szBG.width / 2, 21));
	m_labName->enableOutline(Color4B::BLACK, 1);
	m_spCellBG->addChild(m_labName, 6);

	//武将品质
	m_spQuality = ReadSpriteName("new_ui/hero/herobox_q1.png");
	m_spQuality->setPosition(Vec2(szBG.width / 2, szBG.height / 2));
	m_spCellBG->addChild(m_spQuality, 4);


	//经验条
	Sprite* spExpBG = ReadSpriteName("new_ui/hero/herobox_jiangh1.png");
	spExpBG->setPosition(Vec2(szBG.width / 2, 33));
	m_spCellBG->addChild(spExpBG, 3);

	m_spExpBar = ReadSpriteName("new_ui/hero/herobox_jiangh3.png");
	m_spExpBar->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spExpBar->setPosition(Vec2(6, spExpBG->getContentSize().height / 2));
	spExpBG->addChild(m_spExpBar, 3);

	//经验已满
	m_spExpFull = ReadSpriteName("new_ui/hero/hero_exp_txt3.png");
	m_spExpFull->setPosition(Vec2(szBG.width / 2, 45));
	m_spCellBG->addChild(m_spExpFull, 7);
}

void ExpFruitSprite::updateSprite(AddExpHero* ptrAddHero, EventDelegate* ptrDelegate)
{
	if(ptrAddHero == NULL)
	{
		setVisible(false);
		return;
	}
	else
	{
		setVisible(true);
	}

	m_pAddHero = ptrAddHero;
	m_pDelegate = ptrDelegate;
	HistoryActor* ptrActor = &history->actors[ptrAddHero->getHeroId() - 1];

	//职业
	string strJob = StringUtils::format("new_ui/hero/job%d.png", ptrActor->GetActorJobImgId());
	m_spJob->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strJob.c_str())));

	//等级
	m_labLv->setString(StringUtils::format("%d", ptrActor->GetActorLv()));

	//形象
	int fashion_width = 48;
	if(ptrActor->mFashion[0] <= 0)
	{
		m_spFashion1->setVisible(false);
		Texture2D* texfa = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
		m_spFashion2->setTexture(texfa);
		fashion_width = texfa->getContentSize().width;
	}
	else
	{
		m_spFashion1->setVisible(true);
		string mImgPath = StringUtils::format("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
		string mFashionPath = StringUtils::format("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

		Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath);
		m_spFashion2->setTexture(texfa);
		Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath);
		fashion_width = tex->getContentSize().width;
		m_spFashion1->setTexture(tex);
		m_spFashion1->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
		int iZOrder = 0;
		if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
			iZOrder = 2;
		m_spFashion1->setLocalZOrder(iZOrder);
		fashion_width = texfa->getContentSize().width;
	}
	m_spFashion2->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));

	//武将名字
	m_labName->setString(trData->actors[ptrActor->actor_id].getActorName());

	//武将品质
	string strQuality = StringUtils::format("new_ui/hero/herobox_q%d.png", ptrActor->actor_quality);
	m_spQuality->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strQuality.c_str())));

	//经验条
	updateLvState();
}

void ExpFruitSprite::updateExpBar(float fDelay)
{
	if (m_pAddHero->getHeroId()==1)
	{
		ITools::getInstance()->createGlobalPrompt(LL("wufashiyongjingyanguo"), false);
		return;
	}
	history->actors[0].GetActorLv();
	if (history->actors[m_pAddHero->getHeroId()-1].GetActorLv() >= history->actors[0].GetActorLv())
	{
		ITools::getInstance()->createGlobalPrompt(LL("dayuzhujiedengjiwufashiyong"), false);
		return;
	}
	ExpFruitTableView::ms_iSelectIndex = -1;
	if(m_spExpFull->isVisible())
	{
		this->unschedule(schedule_selector(ExpFruitSprite::updateExpBar));
		HistoryActor* ptrActor = &history->actors[m_pAddHero->getHeroId() - 1];
		string strNotice = LL("expfruitnotice1");
		if(ptrActor->actor_quality >= (int)trData->net_hero_grown[m_pAddHero->getHeroId()].size() - 1)
		{
			strNotice = LL("expfruitnotice2");
		}
		ITools::getInstance()->createGlobalPrompt(strNotice.c_str(), false);
		return;
	}
	if(ExpFruitLayer::ms_iTotalNum <= 0)
	{
		this->unschedule(schedule_selector(ExpFruitSprite::updateExpBar));

		HistoryGoods hisGoods(ExpFruitLayer::ms_iGoodsId);
		__String* strNotice = __String::createWithFormat(LL("shuliangbuzu"), hisGoods.GetGoodsName());
		ITools::getInstance()->createGlobalPrompt(strNotice->getCString(), false);
		return;
	}

	int iAddNum = m_pAddHero->getAddNum() + 1;
	m_pAddHero->setAddNum(iAddNum);
	ExpFruitLayer::ms_iTotalNum--;

	if(updateLvState())
	{
		Armature* armLvUp = Armature::create("icon_eff");
		armLvUp->setPosition(Vec2(m_spCellBG->getContentSize().width / 2, m_spCellBG->getContentSize().height / 2));
		armLvUp->getAnimation()->play("war_tx2");
		armLvUp->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
		{
			if(type == COMPLETE)
			{
				armature->removeFromParent();
			}
		});
		m_spCellBG->addChild(armLvUp, 9);
	}
	
	m_pDelegate->updateShow(0);
}

bool ExpFruitSprite::updateLvState()
{
	int iAddExp = m_pAddHero->getAddVal() * m_pAddHero->getAddNum();
	HistoryActor* ptrActor = &history->actors[m_pAddHero->getHeroId() - 1];
	int iMaxLv = ptrActor->actor_quality * LEVELGROWTH;
	if (ptrActor->GetSoulBox())
		iMaxLv = 100;

	int iLastLv = 0;
	int iLastExp = 0;
	int iActorLv = 0;
	int iActorExp = 0;
	getAddLv(m_pAddHero->getAddNum() - 1, iLastLv, iLastExp);
	getAddLv(m_pAddHero->getAddNum(), iActorLv, iActorExp);

	bool bLevelUp = iActorLv > iLastLv ? true : false;
	if(bLevelUp)
	{
		m_labLv->setString(StringUtils::format("%d", iActorLv));
	}

	int iMaxExp = trData->net_hero_exp[iActorLv]->mExp;
	m_spExpBar->setScaleX(float(iActorExp) / iMaxExp);
	if(iActorLv == iMaxLv && iActorExp == iMaxExp)
	{
		m_spExpFull->setVisible(true);
	}
	else
	{
		m_spExpFull->setVisible(false);
	}

	return bLevelUp;
}

void ExpFruitSprite::getAddLv(int iCount, int& iLv, int& iExp)
{
	int iAddExp = m_pAddHero->getAddVal() * iCount;
	HistoryActor* ptrActor = &history->actors[m_pAddHero->getHeroId() - 1];
	iLv = ptrActor->GetActorLv();
	iExp = ptrActor->exp + iAddExp;

	while(iExp >= trData->net_hero_exp[iLv]->mExp)
	{
		if((iLv < ptrActor->actor_quality * LEVELGROWTH)||(ptrActor->GetSoulBox()&&iLv<100))
		{
			iExp -= trData->net_hero_exp[iLv++]->mExp;
		}
		else
		{
			iExp = trData->net_hero_exp[iLv]->mExp;
			break;
		}
	}
}

void ExpFruitSprite::onEnter()
{
	Sprite::onEnter();
}

void ExpFruitSprite::onExit()
{
	Sprite::onExit();
}

bool ExpFruitSprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Rect rect = m_spCellBG->getBoundingBox();
	if(rect.containsPoint(convertToNodeSpace(touch->getLocation())) && isVisible())
	{
		ExpFruitTableView::ms_iSelectIndex = m_pAddHero->getIndex();
		this->schedule(schedule_selector(ExpFruitSprite::updateExpBar), 0.05f);
		m_bIsSelect = true;
	}
	return true;
}

void ExpFruitSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsSelect)
	{
		Rect rect = m_spCellBG->getBoundingBox();
		if(!rect.containsPoint(convertToNodeSpace(touch->getLocation())) || abs(touch->getStartLocation().y - touch->getLocation().y) > 10)
		{
			ExpFruitTableView::ms_iSelectIndex = -1;
			this->unschedule(schedule_selector(ExpFruitSprite::updateExpBar));
			m_bIsSelect = false;
		}
	}
}

void ExpFruitSprite::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_bIsSelect)
	{
		this->unschedule(schedule_selector(ExpFruitSprite::updateExpBar));
		m_bIsSelect = false;
	}
}

//ExpFruitCell////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExpFruitCell* ExpFruitCell::create(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero)
{
	ExpFruitCell* pRet = new ExpFruitCell();
	if(pRet&&pRet->init(ptrDelegate, mAddHero))
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

bool ExpFruitCell::init(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero)
{
	if(!TableViewCell::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;

	initControl();
	updateCell(mAddHero);
	return true;
}

ExpFruitCell::ExpFruitCell()
{

}

bool ExpFruitCell::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}

void ExpFruitCell::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void ExpFruitCell::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void ExpFruitCell::onEnter()
{
	TableViewCell::onEnter();
}
void ExpFruitCell::onExit()
{
	TableViewCell::onExit();
}

void ExpFruitCell::initControl()
{
	for(int i = 0; i < COLUMN_NUM; i++)
	{
		m_mAddHero[i] = ExpFruitSprite::create();
		m_mAddHero[i]->setPosition(Vec2(EXP_ITEM_W / 2 + EXP_ITEM_W * i, EXP_ITEM_H / 2));
		this->addChild(m_mAddHero[i]);
	}
}

void ExpFruitCell::updateCell(map<int, AddExpHero*> mAddHero)
{
	for(int i = 0; i < COLUMN_NUM; i++)
	{
		m_mAddHero[i]->updateSprite(mAddHero[i], m_pDelegate);
	}
}

void ExpFruitCell::updateSprite(int iIndex)
{
	m_mAddHero[iIndex]->updateExpBar(0.0f);
}

//ExpFruitTableView////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ExpFruitTableView::ms_iSelectIndex = -1;

ExpFruitTableView* ExpFruitTableView::create(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero)
{
	ExpFruitTableView* pRet = new ExpFruitTableView();
	if(pRet&&pRet->init(ptrDelegate, mAddHero))
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

bool ExpFruitTableView::init(EventDelegate* ptrDelegate, map<int, AddExpHero*> mAddHero)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	m_mAddHero = mAddHero;

	//创建一个talbleview 将datasource设置为当前的类的对象
	m_pTableView = TableView::create(this, Size(EXP_VIEW_W, EXP_VIEW_H));
	//设置tableview为垂直方向  ScrollView::Direction::VERTICAL 为垂直,ScrollView::Direction::HORIZONTAL 为水平
	m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
	//设置位置
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	m_pTableView->setPosition(Vec2(size.width / 2 - EXP_VIEW_W / 2, size.height / 2 - EXP_VIEW_H / 2 - 10));
	//设置代理对象
	m_pTableView->setDelegate(this);
//GHB	m_pTableView->setTouchPriority(LAYER_SECOND);
	// 填充顺序
	m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//添加tableview到当前layer
	this->addChild(m_pTableView);

	return true;
}

ExpFruitTableView::ExpFruitTableView() : BTLayer(false)
{
	m_iSelectIndex = -1;
	m_pDelegate = NULL;
}

ExpFruitTableView::~ExpFruitTableView()
{

}

void ExpFruitTableView::scrollViewDidScroll(ScrollView* view)
{

}

void ExpFruitTableView::scrollViewDidZoom(ScrollView* view)
{

}

void ExpFruitTableView::tableCellTouched(TableView* table, TableViewCell* cell)
{
	if(ms_iSelectIndex >= 0)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ExpFruitCell* ptrCell = (ExpFruitCell*)cell;
		ptrCell->updateSprite(ms_iSelectIndex % COLUMN_NUM);
	}
}

void ExpFruitTableView::tableCellHighlight(TableView* table, TableViewCell* cell)
{

};

void ExpFruitTableView::tableCellUnhighlight(TableView* table, TableViewCell* cell)
{

};

Size ExpFruitTableView::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(EXP_ITEM_W, EXP_ITEM_H);
}

TableViewCell* ExpFruitTableView::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *ptrCell = table->dequeueCell();
	m_mCell[idx] = NULL;

	map<int, AddExpHero*> mAddHero;
	for(int i = 0; i < COLUMN_NUM; i++)
	{
		mAddHero[i] = m_mAddHero[i + idx * COLUMN_NUM];
	}
	if(ptrCell)
	{
		m_mCell[idx] = (ExpFruitCell*)ptrCell;
		m_mCell[idx]->updateCell(mAddHero);
	}
	else
	{
		m_mCell[idx] = ExpFruitCell::create(m_pDelegate, mAddHero);
	}
	return m_mCell[idx];
}

ssize_t ExpFruitTableView::numberOfCellsInTableView(TableView *table)
{
	return (m_mAddHero.size() - 1) / COLUMN_NUM + 1;
}

void ExpFruitTableView::updateTableView(map<int, AddExpHero*> mShopGoods)
{
	m_mAddHero = mShopGoods;
	m_pTableView->reloadData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ExpFruitLayer::ms_iTotalNum = 0; 
int ExpFruitLayer::ms_iGoodsId = 0;

ExpFruitLayer* ExpFruitLayer::create(int iGoodsId, EventDelegate* ptrDelegate)
{
	ExpFruitLayer* pRet = new ExpFruitLayer();
	if(pRet&&pRet->init(iGoodsId, ptrDelegate))
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

bool ExpFruitLayer::init(int iGoodsId, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pDelegate = ptrDelegate;
	HistoryGoods hisGoods(iGoodsId);
	ms_iTotalNum = history->m_mConsuNum[iGoodsId];
	ms_iGoodsId = iGoodsId;

	for(int i = 0; i < history->ownArmyNum; i++)
	{
		m_mAddHero[i] = new AddExpHero();
		m_mAddHero[i]->setIndex(i);
		m_mAddHero[i]->setHeroId(history->ownArmy[i] + 1);
		m_mAddHero[i]->setAddVal(hisGoods.GetGoodsPower());
		m_mAddHero[i]->setAddNum(0);
	}

	initControl();
	return true;
}


void ExpFruitLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* spBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	spBlack->setPosition(Vec2(0, 0));
	this->addChild(spBlack);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/hero/hero_exp_bg.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	HistoryGoods hisGoods(ms_iGoodsId);
	m_labTitle = LabelSystemFont(StringUtils::format(LL("expfruittitle"), hisGoods.GetGoodsName(), ms_iTotalNum), 18);
	m_labTitle->setColor(Color3B(153, 51, 0));
	m_labTitle->setPosition(point + Vec2(0, 150));
	this->addChild(m_labTitle);

	//按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(ExpFruitLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(290, 160));
	m_pBtnMenu->addChild(btnClose);

	//武将TableView
	this->addChild(ExpFruitTableView::create(this, m_mAddHero));
}

bool ExpFruitLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	return true;
}

void ExpFruitLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void ExpFruitLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

ExpFruitLayer::ExpFruitLayer()
{

}
ExpFruitLayer::~ExpFruitLayer()
{
	for(UInt i = 0; i < m_mAddHero.size(); i++)
	{
		SAFE_DELETE(m_mAddHero[i]);
	}
}

void ExpFruitLayer::onEnter()
{
	Layer::onEnter();
}

void ExpFruitLayer::onExit()
{
	Layer::onExit();
}

void ExpFruitLayer::updateShow(int index)
{
	HistoryGoods hisGoods(ms_iGoodsId);
	m_labTitle->setString(StringUtils::format(LL("expfruittitle"), hisGoods.GetGoodsName(), ms_iTotalNum));
}

void ExpFruitLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (ms_iTotalNum < history->m_mConsuNum[ms_iGoodsId])
	{
		ExpAddHttpRequest();
	}
	else
	{
		removeFromParent();
	}
}

void ExpFruitLayer::ExpAddHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	int iId = history->m_mConsuNetId[ms_iGoodsId];
	string strActor = getActorListData();
	strActor.erase(strActor.end() - 1);
	__String* data = __String::createWithFormat("id=%d&arr=%s&gameToken=%s", iId, strActor.c_str(), strToken.c_str());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"id\":%d,\"arr\":%s,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iId, strActor.c_str(), strToken.c_str(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("addEXP",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(ExpFruitLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void ExpFruitLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onExpAddSucceed(value["result"]);
	}
}

string ExpFruitLayer::getActorListData()
{
	Json::Value valActorList(arrayValue);
	for(UInt i = 0; i < m_mAddHero.size(); i++)
	{
		if(m_mAddHero[i]->getAddNum())
		{
			Json::Value valActor;
			valActor["hid"] = m_mAddHero[i]->getHeroId();
			valActor["num"] = m_mAddHero[i]->getAddNum();
			valActorList.append(valActor);
		}
	}

	FastWriter fastWriter;
	return fastWriter.write(valActorList);
}

void ExpFruitLayer::onExpAddSucceed(Json::Value valRes)
{
	if(valRes["minus"].isInt())
	{
		int iMinus = valRes["minus"].asInt();
		history->m_mConsuNum[ms_iGoodsId] -= iMinus;
	}

	map<int, HeroElem*> mHeroList;
	Json::Value heroList = valRes["heros"];
	if(heroList.isArray())
	{
		for(UInt i = 0; i < heroList.size(); i++)
		{
			HeroElem* ptrHero = new HeroElem();
			ptrHero->loadJson(heroList[i]);
			mHeroList[ptrHero->getHeroID()] = ptrHero;

			//更新武将数据
			HistoryActor* ptrActor = &history->actors[ptrHero->getHeroID() - 1];
			if (ptrHero->getLV() > ptrActor->GetActorLv()) //升级刷新五围属性
			{
				ptrActor->SetActorLv(ptrHero->getLV());
				ptrActor->InitFiveAllAbility();
				ptrActor->UpdateSkills();
			}
			ptrActor->exp = ptrHero->getEXP();
		}
	}

	m_pDelegate->updateShow(0);
	this->removeFromParent();
}
