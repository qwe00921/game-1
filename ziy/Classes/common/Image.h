#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "platform/CCImage.h"

USING_NS_CC;

class TRImage;
typedef TRImage* TRImagePtr;

class TRImage
{
private:
	enum ImageFileType
	{
		IMAGE_BMP,
		IMAGE_PNG
	};
public:
	TRImage(int w, int h);
private:
	~TRImage();
	TRImage(int w, int h, int bytePixel);
private:
	bool isBMPTransparent(char* pointBuffer)
	{
		unsigned char* buf = (unsigned char*)pointBuffer;
		return (buf[0] >= 0xf7 && buf[1] == 0x0 && buf[2] >= 0xf7);
	}
	void computeBMPAlpha();
public:
	char **m_bitmap;
	short width;
	short height;
public:
	bool hasAlpha() { return m_bHasAlpha; }
	bool isBMPType() { return m_imgFileType == IMAGE_BMP; }
#ifdef GAME_RES_MINI
	void setMiniFileName(const char* fileName) { mfileName = fileName; }
#endif // GAME_RES_MINI
	static TRImagePtr DecodeShrinkCCImage(Image* ccimg, int src_x, int src_y, int src_width, int src_height, int img_width, int img_height);
private:
	char* imgBuffer;
	bool m_bHasAlpha;
	ImageFileType m_imgFileType;
private:
	static TRImagePtr DecodeBMP32(char* indata);
	static TRImagePtr DecodeCCImage(Image* ccimg);
public:
	/**
	*  加载图片
	* @param name 图片名字（不用带后缀名，后缀名固定为.bmp）
	* @param is_globe 是否是全局资源 0：不是（在caocaos目录下面的资源） 1：是（Resources目录下面的资源）
	*/
	static TRImagePtr Create(const char* name, int is_global);
	/**
	*  从文件名加载图片
	* @param fileName 图片名字（带后缀名）
	* @param is_globe 是否是全局资源 0：不是（在caocaos目录下面的资源） 1：是（Resources目录下面的资源）
	*/
	static TRImagePtr CreateByFileName(const char* fileName, int is_global);
	static TRImagePtr CreateShrinkImageByFileName(const char* fileName, int src_x, int src_y, int src_width, int src_height, int img_width, int img_height);
	static TRImagePtr CreateBMPShrinkImageByFileName(const char* fileName, float num, int is_jpg);
private:
#ifdef GAME_RES_MINI
	const char* mfileName;
#endif // GAME_RES_MINI
	friend void DelImage(TRImagePtr img);
};

/*
 * 删除图片对象
 */
void DelImage(TRImagePtr img);

/**
* 安全删除图片指针
*/
#define SAFE_DELETE_IMAGE(A) if(A!=NULL){DelImage(A);A=NULL;}

/**
* 加载游戏图片
* @param name 图片名字（不用带后缀名，后缀名固定为.bmp）
*/
inline TRImagePtr CreateImage(const char* name)
{
	return TRImage::Create(name, 0);
}

/**
* 加载游戏图片
* @param fileName 文件名字（包含后缀名）
*/
inline TRImagePtr CreateImageFile(const char* fileName)
{
	return TRImage::CreateByFileName(fileName, 0);
}

/**
* 加载游戏图片并直接剪切和缩小
* @param fileName 文件名字（包含后缀名）
* @param num 缩小倍数
*/
inline TRImagePtr CreateShrinkImageFile(const char* fileName, float num,int is_jpg)
{
	return TRImage::CreateBMPShrinkImageByFileName(fileName, num, is_jpg);
}

/**
* 加载游戏图片并直接剪切和缩小
* @param fileName 文件名字（包含后缀名）
*@src_x, src_y 从什么地方开始剪切
*@src_width,src_height 原图片剪切的范围
*@img_width，img_height 得到的图片的宽高
*/
inline TRImagePtr CreateShrinkImageFile(const char* fileName, int src_x, int src_y, int src_width, int src_height,int img_width,int img_height)
{
	return TRImage::CreateShrinkImageByFileName(fileName,src_x,src_y, src_width,src_height,img_width, img_height);
}

/**
* 加载全局图片
* @param name 图片名字（不用带后缀名，后缀名固定为.bmp）
*/
inline TRImagePtr CreateGlobalImage(const char* name)
{
	return TRImage::Create(name, 1);
}

/**
* 加载全局图片
* @param fileName 文件名字（包含后缀名）
*/
inline TRImagePtr CreateGlobalImageFile(const char* fileName)
{
	return TRImage::CreateByFileName(fileName, 1);
}

/**
* 优先加载本地图片，其次加载全局图片
* @param name 图片名字（不用带后缀名，后缀名固定为.bmp）
*/
TRImagePtr CreateLocalImage(const char* name);

/**
* 优先加载本地图片，其次加载全局图片
* @param fileName 文件名字（包含后缀名）
*/
TRImagePtr CreateLocalImageFile(const char* fileName);

/**
* 加载游戏图片
* @param name 图片名字
* @param is_jpg 是否是JPG格式图片
*/
TRImagePtr CreateImage(const char* name, int is_jpg);

/**
* 加载游戏图片
* @param chName 图片名字
* @param fScale 放大倍数
* @param iGlobal 是否是全局图片
*/
TRImagePtr CreateScaleImage(const char* chName, float fScale, int iGlobal);

#endif //#ifndef _IMAGE_H_