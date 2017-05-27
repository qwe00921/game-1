//
//  PlayerInfo.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/26.
//
//
#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "PlayerInfo.h"

//////////////////////////////////////////////////////////////////////////
HeroElem::HeroElem()
{
	m_iHero = 0;
	m_iExp = 0;
	m_iLv = 0;
	m_iJob = 0;
	m_iFashion1 = 0;
	m_iFashion2 = 0;
	m_iGender = 0;
	m_iQuality = 0;
	m_iGrade = 0;
	m_iIndex = 0;
	m_iSoulBox = 0;
	m_iHeroCP = 0;
}

void HeroElem::loadJson(Json::Value jsonVal)
{
	ParseInt(m_iHero, jsonVal["heroID"]);
	ParseInt(m_iLv, jsonVal["lv"]);
	ParseInt(m_iExp, jsonVal["EXP"]);
	ParseInt(m_iJob, jsonVal["jobID"]);
	ParseInt(m_iFashion1, jsonVal["fashion1"]);
	ParseInt(m_iFashion2, jsonVal["fashion2"]);
	//ParseInt(m_iGender, jsonVal["gender"]);
	ParseInt(m_iQuality, jsonVal["quality"]);
	ParseInt(m_iGrade, jsonVal["grade"]);
	ParseInt(m_iIndex, jsonVal["index"]);
	ParseInt(m_iHeroCP, jsonVal["cp"]);
	ParseDoubleOrInt(m_mPractice[0], jsonVal["practice_wl"]);
	ParseDoubleOrInt(m_mPractice[1], jsonVal["practice_zl"]);
	ParseDoubleOrInt(m_mPractice[2], jsonVal["practice_ts"]);
	ParseDoubleOrInt(m_mPractice[3], jsonVal["practice_mj"]);
	ParseDoubleOrInt(m_mPractice[4], jsonVal["practice_yq"]);

	if (jsonVal["isSoulBox"].isBool() && jsonVal["isSoulBox"].asBool())
	{
		m_iSoulBox = 1;
	}
	if (jsonVal["gender"].isInt())
		m_iGender = jsonVal["gender"].asInt() + 1;

	if (jsonVal["skills"].isObject())
	{
		for (Json::Value::iterator it = jsonVal["skills"].begin(); it != jsonVal["skills"].end(); it++)
		{
			string SkillIdS = it.key().asCString();
			int iSkillId = StringToInt(SkillIdS.c_str());
			int iSkillLv = jsonVal["skills"][SkillIdS]["lv"].asInt();
			mSkillLv[iSkillId] = iSkillLv;
		}
	}

	if (jsonVal["goods_add_attr"].isArray())
	{
		int iGoodsId = 0,iCount = 0;
		Json::Value vGoodAddAttr = jsonVal["goods_add_attr"];

		for (UInt i = 0; i < vGoodAddAttr.size(); i++)
		{
			ParseInt(iGoodsId, vGoodAddAttr[i]["goodID"]);
			ParseInt(iCount, vGoodAddAttr[i]["times"]);
			mSpecialItem[iGoodsId] = iCount;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
GoodsElem::GoodsElem()
{
	m_iNetId = 0;
	m_iGood = 0;
	m_iStarLv = 0;
	m_iHero = 0;
	m_iStone = 0;
}

void GoodsElem::loadJson(Json::Value jsonVal)
{
	m_mProperty.clear();
	ParseInt(m_iNetId, jsonVal["id"]);
	ParseInt(m_iGood, jsonVal["goodID"]);
	ParseInt(m_iStarLv, jsonVal["starLV"]);
	ParseInt(m_iHero, jsonVal["heroID"]);
	ParseInt(m_iStone, jsonVal["suit"]);
	Json::Value property = jsonVal["properties"];
	for(UInt i = 0; i < property.size(); i++)
	{
		m_mProperty[i] = property[i]["propert"].asInt();
	}
}
//////////////////////////////////////////////////////////////////////////
ConsumeElem::ConsumeElem()
{
	m_iNetId = 0;
	m_iGoodId = 0;
	m_iCount = 0;
}

void ConsumeElem::loadJson(Json::Value jsonVal)
{
	ParseInt(m_iNetId, jsonVal["id"]);
	ParseInt(m_iGoodId, jsonVal["goodID"]);
	ParseInt(m_iCount, jsonVal["count"]);
}
//////////////////////////////////////////////////////////////////////////
PlayerInfo::PlayerInfo()
{
	m_iTotalCP = 0;
	m_roleCreateTime = 0;
	reset();//数据重置
}

bool PlayerInfo::loadJson(Json::Value jsonVal)
{
	reset();//数据重置
	loadPlayer(jsonVal);
	loadHero(jsonVal["hero"]);
	loadGoods(jsonVal["good"]);
	loadConsumes(jsonVal["consumes"]);
	loadHeroChip(jsonVal["heroChip"]);
	loadGoodsChip(jsonVal["goodChip"]);
	loadSpecialGoods(jsonVal["special"]);
	loadScriptGoods(jsonVal["script"]);
	loadMapCity(jsonVal["mapCity"]);
	loadMapTreasure(jsonVal["mapTreasures"]);
	loadMapAttackNum(jsonVal["warCount"]);
	loadScriptCode(jsonVal["scriptCodes"]);
	loadGuideStatus(jsonVal["guideStatus"]);


	m_valActivity = jsonVal["activities"];

	//战棋天下VIP
	double iTime1 = jsonVal["vipCard"]["dueTime"].asDouble();
	double iTime2 = jsonVal["vipCard"]["receiveTime"].asDouble();
	double iTime3 = jsonVal["login"]["time"].asDouble();
	m_iVIP = iTime1 > (iTime3 + 100) ? 1 : 0;
	m_iVIPGift = (iTime3 > (iTime2 + 100) && m_iVIP) ? 1 : 0;
	m_dTime = iTime1;

	return true;
}

void PlayerInfo::loadPlayer(Json::Value jsonVal)
{
	ParseInt(m_iRmb6, jsonVal["rmb6"]);
	ParseInt(m_iRmb12, jsonVal["rmb12"]);
	ParseInt(m_iRmb30, jsonVal["rmb30"]);
	ParseInt(m_iRmb98, jsonVal["rmb98"]);
	ParseInt(m_iRmb328, jsonVal["rmb328"]);
	ParseInt(m_iRmb648, jsonVal["rmb648"]);

	ParseInt(m_iGold, jsonVal["gold"]);
	ParseInt(m_iJade, jsonVal["jade"]);
	ParseInt(m_iImprovedStone, jsonVal["improvedStone"]);
	ParseInt(m_iJadeSeal, jsonVal["jadeSeal"]);
	
	ParseInt(m_iSweepJadeSum, jsonVal["warSweepJadeSum"]);

	ParseString(m_strName, jsonVal["name"]);
	ParseInt(m_iHead, jsonVal["head"]);
	ParseInt(m_iVIPExp, jsonVal["VIPExp"]);
	ParseInt(m_iVIPNextExp, jsonVal["VIPNextExp"]);
	ParseInt(m_iLV, jsonVal["lv"]);
	ParseInt(m_iExp, jsonVal["exp"]);
	ParseInt(m_iNextExp, jsonVal["nextExp"]);
	ParseInt(m_iHasUnion, jsonVal["hasUnion"]);
	ParseInt(m_iLastMapID, jsonVal["lastWar"]["mapID"]);
	ParseInt(m_iLastWarID, jsonVal["lastWar"]["warID"]);
	ParseInt(m_iLoginNum, jsonVal["login"]["loginNum"]);

	if (jsonVal["roleCreatedAt"].isIntegral()) {
		m_roleCreateTime = jsonVal["roleCreatedAt"].asUInt();
	}
	
}

void PlayerInfo::loadHero(Json::Value jsonVal)
{
	m_mHero.clear();
	Json::Value heroList = jsonVal;
	if (heroList.isArray())
	{
		for (UInt i = 0; i < heroList.size(); i++)
		{
			HeroElem* ptrHero = new HeroElem();
			ptrHero->loadJson(heroList[i]);
			m_mHero[ptrHero->getHeroID()] = ptrHero;
			m_iTotalCP += ptrHero->getHeroCP();
		}
	}
}

void PlayerInfo::loadGuideStatus(Json::Value jsonVal)
{
	m_guideStatus.clear();
	if (jsonVal.isObject())
	{
		for (Json::Value::iterator it = jsonVal.begin(); it != jsonVal.end(); it++)
		{
			string key = it.key().asCString();
			if (jsonVal[key.c_str()].isInt()) {
				int value = jsonVal[key.c_str()].asInt();
				m_guideStatus.insert(std::make_pair(key, value));
			}

		}
	}
}

void PlayerInfo::setGuideStatus(const char* key, int v) {
	m_guideStatus[key] = v;
}

int PlayerInfo::getGuideStatus(const char* key) {
	return m_guideStatus[key];
}

void PlayerInfo::loadGoods(Json::Value jsonVal)
{
	int iListindex = 0;
	map<int, int> leadGoods;
	m_mGoods.clear();
	Json::Value goodList = jsonVal;
	if (goodList.isArray())
	{
		for (UInt i = 0; i < goodList.size(); i++)
		{
			GoodsElem* ptrGoods = new GoodsElem();
			ptrGoods->loadJson(goodList[i]);
			m_mGoods[i] = ptrGoods;

			if (ptrGoods->getHeroID() == 1)
				leadGoods[iListindex++] = ptrGoods->getGoodID();
		}
	}
	setLeadFashion(leadGoods);
}

void PlayerInfo::loadConsumes(Json::Value jsonVal)
{
	m_mConsume.clear();
	Json::Value consume = jsonVal;
	if (consume.isArray())
	{
		for (UInt i = 0; i < consume.size(); i++)
		{
			ConsumeElem* ptrConsume = new ConsumeElem();
			ptrConsume->loadJson(consume[i]);
			m_mConsume[ptrConsume->getGoodID()] = ptrConsume;
		}
	}
}

void PlayerInfo::loadHeroChip(Json::Value jsonVal)
{
	m_mHeroSoul.clear();
	Json::Value valSoul = jsonVal;
	if (valSoul.isArray())
	{
		for (UInt i = 0; i < valSoul.size(); i++)
		{
			ConsumeElem* ptrSoul = new ConsumeElem();
			ptrSoul->loadJson(valSoul[i]);
			m_mHeroSoul[ptrSoul->getGoodID()] = ptrSoul;
		}
	}
}

void PlayerInfo::loadGoodsChip(Json::Value jsonVal)
{
	m_mEquipChip.clear();
	Json::Value valChip = jsonVal;
	if (valChip.isArray())
	{
		for (UInt i = 0; i < valChip.size(); i++)
		{
			ConsumeElem* ptrChip = new ConsumeElem();
			ptrChip->loadJson(valChip[i]);
			m_mEquipChip[ptrChip->getGoodID()] = ptrChip;
		}
	}
}

void PlayerInfo::loadSpecialGoods(Json::Value jsonVal)
{
	m_mGifts.clear();
	Json::Value valGifts = jsonVal;
	if (valGifts.isArray())
	{
		for (UInt i = 0; i < valGifts.size(); i++)
		{
			ConsumeElem* ptrGifts = new ConsumeElem();
			ptrGifts->loadJson(valGifts[i]);
			m_mGifts[ptrGifts->getGoodID()] = ptrGifts;
		}
	}
}

void PlayerInfo::loadScriptGoods(Json::Value jsonVal)
{
	m_mScriptGoods.clear();
	Json::Value scriptGoods = jsonVal;
	if (scriptGoods.isArray())
	{
		for (UInt i = 0; i < scriptGoods.size(); i++)
		{
			ConsumeElem* ptrScriptGoods = new ConsumeElem();
			ptrScriptGoods->loadJson(scriptGoods[i]);
			m_mScriptGoods[ptrScriptGoods->getGoodID()] = ptrScriptGoods;
		}
	}
}

void PlayerInfo::loadMapCity(Json::Value jsonVal)
{
	m_mMapCity.clear();
	Json::Value mapCityList = jsonVal;
	if (mapCityList.isArray())
	{
		for (UInt i = 0; i < mapCityList.size(); i++)
		{
			m_mMapCity[mapCityList[i].asInt()] = 1;
		}
	}
}

void PlayerInfo::loadMapTreasure(Json::Value jsonVal)
{
	m_MapTreasure.clear();
	if (jsonVal.isObject())
	{
		for (Json::Value::iterator it = jsonVal.begin(); it != jsonVal.end(); it++)
		{
			string MapIdS = it.key().asCString();
			int iMapId = StringToInt(MapIdS.c_str());
			if (jsonVal[MapIdS].isObject())
			{
				Json::Value MapDate = jsonVal[MapIdS];
				for (Json::Value::iterator it = MapDate.begin(); it != MapDate.end(); it++)
				{
					string iStarS = it.key().asCString();
					int iStar = StringToInt(iStarS.c_str());
					int iCount = MapDate[iStarS].asInt();
					m_MapTreasure[iMapId][iStar] = iCount;
				}
			}
		}
	}
}

void PlayerInfo::loadScriptCode(Json::Value jsonVal)
{
	m_mScriptCode.clear();
	if (jsonVal.isObject())
	{
		for (Json::Value::iterator it = jsonVal.begin(); it != jsonVal.end(); it++)
		{
			string MapIdS = it.key().asCString();
			int iMapId = StringToInt(MapIdS.c_str());
			if (jsonVal[MapIdS].isArray())
			{
				Json::Value MapDate = jsonVal[MapIdS];
				for (UInt i = 0; i < MapDate.size(); i++)
				{
					m_mScriptCode[iMapId][MapDate[i].asInt()] = 1;
				}
			}
		}
	}
}

void PlayerInfo::loadMapAttackNum(Json::Value jsonVal)
{
	m_mMapAttackNum.clear();
	if (jsonVal.isObject())
	{
		for (Json::Value::iterator it = jsonVal.begin(); it != jsonVal.end(); it++)
		{
			string WarIdS = it.key().asCString();
			int iWarId = StringToInt(WarIdS.c_str());
			if (jsonVal[WarIdS]["num"].isInt())
			{
				m_mMapAttackNum[iWarId][0] = jsonVal[WarIdS]["num"].asInt();
			}
			else
				m_mMapAttackNum[iWarId][0] = 0;
			if (jsonVal[WarIdS]["refNum"].isInt())
			{
				m_mMapAttackNum[iWarId][1] = jsonVal[WarIdS]["refNum"].asInt();
			}
			else
				m_mMapAttackNum[iWarId][1] = 0;
		}
	}
}

HeroElem* PlayerInfo::getMainHero()
{
	HeroElem* ptrHero = NULL;
	if (m_mHero.find(1) != m_mHero.end())
		ptrHero = m_mHero[1];
	return ptrHero;
}

void PlayerInfo::setBuyJadeState(int iPrice)
{
	if (iPrice == 6)
	{
		m_iRmb6++;
	}
	else if (iPrice == 12)
	{
		m_iRmb12++;
	}
	else if (iPrice == 30)
	{
		m_iRmb30++;
	}
	else if (iPrice == 98)
	{
		m_iRmb98++;
	}
	else if (iPrice == 328)
	{
		m_iRmb328++;
	}
	else if (iPrice == 648)
	{
		m_iRmb648++;
	}
}

void PlayerInfo::reset()
{
	m_iRmb6 = 0;
	m_iRmb12 = 0;
	m_iRmb30 = 0;
	m_iRmb98 = 0;
	m_iRmb328 = 0;
	m_iRmb648 = 0;
	m_iSweepJadeSum = 0;

	m_iGold = 0;
	m_iJade = 0;
	m_iImprovedStone = 0;
	m_iJadeSeal = 0;
	

	m_iVIP = 0;
	m_iVIPGift = 0;
	m_dTime = 0;

	m_strName = "";
	m_iHead = 0;
	m_iVIPExp = 0;
	m_iVIPNextExp = 0;
	m_iLV = 0;
	m_iExp = 0;
	m_iNextExp = 0;
	m_iHasUnion = 0;
	m_iLastMapID = 0;
	m_iLastWarID = 0;
	m_iLoginNum = 0;

	m_LeadFashion = 0;
	m_LeadWapFashion = 0;
	m_iTotalCP = 0;
	m_guildId = 0;
	m_guildName = "";
	m_patriarch = "";
}

void PlayerInfo::setLeadFashion(map<int, int> eGoodsList)
{
	for (size_t i = 0; i < eGoodsList.size(); i++)
	{
		GoodsItem* ptrGoods = trData->m_mGoodsItem[eGoodsList[i]];
		if (ptrGoods->isFashion())
			m_LeadFashion = ptrGoods->iGoodsId;
		else if(ptrGoods->isWapFashion())
			m_LeadWapFashion = ptrGoods->iGoodsId;
	}
}

int PlayerInfo::getBuyJadeState(int iPrice)
{
	int iTimes = 0;
	switch(iPrice)
	{
	case 6:
		iTimes = m_iRmb6;
		break;
	case 12:
		iTimes = m_iRmb12;
		break;
	case 30:
		iTimes = m_iRmb30;
		break;
	case 98:
		iTimes = m_iRmb98;
		break;
	case 328:
		iTimes = m_iRmb328;
		break;
	case 648:
		iTimes = m_iRmb648;
		break;
	default:
		break;
	}
	return iTimes;
}
