#include "ChipSoulDate.h"
#include "Defines.h"
#include "main.h"

int ChipSoulDate::GetActorChipNumByActorId(int iActorId)
{
	if (mActorChipByActorId.find(iActorId) != mActorChipByActorId.end())
	{
		return mActorChipByActorId[iActorId]->mNum;
	}
	else
		return 0;
}
void ChipSoulDate::AddActorSoulNumByActorId(int iActorId, int iNUm, int iId)
{
	if (mActorChipByActorId.find(iActorId) != mActorChipByActorId.end())
	{
		mActorChipByActorId[iActorId]->mNum += iNUm;
		mActorChipByActorId[iActorId]->id = iId;

		if(mActorChipByActorId[iActorId]->mNum > GOODS_MAX_STORE)
		{
			mActorChipByActorId[iActorId]->mNum = GOODS_MAX_STORE;
		}
	}
	else
	{
		if (trData->net_hero_list.find(iActorId) != trData->net_hero_list.end())
		{
			ChipSoulDateElem* ptrASDE = new ChipSoulDateElem();
			ptrASDE->mElemId = iActorId;
			ptrASDE->mGoodsId = trData->net_hero_list[iActorId]->good_id;
			ptrASDE->mNum = iNUm;
			ptrASDE->id = iId;
			mActorChipByActorId[ptrASDE->mElemId] = ptrASDE;
			mActorChipByGoodsId[ptrASDE->mGoodsId] = ptrASDE;
		}
	}
}
void ChipSoulDate::ReduceActorSoulNumByActorId(int iActorId, int iNUm)
{
	if (mActorChipByActorId.find(iActorId) != mActorChipByActorId.end())
	{
		mActorChipByActorId[iActorId]->mNum -= iNUm;
	}
}

ChipSoulDateElem* ChipSoulDate::GetSoulElemByActorId(int iActorId)
{
	if (mActorChipByActorId.count(iActorId))
	{
		return mActorChipByActorId[iActorId];
	}
	else
		return NULL;
}

map<int, int> ChipSoulDate::getActorSoulListByActorId()
{
	map<int, int> retmap;
	for (map<int, ChipSoulDateElem*>::iterator it = mActorChipByActorId.begin(); it != mActorChipByActorId.end(); it++)
	{
		if (it->second->mNum > 0)
			retmap[it->first] = it->second->mNum;
	}
	return retmap;
}

int ChipSoulDate::GetActorSoulNumByGoodsId(int iGoodsId)
{
	if (mActorChipByGoodsId.find(iGoodsId) != mActorChipByGoodsId.end())
	{
		return mActorChipByGoodsId[iGoodsId]->mNum;
	}
	else
		return 0;
}

void ChipSoulDate::AddActorSoulNumByGoodsId(int iGoodsId, int iNUm, int iId)
{
	if (mActorChipByGoodsId.find(iGoodsId) != mActorChipByGoodsId.end())
	{
		mActorChipByGoodsId[iGoodsId]->mNum += iNUm;
		mActorChipByGoodsId[iGoodsId]->id = iId;

		if(mActorChipByGoodsId[iGoodsId]->mNum > GOODS_MAX_STORE)
		{
			mActorChipByGoodsId[iGoodsId]->mNum = GOODS_MAX_STORE;
		}
	}
	else
	{
		for (map<int, NetHeroListPtr>::iterator it = trData->net_hero_list.begin(); it != trData->net_hero_list.end(); it++)
		{
			if (it->second->good_id == iGoodsId)
			{
				ChipSoulDateElem* ptrASDE = new ChipSoulDateElem();
				ptrASDE->mElemId = it->first;
				ptrASDE->mGoodsId = it->second->good_id;
				ptrASDE->mNum = iNUm;
				ptrASDE->id = iId;
				mActorChipByActorId[ptrASDE->mElemId] = ptrASDE;
				mActorChipByGoodsId[ptrASDE->mGoodsId] = ptrASDE;
				break;
			}
		}
	}
}

void ChipSoulDate::ReduceActorSoulNumByGoodsId(int iGoodsId, int iNUm)
{
	if (mActorChipByGoodsId.find(iGoodsId) != mActorChipByGoodsId.end())
	{
		mActorChipByGoodsId[iGoodsId]->mNum -= iNUm;
	}
}

ChipSoulDateElem * ChipSoulDate::GetSoulElemByGoodsId(int iGoodsId)
{
	if (mActorChipByGoodsId.count(iGoodsId))
	{
		return mActorChipByGoodsId[iGoodsId];
	}
	else
		return NULL;
}

map<int, int> ChipSoulDate::getActorSoulListByGoodsId()
{
	map<int, int> retmap;
	for (map<int, ChipSoulDateElem*>::iterator it = mActorChipByGoodsId.begin(); it != mActorChipByGoodsId.end(); it++)
	{
		if (it->second->mNum > 0)
			retmap[it->first] = it->second->mNum;
	}
	return retmap;
}

/////////////////
int ChipSoulDate::GetEqChipNumByEqId(int iEqId)
{
	if (mEqChipByEqId.find(iEqId) != mEqChipByEqId.end())
	{
		return mEqChipByEqId[iEqId]->mNum;
	}
	else
		return 0;
}
void ChipSoulDate::AddEqSoulNumByEqId(int iEqId, int iNUm, int iId)
{
	if (mEqChipByEqId.find(iEqId) != mEqChipByEqId.end())
	{
		mEqChipByEqId[iEqId]->mNum += iNUm;
		mEqChipByEqId[iEqId]->id = iId;

		if(mEqChipByEqId[iEqId]->mNum > GOODS_MAX_STORE)
		{
			mEqChipByEqId[iEqId]->mNum = GOODS_MAX_STORE;
		}
	}
	else
	{
		int iGoodsId = trData->getMakeGoodsId(iEqId);
		if (iGoodsId)
		{
			ChipSoulDateElem* ptrASDE = new ChipSoulDateElem();
			ptrASDE->mElemId = iGoodsId;
			ptrASDE->mGoodsId = iEqId;
			ptrASDE->mNum = iNUm;
			ptrASDE->id = iId;
			mEqChipByEqId[ptrASDE->mElemId] = ptrASDE;
			mEqChipByGoodsId[ptrASDE->mGoodsId] = ptrASDE;
		}
	}
}
void ChipSoulDate::ReduceEqSoulNumByEqId(int iEqId, int iNUm)
{
	if (mEqChipByEqId.find(iEqId) != mActorChipByActorId.end())
	{
		mEqChipByEqId[iEqId]->mNum -= iNUm;
	}
}
map<int, int> ChipSoulDate::getEqSoulListByEqId()
{
	map<int, int> retmap;
	for (map<int, ChipSoulDateElem*>::iterator it = mEqChipByEqId.begin(); it != mEqChipByEqId.end(); it++)
	{
		if (it->second->mNum > 0)
			retmap[it->first] = it->second->mNum;
	}
	return retmap;
}

int ChipSoulDate::GetEqSoulNumByGoodsId(int iGoodsId)
{
	if (mEqChipByGoodsId.find(iGoodsId) != mEqChipByGoodsId.end())
	{
		return mEqChipByGoodsId[iGoodsId]->mNum;
	}
	else
		return 0;
}

void ChipSoulDate::AddEqSoulNumByGoodsId(int iGoodsId, int iNUm, int iId)
{
	if (mEqChipByGoodsId.find(iGoodsId) != mEqChipByGoodsId.end())
	{
		mEqChipByGoodsId[iGoodsId]->mNum += iNUm;
		mEqChipByGoodsId[iGoodsId]->id = iId;

		if(mEqChipByGoodsId[iGoodsId]->mNum > GOODS_MAX_STORE)
		{
			mEqChipByGoodsId[iGoodsId]->mNum = GOODS_MAX_STORE;
		}
	}
	else
	{
		int iMakeId = trData->getMakeGoodsId(iGoodsId);
		if(iMakeId)
		{
			ChipSoulDateElem* ptrASDE = new ChipSoulDateElem();
			ptrASDE->mElemId = iMakeId;
			ptrASDE->mGoodsId = iGoodsId;
			ptrASDE->mNum = iNUm;
			ptrASDE->id = iId;
			mEqChipByEqId[ptrASDE->mElemId] = ptrASDE;
			mEqChipByGoodsId[ptrASDE->mGoodsId] = ptrASDE;
		}
	}
}

void ChipSoulDate::ReduceEqSoulNumByGoodsId(int iGoodsId, int iNUm)
{
	if (mEqChipByGoodsId.find(iGoodsId) != mEqChipByGoodsId.end())
	{
		mEqChipByGoodsId[iGoodsId]->mNum -= iNUm;
	}
}

map<int, int> ChipSoulDate::getEqSoulListByGoodsId()
{
	map<int, int> retmap;
	for (map<int, ChipSoulDateElem*>::iterator it = mEqChipByGoodsId.begin(); it != mEqChipByGoodsId.end(); it++)
	{
		if (it->second->mNum > 0)
			retmap[it->first] = it->second->mNum;
	}
	return retmap;
}


void ChipSoulDate::ClearChipList()
{
	for (map<int, ChipSoulDateElem*>::iterator it = mActorChipByGoodsId.begin(); it != mActorChipByGoodsId.end(); it++)
	{
		delete it->second;
	}
	for (map<int, ChipSoulDateElem*>::iterator it = mEqChipByEqId.begin(); it != mEqChipByEqId.end(); it++)
	{
		delete it->second;
	}
	mActorChipByGoodsId.clear();
	mActorChipByActorId.clear();

	mEqChipByEqId.clear();
	mEqChipByGoodsId.clear();
}

map<int, ChipSoulDateElem*> ChipSoulDate::getChipSoulMap()
{
	map<int, ChipSoulDateElem*> mapChipSoul;

	for (map<int, ChipSoulDateElem*>::iterator it = mEqChipByGoodsId.begin(); it != mEqChipByGoodsId.end(); it++)
	{
		if(it->second->mNum > 0)
		{
			mapChipSoul[it->first] = it->second;
		}
	}

	for (map<int, ChipSoulDateElem*>::iterator it = mActorChipByGoodsId.begin(); it != mActorChipByGoodsId.end(); it++)
	{
		if(it->second->mNum > 0)
		{
			mapChipSoul[it->first] = it->second;
		}
	}

	return mapChipSoul;
}

int ChipSoulDate::getNetIdByGoodsId(int iGoodsId)
{
	int iNetId = -1;
	HistoryGoods hisGoods(iGoodsId);
	if (hisGoods.isSoul())
	{
		if(mActorChipByGoodsId.count(iGoodsId))
		{
			iNetId = mActorChipByGoodsId[iGoodsId]->id;
		}
	}
	else if (hisGoods.isPiece())
	{
		if (mEqChipByGoodsId.count(iGoodsId))
		{
			iNetId = mEqChipByGoodsId[iGoodsId]->id;
		}
	}
	return iNetId;
}

void ChipSoulDate::ReduceTotalChipNum(int iGoodsId, int iNUm)
{
	if (mEqChipByGoodsId.find(iGoodsId) != mEqChipByGoodsId.end())
	{
		mEqChipByGoodsId[iGoodsId]->mNum -= iNUm;
	}
	else if (mActorChipByGoodsId.find(iGoodsId) != mActorChipByGoodsId.end())
	{
		mActorChipByGoodsId[iGoodsId]->mNum -= iNUm;
	}
}

ChipSoulDate::ChipSoulDate()
{

}
ChipSoulDate::~ChipSoulDate()
{

}