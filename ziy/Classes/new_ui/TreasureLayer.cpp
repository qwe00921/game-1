//
//  TreasureLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/3/30.
//
//

#include "TreasureLayer.h"
#include "Defines.h"
#include "GoodsDealLayer.h"
#include "PopLayer.h"

#define PAGE_VIEW_MAX  12  //单页显示最大宝物数目

bool TreasureLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iCurIndex = 1;
	m_iMaxIndex = 1;
	m_iOldIndex = -1;

	initGoodsMap();
	initControl();
	updateCurGoods(trData->m_mItemClass.size());
	updateBtnState(trData->m_mItemClass.size());
	return true;
}


bool TreasureLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	for(int i = 0; i < PAGE_VIEW_MAX; i++)
	{
		Rect rectIcon = m_mTreasureElem[i]->getIcon()->getBoundingBox();
		if(m_mTreasureElem[i]->getGoodID()>-1 && rectIcon.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			TreasureInfoLayer* treasureLayer = TreasureInfoLayer::create(m_mTreasureElem[i]->getGoodID());
			this->addChild(treasureLayer);
		}
	}

	return true;
}
void TreasureLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}
void TreasureLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

TreasureLayer::TreasureLayer()
{

}
TreasureLayer::~TreasureLayer()
{

}

void TreasureLayer::onEnter()
{
	Layer::onEnter();
}

void TreasureLayer::onExit()
{
	Layer::onExit();
}

void TreasureLayer::initGoodsMap()
{
	m_mapGoods.clear();
	int iIndex = 0;
	map<int, GoodsItem*>::iterator itGoods;
	for(itGoods = trData->m_mGoodsItem.begin(); itGoods != trData->m_mGoodsItem.end(); itGoods++)
	{
		GoodsItem* ptrGoods = itGoods->second;
		if (ptrGoods->iTypeId == 10)
		{
			m_mapGoods[iIndex++] = ptrGoods->iGoodsId;
		}
	}
}

void TreasureLayer::updateGoodsView()
{
	//刷新页码
	m_iMaxIndex = (m_mCurGoods.size() - 1) / PAGE_VIEW_MAX + 1;
	__String* ptrStore = __String::createWithFormat("%d/%d", getTreasureStore(), m_mCurGoods.size());
	m_labStore->setString(ptrStore->getCString());
	__String* ptrNum = __String::createWithFormat("%d/%d", m_iCurIndex, m_iMaxIndex);
	m_labPage->setString(ptrNum->getCString());

	int iInitId = (m_iCurIndex - 1) * PAGE_VIEW_MAX;
	for(int i = 0; i < PAGE_VIEW_MAX; i++)
	{
		m_mTreasureElem[i]->getIcon()->removeAllChildren();
		if(iInitId + i < (int)m_mCurGoods.size())
		{
			m_mTreasureElem[i]->setGoodID(m_mCurGoods[iInitId + i]);
			HistoryGoods hisGood(m_mCurGoods[iInitId + i]);
			string strPath = history->GetGoodsImagePath(hisGood.good_id);
			Sprite* goodIcon = ReadSpriteNormal(strPath.c_str());
			Sprite* curIcon = m_mTreasureElem[i]->getIcon();
			goodIcon->setPosition(Vec2(curIcon->getContentSize().width / 2, curIcon->getContentSize().height / 2));
			curIcon->addChild(goodIcon);
			m_mTreasureElem[i]->getName()->setString(hisGood.GetGoodsName());
			if (hisGood.getStoreNum() == 0) //库存为0，图片和名字灰色显示
			{
				addSpriteGray(goodIcon);
				m_mTreasureElem[i]->getName()->setColor(Color3B(153, 153, 153));
			}
			else
			{
				m_mTreasureElem[i]->getName()->setColor(Color3B(102, 51, 0));
			}
		}
		else
		{
			m_mTreasureElem[i]->setGoodID(-1);
			m_mTreasureElem[i]->getName()->setString("");
		}
	}
}

void TreasureLayer::updateCurGoods(int iTypeId)
{
	m_mCurGoods.clear();
	m_iCurIndex = 1;
	int iIndex = 0;
	for(UInt i = 0; i < m_mapGoods.size(); i++)
	{
		int iItid = trData->m_mGoodsItem[m_mapGoods[i]]->iItId;
		int iCurId = trData->m_mItemType[iItid]->typeId;
		if (iTypeId >= (int)trData->m_mItemClass.size() || iTypeId == iCurId - 1)
		{
			m_mCurGoods[iIndex++] = m_mapGoods[i];
		}
	}
	updateGoodsView();
}

void TreasureLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//背景1
	Sprite* bg1 = ReadSpriteName("new_ui/manual/manual_bg1.png");
	bg1->setPosition(point);
	this->addChild(bg1);

	//类型按钮
	m_pBtnType = Menu::create();
	m_pBtnType->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnType);
	int iTotal = trData->m_mItemClass.size() + 1;
	for(int i = 0; i < iTotal; i++)
	{
		Sprite* item_n = ReadSpriteName("new_ui/manual/manual_bnt1.png");
		Sprite* item_s = ReadSpriteName("new_ui/manual/manual_bnt2.png");
		if(i < iTotal - 1 && i % 2)
		{
			item_n->setFlippedX(true);
			item_s->setFlippedX(true);
		}
		Vec2 itemPoint = Vec2(point.x - 270 + 540 * (i % 2), point.y + 125 - 60 * (i / 2));
		//类型名称
		__String strType = "";
		if(i == iTotal - 1) //按钮"全部"
		{
			itemPoint = Vec2(point.x - 270, point.y - 143);
			strType = LL("quanbu");
		}
		else
		{
			strType = trData->m_mItemClass[i]->name;
		}
		
		int iOffsetX = (i < iTotal - 1 && i % 2) ? 12 : -12; //左右两边偏移不一样
		Label* labType_n = LabelSystemFont(strType.getCString(), 18);
		labType_n->setPosition(Vec2(item_n->getContentSize().width / 2 + iOffsetX, item_n->getContentSize().height / 2));
		labType_n->setColor(Color3B(102, 51, 0));
		item_n->addChild(labType_n);
		Label* labType_s = LabelSystemFont(strType.getCString(), 18);
		labType_s->setPosition(Vec2(item_s->getContentSize().width / 2 + iOffsetX, item_s->getContentSize().height / 2));
		labType_s->setColor(Color3B(102, 51, 0));
		item_s->addChild(labType_s);

		m_pBtnSelect[i] = item_s;
		m_mpMenuItem[i] = MenuItemSprite::create(item_n, item_s, CC_CALLBACK_1(TreasureLayer::callBackItemClass, this));
		m_mpMenuItem[i]->setTag(i);
		m_mpMenuItem[i]->setPosition(itemPoint);
		m_pBtnType->addChild(m_mpMenuItem[i]);
	}

	//背景2
	Sprite* bg2 = ReadSpriteName("new_ui/manual/manual_bg2.png");
	bg2->setPosition(point);
	this->addChild(bg2);

	//装备表格
	for(int i = 0; i < PAGE_VIEW_MAX; i++)
	{
		Vec2 curPoint = Vec2(point.x - 180 + 120 * (i % 4), point.y + 104 - 96 * (i / 4));
		//背景
		Sprite* spGoodBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		spGoodBG->setPosition(curPoint);
		this->addChild(spGoodBG);
		//名称
		Label* nameLab = LabelSystemFont("", 16);
		nameLab->setPosition(Vec2(curPoint.x, curPoint.y - spGoodBG->getContentSize().height / 2 - 12));
		nameLab->setColor(Color3B(102, 51, 0));
		this->addChild(nameLab);

		m_mTreasureElem[i] = new TreasureElem();
		m_mTreasureElem[i]->setGoodID(-1);
		m_mTreasureElem[i]->setIcon(spGoodBG);
		m_mTreasureElem[i]->setName(nameLab);
	}

	//图鉴总数
	m_labStore = LabelSystemFont("1/1", 16);
	m_labStore->setPosition(Vec2(point.x - 208, point.y - 160));
	m_labStore->setColor(Color3B::YELLOW);
	this->addChild(m_labStore);

	//页码
	m_labPage = LabelSystemFont("1/1", 16);
	m_labPage->setPosition(Vec2(point.x, point.y - 160));
	m_labPage->setColor(Color3B::YELLOW);
	this->addChild(m_labPage);

	//功能按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//上一页
	Sprite* last_n = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* last_s = ReadSpriteName("new_ui/img_global/switch1.png");
	MenuItemSprite* btnLast = MenuItemSprite::create(last_n, last_s, CC_CALLBACK_1(TreasureLayer::callBackLastView, this));
	btnLast->setPosition(Vec2(point.x - 55, point.y - 160));
	m_pBtnMenu->addChild(btnLast);

	//下一页
	Sprite* next_n = ReadSpriteName("new_ui/img_global/switch1.png");
	next_n->setFlippedX(true);
	Sprite* next_s = ReadSpriteName("new_ui/img_global/switch1.png");
	next_s->setFlippedX(true);
	MenuItemSprite* btnNext = MenuItemSprite::create(next_n, next_s, CC_CALLBACK_1(TreasureLayer::callBackNextView, this));
	btnNext->setPosition(Vec2(point.x + 55, point.y - 160));
	m_pBtnMenu->addChild(btnNext);

	//关闭按钮
	Sprite* close_n = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* close_s = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(close_n, close_s, CC_CALLBACK_1(TreasureLayer::callBackClose, this));
	btnClose->setPosition(Vec2(point.x + 256, point.y + 175));
	m_pBtnMenu->addChild(btnClose);
}

int TreasureLayer::getTreasureStore()
{
	int iStoreNum = 0;
	for(unsigned int i = 0; i < m_mCurGoods.size(); i++)
	{
		HistoryGoods hisGood(m_mCurGoods[i]);
		if (hisGood.getStoreNum())
		{
			iStoreNum++;
		}
	}
	return iStoreNum;
}

void TreasureLayer::callBackItemClass(Ref* sender)
{
	int iCurIndex = ((MenuItemSprite*)sender)->getTag();

	if(iCurIndex != m_iOldIndex)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		updateCurGoods(iCurIndex);
	}
	updateBtnState(iCurIndex);
}

void TreasureLayer::callBackClose(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void TreasureLayer::callBackLastView(Ref* sender)
{
	if(m_iMaxIndex > 1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_iCurIndex--;
		if(m_iCurIndex == 0 && m_iMaxIndex > 1)
		{
			m_iCurIndex = m_iMaxIndex;
		}
		updateGoodsView();
	}
}

void TreasureLayer::callBackNextView(Ref* sender)
{
	if(m_iMaxIndex > 1)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		m_iCurIndex++;
		if(m_iCurIndex > m_iMaxIndex)
		{
			m_iCurIndex = 1;
		}
		updateGoodsView();
	}
}

void TreasureLayer::updateBtnState(int iIndex)
{
	if(m_iOldIndex >= 0)
	{
		int iOffsetX = (m_iOldIndex < (int)m_mpMenuItem.size() - 2 && m_iOldIndex % 2) ? -8 : 8;
		m_pBtnSelect[m_iOldIndex]->setPosition(m_pBtnSelect[m_iOldIndex]->getPosition() + Vec2(iOffsetX, 0));
		m_mpMenuItem[m_iOldIndex]->unselected();
	}
	int iCurOffsetX = (iIndex < (int)m_mpMenuItem.size() - 2 && iIndex % 2) ? -8 : 8;
	m_pBtnSelect[iIndex]->setPosition(m_pBtnSelect[iIndex]->getPosition() - Vec2(iCurOffsetX, 0));
	m_mpMenuItem[iIndex]->selected();
	m_iOldIndex = iIndex;
}

//宝物信息//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TreasureInfoLayer* TreasureInfoLayer::create(int iGoodId)
{
	TreasureInfoLayer* pRet = new TreasureInfoLayer();
	if(pRet&&pRet->init(iGoodId))
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

bool TreasureInfoLayer::init(int iGoodId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_pHisGoods = new HistoryGoods(iGoodId);
	initControl();
	return true;
}


bool TreasureInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}

void TreasureInfoLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void TreasureInfoLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void TreasureInfoLayer::onExit()
{
	Layer::onExit();
}

TreasureInfoLayer::TreasureInfoLayer()
{

}

TreasureInfoLayer::~TreasureInfoLayer()
{
	SAFE_DELETE(m_pHisGoods);
}

void TreasureInfoLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* pInfoBG = ReadSpriteName("new_ui/info_bg2.png");
	pInfoBG->setPosition(Vec2(point.x, point.y));
	this->addChild(pInfoBG);

	//ICON底框
	Sprite* imgBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBG->setPosition(Vec2(point.x - 85, point.y + 125));
	this->addChild(imgBG);

	//物品ICON
	string imgStr = history->GetGoodsImagePath(m_pHisGoods->good_id);
	Sprite* imgGoods = ReadSpriteNormal(imgStr.c_str());
	imgGoods->setPosition(Vec2(point.x - 85, point.y + 125));
	this->addChild(imgGoods);

	//名称
	Label* nameLab = LabelSystemFont(m_pHisGoods->GetGoodsName(), 16);
	nameLab->setPosition(Vec2(point.x - 45 + nameLab->getContentSize().width / 2, point.y + 140));
	nameLab->setColor(Color3B(0xff, 0xff, 0x99));
	this->addChild(nameLab);

	//类型
	Label* typeLab = LabelSystemFont(__String::createWithFormat("(%s)", m_pHisGoods->GetAttributeInfo())->getCString(), 16);
	typeLab->setPosition(Vec2(point.x - 45 + nameLab->getContentSize().width + typeLab->getContentSize().width / 2, point.y + 140));
	typeLab->setColor(Color3B(0xff, 0xff, 0xff));
	this->addChild(typeLab);

	//库存
	Label* storeLab = LabelSystemFont(__String::createWithFormat(LL("yiyongyoudjian"), m_pHisGoods->getStoreNum())->getCString(), 16);
	storeLab->setPosition(Vec2(point.x - 45 + storeLab->getContentSize().width / 2, point.y + 115));
	storeLab->setColor(Color3B(0x66, 0, 0));
	this->addChild(storeLab);

	//攻击或防御
	GoodsItem* ptrGood = m_pHisGoods->getGoodAttr();
	string strPower = "---";
	if(ptrGood->isEquip())
	{
		strPower = IntToString(m_pHisGoods->GetGoodsPower());
	}
	//攻击文字
	Label* powerLab = LabelSystemFont(__String::createWithFormat("%s:", m_pHisGoods->getAttributeText().c_str())->getCString(), 16);
	powerLab->setPosition(Vec2(point.x - 118 + powerLab->getContentSize().width / 2, point.y + 75));
	powerLab->setColor(Color3B(0x66, 0, 0));
	this->addChild(powerLab);

	//攻击数据
	Label* numLab = LabelSystemFont(strPower.c_str(), 16);
	numLab->setPosition(Vec2(point.x - 118 + powerLab->getContentSize().width + numLab->getContentSize().width / 2, point.y + 75));
	numLab->setColor(Color3B(0xff, 0xff, 0));
	this->addChild(numLab);

	//特性
	Label* labEffect = LabelSystemFont(__String::createWithFormat("%s:", LL("texing"))->getCString(), 16);
	labEffect->setPosition(Vec2(point.x - 118 + labEffect->getContentSize().width / 2, point.y + 55));
	labEffect->setColor(Color3B(0x66, 0, 0));
	this->addChild(labEffect);
	__String* strEffect = __String::createWithFormat("%s(%s)", ptrGood->GetSpecialEffectName(m_pHisGoods->GetRealLevel()), ptrGood->GetSpecialEffectInfo(m_pHisGoods->GetRealLevel()));
	Label* labEffectVal = LabelSystemFont(strEffect->getCString(), 16);
	labEffectVal->setPosition(Vec2(point.x - 118 + labEffect->getContentSize().width + 192 / 2, point.y + 55 - labEffectVal->getContentSize().height / 2));
	labEffectVal->setColor(Color3B::YELLOW);
	labEffectVal->setDimensions(192, labEffectVal->getContentSize().height * 2);
	labEffectVal->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(labEffectVal);

	//物品信息
	Label* infoLab = LabelSystemFont(ptrGood->strInfo.c_str(), 16);
	infoLab->setPosition(Vec2(point.x, point.y - 16));
	infoLab->setDimensions(240, 90);
	infoLab->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(infoLab);

	//获得来源
	Label* labSource = LabelSystemFont(LL("huodelaiyuan"), 16);
	labSource->setPosition(Vec2(point.x - 118 + labSource->getContentSize().width / 2, point.y - 90));
	labSource->setColor(Color3B(0x66, 0, 0));
	this->addChild(labSource);

	//宝物信息
	Label* labMethod = LabelSystemFont(trData->m_mTreasureInfo[m_pHisGoods->good_id+1]->method, 16);
	labMethod->setPosition(Vec2(point.x, point.y - 145));
	labMethod->setDimensions(240, 90);
	labMethod->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(labMethod);
}