#ifndef __DESTROYSIXCOUNTRIES_LAYER__
#define  __DESTROYSIXCOUNTRIES_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "TitleBarLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class DestroySixCountriesInfo
{
	CC_SYNTHESIZE(Label*,mName,MNAME);
	CC_SYNTHESIZE(Sprite*, mLine, LINE);
	CC_SYNTHESIZE(Sprite*, mCity, CITY);
	CC_SYNTHESIZE(Sprite*, mStar1, STAR1);
	CC_SYNTHESIZE(Sprite*, mStar2, STAR2);
	CC_SYNTHESIZE(Sprite*, mStar3, STAR3);
	CC_SYNTHESIZE(int, mIndex, INDEX);
	CC_SYNTHESIZE(Sprite*, mRank, RANK);
};

class DestroySixCountriesLayer :public BTLayer
{
public:
	static DestroySixCountriesLayer* create(int iMapWarId = -1,int iModel = NORMAL_MODEL);
	bool init(int iMapWarId,int iModel = NORMAL_MODEL);
	void onExit();
	DestroySixCountriesLayer();
	~DestroySixCountriesLayer();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);

	void AddMapElement();
	void setMapOffset();
	void setTotalStarNum();
	void setMap();
	void updateShow();
	char* getMapName();
	void showGKinfo();
	void showRankList();
	void setCurScene();
	void updateMoneyShow();
	void updataModel(int iModel);
	int GetModelTotalMap(int iModel,int& iStartSceen);

	void clickMenuPre(Ref* sender);
	void clickMenuNext(Ref* sender);
	void clickMenuBack(Ref* sender);
	void ChildCloseLayer(int iNomal);
	void clickMenuActScene(Ref* sender);
	void clickYXZMenu(Ref* sender);
	void clickOrdinaryMenu(Ref* sender);
	void clickDifficultyMenu(Ref* sender);

	Sprite* mMap;
	Sprite* mMountain;
	Sprite* mCloud;
	Label* c_name;
	Sprite* infoTitleSp;

	DSize mSize;
	int scene_index;
	int map_width;
	int map_height;
	float mVelocity;
	bool enable_click;
	int war_index;
	int rank_index;
	int get_star_num;
	int total_star_num;
	map<int, DestroySixCountriesInfo*> mElem;
	int total_scene;
	int start_scene;
	int mMapWarId;
	int mCurMapIndex;
	MenuItemSprite* miS;
	MenuItemSprite* miS1;
	Sprite* jxbx;
	Label* jx_num;
	int mSignWarId;
	Menu* mDMenu;
	Menu* mOMenu;

	int mNdModel;
};
#endif //(__DESTROYSIXCOUNTRIES_LAYER__)