#ifndef _TRACCOUNTBUY_H_
#define _TRACCOUNTBUY_H_
    
#include "DList.h"
#include "DString.h"
#include "jsons.h"

using namespace Json;

class DataInt
{
public:
	DataInt();
	bool ParseJson(Json::Value jsonVal);
	Json::Value SaveJson();
	bool ParseExGift(Json::Value jsonVal);
	Json::Value SaveExGift();
public:
	int m_iId;
	int m_iVal;
	int m_YBsize;
};

class DataGift
{
public:
	DataGift();
	bool ParseJson(Json::Value jsonVal);
	Json::Value SaveJson();
public:
	int m_iId;
	int m_iCount;
	int m_iUsed;
};

class ExGiftAttr
{
public:
	ExGiftAttr();
	ExGiftAttr(ExGiftAttr* pExGift);
	~ExGiftAttr();
	bool ParseJson(Json::Value jsonVal);
	Json::Value SaveJson();
public:
	int iGiftId;
	DString strGiftName;
	DString strGiftInfo;
	DList<DataInt>* pGiftlist; 
};

class TrAccountBuy
{
public:
	TrAccountBuy();
	~TrAccountBuy();
	bool LoadPayRecord();
	bool SavePayRecord(bool isUpdate = true);
	bool ParseJson(Json::Value jsonVal,bool isOnlyUpdatePay = false);
	void GeneratePayRecord(TrAccountBuy* pPayRec);
	bool isBuy(int buyId);
	void AddPayTimes(int iBuyId, bool bNeedSave = true,int ybsize = 0); //添加购买记录次数
	int GetPayTimes(int iBuyId); //获取购买记录次数
    void SetPayTimes(int iBuyId, int iTimes); //设置购买记录次数
	void SetGoldNum(int iNum,bool bNeedSave = true); //设置当前元宝数目
	int GetGoldNum() //获取当前元宝数目
	{
		return m_iGoldNum;
	}
	bool LoadExchangeGift(); //读取本地兑换礼包文件
	bool SaveExGift();
	bool ParseExGift(Json::Value jsonValue); //解析下载到的兑换礼包数据
	void GenerateExchangeGift(TrAccountBuy* pTrAccount);
	void SetExGiftTimes(int iGiftId, int iNum); //设置兑换礼包的次数
	int GetExGiftTimes(int iGiftId);
	int GetExGiftUsed(int iGiftId); //获取兑换礼包的领取次数
	void AddExGiftUsed(int iGiftId); //增加兑换礼包的领取次数
	ExGiftAttr* GetExGiftInfo(int iGiftId); //获取兑换礼包的内容
	bool IsGoldExGift(int iGiftId); //判断是不是只包含元宝的礼包
	bool IsHavePayRec();
	Json::Value SavePayRecJson();
	bool LoadConfig();
private:
	Json::Value SaveJson();
	DataInt* FindPayItem(int iId);
	DataGift* FindExGift(int iGiftId);
	int GetPaySize()
	{
		return m_pPayRecord->size();
	}
	DataInt* GetPayItem(int index)
	{
		return m_pPayRecord->elementAt(index);
	}
public:
	DList<DataGift>* m_pGiftRecord; //兑换礼包的领取记录
	DList<ExGiftAttr>* m_pExGift; //兑换礼包数据

private:
	DList<DataInt>* m_pPayRecord; //支付记录
	int m_iGoldNum; //元宝数目
};

#endif // _TRACCOUNTBUY_H_