#ifndef __TRGAME_PAYMENTTOOL_
#define __TRGAME_PAYMENTTOOL_

#include <stdio.h>
#include "Common.h"
#include "main.h"
#include "StatConfig.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum
{
	PAY_TYPE_APPSTORE = 0, //AppStore支付
	PAY_TYPE_WIN32 = 1,  //Win32模拟器支付
	PAY_TYPE_ALIPAY = 2, //支付宝支付
	PAY_TYPE_WXPAY = 3, //微信支付
	PAY_TYPE_QDSDK = 4//渠道SDK支付
};

class PaymentTool:public Ref,EventDelegate
{
public:
	static PaymentTool* getInstance();
	~PaymentTool();
	virtual void SelectPayRequest(int iPayType);
	void startPay(int GoodsId, string chargeId, string strTitle, EventDelegate* ptrDelegate, int iActid = 0);
	void startRestore(string strTradeNo, string strTitle, EventDelegate* ptrDelegate);
	void OnResponse(ScreenResponse* res, StatConfig::PayType payType);
	string getNotifyUrl(); //获取第三方支付回调地址
	void showWarning(string strWarn); //显示支付警告
	void setIOSReceipt(string strReceipt, string strRecNum); //设置IOS支付凭证相关数据
	void setmSdkPayCallName(const char* strName);//设置sdk支付回调接口名称
	void SDKPayResult(const char* strJson);
	void getOrderStatus(const char* orderId);
private:
	PaymentTool();
	void startPayment();
	void GetOrderNumber();
	void onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata);
	void onPaySucceed(Json::Value valJson, int iSucceed);

	void SendWinSuccessDate();
	void SendIosSuccessDate();
	void SendPayFailDate();
	void SendAndroidSuccessDate();
	void ChildCloseLayer(int iNomal);
	string getPayType(); //获取支付类型标签
private:
	static PaymentTool* m_instance;
	int mGoodsId;
	int mPayType;
	int m_iActId;
	bool mBRestore; //恢复订单

	string mSign;//sdk sign
	string mAccountId;//sdk
	string mSdkOrderId;//sdk orderId
	string mSdkOrderSubmitTime;

	string mChargeId;
	string mTradeNo;
	string mReceipt; //IOS支付凭证
	string mRecNum; //IOS支付订单
	string mTitle; //计费点名称
	string mSdkPayCallName; //sdk支付回调接口名称
	EventDelegate* mDelegate;
	TrPayInfo::TrChargeItem* mChargeItem;
};
#endif /* __TRGAME_WINCONDITIONTOOL_ */
