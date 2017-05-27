#include "otherfunction.h"
#include "cocos2d.h"
#include "Common.h"
#include "Data.h"
#include "LoginLayer.h"
#include "LogoLayer.h"

DString statGetDeviceParams(StatDeviceParamType type)
{
	switch (type)
	{
	case StatParamAccount:	
	{
		DString result = DString::format("package_name=%s&platform=%s", 
			statGetDeviceParams(StatParamPackageName)->getChar(),
			statGetDeviceParams(StatParamPlatform)->getChar());
		return result;
	}
	case StatParamPackageName:
		return win32Const->packageName;
	case StatParamPlatform:
		return win32Const->platform;
	case StatParamVersion:
		return win32Const->versionName;
	case StatParamMMChannelId:
		return "MM0000";
	case StatParamFeatureSMS:
		return "0";
	default:
		return "";
	}

}
void showAdWebView(){
}

void sendWXAuthRequest()
{
	LoginLayer::onHttpReqWXQQLogin("7DF6DC5B164B6D53C86BD91EC568A555", "weixin");
}

void sendQQAuthRequest()
{
	LoginLayer::onHttpReqWXQQLogin("7DF6DC5B164B6D53C86BD91EC568A666", "QQ");
}

void setPlatformLoginInfo(const char* strJson) {
	LogoLayer::setLoginInfo(strJson);
}

void cppBridgeCall(const char* strJson) {

}