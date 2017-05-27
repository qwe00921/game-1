#ifndef _PKScreen_H_ 
#define _PKScreen_H_

/*
* 自动单挑界面
*/

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d;

class PKLayer : public BTLayer
{
public:
	virtual bool init();
	CREATE_FUNC(PKLayer);
	void RunSpriteAction();
	Vec2 GetVSPosition();
public:
	Sprite * m_pSpriteVS;
};

typedef class PKActor
{
public:
	PKActor(HistoryActorPtr actor,int relation);
	~PKActor();
	void onPaint(DGraphics* g);
	int setState(int state);
	void setOppPKActor(PKActor* oppActor);
public:
	int frame;
	int direction;
	int state;
	int is_dead;
	int x;
	int y;
	int relation;
	int delay;
	HistoryActorPtr hisActor;
	PKActor* oppPKActor;
private:
	TRImagePtr imgMov;
	TRImagePtr imgAtk;
	TRImagePtr imgSpc;
	int iScale;
}* PKActorPtr;

class PKScreen : public Screen
{
public:
	PKScreen(int actor_id1, int actor_id2, int logoIndex);
	~PKScreen();
	void PKActorAttack1(short relation,short attack_type,short is_deadly);
	void PKActorAttack2(short relation,short attack_type,short is_hitted);
	void PKActorDie( short relation);
	void PKActorTalk( short relation,DString buf,short delay);
	void PKEnd();
	void PKShowKO();
	void PKActorAction(int relation,int action);
	void PKActorAppear(int relation,int action,DString str);
	void onPaint(DGraphics* g);
	void handleTimer();
	void onMessage(MessagePtr message);
	void pointReleased(int x, int y);
private:
	DRect rect;
	int pk_state;
	int need_frame;
	int need_frame1;
	int action1;
	int action2;
	int action3;
	int action4;
	int pre_state;
	BattleScreenPtr battle_scr;
	DString talk_buf;
	PKActorPtr actor_own;
	PKActorPtr actor_oppo;
	PKActorPtr cur_actor;
	PKActorPtr other_actor;
	TRImagePtr m_pScreenBG;
	TRImagePtr nameLabel;
	TRImagePtr imgVS;
	TRImagePtr face_own;
	TRImagePtr face_oppo;

	PKLayer* m_pPKLayer;
	int iLastVsX;
	int iLastVsY;
};
#endif 