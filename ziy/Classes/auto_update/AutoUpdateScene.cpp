#include "AutoUpdateScene.hpp"
#include "ui/UILayout.h"
#include "LogoLayer.h"
#include "PopLayer.h"

USING_NS_CC;
using namespace ui;


bool AutoUpdateScene::init()
{
	if(!Layer::init())
	{
		return false;
	}

	loadingBarChangeDeltaTime = 0.3;
	loadingPercent = oldLoadingPercent = currentLoadingPercent = 0;


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 point = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	//背景
	Sprite* spLayerBG = ReadSpriteName("new_ui/main/logo_bg1.jpg");
	spLayerBG->setPosition(point);
	this->addChild(spLayerBG);

	//进度
	m_labPercent = LabelSystemFont("", 16);
	m_labPercent->setPosition(point + Vec2(0, -120));
	m_labPercent->enableOutline(Color4B(102, 50, 0, 255), 1);
	m_labPercent->setVisible(false);
	this->addChild(m_labPercent);

	//进度条
	m_spBarBG = ReadSpriteName("new_ui/main/hot_gx1.png");
	m_spBarBG->setPosition(point + Vec2(0, -140));
	m_spBarBG->setVisible(false);
	this->addChild(m_spBarBG);

	m_spLoading = ReadSpriteName("new_ui/main/hot_gx2.png");
	m_spLoading->setAnchorPoint(Vec2(0.0f, 0.5f));
	m_spLoading->setPosition(Vec2(0, m_spBarBG->getContentSize().height / 2));
	m_spBarBG->addChild(m_spLoading);

	worker = AutoUpdateWorker::create();
	worker->setDelegator(this);
	worker->retain();

	return true;
}

void AutoUpdateScene::onEnter()
{
	Node::onEnter();
	scheduleUpdate();
	worker->checkUpdate();
}

void AutoUpdateScene::onExit()
{
	Node::onExit();
	history->m_iPatchVersion = UserDefault::getInstance()->getIntegerForKey("patch_version");
	trData->ChessInitData();

	ChessApplication::getInstance()->AddFunctionLayer(LogoLayer::create());
}

void AutoUpdateScene::update(float delta)
{
	Node::update(delta);
	double deltaTime = utils::gettime() - lastLoadingBarChangeTime;
	if(deltaTime >= loadingBarChangeDeltaTime)
		currentLoadingPercent = loadingPercent;
	else
		currentLoadingPercent = oldLoadingPercent + (loadingPercent - oldLoadingPercent) / loadingBarChangeDeltaTime * deltaTime;
	updatePrecent();
}

void AutoUpdateScene::callBackConfirm()
{
	loadingBarChangeDeltaTime = 0.3;
	loadingPercent = oldLoadingPercent = currentLoadingPercent = 0;
	updatePrecent();
	worker->checkUpdate();
}

void AutoUpdateScene::setLodingPercent(float newValue)
{
	oldLoadingPercent = currentLoadingPercent;
	loadingPercent = newValue;
	lastLoadingBarChangeTime = utils::gettime();
}

void AutoUpdateScene::checkUpdateCB(int size)
{
	if(size <= 0) //check error
	{ 
		this->removeFromParent();
	}
	else // need update size bytes
	{
		m_fTotalSize = getTotalSize((float)size);
		m_labPercent->setVisible(true);
		m_spBarBG->setVisible(true);
		worker->downloadUpdate();
	}
}

void AutoUpdateScene::updateProgressingCB(float progress)
{
	setLodingPercent(progress * 100);
}

void AutoUpdateScene::updateResultCB(bool result)
{
	CCLOG("GHB UPDATE RES %d\n", result);
	if (result)
	{
		this->removeFromParent();
	}
	else
	{
		ChessApplication::getInstance()->AddFunctionLayer(PopLayer::create(POP_UPDATE_FAIL, this));
	}
}

float AutoUpdateScene::getTotalSize(float fSize)
{
	const int MB = 1024 * 1024; //定义MB的计算常量
	return fSize / MB;
}

void AutoUpdateScene::updatePrecent()
{
	string strPrecent = StringUtils::format("%s(%.2fM/%.2fM)", LL("updateloading"), m_fTotalSize * currentLoadingPercent / 100, m_fTotalSize);
	m_labPercent->setString(strPrecent);
	m_spLoading->setScaleX(currentLoadingPercent / 100);
}