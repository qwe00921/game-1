//
//  UserDate.h
//  TRGame
//
//  Created by Jimmy on 17/02/13.
//
//

#ifndef _UserDate_H_
#define _UserDate_H_

#include "Defines.h"

class ReceiptInfo
{
public:
	ReceiptInfo();
	void setReceipt(string strOrder, string strReceipt, string strRecNum);
	bool parseJson(Json::Value jsonVal);
	Json::Value saveJson();

	CC_SYNTHESIZE(int, m_iMark, MarkCode);//内核编号
	CC_SYNTHESIZE(string, m_strOrder, OrderID);//订单号
	CC_SYNTHESIZE(string, m_strTitle, Title);//订单标题
	CC_SYNTHESIZE(string, m_strReceipt, Receipt);//支付凭证
	CC_SYNTHESIZE(string, m_strRecNum, RecNum);//支付凭证单号
};

class UserDate : public Ref
{
public:
	static UserDate*  getInstance();
	void addTradeNo(string strOrder, string strTitle); //添加订单
	void addReceipt(string strOrder, string strReceipt, string strRecNum); //添加支付凭证
	void deleteReceipt(string strOrder); //删除支付凭证

	ReceiptInfo* getReceiptInfo(); //获取需要恢复的订单信息
private:
	UserDate();
	bool loadUserDate();
	bool parseJson(int iType);
	void saveJson(int iType);

private:
	static UserDate* m_pInstance;
	Json::Value m_jsValue;

	map<string, ReceiptInfo*> m_mReceipt;
};

#endif //_UserDate_H_