#ifndef _BUTTON_H_
#define _BUTTON_H_

/////////////////////////////////////////////////////////////////////////
//按钮控件

#define COMMON_BUTTON_HEIGHT 30

enum //偏移消息ID
{
	MSG_BUTTON_PRESSED = 0,               //按下
	MSG_BUTTON_RELEASED = 1,             //释放
};

enum  //按钮显示状态
{
	BUTTON_STATE_NORMAL,
	BUTTON_STATE_PREMOVE,
	BUTTON_STATE_LMOVEING,
	BUTTON_STATE_RMOVEING,
	BUTTON_STATE_SHOWALWAYS
};

class Button : public TRControl
{
private:
	DString caption;
	int isPressed;
	TRImagePtr leftImg;
	TRImagePtr rightImg;
	TRImagePtr leftDisableImg;
	bool drawButtonRect;
	int textColor;
public:
	Button(DRectPtr rect, const char* caption, int msgBaseId, TRImagePtr leftImg, TRImagePtr rightImg);
	Button(DRectPtr rect, const char* caption, int msgBaseId);
	~Button();
	virtual	int pointerPressed(int x, int y);
	virtual	int pointerReleased(int x, int y);
	virtual	int pointerPressDragged(int x, int y);
	virtual void onPaint(DGraphics* g);
	/**
	* 是否绘制按键边框
	*/
	void setDrawButtonRect(bool b);
	/**
	* 设置字体颜色（不设为黑色）
	*/
	void setTextColor(int color);
	/**
	* 设置按钮不可用时图片
	*/
	void setLeftDisableImage(TRImagePtr disImg);
	/**
	* 设置按钮文字
	*/
	void setCaption(DString text);
	void SetNormalImage(TRImagePtr normalImg);
public:
	int show_state;    //显示状态
	int wait_move;    //等待移动时间
	int move_speed;   //按钮移动速度， 默认为1
	bool isRmove;     //是否是右移  true 向右收  false 向左收
	bool isYmove;     //是否是Y轴平移
	void UpdateBtnState(int ledge_x, int redge_x);
};

#endif // _BUTTON_H_