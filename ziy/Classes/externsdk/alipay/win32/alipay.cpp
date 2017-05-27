#include "cocos2d.h"
#include "Common.h"
//#include <pthread.h>
#include "StatConfig.h"

#define LOGD CCLOG


//static pthread_t pidAlipay;

static void* th_alipay(void *r)
{
	Sleep(200);
	DString* tradeNo = (DString*)r;
	StatConfig::onPayResult(StatConfig::PAY_TYPE_ALIPAY, (*tradeNo)->getChar(), 1, "succeed");
	SAFE_DELETE(tradeNo);

	return NULL;
}

int payStartAlipay(const char* trGameId, const char* outTradeNo, const char* subject, const char* body, const char* totalFee)
{
	LOGD("payStartAlipay,trGameId=%s,outTradeNo=%s,subject=%s,body=%s,totalFee=%s", trGameId, outTradeNo, subject, body, totalFee);

	DString* tradeNo = new DString(outTradeNo);
	//pthread_create(&pidAlipay, NULL, th_alipay, tradeNo);

	return 1;
}
















