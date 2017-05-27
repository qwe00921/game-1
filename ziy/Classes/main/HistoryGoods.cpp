#include "Common.h"
#include "main.h"

HistoryGoods::HistoryGoods()
{
	this->m_iNetId = -1;
	this->good_id = -1;
	this->level = 1;
	this->exp = 0;
	this->goodAddLevel = 0;
	this->iQuality = 0;
	this->is_press = false;
	this->reLevelUpExp = -1;
	this->equipment_actorId = -1;
	this->m_pGoods = getGoodAttr();
	this->special_power = 0;
	this->setGoodsPower(0);
	this->iNewGoods = false;
	memset(StrAddAbility, 0, sizeof(StrAddAbility));
	memset(StrengEffectIndex, 0, sizeof(StrengEffectIndex));
	this->m_iStone = 0;
}
HistoryGoods::HistoryGoods(int iGoodId)
{
	this->m_iNetId = -1;
	this->good_id = iGoodId;
	this->level = 1;
	this->exp = 0;
	this->goodAddLevel = 0;
	this->iQuality = 0;
	this->reLevelUpExp = -1;
	this->equipment_actorId = -1;
	this->is_press = false;
	this->iNewGoods = false;
	this->m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		this->setGoodsPower(m_pGoods->iPower);
		this->special_power = m_pGoods->iSpePower;
	}
	else
	{
		this->setGoodsPower(0);
		this->special_power = 0;
	}
	memset(StrAddAbility, 0, sizeof(StrAddAbility));
	memset(StrengEffectIndex, 0, sizeof(StrengEffectIndex));
	this->m_iStone = 0;
}
HistoryGoods::HistoryGoods(int iGoodId, int level)
{
	this->goodAddLevel = 0;
	init(iGoodId, level);
}
HistoryGoods::HistoryGoods(HistoryGoodsPtr ptrGood)
{
	this->m_iNetId = ptrGood->m_iNetId;
	this->good_id = ptrGood->good_id;
	this->level = ptrGood->level;
	this->exp = ptrGood->exp;
	this->goodAddLevel = 0;
	this->iQuality = ptrGood->iQuality;
	this->is_press = ptrGood->is_press;
	this->reLevelUpExp = ptrGood->reLevelUpExp;
	this->m_pGoods = getGoodAttr();
	this->setGoodsPower(ptrGood->GetGoodsPower());
	this->special_power = ptrGood->special_power;
	this->StrengEffect = ptrGood->StrengEffect;
	this->iNewGoods = false;
	for (int i = 0; i < 6; i++)
	{
		this->StrAddAbility[i] = ptrGood->StrAddAbility[i];
	}
	for (int j = 0; j < 10; j++)
	{
		this->StrengEffectIndex[j] = ptrGood->StrengEffectIndex[j];
	}
	this->equipment_actorId = -1;
	this->m_iStone = ptrGood->m_iStone;
}
HistoryGoods::HistoryGoods(GoodsElem* ptrGoods)
{
	setGood(ptrGoods);
}

void HistoryGoods::setGood(HistoryGoodsPtr good)
{
	this->m_iNetId = good->m_iNetId;
	this->good_id = good->good_id;
	this->level = good->level;
	this->exp = good->exp;
	this->iQuality = good->iQuality;
	this->StrengEffect = good->StrengEffect;
	this->is_press = good->is_press;
	this->reLevelUpExp = good->reLevelUpExp;
	this->special_power = good->special_power;
	this->m_pGoods = getGoodAttr();
	this->iNewGoods = false;
	this->setGoodsPower(good->GetGoodsPower());
	for (int i = 0; i < 6; i++)
	{
		this->StrAddAbility[i] = good->StrAddAbility[i];
	}
	for (int j = 0; j < 10; j++)
	{
		this->StrengEffectIndex[j] = good->StrengEffectIndex[j];
	}
	this->m_iStone = good->m_iStone;
}
void HistoryGoods::setGood(GoodsElem* ptrGoods)
{
	this->m_iNetId = ptrGoods->getNetID();
	this->good_id = ptrGoods->getGoodID();
	this->level = 1;
	this->exp = 0;
	this->goodAddLevel = 0;
	this->iQuality = ptrGoods->getStarLV();
	this->reLevelUpExp = -1;
	this->equipment_actorId = ptrGoods->getHeroID() - 1;
	this->is_press = false;
	this->m_pGoods = getGoodAttr();
	this->setGoodsPower(m_pGoods->iPower);
	this->special_power = m_pGoods->iSpePower;
	this->iNewGoods = false;
	this->StrengEffect.clear();
	this->StrengEffect = ptrGoods->m_mProperty;

	memset(StrAddAbility, 0, sizeof(StrAddAbility));
	memset(StrengEffectIndex, 0, sizeof(StrengEffectIndex));
	this->m_iStone = ptrGoods->getStoneID();
}
void HistoryGoods::init(int iGoodId, int level)
{
	this->m_iNetId = -1;
	this->iQuality = 0;
	this->is_press = false;
	this->reLevelUpExp = -1;
	this->equipment_actorId = -1;
	memset(StrAddAbility, 0, sizeof(StrAddAbility));
	memset(StrengEffectIndex, 0, sizeof(StrengEffectIndex));
	this->StrengEffect.clear();

	this->good_id = iGoodId;
	this->m_pGoods = getGoodAttr();
	if (m_pGoods)
	{
		this->level = 1;
		this->exp = 0;
		setGoodsPower(m_pGoods->iPower);
		this->special_power = m_pGoods->iSpePower;
	}
	else
	{
		this->level = 1;
		this->exp = 0;
		setGoodsPower(0);
		this->special_power = 0;
	}
	this->m_iStone = 0;
}

bool HistoryGoods::isSpecial()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->isSpecial();
	}
	return false;
}
int HistoryGoods::getInductNum()
{
	int ret = 1;
	for(UInt i = 0; i < m_pGoods->mSeid.size(); i++)
	{
		int iSeid = m_pGoods->mSeid[i];
		if(trData->m_mSeid[iSeid]->type == 29 || trData->m_mSeid[iSeid]->type == 108)
		{
			if(ret < trData->m_mSeid[iSeid]->special_effe[0])
			{
				ret = trData->m_mSeid[iSeid]->special_effe[0];
			}
		}
	}
	return ret;
}

bool HistoryGoods::isNormalGood()
{
	return trData->m_mGoodsItem.count(good_id) ? true : false;
}

GoodsItem* HistoryGoods::getGoodAttr()
{
	map<int, GoodsItem*>::iterator itGoods = trData->m_mGoodsItem.find(good_id);
	if (itGoods != trData->m_mGoodsItem.end())
	{
		return trData->m_mGoodsItem[good_id];
	}
	return NULL;
}


int HistoryGoods::GetGoodsType()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->m_iGoodType;
	}
	return 0;
}

int HistoryGoods::GetGoodsAbilityType()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->GetGoodsAbilityType();
	}
	return 0;
}

char* HistoryGoods::GetAttributeInfo()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->GetAttributeInfo();
	}
	return "---";
}

char* HistoryGoods::GetSpecialEffectInfo()
{
	m_pGoods = getGoodAttr();
	if (m_pGoods && m_pGoods->iUeid != 28 && m_pGoods->iUeid != 37 && m_pGoods->iUeid != 42)
	{
		return m_pGoods->GetSpecialEffectInfo(GetRealLevel());
	}
	return "---";
}

char* HistoryGoods::GetGoodsName(int iNum)
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		if (iNum > 1)
		{
			return ChessSprintf("%sx%d", m_pGoods->strName.c_str(), iNum);
		}
		else
		{
			return (char*)m_pGoods->strName.c_str();
		}
	}
	return "";
}

char* HistoryGoods::GetGoodsAbilityInfo()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->GetGoodsAbilityInfo(GetRealLevel());
	}
	return "";
}

char* HistoryGoods::GetGoodsLevel()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		if (m_pGoods->isEquip() || m_pGoods->iUeid == 27 || m_pGoods->iUeid == 41) //售卖武将需显示等级
		{
			return IntToString(level);
		}
	}
	return "--";
}

int HistoryGoods::GetGoodsPrice()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->iPrice * 100;
	}
	return 0;
}

int HistoryGoods::getUeId()
{
	m_pGoods = getGoodAttr();
	if(m_pGoods)
	{
		return m_pGoods->iUeid;
	}
	return 0;
}

short HistoryGoods::GetRealLevel() //获取物品真实等级
{
	if (goodAddLevel > 0)
	{
		Seid *sSeid = trData->m_mSeid[goodAddLevel];
		if (level >= sSeid->special_effe[0] && sSeid->special_effe_num > 1)
			return level + sSeid->special_effe[1];
		else if (level >= 7)
			return level + sSeid->special_effe[0];
	}
	return level;
}

void HistoryGoods::setGoodAddLevel(short goodAddLevel)
{
	this->goodAddLevel = goodAddLevel;
}
int HistoryGoods::getGoodsSeType()
{
	int iSeid = trData->m_mGoodsItem[good_id]->iSeSeid;
	return trData->m_mSeid[iSeid]->type;
}
int HistoryGoods::GetOrSetQuality(int iAdd, bool is_get)
{
	if (is_get)
		return this->iQuality;
	else
		this->iQuality += iAdd;
	return 0;
}
std::map<int, int> HistoryGoods::GetStrengEffect()
{
	return this->StrengEffect;
}
void HistoryGoods::AddStrengEffect(int iEffect)
{
	StrengEffect[StrengEffect.size()] = iEffect;
	setStrAddEffect();
}
void HistoryGoods::EraseStrengEffect(int index)
{
	StrengEffect.erase(index);
	setStrAddEffect();
}
void HistoryGoods::UpdateStrengEffect(int index,int iNum)
{
	StrengEffect[index] = iNum;
	setStrAddEffect();
}
std::map<int, int> HistoryGoods::getGoodsAddPower()
{
	std::map<int, int> addpower;
	addpower[0] = 0;
	addpower[1] = 0;
	addpower[2] = 0;
	addpower[3] = 0;
	addpower[4] = 0;
	addpower[5] = 0;
	if (isNormalGood())
	{
		GoodsItem* ptrGoods = getGoodAttr();
		int itid = ptrGoods->iItId;
		int real_power = ptrGoods->iPower + (GetRealLevel() - 1) * ptrGoods->iUpVal;
		int iType = ptrGoods->GetGoodsAbilityType();
		if(iType & GOODS_POWER) //攻击力
		{
			addpower[0] = real_power;
		}
		if(iType & GOODS_INTEL) //精神力
		{
			addpower[2] = real_power;
		}
		if(iType & GOODS_DEFENCE) //防御力
		{
			addpower[1] = real_power;
		}
	}
	return addpower;
}
void HistoryGoods::setStrAddEffect()
{
	reLevelUpExp = -1;
	memset(StrAddAbility, 0, sizeof(StrAddAbility));
	memset(StrengEffectIndex, 0, sizeof(StrengEffectIndex));
	int iIndex = 0;
	int is_bouble = 1 + IS_HALF_BASEABILITY;
	for (size_t i = 0; i < StrengEffect.size(); i++)
	{
		for (int j = 0; j < trData->strengthen_property_num; j++)
		{
			StrengthenPropertyPtr ptrStr = &trData->strengthen_property[j];
			int iSeidId = ptrStr->id + MAX_SEID_NUM;
			int iEffectNum = trData->m_mSeid[iSeidId]->special_effe[0];
			if (ptrStr->id == StrengEffect[i])
			{
				if (iIndex<10)
					StrengEffectIndex[iIndex++] = j+1;
				if (ptrStr->se_type == 1000)
				{
					if (reLevelUpExp < 0)
						reLevelUpExp = abs(iEffectNum);
					else
						reLevelUpExp += abs(iEffectNum);
				}
				else if (ptrStr->se_type == 1001)//武力
				{
					StrAddAbility[A_POWER] += abs(iEffectNum) / is_bouble;
				}
				else if (ptrStr->se_type == 1002)//智力
				{
					StrAddAbility[A_INTELLEGENCE] += abs(iEffectNum) / is_bouble;
				}
				else if (ptrStr->se_type == 1003)//统帅
				{
					StrAddAbility[A_DEFENCE] += abs(iEffectNum) / is_bouble;
				}
				else if (ptrStr->se_type == 1004)//敏捷
				{
					StrAddAbility[A_EXPLOSION] += abs(iEffectNum) / is_bouble;
				}
				else if (ptrStr->se_type == 1005)//运气
				{
					StrAddAbility[A_FORTURN] += abs(iEffectNum) / is_bouble;
				}
				else if (ptrStr->se_type == 1006)//移动
				{
					StrAddAbility[A_SPEED] += abs(iEffectNum);
				}
				break;
			}
		}
	}
	if (reLevelUpExp == -1)
		reLevelUpExp = -2;
}
int HistoryGoods::getReLevelUpExp()
{
	return 100;
}
int* HistoryGoods::getStrAddAbility()
{
	if (reLevelUpExp == -1)
		setStrAddEffect();
	return StrAddAbility;
}
int* HistoryGoods::getStrengEffectIndex()
{
	if (reLevelUpExp == -1)
		setStrAddEffect();
	return StrengEffectIndex;
}

int HistoryGoods::getStoreNum()
{
	int iStoreNum = 0;
	if(m_pGoods)
	{
		if(m_pGoods->bIsConsume) //消耗品
		{
			iStoreNum = history->m_mConsuNum[good_id];
		}
		else if(m_pGoods->m_iGoodType == GOOD_TYPE_PIECE)
		{
			map<int, ChipSoulDateElem*> mapChipSoul = history->mChipSoulDate->getChipSoulMap();
			if(mapChipSoul.find(good_id) != mapChipSoul.end())
			{
				iStoreNum = mapChipSoul[good_id]->mNum;
			}
		}
		else
		{
			for(int i = 0; i < history->goods->size(); i++)
			{
				HistoryGoodsPtr ptrGood = (HistoryGoodsPtr)history->goods->elementAt(i);
				if(ptrGood->good_id == good_id)
				{
					iStoreNum++;
				}
			}
			for(int i = 0; i < history->ownArmyNum; i++)
			{
				HistoryActorPtr ptrActor = &history->actors[history->ownArmy[i]];
				if(ptrActor->weapon.good_id == good_id || ptrActor->coat.good_id == good_id || ptrActor->assist_good.good_id == good_id || ptrActor->fashion.good_id == good_id || ptrActor->wap_fashion.good_id == good_id)
				{
					iStoreNum++;
				}
			}

			if(iStoreNum > 1)
			{
				iStoreNum = 1;
			}
		}
	}
	return iStoreNum;
}

bool HistoryGoods::isTreasure()
{
	bool bTreasure = false;
	m_pGoods = getGoodAttr();
	if (m_pGoods && m_pGoods->iItId > 0 && m_pGoods->iItId < 999 && m_pGoods->iItId % 2 == 0)
	{
		bTreasure = true;
	}
	return bTreasure;
}

int HistoryGoods::getImageId()
{
	return trData->m_mGoodsItem[good_id]->iImgId;
}

char* HistoryGoods::getSpecialEffectName()
{
	m_pGoods = getGoodAttr();
	return m_pGoods->GetSpecialEffectName(0);
}

char* HistoryGoods::getSpecialEffectInfo()
{
	m_pGoods = getGoodAttr();
	return m_pGoods->GetSpecialEffectInfo(0);
}

bool HistoryGoods::isConsume()
{
	return trData->isConsume(good_id);
}

bool HistoryGoods::isForgeEnable()
{
	bool bIsForge = false;
	if(m_pGoods && m_pGoods->iTypeId)
	{
		bIsForge = true;
	}
	return bIsForge;
}

bool HistoryGoods::isEquipEnable(int iActorId)
{
	bool bEnable = true;
	map<int, int> mSeid = m_pGoods->mSeid;
	for (UInt i = 0; i < mSeid.size(); i++)
	{
		int iSeid = mSeid[i];
		if(trData->m_mSeid.count(iSeid))
		{
			Seid* ptrSeid = trData->m_mSeid[iSeid];
			if (ptrSeid->type == TYPE733)
			{
				bEnable = false;
				for(int i = 0; i < ptrSeid->special_effe_num; i++)
				{
					if (ptrSeid->special_effe[i] == iActorId + 1)
					{
						return true;
					}
				}
			}
			else if (ptrSeid->type == TYPE734)
			{
				HistoryActor hisActor = history->actors[iActorId];
				bEnable = false;
				for(int i = 0; i < ptrSeid->special_effe_num; i++)
				{
					if(ptrSeid->special_effe[i] == hisActor.occupation_id + 1)
					{
						return true;
					}
				}
			}
		}
	}
	return bEnable;
}

string HistoryGoods::getInfo()
{
	return trData->m_mGoodsItem[good_id]->strInfo;
}

string HistoryGoods::getAttributeText()
{
	string attrText = "";
	int iType = GetGoodsAbilityType();
	if(iType & GOODS_POWER) //攻击力
	{
		attrText = LL("gongji");
	}
	if(iType & GOODS_INTEL) //精神力
	{
		attrText = LL("jingshen");
	}
	if(iType & GOODS_DEFENCE) //防御力
	{
		attrText = LL("fangyu");
	}
	return attrText;
}

string HistoryGoods::getAllStrengEffextInfo()
{
	string strInfo = "";
	for (size_t i = 0; i < StrengEffect.size(); i++)
	{
		for (int j = 0; j < trData->strengthen_property_num; j++)
		{
			StrengthenPropertyPtr ptrStr = &trData->strengthen_property[j];
			if (ptrStr->id == StrengEffect[i])
			{
				strInfo.append(ptrStr->name);
				strInfo.append("");
				break;
			}
		}
	}
	return strInfo;
}

int HistoryGoods::getPrice()
{
	return m_pGoods->iPrice;
}

int HistoryGoods::getItId()
{
	return m_pGoods->iItId;
}

map<int, int> HistoryGoods::getSeid()
{
	return m_pGoods->mSeid;
}

int HistoryGoods::getSeSeid()
{
	return m_pGoods->iSeSeid;
}

int HistoryGoods::getMaxStar()
{
	return m_pGoods->iTypeId;
}

int HistoryGoods::getSpeAttrNum()
{
	int iNum = 0;
	for(int i = 0; i < m_pGoods->iTypeId; i++)
	{
		if(trData->strengthen_consume[i].special_goods)
		{
			iNum++;
		}
	}
	return iNum;
}

int HistoryGoods::getUpVal()
{
	return m_pGoods->iUpVal;
}
int HistoryGoods::getStrengNeedMoney(int iQuality /*= -1*/)
{
	if (iQuality == -1)
	{
		iQuality = this->iQuality;
	}
	int money = 0;
	if (iQuality >= 0 && iQuality < m_pGoods->iTypeId)
	{
		money = trData->strengthen_consume[iQuality].money;
	}
	return money;
}
int HistoryGoods::getStrengNeedStore(int iQuality /*= -1*/)
{
	if (iQuality == -1)
	{
		iQuality = this->iQuality;
	}
	int store = 0;
	if (iQuality >= 0 && iQuality < m_pGoods->iTypeId)
	{
		store = trData->strengthen_consume[iQuality].gem_num;
	}
	return store;
}
int HistoryGoods::getStarAddVal(int iQuality /*= -1*/)
{
	if(iQuality == -1)
	{
		iQuality = this->iQuality;
	}
	int iAddVal = 0;
	if(GetGoodsAbilityType() & GOODS_DEFENCE) //防御力
	{
		if(iQuality > 0 && iQuality <= m_pGoods->iTypeId)
		{
			iAddVal = trData->strengthen_consume[iQuality - 1].def_num + iQuality * m_pGoods->iUpVal;
		}
	}
	else
	{
		if(iQuality > 0 && iQuality <= m_pGoods->iTypeId)
		{
			iAddVal = trData->strengthen_consume[iQuality - 1].atk_num + iQuality * m_pGoods->iUpVal;
		}
	}
	return iAddVal;
}

bool HistoryGoods::isSoul()
{
	return m_pGoods->iItId == 99 ? true : false;
}

bool HistoryGoods::isPiece()
{
	return m_pGoods->iItId == 98 ? true : false;
}

bool HistoryGoods::isExpFruit()
{
	return m_pGoods->iUeid == 13 ? true : false;
}

bool HistoryGoods::isGift()
{
	return m_pGoods->iUeid == 14 ? true : false;
}

bool HistoryGoods::isAssist()
{
	return m_pGoods->isAssist();
}

bool HistoryGoods::isPreWarConsume()
{
	return m_pGoods->iItId == 998 ? true : false;
}

bool HistoryGoods::isPlotGoods()
{
	return m_pGoods->iItId == 997 ? true : false;
}

bool HistoryGoods::isSuit()
{
	bool bRet = false;
	if (isEquip())
	{
		if (m_pGoods->mSuitList.size() == 1 && m_pGoods->mSuitList.count(0) != 0)
		{
			bRet = false;
		}
		else
		{
			bRet = true;
		}
	}
	else
	{
		bRet = false;
	}

	return bRet;
}

bool HistoryGoods::isHero()
{
	return m_pGoods->iItId == 200 ? true : false;
}

bool HistoryGoods::isFashion()
{
	bool bRet = false;
	int iGetType = GetGoodsType();
	if (iGetType == GOOD_TYPE_FASHING || iGetType == GOOD_TYPE_WAP_FASHING)
	{
		bRet = true;
	}
	return bRet;
}

bool HistoryGoods::isEquip()
{
	return m_pGoods->isEquip() || m_pGoods->isAssist();
}

bool HistoryGoods::isAddAbilityGoods()
{
	return m_pGoods->bIsConsume&&m_pGoods->iUeid == 88;
}
bool HistoryGoods::isNewGoods()
{
	return this->iNewGoods;
}

bool HistoryGoods::isGetEnable()
{
	return !(m_pGoods->iIsOnly && getStoreNum());
}

bool HistoryGoods::isOnly()
{
	return m_pGoods->iIsOnly != 0;
}

bool HistoryGoods::isFmEnable()
{
	return isAssist() || isOnly();
}

map<int, int> HistoryGoods::getFmGoodsMap()
{
	map<int, int> mGoodsId;
	int iIndex = 0;
	if (isFmEnable())
	{
		for (map<int, int>::iterator it = trData->m_mFmStoneId.begin(); it != trData->m_mFmStoneId.end(); it++)
		{
			int iGoodsId = it->first;
			GoodsItem* ptrGoods = trData->m_mGoodsItem[iGoodsId];
			if (ptrGoods->mFmItid.count(GOOD_TYPE_ALLCLASS) > 0 || ptrGoods->mFmItid.count(m_pGoods->iItId) > 0)
			{
				mGoodsId[iIndex++] = ptrGoods->iGoodsId;
			}
		}
	}
	return mGoodsId;
}

int HistoryGoods::GetGoodsPower()
{
	return power^m_EncryptKey;
}

void HistoryGoods::setGoodsPower(int iSet /*= -1*/, int iAdd /*= 0*/)
{
	int iNewEncryptKey = rand();
	if (iSet >= 0)
	{
		power = iSet^iNewEncryptKey;
	}
	else
	{
		power = (GetGoodsPower() + iAdd) ^ iNewEncryptKey;
	}
	m_EncryptKey = iNewEncryptKey;
}

bool HistoryGoods::EnableEquip(int iActorId)
{
	return !trData->m_mGoodsItem[good_id]->mNoHid.count(iActorId);
}