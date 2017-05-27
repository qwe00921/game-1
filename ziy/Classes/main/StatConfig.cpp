#include "StatConfig.h"
#include "TalkingData.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "jsons.h"
#include "Common.h"
#include "PasswordLib.h"
#include "main.h"
#include "TrPayInfo.h"
#include "CocoMd5.h"
#include "DRunnable.h"
#include "CocoMd5.h"
#include "LoginLayer.h"
#include "PaymentTool.h"
#include "UserDate.h"
#include "PopLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC_EXT;

#define LOGD CCLOG

//登陆相关///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char HTTP_GET_WX_ACCESS[] = "https://api.weixin.qq.com/sns/oauth2/access_token";
//登陆相关///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DResponseProcess : public DRunnable
{
public:
	enum ResponseType
	{
		TYPE_GET_WX_ACCESS_FINISHED,
	};
public:
	DResponseProcess(ResponseType type, DString text) : DRunnable(true)
	{
		mText = text;
		mType = type;
		mFileName = "";
	}
	DResponseProcess(ResponseType type, DString text, DString fileName) : DRunnable(true)
	{
		mText = text;
		mType = type;
		mFileName = fileName;
	}
	virtual void run()
	{
		switch (mType)
		{
		case TYPE_GET_WX_ACCESS_FINISHED:
			processGetWXAccess();
			break;
		default:
			break;
		}
	}
private:
	void processGetWXAccess()
	{
		Json::Reader reader;
		Json::Value root;
		if(reader.parse(mText->getChar(), root, false))
		{
			if (root["openid"].isString())
			{
				LoginLayer::onHttpReqWXQQLogin(root["openid"].asString(), "weixin");
				return;
			}
		}
		LoginLayer::showLoginWarn(LL("denglushibaiqingchongshi")); // 提示登陆失败
	}

private:
	DString mText;
	ResponseType mType;
	DString mFileName;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StatConfig::StatConfig()
{
	mLocalAccountId = DString("LC") + TDCCTalkingDataGA::getDeviceId();
}

StatConfig::~StatConfig()
{
}

const char* StatConfig::getLocalAccountId()
{
	return mLocalAccountId->getChar();
}

bool StatConfig::readResponseString(HttpResponse* response, DString& outStr)
{
	LOGD("readResponseString");
	if (response == NULL)
	{
		LOGD("response = null");
		return false;
	}
	const int statusCode = response->getResponseCode();
	LOGD("Http status code:%d", statusCode);
	if (!response->isSucceed())
	{
		LOGD("response failed,error buffer:%s", response->getErrorBuffer());
		return false;
	}
	std::vector<char> *buffer = response->getResponseData();
	if (buffer->size() >= 100 * 1000)
	{
		LOGD("response body too long=%d", buffer->size());
		return false;
	}
	char* dataBuffer = new char[buffer->size() + 1];
	memset(dataBuffer, 0, buffer->size() + 1);
	for (unsigned int i = 0; i < buffer->size(); i++)
	{
		dataBuffer[i] = (*buffer)[i];
	}
	outStr = dataBuffer;
	SAFE_DELETE_ARRAY(dataBuffer);
	return true;
}

void StatConfig::onUserLevelChanged(int level)
{
	TDCCAccount* account = TDCCAccount::setAccount(getLocalAccountId());
	account->setLevel(level);
}

DString StatConfig::getGameAccountInfo()
{
// 	DString result = DString::format("account_id=%s&package_name=%s&platform=%s&channel_id=%s", getAccountName(), 
// 		statGetDeviceParams(StatParamPackageName)->getNativeCharPtr(), statGetDeviceParams(StatParamPlatform)->getNativeCharPtr(), 
// 		funConst->TalkingDataChannelId->getNativeCharPtr());
	DString result = DString::format("account_id=%s&package_name=%s&platform=%s&channel_id=%s&version_name=%s", "TR100001",
		statGetDeviceParams(StatParamPackageName)->getChar(), statGetDeviceParams(StatParamPlatform)->getChar(),
		funConst->getChannelId()->getChar(), statGetDeviceParams(StatParamVersion)->getChar());
	return result;
}

void StatConfig::onBeginMission(const char* missionId)
{
	TDCCMission::onBegin(missionId);
}

void StatConfig::onCompletedMission(const char* missionId)
{
	TDCCMission::onCompleted(missionId);
}

void StatConfig::onFailedMission(const char* missionId, const char* cause)
{
	TDCCMission::onFailed(missionId, cause);
}

void StatConfig::onEvent(const char* eventId)
{
	TDCCTalkingDataGA::onEvent(eventId);
}

void StatConfig::onEvent(const char* eventId, std::map< std::string, std::string >* map)
{
	TDCCTalkingDataGA::onEvent(eventId, map);
}

void StatConfig::onPurchaseBuy(int buyId, int num)
{
	DString item = DString::format("buyid%d", buyId);
	TDCCItem::onPurchase(item->getChar(), num, 0.0);
}

void StatConfig::onUseBuy(int buyId, int num)
{
	DString item = DString::format("buyid%d", buyId);
	TDCCItem::onUse(item->getChar(), num);
}

int StatConfig::startIOSAppStorepay(const char* tradeNo, const char* chTitle, const char* productID, int totalFee)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	extern bool iosBuyItem(const char* tradeNo, const char* productID);
	bool isSuccess = iosBuyItem(tradeNo, productID);
	if (isSuccess)
	{
		UserDate::getInstance()->addTradeNo(tradeNo, chTitle);
		TDCCVirtualCurrency::onChargeRequest(tradeNo, productID, totalFee / 100.0, "CNY", 0.0, "iospay");
		return 1;
	}
	else
	{
		PopLayer* ptrPop = PopLayer::create(POP_NOTICE, nullptr);
		ptrPop->setTitle(LL("trypayafterreload"));
		Director::getInstance()->getRunningScene()->addChild(ptrPop);
		buyItemResultMain(tradeNo, "", "", false, false);
		return 0;
	}
#else
	return 1;
#endif
}

int StatConfig::showAppStore(const char* appId)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	extern void iosShowAppStore(const char* appId);
	iosShowAppStore(appId);
	return 1;
#else
	return 1;
#endif
}

int StatConfig::submitExtendData(const char* strJson) {
	if (modConst->mIsTestService) {
		cppBridgeCall(strJson);
		return 0;
	}
	Json::Reader reader;
	Json::Value infoJson;
	reader.parse(strJson, infoJson);
	if (!infoJson.isNull()) {
		TDCCAccount* account = TDCCAccount::setAccount(infoJson["markCode"].asCString());
		account->setAccountName(infoJson["accountId"].asCString());
		if (infoJson["level"].asInt() > 0) {
			account->setLevel(infoJson["level"].asInt());
		}
	}

	cppBridgeCall(strJson);

	return 1;
}



int StatConfig::startAlipay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee)
{
	DString strFee = DString::format("%d.%02d", totalFee / 100, totalFee % 100);

	StatConfig* statConfig = Chess::getInstance()->getStatConfig();

	const char* accountId = "TR100001";

	DString appId = funConst->TalkingDataAppId;
	DString channelId = funConst->getChannelId();

	DString body = DString::format("trid,%s,chargeid,%s,buyid,%d,platform,%s,pkgname,%s,channel,%s,ver,%s",
		accountId, chargeId, buyId, statGetDeviceParams(StatParamPlatform)->getChar(),
		statGetDeviceParams(StatParamPackageName)->getChar(), channelId->getChar(),
		statGetDeviceParams(StatParamVersion)->getChar());

	{
		DString iapId = DString::format("buyid%d", buyId);
		TDCCVirtualCurrency::onChargeRequest(tradeNo, iapId->getChar(), totalFee / 100.0, "CNY", 0.0, "alipay");
	}

	int result = payStartAlipay(accountId, tradeNo, subject, body->getChar(), strFee->getChar());
	LOGD("startAlipay,result=%d", result);
	return result;
}

int StatConfig::startwxpay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee)
{
    DString strFee = DString::format("%d", totalFee);
    StatConfig* statConfig = Chess::getInstance()->getStatConfig();
    const char* accountId = "TR100001";
    DString appId = funConst->TalkingDataAppId;
    DString channelId = funConst->getChannelId();
    
    DString body = DString::format("trid,%s,chargeid,%s,buyid,%d,platform,%s,pkgname,%s,channel,%s,ver,%s",
                                   accountId, chargeId, buyId, statGetDeviceParams(StatParamPlatform)->getChar(),
                                   statGetDeviceParams(StatParamPackageName)->getChar(), channelId->getChar(),
                                   statGetDeviceParams(StatParamVersion)->getChar());
    DString iapId = DString::format("buyid%d", buyId);
    TDCCVirtualCurrency::onChargeRequest(tradeNo, iapId->getChar(), totalFee / 100.0, "CNY", 0.0, "wxpay");
    
    int result = payStartwxpay(accountId, tradeNo, subject, body->getChar(), strFee->getChar());
    LOGD("startAlipay,result=%d", result);
    return result;
}

int StatConfig::startSDKPay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee,const char* sign,const char* accountId,const char* sdkOrderId,const char* submitTime,const char* nickName)
{
	DString strFee = DString::format("%d", totalFee);
	StatConfig* statConfig = Chess::getInstance()->getStatConfig();
	const char* gameID = "TR100001";
	DString appId = funConst->TalkingDataAppId;
	DString channelId = funConst->getChannelId();

	DString body = DString::format("trid,%s,chargeid,%s,buyid,%d,platform,%s,pkgname,%s,channel,%s,ver,%s,sign,%s,accountId,%s,sdkOrderId,%s,submitTime,%s,nickname,%s",
		gameID, chargeId, buyId, statGetDeviceParams(StatParamPlatform)->getChar(),
		statGetDeviceParams(StatParamPackageName)->getChar(), channelId->getChar(),
		statGetDeviceParams(StatParamVersion)->getChar(),sign,accountId,sdkOrderId, submitTime, nickName);
	DString iapId = DString::format("buyid%d", buyId);
	TDCCVirtualCurrency::onChargeRequest(tradeNo, iapId->getChar(), totalFee / 100.0, "CNY", 0.0, "SDKPay");

	int result = payStartSDKpay(gameID, tradeNo, subject, body->getChar(), strFee->getChar());
	LOGD("startSDKPay,result=%d", result);
	return result;
}


void StatConfig::onPayResult(PayType payType, const char* tradeNo, int resultCode, const char* resultMessage)
{
	LOGD("onPayResult,payType=%d,tradeNo=%s,resultCode=%d,resultMessage=%s", payType, tradeNo, resultCode, resultMessage);

	ScreenResponse res(ScreenResponse::TYPE_PAY);
	res.isSucceed = (resultCode == 1);
	res.tradeId = tradeNo;
	res.resultMessage = resultMessage;

	if (res.isSucceed&&!modConst->mIsTestService)
	{
		TDCCVirtualCurrency::onChargeSuccess(tradeNo);
	}

	PaymentTool::getInstance()->OnResponse(&res, payType);
}


void StatConfig::onLoadItunesPriceResult(int success)
{

	ScreenResponse res(ScreenResponse::TYPE_REFRESH_ITUNES_PRICE);
	res.isSucceed = (success == 1);
	
	Chess::getInstance()->OnResponse(&res);
}

void StatConfig::onRestoreIosProductResult(int success)
{
	ScreenResponse res(ScreenResponse::TYPE_RESTORE_IOS_PRODUCT);
	res.isSucceed = (success == 1);
	Chess::getInstance()->OnResponse(&res);
}

#ifdef HMW_PAY
extern void HMWPayStartWithOrder(const char * orderId, const char * productName, double productPrice);
int StatConfig::startHMWPay(const char* tradeNo, const char* Id, double totalFee)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    HMWPayStartWithOrder(tradeNo, Id, totalFee);
#else
    return 1;
#endif
}
#endif

void StatConfig::onUpdateWXPayFinished(HttpClient* client, HttpResponse* response)
{
	LOGD("onUpdateWXPayFinished");
}

void StatConfig::startWXHttpRequest(DString appid, DString mch_id, DString out_trade_no, DString nonce_str, DString subject, DString body)
{
	LOGD("strartWXHttpRequest");
	HttpRequest* request = new HttpRequest();

	DString notifyUrl = "https://api.trgame.cn/service/wxpay/notify_url.php";
	LOGD("notifyUrl=%s", notifyUrl->getChar());
	request->setUrl(notifyUrl->getChar());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(Chess::getInstance()->getStatConfig(), httpresponse_selector(StatConfig::onUpdateWXPayFinished));
	request->setTag("WX_Pay_Record");

	DString postdata = "appid=" + appid + "&mch_id=" + mch_id + "&out_trade_no=" + out_trade_no + "&nonce_str=" + nonce_str + "&subject=" + subject + "&body=" + body;
	request->setRequestData(postdata->getChar(), strlen(postdata->getChar()));//设置请求数据，传给服务器
	HttpClient::getInstance()->send(request);
	request->release();
}

void StatConfig::GetWXAccessToken(DString appid, DString secret, DString code)
{
	LOGD("GetWXAccessToken");
	HttpRequest* request = new HttpRequest();
	DString strURL = ChessSprintf("%s?appid=%s&secret=%s&code=%s&grant_type=authorization_code", HTTP_GET_WX_ACCESS, appid->getChar(), secret->getChar(), code->getChar());
	LOGD("notifyUrl=%s", strURL->getChar());
	request->setUrl(strURL->getChar());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(Chess::getInstance()->getStatConfig(), httpresponse_selector(StatConfig::onGetAccessFinished));
	request->setTag("WX_GET_ACCESS");
	HttpClient::getInstance()->send(request);
	request->release();
}

void StatConfig::onGetAccessFinished(HttpClient* client, HttpResponse* response)
{
	LOGD("onGetAccessFinished");
	DString text = "";
	if(readResponseString(response, text))
	{
		Chess::getInstance()->getThreadRun()->addRunnable(new DResponseProcess(DResponseProcess::TYPE_GET_WX_ACCESS_FINISHED, text));
	}
	else
	{
		LoginLayer::showLoginWarn(LL("denglushibaiqingchongshi"));
	}
}

void StatConfig::SetAccountConfig(std::string accountS, std::string accountNameS, int iType)
{
	//LOGD("TDCCTalkingDataGA::getDeviceId=%s", TDCCTalkingDataGA::getDeviceId());
	TDCCAccount* account = TDCCAccount::setAccount(accountS.c_str());
	if (iType == 1)
	{
		account->setAccountType(TDCCAccount::kAccountAnonymous);
	}
	else if (iType == 2)
	{
		account->setAccountType(TDCCAccount::kAccountRegistered);
		account->setAccountName(accountNameS.c_str());
	}
	else if (iType == 3)
	{
		account->setAccountType(TDCCAccount::kAccountRegistered);
	}
	else if (iType == 4)
	{
		account->setAccountType(TDCCAccount::kAccountType1);
		account->setAccountName(accountNameS.c_str());
	}
	else if (iType == 5)
	{
		account->setAccountType(TDCCAccount::kAccountType2);
		account->setAccountName(accountNameS.c_str());
	}
}
