#ifndef __BATTLETITLE_LAYER__
#define  __BATTLETITLE_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BattleTitleLayer :public BTLayer
{
public:
	CREATE_FUNC(BattleTitleLayer);
	bool init();
	BattleTitleLayer();
	~BattleTitleLayer();
};
#endif //(__BATTLETITLE_LAYER__)