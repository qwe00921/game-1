#ifndef __TRGAME_PICTUREPOPLAYER_LAYER__
#define  __TRGAME_PICTUREPOPLAYER_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class PicturePopLayer :public BTLayer
{
public:
	static PicturePopLayer* create(string BgPathS, bool bIsGlobal);
	bool init(string BgPathS, bool bIsGlobal);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	PicturePopLayer(){};
	~PicturePopLayer(){};
};
#endif //(__TRGAME_PICTUREPOPLAYER_LAYER__)