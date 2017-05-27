#ifndef _CHESSREDESCREEN_H_ 
#define _CHESSREDESCREEN_H_

/*
* 出战界面、剧情界面
*/

#include "RedeMenuLayer.h"
#include "MenubarLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ChoiceBoxLayer.h"

using namespace cocos2d::extension;
using namespace cocos2d;

class RedeLayer : public BTLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RedeLayer);
};

enum
{
	REDE_STATE_NORMAL,
	REDE_STATE_ACTOR_MOVE,
	REDE_STATE_ACTOR_SHOW_MENU_VISIBLE,
	REDE_STATE_WAIT_COMMAND,
	REDE_STATE_MAP_NORMAL,
	REDE_STATE_MAP_FACE_APPEAR,
	REDE_STATE_MAP_FACE_MOVE,
	REDE_STATE_MAP_FACE_DISAPPEAR,
	REDE_STATE_MAP_TELL_INFO,
	REDE_STATE_START_BATTLE
};
enum
{
	CTRL_BUTTON2 = 20,
	CTRL_BUTTON3 = 30,
	CTRL_BUTTON4 = 40,
	CTRL_BUTTON5 = 50,
	CTRL_BUTTON6 = 60,
	CTRL_BUTTON7 = 70,
	CTRL_BUTTON8 = 80,
	CTRL_BUTTON9 = 90,
	CTRL_BUTTON_START_BATTLE = 100,
	CTRL_BUTTON_ARMOR = 110,
	CTRL_BUTTON_BUY = 120,
	CTRL_BUTTON_SELL = 130,
	CTRL_BUTTON_VIPGIFT = 140,
	CTRL_BUTTON_CHALLENGE = 160,
	CTRL_BUTTON_SMITHING = 170,
	CTRL_BUTTON_FRUIT = 180,
	CTRL_BUTTON_MAINMENU = 200
};
class RedeScreen : public Screen,public EventDelegate
{
public:
	RedeScreen();
	~RedeScreen();
	void setMenuVisible(bool bVisible);
    void hideMenu(bool hide);
	void gameEnding();//结局界面
	void InitActorQueue();
	void onRemove();
	void onPaint(DGraphics* g);
	void paintNormal(DGraphics* g);
	void paintMap(DGraphics* g);
	void handleTimer();
	void onMessage(MessagePtr message);
	int pointerClicked(int x, int y);
	int pointerPressed(int x, int y);
	int RpointerPressed(int x, int y);
	int pointerDragged(int x, int y);
	bool GetMenuVisible();
	void setLayerVisible(bool bVisible);
	void ChildCloseLayer(int iNomal);
	void SetScreenName(string strName);
private:
	void handleActorMove();
public:
	int chapter_index;
	DString chapter_name;
	int rede_state;
	DList<HistoryActor>* moveList;
	Layer* talk_layer;
	HistoryActorPtr cur_actor;
	TRImagePtr talk_img;
	ScriptHandler handler;
	int menu_visible;
	DString buf[3];
	int cur_index;
	int cur_line;
	int wait_click;
	bool startTalk;
	bool firstTalk;
	int paint_bust_offset;
	RedeLayer* m_pRedeLayer;
	RedeMenuLayer* m_pMenuLayer;
	MenuBarLayer* m_pBarLayer;
	ChoiceBoxLayer* m_pChoiceBox;
private:
	HistoryActorPtr cur_point_actor;
	HistoryActorPtr* ActorQueue;
	int actor_x_old;
	int actor_y_old;
	int actor_last_direction;
	int isCountFreeMove;
	int move_step;
	bool isRPressed; //是否是鼠标长按
	Sprite* m_ScreenNameBgSp;
	Label* m_ScreenNameLab;
};
#endif 