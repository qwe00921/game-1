//
//  PVPHomeLayer.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/21.
//
//

#include "PVPHomeLayer.h"
#include "PVPActorLayer.h"
#include "PVPArrayLayer.h"
#include "PVPReplayLayer.h"
#include "PVPRewardLayer.h"
#include "PVPRankLayer.h"

#define MAX_ACTOR_NUM 3

enum 
{
	PVP_CALL_ARRAY = 0, //守阵
	PVP_CALL_INFO = 1, //信息
	PVP_CALL_REWARD = 2, //奖励
	PVP_CALL_RANK = 3, //排行
	PVP_CALL_CLOSE = 9 //关闭
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPActorSprite::PVPActorSprite()
{

}

PVPActorSprite::~PVPActorSprite()
{

}

PVPActorSprite* PVPActorSprite::create(bool bIsClicked /*= true*/)
{
	PVPActorSprite* pRet = new PVPActorSprite();
	if (pRet&&pRet->init(bIsClicked))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PVPActorSprite::init(bool bIsClicked)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_ptrActor = nullptr;
	m_bIsClicked = bIsClicked;

	initControl();
	return true;
}

void PVPActorSprite::updateSprite(PVPActorData* ptrActor)
{
	m_ptrActor = ptrActor;
	//主角形象
	string strImg = StringUtils::format("new_ui/pvp/1_%d_%d.png", ptrActor->getGender(), ptrActor->getFashion1());
	m_spImg->setTexture(ReadSpriteTexture(strImg.c_str()));

	//名字
	m_labName->setString(ptrActor->getName());

	//排名
	m_spRank->removeAllChildren();
	int iRank = ptrActor->getRankID();
	if (iRank < 4)
	{
		m_spRank->setOpacity(255);
		string strRank = StringUtils::format("new_ui/img_global/global_ph_icon%d.png", iRank);
		m_spRank->setTexture(ReadSpriteTexture(strRank.c_str()));
	}
	else
	{
		m_spRank->setOpacity(0);
		auto spRankNum = ITools::getInstance()->getNumSprite(iRank, false, 11);
		spRankNum->setPosition(Vec2(0, 20));
		m_spRank->addChild(spRankNum);
	}

	//王族
	m_spVip->setVisible(ptrActor->getIsVIP());
}

void PVPActorSprite::initControl()
{
	//主角形象
	m_spImg = ReadSpriteName("new_ui/pvp/1_1_1.png");
	m_spImg->setPosition(Vec2(0, 0));
	this->addChild(m_spImg);

	//名字背景
	auto spNameBG = ReadSpriteName("new_ui/daily/stalker_bg2.png");
	spNameBG->setPosition(Vec2(0, -70));
	this->addChild(spNameBG);

	//名字
	m_labName = LabelSystemFont("XXX", 16);
	m_labName->setPosition(Vec2(0, -77));
	m_labName->enableOutline(Color4B::BLACK, 1);
	this->addChild(m_labName);

	//排名
	m_spRank = ReadSpriteName("new_ui/img_global/global_ph_icon1.png");
	m_spRank->setPosition(Vec2(-50, -43));
	this->addChild(m_spRank);

	//王族
	m_spVip = ReadSpriteName("new_ui/main/vip1.png");
	m_spVip->setPosition(Vec2(44, -45));
	this->addChild(m_spVip);

	//挑战
	MenuItem* btnClick = MenuItem::create(CC_CALLBACK_1(PVPActorSprite::callBackClick, this));
	btnClick->setContentSize(m_spImg->getContentSize());
	btnClick->setEnabled(m_bIsClicked);
	Menu* menuClick = Menu::create(btnClick, NULL);
	menuClick->setPosition(Vec2(0, 0));
	this->addChild(menuClick);
}

void PVPActorSprite::callBackClick(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->getParent()->addChild(PVPActorLayer::create(m_ptrActor));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPHomeLayer::PVPHomeLayer()
{

}

PVPHomeLayer::~PVPHomeLayer()
{

}

PVPHomeLayer* PVPHomeLayer::create()
{
	PVPHomeLayer* pRet = new PVPHomeLayer();
	if(pRet&&pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool PVPHomeLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	initControl();
	updateDisplay();
	return true;
}

void PVPHomeLayer::initControl()
{
	DSizeF size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	//遮罩
	LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
	layBlack->setPosition(Vec2(0, 0));
	this->addChild(layBlack);

	//背景1
	auto spBG1 = ReadSpriteName("new_ui/pvp/pvp_bg1.png");
	spBG1->setPosition(point + Vec2(0, 3));
	this->addChild(spBG1);

	//背景2
	auto spBG2 = ReadSpriteName("new_ui/pvp/pvp_bg2.png");
	spBG2->setPosition(point + Vec2(0, -13));
	this->addChild(spBG2);

	//我的排名
	m_spRank = ReadSpriteName("new_ui/pvp/pvp_txt1.png");
	m_spRank->setPosition(point + Vec2(-234, 112));
	this->addChild(m_spRank);

	//挑战武将
	map<int, Vec2> mPoint;
	mPoint[0] = Vec2(0, 78);
	mPoint[1] = Vec2(-180, 12);
	mPoint[2] = Vec2(180, 12);
	for (int i = 0; i < MAX_ACTOR_NUM; i++)
	{
		m_mSpActor[i] = PVPActorSprite::create();
		m_mSpActor[i]->setPosition(point + mPoint[i]);
		this->addChild(m_mSpActor[i]);
	}

	//挑战次数
	m_labTimes = LabelSystemFont("", 16);
	m_labTimes->setPosition(point + Vec2(0, -88));
	m_labTimes->setColor(Color3B::YELLOW);
	m_labTimes->enableOutline(Color4B(158, 116, 48, 255), 1);
	this->addChild(m_labTimes);

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//守阵
	Sprite* spArrayN = ReadSpriteName("new_ui/pvp/pvp_ban1.png");
	Sprite* spArrayS = ReadSpriteName("new_ui/pvp/pvp_ban1.png");
	spArrayS->setScale(fScale);
	MenuItemSprite* btnArray = MenuItemSprite::create(spArrayN, spArrayS, CC_CALLBACK_1(PVPHomeLayer::callBackFunc, this));
	spArrayS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spArrayS->setPosition(Vec2(spArrayS->getContentSize().width / 2, spArrayS->getContentSize().height / 2));
	btnArray->setPosition(point + Vec2(-243, -153));
	btnArray->setTag(PVP_CALL_ARRAY);
	ptrMenu->addChild(btnArray);

	//排行
	Sprite* spRankN = ReadSpriteName("new_ui/pvp/pvp_ban4.png");
	Sprite* spRankS = ReadSpriteName("new_ui/pvp/pvp_ban4.png");
	spRankS->setScale(fScale);
	MenuItemSprite* btnRank = MenuItemSprite::create(spRankN, spRankS, CC_CALLBACK_1(PVPHomeLayer::callBackFunc, this));
	spRankS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRankS->setPosition(Vec2(spRankS->getContentSize().width / 2, spRankS->getContentSize().height / 2));
	btnRank->setPosition(point + Vec2(-165, -153));
	btnRank->setTag(PVP_CALL_RANK);
	ptrMenu->addChild(btnRank);

	//奖励
	Sprite* spRewardN = ReadSpriteName("new_ui/pvp/pvp_ban3.png");
	Sprite* spRewardS = ReadSpriteName("new_ui/pvp/pvp_ban3.png");
	spRewardS->setScale(fScale);
	MenuItemSprite* btnReward = MenuItemSprite::create(spRewardN, spRewardS, CC_CALLBACK_1(PVPHomeLayer::callBackFunc, this));
	spRewardS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spRewardS->setPosition(Vec2(spRewardS->getContentSize().width / 2, spRewardS->getContentSize().height / 2));
	btnReward->setPosition(point + Vec2(165, -153));
	btnReward->setTag(PVP_CALL_REWARD);
	ptrMenu->addChild(btnReward);

	//信息
	Sprite* spInfoN = ReadSpriteName("new_ui/pvp/pvp_ban2.png");
	Sprite* spInfoS = ReadSpriteName("new_ui/pvp/pvp_ban2.png");
	Sprite* spInfoD = ReadSpriteName("new_ui/pvp/pvp_ban2.png");
	spInfoS->setScale(fScale);
	addSpriteGray(spInfoD);
	MenuItemSprite* btnInfo = MenuItemSprite::create(spInfoN, spInfoS, spInfoD, CC_CALLBACK_1(PVPHomeLayer::callBackFunc, this));
	spInfoS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spInfoS->setPosition(Vec2(spInfoS->getContentSize().width / 2, spInfoS->getContentSize().height / 2));
	btnInfo->setPosition(point + Vec2(243, -153));
	btnInfo->setTag(PVP_CALL_INFO);
	btnInfo->setEnabled(false);
	ptrMenu->addChild(btnInfo);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(PVPHomeLayer::callBackFunc, this));
	btnClose->setPosition(point + Vec2(290, 165));
	btnClose->setTag(PVP_CALL_CLOSE);
	ptrMenu->addChild(btnClose);
}

void PVPHomeLayer::updateDisplay()
{
	PVPData* ptrData = PVPData::getInstance();

	//名次
	m_spRank->removeAllChildren();
	Size szMark = m_spRank->getContentSize();
	auto spRankVal = ITools::getInstance()->getNumSprite(ptrData->getRandID(), false, 11);
	spRankVal->setPosition(Vec2(szMark.width + 10, szMark.height / 2));
	m_spRank->addChild(spRankVal);

	//挑战次数
	m_labTimes->setString(StringUtils::format(LL("pvpentertimes"), ptrData->getEntryTimes(), ptrData->getMaxTimes()));

	//挑战者
	int iIndex = 0;
	for (map<int, PVPActorData*>::iterator it = ptrData->m_mPVPActor.begin(); it != ptrData->m_mPVPActor.end(); it++)
	{
		m_mSpActor[iIndex++]->updateSprite(it->second);
	}
}

void PVPHomeLayer::onHttpRequest()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());
	ITools::getInstance()->connectingNetwork("PVPRank", HttpRequest::Type::POST, this, callfuncND_selector(PVPHomeLayer::onHttpRequestCompleted), data->getCString(), ITools::getInstance()->mHead, "REQ_RANK_DATA");
	ITools::getInstance()->addWaitLayer();
}

void PVPHomeLayer::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	string responseData = temp;
	CCLOG("%s...", responseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData, value) && value.size() > 0)
	{
		if (strTag == "REQ_RANK_DATA")
		{
			this->addChild(PVPRankLayer::create(value["result"]));
		}
	}
}

void PVPHomeLayer::callBackFunc(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	
	int iTag = ((MenuItemSprite*)sender)->getTag();
	switch (iTag)
	{
	case PVP_CALL_ARRAY:
		this->addChild(PVPArrayLayer::create(ARRAY_DEFEND));
		break;
	case PVP_CALL_INFO:
		this->addChild(PVPReplayLayer::create());
		break;
	case PVP_CALL_REWARD:
		this->addChild(PVPRewardLayer::create());
		break;
	case PVP_CALL_RANK:
		onHttpRequest();
		break;
	case PVP_CALL_CLOSE:
		this->removeFromParent();
		break;
	default:
		break;
	}
}