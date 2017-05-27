#ifndef _CONTROL_H
#define _CONTROL_H


#include "Graphics.h"

class TRControl
{
public:
	TRControl();
	TRControl(DRectPtr rect);
	virtual ~TRControl();
	void setOwner(ScreenPtr ctrlOwner);
	virtual	int pointerPressed(int x, int y);
	virtual	int pointerReleased(int x, int y);
	virtual	int RpointerPressed(int x, int y);
	virtual	int RpointerReleased(int x, int y);
	virtual	int pointerDragged(int x, int y);
	virtual	int pointerPressDragged(int x, int y);
	virtual int pointerClicked(int x, int y);
	virtual	void onPressed(int keyCode);
	virtual	void onReleased(int keyCode);
	virtual	void onPaint(DGraphics* g);
	virtual void handleTimer();
	virtual void move(int x,int y);
public:
	int frames;
	char visible;
	char focus;
	char cangetfocus;
	char enable;
	int msgBaseId;

	DRect rect;
	ScreenPtr owner;
private:
	/*
	* 禁止对象拷贝，声明私有方法并且不实现
	*/
	TRControl(const TRControl&);
	TRControl& operator=(const TRControl&);
};

#endif