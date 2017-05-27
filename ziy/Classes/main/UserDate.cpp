//
//  UserDate.cpp
//  TRGame
//
//  Created by Jimmy on 17/02/13.
//
//

#include "UserDate.h"


#define FILE_IOS_PAY "iospay.dat"
#define FILE_LOGIN_INFO "login.dat"

enum 
{
	JSON_IOS_PAY = 0,
	JSON_LOGIN_INFO = 1
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ReceiptInfo::ReceiptInfo()
{
	m_iMark = 0;
	m_strOrder = "";
	m_strTitle = "";
	m_strReceipt = "";
	m_strRecNum = "";
}

void ReceiptInfo::setReceipt(string strOrder, string strReceipt, string strRecNum)
{
	m_strOrder = strOrder;
	m_strReceipt = strReceipt;
	m_strRecNum = strRecNum;
}

bool ReceiptInfo::parseJson(Json::Value jsonVal)
{
	ParseInt(m_iMark, jsonVal["markcode"]);
	ParseString(m_strOrder, jsonVal["orderid"]);
	ParseString(m_strTitle, jsonVal["title"]);
	ParseString(m_strReceipt, jsonVal["receipt"]);
	ParseString(m_strRecNum, jsonVal["recnum"]);
	return true;
}

Json::Value ReceiptInfo::saveJson()
{
	Json::Value jsonVal;
	jsonVal["markcode"] = m_iMark;
	jsonVal["orderid"] = m_strOrder;
	jsonVal["title"] = m_strTitle;
	jsonVal["receipt"] = m_strReceipt;
	jsonVal["recnum"] = m_strRecNum;
	return jsonVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UserDate* UserDate::m_pInstance = nullptr;

UserDate* UserDate::getInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new UserDate();
		m_pInstance->loadUserDate();
	}
	return m_pInstance;
}

void UserDate::addTradeNo(string strOrder, string strTitle)
{
	ReceiptInfo* ptrReceipt = new ReceiptInfo();
	ptrReceipt->setMarkCode(history->m_pLoginInfo->getMarkCode());
	ptrReceipt->setOrderID(strOrder);
	ptrReceipt->setTitle(strTitle);
	m_mReceipt[strOrder] = ptrReceipt;
	saveJson(JSON_IOS_PAY);
}

void UserDate::addReceipt(string strOrder, string strReceipt, string strRecNum)
{
	if(m_mReceipt.count(strOrder) > 0)
	{
		m_mReceipt[strOrder]->setReceipt(strOrder, strReceipt, strRecNum);
		saveJson(JSON_IOS_PAY);
	}
}

void UserDate::deleteReceipt(string strOrder)
{
	if (m_mReceipt.count(strOrder) > 0)
	{
		delete m_mReceipt[strOrder];
		m_mReceipt.erase(strOrder);
		saveJson(JSON_IOS_PAY);
	}
}

ReceiptInfo* UserDate::getReceiptInfo()
{
	if (modConst->m_bRestoreClose)
	{
		return nullptr;
	}

	ReceiptInfo* ptrReceipt = nullptr;
	for (map<string, ReceiptInfo*>::iterator it = m_mReceipt.begin(); it != m_mReceipt.end(); it++)
	{
		ReceiptInfo* ptrInfo = it->second;
		if (ptrInfo->getMarkCode() == history->m_pLoginInfo->getMarkCode() && ptrInfo->getReceipt() != "")
		{
			ptrReceipt = ptrInfo;
			break;
		}
	}
	return ptrReceipt;
}

bool UserDate::loadUserDate()
{
	Reader reader;

	char* chDate = ChessReadFile(FILE_IOS_PAY, 1);
	if (chDate && reader.parse(chDate, m_jsValue, false))
	{
		parseJson(JSON_IOS_PAY);
	}
	ChessFree(chDate);

	return true;
}

UserDate::UserDate()
{

}

bool UserDate::parseJson(int iType)
{
	switch(iType)
	{
	case JSON_IOS_PAY:
	{
		Json::Value jsonList = m_jsValue["payList"];
		if (jsonList.isArray())
		{
			for (UInt i = 0; i < jsonList.size(); i++)
			{
				ReceiptInfo* ptrInfo = new ReceiptInfo();
				ptrInfo->parseJson(jsonList[i]);
				m_mReceipt[ptrInfo->getOrderID()] = ptrInfo;
			}
		}
	}
	break;
	default:
		break;
	}

	return true;
}

void UserDate::saveJson(int iType)
{
	if (modConst->m_bRestoreClose)
	{
		return;
	}

	switch(iType)
	{
	case JSON_IOS_PAY:
	{
		Json::Value jsonVal;
		Json::Value jsonList(arrayValue);
		for(map<string, ReceiptInfo*>::iterator it = m_mReceipt.begin(); it != m_mReceipt.end(); it++)
		{
			ReceiptInfo* ptrInfo = it->second;
			jsonList.append(ptrInfo->saveJson());
		}
		jsonVal["payList"] = jsonList;

		FastWriter fastWriter;
		std::string strWriter = fastWriter.write(jsonVal);
		ChessWriteFile(FILE_IOS_PAY, (char*)strWriter.c_str(), strWriter.size());
	}
	break;
	default:
		break;
	}
}
