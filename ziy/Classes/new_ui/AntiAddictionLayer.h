#ifndef __TRGAME_ANTIADDICTION_LAYER__
#define  __TRGAME_ANTIADDICTION_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
/************************************************************************/
/* 健康游戏说明	                                                        */
/************************************************************************/
class AntiAddictionLayer :public BTLayer
{
public:
	CREATE_FUNC(AntiAddictionLayer);
	bool init();
	void onExit();
	AntiAddictionLayer();
	~AntiAddictionLayer();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);

	void ShowLogoLayer(float f);
};
#endif //(__TRGAME_ANTIADDICTION_LAYER__)