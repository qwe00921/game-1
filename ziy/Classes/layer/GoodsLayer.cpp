//
//  GoodsLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/23.
//
//

#include "GoodsLayer.h"
#include "SuitInfoLayer.h"

GoodsLayer* GoodsLayer::create(int iGoodsId)
{
	GoodsLayer* pRet = new GoodsLayer();
	if(pRet&&pRet->init(iGoodsId))
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

GoodsLayer* GoodsLayer::create(HistoryGoods* ptrGoods)
{
	GoodsLayer* pRet = new GoodsLayer();
	if(pRet&&pRet->init(ptrGoods))
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

bool GoodsLayer::init(int iGoodsId)
{
	if(!Layer::init())
	{
		return false;
	}

	m_ptrGoods = new HistoryGoods(iGoodsId);

	initControl();
	return true;
}

bool GoodsLayer::init(HistoryGoods* ptrGoods)
{
	if (!Layer::init())
	{
		return false;
	}

	m_ptrGoods = new HistoryGoods(ptrGoods);

	initControl();
	return true;
}

GoodsLayer::GoodsLayer()
{

}

GoodsLayer::~GoodsLayer()
{
	SAFE_DELETE(m_ptrGoods);
}

bool GoodsLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}

void GoodsLayer::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void GoodsLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	if (!m_ptrGoods->isSuit() || !m_spLayBG->getBoundingBox().containsPoint(convertToNodeSpace(touch->getLocation())))
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		this->removeFromParent();
	}
}

void GoodsLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	m_spLayBG = ReadSpriteName("new_ui/img_global/info_bg1.png");
	m_spLayBG->setPosition(point);
	this->addChild(m_spLayBG);

	//ICON底框
	Sprite* imgBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	imgBG->setPosition(Vec2(point.x - 90, point.y + 132));
	this->addChild(imgBG);

	//物品ICON
	Sprite* imgGoods = history->getGoodsSprite(m_ptrGoods->good_id);
	imgGoods->setPosition(Vec2(point.x - 90, point.y + 132));
	this->addChild(imgGoods);

	//名称
	Label* nameLab = LabelSystemFont(m_ptrGoods->GetGoodsName(), 18);
	nameLab->setColor(Color3B(0xff, 0xff, 0x99));
	nameLab->setAnchorPoint(Vec2(0.0f, 0.5f));
	nameLab->setPosition(Vec2(point.x - 50, point.y + 154));
	this->addChild(nameLab);

	if (m_ptrGoods->isNormalGood())
	{
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

		if (m_ptrGoods->getStoneID())
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

			//攻击值
			Label* labPowerVal = LabelSystemFont(IntToString(m_ptrGoods->GetGoodsPower() + m_ptrGoods->getStarAddVal()), 16);
			labPowerVal->setAnchorPoint(Vec2(0.0f, 0.5f));
			labPowerVal->setPosition(Vec2(point.x - 123 + labPower->getContentSize().width, point.y + 86 - iOffsetLine * 20));
			this->addChild(labPowerVal);

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
		labInfo->setDimensions(240, 90);
		labInfo->setHorizontalAlignment(TextHAlignment::LEFT);
		this->addChild(labInfo);
	}

	if (m_ptrGoods->isSuit())
	{
		auto ptrMenu = Menu::create();
		ptrMenu->setPosition(Vec2(0, 0));
		this->addChild(ptrMenu);

		//套装
		Sprite* spSuitN = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
		Sprite* spSuitS = ReadSpriteName("new_ui/img_global/suit_bnt1.png");
		spSuitS->setScale(0.9f);
		MenuItemSprite* btnSuit = MenuItemSprite::create(spSuitN, spSuitS, CC_CALLBACK_1(GoodsLayer::callBackSuit, this));
		spSuitS->setAnchorPoint(Vec2(0.5f, 0.5f));
		spSuitS->setPosition(Vec2(spSuitS->getContentSize().width / 2, spSuitS->getContentSize().height / 2));
		btnSuit->setPosition(point.x + 86, point.y + 140);
		ptrMenu->addChild(btnSuit);
	}
}

void GoodsLayer::callBackSuit(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	ChessApplication::getInstance()->AddFunctionLayer(SuitInfoLayer::create(m_ptrGoods->good_id));
}
