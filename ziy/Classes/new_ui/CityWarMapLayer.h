#ifndef __TRGAME_CITYWARMAP_LAYER__
#define  __TRGAME_CITYWARMAP_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CwMapElem
{
	CC_SYNTHESIZE(int,eIndex,eINDEX);
	CC_SYNTHESIZE(Sprite*,eBg,eBG);
public:
	map<int, map<int,Armature*> > eCloud;
};

class CityWarMapLayer :public BTLayer
{
public:
	CREATE_FUNC(CityWarMapLayer);
	bool init();
	CityWarMapLayer();
	~CityWarMapLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	float m_fMapWidth;
	float m_fMapHeight;
	map<int, CwMapElem*> mMapElemList;
	Sprite* mMapElemBg;
	DSize mSize;
	float m_fScale;
};

class CityWarInfoLayer :public BTLayer, public ScrollViewDelegate
{
public:
	CREATE_FUNC(CityWarInfoLayer);
	bool init();
	CityWarInfoLayer();
	~CityWarInfoLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void clickMenuBack(Ref* sender);
	void clickMenuShrink(Ref* sender);
	void clickMenuEnlarge(Ref* sender);

private:
	ScrollView* mScrollview;
	float m_fScale;
	CityWarMapLayer* mSCLayer;
};
#endif //(__TRGAME_ACTION_LAYER__)