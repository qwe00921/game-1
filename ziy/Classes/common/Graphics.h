#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


#include "Image.h"
#include "Sys.h"


enum TRANS_TYPE
{
	TRANS_NONE = 0, //< 0: 不透明
	TRANS_NORMAL = 0x1, //< 0: bmp透明
	TRANS_MIRROR = 0x2, //< 2: 镜像
	TRANS_POISON = 0x4,
	TRANS_FINISH = 0x8,
	TRANS_DISABLE = 0x10,
	TRANS_WHITE_1 = 0x20,//4//5
	TRANS_WHITE_2 = 0x40,
	TRANS_WHITE_3 = 0x80,
	TRANS_WHITE_4 = 0x100,
	TRANS_WHITE_5 = 0x200,
	TRANS_WHITE_6 = 0x400,
	TRANS_WHITE_7 = 0x800,
	TRANS_WHITE_8 = 0x1000,
	TRANS_WHITE_9 = 0x2000,//13
	TRANS_ALPHA_1 = 0x4000,//14
	TRANS_ALPHA_2 = 0x8000,
	TRANS_ALPHA_3 = 0x10000,
	TRANS_ALPHA_4 = 0x20000,
	TRANS_ALPHA_5 = 0x40000,
	TRANS_ALPHA_6 = 0x80000,
	TRANS_ALPHA_7 = 0x100000,
	TRANS_ALPHA_8 = 0x200000,
	TRANS_ALPHA_9 = 0x400000,//22
	TRANS_WITH_BG_IMG = 0x800000,
	TRANS_ALPHA_10 = 0x1000000,
	TRANS_DIE = 0x2000000
};

class DBufferGraphics
{
public:
	DBufferGraphics(int w, int h);
	~DBufferGraphics();
public:	
	void drawImage(TRImagePtr img, int x, int y)
	{
		drawImage(img, x, y, 1);
	}
	/**
	* @param transform 0:不透明，1:普通（透明），2：镜像，参考 TRANS_TYPE
	*/
	void drawImage(TRImagePtr img, int x, int y, int transform);
	void drawImage(TRImagePtr img, int begin_x, int begin_y, int x, int y, int transform);
	void drawRegion(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y);
	void drawRegion_UnNormal(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y);
	void drawRegion_UnNormal(TRImagePtr img, int src_x, int src_y, int width, int height, int real_width, int real_height, int transform, int x, int y);
	void drawRegion_Large(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y);
/*	void drawRegion_narrow(TRImagePtr img, int src_x, int src_y, int src_width, int src_height, int width, int height, int transform, int x, int y);//缩小算法*/
	/**
	* 将图片进行平铺绘制
	* @param img 待绘制的图片
	* @param srcX 图片的 X 坐标
	* @param srcY 图片的 Y 坐标
	* @param srcW 图片绘制部分的宽度
	* @param srcH 图片绘制部分的高度
	* @param dstX 平铺起点 X 坐标
	* @param dstY 平铺起点 Y 坐标
	* @param dstW 平铺的宽度
	* @param dstH 平铺的高度
	*/
	void drawTileImage(TRImagePtr img, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH);

	void drawPoint(int x, int y);
	void drawRect(int x1, int y1, int x2, int y2);
	void drawLine(int x1, int y1, int x2, int y2);

	void fillRect(int x, int y, int w, int h);

	void drawString(const char* s, int x, int y,int font_space = 0);//font_space：额外字间距 0为默认字间距 如果不为0则是在默认字间距的基础上加上此数值

	void setColor_R_G_B(int red, int green, int blue);
	void setColor_RGB(int RGB);
	void setBgColor_R_G_B(int R, int G, int B);
	void setBgColor_RGB(int RGB);
	
	void setClip(int x, int y, int width, int height);
	void resetClip();

	void resetFont();
	void setFont(int font);
	void setFont(DPixelFont* pxfont);
	int getCurrFontWidth();
	int getCurrFontHeight();
	int getStringWidth(const char *str);
public:
	void fillTransRect_Black(int x, int y, int w, int h);
	void fillTransRectButRect_Black(int x, int y, int w, int h, int bx1, int by1, int bw1, int bh1, int bx2, int by2, int bw2, int bh2);//填充黑色（排除其中的2个矩形区域）
	void fillTransRect_White(int x, int y, int w, int h);
	void fillTransRect_Red(int x, int y, int w, int h);
	void fillTransRect_Green(int x, int y, int w, int h);
	void fillTransRect_Blue(int x, int y, int w, int h);
	void fillTransRect_TintRed(int x, int y, int w, int h);
	void fillTransRect_TintGreen(int x, int y, int w, int h);
	void fillTransRect_TintBlue(int x, int y, int w, int h);
	void fillTransRect_TintBlack(int x, int y, int w, int h);
	void fillTransRect_Grey(int x, int y, int w, int h);
	void DrawTransRect_FrameBlack(int x, int y, int w, int h);	
public:
	void postScreen();
	void resetBuffer();
public:
	static int getDefaultFontHeight();
	static int getFontWidth(int font);
	static int getStringFontWidth(const char* str, int font);
private:
	static int MoreUCSStrlen(unsigned short* str);
	static int MoreUCSToUTF8(unsigned short *ucs2_ptr, int ucs2_len, char *utf8_ptr, int utf8_len);
	void AveragePoint(char output[4], UUCHAR *input1, UUCHAR *input2, UUCHAR *input3, UUCHAR *input4);
	int isBGTrans(int i, int j, int transform);
	static int MoreUTF8ToUCS(const char* utf8_ptr, int utf8_len, unsigned short* ucs2_ptr, int ucs2_len);
	void TransPoint(char input[][4], char * bg_color, int transform);
private:
	bool isBMPTransparent(UUCHAR* pointBuffer)
	{
		return (byte_per_pixel == 4 && pointBuffer[0] >= 0xf7 && pointBuffer[1] == 0x0 && pointBuffer[2] >= 0xf7);
	}
	void copyLinePixel(UUCHAR* destBuf, UUCHAR* srcBuf, int length);
	void drawPixel(UUCHAR* destPixel, UUCHAR* srcPixel);
	void drawLinePixel(UUCHAR* destBuf, UUCHAR* srcBuf, int length);
private:
	/*
	 * 禁止 DGraphics 对象拷贝，声明私有方法并且不实现
	 */
	DBufferGraphics(const DBufferGraphics&);
	DBufferGraphics& operator=(const DBufferGraphics&);
	
public:
	TRImagePtr bg_img;
	char* screenBuf;
private:
	int clipX1;
	int clipY1;
	int clipX2;
	int clipY2;
	int bgColor;
	int currentColor;
	int currentColor_old;
	const int bufferWidth;
	const int bufferHeight;
	DPixelFont* currentPixelFont;
};


typedef DBufferGraphics DGraphics;



#endif //#ifndef _GRAPHICS_H_