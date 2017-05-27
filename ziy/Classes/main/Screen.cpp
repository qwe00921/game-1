#include "Common.h"
#include "main.h"
#include "button.h"
#include "StatConfig.h"
#include "PopLayer.h"

ScreenResponse::ScreenResponse(RESPONSE_TYPE type)
{
	tradeId = "";
	isSucceed = 0;
	resultMessage = "";
	mType = type;
}

ScreenResponse::~ScreenResponse()
{
}

void Screen::onAppend()
{
}

void Screen::onRemove()
{
}

void Screen::onMessage(MessagePtr message)
{		
}

int Screen::pointerPressed(int x, int y) 
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->pointerPressed(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::pointerPressDragged(int x, int y) 
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->pointerPressDragged(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::pointerReleased(int x, int y)
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->pointerReleased(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::RpointerPressed(int x, int y) 
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->RpointerPressed(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::RpointerReleased(int x, int y)
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->RpointerReleased(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::pointerDragged(int x, int y)
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->pointerDragged(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::pointerClicked(int x, int y)
{
	int i;
	TRControlPtr ctrl;
	for(i = 0;i<controlList->size();i++)
	{
		ctrl = (TRControlPtr)controlList->elementAt(i);
		if(ctrl->visible)
		{
			if(ctrl->pointerClicked(x,y))
				return 1;
		}
	}
	return 0;
}
int Screen::onKeyPressed(int keyCode)
{
	if (KEY_CODE_BACK == keyCode)
	{		
		ChessExit();
		return 1;
	}
	
	if (activeControl != NULL)
	{
		activeControl->onPressed(keyCode);
	}

	return 0;
}

int Screen::onKeyReleased(int keyCode)
{
	if (activeControl != NULL)
	{
		activeControl->onReleased(keyCode);
	}
	return 0;
}

void Screen::onSysTimer()
{
	deleteCtrlList->removeAllElements();
	handleTimer();
}

void Screen::handleTimer()
{
	int i;
	TRControlPtr component = NULL;
    for (i = 0; i < controlList->size(); i++) 
	{
        component = (TRControlPtr) controlList->elementAt(i);
//		if(component->focus)
		component->handleTimer();
    }
  	frames++;
}

void Screen::onPaint(DGraphics* g)
{
	for (int i = 0; i < controlList->size(); i++)
	{
		TRControlPtr component = (TRControlPtr)controlList->elementAt(i);
		if (component != NULL&&component->visible)
		{
			component->onPaint(g);
		}
	}
}
TRControlPtr Screen::getActiveControl()
{
    return activeControl;
}

void Screen::setActiveControl(TRControlPtr ctrl)
{
	if (activeControl)
	{
		activeControl->focus = 0;
	}
    activeControl = ctrl;
	if (ctrl != NULL)
	{
		ctrl->focus = 1;
	}
}
void Screen::appendControl(TRControlPtr ctrl)
{
    ctrl->setOwner(this);
	controlList->addElement(ctrl);
}

void Screen::appendActiveControl(TRControlPtr ctrl)
{
    appendControl(ctrl);
    setActiveControl(ctrl);
}

void Screen::removeControl(TRControlPtr ctrl)
{
	if (ctrl == activeControl)
	{
		activeControl = NULL;
	}
	controlList->removeElement(ctrl);
	deleteCtrlList->addElement(ctrl);
}

void Screen::removeAllControl()
{
	int i;
	for(i = controlList->size() - 1;i>=0;i--)
	{
		TRControlPtr ctrl = controlList->removeElementAt(i);
		deleteCtrlList->addElement(ctrl);
	}
	activeControl = NULL;
}

Screen::Screen(SCREEN_ID scrId) : screenId(scrId)
{
	frames = 0;
	activeControl = NULL;
	visible = 1;
	controlList = new DList<TRControl>(false);
	deleteCtrlList = new DList<TRControl>(true);
	chess = Chess::getInstance();
}

int Screen::onResponse(ScreenResponse* res)
{
	return 0;
}

Screen::~Screen()
{
	removeAllControl();
	SAFE_DELETE(controlList);
	SAFE_DELETE(deleteCtrlList);
}

ChessPtr Screen::getChess()
{
	return chess;
}

void Screen::setLayerVisible(bool bVisible)
{

}
