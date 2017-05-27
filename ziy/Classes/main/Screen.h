#ifndef _BASESCREEN_H_
#define _BASESCREEN_H_

#include "dvector.h"
#include "DControl.h"
#include "Message.h"
#include "DString.h"
#include "Graphics.h"

/**
* ScreenResponse 是所有Response的基类，当前用来处理计费返回
*/
class ScreenResponse
{
public:
	enum RESPONSE_TYPE
	{
		TYPE_PAY,
		TYPE_UPPER_SCREEN_CLOSE,
		TYPE_SHOW_EXIT_WINDOW,
		TYPE_REFRESH_ITUNES_PRICE,
		TYPE_BUY_LEVELS,
		TYPE_RESTORE_IOS_PRODUCT,
		TYPE_PAY_DOWNLOAD,
		TYPE_TRANSACTION_ORDER
	};
public:
	DString tradeId;
	bool isSucceed;
	DString resultMessage;
public:
	bool isTypePay() const
	{
		return mType == TYPE_PAY;
	}


	
private:
	RESPONSE_TYPE mType;
public:
	ScreenResponse(RESPONSE_TYPE type);
	virtual ~ScreenResponse();
};

/**
 * Screen 是所有界面的基类
 */
class Screen
{
public:	
	enum
	{
		/**
		* 按钮返回
		*/
		KEY_CODE_BACK = -1
	};
public:
	int frames;	
	/**
	* 当前激活的控件
	*/
    TRControlPtr activeControl;
	/**
	* 是否可见
	*/
	int visible;
	/**
	* 屏幕id
	*/
	const SCREEN_ID screenId;
public:
	/**
	* 构造函数
	* @warning 注意没有提供虚析构函数
	*/
	Screen(SCREEN_ID scrId);

	/*
	* 析构函数
	*/
	virtual ~Screen();

	/**
	* 获得当前激活的控件
	* @return 激活的控件，可能为 NULL
	*/
	TRControlPtr getActiveControl();

	/**
	* 设置当前激活的控件
	* @param ctrl 激活的控件，可以为 NULL
	*/
	void setActiveControl(TRControlPtr ctrl);

	/**
	* 添加控件到当前屏幕
	* @param ctrl 被添加的控件
	*/
	void appendControl(TRControlPtr ctrl);

	/**
	* 添加控件到当前屏幕，并设置为激活
	* @param ctrl 被添加的控件
	*/
	void appendActiveControl(TRControlPtr ctrl);

	/**
	* 从当前屏幕删除控件，如果控件为激活控件，则设置激活控件为 NULL
	* @param ctrl 被删除的控件
	*/
	void removeControl(TRControlPtr ctrl);

	/**
	* 从当前屏幕删除所有控件
	*/
	void removeAllControl();

	/**
	* 从屏幕队列中删除指定屏幕
	* @param gameScreen 被删除的屏幕
	* @param changeActiveScreen 是否修改激活的屏幕
	*/
	void removeScreen(ScreenPtr gameScreen, int changeActiveScreen){
		chess->removeScreen(gameScreen,changeActiveScreen);
	}

	/**
	* 添加制定屏幕到屏幕队列中
	* @param gameScreen 被添加的屏幕
	*/
	void appendScreen(ScreenPtr gameScreen){
		chess->appendScreen(gameScreen);
	}

public:
	/**
	* 在 appendScreen 时调用
	*/
	virtual	void onAppend();

	/**
	* 在 removeScreen 时调用
	*/
	virtual	void onRemove();

	/**
	* 绘制
	*/
	virtual	void onPaint(DGraphics* g);

	/**
	* 系统定时器回调
	*/
	virtual void onSysTimer();	

	/**
	* 按下按键
	* @param keyCode 键值
	*/
	virtual	int onKeyPressed(int keyCode);

	/**
	* 弹起按键
	* @param keyCode 键值
	*/
	virtual	int onKeyReleased(int keyCode);

	/**
	* 按下触笔
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int pointerPressed(int x, int y);

	/**
	* 弹起触笔
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int pointerReleased(int x, int y);

	/**
	* 触笔拖动
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int pointerDragged(int x, int y);

	/**
	* 触笔按下拖动
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int pointerPressDragged(int x, int y);

	/**
	* 鼠标右键按下
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int RpointerPressed(int x, int y);

	/**
	* 鼠标右键弹起
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual	int RpointerReleased(int x, int y);

	/**
	* 鼠标点击
	* @param x X坐标
	* @param y Y坐标
	*/
	virtual int pointerClicked(int x, int y);

	/**
	* 收到消息
	* @param message 消息
	*/
	virtual	void onMessage(MessagePtr message);

	/**
	* 收到响应
	* @param res 响应
	* @return 1表示已经处理，0表示不处理，继续往下传递
	*/
	virtual int onResponse(ScreenResponse* res);
	
	// 获取chess对象
	virtual ChessPtr getChess();
	virtual void setLayerVisible(bool bVisible);
protected:
	/**
	* 处理定时器
	*/
	virtual void handleTimer();
	/**
	* 控件列表
	*/
	DList<TRControl>* controlList;
	/**
	* 待删除的控件
	*/
	DList<TRControl>* deleteCtrlList;
	/**
	* 通过 Chess 访问 ScreenList
	*/
	ChessPtr chess;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	Screen(const Screen&);
	Screen& operator=(const Screen&);
};


#endif //#ifndef _BASESCREEN_H_