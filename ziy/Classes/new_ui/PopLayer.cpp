//
//  PopLayer.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/3/9.
//
//

#include "PopLayer.h"
#include "Defines.h"
#include "main.h"
#include "BattleScreen.h"
#include "RedeScreen.h"
#include "GuideLayer.h"

PopLayer* PopLayer::create(PopType pType, EventDelegate* delegate, int fruitType /*= 0*/, int iPriority /*= LAYER_POPUP*/)
{
	PopLayer *pRet = new PopLayer();
	if(pRet && pRet->init(pType, delegate, fruitType, iPriority))
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


bool PopLayer::init(PopType pType, EventDelegate* delegate, int fruitType, int iPriority)
{
	if (!Layer::init())
	{
		return false;
	}

	this->pType = pType;
	this->delegate = delegate;
	m_iPriority = iPriority;
	this->mGoldId = 0;
	m_bIsRemove = true;
	m_strTag = "";
	string menuNomalSp = "new_ui/img_global/global_ban3.png";
	string menuSelectSp = "new_ui/img_global/global_ban4.png";

	string bg_path = "new_ui/img_global/pop_bg1.png";
	switch (pType)
	{
	case POP_FWQ_WH:
		bg_path = "new_ui/sign/accounts_sm4.png";
		break;
	case POP_OPEN_CLANFB:
		bg_path = "new_ui/gh/gh_bg9.png";
		menuNomalSp = "new_ui/img_global/global_ban1.png";
		menuSelectSp = "new_ui/img_global/global_ban2.png";
		break;
	case POP_CLAN_ADDGOODS:
		bg_path = "new_ui/gh/gh_bg10.png";
		break;
	default:
		break;
	}

	DSizeF size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	this->addChild(black);

	Sprite* bg = ReadSpriteName(bg_path.c_str());
	bg->setPosition(Vec2(size.width / 2, size.height / 2));
	bg->setCascadeOpacityEnabled(true);
	this->addChild(bg);

	//中华文化
	Sprite* spMark = ReadSpriteName("new_ui/img_global/icon_zhwh.png");
	spMark->setPosition(Vec2(78, 132));
	bg->addChild(spMark);
	spMark->setVisible(false);

	cancel = Menu::create();
	cancel->setPosition(Vec2(bg->getContentSize().width / 2 - 70, 47));
	Sprite* cancelNormalImage = ReadSpriteName(menuNomalSp.c_str());
	Sprite* cancelSelectImage = ReadSpriteName(menuSelectSp.c_str());
	MenuItemSprite* left = MenuItemSprite::create(cancelNormalImage, cancelSelectImage, CC_CALLBACK_1(PopLayer::cancelMenu, this));
	cancel->addChild(left);
	bg->addChild(cancel);

	int iMWidth = cancelNormalImage->getContentSize().width;
	int imHeight = cancelNormalImage->getContentSize().height;

	Sprite* cancelTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_3.png");
	cancelTXTN->setPosition(Vec2(iMWidth/2, imHeight/2));
	cancelNormalImage->addChild(cancelTXTN);
	Sprite* cancelTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_4.png");
	cancelTXTS->setPosition(Vec2(iMWidth / 2, imHeight / 2));
	cancelSelectImage->addChild(cancelTXTS);

	sure = Menu::create();
	sure->setPosition(Vec2(bg->getContentSize().width / 2 + 70, 47));
	Sprite* sureNormalImage = ReadSpriteName(menuNomalSp.c_str());
	Sprite* sureSelectImage = ReadSpriteName(menuSelectSp.c_str());
	MenuItemSprite* right = MenuItemSprite::create(sureNormalImage, sureSelectImage, CC_CALLBACK_1(PopLayer::sureMenu, this));
	sure->addChild(right);
	bg->addChild(sure);

	Sprite* sureTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
	sureTXTN->setPosition(Vec2(iMWidth / 2, imHeight / 2));
	sureNormalImage->addChild(sureTXTN);
	Sprite* sureTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
	sureTXTS->setPosition(Vec2(iMWidth / 2, imHeight / 2));
	sureSelectImage->addChild(sureTXTS);

	switch (pType)
	{
	case POP_NOTICE:
	{
		m_bIsRemove = false;
		spMark->setVisible(true);
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 47));

		m_labTitle = LabelSystemFont("", 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2 + 30, 132));
		bg->addChild(m_labTitle);
	}
		break;
	case POP_UPDATE_FAIL:
	{
		m_bIsRemove = false;
		spMark->setVisible(true);
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 47));

		m_labTitle = LabelSystemFont(LL("updatefailed"), 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2 + 30, 132));
		bg->addChild(m_labTitle);
	}
	    break;
	case POP_EQ_RECAST:
	{
		string strTitle1 = pType == POP_SP_REF ? LL("shoprefresh1") : LL("pubrefresh1");
		string strTitle2 = pType == POP_SP_REF ? LL("shoprefresh2") : LL("pubrefresh2");

		Label* labTitle = LabelSystemFont(LL("recastconfirm"), 18);
		labTitle->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(labTitle);
	}
		break;
	case POP_REQ_AGAIN:
	{
		m_bIsRemove = false;
		spMark->setVisible(true);
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 47));

		m_labTitle = LabelSystemFont(LL("httpreqagain"), 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(m_labTitle);
		history->resetGuide(0);
	}
		break;
	case POP_FWQ_WH:
	{
		m_bIsRemove = false;
		cancel->setVisible(false);
		sure->setVisible(false);

		m_labTitle = LabelSystemFont("", 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2));
		m_labTitle->setColor(Color3B::YELLOW);
		bg->addChild(m_labTitle);
	}
		break;
	case POP_RESIGN:
	case POP_SP_REF:
	case POP_PUB_REF:
	{
		map<int, string> mTitle1;
		map<int, string> mTitle2;

		mTitle1[POP_RESIGN] = LL("resignnotice1");
		mTitle1[POP_SP_REF] = LL("shoprefresh1");
		mTitle1[POP_PUB_REF] = LL("pubrefresh1");
		mTitle2[POP_RESIGN] = LL("resignnotice2");
		mTitle2[POP_SP_REF] = LL("shoprefresh2");
		mTitle2[POP_PUB_REF] = LL("pubrefresh2");

		string strTitle1 = mTitle1[pType];
		string strTitle2 = mTitle2[pType];

		Label* labTitle1 = LabelSystemFont(strTitle1.c_str(), 18);
		labTitle1->setAnchorPoint(Vec2(0.0f, 0.5f));
		bg->addChild(labTitle1);

		Label* labPrice = LabelSystemFont(__String::createWithFormat(LL("refreshprice"), fruitType)->getCString(), 18);
		labPrice->setAnchorPoint(Vec2(0.0f, 0.5f));
		labPrice->setColor(Color3B(0, 255, 255));
		bg->addChild(labPrice);

		Label* labTitle2 = LabelSystemFont(strTitle2.c_str(), 18);
		labTitle2->setAnchorPoint(Vec2(0.0f, 0.5f));
		bg->addChild(labTitle2);

		int iTotalWidth = labTitle1->getContentSize().width + labPrice->getContentSize().width + labTitle2->getContentSize().width;
		int iPosX1 = bg->getContentSize().width / 2 - iTotalWidth / 2;
		int iPosX2 = iPosX1 + labTitle1->getContentSize().width;
		int iPosX3 = iPosX2 + labPrice->getContentSize().width;

		labTitle1->setPosition(Vec2(iPosX1, 132));
		labPrice->setPosition(Vec2(iPosX2, 132));
		labTitle2->setPosition(Vec2(iPosX3, 132));
	}
		break;
	case POP_ZM_WJ:
	case POP_WJ_JJ:
	case POP_BUY_WARCOUNT:
		m_bIsRemove = false;
		cancel->setPosition(Vec2(124, 49));
		sure->setPosition(Vec2(275, 49));
		delegate->SetPopLayerElement(bg);
		break;
	case POP_JB_KYM:
	{
		m_labTitle = LabelSystemFont(LL("keyima"), 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(m_labTitle);
	}
		break;
	case POP_YZ_BOX:
	{
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width/2, 49));
		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_GLC_CHEST:
	{
		if (fruitType == 1)//领取
		{
			sureNormalImage->removeAllChildren();
			sureSelectImage->removeAllChildren();

			Sprite* sureTXTNe = ReadSpriteName("new_ui/img_text/imgtxt4_5.png");
			sureTXTNe->setPosition(Vec2(40, 18));
			sureNormalImage->addChild(sureTXTNe);
			Sprite* sureTXTSe = ReadSpriteName("new_ui/img_text/imgtxt4_6.png");
			sureTXTSe->setPosition(Vec2(40, 18));
			sureSelectImage->addChild(sureTXTSe);
		}
		else
		{
			sure->setVisible(false);
			cancel->setPosition(Vec2(bg->getContentSize().width / 2, 49));

			cancelNormalImage->removeAllChildren();
			cancelSelectImage->removeAllChildren();

			Sprite* cancelTXTNe = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
			cancelTXTNe->setPosition(Vec2(40, 18+5));
			cancelNormalImage->addChild(cancelTXTNe);
			Sprite* cancelTXTSe = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
			cancelTXTSe->setPosition(Vec2(40, 18+5));
			cancelSelectImage->addChild(cancelTXTSe);
		}

		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_GLC_REWARD:
	{
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 49));

		sureNormalImage->removeAllChildren();
		sureSelectImage->removeAllChildren();

		Sprite* SureTXTNe = ReadSpriteName("new_ui/img_text/imgtxt4_1.png");
		SureTXTNe->setPosition(Vec2(40, 18+5));
		sureNormalImage->addChild(SureTXTNe);
		Sprite* SureTXTSe = ReadSpriteName("new_ui/img_text/imgtxt4_2.png");
		SureTXTSe->setPosition(Vec2(40, 18+5));
		sureSelectImage->addChild(SureTXTSe);

		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_CLAN_APPLY_CANCLE:
	case POP_CLAN_APPLY_SURE:
	case POP_CLAN_DELETE:
	case POP_PRAC_GROWN:
	{
		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_DELETE_CLAN:
	{
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 49));

		Label* infoLab = LabelSystemFont(LL("bsghcytc"),16);
		infoLab->setPosition(Vec2(bg->getContentSize().width/2,132));
		bg->addChild(infoLab);
	}
		break;
	case POP_OPEN_CLANFB:
	{
		cancel->setPosition(Vec2(bg->getContentSize().width / 2 - 93, 47));
		sure->setPosition(Vec2(bg->getContentSize().width / 2 + 93, 47));

		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_CLAN_ADDGOODS:
	{
		cancel->setVisible(false);
		sure->setPosition(Vec2(bg->getContentSize().width / 2, 49));

		delegate->SetPopLayerElement(bg);
	}
		break;
	case POP_UNLOAD_ENCH:
	{
		m_labTitle = LabelSystemFont(LL("shifouxiexialingshi"), 18);
		m_labTitle->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(m_labTitle);
	}
		break;
	default:
		break;
	}

	return true;
}

void PopLayer::cancelMenu(Ref* node)
{
	OnlyRemoveThis();
}

void PopLayer::sureMenu(Ref* node)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	sure->setEnabled(false);
	switch (pType) 
	{
	case POP_REQ_AGAIN:
	{
		ITools::getInstance()->connectingAgain(m_strTag);
		ITools::getInstance()->addWaitLayer();
	}
		break;
	case POP_RESIGN:
	case POP_UPDATE_FAIL:
	case POP_EQ_RECAST:
	case POP_SP_REF:
	case POP_PUB_REF:
	case POP_PRAC_GROWN:
	{
		if(this->delegate)
		{
			this->delegate->callBackConfirm();
		}
	}
		break;
	case POP_ZM_WJ:
	case POP_BUY_WARCOUNT:
	case POP_UNLOAD_ENCH:
	{
		if (this->delegate)
		{
			this->delegate->RunFunction(0);
			//history->updateGuide(ZHUJIEMIAN6_GUIDE);
		}
	}
		break;
	case POP_WJ_JJ:
	case POP_GLC_CHEST:
	case POP_GLC_REWARD:
	{
		if (this->delegate)
		{
			this->delegate->RunFunction(1);
			
		}
	}
		break;
	case POP_JB_KYM:
	{
			if (history->game_state == GAME_STATE_REDE)
			{
				RedeScreenPtr scrR = (RedeScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_REDE);
				if (scrR != NULL)
					scrR->getChess()->testAsk = 1;
			}
			else
			{
				BattleScreenPtr scrB = (BattleScreenPtr)Chess::getInstance()->getScreenByID(SCREEN_BATTLE);
				if (scrB != NULL)
					scrB->getChess()->testAsk = 1;
			}
	}
		break;
	case POP_CLAN_APPLY_CANCLE:
	case POP_CLAN_APPLY_SURE:
	case POP_CLAN_DELETE:
	{
		if (this->delegate)
			this->delegate->RunFunction(pType);
	}
		break;
	case POP_DELETE_CLAN:
	{
		if (this->delegate)
			this->delegate->ChildCloseLayer(pType);
	}
		break;
	case POP_OPEN_CLANFB:
	{
		if (this->delegate)
		{
			this->delegate->callBackConfirm();
		}
	}
		break;
	default:
		break;
	}

	this->removeFromParent();
	//history->updateGuide(ZHUJIEMIAN6_GUIDE);
}

PopLayer::PopLayer()
{
}

PopLayer::~PopLayer()
{

}

void PopLayer::onEnter()
{
	Layer::onEnter();
}

void PopLayer::onExit()
{
	Layer::onExit();
}

void PopLayer::OnlyRemoveThis()
{
	switch (pType)
	{
	case POP_ZM_WJ:
	{
		if (this->delegate)
			delegate->ChildCloseLayer(0);
	}
		break;
	default:
		break;
	}

	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}

void PopLayer::setTitle(const char* chTitle)
{
	m_labTitle->setString(chTitle);
}

void PopLayer::setTag(string strTag)
{
	m_strTag = strTag;
}
