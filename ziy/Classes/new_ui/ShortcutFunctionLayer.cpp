#include "ShortcutFunctionLayer.h"
#include "Defines.h"

ShortcutFunctionLayer* ShortcutFunctionLayer::create(EventDelegate* ptrDelegate, int iSType, int iDate, int iGoodsNum)
{
	ShortcutFunctionLayer* pRet = new ShortcutFunctionLayer();
	if (pRet&&pRet->init(ptrDelegate, iSType,iDate,iGoodsNum))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return pRet;
	}
}

bool ShortcutFunctionLayer::init(EventDelegate* ptrDelegate, int iSType, int iDate, int iGoodsNum)
{
	if (!Layer::init())
	{
		return false;
	}
	mDelegate = ptrDelegate;
	mSType = iSType;
	mDateNum = iDate;
	mGoodsNum = iGoodsNum;

	DSize size = ChessViewTool::getInstance()->getFullSize();

	LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,100));
	black_layer->setPosition(Vec2(0,0));
	this->addChild(black_layer);
	black_layer->setVisible(false);

	Sprite* bg = ReadSpriteName("new_ui/img_global/pop_bg1.png");
	bg->setPosition(Vec2(size.width/2,size.height/2));
	this->addChild(bg);

	mMenuClose = Menu::create();
	mMenuClose->setPosition(Vec2(344, 174+40));
	bg->addChild(mMenuClose);

	Sprite* closeNormalImage = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* closeSelectImage = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* close_menu = MenuItemSprite::create(closeNormalImage, closeSelectImage, CC_CALLBACK_1(ShortcutFunctionLayer::ClickCloseMenu, this));
	mMenuClose->addChild(close_menu);

	mMenuLeft = Menu::create();
	mMenuLeft->setPosition(Vec2(bg->getContentSize().width/4+20, 47));
	bg->addChild(mMenuLeft);

	Sprite* leftNormalImage = ReadSpriteName("new_ui/img_global/global_ban3_left.png");
	Sprite* leftSelectImage = ReadSpriteName("new_ui/img_global/global_ban4_left.png");
	MenuItemSprite* left_menu = MenuItemSprite::create(leftNormalImage, leftSelectImage, CC_CALLBACK_1(ShortcutFunctionLayer::ClickLeftMenu, this));
	mMenuLeft->addChild(left_menu);

	int iButtonWidth = leftNormalImage->getContentSize().width;
	int iButtonHeight = leftNormalImage->getContentSize().height;

	mMenuRight = Menu::create();
	mMenuRight->setPosition(Vec2(bg->getContentSize().width * (3.0/4.0)-20, 47));
	bg->addChild(mMenuRight);

	Sprite* rightNormalImage = ReadSpriteName("new_ui/img_global/global_ban3_right.png");
	Sprite* rightSelectImage = ReadSpriteName("new_ui/img_global/global_ban4_right.png");
	MenuItemSprite* right_menu = MenuItemSprite::create(rightNormalImage, rightSelectImage, CC_CALLBACK_1(ShortcutFunctionLayer::ClickRightMenu, this));
	mMenuRight->addChild(right_menu);

	switch (mSType)
	{
	case SHORTCUT_TYPE_JSD:
	case SHORTCUT_TYPE_GYSD:
	{
		Label* leftTtfN = LabelSystemFont(LL("saodang"), 18);
		leftTtfN->setPosition(Vec2(iButtonWidth / 2, iButtonHeight / 2));
		leftNormalImage->addChild(leftTtfN);

		Label* leftTtfS = LabelSystemFont(LL("saodang"), 18);
		leftTtfS->setPosition(Vec2(iButtonWidth / 2, iButtonHeight / 2));
		leftTtfS->setColor(Color3B(255, 102, 0));
		leftSelectImage->addChild(leftTtfS);

		Label* rightTtfN = LabelSystemFont(__String::createWithFormat(LL("saodangdci"),mDateNum)->getCString(), 18);
		rightTtfN->setPosition(Vec2(iButtonWidth / 2, iButtonHeight / 2));
		rightNormalImage->addChild(rightTtfN);

		Label* rightTtfS = LabelSystemFont(__String::createWithFormat(LL("saodangdci"), mDateNum)->getCString(), 18);
		rightTtfS->setPosition(Vec2(iButtonWidth / 2, iButtonHeight / 2));
		rightTtfS->setColor(Color3B(255, 102, 0));
		rightSelectImage->addChild(rightTtfS);

		if (mSType == SHORTCUT_TYPE_JSD)
		{
			Label* sdInfo = LabelSystemFont(LL("sysdjkykssdgq"),18);
			sdInfo->setPosition(Vec2(bg->getContentSize().width/2,144));
			bg->addChild(sdInfo);

			Sprite* sdjImg = ReadSpriteName("new_ui/img_global/icon_sdj.png");
			sdjImg->setPosition(Vec2(171,109));
			bg->addChild(sdjImg);

			Label* sdjNum = LabelSystemFont(IntToString(mGoodsNum),18);
			sdjNum->setAnchorPoint(Vec2(0, 0.5));
			sdjNum->setColor(Color3B::YELLOW);
			sdjNum->setPosition(Vec2(171 + sdjImg->getContentSize().width, 109));
			bg->addChild(sdjNum);
		}
		else
		{
			Label* sdInfo = LabelSystemFont(LL("sdjbzsygysdzc"), 18);
			sdInfo->setPosition(Vec2(bg->getContentSize().width / 2, 144));
			bg->addChild(sdInfo);

			char* sData = ChessReadFile("json/refresh_expend.json");
			if (sData == NULL)
			{
				CCAssert(true, "未找到文件,或文件加载错误");
				return false;
			}
			Json::Reader reader;
			Json::Value line;
			if (!reader.parse(sData, line, false))return false;
			int size = line.size();

			int sumNum = history->m_pPlayerInfo->getSweepJadeSum();
			int cast = 0;
			for (int i=0;i<7;i++)
			{
				if (sumNum<line[i]["id"].asInt())
				{
					cast = i ;
					break;
				}
				
			}
			string num = LL("ygysdyc");
			if (history->m_iCurWarId > 400 && history->m_iCurWarId < 420)
			{
				num = (char *)(StringUtils::format(LL("ygysdyc"), line[cast]["expend_war_jy"].asInt()).c_str());
			}
			else
			{
				num = (char *)(StringUtils::format(LL("ygysdyc"), line[cast]["expend_war"].asInt()).c_str());
			}

			ChessFree(sData);

			Label* jyInfo = LabelSystemFont(num.c_str(), 18);
			jyInfo->setColor(Color3B::YELLOW);
			jyInfo->setPosition(Vec2(bg->getContentSize().width / 2, 109));
			bg->addChild(jyInfo);
		}
	}
		break;
	case SHORTCUT_TYPE_JSZD:
	case SHORTCUT_TYPE_CCZC:
	{
		black_layer->setVisible(true);
		Sprite* cancelTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_3_1.png");
		cancelTXTN->setPosition(Vec2(40+13, 18+5));
		leftNormalImage->addChild(cancelTXTN);
		Sprite* cancelTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_4_1.png");
		cancelTXTS->setPosition(Vec2(40 + 13, 18 + 5));
		leftSelectImage->addChild(cancelTXTS);

		Sprite* sureTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_1_1.png");
		sureTXTN->setPosition(Vec2(40 + 13, 18 + 5));
		rightNormalImage->addChild(sureTXTN);
		Sprite* sureTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_2_1.png");
		sureTXTS->setPosition(Vec2(40 + 13, 18 + 5));
		rightSelectImage->addChild(sureTXTS);

		string titleS = "";
		if (SHORTCUT_TYPE_JSZD==mSType)
			titleS = LL("shifoujiesubenhuihe");
		else
			titleS = LL("cthzccz");
		Label* title = LabelSystemFont(titleS.c_str(), 18);
		title->setPosition(Vec2(bg->getContentSize().width/2, 132));
		title->setColor(Color3B(102, 0, 0));
		bg->addChild(title);
		history->updateGuide();
	}
		break;
	case SHORTCUT_TYPE_JSYX:
	{
		LayerColor* black_layer = LayerColor::create(Color4B(0,0,0,150));
		black_layer->setPosition(Vec2(0, 0));
		this->addChild(black_layer, -1);

		Label* title = LabelSystemFont(LL("feifashuju"), 18);
		title->setPosition(Vec2(bg->getContentSize().width / 2, 132));
		bg->addChild(title);

		mMenuClose->setVisible(false);
		mMenuLeft->setVisible(false);
		Sprite* sureTXTN = ReadSpriteName("new_ui/img_text/imgtxt4_1_1.png");
		sureTXTN->setPosition(Vec2(40 + 13, 18 + 5));
		rightNormalImage->addChild(sureTXTN);
		Sprite* sureTXTS = ReadSpriteName("new_ui/img_text/imgtxt4_2_1.png");
		sureTXTS->setPosition(Vec2(40 + 13, 18 + 5));
		rightSelectImage->addChild(sureTXTS);
		mMenuRight->setPositionX(bg->getContentSize().width / 2);
	}
		break;
	}

	return true;
}

void ShortcutFunctionLayer::ClickCloseMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	switch (mSType)
	{
	case SHORTCUT_TYPE_JSZD:
	{
		if (this->mDelegate)
			this->mDelegate->RunFunction(0);
	}
		break;
	}

	this->removeFromParent();
}

void ShortcutFunctionLayer::ClickRightMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	switch (mSType)
	{
	case SHORTCUT_TYPE_JSD:
	case SHORTCUT_TYPE_GYSD:
	{
		if (mDelegate)
			mDelegate->updateRTDate(1, mSType);
	}
		break;
	case SHORTCUT_TYPE_JSZD:
	{
		if (this->mDelegate)
			this->mDelegate->RunFunction(1);
	}
		break;
	case SHORTCUT_TYPE_CCZC:
	{
		if (this->mDelegate)
			this->mDelegate->updateRTDate(1);
	}
		break;
	case SHORTCUT_TYPE_JSYX:
	{
		ChessExit();
	}
		break;
	}

	this->removeFromParent();
}

void ShortcutFunctionLayer::ClickLeftMenu(Ref* sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	switch (mSType)
	{
	case SHORTCUT_TYPE_JSD:
	case SHORTCUT_TYPE_GYSD:
	{
		if (mDelegate)
			mDelegate->updateRTDate(0, mSType);
	}
		break;
	case SHORTCUT_TYPE_JSZD:
	{
		if (this->mDelegate)
			this->mDelegate->RunFunction(0);
	}
		break;
	}

	this->removeFromParent();
}

bool ShortcutFunctionLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void ShortcutFunctionLayer::onExit()
{
	Layer::onExit();
}
void ShortcutFunctionLayer::onEnter()
{
	Layer::onEnter();
}

ShortcutFunctionLayer::ShortcutFunctionLayer()
{

}

ShortcutFunctionLayer::~ShortcutFunctionLayer()
{

}