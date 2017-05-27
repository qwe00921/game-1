#include "Common.h"
#include "TrAccountBuy.h"
#include "main.h"
#include "Data.h"
#include "StatConfig.h"

#define PARSEINT(A, B) if (B.isInt()) A = B.asInt();
#define PARSESTRING(A, B) if (B.isString()) A = B.asCString();

DataInt::DataInt()
{
	m_iId = 0;
	m_iVal = 0;
	m_YBsize = 0;
}

bool DataInt::ParseJson(Json::Value jsonVal)
{
	if(jsonVal["buy_id"].isInt())
	{
		m_iId = jsonVal["buy_id"].asInt();
		if(jsonVal["count"].isInt())
		{
			m_iVal = jsonVal["count"].asInt();
		}
		else
		{
			m_iVal = 1;
		}
		if (!jsonVal["YBsize"].isNull() && jsonVal["YBsize"].isInt())
		{
			m_YBsize = jsonVal["YBsize"].asInt();
		}
		else
		{
			m_YBsize = 0;
		}
		return true;
	}
	return false;
}
Json::Value DataInt::SaveJson()
{
	Json::Value jsonVal;
	jsonVal["buy_id"] = m_iId;
	jsonVal["count"] = m_iVal;
	jsonVal["YBsize"] = m_YBsize;
	return jsonVal;
}

bool DataInt::ParseExGift(Json::Value jsonVal)
{
	if(jsonVal["good_id"].isInt())
	{
		PARSEINT(m_iId, jsonVal["good_id"]);
		PARSEINT(m_iVal, jsonVal["count"]);
		return true;
	}
	return false;
}

Json::Value DataInt::SaveExGift()
{
	Json::Value jsonVal;
	jsonVal["good_id"] = m_iId;
	jsonVal["count"] = m_iVal;
	return jsonVal;
}

DataGift::DataGift()
{
	m_iId = 0;
	m_iCount = 0;
	m_iUsed = 0;
}

bool DataGift::ParseJson(Json::Value jsonVal)
{
	if(jsonVal["gift_id"].isInt())
	{
		PARSEINT(m_iId, jsonVal["gift_id"]);
		PARSEINT(m_iCount, jsonVal["count"]);
		PARSEINT(m_iUsed, jsonVal["used"]);
		return true;
	}
	return false;
}

Json::Value DataGift::SaveJson()
{
	Json::Value jsonVal;
	jsonVal["gift_id"] = m_iId;
	jsonVal["count"] = m_iCount;
	jsonVal["used"] = m_iUsed;
	return jsonVal;
}

ExGiftAttr::ExGiftAttr()
{
	iGiftId = 0;
	strGiftName = "";
	strGiftInfo = "";
	pGiftlist = new DList<DataInt>(true);
}

ExGiftAttr::ExGiftAttr(ExGiftAttr* pExGift)
{
	iGiftId = pExGift->iGiftId;
	strGiftName = pExGift->strGiftName;
	strGiftInfo = pExGift->strGiftInfo;
	pGiftlist = new DList<DataInt>(true);
	for(int i = 0; i < pExGift->pGiftlist->size(); i++)
	{
		DataInt* pCurGood = pExGift->pGiftlist->elementAt(i);
		DataInt* ptrGood = new DataInt();
		ptrGood->m_iId = pCurGood->m_iId;
		ptrGood->m_iVal = pCurGood->m_iVal;
		pGiftlist->addElement(ptrGood);
	}
}

ExGiftAttr::~ExGiftAttr()
{
	SAFE_DELETE(pGiftlist);
}

bool ExGiftAttr::ParseJson(Json::Value jsonVal)
{
	PARSEINT(iGiftId, jsonVal["giftID"]);
	PARSESTRING(strGiftName, jsonVal["giftName"]);
	PARSESTRING(strGiftInfo, jsonVal["remark"]);

	if(!jsonVal["gift_list"].isArray())
	{
		return false;
	}

	Json::Value valList = jsonVal["gift_list"];
	for(unsigned int i = 0; i < valList.size(); i++)
	{
		DataInt* pGift = new DataInt();
		if(pGift->ParseExGift(valList[i]))
		{
			pGiftlist->addElement(pGift);
		}
		else
		{
			pGiftlist->removeAllElements();
			return false;
		}
	}
	return true;
}

Json::Value ExGiftAttr::SaveJson()
{
	Json::Value jsonVal;
	jsonVal["giftID"] = iGiftId;
	jsonVal["giftName"] = strGiftName->getChar();
	jsonVal["remark"] = strGiftInfo->getChar();

	Json::Value goodList(arrayValue);
	for(int i = 0; i < pGiftlist->size(); i++)
	{
		DataInt* pGift = pGiftlist->elementAt(i);
		goodList.append(pGift->SaveExGift());
	}
	jsonVal["gift_list"] = goodList;
	return jsonVal;
}

TrAccountBuy::TrAccountBuy()
{
	m_pPayRecord = new DList<DataInt>(true);
	m_pGiftRecord = new DList<DataGift>(true);
	m_pExGift = new DList<ExGiftAttr>(true);
	m_iGoldNum = 0;
}

TrAccountBuy::~TrAccountBuy()
{
	SAFE_DELETE(m_pPayRecord);
	SAFE_DELETE(m_pGiftRecord);
	SAFE_DELETE(m_pExGift);
}

bool TrAccountBuy::LoadPayRecord()
{
	return false;//TODO
	char* sData = ChessReadFile(FILE_PAY_RECORD, 1);
	if(sData == NULL)
	{
		sData = ChessReadFile(FILE_ACCOUNT_BUY, 1);
		if(sData == NULL)
		{
			return false;
		}
	}

	Reader reader;
	Json::Value jsonVal;
	if(!reader.parse(sData, jsonVal, false))
	{
		ChessFree(sData);
		return false;
	}
	ChessFree(sData);
	return ParseJson(jsonVal);
}

bool TrAccountBuy::SavePayRecord(bool isUpdate)
{
	return false;//TODO
	FastWriter fastWriter;
	std::string out = fastWriter.write(SaveJson());
	return ChessWriteFile(FILE_PAY_RECORD, (char*)out.c_str(), out.size()) ? true : false;
}

bool TrAccountBuy::ParseJson(Json::Value jsonVal, bool isOnlyUpdatePay)
{
	m_iGoldNum = jsonVal["gold_num"].asInt();
	Json::Value buyList = jsonVal["buy_list"];
	if(!buyList.isArray())
	{
		return false;
	}
	m_pPayRecord->removeAllElements();
	for(UInt i = 0; i < buyList.size(); i++)
	{
		DataInt* pPayRec = new DataInt();
		if(pPayRec->ParseJson(buyList[i]))
		{
			m_pPayRecord->addElement(pPayRec);
		}
		else
		{
			m_pPayRecord->removeAllElements();
			return false;
		}
	}

	Json::Value giftList = jsonVal["gift_list"];
	if(!giftList.isArray())
	{
		return false;
	}
	m_pGiftRecord->removeAllElements();
	for(UInt i = 0; i < giftList.size(); i++)
	{
		DataGift* pGift = new DataGift();
		if(pGift->ParseJson(giftList[i]))
		{
			m_pGiftRecord->addElement(pGift);
		}
		else
		{
			m_pGiftRecord->removeAllElements();
			return false;
		}
	}
	return true;
}

Json::Value TrAccountBuy::SaveJson()
{
	Json::Value buyList(arrayValue);
	for(int i = 0; i < m_pPayRecord->size(); i++)
	{
		DataInt* pPayRec = m_pPayRecord->elementAt(i);
		buyList.append(pPayRec->SaveJson());
	}
	Json::Value giftList(arrayValue);
	for(int i = 0; i < m_pGiftRecord->size(); i++)
	{
		DataGift* pGift = m_pGiftRecord->elementAt(i);
		giftList.append(pGift->SaveJson());
	}
	Json::Value jsonVal;
	jsonVal["buy_list"] = buyList;
	jsonVal["gift_list"] = giftList;
	jsonVal["gold_num"] = m_iGoldNum;
	return jsonVal;
}

void TrAccountBuy::GeneratePayRecord(TrAccountBuy* pPayRec)
{
	for(int i = 0; i < pPayRec->GetPaySize(); i++)
	{
		DataInt* payItem = pPayRec->GetPayItem(i);
		DataInt* curPayItem = FindPayItem(payItem->m_iId);
		int iPayVal = payItem->m_iVal;
		if (curPayItem && iPayVal < curPayItem->m_iVal)
		{
			iPayVal = curPayItem->m_iVal;
		}
		SetPayTimes(payItem->m_iId, iPayVal);
	}

	SavePayRecord();
}
DataInt* TrAccountBuy::FindPayItem(int iId)
{
	for(int i = 0; i < m_pPayRecord->size(); i++)
	{
		DataInt* item = m_pPayRecord->elementAt(i);
		if(item->m_iId == iId)
		{
			return item;
		}
	}
	return NULL;
}

void TrAccountBuy::SetGoldNum(int iNum, bool bNeedSave)
{
	m_iGoldNum = iNum;
	SavePayRecord(bNeedSave);
}

bool TrAccountBuy::isBuy(int buyId)
{
	DataInt* item = FindPayItem(buyId);
	if(item == NULL)
	{
		return false;
	}
	return true;
}

void TrAccountBuy::AddPayTimes(int iBuyId, bool bNeedSave, int ybsize)
{
	DataInt* pPayRec = FindPayItem(iBuyId);
	if (pPayRec)
	{
		pPayRec->m_iVal += 1;
		pPayRec->m_YBsize += ybsize;
	}
	else
	{
		pPayRec = new DataInt;
		pPayRec->m_iId = iBuyId;
		pPayRec->m_iVal = 1;
		pPayRec->m_YBsize = ybsize;
		m_pPayRecord->addElement(pPayRec);
	}
	if (bNeedSave)
	{
		SavePayRecord();
	}
}

void TrAccountBuy::SetPayTimes(int iBuyId, int iTimes)
{
	DataInt* pPayRec = FindPayItem(iBuyId);
	if(pPayRec)
	{
		pPayRec->m_iVal = iTimes;
	}
	else
	{
		pPayRec = new DataInt;
		pPayRec->m_iId = iBuyId;
		pPayRec->m_iVal = iTimes;
		m_pPayRecord->addElement(pPayRec);
	}
}

int TrAccountBuy::GetPayTimes(int iBuyId)
{
	DataInt* pPayRec = FindPayItem(iBuyId);
	if (pPayRec)
	{
		return pPayRec->m_iVal;
	}
	return 0;
}

bool TrAccountBuy::LoadExchangeGift()
{
	return false;//TODO
	char* sData = ChessReadFile(FILE_EXCHANGE_GIFT, 1);
	if(sData == NULL)
	{
		return false;
	}

	Reader reader;
	Json::Value jsonVal;
	if(!reader.parse(sData, jsonVal, false))
	{
		ChessFree(sData);
		return false;
	}
	ChessFree(sData);
	return ParseExGift(jsonVal);
}

bool TrAccountBuy::SaveExGift()
{
	return false;//TODO
	FastWriter fastWriter;

	Json::Value exGiftList(arrayValue);
	for(int i = 0; i < m_pExGift->size(); i++)
	{
		ExGiftAttr* pExGift = m_pExGift->elementAt(i);
		exGiftList.append(pExGift->SaveJson());
	}
	std::string out = fastWriter.write(exGiftList);
	return ChessWriteFile(FILE_EXCHANGE_GIFT, (char*)out.c_str(), out.size()) ? true : false;
}

bool TrAccountBuy::ParseExGift(Json::Value jsonValue)
{
	Json::Value line = jsonValue;
	if(!line.isArray())
	{
		return false;
	}

	m_pExGift->removeAllElements();
	for(unsigned int i = 0; i < line.size(); i++)
	{
		ExGiftAttr* pExGift = new ExGiftAttr();
		if(pExGift->ParseJson(line[i]))
		{
			m_pExGift->addElement(pExGift);
		}
		else
		{
			m_pExGift->removeAllElements();
			return false;
		}
	}
	return true;
}

void TrAccountBuy::GenerateExchangeGift(TrAccountBuy* pTrAccount)
{
	m_pExGift->removeAllElements();
	for(int i = 0; i < m_pGiftRecord->size(); i++)
	{
		DataGift* pGift = m_pGiftRecord->elementAt(i);
		pGift->m_iCount = 0;
	}
	for(int i = 0; i < pTrAccount->m_pExGift->size(); i++)
	{
		ExGiftAttr* pCurExGift = pTrAccount->m_pExGift->elementAt(i);
		ExGiftAttr* pExGift = new ExGiftAttr(pCurExGift);
		m_pExGift->addElement(pExGift);
		SetExGiftTimes(pExGift->iGiftId, 1);
	}

	SaveExGift();
	SavePayRecord(false);
}

DataGift* TrAccountBuy::FindExGift(int iGiftId)
{
	for(int i = 0; i < m_pGiftRecord->size(); i++)
	{
		DataGift* pGift = m_pGiftRecord->elementAt(i);
		if(pGift->m_iId == iGiftId)
		{
			return pGift;
		}
	}
	return NULL;
}

void TrAccountBuy::SetExGiftTimes(int iGiftId, int iNum)
{
	DataGift* pGift = FindExGift(iGiftId);
	if(pGift)
	{
		pGift->m_iCount = iNum;
	}
	else
	{
		pGift = new DataGift();
		pGift->m_iId = iGiftId;
		pGift->m_iCount = iNum;
		pGift->m_iUsed = 0;
		m_pGiftRecord->addElement(pGift);
	}
}

int TrAccountBuy::GetExGiftTimes(int iGiftId)
{
	DataGift* pGift = FindExGift(iGiftId);
	if(pGift)
	{
		return pGift->m_iCount;
	}
	return 0;
}

int TrAccountBuy::GetExGiftUsed(int iGiftId)
{
	DataGift* pGift = FindExGift(iGiftId);
	if(pGift)
	{
		return pGift->m_iUsed;
	}
	return 0;
}

void TrAccountBuy::AddExGiftUsed(int iGiftId)
{
	DataGift* pGift = FindExGift(iGiftId);
	if(pGift && !pGift->m_iUsed)
	{
		pGift->m_iUsed = 1;
		SavePayRecord();
	}
}

ExGiftAttr* TrAccountBuy::GetExGiftInfo(int iGiftId)
{
	for(int i = 0; i < m_pExGift->size(); i++)
	{
		ExGiftAttr* pGiftInfo = m_pExGift->elementAt(i);
		if(pGiftInfo->iGiftId == iGiftId)
		{
			return pGiftInfo;
		}
	}
	return NULL;
}

bool TrAccountBuy::IsGoldExGift(int iGiftId)
{
	bool bIsGold = false;
	ExGiftAttr* pGiftInfo = GetExGiftInfo(iGiftId);
	if(pGiftInfo && pGiftInfo->pGiftlist->size() == 1)
	{
		DataInt* pGift = pGiftInfo->pGiftlist->elementAt(0);
		HistoryGoodsPtr ptrGood = new HistoryGoods(pGift->m_iId - 1);
		if(ptrGood->isNormalGood() && ptrGood->getUeId() == 24)
		{
			bIsGold = true;
		}
	}
	return bIsGold;
}

bool TrAccountBuy::IsHavePayRec()
{
	return (m_iGoldNum > 0 || m_pPayRecord->size()>0);
}

Json::Value TrAccountBuy::SavePayRecJson()
{
	Json::Value buyList(arrayValue);
	for (int i = 0; i < m_pPayRecord->size(); i++)
	{
		DataInt* pPayRec = m_pPayRecord->elementAt(i);
		buyList.append(pPayRec->SaveJson());
	}
	Json::Value giftList(arrayValue);
	for (int i = 0; i < m_pGiftRecord->size(); i++)
	{
		DataGift* pGift = m_pGiftRecord->elementAt(i);
		giftList.append(pGift->SaveJson());
	}
	Json::Value jsonVal;
	jsonVal["buy_list"] = buyList;
	jsonVal["gift_list"] = giftList;
	jsonVal["gold_num"] = m_iGoldNum;
	return jsonVal;
}

bool TrAccountBuy::LoadConfig()
{
	return false;//TODO
	char* sData = ChessReadFile(FILE_GAME_CONFIG, 1);
	if(sData == NULL)
	{
		return false;
	}

	Reader reader;
	Json::Value jsonVal;
	if(!reader.parse(sData, jsonVal, false))
	{
		ChessFree(sData);
		return false;
	}
	ChessFree(sData);
	return true;
}