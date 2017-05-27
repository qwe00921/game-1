#include "Common.h"
#include "main.h"
#include "button.h"


//////////////////////////////////////////////////////////////////
//按钮

void Button::onPaint(DGraphics* g)
{
	//绘制边框
	g->setColor_RGB(0xffffff);
	if(drawButtonRect)
	{
		g->fillRect(rect.x, rect.y, rect.width, rect.height);
	}
	if(leftImg)
	{
		int dx, dy = 0;
		if(rect.width <= leftImg->width + 3)
			dx = 0;
		else
			dx = 3;
		dy = (rect.height - leftImg->height) / 2;
		if(enable)
		{
			g->drawImage(leftImg, rect.x + dx, rect.y + dy, 1);
		}

		else
		{
			if(leftDisableImg == NULL)
			{
				g->drawImage(leftImg, rect.x + dx, rect.y + dy, TRANS_DISABLE | 1);
			}
			else
			{
				g->drawImage(leftDisableImg, rect.x + dx, rect.y + dy, 1);
			}
		}

	}
	if(rightImg)
	{
		int dy = 0;

		dy = (rect.height - rightImg->height) / 2;

		if(enable)
			g->drawImage(rightImg, rect.x + rect.width - rightImg->width - 2, rect.y + dy, 1);
		else
			g->drawImage(rightImg, rect.x + rect.width - rightImg->width - 2, rect.y + dy, TRANS_DISABLE | 1);
	}

	if(drawButtonRect)
	{

		if(focus)
		{
			g->setColor_R_G_B(0, 0, 0);
			g->drawRect(rect.x, rect.y, rect.x + rect.width - 1, rect.y + rect.height - 1);

			if(isPressed)
			{
				g->setColor_R_G_B(165, 166, 173);
				g->drawRect(rect.x + 1, rect.y + 1, rect.x + rect.width - 2, rect.y + rect.height - 2);
			}
			else
			{
				g->setColor_R_G_B(255, 255, 255);
				g->drawLine(rect.x + 1, rect.y + 1, rect.x + rect.width - 3, rect.y + 1);
				g->drawLine(rect.x + 1, rect.y + 1, rect.x + 1, rect.y + rect.height - 3);
				g->setColor_R_G_B(132, 134, 140);
				g->drawLine(rect.x + 1, rect.y + rect.height - 2, rect.x + rect.width - 3, rect.y + rect.height - 2);
				g->drawLine(rect.x + rect.width - 2, rect.y + 1, rect.x + rect.width - 2, rect.y + rect.height - 2);
				g->setColor_R_G_B(165, 166, 173);
				g->drawLine(rect.x + 2, rect.y + rect.height - 3, rect.x + rect.width - 4, rect.y + rect.height - 3);
				g->drawLine(rect.x + rect.width - 3, rect.y + 2, rect.x + rect.width - 3, rect.y + rect.height - 4);
			}
		}
		else
		{
			g->setColor_R_G_B(255, 255, 255);
			g->drawLine(rect.x, rect.y, rect.x + rect.width - 1, rect.y);
			g->drawLine(rect.x, rect.y, rect.x, rect.y + rect.height - 1);
			g->setColor_R_G_B(132, 134, 140);
			g->drawLine(rect.x, rect.y + rect.height - 1, rect.x + rect.width - 1, rect.y + rect.height - 1);
			g->drawLine(rect.x + rect.width - 1, rect.y, rect.x + rect.width - 1, rect.y + rect.height - 1);
			g->setColor_R_G_B(165, 166, 173);
			g->drawLine(rect.x + 1, rect.y + rect.height - 2, rect.x + rect.width - 3, rect.y + rect.height - 2);
			g->drawLine(rect.x + rect.width - 2, rect.y, rect.x + rect.width - 2, rect.y + rect.height - 3);
		}
	}
}

int Button::pointerPressDragged(int x, int y)
{
	if((!isPointerInBound(x, y, rect.x, rect.y, rect.width, rect.height)) && isPressed && visible)
	{
		isPressed = 0;
	}
	return 0;
}
int Button::pointerPressed(int x, int y)
{
	if(isPointerInBound(x, y, rect.x, rect.y, rect.width, rect.height) && enable && visible)
	{
		isPressed = 1;
		MessagePtr msg = CreateMessage(msgBaseId + MSG_BUTTON_PRESSED, this);
		owner->onMessage(msg);
		DelMessage(msg);
		owner->setActiveControl(this);
		playSound(SOUND_LEFT_MOUSE_CLICK);
		return 1;
	}
	return 0;
}
int Button::pointerReleased(int x, int y)
{
	if(isPointerInBound(x, y, rect.x, rect.y, rect.width, rect.height))
	{
		if(enable && visible&&isPressed)
		{
			isPressed = 0;
			MessagePtr msg = CreateMessage(msgBaseId + MSG_BUTTON_RELEASED, this);
			owner->onMessage(msg);
			DelMessage(msg);
			return 1;
		}
	}
	return 0;
}

Button::Button(DRectPtr rect, const char* caption, int msgBaseId, TRImagePtr leftImg, TRImagePtr rightImg) :TRControl(rect)
{
	this->isPressed = 0;
	this->enable = 1;
	this->caption = caption;
	this->leftImg = leftImg;
	this->rightImg = rightImg;
	this->msgBaseId = msgBaseId;
	drawButtonRect = true;
	textColor = -1;
	leftDisableImg = NULL;
	this->show_state = BUTTON_STATE_NORMAL;
	this->wait_move = -1;
	this->move_speed = 1;
	this->isRmove = false;
	this->isYmove = false;
}
Button::Button(DRectPtr rect, const char* caption, int msgBaseId) :TRControl(rect)
{
	this->isPressed = 0;
	this->enable = 1;
	this->caption = caption;
	this->leftImg = NULL;
	this->rightImg = NULL;
	this->msgBaseId = msgBaseId;
	drawButtonRect = true;
	textColor = -1;
	leftDisableImg = NULL;
	this->show_state = BUTTON_STATE_NORMAL;
	this->wait_move = -1;
	this->move_speed = 1;
	this->isRmove = false;
	this->isYmove = false;
}

void Button::setDrawButtonRect(bool b)
{
	drawButtonRect = b;
}

void Button::setTextColor(int color)
{
	textColor = color;
}

void Button::setLeftDisableImage(TRImagePtr disImg)
{
	leftDisableImg = disImg;
}

void Button::setCaption(DString text)
{
	this->caption = text;
}

void Button::UpdateBtnState(int ledge_x, int redge_x)
{
	switch(show_state)
	{
	case BUTTON_STATE_NORMAL:
		show_state = BUTTON_STATE_PREMOVE;
		wait_move = 0;
		break;
	case BUTTON_STATE_PREMOVE:
		if(wait_move++ == 300)
		{
			show_state = isRmove ? BUTTON_STATE_RMOVEING : BUTTON_STATE_LMOVEING;
			wait_move = -1;
		}
		break;
	case BUTTON_STATE_LMOVEING:
		if (isYmove == false)
		{
			if(rect.x > ledge_x)
			{
				rect.x -= move_speed;
			}
			else if(rect.x <= ledge_x)
			{
				rect.x = ledge_x;
				show_state = isRmove ? BUTTON_STATE_NORMAL : BUTTON_STATE_SHOWALWAYS;
			}
		}
		else
		{
			if(rect.y > ledge_x)
			{
				rect.y -= move_speed;
			}
			else if(rect.y <= ledge_x)
			{
				rect.y = ledge_x;
				show_state = isRmove ? BUTTON_STATE_NORMAL : BUTTON_STATE_SHOWALWAYS;
			}
		}
		break;
	case BUTTON_STATE_RMOVEING:
		if(isYmove == false)
		{
			if(rect.x < redge_x)
			{
				rect.x += move_speed;
			}
			else if(rect.x >= redge_x)
			{
				rect.x = redge_x;
				show_state = isRmove ? BUTTON_STATE_SHOWALWAYS : BUTTON_STATE_NORMAL;
			}
		}
		else
		{
			if(rect.y < redge_x)
			{
				rect.y += move_speed;
			}
			else if(rect.y >= redge_x)
			{
				rect.y = redge_x;
				show_state = isRmove ? BUTTON_STATE_SHOWALWAYS : BUTTON_STATE_NORMAL;
			}
		}
		break;
	case BUTTON_STATE_SHOWALWAYS:
		break;
	default:
		break;
	}
}

Button::~Button()
{
}

void Button::SetNormalImage(TRImagePtr normalImg)
{
	this->leftImg = normalImg;
}
