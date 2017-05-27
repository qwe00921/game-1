#include "Common.h"
#include "Image.h"
#include "platform/CCImage.h"

TRImage::TRImage(int w, int h)
{
#ifdef GAME_RES_MINI
	mfileName = nullptr;
#endif // GAME_RES_MINI
	m_bHasAlpha = true;
	m_imgFileType = IMAGE_BMP;
	width = w;
	height = h;
	int bytePixel = byte_per_pixel;
	
	imgBuffer = new char[width*height*bytePixel];
	memset(imgBuffer, 0, width*height*bytePixel);

	m_bitmap = new char*[height];
	for (int i = 0; i < height; i++)
	{
		m_bitmap[i] = imgBuffer + width*i*bytePixel;
	}
}

TRImage::TRImage(int w, int h, int bytePixel)
{
#ifdef GAME_RES_MINI
	mfileName = nullptr;
#endif // GAME_RES_MINI
	m_bHasAlpha = true;
	m_imgFileType = IMAGE_BMP;
	width = w;
	height = h;
	
	imgBuffer = new char[width*height*bytePixel];
	memset(imgBuffer, 0, width*height*bytePixel);

	m_bitmap = new char*[height];
	for (int i = 0; i < height; i++)
	{
		m_bitmap[i] = imgBuffer + width*i*bytePixel;
	}
}

TRImage::~TRImage()
{
	SAFE_DELETE_ARRAY(imgBuffer);
	SAFE_DELETE_ARRAY(m_bitmap);
#ifdef GAME_RES_MINI
	if (mfileName != nullptr) {
		MiniSupport::popPtr(mfileName);
	}
#endif // GAME_RES_MINI
}

void DelImage(TRImagePtr img)
{
	SAFE_DELETE(img);
}

#pragma pack(2)
typedef struct _BMPFILEHEADER_ 
{ 
	short    bfType;    
	int   bfSize; 
	short    bfReserved1; 
	short    bfReserved2; 
	int   bfOffBits; //位图文件文件头的长度
	int	biSize; 
	int	biWidth; //位图的宽度
	int	biHeight; //位图的高度
	short	biPlanes; 
	short	biBitCount;//位图的位数
	int	biCompression;
	int	biSizeImage;//位图的数据文件的大小
	int	biXPelsPerMeter; 
	int	biYPelsPerMeter; 
	int	biClrUsed; 
	int	biClrImportant; 
} BMPFILEHEADER;

void TRImage::computeBMPAlpha()
{
	for (int i = 0; i < width*height; i++)
	{
		if (isBMPTransparent(&imgBuffer[4 * i]))
		{
			m_bHasAlpha = true;
			return;
		}
	}

	m_bHasAlpha = false;
}

TRImagePtr TRImage::DecodeBMP32(char* indata)
{
	int Result;
	char  R, G, B, A;
	int i,row,col;
	int trans_row = -1;
	//读取文件头	
	BMPFILEHEADER *file_header = (BMPFILEHEADER *)indata;
	UUCHAR *file_body = (UUCHAR*)(indata + file_header->bfOffBits);

	TRImagePtr ret = new TRImage((int)abs(file_header->biWidth), (int)abs(file_header->biHeight), byte_per_pixel);

	if (file_header->biHeight<0)
	{
		trans_row = 1;
	}
	char ** out = ret->m_bitmap;

	if (file_header->biBitCount == 32)
	{
		//读取文件体
		int real_width = ret->width*4;
		//将body中的数据转换到out中
		if(trans_row == -1)
			row = ret->height - 1;
		else
			row = 0;
		for (i=0,col = 0; i<ret->height*real_width;)
		{
			R = file_body[i++];
			G = file_body[i++];
			B = file_body[i++];
			A = file_body[i++];

			out[row][4*col] = R;
			out[row][4*col+1] = G;
			out[row][4*col+2] = B;
			out[row][4 * col + 3] = (char)0xff;
			if(++col == ret->width)
			{
				row+=trans_row;
				col = 0;
			}
		}
	}
	else if (file_header->biBitCount == 24)
	{
		//读取文件体
		int real_width;
		if (ret->width%4 == 0)
		{
			real_width = ret->width*3;
		}
		else
		{
			real_width = (ret->width*3/4+1)*4;
		}
		//将body中的数据转换到out中
		if(trans_row == -1)
			row = ret->height - 1;
		else
			row = 0;
		for (i=0,col = 0; i<ret->height*real_width;)
		{
			
			R = file_body[i++];
			G = file_body[i++];
			B = file_body[i++];
			out[row][4*col] = B;
			out[row][4*col+1] = G;
			out[row][4*col+2] = R;
			out[row][4 * col + 3] = (char)0xff;
			while(i%real_width >= ret->width*3)
				i++;
			if(++col == ret->width)
			{
				row+=trans_row;
				col = 0;
			}
		}
	}
	else if (file_header->biBitCount == 16)
	{
		//读取文件体
		int real_width = (ret->width + ret->width%2)*2;
		//将body中的数据转换到out中
		if(trans_row == -1)
			row = ret->height - 1;
		else
			row = 0;
		for (i=0,col = 0; i<ret->height*real_width;)
		{
			Result = file_body[i++];
			Result=((Result<<8)&0xFF00)|file_body[i++];
			R = (char)(((Result>>11)|0xFFE0));
			G = (char)(((Result>>5)|0xFFC0));
			B = (char)(((Result)|0xFFE0));
			out[row][4*col] = R;
			out[row][4*col+1] = G;
			out[row][4*col+2] = B;
			out[row][4 * col + 3] = (char)0xff;
			while(i%real_width >= ret->width*2)
				i++;
			if(++col == ret->width)
			{
				row+=trans_row;
				col = 0;
			}
		}	
	}
	else if (file_header->biBitCount == 8)
	{
		int *color_table = (int*)(indata + sizeof(BMPFILEHEADER));//54
		int real_width;
		if (ret->width%4 == 0)
		{
			real_width = ret->width;
		}
		else
		{
			real_width = (ret->width/4+1)*4;
		}
		//将body中的数据转换到out中
		if(trans_row == -1)
			row = ret->height - 1;
		else
			row = 0;
		for (i=0,col = 0; i<ret->height*real_width;)
		{
			out[row][4*col+2] = (char)((color_table[file_body[i]]&0x000000ff));
			out[row][4*col+1] = (char)((color_table[file_body[i]]&0x0000ff00)>>8);
			out[row][4*col+0] = (char)((color_table[file_body[i]]&0x00ff0000)>>16);
			out[row][4 * col + 3] = (char)0xff;
			i++;
			while(i%(real_width) >= ret->width)
				i++;
			if(++col == ret->width)
			{
				row+=trans_row;
				col = 0;
			}
		}

	}

	ret->m_imgFileType = IMAGE_BMP;
	ret->computeBMPAlpha();

	return ret;
}

TRImagePtr TRImage::DecodeCCImage(Image* ccimg)
{
	TRImagePtr ret = new TRImage(ccimg->getWidth(), ccimg->getHeight(), byte_per_pixel);	
	ret->m_imgFileType = IMAGE_PNG;

	if (ccimg->hasAlpha())
	{
		memcpy(ret->imgBuffer, ccimg->getData(), ccimg->getWidth()*ccimg->getHeight()*byte_per_pixel);

		ret->m_bHasAlpha = false;
		unsigned char* buf = (unsigned char*)ret->imgBuffer;

		if (ccimg->isPremultipliedAlpha())
		{
			for (int i = 0; i < ret->width*ret->height; i++)
			{
				if (buf[4 * i + 3] != 0xff)
				{
					ret->m_bHasAlpha = true;
					int alpha = buf[4 * i + 3];
					if (alpha != 0)
					{
						buf[4 * i] = buf[4 * i] * 0xff / alpha;
						buf[4 * i + 1] = buf[4 * i + 1] * 0xff / alpha;
						buf[4 * i + 2] = buf[4 * i + 2] * 0xff / alpha;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < ret->width*ret->height; i++)
			{
				if (buf[4 * i + 3] != 0xff)
				{
					ret->m_bHasAlpha = true;
					break;
				}
			}
		}
	}
	else
	{
		ret->m_bHasAlpha = false;

		memset(ret->imgBuffer, 0xff, ret->width*ret->height*byte_per_pixel);
		for (int i = 0; i < ccimg->getWidth()*ccimg->getHeight(); i++)
		{
			memcpy(ret->imgBuffer + i * 4, ccimg->getData() + i * 3, 3);
		}
	}	
#ifdef GAME_RES_MINI
	ret->setMiniFileName(ccimg->getFilePath().c_str());
#endif // GAME_RES_MINI
	return ret;
}
TRImagePtr TRImage::DecodeShrinkCCImage(Image* ccimg, int src_x, int src_y, int src_width, int src_height, int img_width, int img_height)
{
	if (src_x<0 || src_y<0 || src_x + src_width>ccimg->getWidth() || src_y + src_height>ccimg->getHeight() || img_width <= 0 || img_height <= 0 || src_width <= 0 || src_height<=0)
		return NULL;
	TRImagePtr ret = DecodeCCImage(ccimg);
	TRImagePtr real_ret = new TRImage(img_width, img_height, byte_per_pixel);
	real_ret->m_imgFileType = ret->m_imgFileType;
	real_ret->m_bHasAlpha = ret->m_bHasAlpha;
	UUCHAR** img_buf1 = (UUCHAR**)ret->m_bitmap;
	UUCHAR** img_buf2 = (UUCHAR**)real_ret->m_bitmap;
	int retx = 0, rety = 0;

	for (int i = 0; i < img_height; i++)
	{
		rety = i*src_height / img_height;
		for (int j = 0; j < img_width; j++)
		{
			retx = j*src_width / img_width;
			memcpy(&img_buf2[i][j*byte_per_pixel], &img_buf1[src_y + rety][(src_x + retx)*byte_per_pixel], byte_per_pixel);
		}
	}

	SAFE_DELETE_IMAGE(ret);
	return real_ret;
}

TRImagePtr TRImage::Create(const char* name, int is_globe)
{
	DString filename = DString(name) + ".bmp";

	char* game_data = ChessReadFile(filename->getChar(),0,is_globe);
	if (game_data != NULL)
	{
		TRImagePtr ret = DecodeBMP32(game_data);
		ChessFree(game_data);
		return ret;
	}
	return 0;
}

USING_NS_CC;

TRImagePtr TRImage::CreateByFileName(const char* fileName, int is_global)
{
	unsigned long fileLen = 0;
	char* game_data = ChessReadFile(fileName, 0, is_global, &fileLen);
	if (game_data != NULL)
	{
	 	Image img;
		if (img.initWithImageData((unsigned char *)game_data, fileLen))
		{
			TRImagePtr ret = DecodeCCImage(&img);
#ifdef GAME_RES_MINI
			if (MiniSupport::isDownloading(fileName, 0, is_global)) {
				MiniSupport::pushPtr(fileName, ret);
			}
#endif // GAME_RES_MINI

			ChessFree(game_data);
			return ret;
		}
		ChessFree(game_data);
	}
	return NULL;
}

/**
* 优先加载本地图片，其次加载全局图片
* @param name 图片名字（不用带后缀名，后缀名固定为.bmp）
*/
TRImagePtr CreateLocalImage(const char* name)
{
	//DString localImage = "globalres/" + DString(name);
	//TRImagePtr result = CreateImage(localImage->getChar());
	//if (result != NULL)
	//{
	//	return result;
	//}

	return CreateGlobalImage(name);
}

/**
* 优先加载本地图片，其次加载全局图片
* @param fileName 文件名字（包含后缀名）
*/
TRImagePtr CreateLocalImageFile(const char* fileName)
{
	//DString localImage = "globalres/" + DString(fileName);
	//TRImagePtr result = CreateImageFile(localImage->getChar());
	//if (result != NULL)
	//{
	//	return result;
	//}

	return CreateGlobalImageFile(fileName);
}

TRImagePtr CreateImage(const char* name, int is_jpg)
{
	if (is_jpg == 0)
	{
		return TRImage::Create(name, 0);
	}
	else
	{
		DString filename = DString(name) + ".jpg";
		return TRImage::CreateByFileName(filename->getChar(), 0);
	}
}

TRImagePtr CreateScaleImage(const char* chName, float fScale, int iGlobal)
{
	unsigned long fileLen = 0;
	char* game_data = ChessReadFile(chName, 0, iGlobal, &fileLen);
	if(game_data != NULL)
	{
		Image img;
		if(img.initWithImageData((unsigned char *)game_data, fileLen))
		{
			TRImagePtr ret = TRImage::DecodeShrinkCCImage(&img, 0, 0, img.getWidth(), img.getHeight(), img.getWidth() * fScale, img.getHeight() * fScale);
			ChessFree(game_data);
			return ret;
		}
		ChessFree(game_data);
	}
	return NULL;
}

TRImagePtr  TRImage::CreateShrinkImageByFileName(const char* fileName, int src_x, int src_y, int src_width, int src_height, int img_width, int img_height)
{
	unsigned long fileLen = 0;
	char* game_data = ChessReadFile(fileName, 0, 0, &fileLen);
	if (game_data != NULL)
	{
		Image img;
		if (img.initWithImageData((unsigned char *)game_data, fileLen))
		{
			TRImagePtr ret = DecodeShrinkCCImage(&img,src_x,src_y,src_width,src_height,img_width,img_height);
			ChessFree(game_data);
			return ret;
		}
		ChessFree(game_data);
	}
	return NULL;
}

TRImagePtr  TRImage::CreateBMPShrinkImageByFileName(const char* fileName, float num, int is_jpg)
{
	unsigned long fileLen = 0;
	char* game_data = ChessReadFile(fileName, 0, 0, &fileLen);
	if (game_data != NULL)
	{
		if (is_jpg)
		{
			Image img;
			if (img.initWithImageData((unsigned char *)game_data, fileLen))
			{
				TRImagePtr ret = DecodeShrinkCCImage(&img, 0, 0, img.getWidth(), img.getHeight(), img.getWidth() / num, img.getHeight() / num);
				ChessFree(game_data);
				return ret;
			}
			ChessFree(game_data);
		}
		else
		{
			TRImagePtr ret = DecodeBMP32(game_data);
			int src_x = 0;
			int src_y = 0;
			int src_width = ret->width;
			int src_height = ret->height;
			int img_width = ret->width / num;
			int img_height = ret->height / num;
			TRImagePtr real_ret = new TRImage(img_width, img_height, byte_per_pixel);
			real_ret->m_imgFileType = ret->m_imgFileType;
			real_ret->m_bHasAlpha = ret->m_bHasAlpha;
			UUCHAR** img_buf1 = (UUCHAR**)ret->m_bitmap;
			UUCHAR** img_buf2 = (UUCHAR**)real_ret->m_bitmap;
			int retx = 0, rety = 0;

			for (int i = 0; i < img_height; i++)
			{
				rety = i*src_height / img_height;
				for (int j = 0; j < img_width; j++)
				{
					retx = j*src_width / img_width;
					memcpy(&img_buf2[i][j*byte_per_pixel], &img_buf1[src_y + rety][(src_x + retx)*byte_per_pixel], byte_per_pixel);
				}
			}
			SAFE_DELETE_IMAGE(ret);
			ChessFree(game_data);
			return real_ret;
		}
	}
	return NULL;
}