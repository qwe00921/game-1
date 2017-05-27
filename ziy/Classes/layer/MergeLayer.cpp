//
//  MergeLayer.h
//  TRGame
//
//  Created by Jimmy on 16/09/09.
//
//

#include "MergeLayer.h"
#include "PopLayer.h"
#include "GoldExchangeLayer.h"
#include "GoodsLayer.h"
#include "GoodsDealLayer.h"
#include "NetPromptTool.h"
#include "GuideLayer.h"

#define MAX_MERGE_TYPE 9
#define MAX_ITEM_NUM 16
#define MATERIAL_NUM 5
#define SELECT_NUM 12
#define SELECT_STORE_NUM 99

enum
{
	SELECT_MERGE = 0, //合成选择
	SELECT_ENCHANT = 1 //附魔选择
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MaterialSprite* MaterialSprite::create()
{
	MaterialSprite *pRet = new MaterialSprite();
	if(pRet && pRet->init())
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

bool MaterialSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	m_ptrGoods = nullptr;
	m_iGoodsId = 0;
	m_bIsClick = false;
	m_bIsFinished = false;
	_callback = nullptr;

	initControl();
	this->setContentSize(m_spGoodsBG->getContentSize());

	return true;
}

bool MaterialSprite::onTouchBegan(cocos2d::CCTouch *touch, cocos2d::Event *unused_event)
{
	Rect rect = m_spGoodsBG->getBoundingBox();
	if(rect.containsPoint(convertToNodeSpace(touch->getLocation())) && isVisible())
	{
		m_bIsClick = true;
	}
	return true;
}

void MaterialSprite::onTouchMoved(cocos2d::CCTouch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		Rect rect = m_spGoodsBG->getBoundingBox();
		if(!rect.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			m_bIsClick = false;
		}
	}
}

void MaterialSprite::onTouchEnded(cocos2d::CCTouch *touch, cocos2d::Event *unused_event)
{
	if(m_bIsClick)
	{
		if(_callback)
		{
			_callback(this);
		}
		m_bIsClick = false;
	}
}

void MaterialSprite::updateSprite(int iGoodsId, int iCount)
{
	m_ptrGoods = nullptr;
	m_spIcon->setTexture(history->getGoodsTexture(iGoodsId));
	m_iGoodsId = iGoodsId;
	m_iCount = iCount;

	m_spFrame->setVisible(false);
	m_spStar->setVisible(false);

	HistoryGoods hisGoods(iGoodsId);
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
		m_spChip->setTexture(ReadSpriteTexture(strChip.c_str()));
		m_spChip->setVisible(true);
	}
	else
	{
		m_spChip->setVisible(false);
	}

	int iStore = hisGoods.getStoreNum();
	if(hisGoods.isEquip())
	{
		m_iCount = 1;
		m_spPlus->setVisible(true);
		iStore = 0;
	}
	else
	{
		m_spPlus->setVisible(false);
	}

	m_bIsFinished = iStore >= iCount;
	m_labStore->setString(StringUtils::format("%d", iStore));
	m_labStore->setColor(m_bIsFinished ? Color3B::GREEN : Color3B::RED);
	m_labUsed->setString(StringUtils::format("/%d", iCount));

	int iOffsetX = (m_labStore->getContentSize().width - m_labUsed->getContentSize().width) / 2;
	m_labStore->setPosition(Vec2(25 + iOffsetX, -10));
	m_labUsed->setPosition(Vec2(25 + iOffsetX, -10));
}

void MaterialSprite::updateSprite(HistoryGoods* ptrGoods)
{
	m_ptrGoods = ptrGoods;
	m_bIsFinished = true;

	m_spPlus->setVisible(false);

	//边框星级
	if(ptrGoods->iQuality)
	{
		__String* strFrame = __String::createWithFormat("new_ui/img_global/q%d.png", trData->strengthen_consume[ptrGoods->iQuality - 1].quality);
		m_spFrame->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strFrame->getCString())));
		m_spFrame->setVisible(true);

		__String* strStar = __String::createWithFormat("new_ui/img_number/star_%d.png", ptrGoods->iQuality);
		m_spStar->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strStar->getCString())));
		m_spStar->setVisible(true);
	}
	else
	{
		m_spFrame->setVisible(false);
		m_spStar->setVisible(false);
	}

	m_labStore->setString(StringUtils::format("%d", 1));
	m_labStore->setColor(Color3B::GREEN);

	int iOffsetX = (m_labStore->getContentSize().width - m_labUsed->getContentSize().width) / 2;
	m_labStore->setPosition(Vec2(25 + iOffsetX, -10));
	m_labUsed->setPosition(Vec2(25 + iOffsetX, -10));
}

void MaterialSprite::setCallback(const ccMenuCallback& callback)
{
	_callback = callback;
}

void MaterialSprite::removeMaterial()
{
	history->packGoodsReduce(m_iGoodsId, getNetId(), m_iCount);
}

bool MaterialSprite::isFinished()
{
	return m_bIsFinished;
}

int MaterialSprite::getStoneNum()
{
	int iCount = 0;
	if (m_ptrGoods && m_ptrGoods->iQuality)
	{
		iCount = trData->strengthen_consume[m_ptrGoods->iQuality - 1].iTotalGem;
	}
	return iCount;
}

int MaterialSprite::getNetId()
{
	int iNetId = 0;
	if(m_ptrGoods)
	{
		iNetId = m_ptrGoods->getNetID();
	}
	else
	{
		HistoryGoods hisGoods(m_iGoodsId);
		if (hisGoods.isSoul() || hisGoods.isPiece())
		{
			iNetId = history->mChipSoulDate->getNetIdByGoodsId(m_iGoodsId);
		}
		else
		{
			iNetId = history->m_mConsuNetId[m_iGoodsId];
		}
	}

	return iNetId;
}

void MaterialSprite::initControl()
{
	m_spGoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg.png");
	m_spGoodsBG->setPosition(Vec2(m_spGoodsBG->getContentSize().width / 2, m_spGoodsBG->getContentSize().height / 2));
	m_spGoodsBG->setOpacity(0);
	this->addChild(m_spGoodsBG);

	//物品ICON
	m_spIcon = ReadSpriteNormal("img/Items/point.png");
	m_spIcon->setScale(0.80f);
	m_spIcon->setPosition(Vec2(25, 25));
	m_spGoodsBG->addChild(m_spIcon);

	//碎片
	m_spChip = ReadSpriteName("new_ui/hero/icon_soul.png");
	m_spChip->setScale(0.8f);
	m_spChip->setPosition(Vec2(10, 40));
	m_spGoodsBG->addChild(m_spChip);

	//边框
	m_spFrame = ReadSpriteName("new_ui/img_global/q0.png");
	m_spFrame->setScale(0.95f);
	m_spFrame->setPosition(Vec2(25, 25));
	m_spFrame->setVisible(false);
	this->addChild(m_spFrame);

	//星级
	m_spStar = ReadSpriteName("new_ui/img_number/star_1.png");
	m_spStar->setPosition(Vec2(25, 7));
	m_spStar->setVisible(false);
	this->addChild(m_spStar);

	//库存
	m_labStore = LabelSystemFont("", 16);
	m_labStore->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labStore->enableOutline(Color4B::BLACK, 1);
	m_spGoodsBG->addChild(m_labStore);

	//消耗
	m_labUsed = LabelSystemFont("", 16);
	m_labUsed->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labUsed->enableOutline(Color4B::BLACK, 1);
	m_spGoodsBG->addChild(m_labUsed);

	//加号
	m_spPlus = ReadSpriteName("new_ui/hero/hero_add.png");
	m_spPlus->setPosition(Vec2(25, 25));
	m_spGoodsBG->addChild(m_spPlus);
	m_spPlus->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MergeTypeLayer* MergeTypeLayer::create(EventDelegate* ptrDelegate)
{
	MergeTypeLayer *pRet = new MergeTypeLayer();
	if(pRet && pRet->init(ptrDelegate))
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

bool MergeTypeLayer::init(EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrDelegate = ptrDelegate;
	m_iClickIndex = -1;

	initControl();
	initGoodsMap();

	return true;
}

MergeTypeLayer::MergeTypeLayer()
{

}

MergeTypeLayer::~MergeTypeLayer()
{

}

bool MergeTypeLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	for(UInt i = 0; i < m_mRect.size(); i++)
	{
		if(m_mRect[i].containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			m_iClickIndex = i;
		}
	}

	return true;
}

void MergeTypeLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_iClickIndex >= 0 && !m_mRect[m_iClickIndex].containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		m_iClickIndex = -1;
	}
}

void MergeTypeLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_iClickIndex >= 0)
	{
		int iIndex = (m_iCurPage - 1) * MAX_MERGE_TYPE + m_iClickIndex;
		if(m_mGoodsMap.count(iIndex))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			m_ptrDelegate->updateShow(m_mGoodsMap[iIndex]->getTag());
			m_iClickIndex = -1;
			this->removeFromParent();
		}
	}
	else
	{
		Rect rectBG = m_spLayBG->getBoundingBox();
		if(!rectBG.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			this->removeFromParent();
		}
	}
}

void MergeTypeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景框
	m_spLayBG = ReadSpriteName("new_ui/img_global/make_bg3.png");
	m_spLayBG->setPosition(point+Vec2(0,22));
	this->addChild(m_spLayBG);

	//按钮菜单
	Menu* btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(MergeTypeLayer::callBackLast, this));
	spLastS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLastS->setPosition(Vec2(spLastS->getContentSize().width / 2, spLastS->getContentSize().height / 2));
	btnLast->setPosition(point + Vec2(-155, 0));
	btnMenu->addChild(btnLast);

	//下一页
	Sprite* spNextN = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextN->setFlippedX(true);
	Sprite* spNextS = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextS->setFlippedX(true);
	spNextS->setScale(fScale);
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(MergeTypeLayer::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(155, 0));
	btnMenu->addChild(btnNext);

}

void MergeTypeLayer::initGoodsMap()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	map<int, NetMergeType*> mMergeType = trData->m_mMergeType;
	m_iCurPage = 1;
	m_iMaxPage = (mMergeType.size() - 1) / MAX_MERGE_TYPE + 1;

	int iOffsetX = (m_iMaxPage - 1) * 18 / 2;
	for(int i = 0; i < m_iMaxPage; i++)
	{
		Sprite* spPage = ReadSpriteName("new_ui/img_global/suit_bnt3.png");
		spPage->setPosition(point + Vec2(-iOffsetX + i * 18, -168));
		this->addChild(spPage);
		m_mSpPage[i] = spPage;
	}

	int iIndex = 0;
	for(map<int, NetMergeType*>::iterator it = mMergeType.begin(); it != mMergeType.end(); it++)
	{
		NetMergeType* ptrType = it->second;

		//底框
		Sprite* spItemBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		Vec2 vecPos = point + Vec2((iIndex % MAX_MERGE_TYPE % 3 * 88 - 88)*0.8,( -iIndex % MAX_MERGE_TYPE / 3 * 96 + 96)*0.8);
		spItemBG->setPosition(vecPos);
		spItemBG->setTag(it->first);
		this->addChild(spItemBG);

		//ICON
		Sprite* spImg = ReadSpriteNormal(StringUtils::format("img/Items/%d.png", ptrType->iImgId));
		spImg->setPosition(Vec2(33, 33));
		spItemBG->addChild(spImg);

		//类型名
		Label* labType = LabelSystemFont(ptrType->strName, 16);
		labType->setPosition(Vec2(33, 8));
		labType->enableOutline(Color4B::BLACK, 1);
		spItemBG->addChild(labType);

		if(iIndex < MAX_MERGE_TYPE)
		{
			m_mRect[iIndex] = spItemBG->getBoundingBox();
		}
		spItemBG->setScale(0.8f);
		m_mGoodsMap[iIndex++] = spItemBG;
	}
	updateDisplay();
}

void MergeTypeLayer::updateDisplay()
{
	for(UInt i = 0; i < m_mGoodsMap.size(); i++)
	{
		m_mGoodsMap[i]->setVisible((i / MAX_MERGE_TYPE + 1) == m_iCurPage);
	}

	for(UInt i = 0; i < m_mSpPage.size(); i++)
	{
		string strPath = "new_ui/img_global/suit_bnt3.png";
		if(i + 1 == m_iCurPage)
		{
			strPath = "new_ui/img_global/suit_bnt2.png";
		}
		m_mSpPage[i]->setTexture(ReadSpriteTexture(strPath.c_str()));
	}
}

void MergeTypeLayer::callBackLast(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iCurPage > 1)
	{
		m_iCurPage--;
	}
	else if(m_iCurPage == 1 && m_iMaxPage > 1)
	{
		m_iCurPage = m_iMaxPage;
	}
	updateDisplay();
}

void MergeTypeLayer::callBackNext(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iCurPage < m_iMaxPage)
	{
		m_iCurPage++;
	}
	else if(m_iCurPage == m_iMaxPage)
	{
		m_iCurPage = 1;
	}
	updateDisplay();
}

MergeSelectLayer::MergeSelectLayer()
{

}

MergeSelectLayer::~MergeSelectLayer()
{
	if(m_iSelType == SELECT_ENCHANT)
	{
		for (UInt i = 0; i < m_mPtrGoods.size(); i++)
		{
			SAFE_DELETE(m_mPtrGoods[i]);
		}
		m_mPtrGoods.clear();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MergeSelectLayer* MergeSelectLayer::create(int iGoodsId, MaterialSprite* spMaterial, EventDelegate* ptrDelegate)
{
	MergeSelectLayer *pRet = new MergeSelectLayer();
	if(pRet && pRet->init(iGoodsId, spMaterial, ptrDelegate))
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

bool MergeSelectLayer::init(int iGoodsId, MaterialSprite* spMaterial, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iGoodsId = iGoodsId;
	m_spMaterial = spMaterial;
	m_pDelegate = ptrDelegate;
	m_iSelectIndex = -1;
	m_iSelType = spMaterial ? SELECT_MERGE : SELECT_ENCHANT;

	initControl();
	initGoodsMap();
	return true;
}

void MergeSelectLayer::updateState()
{
	if (m_iSelType == SELECT_MERGE)
	{
		m_spMaterial->updateSprite(m_mPtrGoods[m_iSelectIndex]);
		m_iSelectIndex = -1;
		m_pDelegate->updateState();
	}
	else if (m_iSelType == SELECT_ENCHANT)
	{
		m_pDelegate->updateShow(m_mPtrGoods[m_iSelectIndex]->good_id);
	}
	this->removeFromParent();
}

void MergeSelectLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	HistoryGoods hisGoods(m_iGoodsId);

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景框
	auto spLayBG = ReadSpriteName("new_ui/img_global/make_bg4.png");
	spLayBG->setPosition(point);
	this->addChild(spLayBG);

	//标题
	string strTitle = "";
	if (m_iSelType == SELECT_MERGE)
	{
		strTitle = StringUtils::format("%s%s", LL("select"), hisGoods.GetGoodsName());
	}
	else if(m_iSelType == SELECT_ENCHANT)
	{
		strTitle = LL("selectmaterial");
	}
	Label* labTitle = LabelSystemFont(strTitle, 20);
	labTitle->setPosition(point + Vec2(0, 127));
	labTitle->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(labTitle);

	for(int i = 0; i < SELECT_NUM; i++)
	{
		//底框
		auto spItem = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		m_mPostion[i] = point + Vec2(i % 4 * 76 - 114, -i / 4 * 76 + 70);
		spItem->setPosition(m_mPostion[i]);
		this->addChild(spItem);
	}

	//按钮菜单
	Menu* btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(MergeSelectLayer::callBackLast, this));
	spLastS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLastS->setPosition(Vec2(spLastS->getContentSize().width / 2, spLastS->getContentSize().height / 2));
	btnLast->setPosition(point + Vec2(-172, -6));
	btnMenu->addChild(btnLast);

	//下一页
	Sprite* spNextN = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextN->setFlippedX(true);
	Sprite* spNextS = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextS->setFlippedX(true);
	spNextS->setScale(fScale);
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(MergeSelectLayer::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(172, -6));
	btnMenu->addChild(btnNext);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(MergeSelectLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(185, 130));
	btnMenu->addChild(btnClose);
}

void MergeSelectLayer::initGoodsMap()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	int iIndex = 0;
	if(m_iSelType == SELECT_MERGE)
	{
		for(int i = 0; i < history->goods->size(); i++)
		{
			HistoryGoods* ptrGoods = history->goods->elementAt(i);
			if(ptrGoods->good_id == m_iGoodsId)
			{
				m_mPtrGoods[iIndex++] = ptrGoods;
			}
		}
	}
	else if (m_iSelType == SELECT_ENCHANT)
	{
		HistoryGoods hisGoods(m_iGoodsId);
		map<int, int> mStoneId = hisGoods.getFmGoodsMap();
		for (UInt i = 0; i < mStoneId.size(); i++)
		{
			HistoryGoods* ptrGoods = new HistoryGoods(mStoneId[i]);
			m_mPtrGoods[iIndex++] = ptrGoods;
		}
	}

	m_iCurPage = 1;
	m_iMaxPage = (m_mPtrGoods.size() - 1) / SELECT_NUM + 1;

	//物品
	for(int i = 0; i < SELECT_NUM; i++)
	{
		if(m_mPtrGoods.count(i))
		{
			auto spGoods = GoodsSprite::create(m_mPtrGoods[i]);
			spGoods->setPosition(m_mPostion[i]);
			this->addChild(spGoods);
			spGoods->setCallback(CC_CALLBACK_1(MergeSelectLayer::callBackSelect, this));

			//数目
			if(m_iSelType == SELECT_ENCHANT)
			{
				Label* labNum = LabelSystemFont(StringUtils::format("%d", m_mPtrGoods[i]->getStoreNum()), 16);
				labNum->setAnchorPoint(Vec2(1.0f, 0.0f));
				labNum->setPosition(Vec2(spGoods->getContentSize().width - 2, 0));
				labNum->enableOutline(Color4B::BLACK, 1);
				labNum->setTag(SELECT_STORE_NUM);
				spGoods->addChild(labNum);
			}
			m_mSpGoods[i] = spGoods;
		}
	}

	//页码
	int iOffsetX = (m_iMaxPage - 1) * 18 / 2;
	for(int i = 0; i < m_iMaxPage; i++)
	{
		Sprite* spPage = ReadSpriteName("new_ui/img_global/suit_bnt3.png");
		spPage->setPosition(point + Vec2(-iOffsetX + i * 18, -130));
		this->addChild(spPage);
		m_mSpPage[i] = spPage;
	}

	updateDisplay();
}

void MergeSelectLayer::updateDisplay()
{
	//物品
	for(int i = 0; i < SELECT_NUM; i++)
	{
		int iIndex = i + (m_iCurPage - 1) * SELECT_NUM;
		if(m_mPtrGoods.count(iIndex))
		{
			m_mSpGoods[i]->updateSprite(m_mPtrGoods[iIndex]);
			if(m_iSelType == SELECT_ENCHANT)
			{
				Label* labNum = (Label*)(m_mSpGoods[i]->getChildByTag(SELECT_STORE_NUM));
				labNum->setString(StringUtils::format("%d", m_mPtrGoods[i]->getStoreNum()));
			}
			m_mSpGoods[i]->setTag(iIndex);
			m_mSpGoods[i]->setVisible(true);
		}
		else
		{
			if(m_mSpGoods.count(i))
			{
				m_mSpGoods[i]->setVisible(false);
			}
		}
	}

	//页码
	for(UInt i = 0; i < m_mSpPage.size(); i++)
	{
		string strPath = "new_ui/img_global/suit_bnt3.png";
		if(i + 1 == m_iCurPage)
		{
			strPath = "new_ui/img_global/suit_bnt2.png";
		}
		m_mSpPage[i]->setTexture(ReadSpriteTexture(strPath.c_str()));
	}
}


void MergeSelectLayer::callBackLast(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iCurPage > 1)
	{
		m_iCurPage--;
	}
	else if(m_iCurPage == 1 && m_iMaxPage > 1)
	{
		m_iCurPage = m_iMaxPage;
	}
	updateDisplay();
}

void MergeSelectLayer::callBackNext(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_iCurPage < m_iMaxPage)
	{
		m_iCurPage++;
	}
	else if(m_iCurPage == m_iMaxPage)
	{
		m_iCurPage = 1;
	}
	updateDisplay();
}

void MergeSelectLayer::callBackSelect(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	GoodsSprite* spGoods = (GoodsSprite*)sender;
	m_iSelectIndex = spGoods->getTag();
	int iDealType = m_iSelType == SELECT_MERGE ? DEAL_MAKE : DEAL_ENCHANT;
	ChessApplication::getInstance()->AddFunctionLayer(GoodsDealLayer::create(m_mPtrGoods[m_iSelectIndex], this, iDealType));
}

void MergeSelectLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MergeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_iClickIndex = -1;
	m_iGoodsId = 0;
	m_iNoticeIndex = 0;

	initControl();
	updateShow(2);

    return true;
}

void MergeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景框
	Sprite* spBG = ReadSpriteName("new_ui/img_global/global_bg.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题文字
	Sprite* spTitle = ReadSpriteName("new_ui/img_text/bt4_1.png");
	spTitle->setPosition(Vec2(spBG->getContentSize().width / 2, 362+20));
	spBG->addChild(spTitle);

	//类型背景
	Sprite* spTypeBG = ReadSpriteName("new_ui/img_global/make_bg2.png");
	spTypeBG->setPosition(point + Vec2(-172, 138));
	this->addChild(spTypeBG);

	//类型
	m_labType = LabelSystemFont("", 16);
	m_labType->setAnchorPoint(Vec2(0, 0.5f));
	m_labType->setPosition(Vec2(30, 13));
	m_labType->enableOutline(Color4B::BLACK, 1);
	spTypeBG->addChild(m_labType);

	//装备栏背景
	Sprite* spLBG = ReadSpriteName("new_ui/img_global/hero_bg2.png");
	spLBG->setPosition(point + Vec2(-154, -12));
	this->addChild(spLBG);

	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		//底框
		auto spItem = ReadSpriteName("new_ui/img_global/item_box_bg.png");
		spItem->setPosition(point + Vec2(i % 4 * 58 - 241, -i / 4 * 58 + 75));
		this->addChild(spItem);

		//ICON
		auto spGoods = ReadSpriteNormal("img/Items/point.png");
		spGoods->setScale(0.80f);
		spGoods->setPosition(Vec2(25, 25));
		spItem->addChild(spGoods);

		m_mSpItemBG[i] = spItem;
		m_mSpGoods[i] = spGoods;
	}

	//材料背景
	Sprite* spRBG = ReadSpriteName("new_ui/img_global/make_bg1.png");
	spRBG->setPosition(point + Vec2(132, -6));
	this->addChild(spRBG);

	//选中框
	m_spSelect = ReadSpriteName("new_ui/img_global/selected_box1.png");
	this->addChild(m_spSelect);

	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		//底框
		auto spItem = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		spItem->setScale(0.75f);
		Vec2 vecPos;
		if (i == 0)
		{
			vecPos = point + Vec2(133, 108);
		}
		else
		{
			vecPos = point + Vec2((i -1) % 2 * 190 + 38, -(i - 1) / 2 * 96 + 42);
		}
		spItem->setPosition(vecPos);
		this->addChild(spItem);

		auto spMaterial = MaterialSprite::create();
		spMaterial->setPosition(vecPos);
		spMaterial->setCallback(CC_CALLBACK_1(MergeLayer::callBackMaterial, this));
		this->addChild(spMaterial);
		m_mSpMaterial[i] = spMaterial;
	}

	//获得物品背景
	Sprite* spGetBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	spGetBG->setPosition(spRBG->getPosition());
	this->addChild(spGetBG);

	//获得物品
	m_spGoods = ReadSpriteNormal("img/Items/point.png");
	m_spGoods->setPosition(spGetBG->getPosition());
	this->addChild(m_spGoods);

	//获得数目
	m_labCount = LabelSystemFont("", 16);
	m_labCount->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_labCount->setPosition(Vec2(spGetBG->getContentSize().width - 5, 5));
	m_labCount->enableOutline(Color4B::BLACK, 1);
	spGetBG->addChild(m_labCount);

	//物品名字
	m_labGoods = LabelSystemFont("", 16);
	m_labGoods->setPosition(spGetBG->getPosition() + Vec2(0, -43));
	m_labGoods->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labGoods);

	//返还强化石
	m_labStone1 = LabelSystemFont(LL("returnstone"), 16);
	m_labStone1->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_labStone1->enableOutline(Color4B::BLACK, 1);
	spRBG->addChild(m_labStone1);

	m_labStone2 = LabelSystemFont("", 16);
	m_labStone2->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labStone2->enableOutline(Color4B::BLACK, 1);
	m_labStone2->setColor(Color3B(255, 0, 255));
	spRBG->addChild(m_labStone2);

	//消耗金币
	m_spGold = ReadSpriteName("new_ui/img_global/icon_gb.png");
	m_spGold->setAnchorPoint(Vec2(0.0f, 0.5f));
	spRBG->addChild(m_spGold);

	m_labGold = LabelSystemFont("", 16);
	m_labGold->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_labGold->setColor(Color3B::YELLOW);
	m_labGold->enableOutline(Color4B::BLACK, 1);
	spRBG->addChild(m_labGold);

	//页码
	m_labPage = LabelSystemFont("", 16);
	m_labPage->setPosition(point + Vec2(-153, -160));
	m_labPage->setColor(Color3B(102,0,0));
	this->addChild(m_labPage);

	//按钮菜单
	Menu* btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//分类下拉
	Sprite* spTypeN_xiala = ReadSpriteName("new_ui/img_global/global_jantou.png");
	Sprite* spTypeS_xiala = ReadSpriteName("new_ui/img_global/global_jantou.png");
	auto btnType1 = MenuItemSprite::create(spTypeN_xiala, spTypeS_xiala, CC_CALLBACK_1(MergeLayer::callBackType, this));
	btnType1->setPosition(point + Vec2(-248 + 123-15, 138));
	btnMenu->addChild(btnType1);

	//分类
	Sprite* spTypeN = ReadSpriteName("new_ui/img_global/global_ban11.png");
	Sprite* spTypeTextN = ReadSpriteName("new_ui/img_text/imgtxt7_1.png");
	spTypeTextN->setPosition(Vec2(spTypeN->getContentSize().width / 2, spTypeN->getContentSize().height / 2));
	spTypeN->addChild(spTypeTextN);
	Sprite* spTypeS = ReadSpriteName("new_ui/img_global/global_ban12.png");
	Sprite* spTypeTextS = ReadSpriteName("new_ui/img_text/imgtxt7_2.png");
	spTypeTextS->setPosition(Vec2(spTypeS->getContentSize().width / 2, spTypeS->getContentSize().height / 2));
	spTypeS->addChild(spTypeTextS);
	auto btnType = MenuItemSprite::create(spTypeN, spTypeS, CC_CALLBACK_1(MergeLayer::callBackType, this));
	btnType->setPosition(point + Vec2(-248, 138-2));
	btnMenu->addChild(btnType);

	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(MergeLayer::callBackLast, this));
	spLastS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLastS->setPosition(Vec2(spLastS->getContentSize().width / 2, spLastS->getContentSize().height / 2));
	btnLast->setPosition(point + Vec2(-208, -160));
	btnMenu->addChild(btnLast);

	//下一页
	Sprite* spNextN = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextN->setFlippedX(true);
	Sprite* spNextS = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextS->setFlippedX(true);
	spNextS->setScale(fScale);
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(MergeLayer::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(-98, -160));
	btnMenu->addChild(btnNext);

	//获得物品
	Sprite* spGoodsN = ReadSpriteNormal("img/Items/point.png");
	auto btnGoods = MenuItemSprite::create(spGoodsN, NULL, CC_CALLBACK_1(MergeLayer::callBackGoods, this));
	btnGoods->setPosition(m_spGoods->getPosition());
	btnMenu->addChild(btnGoods);

	//制作
	Sprite* spMakeN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spMakeTextN = ReadSpriteName("new_ui/img_text/imgtxt7_3.png");
	spMakeTextN->setPosition(Vec2(spMakeN->getContentSize().width / 2, spMakeN->getContentSize().height / 2));
	spMakeN->addChild(spMakeTextN);
	Sprite* spMakeS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spMakeTextS = ReadSpriteName("new_ui/img_text/imgtxt7_4.png");
	spMakeTextS->setPosition(Vec2(spMakeS->getContentSize().width / 2, spMakeS->getContentSize().height / 2));
	spMakeS->addChild(spMakeTextS);
	MenuItemSprite* btnMake = MenuItemSprite::create(spMakeN, spMakeS, CC_CALLBACK_1(MergeLayer::callBackMerge, this));
	btnMake->setPosition(point + Vec2(132, -158));
	btnMenu->addChild(btnMake);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(MergeLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(300, 175));
	btnMenu->addChild(btnClose);
}

void MergeLayer::initGoodsMap(int iType)
{
	m_mGoodsId.clear();
	m_labType->setString(trData->m_mMergeType[iType]->strName);

	int iIndex = 0;
	for (map<int, NetMergeData*>::iterator it = trData->m_mMergeData.begin(); it != trData->m_mMergeData.end(); it++)
	{
		NetMergeData* ptrData = it->second;
		if (ptrData->iType == iType)
		{
			m_mGoodsId[iIndex++] = ptrData->iGoodsId;
		}
	}

	//排序
	map<int, bool> __MergeMapFlag;
	for (auto _goods : trData->m_mMergeData)
	{
		if (_goods.second->iType== iType)
		{
			bool mergeFlag = true;
			for (auto vk : _goods.second->mMaterial)
			{
				if (history->m_pPlayerInfo->m_mEquipChip.find(vk.first)!= history->m_pPlayerInfo->m_mEquipChip.end())
				{
					if (history->m_pPlayerInfo->m_mEquipChip[vk.first]->getCount() >= vk.second)
					{

					}
					else {
						mergeFlag = false;
					}
				}
				else {
					bool _goodflag=false;
					for (auto _cc : history->m_pPlayerInfo->m_mGoods)
					{
						if (_cc.second->getGoodID() == vk.first)
						{
							_goodflag = true;
						}
					}
					if (!_goodflag)
					{
						mergeFlag = false;
					}
				}
			}
			if (mergeFlag)
			{
				__MergeMapFlag[_goods.second->iGoodsId] = true;
			}
		}
	}
	for (auto __vk: m_mGoodsId)
	{
		if (__MergeMapFlag.find(__vk.second) != __MergeMapFlag.end())
		{
			for (int i=1;i<=__vk.first;i++)
			{
				
				
				if (__MergeMapFlag.find(m_mGoodsId[__vk.first - i]) == __MergeMapFlag.end())
				{
					int shunxu;
					shunxu = m_mGoodsId[__vk.first - i + 1];
					m_mGoodsId[__vk.first - i + 1] = m_mGoodsId[__vk.first - i];
					m_mGoodsId[__vk.first - i] = shunxu;
				}
			}
		}
	}

	//排序

	m_iCurPage = 1;
	if (m_mGoodsId.size() > 0)
	{
		m_iMaxPage = (m_mGoodsId.size() - 1) / MAX_ITEM_NUM + 1;
		m_spSelect->setVisible(true);
	}
	else
	{
		m_iMaxPage = 1;
		m_spSelect->setVisible(false);
	}
	updateItemBox();
}

void MergeLayer::updateItemBox()
{
	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		int iIndex = i + (m_iCurPage - 1) * MAX_ITEM_NUM;
		if (m_mGoodsId.count(iIndex))
		{
			m_mSpGoods[i]->setTexture(history->getGoodsTexture(m_mGoodsId[iIndex]));
			m_mSpGoods[i]->setTag(m_mGoodsId[iIndex]);
		}
		else
		{
			m_mSpGoods[i]->setTexture(GetSpriteTexture("img/Items/point.png"));
			m_mSpGoods[i]->setTag(0);
		}
	}

	updateMaterial(m_mSpGoods[0]->getTag());
	m_spSelect->setPosition(m_mSpItemBG[0]->getPosition());
	m_labPage->setString(StringUtils::format("%d/%d", m_iCurPage, m_iMaxPage));
}

void MergeLayer::updateMaterial(int iGoodsId)
{
	m_iGoodsId = iGoodsId;
	map<int, int> mGoodsId;
	map<int, int> mMaterial;
	if(trData->m_mMergeData.count(iGoodsId))
	{
		mMaterial = trData->m_mMergeData[iGoodsId]->mMaterial;
		int iIndex = 0;
		for (map<int, int>::iterator it = mMaterial.begin(); it != mMaterial.end(); it++)
		{
			mGoodsId[iIndex++] = it->first;
		}

		m_labGold->setString(StringUtils::format("%d", trData->m_mMergeData[iGoodsId]->iGold));
		int iWidth = m_spGold->getContentSize().width + 5 + m_labGold->getContentSize().width;
		m_spGold->setPosition(Vec2(151 - iWidth / 2, 18));
		m_labGold->setPosition(Vec2(151 + iWidth / 2, 18));

		//合成品数目
		int iCount = trData->m_mMergeData[iGoodsId]->iCount;
		m_labCount->setVisible(iCount > 1);
		if (iCount > 1)
		{
			m_labCount->setString(StringUtils::format("%d", iCount));
		}
	}

	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		if (mGoodsId[i])
		{
			m_mSpMaterial[i]->updateSprite(mGoodsId[i], mMaterial[mGoodsId[i]]);
		}
		m_mSpMaterial[i]->setVisible(mGoodsId[i] != 0);
		m_mSpMaterial[i]->setTag(mGoodsId[i]);
	}

	m_spGoods->setTexture(history->getGoodsTexture(iGoodsId));
	HistoryGoods hisGoods(iGoodsId);
	m_labGoods ->setString(hisGoods.GetGoodsName());
	updateState();
}

string MergeLayer::getNetIdData()
{
	Json::Value valWarList(arrayValue);
	for (UInt i = 0; i < m_mSpMaterial.size(); i++)
	{
		if (m_mSpMaterial[i]->isVisible())
		{
			Json::Int valNetId = m_mSpMaterial[i]->getNetId();
			valWarList.append(valNetId);
		}
	}

	string strRes;
	FastWriter fastWriter;
	strRes = fastWriter.write(valWarList);
	strRes.erase(strRes.end() - 1); //去掉末尾的'\n'
	return strRes;
}

void MergeLayer::callBackClose(Ref *sender)
{
    playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

bool MergeLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		Rect rc = m_mSpItemBG[i]->getBoundingBox();
		if(rc.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			m_iClickIndex = i;
		}
	}

	return true;
}

void MergeLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_iClickIndex >= 0)
	{
		Rect rc = m_mSpItemBG[m_iClickIndex]->getBoundingBox();
		if (!rc.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			m_iClickIndex = -1;
		}
	}
}

void MergeLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if(m_iClickIndex >= 0)
	{
		int iGoodsId = m_mSpGoods[m_iClickIndex]->getTag();
		if (iGoodsId)
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			m_spSelect->setPosition(m_mSpItemBG[m_iClickIndex]->getPosition());
			updateMaterial(iGoodsId);
		}
		m_iClickIndex = -1;
	}
}

MergeLayer::MergeLayer()
{

}

MergeLayer::~MergeLayer()
{
    
}

void MergeLayer::updateShow(int index)
{
	if (index != 9999999)
	{
		initGoodsMap(index);
	}
}

void MergeLayer::updateState()
{
	int iTotalStone = 0;
	for(UInt i = 0; i < m_mSpMaterial.size(); i++)
	{
		if(m_mSpMaterial[i]->isVisible())
		{
			iTotalStone += m_mSpMaterial[i]->getStoneNum();
		}
	}

	if(iTotalStone)
	{
		m_labStone2->setString(StringUtils::format("%d", iTotalStone));
		int iWidth = m_labStone1->getContentSize().width + 5 + m_labStone2->getContentSize().width;
		m_labStone1->setPosition(Vec2(151 - iWidth / 2, 45));
		m_labStone2->setPosition(Vec2(151 + iWidth / 2, 45));
		m_labStone1->setVisible(true);
		m_labStone2->setVisible(true);
	}
	else
	{
		m_labStone1->setVisible(false);
		m_labStone2->setVisible(false);
	}
}

void MergeLayer::callBackLast(Ref *sender)
{
    playSound(SOUND_LEFT_MOUSE_CLICK);
    if(m_iCurPage > 1)
    {
        m_iCurPage--;
    }
    else if(m_iCurPage == 1 && m_iMaxPage > 1)
    {
        m_iCurPage = m_iMaxPage;
    }
	updateItemBox();
}

void MergeLayer::callBackNext(Ref *sender)
{
    playSound(SOUND_LEFT_MOUSE_CLICK);
    if(m_iCurPage < m_iMaxPage)
    {
        m_iCurPage++;
    }
    else if(m_iCurPage == m_iMaxPage)
    {
        m_iCurPage = 1;
    }
	updateItemBox();
}

void MergeLayer::onHttpRequest()
{
	string strNetId = getNetIdData();
	string strToken = history->m_pLoginInfo->getGameToken();
	string strData = StringUtils::format("id=%d&original=%s&gameToken=%s", m_iGoodsId, strNetId.c_str(), strToken.c_str());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData);
	string data = StringUtils::format("{\"id\":%d,\"original\":%s,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iGoodsId, strNetId.c_str(), strToken.c_str(), validateCode.c_str());
	CCLOG("%s", data.c_str());

	ITools::getInstance()->connectingNetwork("compose",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(MergeLayer::onHttpRequestCompleted),
		data,
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void MergeLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onHttpReqSucceed(value["result"]);
	}
}

void MergeLayer::onHttpReqSucceed(Json::Value resVal)
{
	m_iNoticeIndex = 0;
	m_strNotice.clear();

	//去除消耗金币或材料
	history->money -= trData->m_mMergeData[m_iGoodsId]->iGold; //减去金币消耗
	for(UInt i = 0; i < m_mSpMaterial.size(); i++)
	{
		if(m_mSpMaterial[i]->isVisible())
		{
			m_mSpMaterial[i]->removeMaterial();
		}
	}

	m_strNotice = history->onHttpGoodsAdd(resVal);
	this->schedule(schedule_selector(MergeLayer::playGoodsNotice), 0.8f);
	updateMaterial(m_iGoodsId);

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}

	if (trData->m_mMergeData[m_iGoodsId]->barrage)
	{
		NetPromptTool::getInstance()->AddNetPrompt(PROMPT_WQHC, m_iGoodsId);
	}
}

void MergeLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->unschedule(schedule_selector(MergeLayer::playGoodsNotice));
	}
}

void MergeLayer::callBackType(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(MergeTypeLayer::create(this));
}

void MergeLayer::callBackMaterial(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	MaterialSprite* spMaterial = (MaterialSprite*)sender;
	int iGoodsId = spMaterial->getTag();
	HistoryGoods hisGoods(iGoodsId);

	if (hisGoods.isEquip())
	{
		bool bIsExist = false;
		for(int i = 0; i < history->goods->size(); i++)
		{
			HistoryGoods* ptrGoods = history->goods->elementAt(i);
			if (ptrGoods->good_id == hisGoods.good_id)
			{
				bIsExist = true;
				break;
			}
		}

		if (bIsExist)
		{
			ChessApplication::getInstance()->AddFunctionLayer(MergeSelectLayer::create(iGoodsId, spMaterial, this));
		}
		else
		{
			string strNotice = StringUtils::format("%s %s", hisGoods.GetGoodsName(), LL("storeisnull"));
			ITools::getInstance()->createGlobalPrompt(strNotice, false);
		}
	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoodsLayer::create(iGoodsId));
	}
}

void MergeLayer::callBackGoods(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoodsLayer::create(m_iGoodsId));
}

void MergeLayer::callBackMerge(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	for(UInt i = 0; i < m_mSpMaterial.size(); i++) //材料不足
	{
		if(m_mSpMaterial[i]->isVisible() && !m_mSpMaterial[i]->isFinished())
		{
			ITools::getInstance()->createGlobalPrompt(LL("materialnotenough"), false);
			return;
		}
	}

	if(history->money < trData->m_mMergeData[m_iGoodsId]->iGold) //金币不足
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
		return;
	}

	HistoryGoods hisGoods(m_iGoodsId);
	if (hisGoods.isGetEnable())
	{
		onHttpRequest();
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("wufazhizuo"), false);
	}
	//history->updateGuide(ZHUJIEMIAN5_GUIDE);
}