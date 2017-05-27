#include "BattleLadder.h"
#include "Defines.h"
#include "LadderLayer.h"


BattleLadder* BattleLadder::m_instance = NULL;
BattleLadder* BattleLadder::getInstance(){
	if (m_instance == NULL){
		m_instance = new BattleLadder();
	}
	return m_instance;
}

void BattleLadder::ClearAllData()
{
	m_iCurWarId = 0;
	m_mActorName.clear();

	ClearHeroData();
}

void BattleLadder::ClearHeroData()
{
	for (map<int, map<int, LadderElemData*> >::iterator mIt = m_mHeroData.begin(); mIt != m_mHeroData.end(); mIt++)
	{
		for (map<int, LadderElemData*>::iterator iIt = mIt->second.begin(); iIt != mIt->second.end(); iIt++)
		{
			if (iIt->second)
				delete iIt->second;
		}
	}
	m_mHeroData.clear();
	mOpenWar.clear();
	mBattleData.clear();
}

map<int, int> BattleLadder::GetLadderLevelData()
{
	return mBattleData;
}

void BattleLadder::SetLadderLevelData(int iWarId)
{
	mBattleData[iWarId] = 1;
}

void BattleLadder::LoadBattleLadderData(Json::Value NetJsonValue)
{
	if (NetJsonValue["data"].isObject())
	{

		Json::Value jsonValue = NetJsonValue["data"];
		ClearHeroData();
		for (Json::Value::iterator it = jsonValue.begin(); it != jsonValue.end(); it++)
		{
			string WarIdS = it.key().asCString();
			int iWarId = StringToInt(WarIdS.c_str());
			if (jsonValue[WarIdS].isArray())
			{
				Json::Value MapDate = jsonValue[WarIdS];
				for (UInt i = 0; i < MapDate.size(); i++)
				{
					Json::Value ActorData = MapDate[i];

					LadderElemData* ptrLadderElem = new LadderElemData();

					if (ActorData["name"].isString())
						ptrLadderElem->mName = ActorData["name"].asString();

					if (ActorData["hero"].isArray())
					{
						for (UInt j = 0; j < ActorData["hero"].size(); j++)
						{
							HeroElem* ptrHElem = new HeroElem();
							ptrHElem->loadJson(ActorData["hero"][j]);
							ptrLadderElem->mHeroElem[ptrHElem->geteIndex()] = ptrHElem;
						}
					}

					if (ActorData["goods"].isArray())
					{
						for (UInt j = 0; j < ActorData["goods"].size(); j++)
						{
							GoodsElem* ptrGElem = new GoodsElem();
							ptrGElem->loadJson(ActorData["goods"][j]);
							ptrLadderElem->mGoodsItem[j] = ptrGElem;
						}
					}

					m_mHeroData[iWarId][i] = ptrLadderElem;
				}
			}
		}

		Json::Value jsonOpenValue = NetJsonValue["openWar"];
		if (jsonOpenValue.isArray())
		{
			for (size_t j = 0; j < jsonOpenValue.size(); j++)
			{
				mOpenWar[jsonOpenValue[j].asInt()] = 1;
			}
		}

		Json::Value jsonBattleValue = NetJsonValue["expedition"];
		if (jsonBattleValue.isArray())
		{
			for (size_t j = 0; j < jsonBattleValue.size(); j++)
			{
				mBattleData[jsonBattleValue[j].asInt()] = 1;
			}
		}
	}
}

bool BattleLadder::isAllStar(int iWarId)
{
	if (iWarId >= 10000 && (iWarId - 10000) % 2 == 1)
		return true;
	else
		return false;
}

HeroElem* BattleLadder::getIndexHeroElem(int iWarId, int iIndex,int& iElemIndex)
{
	HeroElem* pRet = NULL;

	if (m_mHeroData.count(iWarId))
	{
		for (size_t i = 0; i < m_mHeroData[iWarId].size(); i++)
		{
			LadderElemData* ptrLadderElemData = m_mHeroData[iWarId][i];
			if (ptrLadderElemData->mHeroElem.count(iIndex))
			{
				pRet = ptrLadderElemData->mHeroElem[iIndex];
				iElemIndex = i;
				break;
			}
		}
	}
	return pRet;
}

map<int, int> BattleLadder::GetLadderOpenWarData()
{
	return mOpenWar;
}

map<int, map<int, LadderElemData*> >* BattleLadder::GetLadderHeroData()
{
	return &m_mHeroData;
}

string BattleLadder::GetBattleActorName(int iActorId, int iType)
{
	if (m_mActorName.count(iActorId))
		return m_mActorName[iActorId][iType];
	else
		return "";
}
void BattleLadder::SetBattleActorName(int iActorId, string NameS, int iType)
{
	m_mActorName[iActorId][iType] = NameS;
}
void BattleLadder::ClearBattleActorName()
{
	m_mActorName.clear();
}

BattleLadder::BattleLadder()
{
	m_iCurWarId = 0;
}

BattleLadder::~BattleLadder()
{

}

void BattleLadder::SendNetData()
{
	CocosMD5 md5;
	string GameTokenS = history->m_pLoginInfo->getGameToken();
	__String* validateCs = __String::createWithFormat("gameToken=%s", GameTokenS.c_str());
	string validateCode = md5.ToMD5(validateCs->getCString());

	__String* data = __String::createWithFormat("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", GameTokenS.c_str(), validateCode.c_str());
	CCLOG("%s", data->getCString());

	ITools::getInstance()->connectingNetwork("getExpeditionData",
		HttpRequest::Type::POST,
		this,
		callfuncND_selector(BattleLadder::onHttpRequestCompleted),
		data->getCString(),
		ITools::getInstance()->mHead);

	ITools::getInstance()->addWaitLayer();
}

void BattleLadder::onHttpRequestCompleted(cocos2d::Node *sender, void* resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	__String* responseData = __String::create(temp);
	CCLOG("onHttpRequestCompleted%s", responseData->getCString());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData->getCString(), value) && value.size() > 0)
	{
		if (value["result"].isObject())
		{
			LoadBattleLadderData(value["result"]);

			ChessApplication::getInstance()->AddFunctionLayer(LadderLayer::create());
		}
	}
}