#ifndef _TR_PAY_INFO_H_
#define _TR_PAY_INFO_H_


#include "DList.h"
#include "DString.h"
#include "jsons.h"


/**所有付费信息*/
class TrPayInfo
{
public:
	enum ChannelType
	{
		CHANNEL_ALIPAY = 1,
		CHANNEL_LIANTONGPAY = 5,//联通
		CHANNEL_COOEEPAY = 6,//酷宇
		CHANNEL_DIANXINPAY = 7,//电信
		CHANNEL_IOSPAY = 8,   //IOS支付
		CHANNEL_WXPAY = 11  //微信支付
	};
public:
	/**计费渠道*/
	class TrChargeChannel
	{
	public:
		TrChargeChannel();
	public:
		int channelId;
		int discount;
		DString iosProductID;
	public:
		Json::Value toJsonValue();
	public:
		static TrChargeChannel* parseJsonValue(Json::Value jsonValue);
	};
	/**计费信息*/
	class TrChargeItem
	{
	public:
		TrChargeItem();
		~TrChargeItem();
	public:
		DString chargeId;
		DString subject;
		DString body;
		int price;
	public:
		int getChannelSize()
		{
			return mChannelList->size();
		}
		TrChargeChannel* getChannelItem(int index)
		{
			return mChannelList->elementAt(index);
		}
		TrChargeChannel* findChannelItemById(int channelId);
	public:
		Json::Value toJsonValue();
	private:
		DList<TrChargeChannel>* mChannelList;
	public:
		static TrChargeItem* parseJsonValue(Json::Value jsonValue);
	};
public:
	TrPayInfo();
	~TrPayInfo();
public:
	bool loadJsonValue(Json::Value jsonValue);
public:
	Json::Value toJsonValue();
public:
	int getChargeSize()
	{
		return mChargeList->size();
	}
	TrChargeItem* getChargeItem(int index)
	{
		return mChargeList->elementAt(index);
	}
	TrChargeItem* findChargeItem(const char* chargeId);
private:
	DList<TrChargeItem>* mChargeList;
};

/**物品购买信息*/
class TrGoodCharge
{
public:
	/**物品购买项*/
	class TrGoodItem
	{
	public:
		TrGoodItem();
	public:
		int id;
		int goodId;
        int goodLevel;
		int num;
		int levelStrict;
		int stageStrict;
		DString chargeId;
 		bool isReceived;
		int jh_id;//是否正式版领取0 否 1 是
        bool isInSecretShop;
		int rebuy;
		bool is_free;
		int is_display;
		////////////////////
		DString iosProductID;
		DString iosPriceLocal;
        DString iosPriceToDisplay;
		int itunesPrice;
		DString title;
		DString desc;
		////////////////////
	public:
		bool isFree();
	public:
		Json::Value toJsonValue();
	public:
		static TrGoodItem* parseJsonValue(Json::Value jsonValue);
		void parseItunesJsonValue(Json::Value jsonValue);
	};
public:
	TrGoodCharge();
	~TrGoodCharge();
public:
	bool loadJsonValue(Json::Value jsonValue);
public:
	Json::Value toJsonValue();
public:
	int getGoodSize()
	{
		return mGoodList->size();
	}
	TrGoodItem* getGoodItem(int index)
	{
		return mGoodList->elementAt(index);
	}
	TrGoodItem* getGoodItemById(int id)
	{
		for (int i = 0; i < mGoodList->size();i++)
		{
			if (mGoodList->elementAt(i)->id == id)
			{
				return mGoodList->elementAt(i);
			}
		}
		return NULL;
	}
	void removeAllGoodCharge()
	{
		mGoodList->removeAllElements();
	}
	void removeGoodCharge(TrGoodItem* item)
	{
		mGoodList->removeElement(item);
	}
	bool loadItunesPriceJsonValue(const char* sData);
    bool isItunesPriceLoadSuccess();
	
	
private:
	DList<TrGoodItem>* mGoodList;
};

/**游戏物品和付费*/
class TrGamePay
{
public:
	TrGamePay();
	~TrGamePay();
public:
	TrPayInfo* getPayInfo()
	{
		return mPayInfo;
	}

	TrGoodCharge* getGoodCharge()
	{
		return mGoodCharge;
	}
public:
	bool loadJsonValue(Json::Value root);
	Json::Value toJsonValue();
public:
	bool loadConfig();
	bool saveConfig();
	bool checkData();
	void loadDefault();
	void loadItunesPrice(const char* json);//解析itunes价格
    bool isItunesPriceLoadSuccess();
private:
	Json::Value loadFileJsonValue(const char* file);

private:
	TrPayInfo* mPayInfo;
	TrGoodCharge* mGoodCharge;
};

#endif //#ifndef _TR_PAY_INFO_H_