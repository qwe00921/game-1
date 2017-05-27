#ifndef __ACTORTALK_LAYER__
#define  __ACTORTALK_LAYER__
#include "cocos2d.h"
#include "stdio.h"
#include "cocos-ext.h"
#include "Common.h"
#include "main.h"
#include "RedeScreen.h"
#include "editor-support/cocostudio/CCArmature.h"

using namespace cocostudio;
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ActorTalkInfo
{
	CC_SYNTHESIZE(int, m_actorid, MACTORID);
	CC_SYNTHESIZE(int, m_direction, DIRECTION);
public:
	ActorTalkInfo(){
		talk_list_index = 0;
	};
	~ActorTalkInfo(){
		CCLOG("~ActorTalkInfo");
	};
	map<int, string> talk_list;
	int talk_list_index;
};

class ActorTalkElem
{
public:
	ActorTalkElem(){};
	~ActorTalkElem(){
		CCLOG("~ActorTalkElem");
	};

	CC_SYNTHESIZE(Sprite*,mImg,MIMG);
	CC_SYNTHESIZE(Armature*, mArmature, MARMATURE);
	CC_SYNTHESIZE(Sprite*, mStrBg, MSTRBG);
	CC_SYNTHESIZE(Label*, mName, MNAME);
	CC_SYNTHESIZE(Label*, mTalkStr, MTALKSTR);
	CC_SYNTHESIZE(int, mActorId, MACTORID);
	CC_SYNTHESIZE(bool, mTalking,MTALKING);
	CC_SYNTHESIZE(int,mPage,MPAGE);
	CC_SYNTHESIZE(int, mCurPage, MCURPAGE);
};

class ActorTalkLayer :public BTLayer
{
public:
	static ActorTalkLayer* create(string stringinfo,RedeScreen* ptrRead);
	bool init();
	void onExit();
	bool onTouchBegan(Touch *pTouch, Event *pEvent);
	void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
	ActorTalkLayer();
	~ActorTalkLayer();
	void GenerateTalkList(const char *buf, int actor_id);
	void GenerateTalkList(const char *buf);
	void nextTalk();
	void RemoveThis();
	void SetMaxPage(ActorTalkElem* ptrTalkElem);

	string mStringBuff;
	map<int, ActorTalkInfo*> string_list;
	int string_list_index;
 	ActorTalkElem left_talk;
 	ActorTalkElem right_talk;
	ActorTalkElem* mCurTalkElem;
	RedeScreen* mReadScreen;
	int m_iStringWidth;
	int m_iLineHeight;
};


enum
{
	SHOW_REDE_INFO//R场景历史介绍
};

class ShowInfoLayer :public BTLayer
{
public:
	static ShowInfoLayer* create(string infoStr, EventDelegate* ptrDelegate = NULL, int iType = SHOW_REDE_INFO);
	bool init(string infoStr, EventDelegate* ptrDelegate, int iType);
	ShowInfoLayer();
	~ShowInfoLayer();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	int m_iTotalPage;
	int m_iType;
	int m_iCurPage;
	int m_iLineHeight;
	bool m_bRemoveThis;
	EventDelegate* mDelegate;
	Label* m_labInfo;
};
#endif //(__ACTORTALK_LAYER__)