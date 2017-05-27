#ifndef __TRGAME_ACTORJOINOWNLAYER__
#define  __TRGAME_ACTORJOINOWNLAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "editor-support/cocostudio/CCArmature.h"

using namespace cocostudio;

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum
{
	FUNCTION_JOIN,
	FUNCTION_ASCENDSTAIRS
};

class ActorJoinOwnLayer :public BTLayer
{
public:
	static ActorJoinOwnLayer* create(int iActorId, int iFunctionType = FUNCTION_JOIN, EventDelegate* ptrDelegate = NULL);
	bool init(int iActorId, int iFunctionType, EventDelegate* ptrDelegate);
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	void onExit();
	void onEnter();
	ActorJoinOwnLayer();
	~ActorJoinOwnLayer();
	void addSkillElem(Sprite* ptrBg);
	void addGrownElem(Sprite* ptrBg);
	void addAbilityGrownElem(Sprite* ptrBg,char* ptrBbilityName,float iNumOld,float iNumNew,int iOffset);
	void updateEnableRemove(float f);

	int mActorId;
	EventDelegate* mDelegate;
	bool bEnableRemove;
};
#endif //(__TRGAME_ACTORJOINOWNLAYER__)