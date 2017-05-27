//
//  BackpackLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/4/5.
//
//

#include "BackpackLayer.h"
#include "Defines.h"
#include "GoodsDealLayer.h"
#include "SkillAndGoodsInfoLayer.h"
#include "GoldExchangeLayer.h"
#include "Net_HomePage.h"

#define PACK_GOOD_MAX  32  //单页显示最大物品数目

enum
{
	SHOW_EQUIP = 0,
	SHOW_PROP,
	SHOW_PIECE,
	SHOW_ALL
};

BPGoodsSprite* BPGoodsSprite::create()
{
	BPGoodsSprite *pRet = new BPGoodsSprite();
	if(pRet && pRet->init())
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

bool BPGoodsSprite::init()
{
	if(!Sprite::init())
	{
		return false;
	}

	//物品ICON
	m_spIcon = ReadSpriteNormal("img/Items/1.png");
	m_spIcon->setScale(0.8f);
	this->addChild(m_spIcon);

	//碎片
	m_spChip = ReadSpriteName("new_ui/hero/icon_soul.png");
	m_spChip->setScale(0.8f);
	m_spChip->setPosition(Vec2(-15, 15));
	this->addChild(m_spChip);

	//库存
	m_labNum = LabelSystemFont("", 14);
	m_labNum->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_labNum->setPosition(Vec2(24, -25));
	m_labNum->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labNum);

	//边框
	m_spFrame = ReadSpriteName("new_ui/img_global/q1.png");
	this->addChild(m_spFrame);

	//星级
	m_spStar = ReadSpriteName("new_ui/img_number/star_1.png");
	m_spStar->setPosition(Vec2(0, -18));
	this->addChild(m_spStar);

	return true;
}

void BPGoodsSprite::updateSprite(HistoryGoods* ptrGoods)
{
	if (ptrGoods == NULL)
	{
		setVisible(false);
		return;
	}
	else
	{
		setVisible(true);
	}

	//物品ICON
	m_spIcon->setTexture(GetSpriteTexture(history->GetGoodsImagePath(ptrGoods->good_id).c_str()));

	//碎片
	if(ptrGoods->GetGoodsType() == GOOD_TYPE_PIECE)
	{
		string strPath = "";
		if(ptrGoods->isSoul())
		{
			strPath = "new_ui/hero/icon_soul.png";
		}
		else if(ptrGoods->isPiece())
		{
			strPath = "new_ui/img_global/icon_sp.png";
		}
		m_spChip->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
		m_spChip->setVisible(true);
	}
	else
	{
		m_spChip->setVisible(false);
	}

	//库存
	if(ptrGoods->getStoreNum() > 1)
	{
		m_labNum->setString(StringUtils::format("%d", ptrGoods->getStoreNum()));
	}
	else
	{
		m_labNum->setString("");
	}

	//边框星级
	if (ptrGoods->iQuality)
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
}

bool BackpackLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iCurIndex = 1;
	m_iMaxIndex = 1;
	m_iCurType = SHOW_EQUIP;

	initControl();
	initGoodsMap();
	HongDian::getInstence()->autoUpdateBaoguo();
	return true;
}

void BackpackLayer::initControl()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/img_global/global_bg2.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题
	//Sprite* title = ReadSpriteName("new_ui/img_text/bt2_1.png");
	//title->setPosition(point + Vec2(0, 168));
	//this->addChild(title);

	//列表背景
	//Sprite* listBG = ReadSpriteName("new_ui/shop/shop_bg5.png");
	//listBG->setPosition(point + Vec2(0, -8));
	//this->addChild(listBG);

	//页码
	m_pLabPage = LabelSystemFont("1/1", 16);
	m_pLabPage->setPosition(point + Vec2(0, -162));
	m_pLabPage->setColor(Color3B(102,0,0));
	this->addChild(m_pLabPage);

	//物品表格
	for(int i = 0; i < PACK_GOOD_MAX; i++)
	{
		Sprite* item = ReadSpriteName("new_ui/img_global/item_box_bg.png");
		item->setPosition(point + Vec2(-230 + 66 * (i % 8), 82 - 62 * (i / 8)));
		this->addChild(item);

		BPGoodsSprite* spGoods = BPGoodsSprite::create();
		spGoods->setPosition(Vec2(item->getContentSize().width / 2, item->getContentSize().height / 2));
		item->addChild(spGoods);

		m_mPackGoods[i] = new PackGoods();
		m_mPackGoods[i]->setHisGood(NULL);
		m_mPackGoods[i]->setBG(item);
		m_mPackGoods[i]->setSPGoods(spGoods);
	}

	//功能按钮菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//金币充值
	Sprite* spGoldN = ReadSpriteName("new_ui/main/main_bg2.png");
	Sprite* spGoldS = ReadSpriteName("new_ui/main/main_bg2.png");
	MenuItemSprite* btnGold = MenuItemSprite::create(spGoldN, spGoldS, CC_CALLBACK_1(BackpackLayer::callBackGold, this));
	btnGold->setPosition(point + Vec2(-220, 144));
	m_pBtnMenu->addChild(btnGold);

	//金额
	m_pLabMoney = LabelSystemFont(IntToStringEx(history->money), 16);
	m_pLabMoney->setColor(Color3B(0xff, 0xff, 0));
	m_pLabMoney->setAnchorPoint(Vec2(1.0f, 0.5f));
	m_pLabMoney->setPosition(point + Vec2(-190, 144));
	this->addChild(m_pLabMoney);

	//金币ICON
	Sprite* goldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	goldIcon->setPosition(point + Vec2(-170, 144));
	this->addChild(goldIcon);

	//装备
	Sprite* spEquipN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spEqTextN = ReadSpriteName("new_ui/shop/ban_txt1.png");
	spEqTextN->setPosition(Vec2(spEquipN->getContentSize().width / 2, spEquipN->getContentSize().height / 2));
	spEquipN->addChild(spEqTextN);
	Sprite* spEquipS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spEqTextS = ReadSpriteName("new_ui/shop/ban_txt2.png");
	spEqTextS->setPosition(Vec2(spEquipS->getContentSize().width / 2, spEquipS->getContentSize().height / 2));
	spEquipS->addChild(spEqTextS);
	Sprite* spEquipD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spEqTextD = ReadSpriteName("new_ui/shop/ban_txt2.png");
	spEqTextD->setPosition(Vec2(spEquipD->getContentSize().width / 2, spEquipD->getContentSize().height / 2));
	spEquipD->addChild(spEqTextD);
	m_mBtnType[0] = MenuItemSprite::create(spEquipN, spEquipS, spEquipD, CC_CALLBACK_1(BackpackLayer::callBackShowSwitch, this));
	m_mBtnType[0]->setTag(SHOW_EQUIP);
	m_mBtnType[0]->setPosition(point + Vec2(298+10, 70+40));
	m_mBtnType[0]->setEnabled(false);
	m_pBtnMenu->addChild(m_mBtnType[0]);

	//道具
	Sprite* spPropN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spPrTextN = ReadSpriteName("new_ui/shop/ban_txt3.png");
	spPrTextN->setPosition(Vec2(spPropN->getContentSize().width / 2, spPropN->getContentSize().height / 2));
	spPropN->addChild(spPrTextN);
	Sprite* spPropS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spPrTextS = ReadSpriteName("new_ui/shop/ban_txt4.png");
	spPrTextS->setPosition(Vec2(spPropS->getContentSize().width / 2, spPropS->getContentSize().height / 2));
	spPropS->addChild(spPrTextS);
	Sprite* spPropD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spPrTextD = ReadSpriteName("new_ui/shop/ban_txt4.png");
	spPrTextD->setPosition(Vec2(spPropD->getContentSize().width / 2, spPropD->getContentSize().height / 2));
	spPropD->addChild(spPrTextD);
	m_mBtnType[1] = MenuItemSprite::create(spPropN, spPropS, spPropD, CC_CALLBACK_1(BackpackLayer::callBackShowSwitch, this));
	m_mBtnType[1]->setTag(SHOW_PROP);
	m_mBtnType[1]->setPosition(point + Vec2(298 + 10, 0+30));
	m_pBtnMenu->addChild(m_mBtnType[1]);

	//碎片
	Sprite* spPieceN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spPieTextN = ReadSpriteName("new_ui/shop/ban_txt5.png");
	spPieTextN->setPosition(Vec2(spPieceN->getContentSize().width / 2, spPieceN->getContentSize().height / 2));
	spPieceN->addChild(spPieTextN);
	Sprite* spPieceS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spPieTextS = ReadSpriteName("new_ui/shop/ban_txt6.png");
	spPieTextS->setPosition(Vec2(spPieceS->getContentSize().width / 2, spPieceS->getContentSize().height / 2));
	spPieceS->addChild(spPieTextS);
	Sprite* spPieceD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spPieTextD = ReadSpriteName("new_ui/shop/ban_txt6.png");
	spPieTextD->setPosition(Vec2(spPieceD->getContentSize().width / 2, spPieceD->getContentSize().height / 2));
	spPieceD->addChild(spPieTextD);
	m_mBtnType[2] = MenuItemSprite::create(spPieceN, spPieceS, spPieceD, CC_CALLBACK_1(BackpackLayer::callBackShowSwitch, this));
	m_mBtnType[2]->setTag(SHOW_PIECE);
	m_mBtnType[2]->setPosition(point + Vec2(298 + 10, -70+20));
	m_pBtnMenu->addChild(m_mBtnType[2]);

	for (int i=0;i<=2;i++)
	{
		HongDian::getInstence()->addHongDianNode(m_mBtnType[i],200+i);
		
	}
	
	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(BackpackLayer::callBackLast, this));
	spLastS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spLastS->setPosition(Vec2(spLastS->getContentSize().width / 2, spLastS->getContentSize().height / 2));
	btnLast->setPosition(point + Vec2(-55, -162));
	m_pBtnMenu->addChild(btnLast);

	//下一页
	Sprite* spNextN = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextN->setFlippedX(true);
	Sprite* spNextS = ReadSpriteName("new_ui/img_global/switch1.png");
	spNextS->setFlippedX(true);
	spNextS->setScale(fScale);
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(BackpackLayer::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(55, -162));
	m_pBtnMenu->addChild(btnNext);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(BackpackLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(297, 173));
	m_pBtnMenu->addChild(btnClose);
}

void BackpackLayer::callBackClose(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void BackpackLayer::callBackNext(Ref* sender)
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
void BackpackLayer::callBackLast(Ref* sender)
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

bool BackpackLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	for(int i = 0; i < PACK_GOOD_MAX; i++)
	{
		Rect rectBG = m_mPackGoods[i]->getBG()->getBoundingBox();
		HistoryGoodsPtr ptrGoods = m_mPackGoods[i]->getHisGood();
		if(ptrGoods && rectBG.containsPoint(convertToNodeSpace(touch->getLocation())))
		{
			playSound(SOUND_LEFT_MOUSE_CLICK);
			this->addChild(GoodsDealLayer::create(ptrGoods, this));
		}
	}
	return true;
}
void BackpackLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}
void BackpackLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

BackpackLayer::BackpackLayer()
{

}

BackpackLayer::~BackpackLayer()
{

}

void BackpackLayer::onEnter()
{
	Layer::onEnter();
}

void BackpackLayer::onExit()
{
	Layer::onExit();
}

void BackpackLayer::updateGoodsView()
{
	if(m_mCurGoods.size())
	{
		m_iMaxIndex = (m_mCurGoods.size() - 1) / PACK_GOOD_MAX + 1;
	}
	else
	{
		m_iMaxIndex = 1;
	}

	if (m_iCurIndex > m_iMaxIndex)
	{
		m_iCurIndex = m_iMaxIndex;
	}

	m_pLabPage->setString(StringUtils::format("%d/%d", m_iCurIndex, m_iMaxIndex));

	int iInitId = (m_iCurIndex - 1) * PACK_GOOD_MAX;
	for(int i = 0; i < PACK_GOOD_MAX; i++)
	{
		HistoryGoods* ptrGoods = NULL;
		if(iInitId + i < (int)m_mCurGoods.size())
		{
			ptrGoods = m_mCurGoods[iInitId + i];
		}
		m_mPackGoods[i]->setHisGood(ptrGoods);
		m_mPackGoods[i]->getSPGoods()->updateSprite(ptrGoods);
	}
}

void BackpackLayer::updateShow(int index)
{
	m_pLabMoney->setString(IntToStringEx(history->money));
	updateGoodsMap();

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

void BackpackLayer::updateCurGoods()
{
	m_mCurGoods = m_mGoodsMap[m_iCurType];
	updateGoodsView();
}

void BackpackLayer::callBackShowSwitch(Ref* sender)
{
	MenuItemSprite* pItem = (MenuItemSprite*)sender;
	int iTag = pItem->getTag();

	for(UInt i = 0; i < m_mBtnType.size(); i++)
	{
		m_mBtnType[i]->setEnabled(iTag != i);
	}

	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iCurType = iTag;
	m_iCurIndex = 1;
	updateCurGoods();
}

void BackpackLayer::callBackGold(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
}

void BackpackLayer::initGoodsMap()
{
	map<int, HistoryGoods*> mapGoods; 
	int iIndex = 0;

	HistoryGoodsPtr ptreGoods = NULL;
	for(int i = 0; i < history->goods->size(); i++)
	{
		ptreGoods = history->goods->elementAt(i);
		if(!ptreGoods->isFashion())
			mapGoods[iIndex++] = ptreGoods;
	}
	m_mGoodsMap[0] = mapGoods;

	mapGoods.clear();
	iIndex = 0;
	for(map<int, int>::iterator it = history->m_mConsuNum.begin(); it != history->m_mConsuNum.end(); it++)
	{
		if(it->second)
		{
			HistoryGoods* ptrGoods = new HistoryGoods(it->first);
			ptrGoods->setNetID(history->m_mConsuNetId[it->first]);
			mapGoods[iIndex++] = ptrGoods;
		}
	}
	m_mGoodsMap[1] = mapGoods;

	mapGoods.clear();
	iIndex = 0;
	map<int, ChipSoulDateElem*> mapChipSoul = history->mChipSoulDate->getChipSoulMap();
	for(map<int, ChipSoulDateElem*>::iterator it = mapChipSoul.begin(); it != mapChipSoul.end(); it++)
	{
		HistoryGoods* ptrGoods = new HistoryGoods(it->first);
		ptrGoods->setNetID(it->second->id);
		mapGoods[iIndex++] = ptrGoods;
	}
	m_mGoodsMap[2] = mapGoods;
	updateCurGoods();
}

void BackpackLayer::updateGoodsMap()
{
	map<int, HistoryGoods*> mapGoods;
	int iIndex = 0;

	HistoryGoodsPtr ptreGoods = NULL;
	m_mGoodsMap[0].clear();
	for(int i = 0; i < history->goods->size(); i++)
	{
		ptreGoods = history->goods->elementAt(i);
		if (!ptreGoods->isFashion())
			mapGoods[iIndex++] = ptreGoods;
	}
	m_mGoodsMap[0] = mapGoods;

	mapGoods = m_mGoodsMap[1];
	iIndex = 0;
	for(map<int, int>::iterator it = history->m_mConsuNum.begin(); it != history->m_mConsuNum.end(); it++)
	{
		if(it->second)
		{
			if(mapGoods[iIndex])
			{
				HistoryGoods hisGoods(it->first);
				hisGoods.setNetID(history->m_mConsuNetId[it->first]);
				mapGoods[iIndex++]->setGood(&hisGoods);
			}
			else
			{
				HistoryGoods* ptrGoods = new HistoryGoods(it->first);
				ptrGoods->setNetID(history->m_mConsuNetId[it->first]);
				mapGoods[iIndex++] = ptrGoods;
			}
		}
	}
	for(int i = mapGoods.size() - 1; i >= iIndex; i--)
	{
		SAFE_DELETE(mapGoods[i]);
		mapGoods.erase(i);
	}
	m_mGoodsMap[1] = mapGoods;

	mapGoods = m_mGoodsMap[2];
	iIndex = 0;
	map<int, ChipSoulDateElem*> mapChipSoul = history->mChipSoulDate->getChipSoulMap();
	for(map<int, ChipSoulDateElem*>::iterator it = mapChipSoul.begin(); it != mapChipSoul.end(); it++)
	{
		if(mapGoods[iIndex])
		{
			HistoryGoods hisGoods(it->first);
			hisGoods.setNetID(it->second->id);
			mapGoods[iIndex++]->setGood(&hisGoods);
		}
		else
		{
			HistoryGoods* ptrGoods = new HistoryGoods(it->first);
			ptrGoods->setNetID(it->second->id);
			mapGoods[iIndex++] = ptrGoods;
		}
	}
	for(int i = mapGoods.size() - 1; i >= iIndex; i--)
	{
		SAFE_DELETE(mapGoods[i]);
		mapGoods.erase(i);
	}
	m_mGoodsMap[2] = mapGoods;
	updateCurGoods();
}
