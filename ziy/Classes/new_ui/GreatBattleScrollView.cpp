#include "GreatBattleScrollview.h"
#include "Defines.h"
#include "GoodsSprite.h"
#include "GreatBattleLayer.h"

GreatBattleScrollview* GreatBattleScrollview::create(EventDelegate* ptrDelegate)
{
	GreatBattleScrollview* pRet = new GreatBattleScrollview();
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

bool GreatBattleScrollview::init(EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iCurPage = 0;
	m_bEnableMove = true;
	mDelegate = ptrDelegate;
	InitDateList();
	mViewWidth = 530;
	mViewHeight = 306;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	mScrollView = ScrollView::create(Size(mViewWidth, mViewHeight), NULL);

	mScrollView->setDelegate(this);

	mScrollView->setContentSize(Size(mViewWidth * m_iMaxPage, mViewHeight));

	mScrollView->setContentOffset(Vec2::ZERO);

	mScrollView->setDirection(ScrollView::Direction::HORIZONTAL);

	mScrollView->setPosition(Vec2((600 - mViewWidth) / 2, 19));

	mScrollView->setAnchorPoint(Vec2(0,0));

	mScrollView->setBounceable(true);

	mScrollView->setTouchEnabled(false);

	this->addChild(mScrollView);

	mRightArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mRightArrowSp->setPosition(Vec2((600 - mViewWidth) / 2 -11, 172));
	this->addChild(mRightArrowSp);

	mLeftArrowSp = ReadSpriteName("new_ui/img_global/turn_page.png");
	mLeftArrowSp->setFlippedX(true);
	mLeftArrowSp->setPosition((600 - mViewWidth) / 2 + mViewWidth +11, 172);
	this->addChild(mLeftArrowSp);

	GreatBattleScrollviewLayer* ptrRSVLayer = NULL;

	for (int i = 0; i < m_iMaxPage; i++)
	{
		ptrRSVLayer = GreatBattleScrollviewLayer::create(mElemData[i], mDelegate);
		ptrRSVLayer->setPosition(Vec2(mViewWidth*i, 0));
		ptrRSVLayer->setAnchorPoint(Vec2(0, 0));
		mScrollView->addChild(ptrRSVLayer);
	}

	setArroeState();
	return true;
}

bool GreatBattleScrollview::onTouchBegan(Touch *touch, Event *unused_event)
{
	//playSound(SOUND_LEFT_MOUSE_CLICK);
	m_bEnableMove = true;
	m_vStarPoint = touch->getLocation();
	return true;
}

void GreatBattleScrollview::onTouchMoved(Touch *touch, Event *unused_event)
{
	float fDir = touch->getLocation().x - m_vStarPoint.x;
	if (m_bEnableMove && (fDir > 70.0f && m_iCurPage || fDir< -70.0f && m_iCurPage < m_iMaxPage-1))
	{
		if (fDir > 0)
			m_iCurPage--;
		else
			m_iCurPage++;
		mScrollView->setContentOffsetInDuration(Vec2(-mViewWidth * m_iCurPage, 0), 0.3f);
		m_bEnableMove = false;
		setArroeState();
	}
}

void GreatBattleScrollview::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_bEnableMove = true;
}

void GreatBattleScrollview::InitDateList()
{
	mElemData.clear();
	int iIndex = 0;
	for (map<int, NetZmzyMap*>::iterator eIt = trData->net_zmzy_map.begin(); eIt != trData->net_zmzy_map.end(); eIt++)
	{
		mElemData[iIndex++] = eIt->first;
	}
	m_iMaxPage = mElemData.size();
}


void GreatBattleScrollview::setArroeState()
{
	if (!m_iCurPage)
		mRightArrowSp->setVisible(false);
	else
		mRightArrowSp->setVisible(true);
	if (m_iCurPage >= m_iMaxPage-1)
		mLeftArrowSp->setVisible(false);
	else
		mLeftArrowSp->setVisible(true);
}

GreatBattleScrollview::GreatBattleScrollview()
{

}

GreatBattleScrollview::~GreatBattleScrollview()
{

}

void GreatBattleScrollview::scrollViewDidScroll(ScrollView* view)
{

}

void GreatBattleScrollview::scrollViewDidZoom(ScrollView* view)
{

}


//GreatBattleScrollviewLayer

GreatBattleScrollviewLayer* GreatBattleScrollviewLayer::create(int iGreatId,EventDelegate* ptrDelegate)
{
	GreatBattleScrollviewLayer* pRet = new GreatBattleScrollviewLayer();
	if (pRet&&pRet->init(iGreatId,ptrDelegate))
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

bool GreatBattleScrollviewLayer::init(int iGreatId,EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}
	this->setSwallow(false);
	mDelegate = ptrDelegate;
	m_iGreatId = iGreatId;

	NetZmzyMap* ptrNetZmzyMap = trData->net_zmzy_map[iGreatId];

	Sprite* bg = ReadSpriteName(StringUtils::format("new_ui/zmzy/zmzy_img%d.png", ptrNetZmzyMap->bgimg).c_str());
	bg->setPosition(Vec2(265,153));
	this->addChild(bg);

	//说明
	Label* infoLab = LabelSystemFont(ptrNetZmzyMap->explain.c_str(),16);
	infoLab->setDimensions(378,66);
	infoLab->setLineHeight(22);
	infoLab->enableOutline(Color4B::BLACK, 2);
	infoLab->setPosition(Vec2(18,138));
	infoLab->setAnchorPoint(Vec2(0,1));
	bg->addChild(infoLab);

	int iPointX = 18, iPointY = 20;
	for (size_t i = 0; i < ptrNetZmzyMap->itemid.size(); i++)
	{
		GoodsSprite* ptrGoodsSprite = GoodsSprite::create(ptrNetZmzyMap->itemid[i], 1);
		ptrGoodsSprite->setAnchorPoint(Vec2(0,0));
		ptrGoodsSprite->setPosition(Vec2(iPointX+i*54,iPointY));
		//ptrGoodsSprite->setSupDrag(true);
		bg->addChild(ptrGoodsSprite);
	}

	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(456,66));
	bg->addChild(ptrMenu);

	Sprite* ptrNomalSp = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	Sprite* ptrSelectSp = ReadSpriteName("new_ui/zmzy/zmzy_ban1.png");
	ptrSelectSp->setScale(0.9f);

	MenuItemSprite* ptrAttackItemM = MenuItemSprite::create(ptrNomalSp, ptrSelectSp,CC_CALLBACK_1(GreatBattleScrollviewLayer::ClickAttackMenu,this));
	ptrMenu->addChild(ptrAttackItemM);
	ptrSelectSp->setAnchorPoint(Vec2(0.5f,0.5f));
	ptrSelectSp->setPosition(Vec2(ptrSelectSp->getContentSize().width/2, ptrSelectSp->getContentSize().height/2));

	return true;
}

void GreatBattleScrollviewLayer::ClickAttackMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GreatBattleSelectLayer::create(m_iGreatId,mDelegate));
}

bool GreatBattleScrollviewLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void GreatBattleScrollviewLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void GreatBattleScrollviewLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

GreatBattleScrollviewLayer::GreatBattleScrollviewLayer()
{

}
GreatBattleScrollviewLayer::~GreatBattleScrollviewLayer()
{

}