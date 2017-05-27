#include "otherfunction.h"
#include "cocos2d.h"
#include "Common.h"
#include "Data.h"
#include "LoginLayer.h"


/**
 * 边玩边下功能
 */
void gotoOtaPlayAndDownload()
{
	CCLOG("gotoOtaPlayAndDownload");
}

DString statGetDeviceParams(StatDeviceParamType type)
{
    switch (type)
    {
        case StatParamAccount:
        {
            const int bufferSize = 1024;
            char* buffer = new char[1024];
            snprintf(buffer, bufferSize, "package_name=%s&platform=%s", "com.bantu.trgame", "win32");
            DString result = buffer;
            SAFE_DELETE_ARRAY(buffer);
            return result;
        }
        case StatParamPackageName:
        {
            //return "com.bantu.trgame";
            NSString *identifier = [[NSBundle mainBundle] bundleIdentifier];
            DString ret = [identifier UTF8String];
            return ret;
        }
        case StatParamPlatform:
            return "ios";
        case StatParamVersion:
        {
            //CFBundleShortVersionString
            //kCFBundleVersionKey
            NSString *version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
            DString ret = [version UTF8String];
            return ret;
        }
        case StatParamPatchVersion:
        {
            return "";
        }
        case StatParamMMChannelId:
        {
            return "";
        }
        default:
        {
            return "";
        }
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
