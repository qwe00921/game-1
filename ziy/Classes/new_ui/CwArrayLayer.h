#ifndef __TRGAME_CWARRAY_LAYER__
#define  __TRGAME_CWARRAY_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CwArrayElem
{
	CC_SYNTHESIZE(Sprite*,eBg,eBG);
	CC_SYNTHESIZE(int,eActorId,eACTORID);
	CC_SYNTHESIZE(int,eMapX,eMAPX);
	CC_SYNTHESIZE(int, eMapY, eMAPY);
	CC_SYNTHESIZE(Sprite*,eActorSImg,eACTORSIMG);
	CC_SYNTHESIZE(Sprite*, eActorSFashion, eACTORSFASHION);
};

class CwArrayLayer :public BTLayer
{
public:
	static CwArrayLayer* create(int iWarId,EventDelegate* ptrDelegate);
	bool init(int iWarId, EventDelegate* ptrDelegate);
	CwArrayLayer();
	~CwArrayLayer();

	void onEnter();
	void onExit();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void LoadSFile();

	void clickMenuBack(Ref* sender);
	void RunFunction(int iActorId);
	map<int, int> GetActorIdList();
private:
	int m_iWarId;
	EventDelegate* mDelegate;
	ScriptHandler handler;
	Sprite* mBg;
	int m_iBgWidth;
	int m_iBgHeight;

	map<int, CwArrayElem*> mElemList;
	DSize mSize;
	int mClickIndex;
};
#endif //(__TRGAME_CWARRAY_LAYER__)