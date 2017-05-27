#include "MapDateInfo.h"
#include "Defines.h"
#include "main.h"
#include "PopLayer.h"

void MapDateElem::loadJson(Json::Value jsonVal)
{
	ParseInt(mMapId, jsonVal["mapID"]);
	ParseInt(mWarId, jsonVal["warID"]);
	ParseInt(mStar, jsonVal["star"]);
	ParseInt(mScore, jsonVal["score"]);
}

void MapDateInfo::loadJson(Json::Value JsonDate)
{
	clearMapDate();

	history->mArmyLevel = 1;
	if (JsonDate.isArray())
	{
		for (UInt i = 0; i < JsonDate.size(); i++)
		{
			MapDateElem* ptrMDE = new MapDateElem();
			ptrMDE->loadJson(JsonDate[i]);
			int iWarId = ptrMDE->getWARID();
			if (trData->m_mNetWarInfo.count(iWarId) && trData->m_mNetWarInfo[iWarId]->iType == BATTLE_SIXWAR)
			{
				m_MapInfo[ptrMDE->getWARID()] = ptrMDE;

				if (trData->m_mNetWarInfo[iWarId]->iModel == NORMAL_MODEL&&mMaxWarid <= ptrMDE->getWARID())
					mMaxWarid = ptrMDE->getWARID();
				if (trData->m_mNetWarInfo[iWarId]->iModel == DISSICULT_MODEL&&mJyMaxWarid <= ptrMDE->getWARID())
					mJyMaxWarid = ptrMDE->getWARID();
				history->mArmyLevel += ptrMDE->getSTAR();
			}
			else
			{
				m_HeroicMapInfo[ptrMDE->getWARID()] = ptrMDE;
			}

			if (history->m_pPlayerInfo->m_mMapAttackNum.count(iWarId) > 0)
			{
				ptrMDE->setATTACKNUM(history->m_pPlayerInfo->m_mMapAttackNum[iWarId][0]);
				ptrMDE->setRefNum(history->m_pPlayerInfo->m_mMapAttackNum[iWarId][1]);
			}
		}
	}
	if (history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
}

MapDateElem* MapDateInfo::getMapDateElem(int iWarId)
{
	if (m_MapInfo.find(iWarId) != m_MapInfo.end())
		return m_MapInfo[iWarId];
	else
		return NULL;
}

MapDateElem* MapDateInfo::getHeroicMapDateElem(int iWarId)
{
	if (m_HeroicMapInfo.count(iWarId)>0)
		return m_HeroicMapInfo[iWarId];
	else
		return NULL;
}

int MapDateInfo::getMaxWarId(int iModel)
{
	if (iModel == DISSICULT_MODEL)
		return mJyMaxWarid;
	else
		return mMaxWarid;
}

int MapDateInfo::getNextWarId(int iModel)
{
	if (iModel == NORMAL_MODEL)
	{
		if (mMaxWarid < 1)
		{
			return trData->m_mNetWarInfo.begin()->first;
		}

		map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.find(mMaxWarid);
		if ((++it)->second->iType != BATTLE_SIXWAR || it->second->iModel == DISSICULT_MODEL)
		{
			it--;
		}
		return it->first;
	}
	else
	{
		if (mJyMaxWarid < 1)
		{
			for (map<int, NetMapNamePtr>::iterator iIt = trData->net_map_name.begin(); iIt != trData->net_map_name.end(); iIt++)
			{
				if (iIt->second->mModel == iModel&&trData->map_total_war.count(iIt->first))
				{
					return trData->map_total_war[iIt->first].begin()->first;
				}
			}

			return trData->m_mNetWarInfo.begin()->first;
		}

		map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.find(mJyMaxWarid);
		if ((++it)->second->iType != BATTLE_SIXWAR || it->second->iModel == NORMAL_MODEL)
		{
			it--;
		}
		return it->first;
	}

}

int MapDateInfo::getCurWarId(int iModel)
{
	if (iModel == DISSICULT_MODEL)
		return mJyCurWarid;
	else
		return mCurWarid;
}

void MapDateInfo::setMaxWarId(int iWarId)
{
	if (trData->m_mNetWarInfo[iWarId]->iModel == DISSICULT_MODEL&&iWarId > mJyMaxWarid)
	{
		mJyCurWarid = 0;
		mJyMaxWarid = iWarId;
	}
	else if (trData->m_mNetWarInfo[iWarId]->iModel == NORMAL_MODEL && iWarId > mMaxWarid)
	{
		mCurWarid = 0;
		mMaxWarid = iWarId;
	}
}

void MapDateInfo::setCurWarId(int iWarId)
{
	if (trData->m_mNetWarInfo[iWarId]->iModel == DISSICULT_MODEL)
		mJyCurWarid = iWarId;
	else
		mCurWarid = iWarId;
}

MapDateInfo::MapDateInfo()
{
	mMaxWarid = 0;
	mCurWarid = 0;
	mJyMaxWarid = 0;
	mJyCurWarid = 0;
}
MapDateInfo::~MapDateInfo()
{

}

void MapDateInfo::clearMapDate()
{
	for (map<int, MapDateElem*>::iterator mdeIt = m_MapInfo.begin(); mdeIt != m_MapInfo.end(); mdeIt++)
	{
		if (mdeIt->second != NULL)
			delete mdeIt->second;
	}
	for (map<int, MapDateElem*>::iterator mdeIt = m_HeroicMapInfo.begin(); mdeIt != m_HeroicMapInfo.end(); mdeIt++)
	{
		if (mdeIt->second != NULL)
			delete mdeIt->second;
	}
	m_MapInfo.clear();
	m_HeroicMapInfo.clear();
	mMaxWarid = 0;
	mCurWarid = 0;
	mJyMaxWarid = 0;
	mJyCurWarid = 0;
}

void MapDateInfo::AddBattleWinDate(int iMapid, int iWarid, int iStar, int iScore,int iBattleNum)
{
	setMaxWarId(iWarid);

	if (m_MapInfo.find(iWarid) != m_MapInfo.end())
	{
		int iOldStar = m_MapInfo[iWarid]->getSTAR();
		int iOldScore = m_MapInfo[iWarid]->getSCORE();

		if (iStar > iOldStar)
		{
			m_MapInfo[iWarid]->setSTAR(iStar);
			history->mArmyLevel += iStar - iOldStar;
		}
		if (iScore > iOldScore)
		{
			m_MapInfo[iWarid]->setSCORE(iScore);
		}
		m_MapInfo[iWarid]->setATTACKNUM(m_MapInfo[iWarid]->getATTACKNUM() + iBattleNum);

		history->m_pPlayerInfo->m_mMapAttackNum[iWarid][0]++;
	}
	else
	{
		MapDateElem* ptrMDE = new MapDateElem();
		ptrMDE->setMAPID(iMapid);
		ptrMDE->setWARID(iWarid);
		ptrMDE->setSTAR(iStar);
		ptrMDE->setSCORE(iScore);
		ptrMDE->setATTACKNUM(iBattleNum);
		m_MapInfo[ptrMDE->getWARID()] = ptrMDE;
		history->mArmyLevel += iStar;

		history->m_pPlayerInfo->m_mMapAttackNum[iWarid][0]++;
	}
}

void MapDateInfo::AddHeroicBattleWinDate(int iWarid,int iBattleNum)
{
	if (m_HeroicMapInfo.count(iWarid) >0)
	{
		m_HeroicMapInfo[iWarid]->setATTACKNUM(m_HeroicMapInfo[iWarid]->getATTACKNUM() + iBattleNum);
		history->m_pPlayerInfo->m_mMapAttackNum[iWarid][0]++;
	}
	else
	{
		MapDateElem* ptrMDE = new MapDateElem();
		if (history->m_iBattleType == BATTLE_HEROIC)
		{
			ptrMDE->setMAPID(MAPID_SCENE_HEROIC);
		}
		else if(history->m_iBattleType == BATTLE_ACTSCENE)
		{
			ptrMDE->setMAPID(MAPID_SCENE_ACTIVITY);
		}
		else if (history->m_iBattleType == BATTLE_ROMANCE || history->m_iBattleType == BATTLE_WOLFCITY || history->m_iBattleType == BATTLE_IMAGE || history->m_iBattleType == BATTLE_CLANARMYBATTLE)
		{
			ptrMDE->setMAPID(trData->m_mNetWarInfo[iWarid]->iMapIndex);
			ptrMDE->setSTAR(1);
		}
		else
		{
			ptrMDE->setMAPID(trData->m_mNetWarInfo[iWarid]->iMapIndex);
		}
		ptrMDE->setWARID(iWarid);
		ptrMDE->setATTACKNUM(iBattleNum);
		m_HeroicMapInfo[ptrMDE->getWARID()] = ptrMDE;
		history->m_pPlayerInfo->m_mMapAttackNum[iWarid][0]++;
	}
}

void MapDateInfo::AddHeroicBattleRefNum(int iWarid, int iBattleNum)
{
	if (m_HeroicMapInfo.count(iWarid) > 0)
	{
		m_HeroicMapInfo[iWarid]->setRefNum(m_HeroicMapInfo[iWarid]->getRefNum() + iBattleNum);
		history->m_pPlayerInfo->m_mMapAttackNum[iWarid][1]++;
	}
}

map<int, int> MapDateInfo::getMapTreasure(int iMapId)
{
	map<int, int> pRet;
	if (m_MapTreasure.count(iMapId) > 0)
	{
		for (map<int, int>::iterator it = m_MapTreasure[iMapId].begin(); it != m_MapTreasure[iMapId].end(); it++)
		{
			pRet[it->first] = 1;
		}
	}
	return pRet;
}
void MapDateInfo::addMapTreasure(int iMapId, int iStar, int iCount)
{
	m_MapTreasure[iMapId][iStar] = iCount;
}
void MapDateInfo::setMapTreasure(map<int, map<int, int> > mMapTrea)
{
	m_MapTreasure.clear();
	m_MapTreasure = mMapTrea;
}
int MapDateInfo::getMapTotalStar(int iMapId)
{
	int iRet = 0;
	map<int, int> mapWarList = trData->map_total_war[iMapId];
	for (map<int, int>::iterator it = mapWarList.begin(); it != mapWarList.end(); it++)
	{
		if (m_MapInfo.count(it->first) > 0)
		{
			iRet += m_MapInfo[it->first]->getSTAR();
		}
	}
	return iRet;
}

int MapDateInfo::getRomanceWinGq(int iMapId)
{
	int iRet = 0;
	map<int, int> mapWarList = trData->map_total_war[iMapId];
	for (map<int, int>::iterator it = mapWarList.begin(); it != mapWarList.end(); it++)
	{
		if (m_HeroicMapInfo.count(it->first) > 0)
		{
			iRet += 1;
		}
	}
	return iRet;
}

BattleWolfCity* MapDateInfo::GetBattleWolfData()
{
	return &mBattleWolfCity;
}

BattleImageWar* MapDateInfo::GetBattleImageData()
{
	return &mBattleImageWar;
}

BattleGreatWar* MapDateInfo::GetBattleGreatData()
{
	return &mBattleGreatWar;
}

bool MapDateInfo::isPassed(int iWarId)
{
	bool bIsPassed = false;
	if (history->m_pPlayerInfo->m_mMapAttackNum.count(iWarId) > 0)
	{
		bIsPassed = true;
	}
	return bIsPassed;
}

///////////////////////////
BattleWolfCity::BattleWolfCity()
{
	for (map<int, NetWarInfo*>::iterator eIt = trData->m_mNetWarInfo.begin(); eIt != trData->m_mNetWarInfo.end(); eIt++)
	{
		if (eIt->second->iType == BATTLE_WOLFCITY)
		{
			mMapWolfWarList[eIt->second->iWarId] = 1;
		}
	}
}
BattleWolfCity::~BattleWolfCity()
{

}

void BattleWolfCity::SetWolfBattleData(int iWarId)
{
	mBattleData[iWarId] = 1;
}

int BattleWolfCity::GetWolfBattleState(int iWarid)
{
	if (mBattleData.count(iWarid))
		return mBattleData[iWarid];
	else
		return 0;
}

void BattleWolfCity::SetWolfTreasures(int iStar,int iCount)
{
	mTreasures[iStar] = iCount;
}

map<int, int> BattleWolfCity::GetWolfTreasures()
{
	return mTreasures;
}

void BattleWolfCity::SetWolfGuardData(int iWarId, map<int, int> m_guard)
{
	if (!mGuard.count(iWarId)&& m_guard.size())
	{
		mGuard[iWarId] = m_guard;

		for (map<int, int>::iterator iIt = m_guard.begin(); iIt != m_guard.end(); iIt++)
		{
			mAllGuardActor[iIt->second] = 1;
		}
	}
}

map<int, int> BattleWolfCity::GetWolfGuardData(int iWarid)
{
	map<int, int> pRet;
	if (mGuard.count(iWarid))
		return mGuard[iWarid];
	else
		return pRet;
}

void BattleWolfCity::LoadJsonValue(Json::Value jsonData)
{
	mBattleData.clear();
	mTreasures.clear();
	mGuard.clear();
	mAllGuardActor.clear();
	mAddReward.clear();

	Json::Value TreasuresJson = jsonData["map9000"];

	if (TreasuresJson.isObject())
	{
		for (Json::Value::iterator it = TreasuresJson.begin(); it != TreasuresJson.end(); it++)
		{
			string iStarS = it.key().asCString();
			int iStar = StringToInt(iStarS.c_str());
			int iCount = TreasuresJson[iStarS].asInt();
			mTreasures[iStar] = iCount;
		}
	}

	Json::Value mapdataJson = jsonData["gaolang"];

	if (mapdataJson.isArray())
	{
		for (UInt i = 0; i < mapdataJson.size(); i++)
		{
			int iWarId = 0,iHeroIndex = 0;
			map<int, int> heroId;

			if (mapdataJson[i]["warID"].isInt())
				iWarId = mapdataJson[i]["warID"].asInt();
			Json::Value heroIdJson = mapdataJson[i]["heroID"];

			if (iWarId > 0)
			{
				if (heroIdJson.isArray())
				{
					for (UInt j = 0; j < heroIdJson.size(); j++)
					{
						if (heroIdJson[j].isInt())
						{
							int iActorId = heroIdJson[j].asInt();
							mAllGuardActor[iActorId] = 1;
							heroId[iHeroIndex++] = iActorId;
						}
					}
				}

				mBattleData[iWarId] = 1;

				if (heroId.size())
				{
					mGuard[iWarId] = heroId;
				}
			}
		}
	}

	if (!jsonData["goods"].isNull())
	{
		getRewardGoods(jsonData);
	}
}

map<int, int> BattleWolfCity::GetWolfMapWarList()
{
	return mMapWolfWarList;
}

map<int, int> BattleWolfCity::getAllGuardActor()
{
	return mAllGuardActor;
}

map<int, int> BattleWolfCity::WolfIsOpen(int iWarId)
{
	map<int, int> mRet;//[0]:1开启，2前一关未通过，3前一关未派遣守关武将
	mRet[0] = 0;
	int iBattleState = 0, iGuard = 0, iPreWarId = 0;;

	if (mMapWolfWarList.count(iWarId))
	{
		if (mMapWolfWarList.begin()->first == iWarId)
		{
			iPreWarId = iWarId;
			mRet[0] = 1;
		}
		else
		{
			map<int, int>::iterator retIt = mMapWolfWarList.find(iWarId);
			retIt--;
			iPreWarId = retIt->first;
			iBattleState = mBattleData.count(iPreWarId);

			if (mGuard.count(iPreWarId) && mGuard[iPreWarId].size())
				iGuard = 1;
			else
				iGuard = 0;

			if (iBattleState&&iGuard)
			{
				mRet[0] = 1;
			}
			else if (!iBattleState)
			{
				mRet[0] = 2;
			}
			else if (iBattleState && !iGuard)
			{
				mRet[0] = 3;
			}
		}
	}
	mRet[1] = iPreWarId;

	return mRet;
}

int BattleWolfCity::getWolfCityWinGq()
{
	int iRet = 0;
	map<int, int> mapWarList = trData->map_total_war[MAPID_SCENE_WOLF_CITY];
	for (map<int, int>::iterator it = mapWarList.begin(); it != mapWarList.end(); it++)
	{
		if (mBattleData.count(it->first) > 0&& mBattleData[it->first])
		{
			iRet += 1;
		}
	}
	return iRet;
}

void BattleWolfCity::getRewardGoods(Json::Value jsonData)
{
	Json::Value jsonRGoods = jsonData["goods"];

	if (jsonRGoods.isArray() && !jsonRGoods.size())
	{
		return;
	}

	int iRewardIndex = 0;
	map<int, int> mGoodsIndex;
	if (jsonData["jade"].isInt())
	{
		int iAddJade = jsonData["jade"].asInt() - history->m_iJade;
		if (iAddJade)
		{
			history->m_iJade += iAddJade;
			mAddReward[iRewardIndex][0] = 10000;
			mAddReward[iRewardIndex][1] = iAddJade;
			mGoodsIndex[10000] = iRewardIndex;
			iRewardIndex++;
		}
	}

	if (jsonData["gold"].isInt())
	{
		int iAddGold = jsonData["gold"].asInt() - history->money;
		if (iAddGold)
		{
			history->money += iAddGold;
			mAddReward[iRewardIndex][0] = 10001;
			mAddReward[iRewardIndex][1] = iAddGold;
			mGoodsIndex[10001] = iRewardIndex;
			iRewardIndex++;
		}
	}

	if (jsonData["improvedStone"].isInt())
	{
		int iAddStone = jsonData["improvedStone"].asInt() - history->stone;
		if (iAddStone)
		{
			history->stone += iAddStone;
			mAddReward[iRewardIndex][0] = 10002;
			mAddReward[iRewardIndex][1] = iAddStone;
			mGoodsIndex[10002] = iRewardIndex;
			iRewardIndex++;
		}
	}

	if (jsonData["jadeSeal"].isInt())
	{
		int iAddLong = jsonData["jadeSeal"].asInt() - history->m_iJadeSeal;
		history->m_iJadeSeal += iAddLong;
		if (iAddLong > 0)
		{
			mAddReward[iRewardIndex][0] = 10004;
			mAddReward[iRewardIndex][1] = iAddLong;
			mGoodsIndex[10004] = iRewardIndex;
			iRewardIndex++;
		}
	}

	if (jsonRGoods.isArray())
	{
		for (UInt i = 0; i < jsonRGoods.size(); i++)
		{
			int iGoodsId = jsonRGoods[i]["goodID"].asInt();
			int iId = jsonRGoods[i]["id"].asInt();
			int iCount = jsonRGoods[i]["count"].asInt();

			if (iGoodsId >= 10000 && iGoodsId <= 10004)
				continue;

			history->packGoodsAdd(iGoodsId, iId, iCount);

			if (mGoodsIndex.count(iGoodsId))
			{
				mAddReward[mGoodsIndex[iGoodsId]][1] += iCount;
			}
			else
			{
				mAddReward[iRewardIndex][0] = iGoodsId;
				mAddReward[iRewardIndex][1] = iCount;
				mGoodsIndex[iGoodsId] = iRewardIndex;
				iRewardIndex++;
			}

		}
	}

	if (history->m_layTitleBar)
	{
		history->m_layTitleBar->updateShow(0);
	}
	Director::getInstance()->getRunningScene()->addChild(PopLayer::create(POP_GLC_REWARD, this));
}

void BattleWolfCity::SetPopLayerElement(Sprite* bgSp)
{
	int iBgWidth = bgSp->getContentSize().width;
	int iBgHeight = bgSp->getContentSize().height;

	Sprite* tgjlSp = ReadSpriteName("new_ui/glc_war/glc_txt2.png");
	tgjlSp->setAnchorPoint(Vec2(0.5, 1));
	tgjlSp->setPosition(Vec2(iBgWidth / 2, iBgHeight));
	bgSp->addChild(tgjlSp);

	int eGoodsListIndex = mAddReward.size();

	if (eGoodsListIndex > 4) eGoodsListIndex = 4;
	Sprite* eGoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
	int ieGoodsBgWidth = eGoodsBG->getContentSize().width;
	int iPointX = (iBgWidth - ieGoodsBgWidth*eGoodsListIndex - 10 * (eGoodsListIndex - 1)) / 2 + ieGoodsBgWidth / 2;

	int iGoodsId = 0, iCount = 0;
	for (int i = 0; i < eGoodsListIndex; i++)
	{
		iGoodsId = mAddReward[i][0];
		iCount = mAddReward[i][1];

		Sprite* GoodsBG = ReadSpriteName("new_ui/img_global/item_box_bg2.png");
		GoodsBG->setPosition(Vec2(iPointX + i*(ieGoodsBgWidth + 10), iBgHeight / 2 + 23));
		bgSp->addChild(GoodsBG);

		//物品ICON
		Sprite* spIcon = history->getGoodsSprite(iGoodsId);
		spIcon->setPosition(Vec2(33, 33));
		GoodsBG->addChild(spIcon);

		//数目
		if (iCount > 1)
		{
			Sprite* spNum = ITools::getInstance()->getNumSprite(iCount, true);
			spNum->setAnchorPoint(Vec2(1, 0));
			spNum->setPosition(Vec2(62, 4));
			GoodsBG->addChild(spNum);
		}
	}
}

void BattleWolfCity::RunFunction(int iNomal)
{
	if (iNomal == 1)
	{
		//待定
	}
}

//////////////////////////////////

BattleImageWar::BattleImageWar()
{
	for (map<int, NetWarInfo*>::iterator eIt = trData->m_mNetWarInfo.begin(); eIt != trData->m_mNetWarInfo.end(); eIt++)
	{
		if (eIt->second->iType == BATTLE_IMAGE)
		{
			eAllDegreeWarList[eIt->second->iDegree][eIt->second->iWarId] = 1;
		}
	}
}
BattleImageWar::~BattleImageWar()
{

}

int BattleImageWar::GetCurBattleWar(int iStage)
{
	int iRet = 0;
	if (eAllDegreeWarList.count(iStage))
	{
		map<int, int> eWarList = eAllDegreeWarList[iStage];

		for (map<int, int>::iterator eIt = eWarList.begin(); eIt != eWarList.end(); eIt++)
		{
			if (!mBattleData.count(eIt->first))
			{
				iRet = eIt->first;
				break;
			}
		}
	}

	return iRet;
}

int BattleImageWar::GetBattleStar(int iWarId, int iStage)
{
	int iRet = 1;

	if (eAllDegreeWarList.count(iStage))
	{
		for (map<int, map<int, int> >::iterator eIt = eAllDegreeWarList.begin(); eIt != eAllDegreeWarList.end(); eIt++)
		{
			if (eIt->first < iStage)
				iRet += eIt->second.size();
			else if(eIt->first >= iStage)
				break;
		}
	}
	return iRet;
}

int BattleImageWar::GetStageNum()
{
	return eAllDegreeWarList.size();
}

int BattleImageWar::GetXMTWarStar(int iWarId, int iStage)
{
	int iRet = 0;

	if (eAllDegreeWarList.count(iStage))
	{
		iRet = 1;
		for (map<int, map<int, int> >::iterator eIt = eAllDegreeWarList.begin(); eIt != eAllDegreeWarList.end(); eIt++)
		{
			if (eIt->first < iStage)
				iRet += eIt->second.size();
			else if (eIt->first >= iStage)
				break;
		}
		iRet += iWarId - eAllDegreeWarList[iStage].begin()->first;
	}
	return iRet;
}

int BattleImageWar::GetEnemyImageId(int iEnemyId)
{
	int iRet = 0;
	if (mActorList.count(iEnemyId))
		iRet = mActorList[iEnemyId];
	return iRet;
}
void BattleImageWar::ClearActorList()
{
	mActorList.clear();
}

void BattleImageWar::SetImageActorList(map<int, int> eActorList)
{
	mActorList = eActorList;
	for (map<int, int>::iterator eIt = eActorList.begin(); eIt != eActorList.end(); eIt++)
	{
		mActorList[eIt->first] = eIt->second;
	}
}

void BattleImageWar::SetBattleData(int iWarId)
{
	mBattleData[iWarId] = 1;
}

int BattleImageWar::GetBattleState(int iWarid)
{
	if (mBattleData.count(iWarid))
		return mBattleData[iWarid];
	else
		return 0;
}

map<int, int> BattleImageWar::GetAllBattleData()
{
	return mBattleData;
}

void BattleImageWar::SetTreasures(int iStar, int iCount)
{
	mTreasures[iStar] = iCount;
}

map<int, int> BattleImageWar::GetTreasures()
{
	return mTreasures;
}

void BattleImageWar::LoadJsonValue(Json::Value jsonData)
{
	mBattleData.clear();
	mTreasures.clear();

	Json::Value TreasuresJson = jsonData["map9100"];

	if (TreasuresJson.isObject())
	{
		for (Json::Value::iterator it = TreasuresJson.begin(); it != TreasuresJson.end(); it++)
		{
			string iStarS = it.key().asCString();
			int iStar = StringToInt(iStarS.c_str());
			int iCount = TreasuresJson[iStarS].asInt();
			mTreasures[iStar] = iCount;
		}
	}

	Json::Value mapdataJson = jsonData["xinmo"];

	if (mapdataJson.isArray())
	{
		for (UInt i = 0; i < mapdataJson.size(); i++)
		{
			int iWarId = 0;

			if (mapdataJson[i]["warID"].isInt())
				iWarId = mapdataJson[i]["warID"].asInt();

			if (iWarId > 0)
			{
				mBattleData[iWarId] = 1;
			}
		}
	}
}

/////////////////////////////////

ArmyFuBenBattle::ArmyFuBenBattle()
{

}
ArmyFuBenBattle::~ArmyFuBenBattle()
{

}

void ArmyFuBenBattle::ClearBossGainHp()
{
	mClanBossGainHp.clear();
}

void ArmyFuBenBattle::SetEndBossGainHp(int iWarid)
{
	ClearBossGainHp();

	if (trData->m_mNetWarInfo.count(iWarid))
	{
		map<int, int> mBossList;
		for (size_t i = 0; i < trData->m_mNetWarInfo[iWarid]->drop.size(); i++)
			mBossList[trData->m_mNetWarInfo[iWarid]->mBossId[i]] = 0;

		int iActorId = 0;
		for (int i = 0; i < history->battleEnemyNum; i++)
		{
			iActorId = history->battleEnemy[i]->actor_id + 1;
			if (mBossList.count(iActorId))
			{
				int iHpGain = history->battleEnemy[i]->GetBaseHp() + history->battleEnemy[i]->GetGoodsAddHp() - history->battleEnemy[i]->GetCurHp();
				if (iHpGain < 0) iHpGain = 0;
				SetBossGainHp(iActorId,iHpGain);
			}
			else if (trData->m_mNetWarInfo[iWarid]->iPawnHp)
			{
				int iHpGain = (history->battleEnemy[i]->GetBaseHp() + history->battleEnemy[i]->GetGoodsAddHp() - history->battleEnemy[i]->GetCurHp())*trData->m_mNetWarInfo[iWarid]->iPawnHp/100;
				if (iHpGain < 0) iHpGain = 0;
				SetBossGainHp(iActorId, iHpGain);
			}
		}
	}
}

int ArmyFuBenBattle::GetBossGainHp(int iBossId)
{
	int iGainHp = 0;
	if (mClanBossGainHp.count(iBossId) > 0)
	{
		iGainHp = mClanBossGainHp[iBossId][0] ^ mClanBossGainHp[iBossId][1];
	}
	return iGainHp;
}

int ArmyFuBenBattle::GetAllEnemyGainHp()
{
	int iGainHp = 0;
	for (map<int, map<int, int> >::iterator eIt = mClanBossGainHp.begin(); eIt != mClanBossGainHp.end(); eIt++)
	{
		iGainHp += eIt->second[0] ^ eIt->second[1];
	}
	return iGainHp;
}

void ArmyFuBenBattle::SetBossGainHp(int iBossId, int iSet)
{
	int iEncryptKey = rand();

	mClanBossGainHp[iBossId][0] = iSet^iEncryptKey;

	mClanBossGainHp[iBossId][1] = iEncryptKey;
}
//////////////////////////////////////////
BattleGreatWar::BattleGreatWar()
{
	m_bIsOpen = false;
}

BattleGreatWar::~BattleGreatWar()
{

}

void BattleGreatWar::SetBattleData(int iWarId)
{
	mBattleData[iWarId] = 1;
}
int BattleGreatWar::GetBattleState(int iWarid)
{
	if (mBattleData.count(iWarid))
		return 1;
	else
		return 0;
}

map<int, int> BattleGreatWar::GetAllBattleData()
{
	return mBattleData;
}

bool BattleGreatWar::GreatBattleIsOpen()
{
	return m_bIsOpen;
}

void BattleGreatWar::LoadJsonValue(Json::Value jsonData)
{
	Json::Value arrData = jsonData["arr"];
	int iStartWarId = trData->net_zmzy_map.begin()->second->warid[0];
	if (arrData.isArray())
	{
		for (UInt i = 0; i < arrData.size(); i++)
		{
			int iWarId = arrData[i]["warID"].asInt();
			bool bIsOpen = arrData[i]["isOpen"].asBool();

			if (iStartWarId == iWarId&&bIsOpen)
			{
				m_bIsOpen = true;
				break;
			}
		}
	}

	mBattleData.clear();
	Json::Value mapdataJson = jsonData["zmzy"];
	if (mapdataJson.isArray())
	{
		for (UInt i = 0; i < mapdataJson.size(); i++)
		{
			int iWarId = 0;

			if (mapdataJson[i]["warID"].isInt())
				iWarId = mapdataJson[i]["warID"].asInt();

			if (iWarId > 0)
			{
				mBattleData[iWarId] = 1;
			}
		}
	}
}