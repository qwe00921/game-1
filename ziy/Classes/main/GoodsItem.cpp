#include "Common.h"
#include "main.h"

void GoodsItem::InitGoodsType()
{
	switch(trData->m_mItemType[iItId]->itpos)
	{
	case 0:
		m_iGoodType = GOOD_TYPE_ASSIST;
		break;
	case 1:
		m_iGoodType = GOOD_TYPE_WEAPON;
		break;
	case 2:
		m_iGoodType = GOOD_TYPE_COAT;
		break;
	case 3:
		m_iGoodType = GOOD_TYPE_CONSUME;
		break;
	case 4:
		m_iGoodType = GOOD_TYPE_PIECE;
		break;
	case 900:
		m_iGoodType = GOOD_TYPE_WAP_FASHING;
		break;
	case 901:
		m_iGoodType = GOOD_TYPE_FASHING;
		break;
	default:
		break;
	}

	if(m_iGoodType == GOOD_TYPE_CONSUME)
	{
		bIsConsume = 1;
	}
	else
	{
		bIsConsume = 0;
	}
}

int GoodsItem::GetGoodsAbilityType()
{
	int iEffect = 0;
	int iItfun = trData->m_mItemType[iItId]->itfun;
	if(iItfun & 0x01)
	{
		iEffect |= GOODS_POWER;
	}
	if(iItfun & 0x02)
	{
		iEffect |= GOODS_INTEL;
	}
	if(iItfun & 0x04)
	{
		iEffect |= GOODS_DEFENCE;
	}
	return iEffect;
}

char* GoodsItem::GetAttributeInfo()
{
	return trData->m_mItemType[iItId]->itname;
}

char* GoodsItem::GetSpecialEffectInfo(int level)
{
	if (isSpecial())
	{
		if (trData->m_mSeid.count(mSeid[0]))
		{
			return trData->m_mSeid[mSeid[0]]->Info;
		}
	}
	return "---";
}
char* GoodsItem::GetSpecialEffectName(int level)
{
	if (isSpecial())
	{
		if(trData->m_mSeid.count(mSeid[0]))
		{
			return trData->m_mSeid[mSeid[0]]->sename;
		}
	}
	return "---";
}

char* GoodsItem::GetGoodsAbilityInfo(int level)
{
	if (!bIsConsume)
	{
		int iPower = this->iPower;
		if (!isAssist())
		{
			iPower += (level - 1) * iUpVal;
		}
		int iAbility = GetGoodsAbilityType();
		DString strAbility = "";
		if(iAbility & GOODS_POWER)
		{
			strAbility = LL("gongjilis");
		}
		else if(iAbility & GOODS_INTEL)
		{
			strAbility = LL("jingshenlis");
		}
		else if(iAbility & GOODS_DEFENCE)
		{
			strAbility = LL("fangyulis");
		}
		if (strAbility != "")
		{
			return ChessSprintf(strAbility->getChar(), IntToString_Sign(iPower));
		}
	}
	return "";
}

string GoodsItem::GetJobItem()
{		
	string itemJob = "";
	if (m_iGoodType == GOOD_TYPE_ASSIST) 
	{
		itemJob = LL("quanbubingzhong");
		return itemJob;
	}
	
	for (int i = 0; i < trData->occupation_num; i++)
	{
		if (trData->occupation_good_info[i][iItId])
		{
			itemJob.append(trData->occupations[i].occupation_name);
			itemJob.append(" ");
		}
	}
	return itemJob;
}

string GoodsItem::GetAllSeidInfo()
{
	if (isSpecial()) {
		int iSeid = -1;
		string strSeidInfo = "";
		for (UInt i = 0; i < mSeid.size(); i++)
		{
			iSeid = mSeid[i];
			if(trData->m_mSeid.count(iSeid))
			{
				strSeidInfo.append(trData->m_mSeid[iSeid]->Info);
			}
			strSeidInfo.append(" ");
		}
		return strSeidInfo;
	}
	return "";
}
