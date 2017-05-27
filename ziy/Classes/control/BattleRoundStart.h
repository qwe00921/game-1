#ifndef _BattleRoundStart_H_
#define _BattleRoundStart_H_

enum
{
		//偏移消息ID
	 MSG_BONUS_PRESSED = 0,       //按下
	 MSG_BONUS_RELEASED = 1,      //释放
};
using namespace std;
#include "DString.h"

/*
* 标示我敌军阶段的回合数
*/
class BattleRoundStart : public TRControl
{
private:
	int relation;
	DString strWojunjieduan;
	DString strDidhuihe;
	DString strDijunjieduan;
	TRImagePtr imgRelation;
	TRImagePtr imgRound;
	TRImagePtr imgAir;
	map<int, TRImagePtr>imgNum;
public:
	BattleRoundStart(int relation,int msgBaseId);
	~BattleRoundStart();
	virtual void onPaint(DGraphics* g);
};
#endif