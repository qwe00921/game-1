#ifndef __TRGAME_CHIPSOULDATE_
#define __TRGAME_CHIPSOULDATE_

#include "Common.h"
#include "jsons.h"

using namespace Json;
using namespace std;

class ChipSoulDateElem
{
public:
	ChipSoulDateElem()
	{
		mGoodsId = 0;
		mElemId = 0;
		mNum = 0;
		id = -1;
	}
public:
	short mGoodsId;
	short mElemId;
	int mNum;
	int id;
};

class ChipSoulDate
{
public:
	ChipSoulDate();
	~ChipSoulDate();
	int GetActorChipNumByActorId(int iActorId);
	void AddActorSoulNumByActorId(int iActorId, int iNUm, int id);
	void ReduceActorSoulNumByActorId(int iActorId, int iNUm);
	ChipSoulDateElem* GetSoulElemByActorId(int iActorId);
	map<int, int> getActorSoulListByActorId();

	int GetActorSoulNumByGoodsId(int iGoodsId);
	void AddActorSoulNumByGoodsId(int iGoodsId, int iNUm,int id);
	void ReduceActorSoulNumByGoodsId(int iGoodsId, int iNUm);
	ChipSoulDateElem* GetSoulElemByGoodsId(int iGoodsId);
	map<int, int> getActorSoulListByGoodsId();

	int GetEqChipNumByEqId(int iEqId);
	void AddEqSoulNumByEqId(int iEqId, int iNUm, int id);
	void ReduceEqSoulNumByEqId(int iEqId, int iNUm);
	map<int, int> getEqSoulListByEqId();

	int GetEqSoulNumByGoodsId(int iGoodsId);
	void AddEqSoulNumByGoodsId(int iGoodsId, int iNUm, int id);
	void ReduceEqSoulNumByGoodsId(int iGoodsId, int iNUm);
	map<int, int> getEqSoulListByGoodsId();

	void ClearChipList();

	void ReduceTotalChipNum(int iGoodsId, int iNUm);
	map<int, ChipSoulDateElem*> getChipSoulMap();
	int getNetIdByGoodsId(int iGoodsId);
private:
	map<int, ChipSoulDateElem*> mActorChipByActorId;
	map<int, ChipSoulDateElem*> mActorChipByGoodsId;

	map<int, ChipSoulDateElem*> mEqChipByEqId;
	map<int, ChipSoulDateElem*> mEqChipByGoodsId;
};
#endif /* __TRGAME_CHIPSOULDATE_ */
