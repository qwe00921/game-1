//
//  GoodsSprite.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#include "GoodsSprite.h"
#include "GoodsLayer.h"

GoodsSprite* GoodsSprite::create(int iGoodsId, int iCount /*= 1*/, float fScale /*= 0.8f*/)
{
	GoodsSprite *pRet = new GoodsSprite();
	if(pRet && pRet->init(iGoodsId, iCount, fScale))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

GoodsSprite* GoodsSprite::create(HistoryGoods* ptrGoods, float fScale /*= 1.0f*/)
{
	GoodsSprite *pRet = new GoodsSprite();
	if(pRet && pRet->init(ptrGoods, fScale))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool GoodsSprite::init(int iGoodsId, int iCount, float fScale)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_iGoodsId = iGoodsId;
	m_iCount = iCount;
	m_ptrGoods = nullptr;
	m_fScale = fScale;
	m_bIsClick = false;
	m_bIsDrag = false;
	_callback = nullptr;

	initControl();
	this->setContentSize(m_spGoodsBG->getContentSize());

	return true;
}


bool GoodsSprite::init(HistoryGoods* ptrGoods, float fScale)
{
	if(!Sprite::init())
	{
		return false;
	}

	m_iGoodsId = ptrGoods->good_id;
	m_iCount = 1;
	m_ptrGoods = ptrGoods;
	m_fScale = fScale;
	m_bIsClick = false;
	m_bIsDrag = false;
	_callback = nullptr;
	m_labNum = NULL;

	initControl();
	this->setContentSize(m_spGoodsBG->getContentSize());
	updateSprite(ptrGoods);

	return true;
}

bool GoodsSprite::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Rect rect = m_spGoodsBG->getBoundingBox();
	Vec2 point = convertToNodeSpace(touch->getLocation());
	if(rect.containsPoint(point) && isVisible() && getParent()->isVisible())
	{
		m_posTouch = touch->getLocation();
		m_bIsClick = true;
	}
	return true;
}

void GoodsSprite::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		Rect rect = m_spGoodsBG->getBoundingBox();
		float fOffsetY = m_posTouch.y - touch->getLocation().y;
		if(!rect.containsPoint(convertToNodeSpace(touch->getLocation())) || m_bIsDrag && fabs(fOffsetY) > 5)
		{
			m_bIsClick = false;
		}
	}
}

void GoodsSprite::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		if(_callback)
		{
			_callback(this);
		}
		else
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			ChessApplication::getInstance()->AddFunctionLayer(GoodsLayer::create(m_iGoodsId));
		}
		m_bIsClick = false;
	}
}

void GoodsSprite::initControl()
{
	string strBG = m_fScale == 1.0f ? "new_ui/img_global/item_box_bg2.png" : "new_ui/img_global/item_box_bg.png";
	m_spGoodsBG = ReadSpriteName(strBG.c_str());
	Vec2 vecPos = Vec2(m_spGoodsBG->getContentSize().width / 2, m_spGoodsBG->getContentSize().height / 2);

	m_spGoodsBG->setPosition(vecPos);
	this->addChild(m_spGoodsBG);

	//物品ICON
	m_spImg = history->getGoodsSprite(m_iGoodsId);
	m_spImg->setScale(m_fScale);
	m_spImg->setPosition(vecPos);
	m_spGoodsBG->addChild(m_spImg);

	HistoryGoods hisGoods(m_iGoodsId);
	if(hisGoods.isNormalGood())
	{
		//碎片将魂
		string strChip = "";
		if(hisGoods.isSoul())
		{
			strChip = "new_ui/hero/icon_soul.png";
		}
		else if(hisGoods.isPiece())
		{
			strChip = "new_ui/img_global/icon_sp.png";
		}

		if(strChip != "")
		{
			Sprite* spChip = ReadSpriteName(strChip.c_str());
			spChip->setScale(m_fScale);
			spChip->setAnchorPoint(Vec2(0.0f, 1.0f));
			spChip->setPosition(Vec2(0, vecPos.y * 2 - 2));
			m_spGoodsBG->addChild(spChip);
		}
	}

	if (m_ptrGoods)
	{
		//边框
		m_spFrame = ReadSpriteName("new_ui/img_global/q0_1.png");
		m_spFrame->setPosition(m_spGoodsBG->getPosition());
		this->addChild(m_spFrame);

		//星级
		m_spStar = ReadSpriteName("new_ui/img_number/star_b_1.png");
		m_spStar->setPosition(m_spGoodsBG->getPosition() + Vec2(0, -21));
		this->addChild(m_spStar);
	}

	//数目
	if(m_iCount > 1)
	{
		int iSize = m_fScale == 1.0f ? 16 : 14;
		m_labNum = LabelSystemFont(IntToStringEx(m_iCount), iSize);
		m_labNum->setAnchorPoint(Vec2(1.0f, 0.0f));
		m_labNum->setPosition(Vec2((vecPos.x - 1) * 2, 0));
		m_labNum->enableOutline(Color4B::BLACK, 1);
		m_spGoodsBG->addChild(m_labNum);
	}
}

void GoodsSprite::setSupDrag(bool bIsDrag)
{
	m_bIsDrag = bIsDrag;
}

void GoodsSprite::setCallback(const ccMenuCallback& callback)
{
	_callback = callback;
}

void GoodsSprite::updateSprite(HistoryGoods* ptrGoods)
{
	m_ptrGoods = ptrGoods;

	//物品ICON
	m_spImg->setTexture(history->getGoodsTexture(ptrGoods->good_id));

	//边框星级
	if(ptrGoods->iQuality)
	{
		__String* strFrame = __String::createWithFormat("new_ui/img_global/q%d_1.png", trData->strengthen_consume[ptrGoods->iQuality - 1].quality);
		m_spFrame->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strFrame->getCString())));
		m_spFrame->setVisible(true);

		__String* strStar = __String::createWithFormat("new_ui/img_number/star_b_%d.png", ptrGoods->iQuality);
		m_spStar->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strStar->getCString())));
		m_spStar->setVisible(true);
	}
	else
	{
		m_spFrame->setVisible(false);
		m_spStar->setVisible(false);
	}
}

void GoodsSprite::updateSprite(int iGoodsId, int iCount)
{
	m_iGoodsId = iGoodsId;

	//物品ICON
	m_spImg->setTexture(history->getGoodsTexture(iGoodsId));

	m_iCount = iCount;
	if (iCount > 1)
	{
		if (!m_labNum)
		{
			int iSize = m_fScale == 1.0f ? 16 : 14;
			m_labNum = LabelSystemFont(IntToStringEx(m_iCount), iSize);
			m_labNum->setAnchorPoint(Vec2(1.0f, 0.0f));
			m_labNum->setPosition(Vec2((m_spGoodsBG->getContentSize().width / 2 - 1) * 2, 0));
			m_labNum->enableOutline(Color4B::BLACK, 1);
			m_spGoodsBG->addChild(m_labNum);
		}
		else
		{
			m_labNum->setString(IntToStringEx(m_iCount));
			m_labNum->setVisible(true);
		}
		
	}
	else if (m_labNum)
	{
		m_labNum->setVisible(false);
	}
}

void GoodsSprite::AddGoodsLayer()
{
	ChessApplication::getInstance()->AddFunctionLayer(GoodsLayer::create(m_iGoodsId));
}