//
//  ActivityInfo.cpp
//  TRGame
//
//  Created by Jimmy on 16/06/14.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "ActivityInfo.h"

BTActivity* BTActivity::m_pInstance = NULL;

ActGiftElem::ActGiftElem()
{
	m_iId = 0;
	m_iState = ACT_UNFINISH;
	m_iCondition = 0;
}

void ActGiftElem::loadJson(int iId, Json::Value valElem)
{
	m_iId = iId;
	ParseInt(m_iCondition, valElem["condition"]);

	Json::Value valGoods = valElem["goods"];
	if(valGoods.isArray())
	{
		for(UInt i = 0; i < valGoods.size(); i++)
		{
			int iGoodsId = valGoods[i]["goodID"].asInt();
			int iCount = valGoods[i]["count"].asInt();
			m_mGoods[iGoodsId] = iCount;
		}
	}
}

ActExchElem::ActExchElem()
{
	m_iId = 0;
	m_iState = ACT_UNFINISH;
}

void ActExchElem::loadJson(int iId, Json::Value valElem)
{
	m_iId = iId;
	Json::Value valUse = valElem["original"];
	if(valUse.isArray())
	{
		for(UInt i = 0; i < valUse.size(); i++)
		{
			int iGoodsId = valUse[i]["goodID"].asInt();
			int iCount = valUse[i]["count"].asInt();
			m_mUseGoods[iGoodsId] = iCount;
		}
	}

	Json::Value valGet = valElem["target"];
	if(valGet.isArray())
	{
		for(UInt i = 0; i < valGet.size(); i++)
		{
			int iGoodsId = valGet[i]["goodID"].asInt();
			int iCount = valGet[i]["count"].asInt();
			m_mGetGoods[iGoodsId] = iCount;
		}
	}
}

ActivityElem::ActivityElem()
{
	m_iActId = 0;
	m_iImgId = 0;
	m_iTypeId = 0;
	m_strTitle = "";
	m_strMark = "";
	m_strContent = "";
	m_strSTime = "";
	m_strETime = "";
	m_iRecharge = 0;
	m_iRmbPrice = 0;
	m_iRmbMark = 0;
}

void ActivityElem::loadJson(Json::Value valElem)
{
	ParseInt(m_iActId, valElem["aid"]);
	ParseInt(m_iImgId, valElem["imgID"]);
	ParseInt(m_iTypeId, valElem["type"]);
	ParseString(m_strTitle, valElem["title"]);
	ParseString(m_strMark, valElem["remarks"]);
	ParseString(m_strContent, valElem["content"]);

	DString strTime = valElem["startDate"]["iso"].asCString();
	int iIndex = strTime->indexOf('T');
	strTime = strTime->substring(0, iIndex);
	m_strSTime = strTime->getChar();

	strTime = valElem["endDate"]["iso"].asCString();
	iIndex = strTime->indexOf('T');
	strTime = strTime->substring(0, iIndex);
	m_strETime = strTime->getChar();

	Json::Value valGoods = valElem["goods"];
	for(Json::Value::iterator it = valGoods.begin(); it != valGoods.end(); it++)
	{
		string strItemId = it.key().asCString();
		int iItemId = StringToInt(strItemId.c_str());
		Json::Value valItem = valGoods[strItemId.c_str()];

		switch(m_iTypeId)
		{
		case ACT_LOGIN:
		case ACT_RECHARGE:
		case ACT_LEVEL:
		case ACT_NOTICE:
		case ACT_WXGIFT:
		{
			ActGiftElem* ptrGift = new ActGiftElem();
			ptrGift->loadJson(iItemId, valItem);
			m_mGiftGoods[iItemId] = ptrGift;
		}
		break;
		case ACT_EXCHENGE:
		{
			ActExchElem* ptrChange = new ActExchElem();
			ptrChange->loadJson(iItemId, valItem);
			m_mExchGoods[iItemId] = ptrChange;
		}
		break;
		case ACT_RMBGIFT:
		{
			ActGiftElem* ptrGift = new ActGiftElem();
			ptrGift->loadJson(iItemId, valItem);
			m_mGiftGoods[iItemId] = ptrGift;

			ParseInt(m_iRmbPrice, valItem["money"]);
			ParseInt(m_iRmbMark, valItem["paymark"]);
		}
		break;
		case ACT_EXCHCODE:
			break;
		case ACT_CREDITCARD:
		{
			Json::Value valGifts = valGoods["goods"];
			if(valGifts.isArray())
			{
				for(UInt i = 0; i < valGifts.size(); i++)
				{
					int iGoodsId = valGifts[i]["goodID"].asInt();
					int iCount = valGifts[i]["count"].asInt();
					m_mCardGifts[iGoodsId] = iCount;
				}
			}

			ParseString(m_strUrl1, valGoods["url1"]);
			ParseString(m_strUrl2, valGoods["url2"]);
		}
		break;
		default:
			break;
		}
	}
}

SignGoods::SignGoods()
{
	m_iGoodsId = 0;
	m_iCount = 0;
	m_iIsDouble = 0;
}

void SignGoods::loadJson(Json::Value valElem)
{
	ParseInt(m_iGoodsId, valElem["goodID"]);
	ParseInt(m_iCount, valElem["count"]);
	ParseInt(m_iIsDouble, valElem["isDouble"]);
}

BTActivity::BTActivity()
{

}

bool BTActivity::loadJson(Json::Value valActInfo)
{
	Json::Value valAct = valActInfo["activities"];
	if(valAct.isArray())
	{
		for(UInt i = 0; i < valAct.size(); i++)
		{
			ActivityElem* ptrAct = new ActivityElem();
			ptrAct->loadJson(valAct[i]);
			m_mActivity[ptrAct->getActId()] = ptrAct;
		}
	}
	return true;
}

void BTActivity::clearActivity()
{
	for(map<int, ActivityElem*>::iterator itActivity = m_mActivity.begin(); itActivity != m_mActivity.end(); itActivity++)
	{
		ActivityElem* ptrAct = itActivity->second;
		for(map<int, ActGiftElem*>::iterator itGift = ptrAct->m_mGiftGoods.begin(); itGift != ptrAct->m_mGiftGoods.end(); itGift++)
		{
			SAFE_DELETE(itGift->second);
		}
		ptrAct->m_mGiftGoods.clear();

		for(map<int, ActExchElem*>::iterator itExch = ptrAct->m_mExchGoods.begin(); itExch != ptrAct->m_mExchGoods.end(); itExch++)
		{
			SAFE_DELETE(itExch->second);
		}
		ptrAct->m_mExchGoods.clear();
		ptrAct->m_mCardGifts.clear();

		SAFE_DELETE(itActivity->second);
	}
	m_mActivity.clear();
}

void BTActivity::clearSignInfo()
{
	for (UInt i = 0; i < m_mSignGoods.size(); i++)
	{
		SAFE_DELETE(m_mSignGoods[i]);
	}
	m_mSignGoods.clear();

	m_iMonth = 0;
	m_iTimes = 0;
	m_iIsSign = 0;
	m_iMissTimes = 0;
}

bool BTActivity::isExchInit()
{
	return m_mExchGoods.size() != 0;
}

bool BTActivity::loadExchJson(Json::Value valExch)
{
	Json::Value valArray = valExch["arr"];
	if(valArray.isArray())
	{
		for(UInt i = 0; i < valArray.size(); i++)
		{
			string strType = valArray[i]["type"].asString();
			int iType = EXCH_JJL;
			if (strType == "jjl")
			{
				iType = EXCH_JJL;
			}
			else if (strType == "yxl")
			{
				iType = EXCH_YXL;
			}
			else if (strType == "gll")
			{
				iType = EXCH_GLL;
			}

			Json::Value valGoods = valArray[i]["goods"];
			map<int, ActExchElem*> mExchGoods;
			for(UInt i = 0; i < valGoods.size(); i++)
			{
				Json::Value valItem = valGoods[IntToString(i + 1)];
				ActExchElem* ptrChange = new ActExchElem();
				ptrChange->loadJson(i + 1, valItem);
				mExchGoods[ptrChange->getElemId()] = ptrChange;
			}
			m_mExchGoods[iType] = mExchGoods;
			m_mstrExch[iType] = strType;
		}
	}

	return true;
}

void BTActivity::clearExchInfo()
{
	for(UInt i = 0; i < m_mExchGoods.size(); i++)
	{
		for(UInt j = 0; j < m_mExchGoods[i].size(); j++)
		{
			SAFE_DELETE(m_mExchGoods[i][j]);
		}
		m_mExchGoods[i].clear();
	}
	m_mExchGoods.clear();
	m_mstrExch.clear();
}

int BTActivity::getMaterialId(int iType)
{
	int iGoodsId = 0;
	if(iType == EXCH_JJL)
	{
		iGoodsId = EXCH_GOODS_JJL;
	}
	else if(iType == EXCH_YXL)
	{
		iGoodsId = EXCH_GOODS_YXL;
	}
	else if(iType == EXCH_GLL)
	{
		iGoodsId = EXCH_GOODS_GLL;
	}
	return iGoodsId;
}

std::string BTActivity::getExchType(int iType)
{
	return m_mstrExch[iType];
}

BTActivity* BTActivity::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new BTActivity();
	}
	return m_pInstance;
}

bool BTActivity::init()
{
	clearActivity();
	clearSignInfo();
	clearExchInfo();

	m_pDelegate = nullptr;
	m_pInstance->onHttpRequest(ACT_INIT);
	return true;
}

void BTActivity::updateAllActivity()
{
	for(map<int, ActivityElem*>::iterator it = m_mActivity.begin(); it != m_mActivity.end(); it++)
	{
		updateActById(it->first);
	}
}

void BTActivity::updateActById(int iActId)
{
	Json::Value valActivity = history->m_pPlayerInfo->m_valActivity;

	ActivityElem* ptrActivity = m_mActivity[iActId];
	int iType = ptrActivity->getTypeId();

	if(iType == ACT_RECHARGE)
	{
		if(m_mRefMoney[iActId])
		{
			m_mRefMoney[iActId] = false;
		}
		else
		{
			onHttpRequest(iType, iActId);
			return;
		}
	}

	switch(iType)
	{
	case ACT_LOGIN:
	case ACT_RECHARGE:
	case ACT_LEVEL:
	case ACT_RMBGIFT:
	{
		map<int, int> mCondition;
		mCondition[ACT_LOGIN] = history->m_pPlayerInfo->getLoginNum();
		mCondition[ACT_RECHARGE] = ptrActivity->getRecharge();
		mCondition[ACT_LEVEL] = history->actors[0].GetActorLv();
		mCondition[ACT_RMBGIFT] = history->m_pPlayerInfo->getLoginNum() - 1;

		for(map<int, ActGiftElem*>::iterator itGift = ptrActivity->m_mGiftGoods.begin(); itGift != ptrActivity->m_mGiftGoods.end(); itGift++)
		{
			ActGiftElem* ptrGift = itGift->second;
			if(mCondition[iType] >= ptrGift->getCondition() && ptrGift->getState() == ACT_UNFINISH)
			{
				ptrGift->setState(ACT_FINISH);
			}
		}

		for(Json::Value::iterator it = valActivity.begin(); it != valActivity.end(); it++)
		{
			string strActId = it.key().asCString();
			if(StringToInt(strActId.c_str()) == ptrActivity->getActId())
			{
				Json::Value valActElem = valActivity[strActId.c_str()];
				for(Json::Value::iterator iter = valActElem.begin(); iter != valActElem.end(); iter++)
				{
					string strElemId = iter.key().asCString();
					if(valActElem[strElemId].asInt())
					{
						int iElemId = StringToInt(strElemId.c_str());
						ptrActivity->m_mGiftGoods[iElemId]->setState(ACT_RECEIVED);
					}
				}
			}
		}
	}
		break;
	case ACT_EXCHENGE:
	{
		for(map<int, ActExchElem*>::iterator itExch = ptrActivity->m_mExchGoods.begin(); itExch != ptrActivity->m_mExchGoods.end(); itExch++)
		{
			ActExchElem* ptrChange = itExch->second;
			map<int, int> mUseGoods = ptrChange->m_mUseGoods;
			int iState = ACT_FINISH;
			for(map<int, int>::iterator it = mUseGoods.begin(); it != mUseGoods.end(); it++)
			{
				int iGoodsId = it->first;
				int iCount = it->second;
				HistoryGoods hisGoods(iGoodsId);
				if(hisGoods.getStoreNum() < iCount)
				{
					iState = ACT_UNFINISH;
					break;
				}
			}
			ptrChange->setState(iState);
		}
	}
		break;
	default:
		break;
	}
}

void BTActivity::updateActByType(int iType, EventDelegate* pDelegate /*= nullptr*/)
{
	m_pDelegate = pDelegate;
	for(map<int, ActivityElem*>::iterator it = m_mActivity.begin(); it != m_mActivity.end(); it++)
	{
		if (iType == it->second->getTypeId())
		{
			updateActById(it->first);
		}
	}
}

bool BTActivity::isSignInit()
{
	return m_mSignGoods.size() != 0;
}

bool BTActivity::loadSignJson(Json::Value valSign)
{
	ParseInt(m_iMonth, valSign["month"]);
	ParseInt(m_iTimes, valSign["count"]);
	ParseInt(m_iIsSign, valSign["isSign"]);
	ParseInt(m_iMissTimes, valSign["repairSignCount"]);

	Json::Value valGoods = valSign["content"];
	for(UInt i = 0; i < valGoods.size(); i++)
	{
		Json::Value valItem = valGoods[IntToString(i + 1)];
		SignGoods* ptrSign = new SignGoods();
		ptrSign->loadJson(valItem);
		m_mSignGoods[i] = ptrSign;
	}

	return true;
}

void BTActivity::onHttpRequest(int iReqType, int iActId /*= 1*/)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch(iReqType)
	{
	case ACT_INIT:
	{
		strURL = "getAllActivities";
		string data = StringUtils::format("gameToken=%s", strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "ACT_INIT";
	}
		break;
	case ACT_RECHARGE:
	{
		strURL = "getActivityMoney";
		string data = StringUtils::format("aid=%d&gameToken=%s", iActId, strToken.c_str());
		string validCode = md5.ToMD5(data);
		strData = StringUtils::format("{\"aid\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iActId, strToken.c_str(), validCode.c_str());
		strTag = "ACT_RECHARGE";
	}
		break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(BTActivity::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void BTActivity::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string resData(buffer->begin(), buffer->end());
	//CCLOG("%s...", resData.c_str() + 5000);

	Json::Reader reader;
	Json::Value value;
	if(reader.parse(resData, value) && value.size() > 0)
	{
		onHttpReqSucceed(value["result"], strTag);
	}
}

bool BTActivity::onHttpReqSucceed(Json::Value valRes, string strTag)
{
	if(strTag == "ACT_INIT")
	{
		loadJson(valRes);
		updateAllActivity();
	}
	else if(strTag == "ACT_RECHARGE")
	{
		int iActId = valRes["aid"].asInt();
		int iMoney = valRes["money"].asInt();
		m_mActivity[iActId]->setRecharge(iMoney);
		m_mRefMoney[iActId] = true;
		updateActById(iActId);
		if (m_pDelegate)
		{
			m_pDelegate->updateState();
		}
	}

	return true;
}
