//
//  WebViewLayer.cpp
//  TRGame
//
//  Created by Jimmy on 16/09/08.
//
//

#include "WebViewLayer.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "ui/UIWebView.h"
using namespace cocos2d::experimental::ui;
#endif

WebViewLayer::WebViewLayer()
{

}

WebViewLayer::~WebViewLayer()
{

}

WebViewLayer* WebViewLayer::create(string strUrl)
{
	WebViewLayer* pRet = new WebViewLayer();
	if(pRet&&pRet->init(strUrl))
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

bool WebViewLayer::init(string strUrl)
{
	if(!Layer::init())
	{
		return false;
	}

	m_strUrl = strUrl;
	initControl();
	return true;
}

void WebViewLayer::initControl()
{
	DSize size = ChessViewTool::getInstance()->getFullSize();
	Vec2 point = Vec2(size.width / 2, size.height / 2);
    
    //遮罩
    LayerColor* layBlack = LayerColor::create(Color4B(0, 0, 0, 100));
    layBlack->setPosition(Vec2(0, 0));
    this->addChild(layBlack);

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	WebView *webView = WebView::create();
	webView->setPosition(point);
	webView->setContentSize(Size(size.width * 4 / 5, size.height));
	webView->loadURL(m_strUrl);
	webView->setScalesPageToFit(true);
	webView->setOnShouldStartLoading([](WebView *sender, const std::string &url)
	{
		return true;
	});
	webView->setOnDidFinishLoading([](WebView *sender, const std::string &url)
	{

	});
	webView->setOnDidFailLoading([](WebView *sender, const std::string &url)
	{

	});
	this->addChild(webView);
#else
	//提示
	auto labNotice = LabelSystemFont("Win32 don't support, HAHA", 25);
	labNotice->setPosition(point);
	labNotice->setColor(Color3B::RED);
	labNotice->enableOutline(Color4B(102, 50, 0, 255), 1);
	this->addChild(labNotice);
#endif

	//按钮菜单
	Menu* ptrMenu = Menu::create();
	ptrMenu->setPosition(Vec2(0, 0));
	this->addChild(ptrMenu);

	//关闭
	Sprite* spCloseN = ReadSpriteName("new_ui/img_global/close.png");
	Sprite* spCloseS = ReadSpriteName("new_ui/img_global/close_press.png");
	MenuItemSprite* btnClose = MenuItemSprite::create(spCloseN, spCloseS, CC_CALLBACK_1(WebViewLayer::callBackClose, this));
	btnClose->setPosition(Vec2(size.width - spCloseN->getContentSize().width / 2, size.height - spCloseN->getContentSize().height / 2));
	ptrMenu->addChild(btnClose);
}

void WebViewLayer::callBackClose(Ref *sender)
{
	playSound(SOUND_LEFT_MOUSE_CLICK);
	this->removeFromParent();
}
