#include "Common.h"
#include "main.h"

void TRControl::move(int x,int y)
{
	rect.x +=x;
	rect.y +=y;
}

void TRControl::setOwner(ScreenPtr ctrlOwner)
{
    owner = ctrlOwner;
}
int TRControl::pointerPressed(int x, int y) 
{
	return 0;
}
int TRControl::pointerReleased(int x, int y)
{
	return 0;
}
int TRControl::RpointerPressed(int x, int y) 
{
	return 0;
}
int TRControl::RpointerReleased(int x, int y)
{
	return 0;
}
int TRControl::pointerDragged(int x, int y)
{
	return 0;
}
int TRControl::pointerPressDragged(int x, int y)
{
	return 0;
}
int TRControl::pointerClicked(int x, int y)
{
	return 0;
}
void TRControl::onPressed(int keyCode)
{
}
void TRControl::onReleased(int keyCode)
{
}
void TRControl::onPaint(DGraphics* g)
{
}
void TRControl::handleTimer()
{
	frames++;	
}
TRControl::TRControl(DRectPtr rect)
{
	this->visible = 1;
	this->focus = 0;
	this->enable = 1;
	this->cangetfocus = 1;
	this->rect.x = rect->x;
	this->rect.y = rect->y;
	this->rect.width = rect->width;
	this->rect.height = rect->height;
	this->frames = 0;
	this->owner = NULL;
	this->msgBaseId = -1;

}

TRControl::TRControl()
{
	this->visible = 1;
	this->focus = 0;
	this->enable = 1;
	this->cangetfocus = 1;
	this->frames = 0;
	this->owner = NULL;
	this->msgBaseId = -1;
}

TRControl::~TRControl()
{
}