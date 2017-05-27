#include "GetNetDateTool.h"
#include "StatConfig.h"
#include "Defines.h"
#include "CocoMd5.h"
#include "RomanceLayer.h"
#include "LadderRankTableView.h"
#include "ClanApplyListLayer.h"
#include "ClanHomePageLayer.h"
#include "GreatBattleLayer.h"

GetNetDateTool* GetNetDateTool::m_instance = NULL;
GetNetDateTool* GetNetDateTool::getInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new GetNetDateTool();
	}
	return m_instance;
}

void GetNetDateTool::GetMapStarInfo()
{
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* Vdata = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(Vdata->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());
	CCLOG("MapStarInfo:%s", data->getCString());

	ITools::getInstance()->connectingNetwork("getMapInfo",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_GetMapStarInfo");

	ITools::getInstance()->addWaitLayer();

}

__String* GetNetDateTool::getCommData() {
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());
	return data;
}

void GetNetDateTool::setGuideStatusReq(const char* key, int value) {
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("key=%s&value=%d&gameToken=%s",key,value, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"key\":\"%s\",\"value\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", key,value,GameTokenS.c_str(), validateCode.c_str());
	SendReq("setGuideStatus", data);
}

void GetNetDateTool::SendReq(const char*funcName, __String* data) {
	ITools::getInstance()->connectingNetwork(funcName,
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		funcName);

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendSweepDate(int iWarId, int iType, EventDelegate* ptrDelegate)
{
	mDelegate = ptrDelegate;

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("warID=%d&type=%d&gameToken=%s",iWarId,iType,GameTokenS.c_str());

	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"warID\":%d,\"type\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iWarId, iType, GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("warSweep",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendSweepDate");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendRefWarCount(int iWarId, EventDelegate* ptrDelegate)
{
	mDelegate = ptrDelegate;

	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("warID=%d&gameToken=%s", iWarId,GameTokenS.c_str());

	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"warID\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", iWarId,GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("refreshWarCount",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendRefWarCount");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendUpdataWarRefNum(int iWarId, EventDelegate* ptrDelegate)
{
	mDelegate = ptrDelegate;
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();

	__String* validateCs = __String::createWithFormat("gameToken=%s&warID=%d", GameTokenS.c_str(),iWarId);

	string validateCode = md5.ToMD5(validateCs->getCString());
	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"warID\":%d,\"validateCode\":\"%s\"}", GameTokenS.c_str(), iWarId, validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("getWarRefCount",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendUpdataWarRefNum");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendRomanceOpenData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getUserPlotScript",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendRomanceOpenData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendLadderRankData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getExpeditionRank",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendLadderRankData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendWorldRankData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getCPRank",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendWorldRankData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendChongZhiRankData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("getTempNumRank",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendChongZhiRankData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendClanInfoData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildByUser",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendClanInfoData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendClanHomePageData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("curPage=%d&pageSize=%d&gameToken=%s", 1, PAGE_ELEM_NUM, GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"curPage\":%d,\"pageSize\":%d,\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", 1, PAGE_ELEM_NUM, GameTokenS.c_str(), validateCode.c_str());

	ITools::getInstance()->connectingNetwork("selectGuildList",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead,
		"Post_SendClanHomePageData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::SendGreatBattleData()
{
	Json::Value valWarList(arrayValue);
	Json::Int valWarId = trData->net_zmzy_map.begin()->second->warid[0];
	valWarList.append(valWarId);
	FastWriter fastWriter;
	string strWarId = fastWriter.write(valWarList);
	strWarId.erase(strWarId.end() - 1);

	string strToken = history->m_pLoginInfo->getGameToken();
	string strData = StringUtils::format("gameToken=%s&warIDs=%s", strToken.c_str(), strWarId.c_str());
	CocosMD5 md5;
	string validateCode = md5.ToMD5(strData);
	string data = StringUtils::format("{\"gameToken\":\"%s\",\"warIDs\":%s,\"validateCode\":\"%s\"}", strToken.c_str(), strWarId.c_str(), validateCode.c_str());
	CCLOG("%s", data.c_str());

	ITools::getInstance()->connectingNetwork("isOpenGroup",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(GetNetDateTool::onHttpRequestCompleted),
		data,
		ITools::getInstance()->mHead,
		"Post_SendGreatBattleData");

	ITools::getInstance()->addWaitLayer();
}

void GetNetDateTool::onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata)
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
		if (strTag == "Post_GetMapStarInfo")
		{
			if(value["MapInfo"].isArray())
			{
				Json::Value Vmapinfo = value["result"]["MapInfo"];
				history->mMapDateInfo->loadJson(Vmapinfo);
			}
		}
		else if (strTag == "Post_SendSweepDate")
		{
			if(value["result"].isObject())
			{
				Json::Value Vmapinfo = value["result"];
				mDelegate->SendNetDateResult(true, Vmapinfo, SEND_NET_SWEEP);
				if (Vmapinfo["sweepJadeSum"].isInt())
				{
					history->m_pPlayerInfo->setSweepJadeSum(Vmapinfo["sweepJadeSum"].asInt());
				}
			}
		}
		else if (strTag == "Post_SendRefWarCount")
		{
			if(value["result"].isObject())
			{
				Json::Value Vmapinfo = value["result"];
				mDelegate->SendNetDateResult(true, Vmapinfo, SEND_NET_REFRESH);
			}
		}
		else if (strTag == "Post_SendUpdataWarRefNum")
		{
			if (value["result"].isObject())
			{
				Json::Value Vmapinfo = value["result"];
				mDelegate->SendNetDateResult(true, Vmapinfo, SEND_NET_UPDATA_REFNUM);
			}
		}
		else if (strTag == "Post_SendRomanceOpenData")
		{
			if (value["result"].isObject() && value["result"]["maps"].isObject())
			{
				history->mRomanceOpenData.clear();
				Json::Value jsonVal = value["result"]["maps"];

				for (Json::Value::iterator it = jsonVal.begin(); it != jsonVal.end(); it++)
				{
					string MapIdS = it.key().asCString();
					int iMapId = StringToInt(MapIdS.c_str());
					if (jsonVal[MapIdS].isArray())
					{
						Json::Value MapDate = jsonVal[MapIdS];
						for (UInt i = 0; i < MapDate.size(); i++)
						{
							history->mRomanceOpenData[iMapId][MapDate[i].asInt()] = 1;
						}
					}
				}
			}
		}
		else if (strTag == "Post_SendLadderRankData")
		{
			Json::Value Vmapinfo = value["result"];
			ChessApplication::getInstance()->AddFunctionLayer(LadderRankTableView::create(Vmapinfo));
		}
		else if (strTag == "Post_SendWorldRankData")
		{
			Json::Value Vmapinfo = value["result"];
			ChessApplication::getInstance()->AddFunctionLayer(LadderRankTableView::create(Vmapinfo,WORLD_RANK));
		}
		else if (strTag == "Post_SendChongZhiRankData")
		{
			Json::Value Vmapinfo = value["result"];
			ChessApplication::getInstance()->AddFunctionLayer(LadderRankTableView::create(Vmapinfo , TYPE_CHONGZHI , WORLD_RANK));
		}
		else if (strTag == "Post_SendClanInfoData")
		{
			Json::Value Vmapinfo = value["result"];

			if (Vmapinfo["isTrue"].isInt())
			{
				if (Vmapinfo["isTrue"].asInt() != 0)
				{
					if (Vmapinfo["guildName"].isString()) {
						history->m_pPlayerInfo->setGuildName(Vmapinfo["guildName"].asCString());
						history->m_pPlayerInfo->setGuildId(Vmapinfo["num"].asInt());
					}
					ChessApplication::getInstance()->AddFunctionLayer(ClanHomePageLayer::create(Vmapinfo));
				}
				else
				{
					SendClanHomePageData();
				}
			}
		}
		else if (strTag == "Post_SendClanHomePageData")
		{
			if(value["result"]["guildList"].isArray())
				ChessApplication::getInstance()->AddFunctionLayer(ClanApplyListLayer::create(value["result"]));
		}
		else if (strTag == "Post_SendGreatBattleData")
		{
			if (value["result"]["zmzy"].isArray())
			{
				history->mMapDateInfo->GetBattleGreatData()->LoadJsonValue(value["result"]);
				ChessApplication::getInstance()->AddFunctionLayer(GreatBattleLayer::create());
			}
		}
		else if (strTag == "getGuideStatus") {
			auto jsonResult = value["result"];
			if (jsonResult.isObject()) {
				auto jsonVal = jsonResult["guideStatus"];
				history->m_pPlayerInfo->loadGuideStatus(jsonVal);
			}
		}
		else if (strTag == "setGuideStatus") {
			auto jsonResult = value["result"];
			if (jsonResult.isObject()) {
				auto jsonVal = jsonResult["guideStatus"];
				history->m_pPlayerInfo->loadGuideStatus(jsonVal);
			}
		}
	}
}

GetNetDateTool::GetNetDateTool()
{
	
}