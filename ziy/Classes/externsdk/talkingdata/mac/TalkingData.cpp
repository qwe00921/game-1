#include "TalkingData.h"

#include <map>
#include <string>
#include "cocos2d.h"
#include "Common.h"
#include <pthread.h>
#include "StatConfig.h"

#define LOGD CCLOG

typedef std::pair< std::string, std::string >   EventParamPair;
typedef std::map< std::string, std::string >    EventParamMap;

void TDCCTalkingDataGA::onStart(const char* appId, const char* channelId)
{
	LOGD("onStart,appId=%s,channelId=%s", appId, channelId);
}

void TDCCTalkingDataGA::onEvent(const char* eventId, EventParamMap* map)
{
	LOGD("onEvent,eventId=%s", eventId);
}

void TDCCTalkingDataGA::setLatitude(double latitude, double longitude)
{
	LOGD("setLatitude,latitude=%f,longitude=%f", latitude, longitude);
}


const char* TDCCTalkingDataGA::getDeviceId()
{
	static char ret[] = "01234567890";
	LOGD("getDeviceId=%s", ret);
	return ret;
}

void TDCCTalkingDataGA::onKill()
{
	LOGD("onKill");
}

void TDCCTalkingDataGA::setVerboseLogDisabled()
{
	LOGD("setVerboseLogDisabled");
}

void TDCCTalkingDataGA::setVerboseLogEnabled()
{
	LOGD("setVerboseLogEnabled");
}


///////////////////////////////////////////////////////////////

TDCCAccount::TDCCAccount()
{

}

TDCCAccount::~TDCCAccount()
{

}

TDCCAccount* TDCCAccount::setAccount(const char* accountId)
{
	LOGD("setAccount,accountId=%s", accountId);
	static TDCCAccount* local = new TDCCAccount();
	return local;
}

void TDCCAccount::setAccountName(const char* accountName)
{
	LOGD("setAccountName,accountName=%s", accountName);
}

void TDCCAccount::setAccountType(TDCCAccountType accountType)
{
	LOGD("setAccountType,accountType=%d", accountType);
}

void TDCCAccount::setLevel(int level)
{
	LOGD("setLevel,level=%d", level);
}

void TDCCAccount::setGender(TDCCGender gender)
{
	LOGD("setGender,gender=%d", gender);
}

void TDCCAccount::setAge(int age)
{
	LOGD("setAge,age=%d", age);
}

void TDCCAccount::setGameServer(const char* gameServer)
{
	LOGD("setGameServer,gameServer=%s", gameServer);
}

////////////////////////////////////////////////////////////

void TDCCMission::onBegin(const char* missionId)
{
	LOGD("TDCCMission::onBegin=%s", missionId);
}

void TDCCMission::onCompleted(const char* missionId)
{
	LOGD("TDCCMission::onCompleted=%s", missionId);
}

void TDCCMission::onFailed(const char* missionId, const char* failedCause)
{
	LOGD("TDCCMission::onFailed=%s,cause=%s", missionId, failedCause);
}


///////////////////////////////////////////////////////////////////////////

void TDCCVirtualCurrency::onChargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
{
	LOGD("TDCCVirtualCurrency::onChargeRequest,orderId=%s,iapId=%s,currencyAmount=%f,currencyType=%s,virtualCurrencyAmount=%f,paymentType=%s",
		orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);
}

void TDCCVirtualCurrency::onChargeSuccess(const char* orderId)
{
	LOGD("TDCCVirtualCurrency::onChargeSuccess,orderId=%s", orderId);
}



///////////////////////////////////////////////////////////////////////////

void TDCCItem::onPurchase(const char* item, int number, double price)
{
	LOGD("TDCCItem::onPurchase,item=%s,number=%d,price=%f", item, number, price);
}

void TDCCItem::onUse(const char* item, int number)
{
	LOGD("TDCCItem::onUse,item=%s,number=%d", item, number);
}


///////////////////////////////////////////////////////////////////////////

















