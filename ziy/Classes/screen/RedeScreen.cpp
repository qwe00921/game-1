#include "Common.h"
#include "main.h"
#include "RedeScreen.h"
#include "button.h"
#include "BattleScreen.h"
#include "StatConfig.h"
#include "ActorInfoLayer.h"
#include "EquipmentUp.h"
#include "RedeMenuLayer.h"
#include "PopLayer.h"
#include "BattleTitleLayer.h"

bool RedeLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	return true;
}

void RedeScreen::setMenuVisible(bool bVisible)
{
	if(bVisible)
	{
		rede_state = REDE_STATE_ACTOR_SHOW_MENU_VISIBLE;
		wait_click = TIMER_REDE_MENU_SHOW_FRAME * 2;
		if (history->isFreeMove)
		{
			m_pMenuLayer->setLayerVisible(SHOW_MENU);
		}
		else
		{
			m_pMenuLayer->setLayerVisible(SHOW_ALL);
		}
		menu_visible = 1;
	}
	else
	{
		m_pMenuLayer->setLayerVisible(SHOW_NONE);
		rede_state = REDE_STATE_NORMAL;
		menu_visible = 0;
	}
}

void RedeScreen::hideMenu(bool hide){
}
//寻路算法
//x = 8a-8b+312
//y = 4a+4b-215
//0右上1右下2左下3左上
void RedeScreen::paintNormal(DGraphics* g)
{
	for (int i = 0; i < history->RedeActorNum; i++)
	{

		if (ActorQueue[i] == NULL){
			continue;
		}

		unsigned short min_x = ActorQueue[i]->x;
		unsigned short min_y = ActorQueue[i]->y;
		int min_index = i;
		for (int j = i; j < history->RedeActorNum; j++)
		{
			unsigned short acr_x = ActorQueue[j]->x;
			unsigned short acr_y = ActorQueue[j]->y;

			if (min_y > acr_y || (min_y == acr_y && min_x > acr_x))
			{
				min_x = acr_x;
				min_y = acr_y;
				min_index = j;
			}
		}
		if (i != min_index)
		{
			HistoryActorPtr tempPtr = ActorQueue[i];
			ActorQueue[i] = ActorQueue[min_index];
			ActorQueue[min_index] = tempPtr;
		}
	}
	int i;
	return;
	for (i = 0; i < history->RedeActorNum; i++)
	{
		HistoryActorPtr actor = (HistoryActorPtr)ActorQueue[i];

		if (actor == NULL){
			continue;
		}

		if (actor->style & 0x80)
		{
			g->drawImage(talk_img, 8 * (actor->x - actor->y + 1) + 295, 4 * (actor->x + actor->y - 1) - 265, 1);
		}
		TRImagePtr img;
		int actor_image_height = img->height / 20;
		int actor_image_width = img->width;
		switch (actor->direction)
		{
		case 0:
			g->drawRegion(img, 0, actor_image_height*(actor->style & 0x7f), actor_image_width, actor_image_height, 1,
				8 * (actor->x - actor->y + 1) + 300, 4 * (actor->x + actor->y - 1) - 250);
			break;
		case 1:
			g->drawRegion(img, 0, actor_image_height*(actor->style & 0x7f), actor_image_width, actor_image_height, 2,
				8 * (actor->x - actor->y + 1) + 300, 4 * (actor->x + actor->y - 1) - 250);
			break;
		case 2:
			g->drawRegion(img, 0, actor_image_height*(actor->style & 0x7f), actor_image_width, actor_image_height, 1,
				8 * (actor->x - actor->y + 1) + 300, 4 * (actor->x + actor->y - 1) - 250);
			break;
		case 3:
			g->drawRegion(img, 0, actor_image_height*(actor->style & 0x7f), actor_image_width, actor_image_height, 2,
				8 * (actor->x - actor->y + 1) + 300, 4 * (actor->x + actor->y - 1) - 250);
			break;
		}
	}
	if (rede_state == REDE_STATE_WAIT_COMMAND&&cur_point_actor != NULL&&wait_click == 1)
	{
		char * name = trData->actors[cur_point_actor->actor_id].getActorName();
		g->setColor_RGB(0xffffff);
		g->fillRect(8 * (cur_point_actor->x - cur_point_actor->y + 1) + 335, 4 * (cur_point_actor->x + cur_point_actor->y - 1) - 260, g->getStringWidth(name) + 10, 20);
		g->setColor_RGB(0xff);
		g->drawRect(8 * (cur_point_actor->x - cur_point_actor->y + 1) + 335, 4 * (cur_point_actor->x + cur_point_actor->y - 1) - 260,
			8 * (cur_point_actor->x - cur_point_actor->y + 1) + 345 + g->getStringWidth(name), 4 * (cur_point_actor->x + cur_point_actor->y - 1) - 240);
		g->setColor_RGB(0);
		g->drawString(name, 8 * (cur_point_actor->x - cur_point_actor->y + 1) + 340, 4 * (cur_point_actor->x + cur_point_actor->y - 1) - 255);
	}
}

void RedeScreen::paintMap(DGraphics* g)
{
	for(int i = 0;i<history->RedeActorNum;i++)
	{
		HistoryActorPtr actor = (HistoryActorPtr)ActorQueue[i];

		if (actor==NULL){
			continue;
		}

		bool bSmall = false;
		TRImagePtr img = history->GetFaceImage(actor->actor_id, bSmall);

		if(actor == cur_actor)
		{
			switch(rede_state)
			{
			case REDE_STATE_MAP_FACE_APPEAR:
				{
					int trans = TRANS_NORMAL|(1<<(22-frames/REDE_MAP_FACE_APPEAR_FRAME));
					g->drawRegion(img, 0, 0, img->width, img->height, trans, actor->x, actor->y);
					if(frames == 9*REDE_MAP_FACE_APPEAR_FRAME-1)
					{
						rede_state = REDE_STATE_MAP_NORMAL;
						cur_actor = NULL;
					}
				}
				break;
			case REDE_STATE_MAP_FACE_MOVE:
				if(actor->x<actor->dest_x)
				{
					if(actor->x<actor->dest_x-1)
						actor->x+=2;
					else
						actor->x=actor->dest_x;
				}
				else if(actor->x>actor->dest_x)
				{
					if(actor->x>actor->dest_x+1)
						actor->x-=2;
					else
						actor->x = actor->dest_x;
				}
				if(actor->y<actor->dest_y)
				{
					if(actor->y<actor->dest_y-1)
						actor->y+=2;
					else
						actor->y=actor->dest_y;
				}
				else if(actor->y>actor->dest_y)
				{
					if(actor->y>actor->dest_y+1)
						actor->y-=2;
					else
						actor->y = actor->dest_y;
				}
				g->drawImage(img,actor->x,actor->y,1);
				if(actor->x == actor->dest_x&&actor->y== actor->dest_y)
				{
					rede_state = REDE_STATE_MAP_NORMAL;
					cur_actor = NULL;
				}
				break;
			case REDE_STATE_MAP_FACE_DISAPPEAR:
				{
					int trans = TRANS_NORMAL|(1<<(14+frames/REDE_MAP_FACE_APPEAR_FRAME));
					g->drawRegion(img, 0, 0, img->width, img->height, trans, actor->x, actor->y);
					if(frames == 9*REDE_MAP_FACE_APPEAR_FRAME-1)
					{
						rede_state = REDE_STATE_MAP_NORMAL;
						cur_actor = NULL;
					}
				}
				break;
			}
		}
		else
		{
			g->drawImage(img, actor->x, actor->y, 1);
		}
	}
	g->setFont(Chess_FONT_SONG_BORDER_16);
	if(rede_state == REDE_STATE_MAP_TELL_INFO)
	{
		g->fillTransRect_Black(0,screen_size_y - 70,screen_size_x,70);
		g->setColor_RGB(0xffffff);
		char text[200];
		if(this->cur_line>1)
			g->drawString(buf[0]->getChar(),20,screen_size_y - 65);
		if(this->cur_line>2)
			g->drawString(buf[1]->getChar(),20,screen_size_y - 45);
		strcpy(text,buf[cur_line - 1]->getChar());
		if(this->cur_index >= buf[cur_line - 1]->length())
		{
			if(wait_click)
			{
				if (isRPressed&&history->setting_flag[7])
				{
					wait_click = 0;
				}
				else if(history->setting_flag[4])
				{
					if(frames%20<10)
						ChessSetCursor(CURSOR_WAIT_1);
					else
						ChessSetCursor(CURSOR_WAIT_2);
				}
				else
				{
					wait_click --;
				}
			}
			else
				this->rede_state = REDE_STATE_MAP_NORMAL;
		}
		else if(frames%2 == 0)
		{
			if((UUCHAR)text[this->cur_index] >= 0x80)
				this->cur_index+=2;
			for (int i = 0; isRPressed&&history->setting_flag[7]&& i < 9; i++) //长按加速10倍
			{
				if((UUCHAR)text[this->cur_index] >= 0x80)
				{
					this->cur_index += 2;
				}
				else
				{
					break;
				}
			}
			if ((UUCHAR)text[this->cur_index+1] == '[' && (UUCHAR)text[this->cur_index + 2] == 'C')
				this->cur_index += 5;
			else if ((UUCHAR)text[this->cur_index+1] == ']')
				this->cur_index += 2;
			else
				this->cur_index+=1;
		}
		text[this->cur_index]=0;
		g->drawString(text,20,screen_size_y - 65 + (cur_line - 1)*20);
	}
	else
	{
		if(cur_line>0)
		{
			g->fillTransRect_Black(0,screen_size_y - 70,screen_size_x,70);
			g->setColor_RGB(0xffffff);
			for(int i = 0;i<cur_line;i++)
				g->drawString(buf[i]->getChar(),20,screen_size_y - 65 + i*20);
		}
	}
	g->resetFont();
}

void RedeScreen::onPaint(DGraphics* g)
{
	if(history->imgBG)
		g->drawImage(history->imgBG,0,0,0);
	else
	{
		g->setColor_RGB(0);
		g->fillRect(0,0,screen_size_x,screen_size_y);
	}

	if(rede_state<=REDE_STATE_WAIT_COMMAND)
	{
		if (rede_state == REDE_STATE_WAIT_COMMAND)
			paintNormal(g);
	}
	else
	{
		paintMap(g);
	}
	if(cur_point_actor!=NULL)
	{
		ChessSetCursor(CURSOR_HAND);
	}

	Screen::onPaint(g);
	if (chapter_name!=NULL)
	{
		startTalk = false;
		g->setColor_RGB(0);
		//g->fillRect(0, 0, screen_size_x, screen_size_y);
		g->setFont(Chess_FONT_CYJLI_67);
		g->setColor_RGB(0xffffff);
		char* temp = ChessSprintf(LL("didzhang"), chapter_index + 1);
		g->drawString(temp, (screen_size_x - g->getStringWidth(temp)) / 2, screen_size_y / 2 - g->getCurrFontHeight());
		temp = chapter_name->getChar();
		g->drawString(temp, (screen_size_x - g->getStringWidth(temp)) / 2, screen_size_y / 2);
		g->resetFont();
	}
}
void RedeScreen::handleActorMove()
{
	if(this->moveList->size())
	{
		int i;
		for(i = 0;i<this->moveList->size();i++)
		{
			HistoryActorPtr actor = (HistoryActorPtr)this->moveList->elementAt(i);
			if(history->setting_flag[6] == 0)
			{
				actor->x=actor->dest_x;
				actor->y=actor->dest_y;
			}
			else 
			{
				if(actor->style == 1)
					actor->style = 2;
				else
					actor->style = 1;
				switch(actor->direction)
				{
				case 0:
					if(actor->y!=actor->dest_y)
						actor->y--;
					else
					{
						if(actor->x<actor->dest_x)
							actor->direction = 1;
						else if(actor->x>actor->dest_x)
							actor->direction = 3;
					}
					break;
				case 1:
					if(actor->x!=actor->dest_x)
						actor->x++;
					else
					{
						if(actor->y<actor->dest_y)
							actor->direction = 2;
						else if(actor->y>actor->dest_y)
							actor->direction = 0;
					}
					break;
				case 2:
					if(actor->y!=actor->dest_y)
						actor->y++;
					else 
					{
						if(actor->x<actor->dest_x)
							actor->direction = 1;
						else if(actor->x>actor->dest_x)
							actor->direction = 3;
					}
					break;
				case 3:
					if(actor->x!=actor->dest_x)
						actor->x--;
					else 
					{
						if(actor->y<actor->dest_y)
							actor->direction = 2;
						else if(actor->y>actor->dest_y)
							actor->direction = 0;
					}
					break;
				}
			}
			if(actor->y==actor->dest_y&&actor->x==actor->dest_x)
			{
				if(actor->ai_type>=0)
					actor->direction = actor->ai_type;
				actor->style = 0;
				this->moveList->removeElementAt(i);
			}
		}
	}
	if(this->moveList->size()==0)
	{
		this->rede_state = REDE_STATE_NORMAL;
		InitActorQueue();
	}
}

void RedeScreen::handleTimer()
{
	if (talk_layer!=NULL || m_pChoiceBox)
	{
		Screen::handleTimer();
		return;
	}
	Screen::handleTimer();
	switch(this->rede_state)
	{
	case REDE_STATE_MAP_TELL_INFO:
		break;
	case REDE_STATE_ACTOR_SHOW_MENU_VISIBLE:
		wait_click--;
		if (wait_click == TIMER_REDE_MENU_SHOW_FRAME)
		{
			menu_visible = 1;
		}
		else if (wait_click == 0)
		{
			rede_state = REDE_STATE_WAIT_COMMAND;
		}
		break;
	case REDE_STATE_WAIT_COMMAND:
		m_pMenuLayer->m_pMain->setVisible(true);
		if(wait_click == 0)
		{
			if(!handler.RunRScript(this))
			{
				if(cur_actor!=NULL)
					cur_actor->direction = actor_last_direction;
				cur_actor = NULL;
				if (this->rede_state != REDE_STATE_ACTOR_SHOW_MENU_VISIBLE)
				{
					wait_click = 1;
				}
			}
		}
		break;
	case REDE_STATE_START_BATTLE:
	case REDE_STATE_MAP_NORMAL:
	case REDE_STATE_MAP_FACE_APPEAR:
	case REDE_STATE_MAP_FACE_MOVE:
	case REDE_STATE_MAP_FACE_DISAPPEAR:
	case REDE_STATE_NORMAL:
	{
		if(!handler.RunRScript(this))
		{
			if(!menu_visible || history->m_iBattleType != BATTLE_NORMAL) //打开下一个剧本，关闭此剧本。
			{
				appendScreen(new BattleScreen());
				removeScreen(this, 0);
			}
			else
			{
				if(cur_actor != NULL)
					cur_actor->direction = actor_last_direction;
				cur_actor = NULL;
				this->rede_state = REDE_STATE_WAIT_COMMAND;
				m_pMenuLayer->setLayerVisible(SHOW_ALL);
			}
		}
	}
		break;
	}
	if(this->rede_state ==REDE_STATE_ACTOR_MOVE)
	{
		while(handler.RunRScript(this));
		if(frames%2==0||history->setting_flag[6]!=2)
			handleActorMove();
	}
}

void RedeScreen::onMessage(MessagePtr message)
{
	switch(message->msgId)
	{
        case MSG_BOTTOM_MENUBAR + IS_SHOWN:
        {
            hideMenu(true);
            break;
        }
        case MSG_BOTTOM_MENUBAR + IS_HIDE:
        {
            hideMenu(false);
            break;
        }
	case SCR_TEST_ASK + MSG_COMFORM_YES:
		chess->testAsk = 1;
		break;
	case CTRL_BUTTON_START_BATTLE + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_ARMOR + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_BUY + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_SELL + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_FRUIT + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_SMITHING + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_CHALLENGE + MSG_BUTTON_RELEASED:
		break;
	case CTRL_BUTTON_VIPGIFT + MSG_BUTTON_RELEASED:
		break;
	default:
		break;
    }
}

int RedeScreen::pointerClicked(int x, int y)
{
	isRPressed = false;
	if (Screen::pointerClicked(x, y))
		return 1;
	if(rede_state == REDE_STATE_WAIT_COMMAND)
	{
			if(cur_point_actor!=NULL)
			{
				cur_actor = cur_point_actor;
				actor_last_direction = cur_actor->direction;
				wait_click = 0;
			}
	}
	else if(rede_state == REDE_STATE_MAP_TELL_INFO&&wait_click)
	{
		wait_click = 0;
	}
	
	return 1;
}
int RedeScreen::RpointerPressed(int x, int y)
{
	isRPressed = true;
	if (rede_state == REDE_STATE_WAIT_COMMAND&&cur_point_actor != NULL)
	{
		cur_point_actor = NULL;
	}
	return Screen::RpointerReleased(x,y);
}
int RedeScreen::pointerPressed(int x, int y)
{
	pointerDragged(x, y);
	return Screen::pointerPressed(x, y);
}
int RedeScreen::pointerDragged(int x,int y)
{
	Screen::pointerDragged(x,y);
	cur_point_actor = NULL;
	if(rede_state == REDE_STATE_WAIT_COMMAND)
	{
		for(int i = 0;i<history->RedeActorNum;i++)
		{
// 			HistoryActorPtr actor = (HistoryActorPtr)ActorQueue[i];
// 			TRImagePtr img ;
// 			int actor_image_height = img->height / 20;
// 			int actor_image_width = img->width;
// 			if(isPointerInBound(x,y,8*(actor->x - actor->y)+300,
// 				4*(actor->x+actor->y)-250,actor_image_width,actor_image_height))
// 			{
// 				cur_point_actor = actor;
// 				break;
// 			}
		}
	}
	return 1;
}
RedeScreen::RedeScreen() : Screen(SCREEN_REDE)
{
	ActorQueue = new HistoryActorPtr[MAX_OWN_ARMY_NUM];
	memset(ActorQueue, 0, MAX_OWN_ARMY_NUM*sizeof(HistoryActorPtr));
	this->moveList = new DList<HistoryActor>(false);
	this->talk_layer = NULL;
	this->talk_img = CreateGlobalImage("Mark/11");
    handler.LoadRFileName(trData->m_mNetWarInfo[history->m_iCurWarId]->mstrScene[0].c_str());	
	history->game_state = GAME_STATE_REDE;
	this->frames = 0;
	this->menu_visible = 0;
	this->cur_actor = NULL;
	this->cur_point_actor = NULL;
	this->cur_index = 0;
	this->cur_line = 0;
	this->wait_click = 0;
	this->isCountFreeMove = 1;
	this->move_step = 0;
	this->actor_x_old = -1;
	this->actor_y_old = -1;
	this->paint_bust_offset = 0;
	this->buf[0] = (char*)NULL;
	this->buf[1] = (char*)NULL;
	this->buf[2] = (char*)NULL;
	startTalk = false;
	firstTalk = true;
	isRPressed = false;
	history->m_bWaitCommand = false;
	m_pChoiceBox = NULL;

	m_pRedeLayer = RedeLayer::create();
	ChessApplication::getInstance()->AddFunctionLayer(m_pRedeLayer);

	m_pMenuLayer = RedeMenuLayer::create(this);
	m_pRedeLayer->addChild(m_pMenuLayer);
	m_pBarLayer = MenuBarLayer::create(this);
	m_pRedeLayer->addChild(m_pBarLayer);
	m_pBarLayer->setVisible(false);
	setMenuVisible(false);

	InitActorQueue();
	ChessSetWindowSize(SCREEN_SIZE_REDE);
	this->rede_state = REDE_STATE_NORMAL;

	m_ScreenNameBgSp = NULL;
	m_ScreenNameLab = NULL;
}

void RedeScreen::InitActorQueue()
{
	for (int i = 0; i < MAX_OWN_ARMY_NUM; i++)
	{
		ActorQueue[i] = NULL;
	}
	for (int i = 0; i < history->RedeActorNum; i++)
	{
		ActorQueue[i] = history->RedeActor[i];
	}
	for (int i = 0; i < history->RedeActorNum; i++)
	{

		if (ActorQueue[i]==NULL){
 			continue;
 		}

		unsigned short min_x = ActorQueue[i]->x;
		unsigned short min_y = ActorQueue[i]->y;
		int min_index = i;
		for (int j = i; j < history->RedeActorNum; j++)
		{
			unsigned short acr_x = ActorQueue[j]->x;
			unsigned short acr_y = ActorQueue[j]->y;

			if (min_x > acr_x || (min_x == acr_x && min_y > acr_y))
			{
				min_x = acr_x;
				min_y = acr_y;
				min_index = j;
			}
		}
		if (i != min_index)
		{
			HistoryActorPtr tempPtr = ActorQueue[i];
			ActorQueue[i] = ActorQueue[min_index];
			ActorQueue[min_index] = tempPtr;
		}	
	}
}
void RedeScreen::gameEnding()
{

}

RedeScreen::~RedeScreen()
{
	if(this->moveList)
	{
		this->moveList->removeAllElements();
		SAFE_DELETE(moveList);
	}
	SAFE_DELETE_ARRAY(ActorQueue);

	SAFE_DELETE_IMAGE(this->talk_img);

	if (m_ScreenNameBgSp)
		m_ScreenNameBgSp->removeFromParent();
}

void RedeScreen::onRemove()
{
	history->clearSceneData();
	history->initilized = 0;
	m_pRedeLayer->removeFromParent();
	
	history->mTitleLayer = BattleTitleLayer::create();
	ChessApplication::getInstance()->AddFunctionLayer(history->mTitleLayer);
}

bool RedeScreen::GetMenuVisible()
{
	return m_pMenuLayer->m_pMain->isVisible();
}

void RedeScreen::setLayerVisible(bool bVisible)
{
	m_pRedeLayer->setVisible(bVisible);
}

void RedeScreen::ChildCloseLayer(int iNomal)
{
	talk_layer = NULL;
}

void RedeScreen::SetScreenName(string strName)
{
	if (m_ScreenNameBgSp == NULL)
	{
		DSize size = ChessViewTool::getInstance()->getFullSize();
		m_ScreenNameBgSp = ReadSpriteName("new_ui/img_global/r_bg1.png");
		m_ScreenNameBgSp->setAnchorPoint(Vec2(0.5,1));
		m_ScreenNameBgSp->setPosition(Vec2(size.width / 2, size.height));
		ChessApplication::getInstance()->GetActionLayer()->addChild(m_ScreenNameBgSp);

		m_ScreenNameLab = LabelSystemFont(strName.c_str(),20);
		m_ScreenNameLab->setPosition(Vec2(m_ScreenNameBgSp->getContentSize().width / 2, m_ScreenNameBgSp->getContentSize().height/2));
		m_ScreenNameBgSp->addChild(m_ScreenNameLab);
	}
	else
	{
		m_ScreenNameLab->setString(strName.c_str());
	}
}