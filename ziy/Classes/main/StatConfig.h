#ifndef _StatConfig_H_
#define _StatConfig_H_
    
#include "cocos2d.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"
#include "DString.h"
#include "otherfunction.h"
using namespace cocos2d::network;

/**
 * 统计配置类
 */
class StatConfig : cocos2d::Ref 
{
public:
	enum PayType
	{
		PAY_TYPE_ALIPAY,
		PAY_TYPE_IOS_APPSTORE,
		PAY_TYPE_WXPAY,
		PAY_TYPE_QDSDK
	};
public:
	/**
	* 构造函数
	*/
	StatConfig();
	/**
	* 虚析构函数，覆盖 cocos2d::CCObject 析构函数
	*/
	virtual ~StatConfig();
public:
    /**
     * 取得账号信息
     * @return 账号信息DString
     */
    DString getGameAccountInfo();
	/**
	* 触发用户等级修改的事件
	*/
	void onUserLevelChanged(int level);

	static void startWXHttpRequest(DString appid, DString mch_id, DString out_trade_no, DString nonce_str, DString subject, DString body);

	/**
	* 设置talkingdata账户信息
	*/
	void SetAccountConfig(std::string accountS, std::string accountNameS, int iType);//type 1：游客 2：注册

public:
	/**
	* 开始任务
	* @param missionId 任务/关卡/副本id
	*/
	static void onBeginMission(const char* missionId);
	/**
	* 完成任务
	* @param missionId 任务/关卡/副本id
	*/
	static void onCompletedMission(const char* missionId);
	/**
	* 任务失败
	* @param missionId 任务/关卡/副本id
	* @param cause 失败原因
	*/
	static void onFailedMission(const char* missionId, const char* cause);
	/**
	* 触发事件
	* @param eventId 事件id
	*/
	static void onEvent(const char* eventId);
	/**
	* 触发事件
	* @param eventId 事件id
	* @param map 事件参数
	*/
	static void onEvent(const char* eventId, std::map< std::string, std::string >* map);	
	/**
	* 购买物品
	* @param buyId 购买id
	* @param num 购买数量
	*/
	static void onPurchaseBuy(int buyId, int num);
	/**
	* 领取物品
	* @param buyId 购买id
	* @param num 领取数量
	*/
	static void onUseBuy(int buyId, int num);
	
	/**
	* 开始支付宝计费
	* @param tradeNo 输入的订单号
	* @param subject 标题
	* @param buyId 购买id
	* @param chargeId 计费id
	* @param totalFee 费用，单位为分
	* @return 1表示继续执行，等待回调消息；0表示失败
	*/
	static int startAlipay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee);

	/**
	* 开始微信计费
	* @param tradeNo 输入的订单号
	* @param subject 标题
	* @param buyId 购买id
	* @param chargeId 计费id
	* @param totalFee 费用，单位为分
	* @return 1表示继续执行，等待回调消息；0表示失败
	*/
	static int startwxpay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee);

	/**
	* 开始SDK计费
	* @param tradeNo 输入的订单号
	* @param subject 标题
	* @param buyId 购买id
	* @param chargeId 计费id
	* @param totalFee 费用，单位为分
	* @param sign 某sdk需要的额外签名参数
	* @param accountId 某sdk需要的额外签名参数
	* @return 1表示继续执行，等待回调消息；0表示失败
	*/
	static int startSDKPay(const char* tradeNo, const char* subject, int buyId, const char* chargeId, int totalFee,const char* sign, const char* accountId, const char* sdkOrderId,const char* submitTime, const char* nickName);

	/**
	* 开始IOSAppStore计费
	* @param tradeNo 输入的订单号
	* @param productID 计费点
	* @param totalFee 费用，单位为分
	* @return 1表示继续执行，等待回调消息；0表示失败
	*/
	static int startIOSAppStorepay(const char* tradeNo, const char* chTitle, const char* productID, int totalFee);

	/**
	* 开始IOS评论页面
	* @return 1表示继续执行，等待回调消息；0表示失败
	*/
	static int showAppStore(const char* appId);

	/**
	* sdk要求的额外用户数据提交,顺便提交到talkingdata.
	*/
	static int submitExtendData(const char* jsonStr);

	/**
	* 计费返回
	* @param payType 计费类型
	* @param tradeNo 订单号
	* @param resultCode 返回码，1表示成功；0表示失败
	* @param resultMessage 返回消息
	*/
	static void onPayResult(PayType payType, const char* tradeNo, int resultCode, const char* resultMessage);
	static void onLoadItunesPriceResult(int success);
	static void onRestoreIosProductResult(int success);

    static int startHMWPay(const char* tradeNo, const char* Id, double totalFee);
private:
	bool readResponseString(HttpResponse* response, DString& outStr);
	void onUpdateWXPayFinished(HttpClient* client, HttpResponse* response);
	/**
	* 取得本地账号id
	* @return 账号id，LC1234……的格式
	*/
	const char* getLocalAccountId();
	
	//登陆相关请求
public:
	static void GetWXAccessToken(DString appid, DString secret, DString code);
private:
	void onGetAccessFinished(HttpClient* client, HttpResponse* response);
public:
	DString mLocalAccountId;
};

#endif
