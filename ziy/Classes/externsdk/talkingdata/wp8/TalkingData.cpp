#include "TalkingData.h"
#include "TRGameDelegate.h"

#include <map>
#include <string>
#include "cocos2d.h"
#include "Common.h"
//#include <pthread.h>
#include "StatConfig.h"

using namespace PhoneDirect3DXamlAppComponent;

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
	if (map == NULL)
	{
		Platform::Array<Platform::String^>^ mapKey = ref new Platform::Array<Platform::String^>(0);
		Platform::Array<Platform::String^>^ mapValue = ref new Platform::Array<Platform::String^>(0);
		TRGameDelegate::GlobalCallback->tdgaOnEvent(wpString2Platform(eventId), mapKey, mapValue);
	}
	else
	{
		Platform::Array<Platform::String^>^ mapKey = ref new Platform::Array<Platform::String^>(map->size());
		Platform::Array<Platform::String^>^ mapValue = ref new Platform::Array<Platform::String^>(map->size());
		EventParamMap::iterator iter = map->begin();
		unsigned int i = 0;
		for (i = 0; i < map->size() && iter != map->end(); i++, iter++)
		{
			mapKey[i] = wpString2Platform(iter->first.c_str());
			mapValue[i] = wpString2Platform(iter->second.c_str());
		}
		TRGameDelegate::GlobalCallback->tdgaOnEvent(wpString2Platform(eventId), mapKey, mapValue);
	}
}

void TDCCTalkingDataGA::setLatitude(double latitude, double longitude)
{
	LOGD("setLatitude,latitude=%f,longitude=%f", latitude, longitude);
}


const char* TDCCTalkingDataGA::getDeviceId()
{
	//static char ret[] = "01234567890";
	static char ret[200] = { 0 };
	DString str = wpPlatform2String(TRGameDelegate::GlobalCallback->getTalkingDataDeviceId());
	strcpy(ret, str->getNativeCharPtr());
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
	TRGameDelegate::GlobalCallback->tdgaSetAccount(wpString2Platform(accountId));
	static TDCCAccount* local = new TDCCAccount();
	return local;
}

void TDCCAccount::setAccountName(const char* accountName)
{
	LOGD("setAccountName,accountName=%s", accountName);
	TRGameDelegate::GlobalCallback->tdgaSetAccountName(wpString2Platform(accountName));
}

void TDCCAccount::setAccountType(TDCCAccountType accountType)
{
	LOGD("setAccountType,accountType=%d", accountType);
	TRGameDelegate::GlobalCallback->tdgaSetAccountType((int)accountType);
}

void TDCCAccount::setLevel(int level)
{
	LOGD("setLevel,level=%d", level);
	TRGameDelegate::GlobalCallback->tdgaSetLevel(level);
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
	TRGameDelegate::GlobalCallback->tdgaOnMissionBegin(wpString2Platform(missionId));
}

void TDCCMission::onCompleted(const char* missionId)
{
	LOGD("TDCCMission::onCompleted=%s", missionId);
	TRGameDelegate::GlobalCallback->tdgaOnMissionCompleted(wpString2Platform(missionId));
}

void TDCCMission::onFailed(const char* missionId, const char* failedCause)
{
	LOGD("TDCCMission::onFailed=%s,cause=%s", missionId, failedCause);
	TRGameDelegate::GlobalCallback->tdgaOnMissionFailed(wpString2Platform(missionId), wpString2Platform(failedCause));
}


///////////////////////////////////////////////////////////////////////////

void TDCCVirtualCurrency::onChargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
{
	LOGD("TDCCVirtualCurrency::onChargeRequest,orderId=%s,iapId=%s,currencyAmount=%f,currencyType=%s,virtualCurrencyAmount=%f,paymentType=%s",
		orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);
	TRGameDelegate::GlobalCallback->tdgaOnChargeRequest(wpString2Platform(orderId), wpString2Platform(iapId),
		currencyAmount, wpString2Platform(currencyType), virtualCurrencyAmount, wpString2Platform(paymentType));
}

void TDCCVirtualCurrency::onChargeSuccess(const char* orderId)
{
	LOGD("TDCCVirtualCurrency::onChargeSuccess,orderId=%s", orderId);
	TRGameDelegate::GlobalCallback->tdgaOnChargeSuccess(wpString2Platform(orderId));
}



///////////////////////////////////////////////////////////////////////////

void TDCCItem::onPurchase(const char* item, int number, double price)
{
	LOGD("TDCCItem::onPurchase,item=%s,number=%d,price=%f", item, number, price);
	TRGameDelegate::GlobalCallback->tdgaOnPurchase(wpString2Platform(item), number, price);
}

void TDCCItem::onUse(const char* item, int number)
{
	LOGD("TDCCItem::onUse,item=%s,number=%d", item, number);
	TRGameDelegate::GlobalCallback->tdgaOnUse(wpString2Platform(item), number);
}


///////////////////////////////////////////////////////////////////////////

















