#ifndef _CHESS_GAME_MAIN_H_
#define _CHESS_GAME_MAIN_H_

#include "cocos2d.h"
#include "Common.h"
#include "Message.h"
#include "BaseClass.h"

class StatConfig;
class ScreenResponse;
class DThreadRun;


USING_NS_CC;
typedef struct _ChessString_
{
	const char * str;
	int x;
	int y;
	int color;
}ChessString,*ChessStringPtr;


class ChessLayer : public BTLayer
{
public:
	ChessLayer();
	~ChessLayer();

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	virtual bool onTouchBegan(Touch *pTouche, Event *pEvent);

	virtual void onTouchMoved(Touch *pTouche, Event *pEvent);

	virtual void onTouchEnded(Touch *pTouche, Event *pEvent);
	CREATE_FUNC(ChessLayer);
};

class ChessApplication : public cocos2d::Application
{
public:
	ChessApplication();
	virtual ~ChessApplication();

	virtual void initGLContextAttrs();

	/**
	@brief    Implement CCDirector and CCScene init code here.
	@return true    Initialize success, app continue.
	@return false   Initialize failed, app terminate.
	*/
	virtual bool applicationDidFinishLaunching();

	/**
	@brief  The function be called when the application enter background
	@param  the pointer of the application
	*/
	virtual void applicationDidEnterBackground();

	/**
	@brief  The function be called when the application enter foreground
	@param  the pointer of the application
	*/
	virtual void applicationWillEnterForeground();
public:

	static ChessApplication* getInstance();
public:
	void setChessLayerPointX(float x);
	void setChessLayerPointY(float y);
	bool isChessLayerTouchEnabled();
	void AddFunctionLayer(Layer* ptrLayer);
	void removeAllFunctionLayer();
	Layer* getFounctionLayerByTag(int layerTag);
	Layer* GetActionLayer();

private:
	static ChessApplication* instance;
	Scene *scene;
	ChessLayer *chessLayer;
	Layer* FunctionLayer;
	Layer* mActionLayer;
};

class Chess
{
public:
	Chess();
	~Chess();
public: 
	bool isNeedGuide;//新手指引开关

	int auto_run1;
	int is_load;
	int r_point_pressed;
	int l_point_pressed;
	int point_moved;
	DVector<int>* choice;
	int testAsk;
	char *talk_buf;
	StatConfig* statConfig;
	void SetTalkBuf(char* buf);
	void ClearTalkBuf();

	void didFinishLaunching();	
	void convertXY(int& x, int& y);
	void OnTimer();
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnPointerPressed(int x, int y);
	void OnPointerReleased(int x, int y);
	void OnPointerPressDragged(int x, int y);
	void OnPointerDragged(int x, int y);
	void OnRPointerPressed(int x, int y);
	void OnRPointerReleased(int x, int y);
    void OnMsg(MessagePtr message);
	/**
	* 收到响应，转给 Screen 处理
	*/
	void OnResponse(ScreenResponse* res);
	void ExitGame();
	void OnPaint();
/////////////////////////////////////////////////////////////////////////

	/**
	 * 删除 Screen 
	 */
	void removeScreen(ScreenPtr gameScreen, int changeActiveScreen);
	/**
	* 添加 Screen
	*/
	void appendScreen(ScreenPtr gameScreen);
	ScreenPtr getScreenByID(int scr_id);
	/**
	* 删除所有 Screen
	*/
	void removeAllScreen(int screen_id);
	void removeAllScreen();
public:
	/**
	* 获得统计配置
	*/
	StatConfig* getStatConfig();
	/**
	* 获得线程同步类
	*/
	DThreadRun* getThreadRun();
	/**
	* 获得获得绘制对象
	*/
	DGraphics* getGraphics();
private:
	DList<Screen>* screenList;
	DList<Screen>* deleteScreenList;
	ScreenPtr activeScreen;
	ScreenPtr currentScreen;	
	DThreadRun* threadRun;
public:
	DGraphics* graph;
/////////////////////////////////////////////////////////////////////////
public:
	static ChessPtr getInstance(); 
private:   
	static ChessPtr _instance;
private:
	void Init();
};

#endif