#ifndef __TRGAME_ROMANCE_LAYER__
#define  __TRGAME_ROMANCE_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "TitleBarLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocostudio;


class RomanceLayer :public BTLayer
{
public:
	static RomanceLayer* create(bool bEnablePlayMp3 = true);
	bool init(bool bEnablePlayMp3);
	RomanceLayer();
	~RomanceLayer();

	void clickMenuBack(Ref* sender);
	void ChildCloseLayer(int iNomal);
private:
	TitleBarLayer* mTitleBar;
	map<int, int> MapOPenData;
};
#endif //(__TRGAME_ROMANCE_LAYER__)