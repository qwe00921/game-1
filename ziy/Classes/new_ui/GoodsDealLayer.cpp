//
//  GoodsDealLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/3/23.
//
//

#include "GoodsDealLayer.h"
#include "Defines.h"
#include "Common.h"
#include "main.h"
#include "PopLayer.h"
#include "CocoMd5.h"
#include "ExpFruitLayer.h"
#include "GoldExchangeLayer.h"
#include "SuitInfoLayer.h"
#include "ActivityInfo.h"

enum
{
	REQ_BUY = 0,
	REQ_MERGE,
	REQ_USE,
	REQ_EXCH,
	REQ_STORE,
	REQ_USES,
	REQ_USE_ADDABILITYGOODS
};

GoodsDealLayer* GoodsDealLayer::create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate, int iType /*= DEAL_SELL*/)
{
	GoodsDealLayer* pRet = new GoodsDealLayer();
	if(pRet&&pRet->init(ptrGoods, ptrDelegate, iType))
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

bool GoodsDealLayer::init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate, int iType)
{
	if (!Layer::init())
	{
		return false;
	}

	m_iNoticeIndex = 0;
	m_iDealType = iType;
	m_ptrSell = nullptr;
	m_ptrGoods = new HistoryGoods(ptrGoods);
	m_pDelegate = ptrDelegate;
	m_iTotalNum = m_ptrGoods->getStoreNum();

	initControl();
	return true;
}

GoodsDealLayer* GoodsDealLayer::create(SellGoods* pShopGoods, EventDelegate* ptrDelegate)
{
	GoodsDealLayer* pRet = new GoodsDealLayer();
	if(pRet&&pRet->init(pShopGoods, ptrDelegate))
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

bool GoodsDealLayer::init(SellGoods* ptrSell, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_iNoticeIndex = 0;
	m_pDelegate = ptrDelegate;
	m_ptrSell = ptrSell;

	m_ptrGoods = new HistoryGoods(ptrSell->getGoodsId());
	m_iKeyId = ptrSell->getKeyId();
	m_iTotalNum = ptrSell->getCount();
	m_iDealPrice = ptrSell->getPrice();
	m_strPath = ptrSell->getIconPath();
	m_iPayType = ptrSell->getPayType();
	m_iDealType = ptrSell->getDealType();
	if (ptrSell->getDiscount())
	{
		m_iDealPrice = m_iDealPrice * ptrSell->getDiscount() / 10;
	}

	initControl();
	return true;
}

GoodsDealLayer::GoodsDealLayer()
{

}

GoodsDealLayer::~GoodsDealLayer()
{
	SAFE_DELETE(m_ptrGoods);
}


bool GoodsDealLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	return true;
}

void GoodsDealLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void GoodsDealLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	Rect rectBG = m_pDealBG->getBoundingBox();
	if (!rectBG.containsPoint(convertToNodeSpace(touch->getLocation())) && !m_iNoticeIndex) //显示提示时候不移除界面
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		this->removeFromParent();
	}
}

void GoodsDealLayer::onEnter()
{
	Layer::onEnter();
}

void GoodsDealLayer::onExit()
{
	Layer::onExit();
}



void GoodsDealLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	m_pDealBG = ReadSpriteName("new_ui/img_global/info_bg1.png");
	m_pDealBG->setPosition(point);
	this->addChild(m_pDealBG);

	//ICON底框
	Sprite* imgBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBG->setPosition(Vec2(point.x - 90, point.y + 132));
	this->addChild(imgBG);

	//物品ICON
	string imgStr = history->GetGoodsImagePath(m_ptrGoods->good_id);
	Sprite* imgGoods = ReadSpriteNormal(imgStr.c_str());
	imgGoods->setPosition(Vec2(point.x - 90, point.y + 132));
	this->addChild(imgGoods);

	if(m_ptrGoods->isSoul())
	{
		//武魂
		Sprite* spSoul = ReadSpriteName("new_ui/hero/icon_soul.png");
		spSoul->setPosition(Vec2(point.x - 112, point.y + 154));
		this->addChild(spSoul);
	}
	else if(m_ptrGoods->isPiece())
	{
		//碎片
		Sprite* spPiece = ReadSpriteName("new_ui/img_global/icon_sp.png");
		spPiece->setPosition(Vec2(point.x - 112, point.y + 154));
		this->addChild(spPiece);
	}

	if(m_iTotalNum > 1)
	{
		//数目
		Sprite* spNum = ITools::getInstance()->getNumSprite(m_iTotalNum, true);
		spNum->setAnchorPoint(Vec2(1.0f, 0.0f));
		spNum->setPosition(Vec2(point.x - 62, point.y + 103));
		this->addChild(spNum);
	}

	//名称
	Label* nameLab = LabelSystemFont(m_ptrGoods->GetGoodsName(), 18);
	nameLab->setColor(Color3B(0xff, 0xff, 0x99));
	nameLab->setAnchorPoint(Vec2(0.0f, 0.5f));
	nameLab->setPosition(Vec2(point.x - 50, point.y + 154));

	this->addChild(nameLab);

	if (m_ptrGoods->iQuality)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));

		int iQuality = trData->strengthen_consume[m_ptrGoods->iQuality - 1].quality;
		if(iQuality)
		{
			nameLab->setColor(ITools::getInstance()->getPZColor(iQuality));
		}

		__String* strArmat = __String::createWithFormat("q%d", iQuality);
		__String* strStar = __String::createWithFormat("new_ui/img_number/star_b_%d.png", m_ptrGoods->iQuality);

		Sprite* spStar = ReadSpriteName(strStar->getCString());
		spStar->setPosition(Vec2(point.x - 90, point.y + 112));
		this->addChild(spStar);

		Armature* armat = Armature::create("special_eff");
		armat->setPosition(Vec2(point.x - 91, point.y + 131));
		armat->getAnimation()->play(strArmat->getCString());
		this->addChild(armat);
	}

	//类型标签
	Label* labType = LabelSystemFont(StringUtils::format("%s:", LL("labtype")), 16);
	labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	labType->setPosition(Vec2(point.x - 50, point.y + 131));
	labType->setColor(Color3B(0x66, 0, 0));
	this->addChild(labType);

	//类型文字
	Label* labTypeVal = LabelSystemFont(m_ptrGoods->GetAttributeInfo(), 16);
	labTypeVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTypeVal->setPosition(Vec2(point.x - 50 + labType->getContentSize().width, point.y + 131));
	this->addChild(labTypeVal);

	if(m_ptrGoods->getStoneID())
	{
		//附魔标签
		Label* labEnchant = LabelSystemFont(StringUtils::format("%s:", LL("labenchant")), 16);
		labEnchant->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEnchant->setPosition(Vec2(point.x - 50, point.y + 109));
		labEnchant->setColor(Color3B(0x66, 0, 0));
		this->addChild(labEnchant);

		//附魔文字
		HistoryGoods hisGoods(m_ptrGoods->getStoneID());
		Label* labEnchVal = LabelSystemFont(hisGoods.GetGoodsName(), 16);
		labEnchVal->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEnchVal->setPosition(Vec2(point.x - 50 + labEnchant->getContentSize().width, point.y + 109));
		labEnchVal->setColor(Color3B::RED);
		this->addChild(labEnchVal);
	}

	int iOffsetLine = 0; //偏移的行数
	int iResultId = 0;
	if(m_ptrGoods->isPiece())
	{
		//合成标签
		Label* labMerge = LabelSystemFont(__String::createWithFormat("%s:", LL("hecheng"))->getCString(), 16);
		labMerge->setAnchorPoint(Vec2(0.0f, 0.5f));
		labMerge->setPosition(Vec2(point.x - 123, point.y + 86 - iOffsetLine * 20));
		labMerge->setColor(Color3B(0x66, 0, 0));
		this->addChild(labMerge);

		//合成数值
		iResultId = trData->getMakeGoodsId(m_ptrGoods->good_id);
		int iNeedNum = trData->m_mMergeData[iResultId]->mMaterial[m_ptrGoods->good_id];
		__String* strMerge = __String::createWithFormat("%d/%d (%s)", m_ptrGoods->getStoreNum(), iNeedNum, m_ptrGoods->getGoodAttr()->strInfo.c_str());
		Label* labMergeVal = LabelSystemFont(strMerge->getCString(), 16);
		labMergeVal->setAnchorPoint(Vec2(0.0f, 0.5f));
		labMergeVal->setPosition(Vec2(point.x - 123 + labMerge->getContentSize().width, point.y + 86 - iOffsetLine * 20));
		this->addChild(labMergeVal);
		iOffsetLine++;
	}

	HistoryGoods* ptrGoods = m_ptrGoods;
	HistoryGoods hisGoods(iResultId);
	if(hisGoods.isNormalGood())
	{
		ptrGoods = &hisGoods;
	}

	if(ptrGoods->isSpecial())
	{
		//特性标签
		Label* labEffect = LabelSystemFont(__String::createWithFormat("%s:", LL("texing"))->getCString(), 16);
		labEffect->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEffect->setPosition(Vec2(point.x - 123, point.y + 86 - iOffsetLine * 20));
		labEffect->setColor(Color3B(0x66, 0, 0));
		this->addChild(labEffect);

		//特性值
		__String* strEffects = __String::createWithFormat("%s (%s)", ptrGoods->getSpecialEffectName(), ptrGoods->getSpecialEffectInfo());
		Label* labEffectVal = LabelSystemFont(strEffects->getCString(), 16);
		labEffectVal->setAnchorPoint(Vec2(0.0f, 0.5f));
		labEffectVal->setPosition(Vec2(point.x - 123 + labEffect->getContentSize().width, point.y + 86 - iOffsetLine * 20));
		labEffectVal->setColor(Color3B::YELLOW);
		this->addChild(labEffectVal);

		iOffsetLine++;
	}

	if(m_ptrGoods->getAttributeText() != "")
	{
		//攻击标签
		Label* labPower = LabelSystemFont(__String::createWithFormat("%s:", m_ptrGoods->getAttributeText().c_str())->getCString(), 16);
		labPower->setAnchorPoint(Vec2(0.0f, 0.5f));
		labPower->setPosition(Vec2(point.x - 123, point.y + 86 - iOffsetLine * 20));
		labPower->setColor(Color3B(0x66, 0, 0));
		this->addChild(labPower);

		//攻击值普通
		Label* labPowerVal1 = LabelSystemFont(IntToString(m_ptrGoods->GetGoodsPower()), 16);
		labPowerVal1->setAnchorPoint(Vec2(0.0f, 0.5f));
		labPowerVal1->setColor(Color3B::YELLOW);
		labPowerVal1->setPosition(Vec2(point.x - 123 + labPower->getContentSize().width, point.y + 86 - iOffsetLine * 20));
		this->addChild(labPowerVal1);

		if (m_ptrGoods->iQuality)
		{
			//攻击值强化
			Label* labPowerVal2 = LabelSystemFont(IntToString_Sign(m_ptrGoods->getStarAddVal()), 16);
			labPowerVal2->setAnchorPoint(Vec2(0.0f, 0.5f));
			labPowerVal2->setColor(Color3B::GREEN);
			labPowerVal2->setPosition(Vec2(point.x - 123 + labPower->getContentSize().width + labPowerVal1->getContentSize().width, point.y + 86 - iOffsetLine * 20));
			this->addChild(labPowerVal2);
		}

		iOffsetLine++;
	}

	if(m_ptrGoods->getSpeAttrNum())
	{
		//强化标签
		Label* labStreng = LabelSystemFont(__String::createWithFormat("%s:", LL("qianghua"))->getCString(), 16);
		labStreng->setAnchorPoint(Vec2(0.0f, 0.5f));
		labStreng->setPosition(Vec2(point.x - 123, point.y + 86 - iOffsetLine * 20));
		labStreng->setColor(Color3B(0x66, 0, 0));
		this->addChild(labStreng);

		map<int, string> mStarText;
		map<int, Color3B> mccColor;

		mStarText[0] = LL("qhdsanxjhtssx");
		mStarText[1] = LL("qhdliuxjhtssx");
		mStarText[2] = LL("qhdshixjhtssx");
		mccColor[0] = Color3B::WHITE;
		mccColor[1] = Color3B::WHITE;
		mccColor[2] = Color3B::WHITE;

		for(UInt i = 0; i < m_ptrGoods->GetStrengEffect().size(); i++)
		{
			int type = m_ptrGoods->GetStrengEffect()[i];
			for(int j = 0; j < trData->strengthen_property_num; j++)
			{
				if(type == trData->strengthen_property[j].id)
				{
					mStarText[i] = trData->strengthen_property[j].name;
					mccColor[i] = ITools::getInstance()->getPZColor(trData->strengthen_property[j].quality);
				}
			}
		}

		//强化属性
		for(int i = 0; i < m_ptrGoods->getSpeAttrNum(); i++)
		{
			Label* labStrengVal = LabelSystemFont(mStarText[i].c_str(), 16);
			labStrengVal->setColor(mccColor[i]);
			labStrengVal->setAnchorPoint(Vec2(0.0f, 0.5f));
			labStrengVal->setPosition(Vec2(point.x - 123 + labStreng->getContentSize().width, point.y + 86 - iOffsetLine * 20));
			this->addChild(labStrengVal);
			iOffsetLine++;
		}
	}

	//物品属性
	int iOffsetInfo = iOffsetLine ? 8 : 0;
	Label* labInfo = LabelSystemFont(ptrGoods->getGoodAttr()->strInfo.c_str(), 16);
	labInfo->setAnchorPoint(Vec2(0.0f, 1.0f));
	labInfo->setPosition(Vec2(point.x - 123, point.y + 92 - iOffsetLine * 20 - iOffsetInfo));
	labInfo->setDimensions(240, 110);
	labInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	this->addChild(labInfo);

	if (m_iDealType == DEAL_BUY || m_iDealType == DEAL_EXCH || m_iDealType == DEAL_STORE)
	{
		//价格背景
		Sprite* goldBG = ReadSpriteName("new_ui/shop/shop_bg3.png");
		goldBG->setPosition(Vec2(point.x, point.y - 115));
		this->addChild(goldBG);

		//价格ICON
		Sprite* goldIcon = ReadSpriteName(m_strPath.c_str());
		goldIcon->setPosition(Vec2(point.x - 48, point.y - 115));
		this->addChild(goldIcon);

		//价格金额
		Label* labPrice = LabelSystemFont(IntToString(m_iDealPrice), 16);
		labPrice->setColor(Color3B(0xff, 0xff, 0));
		labPrice->setPosition(Vec2(point.x, point.y - 115));
		this->addChild(labPrice);
	}
	else if (m_iDealType == DEAL_MAKE)
	{
		//警告
		Label* labWarn = LabelSystemFont(LL("makewarning"), 16);
		labWarn->setColor(Color3B::RED);
		labWarn->setPosition(Vec2(point.x, point.y - 118));
		this->addChild(labWarn);
	}

	//按钮菜单
	m_pMenu = Menu::create();
	m_pMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pMenu);

	//购买
	Sprite* spBuyN = ReadSpriteName("new_ui/img_global/global_ban9.png");
	Sprite* spBuyTextN = ReadSpriteName("new_ui/img_text/imgtxt5_1.png");
	spBuyTextN->setPosition(Vec2(spBuyN->getContentSize().width / 2, spBuyN->getContentSize().height / 2));
	spBuyN->addChild(spBuyTextN);
	Sprite* spBuyS = ReadSpriteName("new_ui/img_global/global_ban10.png");
	Sprite* spBuyTextS = ReadSpriteName("new_ui/img_text/imgtxt5_2.png");
	spBuyTextS->setPosition(Vec2(spBuyS->getContentSize().width / 2, spBuyS->getContentSize().height / 2));
	spBuyS->addChild(spBuyTextS);
	MenuItemSprite* btnBuy = MenuItemSprite::create(spBuyN, spBuyS, CC_CALLBACK_1(GoodsDealLayer::callBackBuy, this));
	btnBuy->setPosition(point.x, point.y - 151);

	//出售
	Sprite* spSellN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spSellTextN = ReadSpriteName("new_ui/img_text/shop_txt3_3.png");
	spSellTextN->setPosition(Vec2(spSellN->getContentSize().width / 2, spSellN->getContentSize().height / 2));
	spSellN->addChild(spSellTextN);
	Sprite* spSellS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spSellTextS = ReadSpriteName("new_ui/img_text/shop_txt3_4.png");
	spSellTextS->setPosition(Vec2(spSellS->getContentSize().width / 2, spSellS->getContentSize().height / 2));
	spSellS->addChild(spSellTextS);
	MenuItemSprite* btnSell = MenuItemSprite::create(spSellN, spSellS, CC_CALLBACK_1(GoodsDealLayer::callBackSell, this));
	btnSell->setPosition(point.x - 65, point.y - 151);

	//使用
	Sprite* spUseN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spUseTextN = ReadSpriteName("new_ui/img_text/shop_txt3_7.png");
	spUseTextN->setPosition(Vec2(spUseN->getContentSize().width / 2, spUseN->getContentSize().height / 2));
	spUseN->addChild(spUseTextN);
	Sprite* spUseS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spUseTextS = ReadSpriteName("new_ui/img_text/shop_txt3_8.png");
	spUseTextS->setPosition(Vec2(spUseS->getContentSize().width / 2, spUseS->getContentSize().height / 2));
	spUseS->addChild(spUseTextS);
	MenuItemSprite* btnUse = MenuItemSprite::create(spUseN, spUseS, CC_CALLBACK_1(GoodsDealLayer::callBackUse, this));
	btnUse->setPosition(point.x, point.y - 151);

	//使用10次
	Sprite* spUse10N = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spUse10TextN = ReadSpriteName("new_ui/img_text/shop_txt3_15.png");
	spUse10TextN->setPosition(Vec2(spUse10N->getContentSize().width / 2, spUse10N->getContentSize().height / 2));
	spUse10N->addChild(spUse10TextN);
	Sprite* spUse10S = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spUse10TextS = ReadSpriteName("new_ui/img_text/shop_txt3_16.png");
	spUse10TextS->setPosition(Vec2(spUse10S->getContentSize().width / 2, spUse10S->getContentSize().height / 2));
	spUse10S->addChild(spUse10TextS);
	MenuItemSprite* btnUse10 = MenuItemSprite::create(spUse10N, spUse10S, CC_CALLBACK_1(GoodsDealLayer::callBackUses, this));
	btnUse10->setPosition(point.x + 65, point.y - 151);

	//合成
	Sprite* spMergeN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spMergeTextN = ReadSpriteName("new_ui/img_text/shop_txt3_5.png");
	spMergeTextN->setPosition(Vec2(spMergeN->getContentSize().width / 2, spMergeN->getContentSize().height / 2));
	spMergeN->addChild(spMergeTextN);
	Sprite* spMergeS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spMergeTextS = ReadSpriteName("new_ui/img_text/shop_txt3_6.png");
	spMergeTextS->setPosition(Vec2(spMergeS->getContentSize().width / 2, spMergeS->getContentSize().height / 2));
	spMergeS->addChild(spMergeTextS);
	MenuItemSprite* btnMerge = MenuItemSprite::create(spMergeN, spMergeS, CC_CALLBACK_1(GoodsDealLayer::callBackMerge, this));
	btnMerge->setPosition(point.x + 65, point.y - 151);

	//分解
	Sprite* spBreakN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spBreakTextN = ReadSpriteName("new_ui/img_text/shop_txt3_9.png");
	spBreakTextN->setPosition(Vec2(spBreakN->getContentSize().width / 2, spBreakN->getContentSize().height / 2));
	spBreakN->addChild(spBreakTextN);
	Sprite* spBreakS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spBreakTextS = ReadSpriteName("new_ui/img_text/shop_txt3_10.png");
	spBreakTextS->setPosition(Vec2(spBreakS->getContentSize().width / 2, spBreakS->getContentSize().height / 2));
	spBreakS->addChild(spBreakTextS);
	MenuItemSprite* btnBreak = MenuItemSprite::create(spBreakN, spBreakS, CC_CALLBACK_1(GoodsDealLayer::callBackBreak, this));
	btnBreak->setPosition(point.x + 65, point.y - 151);

	//制作
	Sprite* spMakeN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spMakeTextN = ReadSpriteName("new_ui/img_text/imgtxt7_5.png");
	spMakeTextN->setPosition(Vec2(spMakeN->getContentSize().width / 2, spMakeN->getContentSize().height / 2));
	spMakeN->addChild(spMakeTextN);
	Sprite* spMakeS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spMakeTextS = ReadSpriteName("new_ui/img_text/imgtxt7_6.png");
	spMakeTextS->setPosition(Vec2(spMakeS->getContentSize().width / 2, spMakeS->getContentSize().height / 2));
	spMakeS->addChild(spMakeTextS);
	MenuItemSprite* btnMake = MenuItemSprite::create(spMakeN, spMakeS, CC_CALLBACK_1(GoodsDealLayer::callBackMake, this));
	btnMake->setPosition(point.x, point.y - 151);

	//兑换
	Sprite* spExchN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spExchTextN = ReadSpriteName("new_ui/img_text/imgtxt5_5.png");
	spExchTextN->setPosition(Vec2(spExchN->getContentSize().width / 2, spExchN->getContentSize().height / 2));
	spExchN->addChild(spExchTextN);
	Sprite* spExchS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spExchTextS = ReadSpriteName("new_ui/img_text/imgtxt5_6.png");
	spExchTextS->setPosition(Vec2(spExchS->getContentSize().width / 2, spExchS->getContentSize().height / 2));
	spExchS->addChild(spExchTextS);
	MenuItemSprite* btnExch = MenuItemSprite::create(spExchN, spExchS, CC_CALLBACK_1(GoodsDealLayer::callBackExch, this));
	btnExch->setPosition(point.x, point.y - 151);

	//套装
	Sprite* spSuitN = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
	Sprite* spSuitS = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
	spSuitS->setScale(0.9f);
	MenuItemSprite* btnSuit = MenuItemSprite::create(spSuitN, spSuitS, CC_CALLBACK_1(GoodsDealLayer::callBackSuit, this));
	spSuitS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spSuitS->setPosition(Vec2(spSuitS->getContentSize().width / 2, spSuitS->getContentSize().height / 2));
	btnSuit->setPosition(point.x + 86, point.y + 140);

	if (m_ptrGoods->isSuit())
	{
		m_pMenu->addChild(btnSuit);
	}

	if(m_iDealType == DEAL_BUY || m_iDealType == DEAL_STORE)
	{
		m_pMenu->addChild(btnBuy);
	}
	else if (m_iDealType == DEAL_EXCH)
	{
		m_pMenu->addChild(btnExch);
	}
	else if (m_iDealType == DEAL_MAKE || m_iDealType == DEAL_ENCHANT)
	{
		m_pMenu->addChild(btnMake);
	}
	else if(m_ptrGoods->isGift()) //礼包
	{
		m_pMenu->addChild(btnUse);
		m_pMenu->addChild(btnUse10);
		btnUse->setPosition(point.x - 65, point.y - 151);
	}
	else if (m_ptrGoods->isAddAbilityGoods())
	{
		m_pMenu->addChild(btnUse);
	}
	else if (!m_ptrGoods->isOnly())
	{
		m_pMenu->addChild(btnSell);
		if(m_ptrGoods->iQuality)
		{
			m_pMenu->addChild(btnBreak);
		}
		else if(m_ptrGoods->isExpFruit())
		{
			m_pMenu->addChild(btnUse);
			btnUse->setPosition(point.x + 65, point.y - 151);
		}
		else
		{
			btnSell->setPosition(point.x, point.y - 151);
		}
	}
}

void GoodsDealLayer::callBackSuit(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(SuitInfoLayer::create(m_ptrGoods->good_id));
}

void GoodsDealLayer::callBackBuy(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	HistoryGoods hisGoods(m_ptrSell->getGoodsId());
	if(!hisGoods.isGetEnable())
	{
		ITools::getInstance()->createGlobalPrompt(LL("wufaduihuan"), false);
		return;
	}

	if (m_ptrSell->getPayType() == PAY_TYPE_GOLD)
	{
		if(history->money < m_iDealPrice)
		{
			ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(m_pDelegate));
			return;
		}
	}
	else if (m_ptrSell->getPayType() == PAY_TYPE_JADE)
	{
		if(history->m_iJade < m_iDealPrice)
		{
			ITools::getInstance()->createGlobalPrompt(LL("gouyubuzu"), false);
			return;
		}
	}
	else if (m_ptrSell->getPayType() == PAY_TYPE_LONG)
	{
		if(history->m_iJadeSeal < m_iDealPrice)
		{
			ITools::getInstance()->createGlobalPrompt(LL("longbibuzu"), false);
			return;
		}
	}

	if (m_ptrSell->getDealType() == DEAL_BUY)
	{
		dealHttpRequest(REQ_BUY);
	}
	else if(m_ptrSell->getDealType() == DEAL_STORE)
	{
		dealHttpRequest(REQ_STORE);
	}
}

void GoodsDealLayer::callBackSell(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->getParent()->addChild(SellPopupLayer::create(m_ptrGoods, m_pDelegate));
	this->removeFromParent();
}

void GoodsDealLayer::callBackUse(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if(m_ptrGoods->isGift())
	{
		dealHttpRequest(REQ_USE);
	}
	else if(m_ptrGoods->isExpFruit())
	{
		this->getParent()->addChild(ExpFruitLayer::create(m_ptrGoods->good_id, m_pDelegate));
		this->removeFromParent();
	}
	else if (m_ptrGoods->isAddAbilityGoods())
	{
		dealHttpRequest(REQ_USE_ADDABILITYGOODS);
	}
}

void GoodsDealLayer::callBackUses(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_ptrGoods->isGift())
	{
		dealHttpRequest(REQ_USES);
	}
}

void GoodsDealLayer::callBackMerge(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iResultId = trData->getMakeGoodsId(m_ptrGoods->good_id);
	int iNeedNum = trData->m_mMergeData[iResultId]->mMaterial[m_ptrGoods->good_id];
	if(m_ptrGoods->getStoreNum() < iNeedNum)
	{
		ITools::getInstance()->createGlobalPrompt(LL("suipianbuzu"), false);
		return;
	}

	dealHttpRequest(REQ_MERGE);
}

void GoodsDealLayer::callBackBreak(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->getParent()->addChild(BreakupLayer::create(m_ptrGoods, m_pDelegate));
	this->removeFromParent();
}

void GoodsDealLayer::callBackMake(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_pDelegate->updateState();
	this->removeFromParent();
}

void GoodsDealLayer::callBackExch(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iGoodsId = BTActivity::getInstance()->getMaterialId(GoodsExchLayer::ms_iExchType);
	HistoryGoods hisGoods(iGoodsId);

	if(!hisGoods.isNormalGood() || hisGoods.getStoreNum() < m_iDealPrice)
	{
		ITools::getInstance()->createGlobalPrompt(LL("exchnotenough"), false);
		return;
	}

	if(!hisGoods.isGetEnable())
	{
		ITools::getInstance()->createGlobalPrompt(LL("wufaduihuan"), false);
		return;
	}

	dealHttpRequest(REQ_EXCH);
}

void GoodsDealLayer::dealHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case REQ_BUY:
	{
		strURL = "shopBuy";
		string validCode = md5.ToMD5(StringUtils::format("key=%d&gameToken=%s", m_iKeyId, strToken.c_str()));
		strData = StringUtils::format("{\"key\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iKeyId, strToken.c_str(), validCode.c_str());
		strTag = "REQ_DEAL_BUY";
	}
	break;
	case REQ_USE:
	{
		strURL = "useGood";
		string validCode = md5.ToMD5(StringUtils::format("id=%d&gameToken=%s", m_ptrGoods->getNetID(), strToken.c_str()));
		strData = StringUtils::format("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_DEAL_USE";
	}
	break;
	case REQ_USES:
	{
		strURL = "useGoods";
		string validCode = md5.ToMD5(StringUtils::format("id=%d&gameToken=%s", m_ptrGoods->getNetID(), strToken.c_str()));
		strData = StringUtils::format("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_DEAL_USES";
	}
	break;
	case REQ_MERGE:
	{
		strURL = "chipCompose";
		string validCode = md5.ToMD5(StringUtils::format("id=%d&gameToken=%s", m_ptrGoods->getNetID(), strToken.c_str()));
		strData = StringUtils::format("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_DEAL_MERGE";
	}
	break;
	case REQ_EXCH:
	{
		strURL = "exChangeGoods";
		string strType = BTActivity::getInstance()->getExchType(GoodsExchLayer::ms_iExchType);
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s&type=%s&gid=%d", strToken.c_str(), strType.c_str(), m_iKeyId));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"type\":\"%s\",\"gid\":%d,\"validateCode\":\"%s\"}", strToken.c_str(), strType.c_str(), m_iKeyId, validCode.c_str());
		strTag = "REQ_DEAL_EXCH";
	}
	break;
	case REQ_STORE:
	{
		strURL = "buyGoodsManItem";
		string validCode = md5.ToMD5(StringUtils::format("key=%d&type=%s&gameToken=%s", m_iKeyId, m_ptrSell->getStrType().c_str(), strToken.c_str()));
		strData = StringUtils::format("{\"key\":%d,\"type\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_iKeyId, m_ptrSell->getStrType().c_str(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_DEAL_BUY";
	}
	break;
	case REQ_USE_ADDABILITYGOODS:
	{
		strURL = "useGoodAddAttr";
		string validCode = md5.ToMD5(StringUtils::format("id=%d&gameToken=%s", m_ptrGoods->getNetID(), strToken.c_str()));
		strData = StringUtils::format("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), strToken.c_str(), validCode.c_str());
		strTag = "REQ_USE_ADDABILITYGOODS";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(GoodsDealLayer::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void GoodsDealLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("%s...", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if(strTag == "REQ_DEAL_BUY")
		{
			onBuySucceed(value["result"]);
		}
		else if(strTag == "REQ_DEAL_USE")
		{
			onUseSucceed(value["result"]);
		}
		else if (strTag == "REQ_DEAL_USES")
		{
			onUseSSucceed(value["result"]);
		}
		else if(strTag == "REQ_DEAL_MERGE")
		{
			onMergeSucceed(value["result"]);
		}
		else if(strTag == "REQ_DEAL_EXCH")
		{
			onExchSucceed(value["result"]);
		}
		else if (strTag == "REQ_USE_ADDABILITYGOODS")
		{
			onUseAddAbilityGoodsSuccess(value["result"]);
		}
	}
}

void GoodsDealLayer::onBuySucceed(Json::Value valRes)
{
	m_iNoticeIndex = 0;
	m_strNotice = history->onHttpGoodsAdd(valRes);
	this->schedule(schedule_selector(GoodsDealLayer::playGoodsNotice), 0.8f);

	m_pDelegate->updateShow(m_ptrSell->getIndex());
	this->setVisible(false);
}

void GoodsDealLayer::onUseSucceed(Json::Value valRes)
{
	m_iNoticeIndex = 0;
	m_strNotice = history->onHttpGoodsAdd(valRes);
	history->m_mConsuNum[m_ptrGoods->good_id]--;
	this->schedule(schedule_selector(GoodsDealLayer::playGoodsNotice), 0.8f);
	m_pDelegate->updateShow(0);
	this->setVisible(false);
}

void GoodsDealLayer::onUseAddAbilityGoodsSuccess(Json::Value valRes)
{
	int iHeroId = 0,iCount = 0,iGoodsId = 0;
	if (valRes["heroID"].isInt())
		iHeroId = valRes["heroID"].asInt();
	if (valRes["times"].isInt())
		iCount = valRes["times"].asInt();
	if (valRes["goodID"].isInt())
		iGoodsId = valRes["goodID"].asInt();

	if (iHeroId > 0)
	{
		history->actors[iHeroId - 1].setAddAbilityGoods(iGoodsId,iCount);
		history->actors[iHeroId - 1].UpdataGoodEffect();
	}

	history->m_mConsuNum[m_ptrGoods->good_id]--;

	m_iNoticeIndex = 0;
	map<int, int> mSeid = m_ptrGoods->getSeid();
	for (size_t i = 0; i < mSeid.size(); i++)
	{
		m_strNotice[i] = trData->m_mSeid[mSeid[i]]->Info;
	}

	this->schedule(schedule_selector(GoodsDealLayer::playGoodsNotice), 0.8f);
	m_pDelegate->updateShow(0);
	this->setVisible(false);
}

void GoodsDealLayer::onUseSSucceed(Json::Value valRes)
{
	map<int, int> mGoodsList,mMGoodsList;
	int iIndex = 0,iGoodsId = 0,iCount = 0;
	for (UInt i = 0; i < valRes["goods"].size(); i++)
	{
		iGoodsId = valRes["goods"][i]["goodID"].asInt();
		iCount = valRes["goods"][i]["count"].asInt();
		if (!(iGoodsId >= 10000 && iGoodsId <= 10004))
			mGoodsList[iGoodsId] += iCount;
	}

	if (valRes["gold"].isInt())
	{
		int iAddGold = valRes["gold"].asInt() - history->money;
		if (iAddGold > 0)
			m_strNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("jinbi"), iAddGold);
	}

	if (valRes["jade"].isInt())
	{
		int iAddJade = valRes["jade"].asInt() - history->m_iJade;
		if (iAddJade > 0)
			m_strNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("gouyu"), iAddJade);
	}

	if (valRes["improvedStone"].isInt())
	{
		int iAddStone = valRes["improvedStone"].asInt() - history->stone;
		if (iAddStone > 0)
			m_strNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("qianghuashi"), iAddStone);
	}

	if (valRes["jadeSeal"].isInt())
	{
		int iAddLong = valRes["jadeSeal"].asInt() - history->m_iJadeSeal;
		if (iAddLong > 0)
			m_strNotice[iIndex++] = StringUtils::format(LL("huodessdd"), LL("longbi"), iAddLong);
	}

	for (map<int, int>::iterator eIt = mGoodsList.begin(); eIt != mGoodsList.end(); eIt++)
	{
		HistoryGoods hisGoods(eIt->first);
		if(eIt->second)
			m_strNotice[iIndex++] = StringUtils::format(LL("huodess"), hisGoods.GetGoodsName(eIt->second));
	}

	m_iNoticeIndex = 0;
	history->onHttpGoodsAdd(valRes);
	history->m_mConsuNum[m_ptrGoods->good_id]-= 10;
	this->schedule(schedule_selector(GoodsDealLayer::playGoodsNotice), 0.8f);
	m_pDelegate->updateShow(0);
	this->setVisible(false);
}

void GoodsDealLayer::onMergeSucceed(Json::Value valRes)
{
	GoodsElem goodsElem;
	int iReduceNum = valRes["minus"].asInt();
	goodsElem.loadJson(valRes["good"]);

	HistoryGoods* ptrGoods = new HistoryGoods(&goodsElem);
	history->goods->addElement(ptrGoods);
	history->mChipSoulDate->ReduceEqSoulNumByGoodsId(m_ptrGoods->good_id, iReduceNum);

	__String* strName = __String::createWithFormat(LL("huodess"), ptrGoods->GetGoodsName());
	ITools::getInstance()->createGlobalPrompt(strName->getCString(), true);

	m_pDelegate->updateShow(0);
	this->removeFromParent();
}

void GoodsDealLayer::onExchSucceed(Json::Value valRes)
{
	//除去兑换消耗
	map<int, map<int, ActExchElem*> > mExchGoods = BTActivity::getInstance()->m_mExchGoods;
	map<int, int> mUseGoods = mExchGoods[GoodsExchLayer::ms_iExchType][m_iKeyId]->m_mUseGoods;
	for(map<int, int>::iterator it = mUseGoods.begin(); it != mUseGoods.end(); it++)
	{
		int iGoodsId = it->first;
		int iCount = it->second;
		history->packGoodsReduce(iGoodsId, history->m_mConsuNetId[iGoodsId], iCount);
	}

	m_iNoticeIndex = 0;
	m_strNotice = history->onHttpGoodsAdd(valRes);
	this->schedule(schedule_selector(GoodsDealLayer::playGoodsNotice), 0.8f);
	m_pDelegate->updateShow(0);
	this->setVisible(false);
}

void GoodsDealLayer::playGoodsNotice(float fDelay)
{
	if(m_iNoticeIndex < (int)m_strNotice.size())
	{
		ITools::getInstance()->createGlobalPrompt(m_strNotice[m_iNoticeIndex++].c_str(), true);
	}
	else
	{
		this->removeFromParent();
	}
}

SellPopupLayer::SellPopupLayer()
{

}

SellPopupLayer::~SellPopupLayer()
{
	SAFE_DELETE(m_ptrGoods);
}

SellPopupLayer* SellPopupLayer::create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate)
{
	SellPopupLayer* pRet = new SellPopupLayer();
	if(pRet&&pRet->init(ptrGoods, ptrDelegate))
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

bool SellPopupLayer::init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrGoods = new HistoryGoods(ptrGoods);
	m_pDelegate = ptrDelegate;
	m_iTotalNum = m_ptrGoods->getStoreNum();
	m_iCurNum = 1;

	initControl();
	updateMenuState();

	return true;
}

void SellPopupLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/shop/shop_sell_bg.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//按钮菜单
	m_pMenu = Menu::create();
	m_pMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pMenu);

	//ICON底框
	Sprite* imgBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBG->setPosition(Vec2(point.x - 90, point.y + 70));
	this->addChild(imgBG);

	//物品ICON
	string imgStr = history->GetGoodsImagePath(m_ptrGoods->good_id);
	Sprite* imgGoods = ReadSpriteNormal(imgStr.c_str());
	imgGoods->setPosition(Vec2(point.x - 90, point.y + 70));
	this->addChild(imgGoods);

	if(m_ptrGoods->isSoul())
	{
		//武魂
		Sprite* spSoul = ReadSpriteName("new_ui/hero/icon_soul.png");
		spSoul->setPosition(Vec2(point.x - 112, point.y + 92));
		this->addChild(spSoul);
	}
	else if(m_ptrGoods->isPiece())
	{
		//碎片
		Sprite* spPiece = ReadSpriteName("new_ui/img_global/icon_sp.png");
		spPiece->setPosition(Vec2(point.x - 112, point.y + 92));
		this->addChild(spPiece);
	}

	if(m_iTotalNum > 1)
	{
		//数目
		Sprite* spNum = ITools::getInstance()->getNumSprite(m_iTotalNum, true);
		spNum->setAnchorPoint(Vec2(1.0f, 0.0f));
		spNum->setPosition(Vec2(point.x - 62, point.y + 41));
		this->addChild(spNum);
	}

	//名称
	Label* nameLab = LabelSystemFont(m_ptrGoods->GetGoodsName(), 18);
	nameLab->setColor(Color3B(0xff, 0xff, 0x99));
	nameLab->setAnchorPoint(Vec2(0.0f, 0.5f));
	nameLab->setPosition(Vec2(point.x - 50, point.y + 88));

	this->addChild(nameLab);

	if(m_ptrGoods->iQuality)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));

		int iQuality = trData->strengthen_consume[m_ptrGoods->iQuality - 1].quality;
		if(iQuality)
		{
			nameLab->setColor(ITools::getInstance()->getPZColor(iQuality));
		}

		__String* strArmat = __String::createWithFormat("q%d", iQuality);
		__String* strStar = __String::createWithFormat("new_ui/img_number/star_b_%d.png", m_ptrGoods->iQuality);

		Sprite* spStar = ReadSpriteName(strStar->getCString());
		spStar->setPosition(Vec2(point.x - 90, point.y + 50));
		this->addChild(spStar);

		Armature* armat = Armature::create("special_eff");
		armat->setPosition(Vec2(point.x - 91, point.y + 69));
		armat->getAnimation()->play(strArmat->getCString());
		this->addChild(armat);
	}

	//类型标签
	Label* labType = LabelSystemFont(__String::createWithFormat("%s:", LL("labtype"))->getCString(), 16);
	labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	labType->setPosition(Vec2(point.x - 50, point.y + 62));
	labType->setColor(Color3B(0x66, 0, 0));
	this->addChild(labType);

	//类型文字
	Label* labTypeVal = LabelSystemFont(m_ptrGoods->GetAttributeInfo(), 16);
	labTypeVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTypeVal->setPosition(Vec2(point.x - 50 + labType->getContentSize().width, point.y + 62));
	this->addChild(labTypeVal);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(SellPopupLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(173+32, 130+12));
	m_pMenu->addChild(btnClose);

	//购买数背景
	Sprite* spNumBG = ReadSpriteName("new_ui/shop/shop_numBg.png");
	spNumBG->setPosition(point + Vec2(-30, -2));
	this->addChild(spNumBG);

	//购买数目
	m_labNum = LabelSystemFont("1/1", 16);
	m_labNum->setColor(Color3B(0xff, 0xff, 0));
	m_labNum->setPosition(Vec2(spNumBG->getContentSize().width / 2, spNumBG->getContentSize().height / 2));
	spNumBG->addChild(m_labNum);

	//购买-
	Sprite* spMinusN = ReadSpriteName("new_ui/shop/shop_minus.png");
	Sprite* spMinusS = ReadSpriteName("new_ui/shop/shop_minus_press.png");
	Sprite* spMinusD = ReadSpriteName("new_ui/shop/shop_minus_disable.png");
	m_btnMinus = MenuItemSprite::create(spMinusN, spMinusS, spMinusD, CC_CALLBACK_1(SellPopupLayer::callBackMinus, this));
	m_btnMinus->setPosition(point + Vec2(-100, -2));
	m_pMenu->addChild(m_btnMinus);

	//购买+
	Sprite* spPlusN = ReadSpriteName("new_ui/shop/shop_plus.png");
	Sprite* spPlusS = ReadSpriteName("new_ui/shop/shop_plus_press.png");
	Sprite* spPlusD = ReadSpriteName("new_ui/shop/shop_plus_disable.png");
	m_btnPlus = MenuItemSprite::create(spPlusN, spPlusS, spPlusD, CC_CALLBACK_1(SellPopupLayer::callBackPlus, this));
	m_btnPlus->setPosition(point + Vec2(40, -2));
	m_pMenu->addChild(m_btnPlus);

	//最大
	Sprite* spMaxN = ReadSpriteName("new_ui/shop/shop_max.png");
	Sprite* spMaxS = ReadSpriteName("new_ui/shop/shop_max_press.png");
	Sprite* spMaxD = ReadSpriteName("new_ui/shop/shop_max_disable.png");
	m_btnMax = MenuItemSprite::create(spMaxN, spMaxS, spMaxD, CC_CALLBACK_1(SellPopupLayer::callBackMax, this));
	m_btnMax->setPosition(point + Vec2(105, -2));
	m_pMenu->addChild(m_btnMax);
	
	//获得金币  
	Label* labMoney = LabelSystemFont(LL("getmoney"), 16);
	labMoney->setColor(Color3B(0xff, 0xff, 0));
	labMoney->setAnchorPoint(Vec2(0.0f, 0.5f));
	labMoney->setPosition(point + Vec2(-100, -53));
	this->addChild(labMoney);

	//价格背景
	Sprite* spGoldBG = ReadSpriteName("new_ui/shop/shop_bg3.png");
	spGoldBG->setPosition(point + Vec2(45, -53));
	this->addChild(spGoldBG);

	//价格ICON
	Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGoldIcon->setPosition(point + Vec2(-3, -53));
	this->addChild(spGoldIcon);

	//价格金额
	m_labPrice = LabelSystemFont("100", 16);
	m_labPrice->setColor(Color3B(0xff, 0xff, 0));
	m_labPrice->setPosition(point + Vec2(45, -53));
	this->addChild(m_labPrice);

	//确定出售
	Sprite* spSellN = ReadSpriteName("new_ui/img_global/global_ban13.png");
	Sprite* spSellTextN = ReadSpriteName("new_ui/img_text/shop_txt3_1.png");
	spSellTextN->setPosition(Vec2(spSellN->getContentSize().width / 2, spSellN->getContentSize().height / 2));
	spSellN->addChild(spSellTextN);
	Sprite* spSellS = ReadSpriteName("new_ui/img_global/global_ban14.png");
	Sprite* spSellTextS = ReadSpriteName("new_ui/img_text/shop_txt3_2.png");
	spSellTextS->setPosition(Vec2(spSellS->getContentSize().width / 2, spSellS->getContentSize().height / 2));
	spSellS->addChild(spSellTextS);
	MenuItemSprite* btnSell = MenuItemSprite::create(spSellN, spSellS, CC_CALLBACK_1(SellPopupLayer::callBackSell, this));
	btnSell->setPosition(point + Vec2(0, -107));
	m_pMenu->addChild(btnSell);
}


void SellPopupLayer::updateMenuState()
{
	__String* strNum = __String::createWithFormat("%d/%d", m_iCurNum, m_iTotalNum);
	m_labNum->setString(strNum->getCString());
	m_labPrice->setString(IntToString(m_ptrGoods->getPrice() * m_iCurNum));

	m_btnPlus->setEnabled(m_iCurNum != m_iTotalNum);
	m_btnMinus->setEnabled(m_iCurNum != 1);
	m_btnMax->setEnabled(m_iCurNum != m_iTotalNum);
}

void SellPopupLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void SellPopupLayer::callBackPlus(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iCurNum++;
	updateMenuState();
}

void SellPopupLayer::callBackMinus(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iCurNum--;
	updateMenuState();
}

void SellPopupLayer::callBackMax(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_iCurNum = m_iTotalNum;
	updateMenuState();
}

void SellPopupLayer::callBackSell(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	if (m_iCurNum > 0)
	{
		sellHttpRequest();
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("shujuyichang"), false);
	}
}

void SellPopupLayer::sellHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("gameToken=%s&id=%d&goodType=%d&count=%d", strToken.c_str(), m_ptrGoods->getNetID(), m_ptrGoods->getItId(), m_iCurNum);
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"id\":%d,\"goodType\":%d,\"count\":%d,\"validateCode\":\"%s\"}", strToken.c_str(), m_ptrGoods->getNetID(), m_ptrGoods->getItId(), m_iCurNum, validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("sellGood",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(SellPopupLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void SellPopupLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onSellSucceed(value["result"]);
	}
}

void SellPopupLayer::onSellSucceed(Json::Value valRes)
{
	int iAddGold = 0;
	if(valRes["gold"].isInt())
	{
		iAddGold = valRes["gold"].asInt() - history->money;
		history->money += iAddGold;
	}
	history->packGoodsReduce(m_ptrGoods->good_id, m_ptrGoods->getNetID(), m_iCurNum);

	string strText1 = __String::createWithFormat(LL("chushou"), m_ptrGoods->GetGoodsName(m_iCurNum))->getCString();
	string strText2 = __String::createWithFormat(LL("huodessdd"), LL("jinbi"), iAddGold)->getCString();
	strText1 += strText2;
	ITools::getInstance()->createGlobalPrompt(strText1.c_str(), true);

	m_pDelegate->updateShow(0);
	this->removeFromParent();
}

BreakupLayer::BreakupLayer()
{

}

BreakupLayer::~BreakupLayer()
{
	SAFE_DELETE(m_ptrGoods);

}

BreakupLayer* BreakupLayer::create(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate)
{
	BreakupLayer* pRet = new BreakupLayer();
	if(pRet&&pRet->init(ptrGoods, ptrDelegate))
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

bool BreakupLayer::init(HistoryGoods* ptrGoods, EventDelegate* ptrDelegate)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrGoods = new HistoryGoods(ptrGoods);
	m_pDelegate = ptrDelegate;

	initControl();

	return true;
}

void BreakupLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));
	int iPercent = history->m_iVIP ? 60 : 40; //返还比例

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spBG = ReadSpriteName("new_ui/shop/shop_sell_bg.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//ICON底框
	Sprite* imgBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBG->setPosition(Vec2(point.x - 90, point.y + 70));
	this->addChild(imgBG);

	//物品ICON
	string imgStr = history->GetGoodsImagePath(m_ptrGoods->good_id);
	Sprite* imgGoods = ReadSpriteNormal(imgStr.c_str());
	imgGoods->setPosition(Vec2(point.x - 90, point.y + 70));
	this->addChild(imgGoods);

	//名称
	Label* nameLab = LabelSystemFont(m_ptrGoods->GetGoodsName(), 18);
	nameLab->setColor(Color3B(0xff, 0xff, 0x99));
	nameLab->setAnchorPoint(Vec2(0.0f, 0.5f));
	nameLab->setPosition(Vec2(point.x - 50, point.y + 92));
	this->addChild(nameLab);

	//强化光环
	int iQuality = trData->strengthen_consume[m_ptrGoods->iQuality - 1].quality;
	if(iQuality)
	{
		nameLab->setColor(ITools::getInstance()->getPZColor(iQuality));
	}

	__String* strArmat = __String::createWithFormat("q%d", iQuality);
	__String* strStar = __String::createWithFormat("new_ui/img_number/star_b_%d.png", m_ptrGoods->iQuality);

	Sprite* spStar = ReadSpriteName(strStar->getCString());
	spStar->setPosition(Vec2(point.x - 90, point.y + 50));
	this->addChild(spStar);

	Armature* armat = Armature::create("special_eff");
	armat->setPosition(Vec2(point.x - 91, point.y + 69));
	armat->getAnimation()->play(strArmat->getCString());
	this->addChild(armat);

	//类型标签
	Label* labType = LabelSystemFont(__String::createWithFormat("%s:", LL("labtype"))->getCString(), 16);
	labType->setAnchorPoint(Vec2(0.0f, 0.5f));
	labType->setPosition(Vec2(point.x - 50, point.y + 67));
	labType->setColor(Color3B(0x66, 0, 0));
	this->addChild(labType);

	//类型文字
	Label* labTypeVal = LabelSystemFont(m_ptrGoods->GetAttributeInfo(), 16);
	labTypeVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labTypeVal->setPosition(Vec2(point.x - 50 + labType->getContentSize().width, point.y + 67));
	this->addChild(labTypeVal);

	//返还
	Label* labReturn = LabelSystemFont(LL("returnpercent"), 16);
	labReturn->setAnchorPoint(Vec2(0.0f, 0.5f));
	labReturn->setPosition(Vec2(point.x - 50, point.y + 44));
	labReturn->setColor(Color3B(0x66, 0, 0));
	this->addChild(labReturn);

	//返还比例
	Label* labRetVal = LabelSystemFont("40%", 16);
	labRetVal->setColor(Color3B::YELLOW);
	labRetVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	labRetVal->setPosition(Vec2(point.x - 50 + labReturn->getContentSize().width, point.y + 44));
	this->addChild(labRetVal);

	//会员特权
	Label* labRetVip = LabelSystemFont(LL("returnpercentvip"), 16);
	labRetVip->setAnchorPoint(Vec2(0.0f, 0.5f));
	labRetVip->setPosition(Vec2(point.x - 50, point.y + 21));
	labRetVip->setColor(Color3B(0x66, 0, 0));
	this->addChild(labRetVip);

	//会员比例
	Label* labRetVipVal = LabelSystemFont("+20%", 16);
	labRetVipVal->setAnchorPoint(Vec2(0.0f, 0.5f));
	if (history->m_iVIP)
	{
		labRetVipVal->setColor(Color3B::YELLOW);
	}
	else
	{
		labRetVipVal->setColor(Color3B(204, 204, 204));
	}
	labRetVipVal->setPosition(Vec2(point.x - 50 + labRetVip->getContentSize().width, point.y + 21));
	this->addChild(labRetVipVal);

	//强化石  
	Label* labStone = LabelSystemFont(LL("returnstone"), 16);
	labStone->setColor(Color3B(0xff, 0xff, 0));
	labStone->setAnchorPoint(Vec2(1.0f, 0.5f));
	labStone->setPosition(point + Vec2(-36, -15));
	this->addChild(labStone);

	//强化石背景
	Sprite* spLabBG1 = ReadSpriteName("new_ui/shop/shop_bg3.png");
	spLabBG1->setPosition(point + Vec2(45, -15));
	this->addChild(spLabBG1);

	//强化石ICON
	Sprite* spStone = ReadSpriteName("new_ui/img_global/icon_qhs.png");
	spStone->setPosition(point + Vec2(-3, -15));
	this->addChild(spStone);

	//强化石数量
	int iStoneTotal = trData->strengthen_consume[m_ptrGoods->iQuality - 1].iTotalGem;
	int iStoneRet = round((float)iStoneTotal * iPercent / 100);
	Label* labStoneVal = LabelSystemFont(IntToString(iStoneRet), 16);
	labStoneVal->setColor(Color3B(0xff, 0xff, 0));
	labStoneVal->setPosition(point + Vec2(45, -15));
	this->addChild(labStoneVal);

	//金币  
	Label* labMoney = LabelSystemFont(LL("returnmoney"), 16);
	labMoney->setColor(Color3B(0xff, 0xff, 0));
	labMoney->setAnchorPoint(Vec2(1.0f, 0.5f));
	labMoney->setPosition(point + Vec2(-36, -50));
	this->addChild(labMoney);

	//价格背景
	Sprite* spLabBG2 = ReadSpriteName("new_ui/shop/shop_bg3.png");
	spLabBG2->setPosition(point + Vec2(45, -50));
	this->addChild(spLabBG2);

	//金币ICON
	Sprite* spGoldIcon = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGoldIcon->setPosition(point + Vec2(-3, -50));
	this->addChild(spGoldIcon);

	//金币数量
	int iMoneyTotal = trData->strengthen_consume[m_ptrGoods->iQuality - 1].iTotalMoney;
	int iMoneyRet = round((float)iMoneyTotal * iPercent / 100);
	Label* labMoneyVal = LabelSystemFont(IntToString(iMoneyRet), 16);
	labMoneyVal->setColor(Color3B(0xff, 0xff, 0));
	labMoneyVal->setPosition(point + Vec2(45, -50));
	this->addChild(labMoneyVal);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(BreakupLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(173+32, 130+12));
	ptrMenu->addChild(btnClose);

	//确定分解
	Sprite* spBreakN = ReadSpriteName("new_ui/img_global/global_ban13.png");
	Sprite* spBreakTextN = ReadSpriteName("new_ui/img_text/shop_txt3_11.png");
	spBreakTextN->setPosition(Vec2(spBreakN->getContentSize().width / 2, spBreakN->getContentSize().height / 2));
	spBreakN->addChild(spBreakTextN);
	Sprite* spBreakS = ReadSpriteName("new_ui/img_global/global_ban14.png");
	Sprite* spBreakTextS = ReadSpriteName("new_ui/img_text/shop_txt3_12.png");
	spBreakTextS->setPosition(Vec2(spBreakS->getContentSize().width / 2, spBreakS->getContentSize().height / 2));
	spBreakS->addChild(spBreakTextS);
	MenuItemSprite* btnBreak = MenuItemSprite::create(spBreakN, spBreakS, CC_CALLBACK_1(BreakupLayer::callBackBreak, this));
	btnBreak->setPosition(point + Vec2(0, -107));
	ptrMenu->addChild(btnBreak);
}

void BreakupLayer::breakHttpRequest()
{
	string strToken = history->m_pLoginInfo->getGameToken();
	__String* data = __String::createWithFormat("gameToken=%s&id=%d", strToken.c_str(), m_ptrGoods->getNetID());
	CocosMD5 md5;
	string validCode = md5.ToMD5(data->getCString());
	__String* strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"id\":%d,\"validateCode\":\"%s\"}", strToken.c_str(), m_ptrGoods->getNetID(), validCode.c_str());
	CCLOG("%s", strData->getCString());

	ITools::getInstance()->connectingNetwork("decompositionEP",
		HttpRequest::Type::POST,
		this, 
		callfuncND_selector(BreakupLayer::onHttpRequestCompleted),
		strData->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void BreakupLayer::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		onBreakSucceed(value["result"]);
	}
}

void BreakupLayer::onBreakSucceed(Json::Value valRes)
{
	int iAddGold = 0;
	if(valRes["gold"].isInt())
	{
		iAddGold = valRes["gold"].asInt() - history->money;
		history->money += iAddGold;
	}

	int iAddStone = 0;
	if(valRes["improvedStone"].isInt())
	{
		iAddStone = valRes["improvedStone"].asInt() - history->stone;
		history->stone += iAddStone;
	}

	string strNotice = __String::createWithFormat(LL("huodessdd"), LL("jinbi"), iAddGold)->getCString();
	if (iAddStone)
	{
		strNotice = __String::createWithFormat("%s%s%sx%d", strNotice.c_str(), LL("dunhao"), LL("qianghuashi"), iAddStone)->getCString();
	}
	ITools::getInstance()->createGlobalPrompt(strNotice.c_str(), true);

	//去除分解物品
	for(int i = 0; i < history->goods->size(); i++)
	{
		if(history->goods->elementAt(i)->getNetID() == m_ptrGoods->getNetID())
		{
			history->goods->removeElementAt(i);
			break;
		}
	}

	m_pDelegate->updateShow(0);
	this->removeFromParent();
}

void BreakupLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void BreakupLayer::callBackBreak(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	breakHttpRequest();
}
