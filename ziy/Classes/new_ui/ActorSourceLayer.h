#ifndef __TRGAME_ACTORSOURCELAYER__
#define  __TRGAME_ACTORSOURCELAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ActorSourceElem
{
	CC_SYNTHESIZE(Sprite*,mEbg,MEBG);
	CC_SYNTHESIZE(int, MEWarIndex, MEWINDEX);
	CC_SYNTHESIZE(int, MEGetType, MEGETTYPE);
};

class ActorSourceLayer :public BTLayer
{
public:
	static ActorSourceLayer* create(int iActorId, int iCurrentSoul,int iNeedSoul, EventDelegate* ptrDelegate);
	bool init(int iCurrentSoul, int iNeedSoul);
	void onExit();
	ActorSourceLayer();
	~ActorSourceLayer();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void addElement(Sprite* ptrBg);

	int mActorId;
	EventDelegate* mDelegate;
	map<int, ActorSourceElem*> elem;
	Sprite* bg;
	bool bRemoveThis;
	int mClickElemIndex;
};
#endif //(__TRGAME_ACTORSOURCELAYER__)