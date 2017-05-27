#ifndef __GET_STAR_LAYER__
#define  __GET_STAR_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class GetStarLayer :public BTLayer
{
public:
	static GetStarLayer* create(int iIndex,int is_battle=0);
	bool init();
	void onExit();
	GetStarLayer();
	~GetStarLayer();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	int mIndex;
	int m_isButtle;
};
#endif //(__TRGAME_ACTION_LAYER__)