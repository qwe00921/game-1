#include "CwArrayLayer.h"
#include "Defines.h"
#include "Net_HomePage.h"
#include "GuardLayer.h"

CwArrayLayer* CwArrayLayer::create(int iWarId, EventDelegate* ptrDelegate)
{
	CwArrayLayer* pRet = new CwArrayLayer();
	if (pRet&&pRet->init(iWarId, ptrDelegate))
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

bool CwArrayLayer::init(int iWarId, EventDelegate* ptrDelegate)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iWarId = iWarId;
	mDelegate = ptrDelegate;

	int map_index = trData->m_mNetWarInfo[iWarId]->iMapId;
	LoadSFile();

	mSize = ChessViewTool::getInstance()->getFullSize();

	__String* map_path = __String::createWithFormat("img/HM/%d.jpg", map_index);
	mBg = ReadSpriteNormal(map_path->getCString());
	mBg->setPosition(Vec2(0,0));
	mBg->setAnchorPoint(Vec2(0,0));
	this->addChild(mBg);

	m_iBgWidth = mBg->getContentSize().width;
	m_iBgHeight = mBg->getContentSize().height;

	int iPointX = 0, iPointY = 0,iElemIndex = 0,iStartX = -1,iStartY = -1;
	for (int i = 0; i < handler.pre_battle_list->size(); i++)
	{
		ScriptDispatchOwnPtr para = (ScriptDispatchOwnPtr)(handler.pre_battle_list->elementAt(i));
		iPointX = para->x * 48 + 24;
		iPointY = m_iBgHeight - (para->y * 48 + 24);

		if (!i)
		{
			iStartX = iPointX;
			iStartY = iPointY;
		}

		Sprite* lgzSp = ReadSpriteName("ui/langezi.png");
		lgzSp->setPosition(Vec2(iPointX,iPointY));
		mBg->addChild(lgzSp);

		Sprite* elem_fashion = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_fashion->setPosition(Vec2(iPointX, iPointY));
		mBg->addChild(elem_fashion, 1);
		elem_fashion->setVisible(false);

		Sprite* elem_img = ReadSpriteName("new_ui/battle/battle_48.png");
		elem_img->setPosition(Vec2(iPointX, iPointY));
		mBg->addChild(elem_img, 1);
		elem_img->setVisible(false);

		CwArrayElem* ptrCwArrayElem = new CwArrayElem();
		ptrCwArrayElem->seteBG(lgzSp);
		ptrCwArrayElem->seteMAPX(para->x);
		ptrCwArrayElem->seteMAPY(para->y);
		ptrCwArrayElem->seteACTORID(-1);
		ptrCwArrayElem->seteACTORSIMG(elem_img);
		ptrCwArrayElem->seteACTORSFASHION(elem_fashion);

		mElemList[iElemIndex++] = ptrCwArrayElem;
	}


	Sprite* mb_nomal = ReadSpriteName("new_ui/battle/flippage3.png");
	Sprite* mb_select = ReadSpriteName("new_ui/battle/flippage3.png");
	mb_select->setScale(0.9f);
	MenuItemSprite* mb_item = MenuItemSprite::create(mb_nomal, mb_select, CC_CALLBACK_1(CwArrayLayer::clickMenuBack, this));

	Menu* mMenu = Menu::create();
	mMenu->setPosition(Vec2(mSize.width - (mb_nomal->getContentSize().width / 2 + 8), mSize.height - (mb_nomal->getContentSize().height / 2 + 8)));
	this->addChild(mMenu, 1);
	mMenu->addChild(mb_item);
	mb_select->setAnchorPoint(Vec2(0.5, 0.5));
	mb_select->setPosition(Vec2(mb_select->getContentSize().width / 2, mb_select->getContentSize().height / 2));



	//居中
	if (iStartX > -1 && iStartY > -1)
	{
		int iEoffsetX = mSize.width / 2 - iStartX;
		int iEoffsetY = mSize.height / 2 - iStartY;

		if (iEoffsetX <= 0)
		{
			if (iEoffsetX < mSize.width - m_iBgWidth)
				iEoffsetX = mSize.width - m_iBgWidth;
		}
		else
			iEoffsetX = 0;

		if (iEoffsetY <= 0)
		{
			if (iEoffsetY < mSize.height - m_iBgHeight)
				iEoffsetY = mSize.height - m_iBgHeight;
		}
		else
			iEoffsetY = 0;

		mBg->setPosition(Vec2(iEoffsetX, iEoffsetY));
	}

	return true;
}

void CwArrayLayer::RunFunction(int iActorId)
{
	if (iActorId > 0&&mClickIndex>-1)
	{
		CwArrayElem* ptrCwAElem = mElemList[mClickIndex];
		ptrCwAElem->seteACTORID(iActorId);
		HistoryActorPtr ptrActor = &history->actors[iActorId - 1];

		ptrCwAElem->geteACTORSIMG()->setVisible(true);
		ptrCwAElem->geteBG()->setVisible(false);

		int fashion_width = 0;
		if (ptrActor->mFashion[0] <= 0)
		{
			ptrCwAElem->geteACTORSFASHION()->setVisible(false);
			Texture2D* tex = ITools::getInstance()->getTextureWithPath(ptrActor->getActorSImgPath("mov", 0));
			fashion_width = tex->getContentSize().width;
			ptrCwAElem->geteACTORSIMG()->setTexture(tex);
		}
		else
		{
			ptrCwAElem->geteACTORSFASHION()->setVisible(true);
			__String* mImgPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[1], ptrActor->mFashion[2]);
			__String* mFashionPath = __String::createWithFormat("img/Nunits/%d_%d_mov.png", ptrActor->mFashion[3], ptrActor->mFashion[4]);

			Texture2D* texfa = ITools::getInstance()->getTextureWithPath(mImgPath->getCString());
			ptrCwAElem->geteACTORSIMG()->setTexture(texfa);
			Texture2D* tex = ITools::getInstance()->getTextureWithPath(mFashionPath->getCString());
			fashion_width = tex->getContentSize().width;
			ptrCwAElem->geteACTORSFASHION()->setTexture(tex);
			ptrCwAElem->geteACTORSFASHION()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
			int iZOrder = 0;
			if (trData->net_hero_dress[ptrActor->mFashion[4]]->mMov[6])
				iZOrder = 2;
			ptrCwAElem->geteACTORSFASHION()->setLocalZOrder(iZOrder);
			fashion_width = texfa->getContentSize().width;
		}
		ptrCwAElem->geteACTORSIMG()->setTextureRect(Rect(0, 6 * fashion_width, fashion_width, fashion_width));
	}
}

void CwArrayLayer::clickMenuBack(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	//ChessApplication::getInstance()->AddFunctionLayer(Net_HomePage::create(false));
	this->removeFromParent();
}

CwArrayLayer::CwArrayLayer()
{

}
CwArrayLayer::~CwArrayLayer()
{
	for (map<int, CwArrayElem*>::iterator eIt = mElemList.begin(); eIt != mElemList.end(); eIt++)
	{
		delete eIt->second;
	}
}

void CwArrayLayer::onEnter()
{
	Layer::onEnter();
}

void CwArrayLayer::onExit()
{
	Layer::onExit();
}

bool CwArrayLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	mClickIndex = -1;
	for (size_t i = 0; i < mElemList.size(); i++)
	{
		Rect lgzSpBox = mElemList[i]->geteBG()->getBoundingBox();
		//TODO
		if (Rect(0,0, lgzSpBox.size.width, lgzSpBox.size.height).containsPoint(mElemList[i]->geteBG()->convertToNodeSpace(touch->getLocation())))
		{
			mClickIndex = i;
			return true;
		}
	}

	return true;
}
void CwArrayLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	float fOffsetX = mBg->getPositionX() + touch->getDelta().x;

	if (fOffsetX <= 0 && fOffsetX >= mSize.width - m_iBgWidth)
	{
		mBg->setPositionX(fOffsetX);
	}

	float fOffsetY = mBg->getPositionY() + touch->getDelta().y;

	if (fOffsetY <= 0 && fOffsetY >= mSize.height - m_iBgHeight)
	{
		mBg->setPositionY(fOffsetY);
	}
}

void CwArrayLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	int iOffsetX = abs((int)(touch->getLocation().x - touch->getStartLocation().x));
	int iOffsetY = abs((int)(touch->getLocation().y - touch->getStartLocation().y));
	if (mClickIndex > -1 && iOffsetX<3 && iOffsetY<3)
	{
		SelectGuradActorTableView* ptrSGATBV = SelectGuradActorTableView::create(GetActorIdList(), m_iWarId, this,ARRAY_SELECT_ACTOR);
		if (ptrSGATBV)
			ChessApplication::getInstance()->AddFunctionLayer(ptrSGATBV);
		else
			ITools::getInstance()->createGlobalPrompt(LL("mqmyshdwjzs"), false);
	}
}

map<int, int> CwArrayLayer::GetActorIdList()
{
	map<int, int> mRetMap;
	int iMapIndex = 0;
	for (size_t i = 0; i < mElemList.size(); i++)
	{
		if (mElemList[i]->geteACTORID() > 0)
		{
			mRetMap[iMapIndex++] = mElemList[i]->geteACTORID();
		}
	}
	return mRetMap;
}

void CwArrayLayer::LoadSFile()
{
	string strSFile = trData->m_mNetWarInfo[m_iWarId]->mstrScene.rbegin()->second;
	handler.GetDispatchInfo(strSFile);
}