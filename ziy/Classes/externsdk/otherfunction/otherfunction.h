#ifndef _other_function_h_
#define _other_function_h_

#include "DString.h"

enum StatDeviceParamType
{
	StatParamAccount = 0,
	StatParamPlatform = 1,
	StatParamPackageName = 2,
	StatParamVersion = 3,
	StatParamMMChannelId = 4,
	StatParamFeatureSMS = 5,
    StatParamPatchVersion = 7
};

DString statGetDeviceParams(StatDeviceParamType type);
int payStartAlipay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee);
int payStartwxpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee);
int payStartSDKpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee);
void cppBridgeCall(const char* strJson);
void sendWXAuthRequest(); //微信授权登陆请求
void sendQQAuthRequest(); //QQ授权登陆请求
void sendSDKAuthRequest(); //SDK授权登陆请求
DString getNotifyUrl(); //获取第三方支付回调地址
void showWarning(const char* strWarn); //显示支付警告
void setPlatformLoginInfo(const char* strJson);//平台登录成功后的用户数据
inline bool statHasFeatureSMS()
{
	DString result = statGetDeviceParams(StatParamFeatureSMS);
	if (result.IsNullOrEmpty())
	{
		return false;
	}

	if (result->equals("1"))
	{
		return true;
	}

	return false;
}

void initAdGoogle();
void showAdMobInterstitial(void (*callback)(bool result));

#endif
