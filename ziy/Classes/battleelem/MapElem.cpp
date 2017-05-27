#include "Common.h"
#include "Main.h"
#include "MapElem.h"

MapElem::MapElem(int x,int y)
{
	this->x = x;
	this->y = y;
	this->frame = 0;
	this->index = 0;
	this->land_id = 26;
	this->barrier_id = 0;
	this->max_index = 4;
	this->change_frame = TIMER_ACTOR_CHANGE_STYLE_FRAME;
	//这边是火的动画需要在这边进行判断修改
	img[0] = CreateGlobalImage("Mark/23");
	img[1] = CreateGlobalImage("Mark/24");
	img[2] = CreateGlobalImage("Mark/25");
	img[3] = CreateGlobalImage("Mark/26");
}

TRImagePtr getGateImage(char * file)
{
	DString filename = DString(file) + ".png";
	return CreateImageFile(filename->getChar());
}

MapElem::MapElem(int x, int y, int barrier_id, int land_id, int iImgIndex, int &iIsFind)
{
	this->x = x;
	this->y = y;
	this->frame = 0;
	this->index = 0;
	this->max_index = 4;
	this->barrier_id = barrier_id;
	this->land_id = land_id;
	this->change_frame = TIMER_ACTOR_CHANGE_STYLE_FRAME;
	//这边是其他动画需要在这边进行判断修改
	switch (barrier_id)
	{
	case 27:
	case 28:
		img[0] = getGateImage("img/Gate/49");
		img[1] = getGateImage("img/Gate/49");
		img[2] = getGateImage("img/Gate/49");
		img[3] = getGateImage("img/Gate/49");
		break;
	case 37:
	case 38:
		img[0] = getGateImage("img/Gate/67");
		img[1] = getGateImage("img/Gate/67");
		img[2] = getGateImage("img/Gate/67");
		img[3] = getGateImage("img/Gate/67");
		break;
	case 3: //“未知”障碍物，全透明，不显示
        img[0] = NULL;
        img[1] = NULL;
        img[2] = NULL;
        img[3] = NULL;
        break;
	case 0:
		img[0] = CreateGlobalImage("Mark/23");
		img[1] = CreateGlobalImage("Mark/24");
		img[2] = CreateGlobalImage("Mark/25");
		img[3] = CreateGlobalImage("Mark/26");
		break;
	case 1:
		img[0] = CreateLocalImage("Mark/27");
		img[1] = CreateLocalImage("Mark/27");
		img[2] = CreateLocalImage("Mark/28");
		img[3] = CreateLocalImage("Mark/28");
		break;
	case 2:
		img[0] = CreateLocalImage("Mark/66");
		img[1] = CreateLocalImage("Mark/66");
		img[2] = CreateLocalImage("Mark/67");
		img[3] = CreateLocalImage("Mark/67");
		break;
	default:
		img[0] = getGateImage(ChessSprintf("%s%d", "img/Gate/", ((barrier_id - 4) * 2 + 1 + iImgIndex)));
		img[1] = getGateImage(ChessSprintf("%s%d", "img/Gate/", ((barrier_id - 4) * 2 + 1 + iImgIndex)));
		img[2] = getGateImage(ChessSprintf("%s%d", "img/Gate/", ((barrier_id - 4) * 2 + 1 + iImgIndex)));
		img[3] = getGateImage(ChessSprintf("%s%d", "img/Gate/", ((barrier_id - 4) * 2 + 1 + iImgIndex)));
		break;
	}
	if (img[0] == NULL)
	{
		iIsFind = 0;
	}
	else
	{
		iIsFind = 1;
	}

}

void MapElem::paint(DGraphics* g)
{
	frame++;
	if(frame%TIMER_ACTOR_CHANGE_STYLE_FRAME == 0)
	{
		index++;
		if(index==max_index)
		{
			index = 0;
		}
	}
	int x1 = x;
	int y1 = y;
	switch(barrier_id)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		break;
	default:
		x1 = x - 1;
		y1 = y - 1;
		break;
	}
    if(img[index] != NULL)
        g->drawImage(img[index], (SCENE_TILE_WIDTH*x1 - history->scr_x), (SCENE_TILE_WIDTH*y1 - history->scr_y), 1);
}
MapElem::~MapElem()
{
	SAFE_DELETE_IMAGE(img[0]);
	SAFE_DELETE_IMAGE(img[1]);
	SAFE_DELETE_IMAGE(img[2]);
	SAFE_DELETE_IMAGE(img[3]);
}