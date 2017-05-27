#include "EquipmentUp.h"
#include "Defines.h"
#include "main.h"
#include "GlobalPrompt.h"
#include "PopLayer.h"
#include "GoldExchangeLayer.h"
#include "BuyJadeLayer.h"
#include "NetPromptTool.h"
#include "MergeLayer.h"
#include "GoodsLayer.h"
#include "GuideLayer.h"
#include "Net_HomePage.h"
#include "HelpLayer.h"

#define MAX_EQUIP_NUM 9
#define ENCHANT_PRICE 500

static int RECAST_PRICE[3] = {10, 50, 100};

////装备强化///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqStrengLayer* EqStrengLayer::create()
{
	EqStrengLayer* pRet = new EqStrengLayer();
	if(pRet&&pRet->init())
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

bool EqStrengLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	return true;
}

void EqStrengLayer::updateLayer(HistoryGoods* ptrGoods)
{
	m_ptrGoods = ptrGoods;
	int iCurStar = ptrGoods->iQuality;
	int iMaxStar = ptrGoods->getMaxStar();
	int iAddVal = ptrGoods->getStarAddVal();
	int iAddValNext = ptrGoods->getStarAddVal(iCurStar + 1);

	m_labStone->setString(IntToString(history->stone));
	m_labGold->setString(IntToStringEx(history->money));

	m_spImg->setTexture(ITools::getInstance()->getTextureWithPath(history->GetGoodsImagePath(ptrGoods->good_id)));

	int iQuality = 0;
	if(iCurStar > 1)
	{
		iQuality = trData->strengthen_consume[iCurStar - 1].quality;
	}
	m_armQualy->getAnimation()->play(StringUtils::format("q%d", iQuality));

	m_labName->setString(ptrGoods->GetGoodsName());
	if(iCurStar > 0)
	{
		m_labName->setColor(ITools::getInstance()->getPZColor(trData->strengthen_consume[iCurStar - 1].quality));
	}
	else
	{
		m_labName->setColor(Color3B::WHITE);
	}
	m_labType->setString(StringUtils::format("(%s)", ptrGoods->GetAttributeInfo()));
	m_labType->setPosition(m_labName->getPosition() + Vec2(m_labName->getContentSize().width + 8, 0));

	m_labAttr->setString(ptrGoods->getAttributeText());
	m_labAttrVal->setString(StringUtils::format("%d", ptrGoods->GetGoodsPower() + iAddVal));
	m_labEffVal->setString(StringUtils::format("%s", ptrGoods->getSpecialEffectName()));

	for(int i = 0; i < iCurStar; i++)
	{
		m_spStars[i]->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/img_number/star_big1.png")));
	}
	for(int i = iCurStar; i < iMaxStar; i++)
	{
		m_spStars[i]->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath("new_ui/img_number/star_big2.png")));
	}
	for(int i = 0; i < MaxStar; i++)
	{
		m_spStars[i]->setVisible(i < iMaxStar);
	}

	if(iAddVal)
	{
		m_mNorAttr[0]->setString(ptrGoods->getAttributeText());
		m_mNorAttr[1]->setString(IntToString_Sign(iAddVal));
	}
	else
	{
		m_mNorAttr[0]->setString("");
		m_mNorAttr[1]->setString("");
	}

	if(iAddValNext)
	{
		m_mNorAttr[2]->setString(ptrGoods->getAttributeText());
		m_mNorAttr[3]->setString(IntToString_Sign(iAddValNext));
	}
	else
	{
		m_mNorAttr[2]->setString("");
		m_mNorAttr[3]->setString("");
	}

	m_spArrow->setVisible(true);

	m_mSpeAttr[0]->setColor(Color3B::WHITE);
	m_mSpeAttr[1]->setColor(Color3B::WHITE);
	m_mSpeAttr[2]->setColor(Color3B::WHITE);
	m_mSattrText[0] = LL("qhdsanxjhtssx");
	m_mSattrText[1] = LL("qhdliuxjhtssx");
	m_mSattrText[2] = LL("qhdshixjhtssx");

	for(UInt i = 0; i < ptrGoods->GetStrengEffect().size(); i++)
	{
		int type = ptrGoods->GetStrengEffect()[i];
		for(int j = 0; j < trData->strengthen_property_num; j++)
		{
			if(type == trData->strengthen_property[j].id && i <= 2)
			{
				m_mSattrText[i] = trData->strengthen_property[j].name;
				m_mSpeAttr[i]->setColor(ITools::getInstance()->getPZColor(trData->strengthen_property[j].quality));
			}
		}
	}

	for(int i = 0; i < (int)m_mSpeAttr.size(); i++)
	{
		m_mSpeAttr[i]->setString(m_mSattrText[i]);
		m_mSpeAttr[i]->setVisible(i < ptrGoods->getSpeAttrNum());
	}

	m_spStoneUse->setVisible(true);
	m_spGoldUse->setVisible(true);
	m_labStoneUse->setString(IntToString(trData->strengthen_consume[iCurStar].gem_num));
	m_labGoldUse->setString(IntToString(trData->strengthen_consume[iCurStar].money));

	m_labSucceed->setVisible(true);
	m_labSucVal->setString(StringUtils::format("%d%%", trData->strengthen_consume[iCurStar].success_rate));

	if(iCurStar == iMaxStar)
	{
		m_spArrow->setVisible(false);
		m_spStoneUse->setVisible(false);
		m_spGoldUse->setVisible(false);
		m_labStoneUse->setString("");
		m_labGoldUse->setString("");
		m_labSucceed->setVisible(false);
		m_labSucVal->setString("");
	}
}

Sprite* EqStrengLayer::getInfoBG()
{
	return m_spInfo;
}

Sprite* EqStrengLayer::getCurStar()
{
	return m_spStars[m_ptrGoods->iQuality];
}

void EqStrengLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//背景
	Sprite* m_spBG = ReadSpriteName("new_ui/img_global/global_bg.png");
	m_spBG->setPosition(point);
	m_spBG->setOpacity(0);
	this->addChild(m_spBG);

	//金币强化石背景
	Sprite* spGoldBG = ReadSpriteName("new_ui/img_global/smithing_bg1.png");
	spGoldBG->setPosition(Vec2(22 + spGoldBG->getContentSize().width / 2, 330));
	m_spBG->addChild(spGoldBG);

	//强化石ICON
	Sprite* spStone = ReadSpriteName("new_ui/img_global/icon_qhs.png");
	spStone->setPosition(Vec2(10 + spStone->getContentSize().width / 2, spGoldBG->getContentSize().height / 2));
	spGoldBG->addChild(spStone);

	//强化石
	m_labStone = LabelSystemFont("0", 16);
	m_labStone->setAnchorPoint(Vec2(0, 0.5));
	m_labStone->setPosition(Vec2(12 + spStone->getContentSize().width, spGoldBG->getContentSize().height / 2));
	m_labStone->setColor(Color3B(255, 0, 255));
	spGoldBG->addChild(m_labStone);

	//金币ICON
	Sprite* spGold = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGold->setPosition(Vec2(88 + spGold->getContentSize().width / 2, spGoldBG->getContentSize().height / 2));
	spGoldBG->addChild(spGold);

	//金币
	m_labGold = LabelSystemFont("0", 16);
	m_labGold->setAnchorPoint(Vec2(0, 0.5));
	m_labGold->setPosition(Vec2(90 + spGold->getContentSize().width, spGoldBG->getContentSize().height / 2));
	m_labGold->setColor(Color3B::YELLOW);
	spGoldBG->addChild(m_labGold);

	//信息背景
	m_spInfo = ReadSpriteName("new_ui/img_global/smithing_bg2.png");
	m_spInfo->setPosition(Vec2(439, 182));
	m_spBG->addChild(m_spInfo);

	//装备背景框
	Sprite* spIconBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	spIconBG->setPosition(Vec2(76, 205));
	m_spInfo->addChild(spIconBG);

	//装备图标
	m_spImg = ITools::getInstance()->getSpriteWithPath("img/Items/1.png");
	m_spImg->setPosition(Vec2(76, 205));
	m_spInfo->addChild(m_spImg);

	//品质框
	m_armQualy = Armature::create("special_eff");
	m_armQualy->setPosition(Vec2(75, 204));
	m_armQualy->getAnimation()->play("q0");
	m_spInfo->addChild(m_armQualy);

	//装备名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setAnchorPoint(Vec2(0, 0.5f));
	m_labName->setPosition(Vec2(410, 282));
	m_labName->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labName);

	//装备类型
	m_labType = LabelSystemFont("", 16);
	m_labType->setAnchorPoint(Vec2(0, 0.5f));
	m_labType->setPosition(Vec2(410, 282) + Vec2(m_labName->getContentSize().width + 8, 0));
	m_labType->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labType);

	//属性
	m_labAttr = LabelSystemFont(LL("gongji"), 16);
	m_labAttr->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttr->setPosition(Vec2(410, 261));
	m_labAttr->setColor(Color3B::YELLOW);
	m_spBG->addChild(m_labAttr);

	//属性值
	m_labAttrVal = LabelSystemFont("", 16);
	m_labAttrVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttrVal->setPosition(Vec2(410, 261) + Vec2(m_labAttr->getContentSize().width + 8, 0));
	m_labAttrVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labAttrVal);

	//特性
	Label* labEffect = LabelSystemFont(LL("texing"), 16);
	labEffect->setAnchorPoint(Vec2(0, 0.5f));
	labEffect->setPosition(Vec2(410, 240));
	labEffect->setColor(Color3B::YELLOW);
	m_spBG->addChild(labEffect);

	//特性值
	m_labEffVal = LabelSystemFont("", 16);
	m_labEffVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labEffVal->setPosition(Vec2(410, 240) + Vec2(labEffect->getContentSize().width + 8, 0));
	m_labEffVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labEffVal);

	//星级显示
	for(int i = 0; i < MaxStar; i++)
	{
		m_spStars[i] = ReadSpriteName("new_ui/img_number/star_big2.png");
		m_spStars[i]->setPosition(Vec2(60 + (i * 19), 155));
		m_spInfo->addChild(m_spStars[i]);
	}

	//普通属性
	m_mNorAttr[0] = LabelSystemFont(LL("gongji"), 16);
	m_mNorAttr[0]->setAnchorPoint(Vec2(0, 0.5f));
	m_mNorAttr[0]->setPosition(Vec2(50, 125));
	m_mNorAttr[0]->setColor(Color3B::WHITE);
	m_spInfo->addChild(m_mNorAttr[0]);

	m_mNorAttr[1] = LabelSystemFont("+10", 16);
	m_mNorAttr[1]->setAnchorPoint(Vec2(0, 0.5f));
	m_mNorAttr[1]->setPosition(Vec2(86, 125));
	m_mNorAttr[1]->setColor(Color3B::WHITE);
	m_spInfo->addChild(m_mNorAttr[1]);

	m_mNorAttr[2] = LabelSystemFont(LL("gongji"), 16);
	m_mNorAttr[2]->setAnchorPoint(Vec2(0, 0.5f));
	m_mNorAttr[2]->setPosition(Vec2(182, 125));
	m_mNorAttr[2]->setColor(Color3B::WHITE);
	m_spInfo->addChild(m_mNorAttr[2]);

	m_mNorAttr[3] = LabelSystemFont("+13", 16);
	m_mNorAttr[3]->setAnchorPoint(Vec2(0, 0.5f));
	m_mNorAttr[3]->setPosition(Vec2(220, 125));
	m_mNorAttr[3]->setColor(Color3B::GREEN);
	m_spInfo->addChild(m_mNorAttr[3]);

	//箭头
	m_spArrow = ReadSpriteName("new_ui/img_global/global_sj.png");
	m_spArrow->setAnchorPoint(Vec2(0, 0.5f));
	m_spArrow->setPosition(Vec2(132, 125));
	m_spInfo->addChild(m_spArrow);

	//特殊属性
	m_mSpeAttr[0] = LabelSystemFont(LL("qhdsanxjhtssx"), 16);
	m_mSpeAttr[0]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[0]->setPosition(Vec2(50, 95));
	m_spInfo->addChild(m_mSpeAttr[0]);

	m_mSpeAttr[1] = LabelSystemFont(LL("qhdliuxjhtssx"), 16);
	m_mSpeAttr[1]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[1]->setPosition(Vec2(50, 75));
	m_spInfo->addChild(m_mSpeAttr[1]);

	m_mSpeAttr[2] = LabelSystemFont(LL("qhdshixjhtssx"), 16);
	m_mSpeAttr[2]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[2]->setPosition(Vec2(50, 55));
	m_spInfo->addChild(m_mSpeAttr[2]);

	//强化石ICON
	m_spStoneUse = ReadSpriteName("new_ui/img_global/icon_qhs.png");
	m_spStoneUse->setPosition(Vec2(388, 80));
	m_spBG->addChild(m_spStoneUse);

	//消耗强化石
	m_labStoneUse = LabelSystemFont("10", 16);
	m_labStoneUse->setAnchorPoint(Vec2(0, 0.5f));
	m_labStoneUse->setPosition(Vec2(400, 80));
	m_labStoneUse->setColor(Color3B(255, 0, 255));
	m_spBG->addChild(m_labStoneUse);

	//金币ICON
	m_spGoldUse = ReadSpriteName("new_ui/img_global/icon_gb.png");
	m_spGoldUse->setPosition(Vec2(466, 80));
	m_spBG->addChild(m_spGoldUse);

	//消耗金币
	m_labGoldUse = LabelSystemFont("10000", 16);
	m_labGoldUse->setAnchorPoint(Vec2(0, 0.5f));
	m_labGoldUse->setPosition(Vec2(478, 80));
	m_labGoldUse->setColor(Color3B::YELLOW);
	m_spBG->addChild(m_labGoldUse);

	//成功率
	m_labSucceed = LabelSystemFont(LL("cgl :"), 16);
	m_labSucceed->setPosition(Vec2(327-70, 34));
	m_labSucceed->setColor(Color3B(153, 51, 0));
	m_spBG->addChild(m_labSucceed);

	//成功率值
	m_labSucVal = LabelSystemFont("+5%", 16);
	m_labSucVal->setPosition(Vec2(377-70, 34));
	m_labSucVal->setColor(Color3B(153, 51, 0));
	m_spBG->addChild(m_labSucVal);

	//m_spInfo->setPosition(m_spInfo->getPosition() + Vec2(20,7));
	//for (auto child: m_spBG->getChildren())
	//{
	//	child->setPosition(child->getPosition() + Vec2(20, 7));
	//}
}

int EqStrengLayer::getSelectIndex()
{
	return 0;
}

////装备重铸///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqRecastLayer* EqRecastLayer::create()
{
	EqRecastLayer* pRet = new EqRecastLayer();
	if(pRet&&pRet->init())
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

bool EqRecastLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iSelectIndex = 0;

	initControl();
	updateCheckBox();

	return true;
}

void EqRecastLayer::updateLayer(HistoryGoods* ptrGoods)
{
	m_ptrGoods = ptrGoods;
	m_iSelectIndex = 0;
	int iCurStar = ptrGoods->iQuality;
	int iMaxStar = ptrGoods->getMaxStar();
	int iAddVal = ptrGoods->getStarAddVal();
	int iAddValNext = ptrGoods->getStarAddVal(iCurStar + 1);

	m_labJade->setString(IntToString(history->m_iJade));

	m_spImg->setTexture(ITools::getInstance()->getTextureWithPath(history->GetGoodsImagePath(ptrGoods->good_id)));

	string strPath = StringUtils::format("new_ui/img_number/star_b_%d.png", iCurStar);
	m_spStar->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));

	int iQuality = 0;
	if(iCurStar > 1)
	{
		iQuality = trData->strengthen_consume[iCurStar - 1].quality;
	}
	m_armQualy->getAnimation()->play(StringUtils::format("q%d", iQuality));

	m_labName->setString(ptrGoods->GetGoodsName());
	if(iCurStar > 0)
	{
		m_labName->setColor(ITools::getInstance()->getPZColor(trData->strengthen_consume[iCurStar - 1].quality));
	}
	else
	{
		m_labName->setColor(Color3B::WHITE);
	}
	m_labType->setString(StringUtils::format("(%s)", ptrGoods->GetAttributeInfo()));
	m_labType->setPosition(m_labName->getPosition() + Vec2(m_labName->getContentSize().width + 8, 0));

	m_labAttr->setString(ptrGoods->getAttributeText());
	m_labAttrVal->setString(StringUtils::format("%d", ptrGoods->GetGoodsPower() + iAddVal));
	m_labEffVal->setString(StringUtils::format("%s", ptrGoods->getSpecialEffectName()));

	m_mSpeAttr[0]->setColor(Color3B::WHITE);
	m_mSpeAttr[1]->setColor(Color3B::WHITE);
	m_mSpeAttr[2]->setColor(Color3B::WHITE);
	m_mSattrText[0] = LL("qhdsanxjhtssx");
	m_mSattrText[1] = LL("qhdliuxjhtssx");
	m_mSattrText[2] = LL("qhdshixjhtssx");

	for(UInt i = 0; i < ptrGoods->GetStrengEffect().size(); i++)
	{
		int type = ptrGoods->GetStrengEffect()[i];
		for(int j = 0; j < trData->strengthen_property_num; j++)
		{
			if(type == trData->strengthen_property[j].id && i <= 2)
			{
				m_mSattrText[i] = trData->strengthen_property[j].name;
				m_mSpeAttr[i]->setColor(ITools::getInstance()->getPZColor(trData->strengthen_property[j].quality));
			}
		}
	}

	for(int i = 0; i < (int)m_mSpeAttr.size(); i++)
	{
		m_mSpeAttr[i]->setString(m_mSattrText[i]);
		m_mSpeAttr[i]->setVisible(i < ptrGoods->getSpeAttrNum());
		m_mCheckBox[i]->setVisible(i < (int)ptrGoods->GetStrengEffect().size());
	}

	updateCheckBox();
}

void EqRecastLayer::updateLayer()
{
	m_labJade->setString(IntToString(history->m_iJade));

	int iType = m_ptrGoods->GetStrengEffect()[m_iSelectIndex];
	for(int i = 0; i < trData->strengthen_property_num; i++)
	{
		if(iType == trData->strengthen_property[i].id)
		{
			m_mSattrText[m_iSelectIndex] = trData->strengthen_property[i].name;
			m_mSpeAttr[m_iSelectIndex]->setString(m_mSattrText[m_iSelectIndex]);
			m_mSpeAttr[m_iSelectIndex]->setColor(ITools::getInstance()->getPZColor(trData->strengthen_property[i].quality));
		}
	}

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

Sprite* EqRecastLayer::getInfoBG()
{
	return m_spInfo;
}

Sprite* EqRecastLayer::getCurStar()
{
	return m_mCheckBox[m_iSelectIndex];
}

void EqRecastLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);

	//背景
	Sprite* m_spBG = ReadSpriteName("new_ui/img_global/global_bg.png");
	m_spBG->setPosition(point);
	m_spBG->setOpacity(0);
	this->addChild(m_spBG);

	//勾玉背景
	Sprite* spJadeBG = ReadSpriteName("new_ui/img_global/smithing_bg1.png");
	spJadeBG->setPosition(Vec2(22 + spJadeBG->getContentSize().width / 2, 330));
	m_spBG->addChild(spJadeBG);

	//勾玉ICON
	Sprite* spJade = ReadSpriteName("new_ui/img_global/icon_gy.png");
	spJade->setPosition(Vec2(10 + spJade->getContentSize().width / 2, spJadeBG->getContentSize().height / 2));
	spJadeBG->addChild(spJade);

	//勾玉
	m_labJade = LabelSystemFont("0", 16);
	m_labJade->setAnchorPoint(Vec2(0, 0.5));
	m_labJade->setPosition(Vec2(12 + spJade->getContentSize().width, spJadeBG->getContentSize().height / 2));
	m_labJade->setColor(Color3B::GREEN);
	spJadeBG->addChild(m_labJade);

	//信息背景
	m_spInfo = ReadSpriteName("new_ui/img_global/smithing_bg3.png");
	m_spInfo->setPosition(Vec2(439, 182));
	m_spBG->addChild(m_spInfo);

	//装备背景框
	Sprite* spIconBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	spIconBG->setPosition(Vec2(76, 205));
	m_spInfo->addChild(spIconBG);

	//装备图标
	m_spImg = ITools::getInstance()->getSpriteWithPath("img/Items/1.png");
	m_spImg->setPosition(Vec2(76, 205));
	m_spInfo->addChild(m_spImg);

	//星级图标
	m_spStar = ReadSpriteName("new_ui/img_number/star_b_1.png");
	m_spStar->setPosition(Vec2(76, 185));
	m_spInfo->addChild(m_spStar);

	//品质框
	m_armQualy = Armature::create("special_eff");
	m_armQualy->setPosition(Vec2(75, 204));
	m_armQualy->getAnimation()->play("q0");
	m_spInfo->addChild(m_armQualy);

	//装备名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setAnchorPoint(Vec2(0, 0.5f));
	m_labName->setPosition(Vec2(410, 282));
	m_labName->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labName);

	//装备类型
	m_labType = LabelSystemFont("", 16);
	m_labType->setAnchorPoint(Vec2(0, 0.5f));
	m_labType->setPosition(Vec2(410, 282) + Vec2(m_labName->getContentSize().width + 8, 0));
	m_labType->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labType);

	//属性
	m_labAttr = LabelSystemFont(LL("gongji"), 16);
	m_labAttr->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttr->setPosition(Vec2(410, 261));
	m_labAttr->setColor(Color3B::YELLOW);
	m_spBG->addChild(m_labAttr);

	//属性值
	m_labAttrVal = LabelSystemFont("", 16);
	m_labAttrVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttrVal->setPosition(Vec2(410, 261) + Vec2(m_labAttr->getContentSize().width + 8, 0));
	m_labAttrVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labAttrVal);

	//特性
	Label* labEffect = LabelSystemFont(LL("texing"), 16);
	labEffect->setAnchorPoint(Vec2(0, 0.5f));
	labEffect->setPosition(Vec2(410, 240));
	labEffect->setColor(Color3B::YELLOW);
	m_spBG->addChild(labEffect);

	//特性值
	m_labEffVal = LabelSystemFont("", 16);
	m_labEffVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labEffVal->setPosition(Vec2(410, 240) + Vec2(labEffect->getContentSize().width + 8, 0));
	m_labEffVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labEffVal);

	//特性选择框
	m_mCheckBox[0] = CheckBoxSprite::create(NULL, this, menu_selector(EqRecastLayer::callBackCheck));
	m_mCheckBox[0]->setPosition(Vec2(60, 141));
	m_mCheckBox[0]->setTag(0);
	m_spInfo->addChild(m_mCheckBox[0], 1);

	m_mCheckBox[1] = CheckBoxSprite::create(NULL, this, menu_selector(EqRecastLayer::callBackCheck));
	m_mCheckBox[1]->setPosition(Vec2(60, 103));
	m_mCheckBox[1]->setTag(1);
	m_spInfo->addChild(m_mCheckBox[1], 1);

	m_mCheckBox[2] = CheckBoxSprite::create(NULL, this, menu_selector(EqRecastLayer::callBackCheck));
	m_mCheckBox[2]->setPosition(Vec2(60, 65));
	m_mCheckBox[2]->setTag(2);
	m_spInfo->addChild(m_mCheckBox[2], 1);

	//特殊属性
	m_mSpeAttr[0] = LabelSystemFont(LL("qhdsanxjhtssx"), 16);
	m_mSpeAttr[0]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[0]->setPosition(Vec2(80, 141));
	m_spInfo->addChild(m_mSpeAttr[0]);

	m_mSpeAttr[1] = LabelSystemFont(LL("qhdliuxjhtssx"), 16);
	m_mSpeAttr[1]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[1]->setPosition(Vec2(80, 103));
	m_spInfo->addChild(m_mSpeAttr[1]);

	m_mSpeAttr[2] = LabelSystemFont(LL("qhdshixjhtssx"), 16);
	m_mSpeAttr[2]->setAnchorPoint(Vec2(0, 0.5f));
	m_mSpeAttr[2]->setPosition(Vec2(80, 65));
	m_spInfo->addChild(m_mSpeAttr[2]);

	//勾玉ICON
	m_spJadeUse = ReadSpriteName("new_ui/img_global/icon_gy.png");
	m_spJadeUse->setPosition(Vec2(427, 80));
	m_spBG->addChild(m_spJadeUse);

	//消耗勾玉
	m_labJadeUse = LabelSystemFont("10", 16);
	m_labJadeUse->setAnchorPoint(Vec2(0, 0.5f));
	m_labJadeUse->setPosition(Vec2(439, 80));
	m_labJadeUse->setColor(Color3B::GREEN);
	m_spBG->addChild(m_labJadeUse);
}

void EqRecastLayer::callBackCheck(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	CheckBoxSprite* ptrCheckBox = (CheckBoxSprite*)sender;
	m_iSelectIndex = ptrCheckBox->getTag();
	updateCheckBox();
}

void EqRecastLayer::updateCheckBox()
{
	for(UInt i = 0; i < m_mCheckBox.size(); i++)
	{
		m_mCheckBox[i]->setChecked(i == m_iSelectIndex);
	}
	m_labJadeUse->setString(IntToString(RECAST_PRICE[m_iSelectIndex]));
}

int EqRecastLayer::getSelectIndex()
{
	return m_iSelectIndex;
}

////装备附魔///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EqEnchantLayer* EqEnchantLayer::create()
{
	EqEnchantLayer* pRet = new EqEnchantLayer();
	if(pRet&&pRet->init())
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

bool EqEnchantLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	m_iSelectIndex = 0;
	m_iStoneId = 0;

	initControl();

	return true;
}

void EqEnchantLayer::updateLayer(HistoryGoods* ptrGoods)
{
	m_ptrGoods = ptrGoods;
	m_iSelectIndex = 0;
	m_iStoneId = ptrGoods->getStoneID();
	int iCurStar = ptrGoods->iQuality;
	int iMaxStar = ptrGoods->getMaxStar();
	int iAddVal = ptrGoods->getStarAddVal();
	int iAddValNext = ptrGoods->getStarAddVal(iCurStar + 1);

	m_spImg->setTexture(ITools::getInstance()->getTextureWithPath(history->GetGoodsImagePath(ptrGoods->good_id)));

	if (iCurStar > 0)
	{
		string strPath = StringUtils::format("new_ui/img_number/star_b_%d.png", iCurStar);
		m_spStar->setTexture(Director::getInstance()->getTextureCache()->addImage(ReadSpritePath(strPath.c_str())));
		m_spStar->setVisible(true);
	}
	else
	{
		m_spStar->setVisible(false);
	}

	int iQuality = 0;
	if(iCurStar > 1)
	{
		iQuality = trData->strengthen_consume[iCurStar - 1].quality;
	}
	m_armQualy->getAnimation()->play(StringUtils::format("q%d", iQuality));

	m_labName->setString(ptrGoods->GetGoodsName());
	if(iCurStar > 0)
	{
		m_labName->setColor(ITools::getInstance()->getPZColor(trData->strengthen_consume[iCurStar - 1].quality));
	}
	else
	{
		m_labName->setColor(Color3B::WHITE);
	}
	m_labType->setString(StringUtils::format("(%s)", ptrGoods->GetAttributeInfo()));
	m_labType->setPosition(m_labName->getPosition() + Vec2(m_labName->getContentSize().width + 8, 0));

	m_labAttr->setString(ptrGoods->getAttributeText());
	m_labAttrVal->setString(ptrGoods->isAssist() ? "" : StringUtils::format("%d", ptrGoods->GetGoodsPower() + iAddVal));
	m_labEffVal->setString(StringUtils::format("%s", ptrGoods->getSpecialEffectName()));

	updateLayer();
}

void EqEnchantLayer::updateLayer()
{
	m_labStone->setString(IntToString(history->stone));
	m_labGold->setString(IntToStringEx(history->money));

	bool bIsStone = m_iStoneId != 0;
	if (bIsStone)
	{
		m_spStone->setTexture(ITools::getInstance()->getTextureWithPath(history->GetGoodsImagePath(m_iStoneId)));
		int iStoneNum = history->m_mConsuNum[m_iStoneId];
		m_labStoneNum->setColor(iStoneNum ? Color3B::GREEN : Color3B::RED);
		m_labStoneNum->setString(StringUtils::format("%d", iStoneNum));
	}
	m_labStoneNum->setVisible(m_iSelectIndex != 0);
	m_spStoneBG->setVisible(bIsStone);
	m_spSelect->setVisible(!bIsStone);
	m_btnReplace->setVisible(bIsStone);

	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

Sprite* EqEnchantLayer::getInfoBG()
{
	return m_spInfo;
}

Sprite* EqEnchantLayer::getCurStar()
{
	return nullptr;
}

void EqEnchantLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//背景
	m_spBG = ReadSpriteName("new_ui/img_global/global_bg.png");
	m_spBG->setPosition(point);
	m_spBG->setOpacity(0);
	this->addChild(m_spBG);

	//金币强化石背景
	Sprite* spGoldBG = ReadSpriteName("new_ui/img_global/smithing_bg1.png");
	spGoldBG->setPosition(Vec2(22 + spGoldBG->getContentSize().width / 2, 330));
	m_spBG->addChild(spGoldBG);

	//强化石ICON
	Sprite* spStone = ReadSpriteName("new_ui/img_global/icon_qhs.png");
	spStone->setPosition(Vec2(10 + spStone->getContentSize().width / 2, spGoldBG->getContentSize().height / 2));
	spGoldBG->addChild(spStone);

	//强化石
	m_labStone = LabelSystemFont("0", 16);
	m_labStone->setAnchorPoint(Vec2(0, 0.5));
	m_labStone->setPosition(Vec2(12 + spStone->getContentSize().width, spGoldBG->getContentSize().height / 2));
	m_labStone->setColor(Color3B(255, 0, 255));
	spGoldBG->addChild(m_labStone);

	//金币ICON
	Sprite* spGold = ReadSpriteName("new_ui/img_global/icon_gb.png");
	spGold->setPosition(Vec2(88 + spGold->getContentSize().width / 2, spGoldBG->getContentSize().height / 2));
	spGoldBG->addChild(spGold);

	//金币
	m_labGold = LabelSystemFont("0", 16);
	m_labGold->setAnchorPoint(Vec2(0, 0.5));
	m_labGold->setPosition(Vec2(90 + spGold->getContentSize().width, spGoldBG->getContentSize().height / 2));
	m_labGold->setColor(Color3B::YELLOW);
	spGoldBG->addChild(m_labGold);

	//信息背景
	m_spInfo = ReadSpriteName("new_ui/img_global/smithing_bg3.png");
	m_spInfo->setPosition(Vec2(439, 182));
	m_spBG->addChild(m_spInfo);

	//装备背景框
	Sprite* spIconBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	spIconBG->setPosition(Vec2(76, 205));
	m_spInfo->addChild(spIconBG);

	//装备图标
	m_spImg = ITools::getInstance()->getSpriteWithPath("img/Items/1.png");
	m_spImg->setPosition(Vec2(76, 205));
	m_spInfo->addChild(m_spImg);

	//星级图标
	m_spStar = ReadSpriteName("new_ui/img_number/star_b_1.png");
	m_spStar->setPosition(Vec2(76, 185));
	m_spInfo->addChild(m_spStar);

	//品质框
	m_armQualy = Armature::create("special_eff");
	m_armQualy->setPosition(Vec2(75, 204));
	m_armQualy->getAnimation()->play("q0");
	m_spInfo->addChild(m_armQualy);

	//装备名字
	m_labName = LabelSystemFont("", 16);
	m_labName->setAnchorPoint(Vec2(0, 0.5f));
	m_labName->setPosition(Vec2(410, 282));
	m_labName->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labName);

	//装备类型
	m_labType = LabelSystemFont("", 16);
	m_labType->setAnchorPoint(Vec2(0, 0.5f));
	m_labType->setPosition(Vec2(410, 282) + Vec2(m_labName->getContentSize().width + 8, 0));
	m_labType->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labType);

	//属性
	m_labAttr = LabelSystemFont(LL("gongji"), 16);
	m_labAttr->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttr->setPosition(Vec2(410, 261));
	m_labAttr->setColor(Color3B::YELLOW);
	m_spBG->addChild(m_labAttr);

	//属性值
	m_labAttrVal = LabelSystemFont("", 16);
	m_labAttrVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labAttrVal->setPosition(Vec2(410, 261) + Vec2(m_labAttr->getContentSize().width + 8, 0));
	m_labAttrVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labAttrVal);

	//特性
	Label* labEffect = LabelSystemFont(LL("texing"), 16);
	labEffect->setAnchorPoint(Vec2(0, 0.5f));
	labEffect->setPosition(Vec2(410, 240));
	labEffect->setColor(Color3B::YELLOW);
	m_spBG->addChild(labEffect);

	//特性值
	m_labEffVal = LabelSystemFont("", 16);
	m_labEffVal->setAnchorPoint(Vec2(0, 0.5f));
	m_labEffVal->setPosition(Vec2(410, 240) + Vec2(labEffect->getContentSize().width + 8, 0));
	m_labEffVal->setColor(Color3B::WHITE);
	m_spBG->addChild(m_labEffVal);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	m_spInfo->addChild(ptrMenu);

	//灵石按钮
	Sprite* spStoneN = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	m_btnStone = MenuItemSprite::create(spStoneN, nullptr, CC_CALLBACK_1(EqEnchantLayer::callBackStone, this));
	m_btnStone->setPosition(Vec2(147, 122));
	ptrMenu->addChild(m_btnStone);

	//宝石背景
	m_spStoneBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	m_spStoneBG->setPosition(Vec2(147, 122));
	m_spInfo->addChild(m_spStoneBG);

	//宝石图标
	m_spStone = ITools::getInstance()->getSpriteWithPath("img/Items/1.png");
	m_spStone->setPosition(Vec2(33, 33));
	m_spStoneBG->addChild(m_spStone);

	//宝石数目
	m_labStoneNum = LabelSystemFont("0", 16);
	m_labStoneNum->setAnchorPoint(Vec2(1.0f, 0.0f));
	m_labStoneNum->setPosition(Vec2(m_spStoneBG->getContentSize().width - 2, 0));
	m_labStoneNum->enableOutline(Color4B::BLACK, 1);
	m_spStoneBG->addChild(m_labStoneNum);

	//选择
	m_spSelect = ReadSpriteName("new_ui/hero/hero_add.png");
	m_spSelect->setPosition(Vec2(147, 122));
	m_spInfo->addChild(m_spSelect);

	//切换
	Sprite* spRepN = ReadSpriteName("new_ui/img_global/icon_fm1.png");
	Sprite* spRepS = ReadSpriteName("new_ui/img_global/icon_fm1.png");
	spRepS->setScale(fScale);
	m_btnReplace = MenuItemSprite::create(spRepN, spRepS, CC_CALLBACK_1(EqEnchantLayer::callBackSelect, this));
	spRepS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRepS->setPosition(Vec2(spRepS->getContentSize().width / 2, spRepS->getContentSize().height / 2));
	m_btnReplace->setPosition(Vec2(227, 122));
	ptrMenu->addChild(m_btnReplace);

	//选择提示
	m_labNotice = LabelSystemFont(LL("enchantnotice"), 16);
	m_labNotice->setPosition(Vec2(147, 72));
	m_labNotice->enableOutline(Color4B(102, 50, 0, 255), 1);
	m_spInfo->addChild(m_labNotice);

	//强化石ICON
	auto spStoneUsed = ReadSpriteName("new_ui/img_global/icon_qhs.png");
	spStoneUsed->setPosition(Vec2(131, 23));
	m_spInfo->addChild(spStoneUsed);

	//消耗强化石
	auto labStoneUsed = LabelSystemFont(StringUtils::format("%d", ENCHANT_PRICE), 16);
	labStoneUsed->setAnchorPoint(Vec2(0.0f, 0.5f));
	labStoneUsed->setPosition(Vec2(143, 21));
	labStoneUsed->setColor(Color3B::MAGENTA);
	m_spInfo->addChild(labStoneUsed);
}

void EqEnchantLayer::callBackStone(Ref *sender)
{
	if (m_iStoneId)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ChessApplication::getInstance()->AddFunctionLayer(GoodsLayer::create(m_iStoneId));
	}
	else
	{
		callBackSelect(sender);
	}
}

void EqEnchantLayer::callBackSelect(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if(m_ptrGoods->getFmGoodsMap().size() > 0)
	{
		ChessApplication::getInstance()->AddFunctionLayer(MergeSelectLayer::create(m_ptrGoods->good_id, nullptr, this));
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("noselectstone"), false);
	}
}

int EqEnchantLayer::getSelectIndex()
{
	return m_iSelectIndex;
}

void EqEnchantLayer::updateShow(int index)
{
	m_iStoneId = index;
	m_iSelectIndex = index;
	updateLayer();
}

////装备主界面///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EquipmentUp* EquipmentUp::create(HistoryGoodsPtr curGoods)
{
	EquipmentUp* pRet = new EquipmentUp();
	if (pRet&&pRet->init(curGoods))
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

bool EquipmentUp::init(HistoryGoodsPtr curGoods)
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_mTreasure = history->GetAllTreasure(MODEL_STRENG);
	if(m_mTreasure.size())
	{
		m_iModel = MODEL_STRENG;
	}
	else
	{
		m_mTreasure = history->GetAllTreasure(MODEL_RECAST);
		m_iModel = MODEL_RECAST;
	}

	m_iCurPage = 1;
	m_iMaxPage = (m_mTreasure.size() - 1) / MAX_EQUIP_NUM + 1;
	m_ptrGoods = NULL;
	m_iTouchEnabled = 0;

	initControl();
	updateCurModel(m_iModel,curGoods);
	if(!curGoods)
		this->updateShow(0);

	HongDian::getInstence()->autoUpdateEquep();
    return true;
}

void EquipmentUp::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;
	auto offsetX = -23;
	//强化成功动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/special_eff.ExportJson", 0, 1));

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景框
	Sprite* spBG = ReadSpriteName("new_ui/img_global/global_bg3.png");
	spBG->setPosition(point);
	this->addChild(spBG);

	//标题文字
	//Sprite* title = ReadSpriteName("new_ui/img_text/bt3_1.png");
	//title->setPosition(Vec2(spBG->getContentSize().width / 2, 362));
	//spBG->addChild(title);

	//左装备栏
	m_layBox = BoxListLayer::create(getModels(), this);
	m_layBox->setPosition(point + Vec2(-154+10, -12));
	this->addChild(m_layBox, 88);

	//强化层
	m_mEqLayer[0] = EqStrengLayer::create();
	this->addChild(m_mEqLayer[0]);

	//重铸层
	m_mEqLayer[1] = EqRecastLayer::create();
	this->addChild(m_mEqLayer[1]);

	//附魔层
	m_mEqLayer[2] = EqEnchantLayer::create();
	this->addChild(m_mEqLayer[2]);

	for (int i=0;i<=2;i++)
	{
		m_mEqLayer[i]->setPosition(m_mEqLayer[i]->getPosition()+Vec2(43,8));
	}

	//页码
	m_labPage = LabelSystemFont(StringUtils::format("%d/%d", m_iCurPage, m_iMaxPage), 16);
	m_labPage->setPosition(point + Vec2(-153, -160));
	m_labPage->setColor(Color3B(102,0,0));
	this->addChild(m_labPage);

	//按钮菜单
	Menu* btnMenu = Menu::create();
	btnMenu->setPosition(Vec2(0, 0));
	this->addChild(btnMenu);

	//上一页
	Sprite* spLastN = ReadSpriteName("new_ui/img_global/switch1.png");
	Sprite* spLastS = ReadSpriteName("new_ui/img_global/switch1.png");
	spLastS->setScale(fScale);
	MenuItemSprite* btnLast = MenuItemSprite::create(spLastN, spLastS, CC_CALLBACK_1(EquipmentUp::callBackLast, this));
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
	MenuItemSprite* btnNext = MenuItemSprite::create(spNextN, spNextS, CC_CALLBACK_1(EquipmentUp::callBackNext, this));
	spNextS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spNextS->setPosition(Vec2(spNextS->getContentSize().width / 2, spNextS->getContentSize().height / 2));
	btnNext->setPosition(point + Vec2(-98, -160));
	btnMenu->addChild(btnNext);

	//强化选项
	Sprite* spForgeN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spFoTextN = ReadSpriteName("new_ui/img_text/imgtxt1_1.png");
	spFoTextN->setPosition(Vec2(spForgeN->getContentSize().width / 2, spForgeN->getContentSize().height / 2));
	spForgeN->addChild(spFoTextN);
	Sprite* spForgeS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spFoTextS = ReadSpriteName("new_ui/img_text/imgtxt1_2.png");
	spFoTextS->setPosition(Vec2(spForgeS->getContentSize().width / 2, spForgeS->getContentSize().height / 2));
	spForgeS->addChild(spFoTextS);
	Sprite* spForgeD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spFoTextD = ReadSpriteName("new_ui/img_text/imgtxt1_2.png");
	spFoTextD->setPosition(Vec2(spForgeD->getContentSize().width / 2, spForgeD->getContentSize().height / 2));
	spForgeD->addChild(spFoTextD);
	m_mMenuType[0] = MenuItemSprite::create(spForgeN, spForgeS, spForgeD, CC_CALLBACK_1(EquipmentUp::callBackModel, this));
	m_mMenuType[0]->setTag(MODEL_STRENG);
	m_mMenuType[0]->setPosition(point + Vec2(301, 60+50));
	btnMenu->addChild(m_mMenuType[0]);
	m_mMenuType[0]->setEnabled(false);

	HongDian::getInstence()->addHongDianNode(m_mMenuType[0],2100);

	//重铸选项
	Sprite* spWashN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spWaTextN = ReadSpriteName("new_ui/img_text/imgtxt1_3.png");
	spWaTextN->setPosition(Vec2(spWashN->getContentSize().width / 2, spWashN->getContentSize().height / 2));
	spWashN->addChild(spWaTextN);
	Sprite* spWashS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spWaTextS = ReadSpriteName("new_ui/img_text/imgtxt1_4.png");
	spWaTextS->setPosition(Vec2(spWashS->getContentSize().width / 2, spWashS->getContentSize().height / 2));
	spWashS->addChild(spWaTextS);
	Sprite* spWashD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spWaTextD = ReadSpriteName("new_ui/img_text/imgtxt1_4.png");
	spWaTextD->setPosition(Vec2(spWashD->getContentSize().width / 2, spWashD->getContentSize().height / 2));
	spWashD->addChild(spWaTextD);
	m_mMenuType[1] = MenuItemSprite::create(spWashN, spWashS, spWashD, CC_CALLBACK_1(EquipmentUp::callBackModel, this));
	m_mMenuType[1]->setTag(MODEL_RECAST);
	m_mMenuType[1]->setPosition(point + Vec2(301, -10+40));
	btnMenu->addChild(m_mMenuType[1]);

	HongDian::getInstence()->addHongDianNode(m_mMenuType[1], 2101);

	//附魔选项
	Sprite* spMEnchN = ReadSpriteName("new_ui/img_global/global_ban7.png");
	Sprite* spMEnTextN = ReadSpriteName("new_ui/img_text/imgtxt1_11.png");
	spMEnTextN->setPosition(Vec2(spMEnchN->getContentSize().width / 2, spMEnchN->getContentSize().height / 2));
	spMEnchN->addChild(spMEnTextN);
	Sprite* spMEnchS = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spMEnTextS = ReadSpriteName("new_ui/img_text/imgtxt1_12.png");
	spMEnTextS->setPosition(Vec2(spMEnchS->getContentSize().width / 2, spMEnchS->getContentSize().height / 2));
	spMEnchS->addChild(spMEnTextS);
	Sprite* spMEnchD = ReadSpriteName("new_ui/img_global/global_ban8.png");
	Sprite* spMEnTextD = ReadSpriteName("new_ui/img_text/imgtxt1_12.png");
	spMEnTextD->setPosition(Vec2(spMEnchD->getContentSize().width / 2, spMEnchD->getContentSize().height / 2));
	spMEnchD->addChild(spMEnTextD);
	m_mMenuType[2] = MenuItemSprite::create(spMEnchN, spMEnchS, spMEnchD, CC_CALLBACK_1(EquipmentUp::callBackModel, this));
	m_mMenuType[2]->setTag(MODEL_ENCHANT);
	m_mMenuType[2]->setPosition(point + Vec2(301, -80+30));
	btnMenu->addChild(m_mMenuType[2]);

	HongDian::getInstence()->addHongDianNode(m_mMenuType[2], 2102);

	//强化按钮
	Sprite* spStrengN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spStrengTextN = ReadSpriteName("new_ui/img_text/imgtxt2_1.png");
	spStrengTextN->setPosition(Vec2(spStrengN->getContentSize().width / 2, spStrengN->getContentSize().height / 2));
	spStrengN->addChild(spStrengTextN);
	Sprite* spStrengS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spStrengTextS = ReadSpriteName("new_ui/img_text/imgtxt2_2.png");
	spStrengTextS->setPosition(Vec2(spStrengS->getContentSize().width / 2, spStrengS->getContentSize().height / 2));
	spStrengS->addChild(spStrengTextS);
	m_mEquip[0] = MenuItemSprite::create(spStrengN, spStrengS, CC_CALLBACK_1(EquipmentUp::callBackStreng, this));
	m_mEquip[0]->setPosition(point + Vec2(220, -160));
	btnMenu->addChild(m_mEquip[0]);

	//重铸按钮
	Sprite* spRecastN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spRecastTextN = ReadSpriteName("new_ui/img_text/imgtxt2_3.png");
	spRecastTextN->setPosition(Vec2(spRecastN->getContentSize().width / 2, spRecastN->getContentSize().height / 2));
	spRecastN->addChild(spRecastTextN);
	Sprite* spRecastS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spRecastTextS = ReadSpriteName("new_ui/img_text/imgtxt2_4.png");
	spRecastTextS->setPosition(Vec2(spRecastS->getContentSize().width / 2, spRecastS->getContentSize().height / 2));
	spRecastS->addChild(spRecastTextS);
	m_mEquip[1] = MenuItemSprite::create(spRecastN, spRecastS, CC_CALLBACK_1(EquipmentUp::callBackRecast, this));
	m_mEquip[1]->setPosition(point + Vec2(220, -160));
	btnMenu->addChild(m_mEquip[1]);

	//附魔按钮
	Sprite* spEnchN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spEnTextN = ReadSpriteName("new_ui/img_text/imgtxt2_7.png");
	spEnTextN->setPosition(Vec2(spEnchN->getContentSize().width / 2, spEnchN->getContentSize().height / 2));
	spEnchN->addChild(spEnTextN);
	Sprite* spEnchS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spEnTextS = ReadSpriteName("new_ui/img_text/imgtxt2_8.png");
	spEnTextS->setPosition(Vec2(spEnchS->getContentSize().width / 2, spEnchS->getContentSize().height / 2));
	spEnchS->addChild(spEnTextS);
	m_mEquip[2] = MenuItemSprite::create(spEnchN, spEnchS, CC_CALLBACK_1(EquipmentUp::callBackEnchant, this));
	m_mEquip[2]->setPosition(point + Vec2(220, -160));
	btnMenu->addChild(m_mEquip[2]);

	//十连强化按钮
	Sprite* spStrengN10 = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spStrengTextN10 = ReadSpriteName("new_ui/img_text/imgtxt2_1_1.png");
	spStrengTextN10->setPosition(Vec2(spStrengN10->getContentSize().width / 2, spStrengN10->getContentSize().height / 2));
	spStrengN10->addChild(spStrengTextN10);
	Sprite* spStrengS10 = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spStrengTextS10 = ReadSpriteName("new_ui/img_text/imgtxt2_2_1.png");
	spStrengTextS10->setPosition(Vec2(spStrengS10->getContentSize().width / 2, spStrengS10->getContentSize().height / 2));
	spStrengS10->addChild(spStrengTextS10);
	m_mEquip[3] = MenuItemSprite::create(spStrengN10, spStrengS10, CC_CALLBACK_1(EquipmentUp::callBackStreng10, this));
	m_mEquip[3]->setPosition(point + Vec2(220-130, -160));
	btnMenu->addChild(m_mEquip[3]);

	//卸下附魔石按钮
	Sprite* spUnLoadEnchN = ReadSpriteName("new_ui/img_global/global_ban1.png");
	Sprite* spUnLoadEnTextN = ReadSpriteName("new_ui/img_text/imgtxt3_7.png");
	spUnLoadEnTextN->setPosition(Vec2(spUnLoadEnchN->getContentSize().width / 2, spUnLoadEnchN->getContentSize().height / 2));
	spUnLoadEnchN->addChild(spUnLoadEnTextN);
	Sprite* spUnLoadEnchS = ReadSpriteName("new_ui/img_global/global_ban2.png");
	Sprite* spUnLoadEnTextS = ReadSpriteName("new_ui/img_text/imgtxt3_8.png");
	spUnLoadEnTextS->setPosition(Vec2(spUnLoadEnchS->getContentSize().width / 2, spUnLoadEnchS->getContentSize().height / 2));
	spUnLoadEnchS->addChild(spUnLoadEnTextS);
	m_UnloadEnchItem = MenuItemSprite::create(spUnLoadEnchN, spUnLoadEnchS, CC_CALLBACK_1(EquipmentUp::callBackUnLoadEnchant, this));
	m_UnloadEnchItem->setPosition(point + Vec2(50, -160));
	btnMenu->addChild(m_UnloadEnchItem);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close1.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press1.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(EquipmentUp::callBackClose, this));
	btnClose->setPosition(point + Vec2(295, 173));
	btnMenu->addChild(btnClose);

	//Touch屏蔽层
	MenuItem* btnNull = MenuItem::create(CC_CALLBACK_1(EquipmentUp::callBackNull, this));
	btnNull->setContentSize(size);
	btnNull->setPosition(point);
	m_menuNull = Menu::create(btnNull, NULL);
	m_menuNull->setPosition(Vec2(0, 0));
	m_menuNull->setVisible(false);
	this->addChild(m_menuNull, 99);
}
void EquipmentUp::callBackStreng10(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_times = 10;
	if (m_ptrGoods->iQuality == m_ptrGoods->getMaxStar())
	{
		ITools::getInstance()->createGlobalPrompt(LL("gwqyjddzgdj"), false);
		return;
	}

	if (history->stone < trData->strengthen_consume[m_ptrGoods->iQuality].gem_num)
	{
		ITools::getInstance()->createGlobalPrompt(LL("qianghuashibuzu"), false);
		return;
	}

	if (history->money < trData->strengthen_consume[m_ptrGoods->iQuality].money)
	{
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
		return;
	}

	onHttpRequest();
}
void EquipmentUp::callBackStreng(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	m_times = 1;
	if(m_ptrGoods->iQuality == m_ptrGoods->getMaxStar())
    {
		ITools::getInstance()->createGlobalPrompt(LL("gwqyjddzgdj"), false);
        return;
    }
	
	if(history->stone < trData->strengthen_consume[m_ptrGoods->iQuality].gem_num)
    {
		ITools::getInstance()->createGlobalPrompt(LL("qianghuashibuzu"), false);
        return;
    }
    
	if(history->money < trData->strengthen_consume[m_ptrGoods->iQuality].money)
    {
		ChessApplication::getInstance()->AddFunctionLayer(GoldExchangeLayer::create(this));
        return;
    }

	onHttpRequest();
	//history->updateGuide(STRENG_GUIDE);
}

void EquipmentUp::callBackRecast(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	int iSelectIndex = m_mEqLayer[m_iModel]->getSelectIndex();
	if(history->m_iJade < RECAST_PRICE[iSelectIndex])
	{
		ChessApplication::getInstance()->AddFunctionLayer(BuyJadeLayer::create(this));
		return;
	}

	int iType = m_ptrGoods->GetStrengEffect()[iSelectIndex];
	int iQuality = 0;
	for(int i = 0; i < trData->strengthen_property_num; i++)
	{
		if(iType == trData->strengthen_property[i].id)
		{
			iQuality = trData->strengthen_property[i].quality;
			break;
		}
	}

	if (iQuality > 3)
	{
		ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_EQ_RECAST, this));
		return;
	}

	onHttpRequest();
}

void EquipmentUp::callBackEnchant(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if(history->stone < ENCHANT_PRICE)
	{
		ITools::getInstance()->createGlobalPrompt(LL("qianghuashibuzu"), false);
		return;
	}

	int iStoneId = m_mEqLayer[m_iModel]->getSelectIndex();
	if (!iStoneId) //未选择灵石
	{
		ITools::getInstance()->createGlobalPrompt(LL("pleaseselectstone"), false);
		return;
	}
	else
	{
		HistoryGoods hisGoods(iStoneId);
		if (!hisGoods.getStoreNum()) //库存不足
		{
			ITools::getInstance()->createGlobalPrompt(LL("fmstonenotenough"), false);
			return;
		}
		else if (m_ptrGoods->getStoneID() == iStoneId) //已附魔该属性
		{
			ITools::getInstance()->createGlobalPrompt(LL("hadthisfmstone"), false);
			return;
		}
	}
	onHttpRequest();
}

void EquipmentUp::callBackUnLoadEnchant(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	if (history->stone < ENCHANT_PRICE)
	{
		ITools::getInstance()->createGlobalPrompt(LL("qianghuashibuzu"), false);
		return;
	}

	int iStoneId = m_ptrGoods->getStoneID();
	if (!iStoneId) //未选择灵石
	{
		ITools::getInstance()->createGlobalPrompt(LL("meiyoulingshi"), false);
		return;
	}

	ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_UNLOAD_ENCH, this));
}

void EquipmentUp::callBackClose(Ref *sender)
{
    playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
	HongDian::getInstence()->autoUpdate();
}

bool EquipmentUp::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    return true;
}

void EquipmentUp::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

void EquipmentUp::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    
}

EquipmentUp::EquipmentUp()
{

}

EquipmentUp::~EquipmentUp()
{
    
}

void EquipmentUp::updateShow(int index)
{
    //特殊参数 用于回调的判断
    if(index == 9999999)
        index = m_iCurIndex;
    
    m_iCurIndex = index;

	m_ptrGoods = m_mTreasure[index];
	m_mEqLayer[m_iModel]->updateLayer(m_ptrGoods);
		
	//刷新标题栏金币勾玉数据
	if(history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
	if (m_iModel == MODEL_ENCHANT&&m_ptrGoods&&m_ptrGoods->getStoneID())
	{
		m_UnloadEnchItem->setVisible(true);
	}
	else
	{
		m_UnloadEnchItem->setVisible(false);
	}
}

void EquipmentUp::callBackLast(Ref *sender)
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
	updateEquipBox();
}

void EquipmentUp::callBackNext(Ref *sender)
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
	updateEquipBox();
}

map<int,boxModel*> EquipmentUp::getModels(int iSetBox)
{
    map<int,boxModel*> boxData;
	for(int i = (m_iCurPage - 1) * MAX_EQUIP_NUM, j = 0; i < m_iCurPage * MAX_EQUIP_NUM; i++, j++)
    {
		boxModel* bm = new boxModel();
		bm->setBGPath("new_ui/img_global/item_box_bg2.png");
		bm->setSelectImagePath("new_ui/img_global/selected_box1_1.png");
		bm->setUIType(1);
		bm->setIsSelected(false);
		bm->setIsOwn("");
		bm->setHeroName("");
		bm->setStoneID(0);

        if(m_mTreasure.count(i))
        {
			HistoryGoods* ptrGoods = m_mTreasure[i];
			bm->setStarLV(ptrGoods->iQuality);
            int pz = 0;
			if(ptrGoods->iQuality != 0)
            {
				pz = trData->strengthen_consume[ptrGoods->iQuality - 1].quality;
            }
            bm->setPZLV(pz);
			bm->setTBPath(history->GetGoodsImagePath(ptrGoods->good_id));
            bm->setIndex(i);

			if(ptrGoods->equipment_actorId > -1)
			{
				bm->setHeroName(trData->actors[ptrGoods->equipment_actorId].getActorName());
				//HongDian::getInstence()->addHongDianNode();
			}

			if (m_iModel == MODEL_ENCHANT)
			{
				bm->setStoneID(ptrGoods->getStoneID());
			}
        }
        else
        {
            bm->setStarLV(0);
            bm->setPZLV(0);
            bm->setTBPath("");
            bm->setIndex(-1);
        }
		boxData[j] = bm;
	}
	int iSetBoxSelected = 0;
	if (iSetBox > -1&& iSetBox < MAX_EQUIP_NUM)
	{
		iSetBoxSelected = iSetBox;
	}
    boxData[iSetBoxSelected]->setIsSelected(true);
    return boxData;
}

void EquipmentUp::callBackModel(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	map<int, string> mstrNotice;
	mstrNotice[MODEL_STRENG] = LL("nostrengequip");
	mstrNotice[MODEL_RECAST] = LL("norecastequip");
	mstrNotice[MODEL_ENCHANT] = LL("noenchantequip");

	int iCurModel = ((MenuItemSprite*)sender)->getTag();

	if(!history->GetAllTreasure(iCurModel).size())
	{
		ITools::getInstance()->createGlobalPrompt(mstrNotice[iCurModel], false);
		return;
	}

	updateCurModel(iCurModel);
}

void EquipmentUp::onHttpRequest()
{
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	string strToken = history->m_pLoginInfo->getGameToken();
	switch(m_iModel)
	{
	case MODEL_STRENG:
	{
		strURL = "strengthen";
		string data = StringUtils::format("id=%d&seid=%d&times=%d&gameToken=%s", m_ptrGoods->getNetID(), m_ptrGoods->getSeSeid(), m_times, strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"id\":%d,\"seid\":%d,\"times\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), m_ptrGoods->getSeSeid(), m_times, strToken.c_str(), validCode.c_str());
		strTag = "MODEL_STRENG";
	}
		break;
	case MODEL_RECAST:
	{
		int iSelectIndex = m_mEqLayer[m_iModel]->getSelectIndex();
		strURL = "reSetStrengthen";
		string data = StringUtils::format("id=%d&seid=%d&gameToken=%s&starLV=%d&oldPropert=%d", m_ptrGoods->getNetID(), m_ptrGoods->getSeSeid(), strToken.c_str(), iSelectIndex + 1, m_ptrGoods->GetStrengEffect()[iSelectIndex]);
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"id\":%d,\"seid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\",\"starLV\":%d,\"oldPropert\":%d}", m_ptrGoods->getNetID(), m_ptrGoods->getSeSeid(), strToken.c_str(), validCode.c_str(), iSelectIndex + 1, m_ptrGoods->GetStrengEffect()[iSelectIndex]);
		strTag = "MODEL_RECAST";
	}
		break;
	case MODEL_ENCHANT:
	{
		int iStoneId = m_mEqLayer[m_iModel]->getSelectIndex();
		strURL = "suitGood";
		string data = StringUtils::format("id=%d&suitID=%d&gameToken=%s", m_ptrGoods->getNetID(), history->m_mConsuNetId[iStoneId], strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"id\":%d,\"suitID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), history->m_mConsuNetId[iStoneId], strToken.c_str(), validCode.c_str());
		strTag = "MODEL_ENCHANT";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(EquipmentUp::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void EquipmentUp::UnLoadEnchantonHttpRequest()
{
	CocosMD5 md5;

	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "removeGood";
	string data = StringUtils::format("id=%d&gameToken=%s", m_ptrGoods->getNetID(),strToken.c_str());
	string validCode = md5.ToMD5(data);
	string strData = StringUtils::format("{\"id\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", m_ptrGoods->getNetID(), strToken.c_str(), validCode.c_str());
	string strTag = "MODEL_UNLOADENCHANT";

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(EquipmentUp::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void EquipmentUp::onHttpRequestCompleted(Node *sender, void *resultdata)
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
		if(strTag == "MODEL_STRENG")
		{
			onStrengSucceed(value["result"]);
		}
		else if(strTag == "MODEL_RECAST")
		{
			onRecastSucceed(value["result"]);
		}
		else if (strTag == "MODEL_ENCHANT")
		{
			onEnchantSucceed(value["result"]);
		}
		else if (strTag == "MODEL_UNLOADENCHANT")
		{
			onUnLoadEnchantSucceed(value["result"]);
		}
	}
	HongDian::getInstence()->autoUpdateEquep();
}

void EquipmentUp::onStrengSucceed(Json::Value resVal)
{
	if(resVal["gold"].isInt())
	{
		history->money = resVal["gold"].asInt();
	}
	if(resVal["improvedStone"].isInt())
	{
		history->stone = resVal["improvedStone"].asInt();
	}
	if (m_times==10)
	{
		auto help=HelpLayer::create("new_ui/img_global/help_bg.png", resVal);


		Director::getInstance()->getRunningScene()->addChild(help);
	}
	//强化动画
	setEqTouchEnabled(false);
	Armature* leftArm = Armature::create("special_eff");
	leftArm->setPosition(Vec2(76, 205));
	leftArm->getAnimation()->play("qh1");
	leftArm->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if(type == COMPLETE)
		{
			armature->removeFromParent();
			setEqTouchEnabled(true);
		}
	});
	m_mEqLayer[m_iModel]->getInfoBG()->addChild(leftArm);

	ChessPlayNetSound(203);
	if(resVal["isSuccess"].isInt() && resVal["isSuccess"].asInt())
	{

		GoodsElem goodsElem;
		goodsElem.loadJson(resVal["good"]);

		//提示框
		if (m_times != 10)
		{
			__String* str = __String::createWithFormat(LL("qhcg"), m_ptrGoods->GetGoodsName(), m_ptrGoods->iQuality + 1);
			ITools::getInstance()->createGlobalPrompt(str->getCString(), true);
		}

		//强化动画
		setEqTouchEnabled(false);
		Armature* starArm = Armature::create("special_eff");
		starArm->setPosition(m_mEqLayer[m_iModel]->getCurStar()->getPosition());
		starArm->getAnimation()->play("qh2");
		starArm->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
		{
			if(type == COMPLETE)
			{
				armature->removeFromParent();
				int good_index = m_layBox->selectIndex;
				int __time = 0;
				for (auto _a:resVal["infoAry"])
				{
					if (_a["isSuccess"].isInt())
					{
						if (_a["isSuccess"].asInt()==1)
						{
							__time++;
						}
					}

				}

				if (m_times == 10)
				{
					m_layBox->data[good_index]->setStarLV(m_layBox->data[good_index]->getStarLV() + __time);
				}
				else
				{
					m_layBox->data[good_index]->setStarLV(m_layBox->data[good_index]->getStarLV() + 1);
				}

				m_layBox->data[good_index]->setPZLV(trData->strengthen_consume[m_ptrGoods->iQuality - 1].quality);
				m_layBox->boxs[m_layBox->selectIndex]->updateBox(m_layBox->data[m_layBox->selectIndex]);
				this->updateShow(m_iCurIndex);
				setEqTouchEnabled(true);
			}
		});
		m_mEqLayer[m_iModel]->getInfoBG()->addChild(starArm);

		//重置属性
		m_ptrGoods->setGood(&goodsElem);
		if(trData->strengthen_consume[m_ptrGoods->iQuality - 1].special_goods)
		{
			ChessPlayNetSound(204);
		}
		if(m_ptrGoods->equipment_actorId > -1)
		{
			history->actors[m_ptrGoods->equipment_actorId].UpdataGoodEffect();
		}
		if(m_ptrGoods->getMaxStar() == 10)
			NetPromptTool::getInstance()->AddNetPrompt(PROMPT_QH, m_ptrGoods->good_id, m_ptrGoods->iQuality);
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("strengfailed"), false);
		this->updateShow(m_iCurIndex);
	}
}

void EquipmentUp::onRecastSucceed(Json::Value resVal)
{
	if(resVal["jade"].isInt())
	{
		history->m_iJade = resVal["jade"].asInt();
	}

	GoodsElem goodsElem;
	goodsElem.loadJson(resVal["good"]);
	//重置属性
	m_ptrGoods->setGood(&goodsElem);
	if(m_ptrGoods->equipment_actorId > -1)
	{
		history->actors[m_ptrGoods->equipment_actorId].UpdataGoodEffect();
	}

	//重铸动画
	ChessPlayNetSound(203);
	setEqTouchEnabled(false);
	Armature* armRecast = Armature::create("special_eff");
	armRecast->setPosition(Vec2(76, 205));
	armRecast->getAnimation()->play("qh1");
	m_mEqLayer[m_iModel]->getInfoBG()->addChild(armRecast);
	armRecast->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
	{
		if(type == COMPLETE)
		{
			armature->removeFromParent();

			//提示框
			string strNotice = StringUtils::format("%s %s", m_ptrGoods->GetGoodsName(), LL("recastsucceed"));
			ITools::getInstance()->createGlobalPrompt(strNotice, true);

			//重铸特效
			Armature* armEffect = Armature::create("special_eff");
			armEffect->setPosition(m_mEqLayer[m_iModel]->getCurStar()->getPosition());
			armEffect->getAnimation()->play("cztx1");
			m_mEqLayer[m_iModel]->getInfoBG()->addChild(armEffect);
			armEffect->getAnimation()->setMovementEventCallFunc([=](Armature* armature, MovementEventType type, const std::string& id)
			{
				if(type == COMPLETE)
				{
					armature->removeFromParent();
					m_mEqLayer[m_iModel]->updateLayer();
					setEqTouchEnabled(true);
				}
			});
		}
	});
}

void EquipmentUp::onEnchantSucceed(Json::Value resVal)
{
	if(resVal["improvedStone"].isInt())
	{
		history->stone = resVal["improvedStone"].asInt();
	}

	GoodsElem goodsElem;
	goodsElem.loadJson(resVal["good"]);
	//重置属性
	m_ptrGoods->setGood(&goodsElem);
	if(m_ptrGoods->equipment_actorId > -1)
	{
		history->actors[m_ptrGoods->equipment_actorId].UpdataGoodEffect();
	}

	//减去消耗
	int iStoneId = m_mEqLayer[m_iModel]->getSelectIndex();
	history->packGoodsReduce(iStoneId, history->m_mConsuNetId[iStoneId], 1);
	history->onHttpGoodsAdd(resVal);
	ChessPlayNetSound(203);

	//提示框
	string strNotice = StringUtils::format("%s %s", m_ptrGoods->GetGoodsName(), LL("enchantsucceed"));
	ITools::getInstance()->createGlobalPrompt(strNotice, true);
	m_mEqLayer[m_iModel]->updateLayer();

	int iSelIndx = m_layBox->selectIndex;
	m_layBox->data[iSelIndx]->setStoneID(m_ptrGoods->getStoneID());
	m_layBox->boxs[iSelIndx]->updateBox(m_layBox->data[iSelIndx]);

	if (m_iModel == MODEL_ENCHANT&&m_ptrGoods&&m_ptrGoods->getStoneID())
	{
		m_UnloadEnchItem->setVisible(true);
	}
	else
	{
		m_UnloadEnchItem->setVisible(false);
	}
}

void EquipmentUp::onUnLoadEnchantSucceed(Json::Value resVal)
{
	if (resVal["improvedStone"].isInt())
	{
		history->stone = resVal["improvedStone"].asInt();
	}

	GoodsElem goodsElem;
	goodsElem.loadJson(resVal["good"]);
	//重置属性
	m_ptrGoods->setGood(&goodsElem);
	if (m_ptrGoods->equipment_actorId > -1)
	{
		history->actors[m_ptrGoods->equipment_actorId].UpdataGoodEffect();
	}

	history->onHttpGoodsAdd(resVal);
	ChessPlayNetSound(203);

	//提示框
	ITools::getInstance()->createGlobalPrompt(LL("cgxxls"), true);

	if (m_iCurIndex > -1)
	{
		updateShow(m_iCurIndex);
	}

	int iSelIndx = m_layBox->selectIndex;
	m_layBox->data[iSelIndx]->setStoneID(m_ptrGoods->getStoneID());
	m_layBox->boxs[iSelIndx]->updateBox(m_layBox->data[iSelIndx]);
}

void EquipmentUp::setEqTouchEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		if(--m_iTouchEnabled == 0)
		{
			m_menuNull->setVisible(!bEnabled);
		}
	}
	else
	{
		if(m_iTouchEnabled++ == 0)
		{
			m_menuNull->setVisible(!bEnabled);
		}
	}
}

void EquipmentUp::callBackNull(Ref *sender)
{
	//屏蔽点击事件，不做任何处理
}

void EquipmentUp::updateEquipBox(int iSetBox)
{
	int iUpdataType = 1;
	if (iSetBox > -1 && iSetBox < MAX_EQUIP_NUM)
	{
		m_layBox->selectIndex = iSetBox;
		iUpdataType = 0;
	}
	m_labPage->setString(StringUtils::format("%d/%d", m_iCurPage, m_iMaxPage));
	m_layBox->updateAll(getModels(iSetBox), iUpdataType);
}

int EquipmentUp::SetCurGoodsPage(HistoryGoodsPtr curGoods)
{
	for (UInt i = 0; i < m_mTreasure.size(); i++)
	{
		if (m_mTreasure[i]->good_id == curGoods->good_id&&m_mTreasure[i]->getNetID() == curGoods->getNetID())
		{
			m_iCurPage = i / MAX_EQUIP_NUM + 1;
			return i;
		}
	}
	return -1;
}

void EquipmentUp::updateCurModel(int iModel, HistoryGoodsPtr curGoods)
{
	m_iModel = iModel;
	m_mTreasure = history->GetAllTreasure(m_iModel);
	m_iCurIndex = 0;
	m_iCurPage = 1;
	m_iMaxPage = (m_mTreasure.size() - 1) / MAX_EQUIP_NUM + 1;


	if (!curGoods)
	{
		updateEquipBox();
	}
	else
	{
		int iCurGoodsId = -1, iSetBoxSelected = -1;

		iCurGoodsId = SetCurGoodsPage(curGoods);
		iSetBoxSelected = iCurGoodsId - (m_iCurPage - 1)*MAX_EQUIP_NUM;

		updateEquipBox(iSetBoxSelected);

		if (iCurGoodsId > -1)
		{
			updateShow(iCurGoodsId);
		}
	}

	for(int i = 0; i < MODEL_MAX; i++)
	{
		bool bVisible = i == iModel;
		m_mEqLayer[i]->setVisible(bVisible);
		if (i==0)
		{
			m_mEquip[3]->setVisible(bVisible);
		}
		m_mEquip[i]->setVisible(bVisible);
		m_mMenuType[i]->setEnabled(!bVisible);
	}
}

void EquipmentUp::callBackConfirm()
{
	onHttpRequest();
}

void EquipmentUp::RunFunction(int iNomal)
{
	UnLoadEnchantonHttpRequest();
}

void EquipmentUp::updateMoneyShow()
{
	m_mEqLayer[m_iModel]->updateLayer();
}