#include "cocos2d.h"
#include "Common.h"
#include <pthread.h>
#include "StatConfig.h"

#define LOGD CCLOG


static pthread_t pidAlipay;

static void* th_SDKpay(void *r)
{
//GHB	Sleep(200);
	DString* tradeNo = (DString*)r;
	StatConfig::onPayResult(StatConfig::PAY_TYPE_QDSDK, (*tradeNo)->getChar(), 1, "succeed");
	SAFE_DELETE(tradeNo);

	return NULL;
}

int payStartSDKpay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
	LOGD("payStartSDKpay,trGameId=%s,outTradeNo=%s,subject=%s,body=%s,totalFee=%s", trGameId, outTradeNo, subject, body, totalFee);

	DString* tradeNo = new DString(outTradeNo);
	//pthread_create(&pidAlipay, NULL, th_SDKpay, tradeNo);

	return 1;
}