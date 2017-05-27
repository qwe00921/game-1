#ifndef __TRGAME_SUITINFO_LAYER__
#define  __TRGAME_SUITINFO_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocostudio;

class SuitInfoElem
{
public:
	SuitInfoElem()
	{
		iIndex = 0;
		weaponS = "";
		coatS = "";
		assistS = "";
	}
private:
	CC_SYNTHESIZE(int,iIndex,eIndex);
	CC_SYNTHESIZE(int, iIsActorSuit, eIsActorSuit);
	CC_SYNTHESIZE(string, weaponS, eWeaponS);
	CC_SYNTHESIZE(string, coatS, eCoatS);
	CC_SYNTHESIZE(string, assistS, eAssistS);
	CC_SYNTHESIZE(string, weaponName, eWeaponName);
	CC_SYNTHESIZE(string, coatName, eCoatName);
	CC_SYNTHESIZE(string, assistName, eAssistName);
	map<int, int> mSeid;
	map<int, int> mActorSeid;
	CC_SYNTHESIZE(string, SuitInfoS, eSuitInfoS);
	CC_SYNTHESIZE(string, suitnameS, eSuitNameS);
};

class SuitShowElem
{
	CC_SYNTHESIZE(Label*,mNameLab,eNameLab);
	CC_SYNTHESIZE(Sprite*, mWeaponBg, eWeaponBg);
	CC_SYNTHESIZE(Sprite*, mWeaponImg, eWeaponImg);
	CC_SYNTHESIZE(Label*, mWeaponName, eWeaponName);
	CC_SYNTHESIZE(Sprite*, mCoatBg, eCoatBg);
	CC_SYNTHESIZE(Sprite*, mCoatImg, eCoatImg);
	CC_SYNTHESIZE(Label*, mCoatName, eCoatName);
	CC_SYNTHESIZE(Sprite*, mAssistBg, eAssistBg);
	CC_SYNTHESIZE(Sprite*, mAssistImg, eAssistImg);
	CC_SYNTHESIZE(Label*, mAssistName, eAssistName);
	CC_SYNTHESIZE(Label*, mSeidInfo1Lab, eSeidInfo1Lab);
	CC_SYNTHESIZE(Label*, mSeidInfo2Lab, eSeidInfo2Lab);
	CC_SYNTHESIZE(Label*, mActorInfoLab, eActorInfoLab);
	CC_SYNTHESIZE(Label*, mActorSuitLab, eActorSuitLab);
};

class SuitInfoLayer :public BTLayer
{
public:
	static SuitInfoLayer* create(int iGoodsId);
	bool init(int iGoodsId);
	SuitInfoLayer();
	~SuitInfoLayer();
	void GetDataElem();
	void UpdateElemShow();

	void clickProMenu(Ref* sender);
	void clickNextMenu(Ref* sender);
	void clickCloseMenu(Ref* sender);
public:
	int mGoodsId;
	int mCurElemIndex;
	map<int, SuitInfoElem*> mDataElem;
	SuitShowElem mShowElem;
	Sprite* bg;
	map<int, Sprite*> mElemDot;
	Sprite* mCurDotSp;
};
#endif //(__TRGAME_SUITINFO_LAYER__)