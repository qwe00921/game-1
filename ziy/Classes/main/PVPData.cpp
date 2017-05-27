//
//  PVPData.cpp
//  TRGame
//
//  Created by Jimmy on 17/03/21.
//
//

#include "PVPData.h"
#include "PVPHomeLayer.h"
#include "PVPArrayLayer.h"

#define FILE_PVP_ARRAY "pvpArray.dat"

enum 
{
	REQ_INIT_DATA = 0, //初始化数据
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PVPArrayItem::loadJson(Json::Value jsonVal)
{
	ParseInt(iPointId, jsonVal["pointid"]);
	ParseInt(iOrderId, jsonVal["orderid"]);
	ParseInt(iActorId, jsonVal["hid"]);
}

Json::Value PVPArrayItem::saveJson()
{
	Json::Value jsonVal;
	jsonVal["pointid"] = iPointId;
	jsonVal["orderid"] = iOrderId;
	jsonVal["hid"] = iActorId;
	return jsonVal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPArrayInfo::PVPArrayInfo()
{
	iArrayId = 0;
	bIsInit = false;
}

PVPArrayInfo::~PVPArrayInfo()
{
	clearArray();
}

void PVPArrayInfo::loadJson(Json::Value jsonVal)
{
	clearArray();

	ParseInt(iArrayId, jsonVal["arrayid"]);
	Json::Value jsonArray = jsonVal["item"];
	if (jsonArray.isArray())
	{
		bIsInit = jsonArray.size() != 0;
		for (UInt i = 0; i < jsonArray.size(); i++)
		{
			PVPArrayItem* ptrArray = new PVPArrayItem();
			ptrArray->loadJson(jsonArray[i]);
			m_mArray[ptrArray->getPointID()] = ptrArray;
		}
	}
}

Json::Value PVPArrayInfo::saveJson()
{
	Json::Value jsonArray(arrayValue);
	for (UInt i = 0; i < m_mArray.size(); i++)
	{
		jsonArray.append(m_mArray[i]->saveJson());
	}

	Json::Value jsonVal;
	jsonVal["arrayid"] = iArrayId;
	jsonVal["item"] = jsonArray;
	return jsonVal;
}

void PVPArrayInfo::updateArray(PVPArrayInfo* ptrArray)
{
	bIsInit = ptrArray->bIsInit;
	iArrayId = ptrArray->iArrayId;
	for (UInt i = 0; i < ptrArray->m_mArray.size(); i++)
	{
		PVPArrayItem* ptrItem = ptrArray->m_mArray[i];
		if (!m_mArray[i])
		{
			m_mArray[i] = new PVPArrayItem();
		}
		m_mArray[i]->setPointID(ptrItem->getPointID());
		m_mArray[i]->setOrderID(ptrItem->getOrderID());
		m_mArray[i]->setActorID(ptrItem->getActorID());
	}
}

void PVPArrayInfo::clearArray()
{
	bIsInit = false;

	for (UInt i = 0; i < m_mArray.size(); i++)
	{
		SAFE_DELETE(m_mArray[i]);
	}
	m_mArray.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPActorData::PVPActorData()
{
	m_pDefend = nullptr;
}

PVPActorData::~PVPActorData()
{
	clearData();
}

void PVPActorData::loadJson(Json::Value jsonVal)
{
	clearData();

	int iGender = 0;
	ParseInt(iGender, jsonVal["gender"]);
	m_iGender = iGender + 1;
	ParseInt(m_iFashion1, jsonVal["fashion1"]);
	ParseInt(m_iFashion2, jsonVal["fashion2"]);
	ParseInt(m_iRank, jsonVal["ranking"]);
	ParseInt(m_iWinNum, jsonVal["winNum"]);
	ParseInt(m_iLoseNum, jsonVal["loseNum"]);
	int iIsVip = 0;
	ParseInt(iIsVip, jsonVal["isVip"]);
	m_bIsVIP = iIsVip ? true : false;
	ParseString(m_strName, jsonVal["nickname"]);

	Json::Value heroList = jsonVal["heroes"];
	if (heroList.isArray())
	{
		for (UInt i = 0; i < heroList.size(); i++)
		{
			HeroElem* ptrHero = new HeroElem();
			ptrHero->loadJson(heroList[i]);
			m_mHero[ptrHero->getHeroID()] = ptrHero;
		}
	}

	Json::Value goodList = jsonVal["goods"];
	if (goodList.isArray())
	{
		for (UInt i = 0; i < goodList.size(); i++)
		{
			GoodsElem* ptrGoods = new GoodsElem();
			ptrGoods->loadJson(goodList[i]);
			m_mGoods[i] = ptrGoods;
		}
	}

	m_pDefend = new PVPArrayInfo();
	m_pDefend->loadJson(jsonVal["defLineup"]);

	initFashion();
}

void PVPActorData::initFashion()
{
	for (UInt i = 0; i < m_mGoods.size(); i++)
	{
		GoodsElem* goodsElem = m_mGoods[i];
		int iGoodsId = goodsElem->getGoodID();
		if (trData->m_mGoodsItem.count(iGoodsId) > 0)
		{
			int iHeroId = goodsElem->getHeroID();
			if (m_mHero.count(iHeroId))
			{
				HistoryGoods hisGoods(goodsElem);
				if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
				{
					m_mHero[iHeroId]->setFashion1(hisGoods.good_id);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
				{
					m_mHero[iHeroId]->setFashion2(hisGoods.good_id);
				}
			}
		}
	}
}

void PVPActorData::initHeroList()
{
	int iIndex = 0;
	for (map<int, HeroElem*>::iterator it = m_mHero.begin(); it != m_mHero.end(); it++, iIndex++)
	{
		int iHeroId = it->first;
		HeroElem* ptrHero = it->second;

		int iMirrorId = PVP_ACTOR_START_ID + iIndex;
		m_mMirrorId[iHeroId] = iMirrorId;

		//初始化字典表
		trData->CopyActor2toActor1(&trData->actors[iMirrorId], &trData->actors[iHeroId - 1]);
		memset(trData->actors[iMirrorId].actor_name, 0, sizeof(trData->actors[iMirrorId].actor_name));
		if (iHeroId == 1)
		{
			strcpy(trData->actors[iMirrorId].actor_name, m_strName.c_str());
		}
		else
		{
			strcpy(trData->actors[iMirrorId].actor_name, trData->actors[iHeroId - 1].actor_name);
		}

		HistoryActor* ptrActor = &history->actors[iMirrorId];
		ptrActor->init(iMirrorId);

		ActorJoinOwnDate NetActorDate;
		NetActorDate.setActorId(iMirrorId + 1);
		NetActorDate.setExp(ptrHero->getEXP());
		NetActorDate.setLv(ptrHero->getLV());
		NetActorDate.setJobID(ptrHero->getJobID());
		NetActorDate.setFashion1(ptrHero->getFashion1());
		NetActorDate.setFashion2(ptrHero->getFashion2());
		NetActorDate.setQuality(ptrHero->getQuality());
		NetActorDate.setGrade(ptrHero->getGrade());
		NetActorDate.setGender(ptrHero->getGender());
		NetActorDate.setSoulBox(ptrHero->geteSoulBox());
		NetActorDate.mSkillLv = ptrHero->mSkillLv;
		NetActorDate.mSpecialItem = ptrHero->mSpecialItem;
		NetActorDate.m_mPractice = ptrHero->m_mPractice;
		history->NewActorJoinOwn(&NetActorDate, false);
	}
	initGoodsList();
}

void PVPActorData::initGoodsList()
{
	for (UInt i = 0; i < m_mGoods.size(); i++)
	{
		GoodsElem* goodsElem = m_mGoods[i];
		int iGoodsId = goodsElem->getGoodID();
		if (trData->m_mGoodsItem.count(iGoodsId) > 0)
		{
			int iHeroId = goodsElem->getHeroID();
			if (m_mHero.count(iHeroId))
			{
				HistoryGoods hisGoods(goodsElem);
				HistoryActor* ptrActor = &history->actors[m_mMirrorId[iHeroId]];
				if (hisGoods.GetGoodsType() == GOOD_TYPE_WEAPON)
				{
					ptrActor->weapon.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_COAT)
				{
					ptrActor->coat.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_ASSIST)
				{
					ptrActor->assist_good.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_FASHING)
				{
					ptrActor->fashion.setGood(&hisGoods);
				}
				else  if (hisGoods.GetGoodsType() == GOOD_TYPE_WAP_FASHING)
				{
					ptrActor->wap_fashion.setGood(&hisGoods);
				}
				ptrActor->UpdataGoodEffect();
			}
		}
	}
}

void PVPActorData::clearData()
{
	for (map<int, HeroElem*>::iterator it = m_mHero.begin(); it != m_mHero.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mHero.clear();

	for (map<int, GoodsElem*>::iterator it = m_mGoods.begin(); it != m_mGoods.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mGoods.clear();

	SAFE_DELETE(m_pDefend);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PVPData* PVPData::m_pInstance = nullptr;

PVPData* PVPData::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new PVPData();
	}
	return m_pInstance;
}

PVPData::PVPData()
{
	m_iRankId = 0;
	m_iEntry = 0;
	m_iMax = 0;
	m_iMapId = 0;
	m_iSelectRank = 0;
	m_iMidIndex = 0;
	m_strMark = "";
	m_mArrayInfo[ARRAY_ATTACK] = new PVPArrayInfo();
	m_mArrayInfo[ARRAY_DEFEND] = new PVPArrayInfo();
}

PVPData::~PVPData()
{
	SAFE_DELETE(m_mArrayInfo[ARRAY_ATTACK]);
	SAFE_DELETE(m_mArrayInfo[ARRAY_DEFEND]);
	clearData();
}

void PVPData::init()
{
	m_mArrayInfo[ARRAY_ATTACK]->clearArray();
	clearData();

	m_iMidIndex = 8;
	loadAtkJson();
}

void PVPData::startPVP()
{
	onHttpRequest(REQ_INIT_DATA);
}

void PVPData::loadJson(Json::Value jsonVal)
{
	clearData();

	ParseInt(m_iRankId, jsonVal["ranking"]);
	ParseInt(m_iEntry, jsonVal["playNum"]);
	ParseInt(m_iMax, jsonVal["maxPlayNum"]);

	m_mArrayInfo[ARRAY_DEFEND]->loadJson(jsonVal["defLineup"]);

	Json::Value valArray = jsonVal["arr"];
	if (valArray.isArray())
	{
		for (UInt i = 0; i < valArray.size(); i++)
		{
			PVPActorData* ptrActor = new PVPActorData();
			ptrActor->loadJson(valArray[i]);
			m_mPVPActor[ptrActor->getRankID()] = ptrActor;
		}
	}
}

void PVPData::clearData()
{
	m_iRankId = 0;
	m_iEntry = 0;
	m_iMax = 0;
	m_iMapId = 0;
	m_iSelectRank = 0;
	m_strMark = "";

	m_mArrayInfo[ARRAY_DEFEND]->clearArray();

	for (map<int, PVPActorData*>::iterator it = m_mPVPActor.begin(); it != m_mPVPActor.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
	m_mPVPActor.clear();
}

int PVPData::getPVPWarId()
{
	int iWarId = 0;
	for (map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.begin(); it != trData->m_mNetWarInfo.end(); it++)
	{
		NetWarInfo* ptrInfo = it->second;
		if (ptrInfo->iType == BATTLE_PVP)
		{
			iWarId = it->first;
			break;
		}
	}
	return iWarId;
}

void PVPData::loadAtkJson()
{
	m_mJsAttack.clear();

	Reader reader;
	Json::Value jsonVal;
	char* chDate = ChessReadFile(FILE_PVP_ARRAY, 1);
	if (chDate && reader.parse(chDate, jsonVal, false))
	{
		if (jsonVal.isArray())
		{
			for (UInt i = 0; i < jsonVal.size(); i++)
			{
				int iUserId = jsonVal[i]["userid"].asInt();
				m_mJsAttack[iUserId] = jsonVal[i]["array"];
			}
		}

		int iUserId = history->m_pLoginInfo->getMarkCode();
		m_mArrayInfo[ARRAY_ATTACK]->loadJson(m_mJsAttack[iUserId]);
	}
	ChessFree(chDate);
}

void PVPData::updateAtcJson(PVPArrayInfo* ptrArray)
{
	m_mArrayInfo[ARRAY_ATTACK]->updateArray(ptrArray);

	int iUserId = history->m_pLoginInfo->getMarkCode();
	m_mJsAttack[iUserId] = m_mArrayInfo[ARRAY_ATTACK]->saveJson();

	Json::Value jsWriter;
	for (map<int, Json::Value>::iterator it = m_mJsAttack.begin(); it != m_mJsAttack.end(); it++)
	{
		Json::Value jsItem;
		jsItem["userid"] = it->first;
		jsItem["array"] = it->second;
		jsWriter.append(jsItem);
	}

	FastWriter fastWriter;
	std::string strWriter = fastWriter.write(jsWriter);
	ChessWriteFile(FILE_PVP_ARRAY, (char*)strWriter.c_str(), strWriter.size());
}

void PVPData::onHttpRequest(int iReqType)
{
	string strToken = history->m_pLoginInfo->getGameToken();
	string strURL = "";
	string strData = "";
	string strTag = "";
	CocosMD5 md5;

	switch (iReqType)
	{
	case REQ_INIT_DATA:
	{
		strURL = "chooseData";
		string validCode = md5.ToMD5(StringUtils::format("gameToken=%s", strToken.c_str()));
		strData = StringUtils::format("{\"gameToken\":\"%s\",\"validateCode\":\"%s\"}", strToken.c_str(), validCode.c_str());
		strTag = "REQ_INIT_DATA";
	}
	break;
	default:
		break;
	}

	CCLOG("%s", strData.c_str());
	ITools::getInstance()->connectingNetwork(strURL.c_str(), HttpRequest::Type::POST, this, callfuncND_selector(PVPData::onHttpRequestCompleted), strData, ITools::getInstance()->mHead, strTag.c_str());

	ITools::getInstance()->addWaitLayer();
}

void PVPData::onHttpRequestCompleted(Node *sender, void *resultdata)
{
	ITools::getInstance()->removeWaitLayer();

	HttpResponse* response = (HttpResponse*)resultdata;
	string strTag = response->getHttpRequest()->getTag();
	std::vector<char>* buffer = response->getResponseData();
	std::string temp(buffer->begin(), buffer->end());
	string responseData = temp;
	CCLOG("%s...", responseData.c_str());

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(responseData, value) && value.size() > 0)
	{
		if (strTag == "REQ_INIT_DATA")
		{
			onInitDataSucceed(value["result"]);
		}
	}
}

void PVPData::onInitDataSucceed(Json::Value valRes)
{
	if (valRes["isTrue"].isInt() && valRes["isTrue"].asInt())
	{
		loadJson(valRes);
		ChessApplication::getInstance()->AddFunctionLayer(PVPHomeLayer::create());
	}
	else
	{
		ITools::getInstance()->createGlobalPrompt(LL("mustsetdefend"), false);
		ChessApplication::getInstance()->AddFunctionLayer(PVPArrayLayer::create(ARRAY_DEFEND, false));
	}
}
