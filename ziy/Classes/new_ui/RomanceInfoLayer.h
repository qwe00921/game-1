#ifndef __TRGAME_ROMANCEINFO_LAYER__
#define  __TRGAME_ROMANCEINFO_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class RomanceInfoLayer :public BTLayer
{
public:
	static RomanceInfoLayer* create(int iRMapId,EventDelegate* ptrDelegate,bool bIsAddSureM = true);
	bool init(int iRMapId,EventDelegate* ptrDelegate, bool bIsAddSureM = true);
	RomanceInfoLayer();
	~RomanceInfoLayer();

	void AddGoodsElem(Sprite* BgSp);

	void clickCloseLayer(Ref* sender);

	void clickSureMenu(Ref* sender);
private:
	void SendNetData();
	void onHttpRequestCompleted(cocos2d::Node *sender, void *resultdata);

	int mMapId;
	EventDelegate* mDelegate;
};
#endif //(__TRGAME_ROMANCEINFO_LAYER__)