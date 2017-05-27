#ifndef __FUNCTIONCLASS__
#define __FUNCTIONCLASS__
#include "cocos2d.h"
#include "cocos-ext.h"

class ActorJoinOwnDate
{
	CC_SYNTHESIZE(int, mActorId, ActorId);
	CC_SYNTHESIZE(int, mLv, Lv);
	CC_SYNTHESIZE(int, mExp, Exp);
	CC_SYNTHESIZE(int, mFashion1, Fashion1);
	CC_SYNTHESIZE(int, mFashion2, Fashion2);
	CC_SYNTHESIZE(int, mGender, Gender);
	CC_SYNTHESIZE(int, mJobID, JobID);
	CC_SYNTHESIZE(int, mQuality, Quality);
	CC_SYNTHESIZE(int, mGrade, Grade);
	CC_SYNTHESIZE(Json::Value, mGoods, GoodsValue);
	CC_SYNTHESIZE(int, mSoulBox, SoulBox);
public:
	map<int, int> mSkillLv;
	map<int, double> m_mPractice;
	map<int, int> mSpecialItem;
	ActorJoinOwnDate()
	{
		mActorId = 0;
		mLv = 0;
		mExp = 0;
		mFashion1 = 0;
		mFashion2 = 0;
		mJobID = 0;
		mQuality = 0;
		mGrade = 0;
		mGender = 0;
		mSoulBox = 0;
	}
};
typedef ActorJoinOwnDate* ActorJoinOwnDatePtr;

#endif