#include "CityWarMapLayer.h"
#include "Defines.h"

bool CityWarInfoLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	DSize mSize = ChessViewTool::getInstance()->getFullSize();

	m_fScale = 1;

	mSCLayer = CityWarMapLayer::create();
	mSCLayer->setPosition(Vec2(0,0));
	mSCLayer->setAnchorPoint(Vec2(0,0));
	//pScrollview
	mScrollview = ScrollView::create(Size(mSize.width, mSize.height), mSCLayer);
	//取消回弹
	mScrollview->setBounceable(false);
	mScrollview->setDirection(ScrollView::Direction::BOTH);
	mScrollview->setDelegate(this);
	mScrollview->setTouchEnabled(true);
	mScrollview->setAnchorPoint(Vec2(0, 0));
	mScrollview->setPosition(Vec2(0, 0));
	this->addChild(mScrollview);


	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(CityWarInfoLayer::clickMenuBack, this));

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8), mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mMenu, 1);
	mMenu->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height / 2));


		//if(false)
	{
		Sprite* mb_nomal1 = ReadSpriteName("new_ui/battle/flippage3.png");
		Sprite* mb_select1 = ReadSpriteName("new_ui/battle/flippage3.png");
		mb_select1->setScale(0.9f);
		MenuItemSprite* mb_item1 = MenuItemSprite::create(mb_nomal1, mb_select1, CC_CALLBACK_1(CityWarInfoLayer::clickMenuShrink, this));

		Menu* mMenu1 = Menu::create();
		mMenu1->setPosition(Vec2(mSize.width - (mb_nomal1->getContentSize().width / 2 + 8), mSize.height - (mb_nomal1->getContentSize().height / 2 + 40)));
		this->addChild(mMenu1, 1);
		mMenu1->addChild(mb_item1);
		mb_select1->setAnchorPoint(Vec2(0.5, 0.5));
		mb_select1->setPosition(Vec2(mb_select1->getContentSize().width / 2, mb_select1->getContentSize().height / 2));
	}
	//if (false)
	{
		Sprite* mb_nomal1 = ReadSpriteName("new_ui/battle/flippage3.png");
		Sprite* mb_select1 = ReadSpriteName("new_ui/battle/flippage3.png");
		mb_select1->setScale(0.9f);
		MenuItemSprite* mb_item1 = MenuItemSprite::create(mb_nomal1, mb_select1, CC_CALLBACK_1(CityWarInfoLayer::clickMenuEnlarge, this));

		Menu* mMenu1 = Menu::create();
		mMenu1->setPosition(Vec2(mSize.width - (mb_nomal1->getContentSize().width / 2 + 8), mSize.height - (mb_nomal1->getContentSize().height / 2 + 80)));
		this->addChild(mMenu1, 1);
		mMenu1->addChild(mb_item1);
		mb_select1->setAnchorPoint(Vec2(0.5, 0.5));
		mb_select1->setPosition(Vec2(mb_select1->getContentSize().width / 2, mb_select1->getContentSize().height / 2));
	}

	return true;
}

bool CityWarInfoLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void CityWarInfoLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void CityWarInfoLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void CityWarInfoLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}


void CityWarInfoLayer::clickMenuShrink(Ref* sender)
{
	if (m_fScale > 0.4f)
	{
		m_fScale -= 0.1f;
		mSCLayer->setScale(m_fScale);
		Vec2 p = mScrollview->getContentOffset();
		mScrollview->setContentOffset(Vec2(p.x*m_fScale, p.y*m_fScale));
	}
}
void CityWarInfoLayer::clickMenuEnlarge(Ref* sender)
{
	if (m_fScale < 1.0f)
	{
		m_fScale += 0.1f;
		mSCLayer->setScale(m_fScale);
		Vec2 p = mScrollview->getContentOffset();
		mScrollview->setContentOffset(Vec2(p.x*m_fScale, p.y*m_fScale));
	}
}

CityWarInfoLayer::CityWarInfoLayer()
{

}
CityWarInfoLayer::~CityWarInfoLayer()
{

}



/////////////////////////////////////////////////

bool CityWarMapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//加载动画文件
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/cloud.ExportJson", 0, 1));
	this->setSwallow(false);
	this->setContentSize(Size(512 * 4, 512 * 3));

	m_fMapWidth = 0;
	m_fMapHeight = 0;

	mMapElemBg = ReadSpriteName("new_ui/hyaline/710.png");
	mMapElemBg->setPosition(Vec2(0, 0));
	mMapElemBg->setAnchorPoint(Vec2(0, 0));
	this->addChild(mMapElemBg);

	float fMapPX = 0, fMapPY = 0;
	float fMapElemW = 0, fMapElemH = 0;
	map<int, map<int, float> > mBgSize;
	for (int i = 0; i < 12; i++)
	{
		string mapES = StringUtils::format("img/dmap/map_bg%d.jpg", i + 1);
		Sprite* mapElemSp = ReadSpriteNormal(mapES.c_str());

		fMapElemW = mapElemSp->getContentSize().width;
		fMapElemH = mapElemSp->getContentSize().height;
		mapElemSp->setAnchorPoint(Vec2(0, 0));
		mMapElemBg->addChild(mapElemSp);

		mBgSize[i][0] = fMapElemW;
		mBgSize[i][1] = fMapElemH;
		if (i < 4)
			m_fMapWidth += fMapElemW;
		if (i % 4 == 0)
			m_fMapHeight += fMapElemH;

		CwMapElem* ptrCwMapElem = new CwMapElem();
		ptrCwMapElem->seteINDEX(i);
		ptrCwMapElem->seteBG(mapElemSp);
		mMapElemList[i] = ptrCwMapElem;
	}

	fMapPY = m_fMapHeight;
	for (int i = 0; i < 12; i++)
	{
		if (i % 4 == 0)
		{
			fMapPX = 0;
			fMapPY -= mBgSize[i / 3 * 4][1];
		}
		else
			fMapPX += mBgSize[i - 1][0];

		mMapElemList[i]->geteBG()->setPosition(Vec2(fMapPX, fMapPY));

		Armature* ptrArmature = NULL;
		string playNameS;
		float fArmaPointX = 0, fArmaPointY = 0;
		for (int j = 0; j < 4; j++)
		{
			fArmaPointX = j % 2 * 244 + 134 + fMapPX;
			fArmaPointY = (1 - j / 2) * 244 + 134 + fMapPY;
			playNameS = StringUtils::format("cloud%d", rand() % 2 + 1);
			ptrArmature = Armature::create("cloud");
			ptrArmature->getAnimation()->play(playNameS.c_str());
			ptrArmature->setPosition(Vec2(fArmaPointX, fArmaPointY));
			mMapElemBg->addChild(ptrArmature, 1);
			mMapElemList[i]->eCloud[j / 2][j % 2] = ptrArmature;

			int iY = i / 4 * 2 + j / 2;
			int iX = i % 4 * 2 + j % 2;

			CCLOG("(%d,%d):%d",iX,iY, trData->net_cloud_xy[iY][iX]);
			if (trData->net_cloud_xy[iY][iX])
				ptrArmature->setVisible(false);
		}
	}



	return true;
}

bool CityWarMapLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void CityWarMapLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void CityWarMapLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}

CityWarMapLayer::CityWarMapLayer()
{

}

CityWarMapLayer::~CityWarMapLayer()
{
	for (map<int, CwMapElem*>::iterator eIt = mMapElemList.begin(); eIt != mMapElemList.end(); eIt++)
	{
		delete eIt->second;
	}
}