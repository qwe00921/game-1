#ifndef __TRGAME_ACTOR_OCCU_INFO_LAYER__
#define  __TRGAME_ACTOR_OCCU_INFO_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"

#include "Common.h"
#include "main.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class OccuSeidElem
{
	CC_SYNTHESIZE(Sprite*, m_bg,BG);//背景
	CC_SYNTHESIZE(Label*, m_name,SNAME);//特技名字
	CC_SYNTHESIZE(Sprite*, m_info,INFO);//特技说明
};

class ActorOccuInfoLayer:public BTLayer
{
public:
	static ActorOccuInfoLayer* create(HistoryActorPtr ptrActor);
	bool init(HistoryActorPtr Actor);
	ActorOccuInfoLayer();
	~ActorOccuInfoLayer(){}
	void onExit();

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void updateArmsImg(float fNum);
	void addSeidElem();
	void ArmsAttackAnimation(float fNum);
private:
	HistoryActorPtr curActor;
	map<int,OccuSeidElem*> OccuSeid;
	int selectOccuSeidNum;
	Sprite* arms;
	Sprite* arms_fashion;
	int fashion_width;
	int arms_width;
	int arms_rect;
	int click_arms;
};


#endif //(__TRGAME_ACTOR_OCCU_INFO_LAYER__)