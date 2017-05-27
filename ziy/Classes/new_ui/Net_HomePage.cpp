//
//  Net_HomePage.cpp
//  TRGame
//
//  Created by 袁文彬 on 16/4/13.
//
//

#include "Net_HomePage.h"
#include "Defines.h"
#include "DestroySixCountriesLayer.h"
#include "BackpackLayer.h"
#include "TitleBarLayer.h"
#include "ArmorListLayer.h"
#include "EquipmentUp.h"
#include "GetNetDateTool.h"
#include "GoodsShopLayer.h"
#include "PubDrinkLayer.h"
#include "MailLayer.h"
#include "HeroicAmbitionLayer.h"
#include "ActivityLayer.h"
#include "ActSceneLayer.h"
#include "NoticeActOpenLayer.h"
#include "RomanceLayer.h"
#include "DailySceneLayer.h"
#include "LadderLayer.h"
#include "BattleLadder.h"
#include "DailySignLayer.h"
#include "MergeLayer.h"
#include "ActivityInfo.h"
#include "NetPromptTool.h"
#include "CwArrayLayer.h"
#include "CityWarMapLayer.h"
#include "GoodsStoreLayer.h"
#include "PaymentTool.h"
#include "UserDate.h"
#include "PayRestoreLayer.h"
#include "GuideLayer.h"
#include "GreatBattleLayer.h"
#include "BattleReplay.h"
#include "PVPData.h"
#include "PVPSelectLayer.h"
#include "HelpLayer.h"

#define MAX_ITEM_NUM 10

enum
{
	REQ_VIPGIFT = 1, //VIP礼包
	REQ_ACTSCENE = 2, //活动副本
	REQ_GM = 3,//GM命令请求
	REQ_MAILINFO = 4, //邮件信息
	REQ_SIGIN,//签到信息
	REQ_GONGHUI,//工会信息
	REQ_JIAREUGONGHUI//是否加入工会
};

float Net_HomePage::ms_fOffsetX = 0.0f;
HongDian* HongDian::m_iHongDian = NULL;
map<int, int> HongDian::novisible;

Net_HomePage* Net_HomePage::create(bool enable_initdate)
{
	Net_HomePage* pRet = new Net_HomePage();
	pRet->mEnableInitDate = enable_initdate;
	if (pRet&&pRet->init())
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

bool Net_HomePage::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Json::Value callStr;
	callStr["name"] = "keepScreenOn";
	callStr["keep"] = false;
	cppBridgeCall(callStr.toStyledString().c_str());

	m_hongdian = HongDian::getInstence();
	this->addChild(m_hongdian);

	m_iClickMode = -1;
	if (mEnableInitDate) {
		ms_fOffsetX = 0.0f;
		//InitHistory();
		NetPromptTool::getInstance()->StartGetNetPrompt();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		ReceiptInfo* ptrReceipt = UserDate::getInstance()->getReceiptInfo();
		if (ptrReceipt)
		{
			PaymentTool::getInstance()->startRestore(ptrReceipt->getOrderID(), ptrReceipt->getTitle(), this);
			PaymentTool::getInstance()->setIOSReceipt(ptrReceipt->getReceipt(), ptrReceipt->getRecNum());
			StatConfig::onPayResult(StatConfig::PAY_TYPE_IOS_APPSTORE, ptrReceipt->getOrderID().c_str(), 1, "IOSPAY");
		}
#endif
	}


	if (history->mMapDateInfo->isPassed(102) == false)
	{
		ms_fOffsetX = 0.0f;
		/*if (history->mMapDateInfo->isPassed(101) == true)
		{
			ms_fOffsetX = -80.0f;
		}
		if (history->mMapDateInfo->isPassed(99) == true&& history->mMapDateInfo->isPassed(100) == false)
		{
			ms_fOffsetX = -80.0f;
		}*/
	}

	this->scheduleOnce(schedule_selector(Net_HomePage::PlayHomeBgMp3), 0.2f);

	initControl();
	history->m_layTitleBar = TitleBarLayer::create();
	this->addChild(history->m_layTitleBar);
	history->m_layHome = this;

	if (history->m_mActScene.size())
	{
		addActSceneMenu();
	}
	else
	{
		onHttpRequest(REQ_ACTSCENE);
	}
	/*auto sign = DailySignLayer::create();
	sign->setVisible(false);
	sign->setTouchEnabled(false);
	sign->setSwallowsTouches(false);
	this->addChild(sign);*/

	if (mEnableInitDate)
	{
		onHttpRequest(REQ_MAILINFO);
	}
	else
	{
		updateMailState();
	}

	if (mEnableInitDate)
	{
		onHttpRequest(REQ_SIGIN);
	}
	if (mEnableInitDate)
	{
		onHttpRequest(REQ_JIAREUGONGHUI);
	}
	

	for (int i= EMIL_GUIDE;i<=ZHUJIEMIAN6_GUIDE;i++)
	{
		if (history->mMapDateInfo->isPassed(94+i-5))
		{
			history->updateGuide(i);
		}
	}
	m_hongdian->autoUpdate();
	
	return true;
}

bool Net_HomePage::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	for (int i = 0; i < (int)m_mArmMode.size(); i++)
	{
		if (m_mArmRect[i].containsPoint(m_spBG1->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickMode = i;
			if (i==2)
			{
				m_mArmMode[i]->getAnimation()->play("city_102_open");
			}
			break;
		}
	}

	return true;
}

void Net_HomePage::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer")!=NULL)
	{
		return;
	}

	float fOffsetX = touch->getDelta().x;
	float fWidth = m_spBG1->getContentSize().width / 2;
	if (m_spBG1->getPosition().x + fOffsetX >= m_szScreen.width - fWidth && m_spBG1->getPosition().x + fOffsetX <= fWidth)
	{
		m_spBG1->setPosition(m_spBG1->getPosition() + Vec2(fOffsetX, 0));
		m_spBG2->setPosition(m_spBG2->getPosition() + Vec2(fOffsetX / 2, 0));
		m_spBG3->setPosition(m_spBG3->getPosition() + Vec2(fOffsetX / 4, 0));
		m_spBG4->setPosition(m_spBG4->getPosition() + Vec2(fOffsetX / 8, 0));
		ms_fOffsetX = m_spBG1->getPosition().x - m_szScreen.width / 2;
	}

	float fAbsX = fabs(touch->getLocation().x - touch->getStartLocation().x);
	int _xa = 10;
	if (Director::getInstance()->getRunningScene()->getChildByName("GuideLayer") != NULL)
	{
		_xa = 50;
	}
	if (m_iClickMode >= 0)
	{
		if (fAbsX > _xa || !m_mArmRect[m_iClickMode].containsPoint(m_spBG1->convertToNodeSpace(touch->getLocation())))
		{
			m_iClickMode = -1;
		}
	}

	if (fAbsX > _xa)
	{
		updateMenu(false);
	}
}

void Net_HomePage::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	if (m_iClickMode >= 0)
	{
		playSound(SOUND_LEFT_MOUSE_CLICK);
		ScaleBy* actScale = ScaleBy::create(0.08f, 0.9f);
		CallFunc* actFunc = CallFunc::create(CC_CALLBACK_0(Net_HomePage::callBackMenu, this));
		m_mArmMode[m_iClickMode]->runAction(Sequence::create(actScale, actScale->reverse(), actFunc, NULL));
		
	}
}

void Net_HomePage::updateState()
{
	m_btnVipGift->setVisible(history->m_iVIPGift ? true : false);
	updateMailState();
}

void Net_HomePage::updateRTDate(map<int, string> mStrNotice)
{
	ChessApplication::getInstance()->AddFunctionLayer(PayRestoreLayer::create(mStrNotice[0], mStrNotice[1]));
}

void Net_HomePage::menuBack(Ref* obj)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	MenuItemSprite* item = (MenuItemSprite*)obj;
	if (item->getTag() == 1)
	{
		CCLOG("合成");
		auto aa = MergeLayer::create();
		//aa->addChild(AbstructHelpBtn::create(0, "new_ui/img_global/suit_bnt1.png","new_ui/gh/gh_bg1.png"));
		if (trData->isSystemOpen(SystemOpenIDS::E_HECHENGXIAOHAO, true))
		ChessApplication::getInstance()->AddFunctionLayer(aa);
	}
	else if (item->getTag() == 2)
	{
		CCLOG("活动");
		if (trData->isSystemOpen(SystemOpenIDS::E_HUODONG, true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(ActivityLayer::create());
			HongDian::novisible[98] = 1;
		}
	}
	else if (item->getTag() == 3)
	{
		CCLOG("add");
		updateMenu(!isOpen);
		history->updateGuide(EMIL_GUIDE);

	}
	else if (item->getTag() == 4)
	{
		CCLOG("签到");
		if (trData->isSystemOpen(SystemOpenIDS::E_QIANDAO, true))
		ChessApplication::getInstance()->AddFunctionLayer(DailySignLayer::create());
	}
	else if (item->getTag() == 5)
	{
		CCLOG("VIP礼包");
		onHttpRequest(REQ_VIPGIFT);
	}
	else if (item->getTag() == 6)
	{

	}
	else if (item->getTag() == 7)
	{

	}
	else if (item->getTag() == 10)
	{
		CCLOG("武将");
		if (trData->isSystemOpen(SystemOpenIDS::E_WUJIANG, true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(ArmorListLayer::create());
			HongDian::novisible[101] = 1;
		}
	}
	else if (item->getTag() == 11)
	{
		CCLOG("装备");
	}
	else if (item->getTag() == 12)
	{
		CCLOG("锻造");
		if (history->GetAllTreasure(MODEL_MAX).size())
		{
			if (trData->isSystemOpen(SystemOpenIDS::E_DUANZAO, true))
			{
				ChessApplication::getInstance()->AddFunctionLayer(EquipmentUp::create());
				HongDian::novisible[103] = 1;
			}
		}
		else
		{
			ITools::getInstance()->createGlobalPrompt(LL("mybwkyqh"), false);
		}
	}
	else if (item->getTag() == 13)
	{
		CCLOG("修炼");
	}
	else if (item->getTag() == 14)
	{
		CCLOG("杂货店");
	}
	else if (item->getTag() == 15)
	{
		CCLOG("背包");
		if (trData->isSystemOpen(SystemOpenIDS::E_BEIBAO, true))
		ChessApplication::getInstance()->AddFunctionLayer(BackpackLayer::create());
	}
	else if (item->getTag() == 16)
	{
		CCLOG("工会");
		int iClanNum = 0;
		for (int i = 0; i < history->ownArmyNum; i++)
		{
			if (history->actors[history->ownArmy[i]].GetActorLv() >= 40)
			{
				iClanNum++;
			}
		}

		if (trData->isSystemOpen(SystemOpenIDS::E_Guild, true))
			GetNetDateTool::getInstance()->SendClanInfoData();
		//else
			//ITools::getInstance()->createGlobalPrompt(LL("xybgssjwjkq"), false);
	}
	else if (item->getTag() == 17)
	{
		//充值榜
		//GetNetDateTool::getInstance()->SendChongZhiRankData();
	}
	else if (item->getTag() >= 20 && item->getTag() < 25)
	{
		CCLOG("活动1");
			ChessApplication::getInstance()->AddFunctionLayer(ActSceneLayer::create(history->m_mActScene[item->getTag() - 20]));
			history->m_iActivityEntry = 1;
		
		
	}
}

Net_HomePage::Net_HomePage()
{
	isOpen = false;
}

Net_HomePage::~Net_HomePage()
{
	history->m_layTitleBar = NULL;
	history->m_layHome = NULL;
}

void Net_HomePage::InitHistory()
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_female.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_male.ExportJson", 0, 1));
	history->loadUserData();
	history->initNetDate();
	if (history->m_pPlayerInfo->getMainHero()) {
		BTActivity::getInstance()->init();
		auto netTool = GetNetDateTool::getInstance();
		PVPData::getInstance()->init();
		netTool->GetMapStarInfo();
		netTool->SendRomanceOpenData();
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	ReceiptInfo* ptrReceipt = UserDate::getInstance()->getReceiptInfo();
	if(ptrReceipt)
	{
		PaymentTool::getInstance()->startRestore(ptrReceipt->getOrderID(), ptrReceipt->getTitle(), this);
		PaymentTool::getInstance()->setIOSReceipt(ptrReceipt->getReceipt(), ptrReceipt->getRecNum());
		StatConfig::onPayResult(StatConfig::PAY_TYPE_IOS_APPSTORE, ptrReceipt->getOrderID().c_str(), 1, "IOSPAY");
	}
#endif
}

void Net_HomePage::initControl()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	m_szScreen = Size(size.width, size.height);
	float fScale = 0.90f;

	//加载人物动画文件
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/icon_eff.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/city_ani.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_female.ExportJson", 0, 1));
	ArmatureDataManager::getInstance()->addArmatureFileInfo(getRespath("special_effect/r_male.ExportJson", 0, 1));

	//背景图片
	m_spBG4 = ReadSpriteName("new_ui/main/city_bg4.jpg");
	this->addChild(m_spBG4);

	m_spBG3 = ReadSpriteName("new_ui/main/city_bg3.png");
	this->addChild(m_spBG3);

	m_spBG2 = ReadSpriteName("new_ui/main/city_bg2.png");
	this->addChild(m_spBG2);

	m_spBG1 = ReadSpriteName("new_ui/main/city_bg1.png");
	this->addChild(m_spBG1);

	float fHight1 = m_spBG1->getContentSize().height;
	m_spBG1->setPosition(point + Vec2(ms_fOffsetX, 0));
	m_spBG2->setPosition(point + Vec2(ms_fOffsetX / 2, (fHight1 - m_spBG2->getContentSize().height) / 2));
	m_spBG3->setPosition(point + Vec2(ms_fOffsetX / 4, (fHight1 - m_spBG3->getContentSize().height) / 2));
	m_spBG4->setPosition(point + Vec2(ms_fOffsetX / 8, (fHight1 - m_spBG4->getContentSize().height) / 2));

	//菜单
	m_pBtnMenu = Menu::create();
	m_pBtnMenu->setPosition(Vec2(0, 0));
	this->addChild(m_pBtnMenu);

	//活动
	Sprite* spActiveN = ReadSpriteName("new_ui/icon_ui/icon20_1.png");
	Sprite* spActiveS = ReadSpriteName("new_ui/icon_ui/icon20_1.png");
	spActiveS->setScale(fScale);
	MenuItemSprite* btnActive = MenuItemSprite::create(spActiveN, spActiveS, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	spActiveS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spActiveS->setPosition(Vec2(spActiveS->getContentSize().width / 2, spActiveS->getContentSize().height / 2));
	btnActive->setTag(2);
	btnActive->setPosition(Vec2(size.width - (8 + btnActive->getContentSize().width / 2), size.height - (8 + btnActive->getContentSize().height / 2)));
	m_pBtnMenu->addChild(btnActive);
	m_hongdian->addHongDianNode(btnActive, 98);

	//签到
	Sprite* spMailN = ReadSpriteName("new_ui/icon_ui/icon20_2.png");
	Sprite* spMailS = ReadSpriteName("new_ui/icon_ui/icon20_2.png");
	spMailS->setScale(fScale);
	MenuItemSprite* btnMail = MenuItemSprite::create(spMailN, spMailS, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	spMailS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spMailS->setPosition(Vec2(spMailS->getContentSize().width / 2, spMailS->getContentSize().height / 2));
	btnMail->setTag(4);
	btnMail->setPosition(btnActive->getPosition() + Vec2(-btnActive->getContentSize().width - 8, 0));
	m_pBtnMenu->addChild(btnMail);
	m_hongdian->addHongDianNode(btnMail, 99);


	////充值榜
	//Sprite* spChongZhiN = ReadSpriteName("new_ui/img_global/icon_cjcz.png");
	//Sprite* spChongZhiS = ReadSpriteName("new_ui/img_global/icon_cjcz.png");
	//spChongZhiS->setScale(fScale);
	//MenuItemSprite* btnChongZhi = MenuItemSprite::create(spChongZhiN, spChongZhiS, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	//spChongZhiS->setAnchorPoint(Vec2(0.5f, 0.5f));
	//spChongZhiS->setPosition(Vec2(spChongZhiS->getContentSize().width / 2, spChongZhiS->getContentSize().height / 2));
	//btnChongZhi->setTag(17);
	//btnChongZhi->setPosition(btnMail->getPosition() + Vec2(-btnMail->getContentSize().width - 8, 0));
	//m_pBtnMenu->addChild(btnChongZhi);

	//add菜单
	Sprite* addNormalImage = ReadSpriteName("new_ui/main/tb_icon2.png");
	Sprite* addSelectImage = ReadSpriteName("new_ui/main/tb_icon1.png");
	add = MenuItemSprite::create(addNormalImage, addSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	add->setTag(3);
	add->setPosition(Vec2(size.width - (8 + add->getContentSize().width / 2), 5 + add->getContentSize().height / 2));
	m_pBtnMenu->addChild(add);

	m_hongdian->addHongDianNode(add,100);

	int py = -100;
	int px = 100;

	//武将
	Sprite* generalsNormalImage = ReadSpriteName("new_ui/icon_ui/icon1_1.png");
	Sprite* generalsSelectImage = ReadSpriteName("new_ui/icon_ui/icon1_1.png");
	generalsSelectImage->setScale(fScale);
	generals = MenuItemSprite::create(generalsNormalImage, generalsSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	generalsSelectImage->setAnchorPoint(Vec2(0.5f, 0.5f));
	generalsSelectImage->setPosition(Vec2(generalsSelectImage->getContentSize().width / 2, generalsSelectImage->getContentSize().height / 2));
	generals->setTag(10);
	generals->setPosition(Vec2(add->getPosition().x - (generals->getContentSize().width + 8), add->getPosition().y + py));
	m_pBtnMenu->addChild(generals);
	m_hongdian->addHongDianNode(generals, 101);

	//背包
	Sprite* knapsackNormalImage = ReadSpriteName("new_ui/icon_ui/icon1_2.png");
	Sprite* knapsackSelectImage = ReadSpriteName("new_ui/icon_ui/icon1_2.png");
	knapsackSelectImage->setScale(fScale);
	knapsack = MenuItemSprite::create(knapsackNormalImage, knapsackSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	knapsackSelectImage->setAnchorPoint(Vec2(0.5f, 0.5f));
	knapsackSelectImage->setPosition(Vec2(knapsackSelectImage->getContentSize().width / 2, knapsackSelectImage->getContentSize().height / 2));
	knapsack->setTag(15);
	knapsack->setPosition(Vec2(generals->getPosition().x - (knapsack->getContentSize().width + 8), add->getPosition().y + py));
	m_pBtnMenu->addChild(knapsack);
	m_hongdian->addHongDianNode(knapsack, 102);

	//锻造
	Sprite* forgingNormalImage = ReadSpriteName("new_ui/icon_ui/icon1_3.png");
	Sprite* forgingSelectImage = ReadSpriteName("new_ui/icon_ui/icon1_3.png");
	forgingSelectImage->setScale(fScale);
	forging = MenuItemSprite::create(forgingNormalImage, forgingSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	forgingSelectImage->setAnchorPoint(Vec2(0.5f, 0.5f));
	forgingSelectImage->setPosition(Vec2(forgingSelectImage->getContentSize().width / 2, forgingSelectImage->getContentSize().height / 2));
	forging->setTag(12);
	forging->setPosition(Vec2(knapsack->getPosition().x - (forging->getContentSize().width + 8), add->getPosition().y + py));
	m_pBtnMenu->addChild(forging);
	m_hongdian->addHongDianNode(forging, 103);

	//工会
	Sprite* ClanNormalImage = ReadSpriteName("new_ui/icon_ui/icon1_4.png");
	Sprite* ClanSelectImage = ReadSpriteName("new_ui/icon_ui/icon1_4.png");
	ClanSelectImage->setScale(fScale);
	clan = MenuItemSprite::create(ClanNormalImage, ClanSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	ClanSelectImage->setAnchorPoint(Vec2(0.5f, 0.5f));
	ClanSelectImage->setPosition(Vec2(ClanSelectImage->getContentSize().width / 2, ClanSelectImage->getContentSize().height / 2));
	clan->setTag(16);
	clan->setPosition(Vec2(forging->getPosition().x - (clan->getContentSize().width + 8), add->getPosition().y + py));
	m_pBtnMenu->addChild(clan);
	m_hongdian->addHongDianNode(clan, 104);

	//合成
	Sprite* shoppingMallNormalImage = ReadSpriteName("new_ui/icon_ui/icon1_10.png");
	Sprite* shoppingMallSelectImage = ReadSpriteName("new_ui/icon_ui/icon1_10.png");
	shoppingMallSelectImage->setScale(fScale);
	shoppingMall = MenuItemSprite::create(shoppingMallNormalImage, shoppingMallSelectImage, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	shoppingMallSelectImage->setAnchorPoint(Vec2(0.5f, 0.5f));
	shoppingMallSelectImage->setPosition(Vec2(shoppingMallSelectImage->getContentSize().width / 2, shoppingMallSelectImage->getContentSize().height / 2));
	shoppingMall->setTag(1);
	shoppingMall->setPosition(Vec2(add->getPosition().x + px, add->getPosition().y + (knapsack->getContentSize().height + 8)));
	m_pBtnMenu->addChild(shoppingMall);
	m_hongdian->addHongDianNode(shoppingMall, 105);

	//VIP礼包
	Sprite* spGiftN = ReadSpriteName("new_ui/img_global/icon_vipbao.png");
	Sprite* spGiftS = ReadSpriteName("new_ui/img_global/icon_vipbao.png");
	spGiftS->setScale(fScale);
	m_btnVipGift = MenuItemSprite::create(spGiftN, spGiftS, CC_CALLBACK_1(Net_HomePage::menuBack, this));
	spGiftS->setAnchorPoint(Vec2(0.5f, 0.5f));
	spGiftS->setPosition(Vec2(spGiftS->getContentSize().width / 2, spGiftS->getContentSize().height / 2));
	m_btnVipGift->setTag(5);
	m_btnVipGift->setPosition(Vec2(5 + m_btnVipGift->getContentSize().width / 2, size.height - 92));
	m_btnVipGift->setVisible(history->m_iVIPGift ? true : false);
	m_pBtnMenu->addChild(m_btnVipGift);

	map<int, Vec2> mPostion;
	mPostion[0] = Vec2(870, 152);
	mPostion[1] = Vec2(821, 296);
	mPostion[2] = Vec2(1042, 108);
	mPostion[3] = Vec2(1280, 265);
	mPostion[4] = Vec2(1002, 295);
	mPostion[5] = Vec2(593, 283);
	mPostion[6] = Vec2(610, 116);
	mPostion[7] = Vec2(312, 300);
	mPostion[8] = Vec2(70, 270);
	mPostion[9] = Vec2(370, 163);

	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		m_mArmMode[i] = Armature::create("city_ani");

		string strName = StringUtils::format("city_10%d", i);
		m_mArmMode[i]->getAnimation()->play(strName);
		m_mArmMode[i]->setPosition(mPostion[i]);
		m_mArmRect[i] = m_mArmMode[i]->getBoundingBox();
		m_spBG1->addChild(m_mArmMode[i]);
		//添加红点提示
		m_hongdian->addHongDianNode(m_mArmMode[i],i);
	}

	//邮箱提示
	m_spMailMark = ReadSpriteName("new_ui/img_global/icon_xh.png");
	m_spMailMark->setPosition(Vec2(25, 25));
	m_mArmMode[2]->addChild(m_spMailMark, 99);
	m_spMailMark->setVisible(false);

	//树叶
	/*auto armLeaf = Armature::create("city_ani");
	armLeaf->getAnimation()->play("city_yz");
	armLeaf->setPosition(Vec2(216, 428));
	m_spBG1->addChild(armLeaf, 5);*/

#ifdef ENABLE_GM
	////gm输入框
	//auto spUserBG = ui::Scale9Sprite::create(getRespath("new_ui/hyaline/92x30.png", 0, 1));
	//m_pEditBox = ui::EditBox::create(spUserBG->getContentSize(), spUserBG);
	//m_pEditBox->setPosition( Vec2(500, 360));
	//m_pEditBox->setFontSize(16);
	//m_pEditBox->setFontColor(Color3B::WHITE);
	//m_pEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	//m_pEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	//m_pEditBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
	//m_pEditBox->setDelegate(this);
	//m_pEditBox->setPlaceHolder("Input GM cmds here.");
	//this->addChild(m_pEditBox);
#endif

	this->setEnterStatus();


	//#ifdef GAME_RES_MINI
	//	std::vector<const char*> packImgs;
	//	packImgs.push_back("special_effect/city_ani0.png");
	//	packImgs.push_back("special_effect/special_eff0.png");
	//	packImgs.push_back("special_effect/icon_eff0.png");
	//	//etc ..
	//	for (auto val : packImgs)
	//	{
	//		MiniSupport::checkMiniTRPacketImage(val, 1, 1);
	//	}
	//
	//#endif // GAME_RES_MINI
}


void grayArmature(Armature* node,int i) {
	/*const Map<std::string, Bone*>& map = node->getBoneDic();
	for (auto& element : map)
	{
		auto ls = element.second->getDisplayManager()->getDecorativeDisplayList();
		for (int i = 0; i < ls.size(); i++)
		{
			auto e = ls.at(i);
			auto disnode = e->getDisplay();
			if (disnode)
			{
				addSpriteGray(disnode);
			}
		}
	}*/
	string strName = StringUtils::format("city_10%d_1", i);
	node->getAnimation()->play(strName);
}

void resetGrayArmature(Armature* node,int i) {
	/*const Map<std::string, Bone*>& map = node->getBoneDic();
	for (auto& element : map)
	{
		auto ls = element.second->getDisplayManager()->getDecorativeDisplayList();
		for (int i = 0; i < ls.size(); i++)
		{
			auto e = ls.at(i);
			auto disnode = e->getDisplay();
			if (disnode)
			{
				removeSpriteGray(disnode);
			}
		}
	}*/
	string strName = StringUtils::format("city_10%d", i);
	node->getAnimation()->play(strName);
}

void Net_HomePage::setEnterStatus() {
	if (m_mArmMode[0] == nullptr) {
		return;
	}
	for (int i = 0; i < MAX_ITEM_NUM; i++)
	{
		if (IsOpen(i, false))
		{
			resetGrayArmature(m_mArmMode[i],i);
		}
		else {
			grayArmature(m_mArmMode[i],i);
		}
	}
}

void Net_HomePage::playAnimation(string name, int type)
{
	m_mArmMode[type]->getAnimation()->play(name);
}

void Net_HomePage::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	__String* strData = NULL;
	string strTag = "";
	CocosMD5 md5;

	switch (iReqType)
	{
	case REQ_VIPGIFT:
	{
		strURL = "receiveVip";
		__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_VIPGIFT";
	}
	break;
	case REQ_ACTSCENE:
	{
		strURL = "getAllActivityWar";
		__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_ACTSCENE";
	}
	break;
	case REQ_GM:
	{
#ifdef ENABLE_GM
		strURL = "gm_cmd";
		const char* cmd = m_pEditBox->getText();
		__String* data = __String::createWithFormat("cmd=%s&gameToken=%s", cmd, strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"cmd\":\"%s\",\"gameToken\":\"%s\",\"validateCode\":\"%s\"}",cmd, strToken.c_str(), validCode.c_str());
		strTag = "REQ_GM";
#endif
	}
	break;
	case REQ_MAILINFO:
	{
		strURL = "getAllEmail";
		__String* data = __String::createWithFormat("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data->getCString());
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_MAILINFO";
	}
	break;
	case REQ_SIGIN:
	{
		strURL = "getSignData";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SIGN_INIT";
	}
	break;
	case REQ_GONGHUI:
	{
		strURL = "getGuildActiveTask";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "SendActiveTaskNetData";
	}
	break;
	case REQ_JIAREUGONGHUI:
	{
		strURL = "selectGuildByUser";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "Post_SendClanInfoData";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData->getCString());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(Net_HomePage::onHttpRequestCompleted), strData->getCString(), ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void Net_HomePage::onHttpRequestCompleted(Node *sender, void *resultdata)
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
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (strTag == "REQ_VIPGIFT")
		{
			onVipGiftSucceed(value["result"]);
		}
		else if (strTag == "REQ_ACTSCENE")
		{
			onActSceneSucceed(value["result"]);
			this->setEnterStatus();
		}
		else if (strTag == "REQ_GM") {
#ifdef ENABLE_GM
			Json::Value valRs = value["result"]["v"];
			Json::Value valKey = value["result"]["k"];
			string keyStr = valKey.asCString();
			if (keyStr.compare("gold") == 0) {
				history->money = valRs.asInt();
			}
			else if (keyStr.compare("jade") == 0) {
				history->m_iJade = valRs.asInt();
			}
			else if (keyStr.compare("jadeSeal") == 0) {
				history->m_iJadeSeal = valRs.asInt();
			}
			else if (keyStr.compare("VIP") == 0) {
				history->m_iVIP = valRs.asInt();
			}
			else if (keyStr.compare("improvedStone") == 0) {
				history->stone = valRs.asInt();
			}
			else if (keyStr.compare("jadeSeal") == 0) {
				history->m_iJadeSeal = valRs.asInt();
			}
			else if (keyStr.compare(".pass") == 0) {
				history->m_pPlayerInfo->loadGuideStatus(valRs);
			}
			else if (keyStr.compare(".addgood") == 0) {
				history->onHttpGoodsAdd(valRs,false);
			}
			else if (keyStr.compare(".advance") == 0) {
				history->m_pPlayerInfo->loadJson(valRs);
				Net_HomePage::InitHistory();
			}
			else if (keyStr.compare(".passAll") == 0) {
				auto netTool = GetNetDateTool::getInstance();
				netTool->GetMapStarInfo();
			}
			else if (keyStr.compare(".hero100") == 0) {
				history->m_pPlayerInfo->loadJson(valRs);
			}

			//刷新标题栏金币勾玉数据
			if (history->m_layTitleBar)
			{
				history->m_layTitleBar->updateShow(0);
			}
#endif // ENABLE_GM

		}
		else if (strTag == "REQ_MAILINFO")
		{
			onMailInfoSucceed(value["result"]);
		}
		else if (strTag == "SIGN_INIT")
		{
			BTActivity::getInstance()->loadSignJson(value["result"]);
			HongDian::getInstence()->autoUpdate();
		}
		else if (strTag == "SendActiveTaskNetData")
		{
			bool flag = false;
			for (int i = 0; i < 3; i++)
			{
				if (value["result"]["tasks"][i]["finishNum"].isInt()) {
					if (value["result"]["tasks"][i]["finishNum"].asInt() == 0)
					{
						flag = true;
					}
				}
			}
			if (flag)
			{
				HongDian::getInstence()->HongDianNodeisVisibol[104] = 1;
				HongDian::getInstence()->autoUpdate();
			}
			else
			{
				HongDian::getInstence()->HongDianNodeisVisibol[104] = 0;
				HongDian::getInstence()->autoUpdate();
			}
			
		}
		else if (strTag == "Post_SendClanInfoData")
		{
			Json::Value Vmapinfo = value["result"];

			if (Vmapinfo["isTrue"].isInt())
			{
				if (Vmapinfo["isTrue"].asInt() != 0)
				{
					onHttpRequest(REQ_GONGHUI);
				}
				else
				{
					if (Vmapinfo["guildName"].isString()) {
						history->m_pPlayerInfo->setGuildName(Vmapinfo["guildName"].asCString());
						history->m_pPlayerInfo->setGuildId(Vmapinfo["num"].asInt());
					}
					
				}
			}
		}
	}
}

bool Net_HomePage::onVipGiftSucceed(Json::Value valRes)
{
	Json::Value valGoods = valRes["goods"];
	if (valGoods.isArray())
	{
		for (UInt i = 0; i < valGoods.size(); i++)
		{
			ConsumeElem consuElem;
			consuElem.loadJson(valGoods[i]);

			HistoryGoods hisGoods(consuElem.getGoodID());
			if (hisGoods.isConsume() || hisGoods.isPreWarConsume() || hisGoods.isPlotGoods())
			{
				history->m_mConsuNum[consuElem.getGoodID()] += consuElem.getCount();
				history->m_mConsuNetId[consuElem.getGoodID()] = consuElem.getNetID();
			}

			__String* strNotice = __String::createWithFormat(LL("huodess"), hisGoods.GetGoodsName(consuElem.getCount()));
			ITools::getInstance()->createGlobalPrompt(strNotice->getCString(), true);
		}
	}

	history->m_iVIPGift = 0;
	m_btnVipGift->setVisible(false);

	return true;
}

bool Net_HomePage::onActSceneSucceed(Json::Value valRes)
{
	Json::Value valAct = valRes["activityWars"];
	if (valAct.isArray())
	{
		history->m_mActScene.clear();
		for (UInt i = 0; i < valAct.size(); i++)
		{
			int iWarId = valAct[i]["warID"].asInt();
			history->m_mActScene[i] = iWarId;

			DString strTime = valAct[i]["startDate"].asCString();
			int iIndex = strTime->indexOf('T');
			strTime = strTime->substring(5, iIndex);
			trData->m_mNetWarInfo[iWarId]->strStart = strTime->getChar();

			strTime = valAct[i]["endDate"].asCString();
			iIndex = strTime->indexOf('T');
			strTime = strTime->substring(5, iIndex);
			trData->m_mNetWarInfo[iWarId]->strEnd = strTime->getChar();
		}
	}

	addActSceneMenu();
	if (history->m_mActScene.size() > 0 && !history->m_bGuide[0]) //新手引导不弹提示
	{
		ChessApplication::getInstance()->AddFunctionLayer(NoticeActOpenLayer::create(history->m_mActScene));
	}
	return true;
}

bool Net_HomePage::onMailInfoSucceed(Json::Value valRes)
{
	Json::Value valMail = valRes["emails"];
	for (UInt i = 0; i < valMail.size(); i++)
	{
		if (valMail[i]["statu"].isInt() && valMail[i]["statu"].asInt() == 1) //未读
		{
			history->m_iMailNum++;
		}
	}
	updateMailState();
	return true;
}

void Net_HomePage::addActSceneMenu()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
	float fScale = 0.90f;

	map<int, MenuItemSprite*> btnActScene;
	for (UInt i = 0; i < history->m_mActScene.size(); i++)
	{
		int iImgId = trData->m_mNetWarInfo[history->m_mActScene[i]]->iImgId;
		__String* strPath = __String::createWithFormat("img/heroz/icon_fbs%d.png", iImgId);
		Sprite* spActiveN = ReadSpriteNormal(strPath->getCString());
		Sprite* spActiveS = ReadSpriteNormal(strPath->getCString());
		spActiveS->setScale(fScale);
		btnActScene[i] = MenuItemSprite::create(spActiveN, spActiveS, CC_CALLBACK_1(Net_HomePage::menuBack, this));
		spActiveS->setAnchorPoint(Vec2(0.5f, 0.5f));
		spActiveS->setPosition(Vec2(spActiveS->getContentSize().width / 2, spActiveS->getContentSize().height / 2));
		btnActScene[i]->setTag(20 + i);
		btnActScene[i]->setPosition(Vec2(size.width - (8 + btnActScene[i]->getContentSize().width / 2), size.height - (68 + btnActScene[i]->getContentSize().height / 2 + i * 60)));
		m_pBtnMenu->addChild(btnActScene[i]);
	}
}

void Net_HomePage::callBackMenu()
{
	switch (m_iClickMode)
	{
	case 0: //游历天下
	{

		if (IsOpen(_DestroySixCountries, true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(DestroySixCountriesLayer::create());
			this->removeFromParent();
		}
		//	ITools::getInstance()->createGlobalPrompt(LL("sixwaropen"), false);

	}
	break;
	case 1: //演义堂
	{

		if (IsOpen(_Romance,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(RomanceLayer::create());
			this->removeFromParent();
		}
	}
	break;
	case 2: //邮件
	{
		//ChessApplication::getInstance()->AddFunctionLayer(MailLayer::create());
		if (IsOpen(_Mail,true))
		{
			auto _mail = MailLayer::create(this);
			_mail->setm_homeClass(this);
			ChessApplication::getInstance()->AddFunctionLayer(_mail);
		}
	}
	break;
	case 3: //英雄志
	{
		
		if (trData->isSystemOpen(SystemOpenIDS::E_ZZCC, true))
		{
			GetNetDateTool::getInstance()->SendGreatBattleData();
		}
	}
	break;
	case 4: //商店
	{
		if (IsOpen(_GoodsShop,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(GoodsShopLayer::create());
		}
	}
	break;
	case 5: //酒馆
	{
		if (IsOpen(_PubDrink,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(PubDrinkLayer::create());
		}
	}
	break;
	case 6: //日常副本
	{
		if (IsOpen(_DailyScene,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(DailySceneLayer::create());
		}
	}
	break;
	case 7: //巅峰对决
	{
		if (IsOpen(_BattleLadder,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(PVPSelectLayer::create());
		}
	}
	break;
	case 8: //排行榜
	{
		if (IsOpen(_WorldRank, true))
		{
			GetNetDateTool::getInstance()->SendWorldRankData();
		}
		// 		ChessApplication::getInstance()->AddFunctionLayer(CityWarInfoLayer::create());
		
	}
	break;
	case 9: //货郎
	{
		if (IsOpen(_GoodsStore,true))
		{
			ChessApplication::getInstance()->AddFunctionLayer(GoodsStoreLayer::create());
		}

	}
	break;
	default:
		break;
	}

	m_iClickMode = -1;
}

void Net_HomePage::updateMenu(bool bVisible)
{
	if (isOpen != bVisible)
	{
		int x;
		int y;
		isOpen = bVisible;

		if (isOpen)
		{
			x = -100;
			y = 100;
			add->selected();
		}
		else
		{
			x = 100;
			y = -100;
			add->unselected();
		}
		generals->runAction(MoveBy::create(0.1f, Vec2(0, y)));
		knapsack->runAction(MoveBy::create(0.1f, Vec2(0, y)));
		forging->runAction(MoveBy::create(0.1f, Vec2(0, y)));
		clan->runAction(MoveBy::create(0.1f, Vec2(0, y)));
		shoppingMall->runAction(MoveBy::create(0.1f, Vec2(x, 0)));
	}
}

void Net_HomePage::updateMailState()
{
	//m_spMailMark->setVisible(history->m_iMailNum > 0);
}

void Net_HomePage::PlayHomeBgMp3(float f)
{
	ChessPlayMp3(MUSIC_HOME);
}

bool Net_HomePage::IsOpen(int Type, bool popTips)
{
	switch (Type)
	{
	case _DestroySixCountries://游历天下
	{
		return trData->isSystemOpen(SystemOpenIDS::E_SixCountries, popTips);

	}
	break;
	case _Romance://演义堂
	{
		return trData->isSystemOpen(SystemOpenIDS::E_Romance, popTips);;

	}
	break;
	case _Mail://邮件
	{
		if (history->m_pPlayerInfo->getGuideStatus(GuideLayer::IntToGuideString(0)) == 1)
		{
			return true;
		}
	}
	break;
	case _GlobalPrompt://英雄志
	{
		return trData->isSystemOpen(SystemOpenIDS::E_ZZCC, popTips);
	}
	break;
	case _GoodsShop://商店
	{
		return trData->isSystemOpen(SystemOpenIDS::E_GoodsShop, popTips);
	}
	break;
	case _PubDrink://酒店
	{
		//if (history->ownArmyNum >= 1)
		//{
		return trData->isSystemOpen(SystemOpenIDS::E_PubDrink, popTips);
		
		//}
		//else {
		//	if (popTips)
		//	{
		//		ITools::getInstance()->createGlobalPrompt(LL("pubdrinkopen"), false);
		//		return false;
		//	}
		//}
	}
	break;
	case _DailyScene://日常副本
	{
		return trData->isSystemOpen(SystemOpenIDS::E_DailyScene, popTips);
	}
	break;
	case _BattleLadder://过关斩将
	{

		return trData->isSystemOpen(SystemOpenIDS::E_BattleLadder, popTips);
	}

	break;
	case _WorldRank://排行榜
	{
		return trData->isSystemOpen(SystemOpenIDS::E_WorldRank, popTips);
	}
	break;
	case _GoodsStore://货郎
	{

		return trData->isSystemOpen(SystemOpenIDS::E_GoodsStore, popTips);
	}
	break;
	default:
		break;
	}
	return false;
}

void Net_HomePage::editBoxReturn(ui::EditBox* editBox)
{
#ifdef ENABLE_GM
	onHttpRequest(REQ_GM);
#endif // ENABLE_GM
}
//HongDian * HongDian::staticHongDian = NULL;
bool HongDian::init()
{
	if (!Node::init())
	{
		return false;
	}
	return true;
}

HongDian * HongDian::getInstence()
{
	if (m_iHongDian == NULL)
	{
		m_iHongDian = HongDian::create();
		//m_iHongDian->retain();
	}
	else
	{
		
	}
	return m_iHongDian;
}

HongDian::~HongDian()
{
	HongDian::m_iHongDian=NULL  ;
}

//HongDian * HongDian::getInstence()
//{
//	if (HongDian::staticHongDian == NULL)
//	{
//		staticHongDian = HongDian::create();
//	}
//	return staticHongDian;
//}

void HongDian::addHongDianNode(Node * node, int TypeNum)
{
	CCASSERT(!node==NULL,"添加的红点节点不能为空");
	Sprite *m_spMark = ReadSpriteName("new_ui/img_global/icon_xh.png");
	if (TypeNum>=98) {
		m_spMark->setPosition(Vec2(node->getContentSize().width-10, node->getContentSize().height-10));
	}
	else {
		m_spMark->setPosition(Vec2(node->getContentSize().width/4 , node->getContentSize().height/4 ));
	}
	m_spMark->setVisible(false);
	//HongDianNode.insert(std::map<int, Node *>::value_type(TypeNum, m_spMark));
	if (HongDianNode[TypeNum]!=NULL)
	{

		CCLOG("重复加载数据:%d", TypeNum);
	}
	HongDianNode[TypeNum] = m_spMark;
	if (TypeNum>1000&& TypeNum<2000)
	{
		HongDianNode[TypeNum]->setTag(3021);
	}
	node->addChild(m_spMark,1000);
}

void HongDian::showHongDian(int TypeNum)
{
	HongDianNode[TypeNum]->setVisible(true);
}

void HongDian::invisibleHongDian(int TypeNum)
{
	HongDianNode[TypeNum]->setVisible(false);
}

void HongDian::autoUpdate()
{

	if (HongDianNode.find(100)== HongDianNode.end())
	{
		return;
	}

	for (auto &vk: HongDianNode)
	{
		if(vk.first < 200&& vk.first >0)
		vk.second->setVisible(false);
	}

	//if (0)
	//{
		//HongDianNode[1]->setVisible(true);
		//HongDianNode[0]->setVisible(true);
		
	//}

	HongDianNode[2]->setVisible(history->m_iMailNum > 0);//邮箱是否有提示
	if (BTActivity::getInstance()->getIsSign()>0)
	{
		HongDianNode[99]->setVisible(true);//签到提示
	}
	
	if (novisible.find(98) == novisible.end())
	{
		HongDianNode[98]->setVisible(true);//活动提示
	}
	
	
	
	
	
	 WuJiangVisible();//武将提示
	 //BeiBaoVisible();//背包提示
	 DuanZaoVisible();//锻造提示
	 GongHuiVisible();//工会提示
	 //HeChengVisible();//合成提示


	for (int i=101;i<=105;i++)
	{
		if (HongDianNode[i]->isVisible())
		{
			HongDianNode[100]->setVisible(true);//add提示
		}
	}
	
}

void HongDian::autoUpdateBaoguo()
{
	//HongDianNode[200]->setVisible(true);
	//HongDianNode[201]->setVisible(true);
	//HongDianNode[202]->setVisible(true);

}

void HongDian::autoUpdateWuJiang(int type)
{
	//for (auto &m: HongDianNode)
	//{
	//	if (m.first>1000&& m.first<2000)
	//	{
	//		m.second->setVisible(true);
	//	}
	//}
	if (HongDianNode.find(100) == HongDianNode.end())
	{
		return;
	}

	for (int i = 0; i < history->ownArmyNum; i++)
	{

		HistoryActorPtr curActor = &history->actors[history->ownArmy[i]];

		autoUpadteInfo(curActor->actor_id, type);
		
	}
}



void HongDian::autoUpadteInfo(int heroID,int type)
{
	if (HongDianNode.find(100) == HongDianNode.end())
	{
		return;
	}

	if (type == ANNIU_HONGDIAN)//添加技能按钮红点
	{

		for (int i=1;i<=7;i++)
		{
			HongDianNode[2000+i]->setVisible(false);
		}
	}

		HistoryActorPtr curActor=&history->actors[heroID];

		
		auto iskillall = curActor->GetCurSkillLvList();

		//HongDianNode[2007]->setVisible(false);
		//HongDianNode[1000 + curActor->actor_id + 1]->setVisible(false);
		
		int iActorId = curActor->actor_id + 1;

	if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
	{
		if (HongDianelem.find(1000 + curActor->actor_id + 1)!= HongDianelem.end())
		{
			HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(false);
		}
	}
	//修炼
	if (trData->net_hero_quality.find(curActor->actor_quality + 1) == trData->net_hero_quality.end())
	{
		int iCurSoulNum = history->mChipSoulDate->GetActorChipNumByActorId(iActorId);
		int iRankSoulNum = trData->net_hero_quality[curActor->actor_quality]->rank_soul;
		if (iCurSoulNum >= 10)
		{
			if (type == JINENG_HONGDIAN)
			{


			}
			else if (type == ANNIU_HONGDIAN)//添加技能按钮红点
			{

				HongDianNode[2004]->setVisible(true);
				HongDianNode[2004]->setPosition(Vec2(60,60));
			}
			else if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
			{
				if (HongDianelem.find(1000 + curActor->actor_id + 1) != HongDianelem.end())
					HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(true);
			}
			else if (type == ZHUJIEMIANN_HONGDIAN)//主界面更新武将红点
			{
				if (novisible.find(101) == novisible.end())
				{
					HongDianNode[101]->setVisible(true);//武将提示
				}
				
			}
		}
	}

	//时装
	int fashion_num = 0;
	int fashion_warnum = 0;
	bool fashion_flag = false;
	for (int i = 0; i < history->goods->size(); i++)
	{
		HistoryGoodsPtr ptrHistoryGoods = history->goods->elementAt(i);
		int iGoodsType = ptrHistoryGoods->GetGoodsType();

		if ((iGoodsType == GOOD_TYPE_FASHING ) && trData->m_mGoodsItem[ptrHistoryGoods->good_id]->mHid.count(curActor->actor_id + 1))
		{
			fashion_num++;
		}
		else if ((iGoodsType == GOOD_TYPE_WAP_FASHING) && trData->m_mGoodsItem[ptrHistoryGoods->good_id]->mHid.count(curActor->actor_id + 1))
		{
			fashion_warnum++;
		}
	}
	if (!curActor->fashion.isNormalGood()&& fashion_num>0)
	{
		fashion_flag = true;
	}
	if (!curActor->wap_fashion.isNormalGood() && fashion_warnum>0)
	{
		fashion_flag = true;
	}
	if (fashion_flag )
	{
		if (type == ANNIU_HONGDIAN)//添加技能按钮红点
		{

			HongDianNode[2005]->setVisible(true);
		}
		else if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
		{

			HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(true);
		}
		else if (type == ZHUJIEMIANN_HONGDIAN)//主界面更新武将红点
		{

			if (novisible.find(101) == novisible.end())
			{
				HongDianNode[101]->setVisible(true);//武将提示
			}
		}
	}
	//进阶
		if (trData->net_hero_quality.find(curActor->actor_quality + 1) != trData->net_hero_quality.end())
		{
			int iCurSoulNum = history->mChipSoulDate->GetActorChipNumByActorId(iActorId);
			int iRankSoulNum = trData->net_hero_quality[curActor->actor_quality]->rank_soul;
			if (iCurSoulNum >= iRankSoulNum)
			{
				if (type == JINENG_HONGDIAN)
				{


				}
				else if (type == ANNIU_HONGDIAN)//添加技能按钮红点
				{

					HongDianNode[2006]->setVisible(true);
				}
				else if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
				{

					HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(true);
				}
				else if (type == ZHUJIEMIANN_HONGDIAN)//主界面更新武将红点
				{

					if (novisible.find(101) == novisible.end())
					{
						HongDianNode[101]->setVisible(true);//武将提示
					}
				}
			}
		}
		//武器，防具，辅助装备
		
		for (int mEtype = 1; mEtype <= 2; mEtype++)
		{
			bool EmFlag = false;
			if (mEtype == 3)
			{
				continue;
			}
			int own_index = 0;
			if (mEtype == GOOD_TYPE_WEAPON&&curActor->weapon.isNormalGood())
			{
				own_index = 1;
				
			}
			else if (mEtype == GOOD_TYPE_COAT&&curActor->coat.isNormalGood())
			{
				own_index = 1;
				
			}
			else if (mEtype == GOOD_TYPE_ASSIST&&curActor->assist_good.isNormalGood())
			{
				own_index = 1;
				
			}

			for (int i = 0; i < history->goods->size(); i++)
			{
				HistoryGoodsPtr ptrGoods = history->goods->elementAt(i);
				if (ptrGoods->GetGoodsType() != mEtype || trData->occupation_good_info[curActor->occupation_id][ptrGoods->getItId()] == 0 || ptrGoods->isFashion() || !ptrGoods->EnableEquip(curActor->actor_id + 1))
				{
					continue;
				}
				if (1)//辅助界面
				{
					if (own_index == 0)
					{
						if (type == ANNIU_HONGDIAN)//添加技能按钮红点
						{
							if (mEtype == GOOD_TYPE_ASSIST)
							{
								if (HongDianNode.find(2003) != HongDianNode.end())
									HongDianNode[2003]->setVisible(true);
							}
							else if (mEtype == GOOD_TYPE_COAT)
							{
								if (HongDianNode.find(2002) != HongDianNode.end())
									HongDianNode[2002]->setVisible(true);
							}
							else if (mEtype == GOOD_TYPE_WEAPON)
							{
								if (HongDianNode.find(2001) != HongDianNode.end())
									HongDianNode[2001]->setVisible(true);
							}
						}
						else if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
						{
							if (HongDianelem.find(1000 + curActor->actor_id + 1) != HongDianelem.end())
								HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(true);
						}
						else if (type == ZHUJIEMIANN_HONGDIAN)//主界面更新武将红点
						{
							if (novisible.find(101) == novisible.end())
							{
								HongDianNode[101]->setVisible(true);//武将提示
							}
						}
					}
				}
			}
		}
		
		//技能
		for (auto iakillNode: iskillall)
		{
			//HongDianNode[10000 + iakillNode.first]->setVisible(false);
			int iSkillLevel = iakillNode.second[0] ^ iakillNode.second[1];
			if (type == JINENG_HONGDIAN)
			{
				if (HongDianNode.find(10000 + iakillNode.first) != HongDianNode.end())
					HongDianNode[10000 + iakillNode.first]->setVisible(false);//添加技能红点
			}
			if (iSkillLevel < curActor->GetActorLv() - trData->net_skill_upgrade[iakillNode.first]->up_limit)
			{
				if (type== JINENG_HONGDIAN)
				{
					if (HongDianNode.find(10000 + iakillNode.first) != HongDianNode.end())
					HongDianNode[10000 + iakillNode.first]->setVisible(true);//添加技能红点
				}
				else if (type == ANNIU_HONGDIAN)//添加技能按钮红点
				{
					if (HongDianNode.find(2007) != HongDianNode.end())
					HongDianNode[2007]->setVisible(true);
				}
				else if (type == WUJIANG_JIEMIAN)//武将列表更新武将红点
				{
					if (HongDianelem.find(1000 + curActor->actor_id + 1) != HongDianelem.end())
						HongDianelem[1000 + curActor->actor_id + 1]->getBG()->getChildByTag(3021)->setVisible(true);
				}
				else if (type == ZHUJIEMIANN_HONGDIAN)//主界面更新武将红点
				{
					if (novisible.find(101) == novisible.end())
					{
						HongDianNode[101]->setVisible(true);//武将提示
					}
				}
				
			}
		}

	
}

void HongDian::autoUpdateEquep(int type)
{
	if (type==0)
	{
		if (HongDianNode.find(2100) == HongDianNode.end())
		{
			return;
		}
	}
	if (HongDianNode.find(100) == HongDianNode.end())
	{
		return;
	}

	bool qianghua_flage = false;

	auto m_mTreasure = history->GetAllTreasure(MODEL_STRENG);
	for (auto MTreasure: m_mTreasure)
	{
		HistoryGoods* ptrGoods = MTreasure.second;
		if (ptrGoods->equipment_actorId > -1 &&((history->stone > trData->strengthen_consume[ptrGoods->iQuality].gem_num)))
		{
			qianghua_flage = true;
		}
	}
	
	bool fumo_flag = false;

	for (int i=0;i<3;i++)
	{
		if (history->getGoodsNum(2700+i) > 0)
		{
			fumo_flag = true;
		}
	}
	for (int i = 0; i<4; i++)
	{
		if (history->getGoodsNum(20001+ i) > 0)
		{
			fumo_flag = true;
		}
	}
	if (type == 0)
	{
		if (qianghua_flage)
		{
			HongDianNode[2100]->setVisible(true);//强化提示
		}
		else
		{
			HongDianNode[2100]->setVisible(false);//强化提示
		}
		
		if (fumo_flag)
		{
			HongDianNode[2102]->setVisible(true);//附魔提示
		}
		else
		{
			HongDianNode[2102]->setVisible(false);//附魔提示
		}
	}
	else if (type == 1)
	{
		if ((fumo_flag || qianghua_flage) && (novisible.find(103) == novisible.end()))
		{
			HongDianNode[103]->setVisible(true);//锻造提示
		}
		else {
			HongDianNode[103]->setVisible(false);//锻造提示
		}
	}
}

void HongDian::autoUpdateHeCheng(int type)
{
	
		//合成
		for (auto _goods : trData->m_mMergeData)
		{
			for (auto vk : _goods.second->mMaterial)
			{
				if (history->getGoodsNum(vk.first)>= vk.second)
				{
					HongDianNode[105]->setVisible(true);//合成提示
				}
			}
		}
	
}

void HongDian::WuJiangVisible()
{
	autoUpdateWuJiang(ZHUJIEMIANN_HONGDIAN);
}

void HongDian::BeiBaoVisible()
{
	HongDianNode[102]->setVisible(true);//背包提示
}

void HongDian::DuanZaoVisible()
{
	autoUpdateEquep(1);
}

void HongDian::GongHuiVisible()
{
	if (HongDianNodeisVisibol.find(104) != HongDianNodeisVisibol.end())
	{
		if (HongDianNodeisVisibol[104]==1)
		{
			HongDianNode[104]->setVisible(true);//工会提示
		}
	}
	
}

void HongDian::HeChengVisible()
{
	autoUpdateHeCheng();
	
}

void HongDianNode::onExit()
{
	Node::onExit();
	this->setVisible(false);
}
