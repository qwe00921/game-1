#ifndef _CHESSSCENESCREEN_H_ 
#define _CHESSSCENESCREEN_H_
/*
* 战斗界面
*/
#include "RedeMenuLayer.h"
#include "MenubarLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SceneTitleLayer.h"
#include "SelectActionLayer.h"
#include "BattleLayer.h"
#include "BattleSprite.h"
#include "TalkLayer.h"
#include "ChoiceBoxLayer.h"

using namespace cocos2d::extension;
using namespace cocos2d;

enum
{
	MSG_COMFORM_YES = 0,       //按下
	MSG_COMFORM_NO = 1,      //释放
	CTRL_MINI_MAP_BUTTON = 90,
	CTRL_MINI_MAP = 100,
	CTRL_BONUS = 130,
	SCR_TEST_ASK = 220,
    MSG_BOTTOM_MENUBAR = 260,
};

enum {
    IS_SHOWN = 0,
    IS_HIDE = 1
};

class BattleScreen :public Screen, public EventDelegate
{
private:
	int last_x;
	int last_y;
	int point_x;
	int point_y;
    int m_iFinishCurRoundReason;
	int randHint; //随机提示下标
	int need_show_land;
	int enable_show_land;
	TRImagePtr imgBG;
	TRImagePtr imgPoint;
	TRImagePtr imgAttackArea;
	TRImagePtr imgEffectArea;
	TRImagePtr imgLoseBG;
	TRImagePtr img_mini_map;
	TRImagePtr scene_title_bg;
	TRImagePtr imgAttackRegion;
	TRImagePtr imgMoveRegion;
	TRImagePtr imgMoveRegionFriend;
	TRImagePtr imgBossIcon;
	ButtonPtr buttonMiniMap;
	int isPaintBust;
public:
	int battle_sec;
	MiniMapPtr mini_map;
	DRect rect_highlight;
	int highlight_frame;
	LandformAttrPtr curLandForm;
	HistoryActorPtr curPointActor;
	BattleControler handler;
	TRControlPtr bonus;
	DPointPtr pointList;
	int pointNum;
	SelectActionLayer* ptrSelectAction;
	bool bNotTriggerClick;
	SceneTitleLayer* ptrSceneTitle;
	Layer* mFunctionLayer;
	map<int, string> WinStrList;
	map<int, string> LostStrList;
	map<string, TRImagePtr> m_mTranslucentImg;
public:
	BattleScreen();
	~BattleScreen();
	void onAppend();
	void onRemove();
	void onPaint(DGraphics* g);	
	int pointerPressed(int x, int y);
	int pointerClicked(int x, int y);
	int pointerPressDragged(int x, int y);
	int pointerDragged(int x, int y);
	int RpointerPressed(int x, int y);
	void onMessage(MessagePtr message);
	void nextRound();
    void autopointClicked(int x,int y);
	void resetWindow();	
	void initControl();	
	void setMiniMapVisible(bool bVisible);
	void RunFunction(int iNomal);
	void updateRTDate(int iNomal);
	void ChildCloseLayer(int iNomal);
	void SetHighLightArea();
	void addArmBuff(HistoryActor* ptrActor, int iBuffid);
	void ResetBattleScreenState();
	void AddBattleScreenShield();
	void updateWeather();
	void updateWeatherScale(float fScale);
private:
	void paintBG(DGraphics* g);
	void paintActor(DGraphics* g);
	void paintMapElem(DGraphics* g);
	void paintMoveRegion(DGraphics* g,bool bOnlyMove = false);
	void paintAttackRegion(DGraphics* g);
	void paintActionGain(DGraphics* g);//绘制行动伤害
	void paintTranslucentImg(DGraphics* g);
	void handleTimer();
	void InitFullScreenPoint(); //初始化全屏攻击的Point
	void PainBustsetbuttonvisible(int visible);
	void updateNotice(string strNotice);
	void paintBossIcon(DGraphics* g);
	void showActorInfo();//显示武将信息
public:
	BattleLayer* m_pBattleLayer;
	MenuBarLayer* m_pBatBarLayer;
	TalkLayer* m_pTalkLayer;
	ActionGainSprite* m_spActGain;
	LabelSprite* m_spLabel;
	ChoiceBoxLayer* m_pChoiceBox;
	Armature* m_armBuff;
private:
	ActorInfoSprite* m_spActorInfo;
	LandInfoSprite* m_spLandInfo;
	Screen* mShieldScreen;
	Armature* m_armWeather;
};

#endif