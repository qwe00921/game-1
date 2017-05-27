#include "PaymentTool.h"
#include "Defines.h"
#include "StatConfig.h"
#include "SelectPayLayer.h"
#include "ActivityInfo.h"
#include "UserDate.h"
#include "NetPromptTool.h"

//#define PAY_TEST1 //测试充值1
PaymentTool* PaymentTool::m_instance = NULL;

PaymentTool* PaymentTool::getInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new PaymentTool();
	}
	return m_instance;
}

void PaymentTool::startPay(int GoodsId, string chargeId, string strTitle, EventDelegate* ptrDelegate, int iActid /*= 0*/)
{
	mGoodsId = GoodsId;
	mChargeId = chargeId;
	mDelegate = ptrDelegate;
	m_iActId = iActid;
	mChargeItem = trData->trGamePay->getPayInfo()->findChargeItem(chargeId.c_str());
	if (strTitle == "")
	{
		strTitle = mChargeItem->subject->getChar();
	}
	mTitle = strTitle;
	mBRestore = false;

	if (!modConst->OpenIosAliPay && CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		mPayType = PAY_TYPE_APPSTORE;
		GetOrderNumber();
	}
	else
	{
		if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) {
			//if (true) {//test
			//	this->setmSdkPayCallName("verifyUCPay");
			//	mPayType = PAY_TYPE_QDSDK;
			//	GetOrderNumber();
			//	return;
			//}
		}
		if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
			mPayType = PAY_TYPE_QDSDK;
			GetOrderNumber();
		}
		else {
			Director::getInstance()->getRunningScene()->addChild(SelectPayLayer::create(GoodsId, chargeId, this), 998);
		}

	}
}

void PaymentTool::startRestore(string strTradeNo, string strTitle, EventDelegate* ptrDelegate)
{
	mBRestore = true;
	mTradeNo = strTradeNo;
	mTitle = strTitle;
	mDelegate = ptrDelegate;
}

void PaymentTool::SelectPayRequest(int iPayType)
{
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	{
		mPayType = PAY_TYPE_WIN32;
	}
	else
	{
		mPayType = iPayType;
	}
	GetOrderNumber();
}

void PaymentTool::startPayment()
{
	if (mPayType == PAY_TYPE_APPSTORE)
	{
		CCLOG("startAppStore");
		if(trData->trGamePay->getPayInfo()->findChargeItem(mChargeId.c_str())->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY) != NULL)
		{
			DString productId = trData->trGamePay->getPayInfo()->findChargeItem(mChargeId.c_str())->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY)->iosProductID;
			StatConfig::startIOSAppStorepay(mTradeNo.c_str(), mTitle.c_str(), productId->getChar(), trData->trGamePay->getPayInfo()->findChargeItem(mChargeId.c_str())->price*
				trData->trGamePay->getPayInfo()->findChargeItem(mChargeId.c_str())->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY)->discount / 100);
		}
	}
	else if (mPayType == PAY_TYPE_WIN32)
	{
		//模拟IOS支付成功
		CCLOG("startWin32PAY");
		string strReceipt = "receipt";
		string strRecNum = "10000";
		setIOSReceipt(strReceipt, strRecNum); //支付凭证
		StatConfig::onPayResult(StatConfig::PAY_TYPE_IOS_APPSTORE, mTradeNo.c_str(), 1, "Succeed");
	}
	else if(mPayType == PAY_TYPE_ALIPAY)
	{
		CCLOG("startAlipay");
		StatConfig::startAlipay(mTradeNo.c_str(), mChargeItem->subject->getChar(),
			mGoodsId, mChargeItem->chargeId->getChar(), mChargeItem->price*mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_ALIPAY)->discount / 100);
	}
	else if(mPayType == PAY_TYPE_WXPAY)
	{
		CCLOG("startWXpay");
		int iDiscount = 100;
		if(mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_WXPAY))
		{
			iDiscount = mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_WXPAY)->discount;
		}
		else if(mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_ALIPAY))
		{
			iDiscount = mChargeItem->findChannelItemById(TrPayInfo::CHANNEL_ALIPAY)->discount;
		}
		StatConfig::startwxpay(mTradeNo.c_str(), mChargeItem->subject->getChar(), mGoodsId, mChargeItem->chargeId->getChar(), mChargeItem->price * iDiscount / 100);
	}
	else if (mPayType == PAY_TYPE_QDSDK)
	{
		CCLOG("start qd sdk pay");
		int iDiscount = 100;
		StatConfig::startSDKPay(mTradeNo.c_str(), mChargeItem->subject->getChar(), mGoodsId, mChargeItem->chargeId->getChar(), mChargeItem->price * iDiscount / 100,mSign.c_str(), mAccountId.c_str(), mSdkOrderId.c_str(), mSdkOrderSubmitTime.c_str(), history->m_pPlayerInfo->getName().c_str());
	}
}

void PaymentTool::GetOrderNumber()
{
	ITools::getInstance()->addWaitLayer();

	string GameTokenS = history->m_pLoginInfo->getGameToken();
	string strType = getPayType();
	CocosMD5 md5;
#ifdef PAY_TEST1
	mChargeItem->price = 100;
#endif // PAY_TEST1

	int iPayM = mChargeItem->price / 100;
	string strValidate = StringUtils::format("gameToken=%s&paymoney=%d&payType=%s&callBack=%s", GameTokenS.c_str(), iPayM, strType.c_str(), mSdkPayCallName.c_str());
	string validateCode = md5.ToMD5(strValidate);

	string strData = "";
	if (m_iActId)
	{
		ActivityElem* ptrActivity = BTActivity::getInstance()->m_mActivity[m_iActId];
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"paymoney\":%d,\"payType\":\"%s\",\"callBack\":\"%s\",\"subject\":\"%s\",\"paymark\":%d,\"aid\":%d,\"validateCode\":\"%s\"}", GameTokenS.c_str(), iPayM, strType.c_str(), mSdkPayCallName.c_str(), mTitle.c_str(), ptrActivity->getRMBMark(), m_iActId, validateCode.c_str());
	}
	else
	{
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"paymoney\":%d,\"payType\":\"%s\",\"callBack\":\"%s\",\"subject\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), iPayM, strType.c_str(), mSdkPayCallName.c_str(), mTitle.c_str(), validateCode.c_str());
	}
	CCLOG("%s", strData.c_str());

	ITools::getInstance()->connectingNetwork("creatrOrder", HttpRequest::Type::POST, this, callfuncND_selector(PaymentTool::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, "GET_ORDER_NUM");
}

void PaymentTool::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		Json::Value ResValue = value["result"];
		if(strTag == "GET_ORDER_NUM")
		{
			if (ResValue["sign"].isString())
			{
				mSign = ResValue["sign"].asCString();
			}
			if (ResValue["sdkOrderId"].isString())
			{
				mSdkOrderId = ResValue["sdkOrderId"].asCString();
			}
			if (ResValue["submitTime"].isString())
			{
				mSdkOrderSubmitTime = ResValue["submitTime"].asCString();
			}
			
			
			if (ResValue["accountId"].isString())
			{
				mAccountId = ResValue["accountId"].asCString();
			}
			if(ResValue["payID"].isString())
			{
				mTradeNo = ResValue["payID"].asCString();
				startPayment();
			}
		}
		else if (strTag == "SEND_WIN_SUCCESS_DATE")
		{
			onPaySucceed(ResValue, 1);
		}
		else if(strTag == "SEND_IOS_SUCCESS_DATE")
		{
			UserDate::getInstance()->deleteReceipt(mTradeNo);
			int iSucceed = ResValue["isTrue"].asInt();
			if (mBRestore)
			{
				if (iSucceed)
				{
					map<int, string> mStrNotice;
					mStrNotice[0] = mTitle;
					mStrNotice[1] = history->getRestoreNotice(ResValue);
					mDelegate->updateRTDate(mStrNotice);
				}
			}
			else
			{
				if (!iSucceed)
				{
					iSucceed = -1;
				}
				onPaySucceed(ResValue, iSucceed);
			}
		}
		else if(strTag == "SEND_ANDROID_SUCCESS_DATE")
		{
			if(ResValue["status"].isInt())
			{
				onPaySucceed(ResValue, ResValue["status"].asInt());
			}
		}
		else if (strTag == "getOrderStatus")
		{
			if (ResValue["status"].isInt())
			{
				if (ResValue["status"].asInt() == 1) {
					StatConfig::onPayResult(StatConfig::PAY_TYPE_QDSDK, ResValue["payID"].asCString(), 1, "SDKPAY");
					NetPromptTool::getInstance()->setGetOrderStatus(false, "");
				}
			}
		}
		
		else if(strTag == "SEND_PAY_FAIL_DATE")
		{
			CCLOG("send pay fail date!");
		}
	}
}

void PaymentTool::onPaySucceed(Json::Value valJson, int iSucceed)
{
	if(iSucceed)
	{
		if(iSucceed == 1)
		{
			if (m_iActId)
			{
				map<int, string> strNotice = history->onHttpGoodsAdd(valJson);
				mDelegate->updateRTDate(strNotice);
			}
			else
			{
				if(valJson["jade"].isInt() && valJson["jadeSeal"].isInt())
				{
					history->m_iJade = valJson["jade"].asInt();
					history->m_iJadeSeal = valJson["jadeSeal"].asInt();
				}
				history->m_pPlayerInfo->setBuyJadeState(trData->net_jade_charge[mGoodsId]->mRmb);
			}
		}
		mDelegate->RunFunction(iSucceed);
	}
	else
	{
		SendAndroidSuccessDate();
	}
}

void PaymentTool::OnResponse(ScreenResponse* res, StatConfig::PayType payType)
{
	if (res->isTypePay())
	{
		if (res->isSucceed&& res->tradeId->equals(mTradeNo.c_str()))
		{
			if (payType == StatConfig::PAY_TYPE_IOS_APPSTORE)
			{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试代码///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//TrPayInfo::TrChargeItem* ptrItem1 = trData->trGamePay->getPayInfo()->findChargeItem("zqtx804");
				//TrPayInfo::TrChargeItem* ptrItem2 = trData->trGamePay->getPayInfo()->findChargeItem("zqtx807");
				//string strTitle1 = ptrItem1->subject->getChar();
				//string strTitle2 = ptrItem2->subject->getChar();
				//if (mBRestore == false && (mTitle == strTitle1 || mTitle == strTitle2 || m_iActId)) //中断程序
				//{
				//	ptrItem1 = nullptr;
				//	strTitle1 = ptrItem1->subject->getChar();
				//}
//测试代码///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
				SendWinSuccessDate();
#else
				SendIosSuccessDate();
#endif
			}
			else
			{
				SendAndroidSuccessDate();
			}
		}
		else
		{
			SendPayFailDate();
			if (mDelegate)
				mDelegate->RunFunction(-1);
		}
	}
}

string PaymentTool::getNotifyUrl()
{
	string strUrl = "";
	if (mPayType == PAY_TYPE_ALIPAY)
	{
		if (modConst->mIsTestService)
		{
			strUrl = "https://longzqtx.leanapp.cn/alipayCallBack";
		}
		else
		{
			strUrl = "https://zqtxqd.leanapp.cn/alipayCallBack";
		}
	}
	else if (mPayType == PAY_TYPE_WXPAY)
	{
		if(modConst->mIsTestService)
		{
			strUrl = "https://longzqtx.leanapp.cn/wxpayCallback";
		}
		else
		{
			strUrl = "https://zqtxqd.leanapp.cn/wxpayCallback";
		}
	}
	else if (mPayType == PAY_TYPE_QDSDK) {
		if (modConst->mIsTestService)
		{
			strUrl = "https://longzqtx.leanapp.cn/" + mSdkPayCallName;
		}
		else
		{
			strUrl = "https://zqtxqd.leanapp.cn/" + mSdkPayCallName;
		}
	}
	return strUrl;
}

void PaymentTool::showWarning(string strWarn)
{
	ITools::getInstance()->createGlobalPrompt(strWarn, false);
	ChildCloseLayer(0);
}

void PaymentTool::setIOSReceipt(string strReceipt, string strRecNum)
{
	mReceipt = strReceipt;
	mRecNum = strRecNum;
}

void PaymentTool::setmSdkPayCallName(const char* strName) {
	mSdkPayCallName = strName;
}

void PaymentTool::SDKPayResult(const char* strJson) {
	Json::Reader reader;
	Json::Value infoJson;
	reader.parse(strJson, infoJson);
	if (!infoJson.isNull()) {
		CCLOG("%s", infoJson.toStyledString().c_str());
		if (infoJson["getOrderStatus"].isBool()) {
			if (infoJson["getOrderStatus"].asBool()) {
				NetPromptTool::getInstance()->setGetOrderStatus(true, infoJson["payId"].asCString());
				return;
			}
		}
		if (infoJson["rs"].isInt() && infoJson["payId"].isString()) {
			StatConfig::onPayResult(StatConfig::PAY_TYPE_QDSDK, infoJson["payId"].asCString(), infoJson["rs"].asInt(), "SDKPAY");
			return;
		}

	}
}

void PaymentTool::SendPayFailDate()
{
	if (mPayType == PAY_TYPE_QDSDK) {// sdk充值不设置订单失败状态,由服务器维护
		return;
	}
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&payID=%s", GameTokenS.c_str(), mTradeNo.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"payID\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), mTradeNo.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("setOrderStatus",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PaymentTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SEND_PAY_FAIL_DATE");
}

void PaymentTool::SendWinSuccessDate()
{
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&payID=%s", GameTokenS.c_str(), mTradeNo.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"payID\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), mTradeNo.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("Win32pay",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PaymentTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SEND_WIN_SUCCESS_DATE");
}

void PaymentTool::SendIosSuccessDate()
{
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&payID=%s&receipt=%s&tid=%s", GameTokenS.c_str(), mTradeNo.c_str(), mReceipt.c_str(), mRecNum.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"payID\":\"%s\",\"receipt\":\"%s\",\"tid\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), mTradeNo.c_str(), mReceipt.c_str(), mRecNum.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("iosPayIsTrue",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PaymentTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SEND_IOS_SUCCESS_DATE");
}
void PaymentTool::getOrderStatus(const char* orderId) {
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&payID=%s", GameTokenS.c_str(), orderId);
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"payID\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), orderId, validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("getOrderStatus",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PaymentTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"getOrderStatus");
}

void PaymentTool::SendAndroidSuccessDate()
{
	ITools::getInstance()->addWaitLayer();
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&payID=%s", GameTokenS.c_str(), mTradeNo.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"payID\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), mTradeNo.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("getOrderStatus",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(PaymentTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"SEND_ANDROID_SUCCESS_DATE");
}

PaymentTool::PaymentTool()
{
	mGoodsId = 0;
	mPayType = -1;
	m_iActId = 0;
	mChargeId = "";
	mTradeNo = "";
	mSign = "";
	mSdkOrderId = "";
	mSdkOrderSubmitTime = "";
	mAccountId = "";
	mReceipt = "";
	mRecNum = "";
	mTitle = "";
	mSdkPayCallName = "";
	mDelegate = NULL;
	mChargeItem = NULL;
	mBRestore = false;
}

PaymentTool::~PaymentTool()
{

}

void PaymentTool::ChildCloseLayer(int iNomal)
{
	if (mDelegate)
	{
		mDelegate->updateParentShow();
	}
}

string PaymentTool::getPayType()
{
	string strType = "";
	if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	{
		strType = "IOS";
	}
	
	switch(mPayType)
	{
	case PAY_TYPE_APPSTORE:
		strType += "PAY";
		break;
	case PAY_TYPE_WIN32:
		strType += "Win32PAY";
		break;
	case PAY_TYPE_ALIPAY:
		strType += "ALIPAY";
		break;
	case PAY_TYPE_WXPAY:
		strType += "WXPAY";
		break;
	case PAY_TYPE_QDSDK:
		strType += "SDKPAY";
		break;
	default:
		break;
	}

	return strType;
}
