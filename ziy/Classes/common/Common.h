#ifndef _CHESS_PROJECT_H_
#define _CHESS_PROJECT_H_
#include "stdlib.h"
#include "stdio.h"
#include "memory.h"
#include "string.h"

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#define EXPORT_PUBLIC __attribute__ ((visibility("default")))
#define EXPORT_LOCAL  __attribute__ ((visibility("hidden")))
#else
#define EXPORT_PUBLIC
#define EXPORT_LOCAL
#endif

#define UUCHAR unsigned char
#define SSCHAR signed char

#pragma   warning(disable : 4103)


#define DFC_DCP1(c) class c;typedef c* c##Ptr;


DFC_DCP1(DStringBuf)
DFC_DCP1(DString)

#define SAFE_DELETE(pObj)  if(pObj!=NULL){delete pObj; (pObj)=NULL;}
#define SAFE_DELETE_ARRAY(pArray)  if(pArray!=NULL){delete []pArray; pArray=NULL;}


class DPoint
{
public:
	DPoint()
	{
		x = 0;
		y = 0;
	}
	DPoint(int px, int py)
	{
		x = px;
		y = py;
	}
public:
	int x;
	int y;
};
typedef DPoint* DPointPtr;

class DSize
{
public:
	DSize()
	{
		width = 0;
		height = 0;
	}
	DSize(int w, int h)
	{
		width = w;
		height = h;
	}
public:
	int width;
	int height;
};


class DRect
{
public:
	DRect()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
	DRect(int px, int py, int rw, int rh)
	{
		x = px;
		y = py;
		width = rw;
		height = rh;
	}
	DRect(const DPoint& pt, const DSize& size)
	{
		x = pt.x;
		y = pt.y;
		width = size.width;
		height = size.height;
	}
public:
	int x;
	int y;
	int width;
	int height;
};
typedef DRect* DRectPtr;


#include "Const.h"
#include "Graphics.h"
#include "Image.h"
#include "Functions.h"
#include "DString.h"
#include "dvector.h"
#include "Sys.h"
#include "DArray.h"
#include "DList.h"
#include <stdlib.h>

#ifdef GAME_RES_MINI//小包
	#include "tools/MiniSupport.h"
#endif


#endif