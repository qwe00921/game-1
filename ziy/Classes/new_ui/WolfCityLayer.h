#ifndef __TRGAME_WOLFCITY_LAYER__
#define  __TRGAME_WOLFCITY_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class WolfCityLayer :public BTLayer
{
public:
	static WolfCityLayer* create();
	bool init();
	WolfCityLayer();
	~WolfCityLayer();

	void clickMenuBack(Ref* sender);
	void ChildCloseLayer(int iNomal);
private:
	map<int, int> MapOPenData;
};
#endif //(__TRGAME_WOLFCITY_LAYER__)