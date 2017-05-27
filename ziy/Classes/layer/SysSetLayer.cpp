//
//  SysSetLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/05/31.
//
//

#include "SysSetLayer.h"

enum 
{
	CHECK_MUSIC = 0,
	CHECK_SOUND,
	CHECK_SHOWHP,
	CHECK_SHOWMAP
};

bool SysSetLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	for(UInt i = 0; i < m_mCheckBox.size(); i++)
	{
		bool bChecked = history->setting_flag[i] ? true : false;
		m_mCheckBox[i]->setChecked(bChecked);
	}

	return true;
}

SysSetLayer::SysSetLayer()
{

}

void SysSetLayer::onEnter()
{
	Layer::onEnter();
}

void SysSetLayer::onExit()
{
	Layer::onExit();
}

bool SysSetLayer::onTouchBegan(Touch *touch, Event *pEvent)
{
	return true;
}

void SysSetLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{

}

void SysSetLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void SysSetLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* black = LayerColor::create(Color4B(0, 0, 0, 100));
	black->setPosition(Vec2(0, 0));
	this->addChild(black);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/sign/accounts_sm3.png");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//开启音乐
	Label* labMusic = LabelSystemFont(LL("tingbeijingyinyue"), 18);
	m_mCheckBox[0] = CheckBoxSprite::create(labMusic, this, menu_selector(SysSetLayer::callBackClick));
	m_mCheckBox[0]->setPosition(point + Vec2(-125, 54));
	m_mCheckBox[0]->setTag(CHECK_MUSIC);
	this->addChild(m_mCheckBox[0]);

	//开启音效
	Label* labSound = LabelSystemFont(LL("tingyinxiao"), 18);
	m_mCheckBox[1] = CheckBoxSprite::create(labSound, this, menu_selector(SysSetLayer::callBackClick));
	m_mCheckBox[1]->setPosition(point + Vec2(-125, 16));
	m_mCheckBox[1]->setTag(CHECK_SOUND);
	this->addChild(m_mCheckBox[1]);

	//显示血条
	Label* labShowHp = LabelSystemFont(LL("zhandoushixianshixuetiao"), 18);
	m_mCheckBox[2] = CheckBoxSprite::create(labShowHp, this, menu_selector(SysSetLayer::callBackClick));
	m_mCheckBox[2]->setPosition(point + Vec2(-125, -22));
	m_mCheckBox[2]->setTag(CHECK_SHOWHP);
	this->addChild(m_mCheckBox[2]);

	//显示小地图
	Label* labShowMap = LabelSystemFont(LL("zhandoushizidongxianshizhanchangsuoxiaotu"), 18);
	m_mCheckBox[3] = CheckBoxSprite::create(labShowMap, this, menu_selector(SysSetLayer::callBackClick));
	m_mCheckBox[3]->setPosition(point + Vec2(-125, -60));
	m_mCheckBox[3]->setTag(CHECK_SHOWMAP);
	this->addChild(m_mCheckBox[3]);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close2.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press2.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(SysSetLayer::callBackClose, this));
	btnClose->setPosition(point + Vec2(200, 125));
	ptrMenu->addChild(btnClose);
}

void SysSetLayer::callBackClick(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);

	CheckBoxSprite* ptrCheckBox = (CheckBoxSprite*)sender;
	int iCheckTag = ptrCheckBox->getTag();
	history->setting_flag[iCheckTag] = m_mCheckBox[iCheckTag]->getChecked();
	if(iCheckTag == CHECK_MUSIC)
	{
		if(history->setting_flag[0])
		{
			ChessPlayMp3(history->bg_music_index);
		}
		else
		{
			ChessStopMp3();
		}
	}
}

void SysSetLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	trData->saveSettingFlag();
	this->removeFromParent();
}