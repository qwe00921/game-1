#include "cocos2d.h"
#include "Common.h"

#include "StatConfig.h"

#define LOGD CCLOG


static void* th_SDKpay(void *r)
{
	DString* tradeNo = (DString*)r;
	StatConfig::onPayResult(StatConfig::PAY_TYPE_QDSDK, (*tradeNo)->getChar(), 1, "succeed");
	SAFE_DELETE(tradeNo);

	return NULL;
}

int payStartSDKpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
	LOGD("payStartSDKpay,trGameId=%s,outTradeNo=%s,subject=%s,body=%s,totalFee=%s", trGameId, outTradeNo, subject, body, totalFee);

	DString* tradeNo = new DString(outTradeNo);

	return 1;
}