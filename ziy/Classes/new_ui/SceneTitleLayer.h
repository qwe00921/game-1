#ifndef __TRGAME_SCENETITLELAYER__
#define  __TRGAME_SCENETITLELAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class SceneTitleLayer :public BTLayer
{
public:
	CREATE_FUNC(SceneTitleLayer);
	bool init();
	void onExit();
	SceneTitleLayer();
	~SceneTitleLayer();
	void UpdateRound();
	void updateTitleX(float f);
	void AutoUpdateTitleX(float f);

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	Label* cur_num;
	Label* battle_scene;
	Sprite* title_bg;

	bool enable_click;
	int title_offset_x;
	int title_show_state;
	int set_pointx_num;
	float fOffsetX;
};
#endif //(__TRGAME_SCENETITLELAYER__)