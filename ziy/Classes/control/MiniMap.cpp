#include "Common.h"
#include "main.h"
#include "MiniMap.h"
void MiniMap::handleTimer()
{
	switch(state)
	{
	case MINI_MAP_STATE_OPEN:
		break;
	case MINI_MAP_STATE_CLOSE:
		return;
	case MINI_MAP_STATE_OPENNING:
		if(rect.x>screen_size_x - rect.width)
			rect.x -=6;
		else
			state = MINI_MAP_STATE_OPEN;
		break;
	case MINI_MAP_STATE_CLOSING:
		if (rect.x<screen_size_x)
			rect.x +=6;
		else
			state = MINI_MAP_STATE_CLOSE;
		break;
	}
}
void MiniMap::onPaint(DGraphics* g)
{
    loadBG();

    if (this->icon_day)
        g->drawImage(this->icon_day,this->rect.x,this->rect.y,0);
    else if(this->icon)
		g->drawImage(this->icon,this->rect.x,this->rect.y,0);

    switch (history->m_iCurAir)
    {
    case AIR_TYPE_SUN:
        g->drawImage(this->img_sun, this->rect.x + (this->rect.width - 16 - this->img_sun->width), this->rect.y, 1);
        break;
    case AIR_TYPE_CLOUDY:
        g->drawImage(this->img_cloud, this->rect.x + (this->rect.width - 16 - this->img_cloud->width), this->rect.y, 1);
        break;
    case AIR_TYPE_RAIN:
        g->drawImage(this->img_rain, this->rect.x + (this->rect.width - 16 - this->img_rain->width), this->rect.y, 1);
        break;
	case AIR_TYPE_WIND:
        g->drawImage(this->img_big_rain, this->rect.x + (this->rect.width - 16 - this->img_big_rain->width), this->rect.y, 1);
        break;
    case AIR_TYPE_SNOW:
        g->drawImage(this->img_snow, this->rect.x + (this->rect.width - 16 - this->img_snow->width), this->rect.y, 1);
        break;
	}



	if (showWhichBigPoint == 0){
		paintMiniMapPointOwnTop(g);
		
	}
	else if (showWhichBigPoint == 1){
		
		paintMiniMapPointFriendTop(g);
	
	}
	else if (showWhichBigPoint==2){

		paintMiniMapPointEnemyTop(g);
	}
	else{
	
		paintMiniMapPoint(g);
	
	}
	g->setColor_RGB(0xffffff);
	g->setColor_RGB(0);
	g->drawRect(rect.x, rect.y, rect.width + rect.x, rect.y + rect.height);
}
void MiniMap::changeState()
{
	if(this->state == 	MINI_MAP_STATE_CLOSE||this->state ==MINI_MAP_STATE_CLOSING)
	{
		this->state = MINI_MAP_STATE_OPENNING;
	}
	else
	{
		this->state = MINI_MAP_STATE_CLOSING;
	}
}
int MiniMap::pointerDragged(int x , int y)
{
	if(isPointerInBound(x,y,rect.x,rect.y,rect.width,rect.height))
	{
		point_x = x;
		point_y = y;
		if(point_in == 0)
			point_in = 1;
		return 1;
	}
	else if(point_in == 1)
	{
		point_in = 0;
	}
	return 0;
	
}
int MiniMap::RpointerPressed(int x, int y)
{
	if(isPointerInBound(x,y,this->rect.x,this->rect.y,this->rect.width,this->rect.height))
	{
		changeState();
		return 1;
	}
	return 0;
}
int MiniMap::pointerPressed(int x, int y)
{
	if(isPointerInBound(x,y,this->rect.x,this->rect.y,this->rect.width,this->rect.height))
	{
		//history->CenterPos((x - rect.x)/6,(y-rect.y)/6);
		history->need_show_land = 0;
		//return 1;
	}
	else
	{
		history->need_show_land = 1;
	}
	return 0;
}

MiniMap::MiniMap(int msgBaseId)
{
	this->img_sun = CreateGlobalImageFile("new_ui/battle/weather1_1.png");
	this->img_rain = CreateGlobalImageFile("new_ui/battle/weather3_1.png");
	this->img_big_rain = CreateGlobalImageFile("new_ui/battle/weather4_1.png");
	this->img_snow = CreateGlobalImageFile("new_ui/battle/weather5_1.png");
	this->img_cloud = CreateGlobalImageFile("new_ui/battle/weather2_1.png");

    this->msgBaseId = msgBaseId;
	this->point_in = 0;
	this->point_x = 0;
	this->point_y = 0;
    rect.x = screen_size_x;
    rect.y = 0;

	showWhichBigPoint = -1;
    old_BG_index = -1;
	this->icon = NULL;
	this->icon_day = NULL;
}
void MiniMap::loadBG(){
    if(old_BG_index == history->BG_index || history->BG_index < 0)
        return;
    old_BG_index = history->BG_index;
	if (this->icon)
	{
		SAFE_DELETE_IMAGE(this->icon);
	}
	
    this->icon = CreateShrinkImageFile((char*)ChessSprintf((char*)"img/HM/%d%s", history->BG_index, ".jpg"), 8, 1);
    //this->icon_day = CreateShrinkImageFile((char*)ChessSprintf((char*)"img/HM/%d_day%s", history->BG_index, ".jpg"), 8, 1);
    //this->icon_night = CreateShrinkImageFile((char*)ChessSprintf((char*)"img/HM/%d_night%s", history->BG_index, ".jpg"), 8, 1);
    rect.width = icon->width;
    rect.height = icon->height;
}
void MiniMap::paintMiniMapPoint(DGraphics* g)
{
	int i;
	for (i = 0; i < history->battleOwnNum; i++)
	{
		HistoryActorPtr actor = history->battleOwn[i];
		if (actor->hide != 1)
		{
			g->setColor_RGB(0x00ff00);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);


			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleFriendNum; i++)
	{
		HistoryActorPtr actor = history->battleFriend[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff00);
			g->setColor_R_G_B(236,80,6);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleEnemyNum; i++)
	{
		HistoryActorPtr actor = history->battleEnemy[i];
		if (actor->hide != 1)
		{
			g->setColor_RGB(0xff);

			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
}


void MiniMap::paintMiniMapPointOwnTop(DGraphics* g)
{
	int i;
	for (i = 0; i < history->battleFriendNum; i++)
	{
		HistoryActorPtr actor = history->battleFriend[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff00);
			g->setColor_R_G_B(236, 80, 6);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleEnemyNum; i++)
	{
		HistoryActorPtr actor = history->battleEnemy[i];
		if (actor->hide != 1)
		{
			g->setColor_RGB(0xff);

			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleOwnNum; i++)
	{
		HistoryActorPtr actor = history->battleOwn[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff0000);
			g->setColor_RGB(0x00ff00);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 10, 10);


			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
}


void MiniMap::paintMiniMapPointFriendTop(DGraphics* g)
{
	int i;
	for (i = 0; i < history->battleEnemyNum; i++)
	{
		HistoryActorPtr actor = history->battleEnemy[i];
		if (actor->hide != 1)
		{
			g->setColor_RGB(0xff);

			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleOwnNum; i++)
	{
		HistoryActorPtr actor = history->battleOwn[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff0000);
			g->setColor_RGB(0x00ff00);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);


			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}

	for (i = 0; i < history->battleFriendNum; i++)
	{
		HistoryActorPtr actor = history->battleFriend[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff00);
			g->setColor_R_G_B(236, 80, 6);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 10, 10);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
}


void MiniMap::paintMiniMapPointEnemyTop(DGraphics* g)
{
	int i;
	for (i = 0; i < history->battleOwnNum; i++)
	{
		HistoryActorPtr actor = history->battleOwn[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff0000);
			g->setColor_RGB(0x00ff00);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);


			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleFriendNum; i++)
	{
		HistoryActorPtr actor = history->battleFriend[i];
		if (actor->hide != 1)
		{
			//g->setColor_RGB(0xff00);
			g->setColor_R_G_B(236, 80, 6);
			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 5, 5);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
	for (i = 0; i < history->battleEnemyNum; i++)
	{
		HistoryActorPtr actor = history->battleEnemy[i];
		if (actor->hide != 1)
		{
			g->setColor_RGB(0xff);

			g->fillRect(rect.x + actor->x * 6, rect.y + actor->y * 6, 10, 10);

			if (actor->action_finished)
			{
				g->setColor_RGB(0);
				g->fillRect(rect.x + actor->x * 6 + 1, rect.y + actor->y * 6 + 1, 3, 3);
			}
		}
	}
}

MiniMap::~MiniMap()
{
	SAFE_DELETE_IMAGE(this->icon);
	SAFE_DELETE_IMAGE(this->img_sun);
	SAFE_DELETE_IMAGE(this->img_rain);
	SAFE_DELETE_IMAGE(this->img_big_rain);
	SAFE_DELETE_IMAGE(this->img_snow);
	SAFE_DELETE_IMAGE(this->img_cloud);

}
