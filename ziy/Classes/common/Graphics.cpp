#include "Common.h"
#include "main.h"
#include "cocos2d.h"
//#include "textures/CCTexture2D.h"
USING_NS_CC;


DBufferGraphics::DBufferGraphics(int w, int h) : bufferWidth(w), bufferHeight(h)
{
	clipX1 = 0;
	clipY1 = 0;
	clipX2 = w;
	clipY2 = h;
	bgColor = 0;
	currentColor = 0;
	currentColor_old = 0;
	screenBuf = new char[w*(h + 1)*byte_per_pixel];
	memset(screenBuf, 0, w*(h+1)*byte_per_pixel);
	bg_img = NULL;
	currentPixelFont = GUI_GetPixelFont(0);
}

DBufferGraphics::~DBufferGraphics()
{
	SAFE_DELETE_ARRAY(screenBuf);
}

int DBufferGraphics::MoreUCSStrlen(unsigned short* str)
{
	unsigned short* str_tmp = str;
	while (*str_tmp)
	{
		++str_tmp;
	}
	return (int)(str_tmp - str);
}
int DBufferGraphics::MoreUCSToUTF8(unsigned short *ucs2_ptr, int ucs2_len, char *utf8_ptr, int utf8_len)
{
	const unsigned short *p; char *q;
	unsigned short c;
	if (NULL == ucs2_ptr || 0 >= utf8_len)
	{
		return 0;
	}
	p = ucs2_ptr;
	q = utf8_ptr;
	if (ucs2_len <= 0)
	{
		ucs2_len = MoreUCSStrlen(ucs2_ptr);
	}
	while (p&&ucs2_len-- > 0)
	{
		c = *p++;
		if (c <= 0x7F)
		{
			if (q - utf8_ptr + 1 >= utf8_len) { break; }
			*q++ = c & 0x7F;
		}
		else if (c <= 0x7FF)
		{
			if (q - utf8_ptr + 2 >= utf8_len) { break; }
			*q++ = ((c & 0x7C0) >> 6) | 0xC0;
			*q++ = (c & 0x3F) | 0x80;
		}
		else if (c <= 0xFFFF)
		{
			if (q - utf8_ptr + 3 >= utf8_len) { break; }
			*q++ = ((c & 0xF000) >> 12) | 0xE0;
			*q++ = ((c & 0xFC0) >> 6) | 0x80;
			*q++ = (c & 0x3F) | 0x80;
		}
	}
	*q = 0;
	return (q - utf8_ptr);
}
int DBufferGraphics::MoreUTF8ToUCS(const char* utf8_ptr, int utf8_len, unsigned short* ucs2_ptr, int ucs2_len)
{
	const char *p; unsigned short *q;
	UUCHAR c, d, e; unsigned short t;
	if (NULL == utf8_ptr || 0 >= ucs2_len)
	{
		return 0;
	}
	p = utf8_ptr;
	q = ucs2_ptr;
	c = d = e = (UUCHAR)0xFF;
	if (utf8_len <= 0)
	{
		utf8_len = strlen(utf8_ptr);
	}
	while (p&&utf8_len-- > 0)
	{
		if ((int)(q - ucs2_ptr + 1) >= ucs2_len) { break; }
		c = *p++;
		if (c <= 0x7F)
		{
			*q++ = c;
		}
		else if (c <= 0xDF)
		{
			d = *p++;
			utf8_len--;
			if (d)
			{
				t = ((c & 0x1F) << 6) | (d & 0x3F);
				*q++ = t;
			}
		}
		else if (c <= 0xEF)
		{
			d = *p++;
			utf8_len--;
			if (d)
			{
				e = *p++;
				utf8_len--;
				if (e)
				{
					t = ((c & 0xF) << 12) | ((d & 0x3F) << 6) | (e & 0x3F);
					*q++ = t;
				}
			}
		}
	}
	*q = 0;
	return (q - ucs2_ptr);
}

const int ColorStringRGB[11][16] = { {0x0000FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xF700FF, 0xFF8C63, 0xFF6B4A, 0xF74A39, 0xAD3129, 0xFFDE5A, 0xFFCE52},
{0xEFBD5A, 0xBD9C52, 0xFFFFFF, 0xEEEEEE, 0xDDDDDD, 0xCCCCCC, 0xBBBBBB, 0xAAAAAA, 0x999999, 0x888888, 0x777777, 0x666666, 0x555555, 0x444444, 0x222222, 0x000000},
{0xFDEFE1, 0xFDD9B4, 0xFECA96, 0xFEB369, 0xFF952D, 0xFF7F00, 0xFF6E00, 0xFF4C00, 0xFF0000, 0xE70000, 0xC30000, 0xB70000, 0x9E0000, 0x860000, 0x6E0000, 0x4A0000},
{0xD2FDFD, 0xB4FDFD, 0x96FEFD, 0x69FEFD, 0x00FFFF, 0x00DDFF, 0x00BBFF, 0x0099FF, 0x0077FF, 0x0055FF, 0x0000FF, 0x0000E7, 0x0000C3, 0x0000AB, 0x00007A, 0x000056},
{0xFDFDE1, 0xFDFDB4, 0xFEFE87, 0xFEFE5A, 0xFFFF2D, 0xFFFF00, 0xEDED00, 0xDBDB00, 0xC9C900, 0xB2B200, 0x9F9F00, 0x8B8B00, 0x787800, 0x656500, 0x515100, 0x3E3E00},
{0xE1FDE1, 0xC1FDC1, 0xA1FEA1, 0x81FE81, 0x00FF00, 0x00ED00, 0x00DB00, 0x00C900, 0x00B800, 0x00A600, 0x009100, 0x008000, 0x006E00, 0x005900, 0x004A00, 0x003300},
{0xAE896E, 0x98765E, 0x756248, 0x64523F, 0x4B4036, 0x2E251E, 0xE7CEA5, 0xEEC388, 0xE1AD72, 0xD9A273, 0xCE9A6C, 0xE7D6BD, 0xE9D1B8, 0xE7C6A5, 0xDEBD94, 0xDEB584},
{0xF7E1FD, 0xDEC1E6, 0xC4A1CE, 0xAB81B7, 0x92619F, 0x794188, 0x5F2170, 0x460159, 0xFDE1FD, 0xFEA9FE, 0xFD86FD, 0xFF6AFF, 0xFF00FF, 0xE817E8, 0xD114D1, 0xBA12BA},
{0xF8F0CE, 0xD9D0AB, 0xBAB088, 0x9B9065, 0x8A7F55, 0x7A6F45, 0x695E34, 0x584D24, 0xABDEF2, 0x85B9CD, 0x5E93A9, 0x386E84, 0x306377, 0x28576B, 0x204C5E, 0x184051},
{0x80C380, 0x70B470, 0x61A561, 0x5FB05F, 0x418841, 0x317931, 0x226A22, 0x125B12, 0xBA925D, 0xAB8450, 0x9D7644, 0x8E6837, 0x805B2A, 0x714D1D, 0x633F11, 0x543104},
{0xBDC629, 0xADBD29, 0xADAD4A, 0x94AD31, 0x8C9C31, 0x739431, 0x738431, 0x637B31, 0x22A0BF, 0x1A80AE, 0x11609D, 0x093F8C, 0x001F7B, 0xC25502,0,0}};


void DBufferGraphics::drawString(const char* text_ptr, int x, int y,int font_space)
{
	int i,j,k,space,space1,xoffset = x,fontoffset = 0,charxoffset = 0;
	int len = strlen(text_ptr);
    if(len<=0)
        return;
	unsigned short *ucs_ptr = (unsigned short*)ChessAlloc(len*8);
	//const UUCHAR *fontptr;//字库指针
	int sanjieFontHeight = getCurrFontHeight();
	int sanjieFontWidth = getCurrFontWidth();
	len = MoreUTF8ToUCS(text_ptr, len, ucs_ptr, len * 3);
	if(y+sanjieFontHeight<0||y>screen_size_y)
	{
		ChessFree(ucs_ptr);
		return;
	}
	int RGB1 = 0, RGB2 = 0,RGB = 0;
	for(k = 0;k<len;k++)
	{
		///////设置集体颜色
		if (ucs_ptr[k] == '['&&ucs_ptr[k + 1] == 'C')
		{
			if (ucs_ptr[k + 2] >= '0'&& ucs_ptr[k + 2] <= '9')
			{
				RGB1 = ucs_ptr[k + 2] - '0';
			}
			else if (ucs_ptr[k + 2] == 'A')
				RGB1 = ucs_ptr[k + 2] - 'A'+10;
			if (ucs_ptr[k + 3] >= '0'&& ucs_ptr[k + 3] <= '9')
			{
				RGB2 = ucs_ptr[k + 3] - '0';
			}
			else if (ucs_ptr[k + 3] >= 'A'&& ucs_ptr[k + 3] <= 'F')
				RGB2 = ucs_ptr[k + 3] - 'A' + 10;

			if (RGB1 < 11 && RGB1 >= 0 && RGB2 >= 0 && RGB2 <= 15)
			{
				RGB = ColorStringRGB[RGB1][RGB2];
				if (byte_per_pixel == 2)
					this->currentColor = ((unsigned short)(RGB >> 3) & 0x001f) | ((unsigned short)(RGB >> 5) & 0x7e0) | ((unsigned short)(RGB >> 8) & 0xf800);
				else
					this->currentColor = ((RGB & 0xff) << 16) | ((RGB & 0xff00)) | ((RGB & 0xff0000) >> 16) | 0xff << 24;
			}
			else
			{
				this->currentColor = this->currentColor_old;
			}
			
			k += 4;
		}
		else if (ucs_ptr[k] == ']')
		{
			if (this->currentColor != this->currentColor_old)
			{
				this->currentColor = this->currentColor_old;
			}
			k += 1;
		}


	/////////////////////////////
		//fontptr = GUI_GetFontArray(ucs_ptr[k],this->currentFont);
		fontoffset = 0;
		DPixelFont::GlyphItem* item = currentPixelFont->findGlyphItem(ucs_ptr[k]);
		if (item)
		{
			if (ucs_ptr[k] == '\n')
			{
				xoffset = x;
				space1 = 0;
				y += sanjieFontHeight+10;
			}
			else
			{
				space1 = item->charWidth;
			}
			if(xoffset>=screen_size_x)
			{
				ChessFree(ucs_ptr);
				return;
			}
			if(xoffset+space1<0)
			{
				xoffset +=space1;
				continue;
			}
			else
			{
				for(i=0;i<sanjieFontHeight;i++)
				{
					space = space1;
					charxoffset = 0;
					while(space>8)
					{
						for(j=0;j<8;j++)
						{
							if((*(item->dataArray.getAddress()+fontoffset)<<j&0x80)!=0)
							{
								drawPoint(xoffset+charxoffset,y+i);
							}
							charxoffset++;
						}
						fontoffset++;
						space -=8;
					}
					for(j=0;j<space;j++)
					{
						if ((*(item->dataArray.getAddress() + fontoffset) << j & 0x80) != 0)
						{
							drawPoint(xoffset+charxoffset,y+i);
						}
						charxoffset++;
					}
					fontoffset++;
				}
				xoffset +=space1;
			}
		}
		else
		{
			if (ucs_ptr[k]=='\n')
			{
				xoffset = x;
				space1 = 0;
				y += sanjieFontHeight + 10;
			}
			else
			{
				space1 = sanjieFontWidth;
			}
			xoffset +=space1;
		}
		if (font_space){
			xoffset += font_space;
		}
		
	}
	ChessFree(ucs_ptr);
}

void DBufferGraphics::drawTileImage(TRImagePtr img, int srcX, int srcY, int srcW, int srcH, int dstX, int dstY, int dstW, int dstH)
{
	int drawX = 0;
	int drawY = 0;
	//上部
	for (drawY = dstY; drawY < dstY + dstH - srcH; drawY += srcH)
	{
		for (drawX = dstX; drawX < dstX + dstW - srcW; drawX += srcW)
		{
			drawRegion(img, srcX, srcY, srcW, srcH, 1, drawX, drawY);
		}
		drawRegion(img, srcX, srcY, dstX + dstW - drawX, srcH, 1, drawX, drawY);
	}
	//下部
	for (drawX = dstX; drawX < dstX + dstW - srcW; drawX += srcW)
	{
		drawRegion(img, srcX, srcY, srcW, dstY + dstH - drawY, 1, drawX, drawY);
	}
	drawRegion(img, srcX, srcY, dstX + dstW - drawX, dstY + dstH - drawY, 1, drawX, drawY);
}

void DBufferGraphics::drawImage(TRImagePtr img, int x, int y, int transform)
{	
	drawRegion(img,0,0,img->width,img->height,transform,x,y);	
}

void DBufferGraphics::drawImage(TRImagePtr img, int begin_x, int begin_y, int x, int y, int transform)
{
	if (begin_x >img->width)
	{
		begin_x = img->width;
	}
	drawRegion(img, img->width - begin_x, 0, begin_x, img->height - begin_y, transform, x, y);
}

void DBufferGraphics::copyLinePixel(UUCHAR* destBuf, UUCHAR* srcBuf, int length)
{
	int endIndex = length - byte_per_pixel;
	int startCopy = 0;
	for (int i = 0; i <= endIndex; i += byte_per_pixel)
	{
		if (isBMPTransparent(&srcBuf[i]))
		{
			if (startCopy < i)
			{
				memcpy(&destBuf[startCopy], &srcBuf[startCopy], i - startCopy);
			}
			startCopy = i + byte_per_pixel;
		}
	}
	if (startCopy < length)
	{
		memcpy(&destBuf[startCopy], &srcBuf[startCopy], length - startCopy);
	}
}

void DBufferGraphics::drawPixel(UUCHAR* destPixel, UUCHAR* srcPixel)
{
	int alpha = srcPixel[3];
	switch (alpha)
	{
	case 0:		
		break;
	case 255:
		memcpy(destPixel, srcPixel, byte_per_pixel);
		break;
	default:
		destPixel[0] = srcPixel[0] * alpha / 255 + destPixel[0] * (255 - alpha) / 255;
		destPixel[1] = srcPixel[1] * alpha / 255 + destPixel[1] * (255 - alpha) / 255;
		destPixel[2] = srcPixel[2] * alpha / 255 + destPixel[2] * (255 - alpha) / 255;
		destPixel[3] = 0xff;
		break;
	}
	
}

void DBufferGraphics::drawLinePixel(UUCHAR* destBuf, UUCHAR* srcBuf, int length)
{
	int endIndex = length - byte_per_pixel;
	int startCopy = 0;
	for (int i = 0; i <= endIndex; i += byte_per_pixel)
	{
		if (srcBuf[i + 3] == 0)
		{
			if (startCopy < i)
			{
				memcpy(&destBuf[startCopy], &srcBuf[startCopy], i - startCopy);
			}
			startCopy = i + byte_per_pixel;
		}
		else if (srcBuf[i + 3] == 0xff)
		{
			continue;
		}
		else
		{
			if (startCopy < i)
			{
				memcpy(&destBuf[startCopy], &srcBuf[startCopy], i - startCopy);
			}
			drawPixel(destBuf + i, srcBuf + i);
			startCopy = i + byte_per_pixel;
		}
	}
	if (startCopy < length)
	{
		memcpy(&destBuf[startCopy], &srcBuf[startCopy], length - startCopy);
	}
}

void DBufferGraphics::drawRegion(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y)
{
	if (width <= 0 || height <= 0 || x >= clipX2 || x + width <= clipX1 || y >= clipY2 || y + height <= clipY1)
	{
		return;
	}

	int real_width = width;
	int real_height = height;
	//处理 clip
	if (x < clipX1)
	{
		int off = clipX1 - x;
		src_x += off;
		x += off;
		width -= off;
	}
	if (x + width > clipX2)
	{
		int off = x + width - clipX2;
		width -= off;
	}
	if (y < clipY1)
	{
		int off = clipY1 - y;
		src_y += off;
		y += off;
		height -= off;
	}
	if (y + height > clipY2)
	{
		int off = y + height - clipY2;
		height -= off;
	}

	if(transform>2)
	{
		drawRegion_UnNormal(img, src_x, src_y, width, height, transform, x, y);
		return;
	}	

	const int linewidth = width * byte_per_pixel;
	const int screenlinewidth = screen_size_x*byte_per_pixel;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	UUCHAR** img_buf = (UUCHAR**)img->m_bitmap;

	if (transform == TRANS_NONE || transform == TRANS_NORMAL)
	{
		if (transform == 0)
		{
			//0 bmp不透明
			if (img->isBMPType() || !img->hasAlpha())
			{
				for (int i = 0; i < height; i++)
				{
					memcpy(screen_buf + x*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][src_x*byte_per_pixel], linewidth);
				}
			}
			else
			{
				for (int i = 0; i < height; i++)
				{
					drawLinePixel(screen_buf + x*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][src_x*byte_per_pixel], linewidth);
				}
			}
		}
		else
		{
			//1 bmp透明
			if (!img->hasAlpha())
			{
				for (int i = 0; i < height; i++)
				{
					memcpy(screen_buf + x*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][src_x*byte_per_pixel], linewidth);
				}
			}
			else if (img->isBMPType())
			{
				for (int i = 0; i < height; i++)
				{
					copyLinePixel(screen_buf + x*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][src_x*byte_per_pixel], linewidth);
				}
			}
			else
			{
				for (int i = 0; i < height; i++)
				{
					drawLinePixel(screen_buf + x*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][src_x*byte_per_pixel], linewidth);
				}
			}
		}
	}
	else if (transform == TRANS_MIRROR)
	{
		//TRANS_MIRROR
		if (!img->hasAlpha())
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					memcpy(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][(img->width-src_x-j-1)*byte_per_pixel], byte_per_pixel);
				}
			}
		}
		else if (img->isBMPType())
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (isBMPTransparent(&img_buf[src_y + i][(img->width - src_x - j - 1)*byte_per_pixel]))
					{
						continue;
					}
					drawPixel(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][(img->width - src_x - j - 1)*byte_per_pixel]);
				}
			}
		}
		else
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					drawPixel(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[src_y + i][(img->width - src_x - j - 1)*byte_per_pixel]);
				}
			}
		}		
	}
	else if (transform == 3)
	{
		//翻转
		if (!img->hasAlpha())
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					memcpy(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[img->height - (src_y + i) - 1][(src_x + j)*byte_per_pixel], byte_per_pixel);
				}
			}
		}
		else if (img->isBMPType())
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					if (isBMPTransparent(&img_buf[img->height - (src_y + i) - 1][(src_x + j)*byte_per_pixel]))
					{
						continue;
					}
					drawPixel(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[img->height - (src_y + i) - 1][(src_x + j)*byte_per_pixel]);
				}
			}
		}
		else
		{
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					drawPixel(screen_buf + (x + j)*byte_per_pixel + (y + i)*screenlinewidth,
						&img_buf[img->height - (src_y + i) - 1][(src_x + j)*byte_per_pixel]);
				}
			}
		}
	}
}

void DBufferGraphics::TransPoint(char input[][4], char * bg_color, int transform)
{
	int white_level = transform & 0x3fe0;
	int trans_level = transform & 0x7fc000;
	int poison = transform&TRANS_POISON;
	int finish = transform&TRANS_FINISH;
	int die = transform&TRANS_DIE;
	int disable = transform&TRANS_DISABLE;
	int trans_bg = transform&TRANS_ALPHA_10;
	UUCHAR R,G,B,bg_R,bg_G,bg_B;
	char *output = *input;
	if(byte_per_pixel == 2)
	{
		short temp;
		memcpy(&temp,output,2);
		R = (char)(((temp>>11)&0x1F)<<3);
		G = (char)(((temp>>5)&0x3F)<<2);
		B = (char)(((temp)&0x1F)<<3);
	}
	else
	{
		B = output[0];
		G = output[1];
		R = output[2];
	}
	if(trans_level||trans_bg)
	{
		if(byte_per_pixel == 2)
		{
			short temp;
			memcpy(&temp,bg_color,2);
			bg_R = (char)(((temp>>11)&0x1F)<<3);
			bg_G = (char)(((temp>>5)&0x3F)<<2);
			bg_B = (char)(((temp)&0x1F)<<3);
		}
		else
		{
			bg_B = bg_color[0];
			bg_G = bg_color[1];
			bg_R = bg_color[2];
		}
	}
	if(poison)
	{
		R = R>>1;
		B = B>>1;
		if((G>>1) + ((B + R)>>1)>255)
		{
			G = 255;
			if(R>B)
				R +=(G>>1) + ((B + R)>>1)-255;
			else
				B +=(G>>1) + ((B + R)>>1)-255;
		}
		else
			G = (G>>1) + ((B + R)>>1);
	}
	if(finish)
	{
		R = R/3;
		G = G/3;
		B = B/3;
	}
	if(die)
	{
		R = G = B = (R+G+B)/3;
	}
	if(disable)
	{
		if(R+G+B<600)
		{
			R = 160;
			B = 160;
			G = 160;
		}
		else
		{
			R = 240;
			B = 240;
			G = 240;
		}
	}
	switch(white_level)
	{
	case TRANS_WHITE_1:
		{
			R = R*8/10 + 51;
			B = B*8/10 + 51;
			G = G*8/10 + 51;
		}
		break;
	case TRANS_WHITE_2:
		{
			R = R*7/10 + 76;
			B = B*7/10 + 76;
			G = G*7/10 + 76;
		}
		break;
	case TRANS_WHITE_3:
		{
			R = R*6/10 + 102;
			B = B*6/10 + 102;
			G = G*6/10 + 102;
		}
		break;
	case TRANS_WHITE_4:
		{
			R = R*5/10 + 127;
			B = B*5/10 + 127;
			G = G*5/10 + 127;
		}
		break;
	case TRANS_WHITE_5:
		{
			R = R*4/10 + 153;
			B = B*4/10 + 153;
			G = G*4/10 + 153;
		}
		break;
	case TRANS_WHITE_6:
		{
			R = R*3/10 + 178;
			B = B*3/10 + 178;
			G = G*3/10 + 178;
		}
		break;
	case TRANS_WHITE_7:
		{
			R = R*2/10 + 204;
			B = B*2/10 + 204;
			G = G*2/10 + 204;
		}
		break;
	case TRANS_WHITE_8:
		{
			R = R/10 + 229;
			B = B/10 + 229;
			G = G/10 + 229;
		}
		break;
	case TRANS_WHITE_9:
		{
			R = 255;
			B = 255;
			G = 255;
		}
		break;
	}
	switch(trans_level)
	{
	case TRANS_ALPHA_1:
		{
			R = R*9/10 + bg_R/10;
			B = B*9/10 + bg_B/10;
			G = G*9/10 + bg_G/10;
		}
		break;
	case TRANS_ALPHA_2:
		{
			R = R*8/10 + bg_R*2/10;
			B = B*8/10 + bg_B*2/10;
			G = G*8/10 + bg_G*2/10;
		}
		break;
	case TRANS_ALPHA_3:
		{
			R = R*7/10 + bg_R*3/10;
			B = B*7/10 + bg_B*3/10;
			G = G*7/10 + bg_G*3/10;
		}
		break;
	case TRANS_ALPHA_4:
		{
			R = R*6/10 + bg_R*4/10;
			B = B*6/10 + bg_B*4/10;
			G = G*6/10 + bg_G*4/10;
		}
		break;
	case TRANS_ALPHA_5:
		{
			R = R*5/10 + bg_R*5/10;
			B = B*5/10 + bg_B*5/10;
			G = G*5/10 + bg_G*5/10;
		}
		break;
	case TRANS_ALPHA_6:
		{
			R = R*4/10 + bg_R*6/10;
			B = B*4/10 + bg_B*6/10;
			G = G*4/10 + bg_G*6/10;
		}
		break;
	case TRANS_ALPHA_7:
		{
			R = R*3/10 + bg_R*7/10;
			B = B*3/10 + bg_B*7/10;
			G = G*3/10 + bg_G*7/10;
		}
		break;
	case TRANS_ALPHA_8:
		{
			R = R*2/10 + bg_R*8/10;
			B = B*2/10 + bg_B*8/10;
			G = G*2/10 + bg_G*8/10;
		}
		break;
	case TRANS_ALPHA_9:
		{
			R = R/10 + bg_R*9/10;
			B = B/10 + bg_B*9/10;
			G = G/10 + bg_G*9/10;
		}
		break;
	}
	if(trans_bg)
	{
		//if(bg_R>2*R)
		//	R=bg_R;
		//else 
		if(bg_R>R)
			R=R*3/10 + bg_R*7/10;
		
		//if(bg_B>2*B)
		//	B=bg_B;
		//else 
		if(bg_B>B)
			B=B*3/10 + bg_B*7/10;
		
		//if(bg_G>2*G)
		//	G=bg_G;
	//	else 
		if(bg_G>G)
			G=G*3/10 + bg_G*7/10;
	}
	if(byte_per_pixel == 2)
	{
		short temp;
		temp = ((short)B<<11) | ((short)G<<5) | ((short)R);
		output[0] = (char)((temp&0x00ff));
		output[1] = (char)((temp&0xff00) >> 8);
	}
	else if(byte_per_pixel == 4)
	{
		output[0] = B;
		output[1] = G;
		output[2] = R;
		output[3] = (char)0xff;
	}
	else
	{
		output[0] = B;
		output[1] = G;
		output[2] = R;
	}
}
void DBufferGraphics::AveragePoint(char output[4], UUCHAR *input1, UUCHAR *input2, UUCHAR *input3, UUCHAR *input4)
{
	int num = 0;
	unsigned int R=0,G=0,B=0;
    if(input1 !=NULL&&
       ((!((input1[0]==0x1f)&&(input1[1]==0xf0))&&byte_per_pixel == 2)
        ||!((input1[0]>=0xf7)&&(input1[1]==0x0)&&input1[2]>=0xf7)))

	{
		if(byte_per_pixel == 2)
		{
			short temp;
			memcpy(&temp,input1,2);
			R += (char)(((temp>>11)&0x1F)<<3);
			G += (char)(((temp>>5)&0x3F)<<2);
			B += (char)(((temp)&0x1F)<<3);
		}
		else
		{
			B += input1[0];
			G += input1[1];
			R += input1[2];
		}
		num++;
	}
	if(input2 !=NULL&&
		((!((input2[0]==0x1f)&&(input2[1]==0xf0))&&byte_per_pixel == 2)
		||!((input2[0]>=0xf7)&&(input2[1]==0x0)&&input2[2]>=0xf7)))
	{
		if(byte_per_pixel == 2)
		{
			short temp;
			memcpy(&temp,input2,2);
			R += (char)(((temp>>11)&0x1F)<<3);
			G += (char)(((temp>>5)&0x3F)<<2);
			B += (char)(((temp)&0x1F)<<3);
		}
		else
		{
			B += input2[0];
			G += input2[1];
			R += input2[2];
		}
		num++;
	}
	if(input3 !=NULL&&
		((!((input3[0]==0x1f)&&(input3[1]==0xf0))&&byte_per_pixel == 2)
		||!((input3[0]>=0xf7)&&(input3[1]==0x0)&&input3[2]>=0xf7)))
	{
		if(byte_per_pixel == 2)
		{
			short temp;
			memcpy(&temp,input3,2);
			R += (char)(((temp>>11)&0x1F)<<3);
			G += (char)(((temp>>5)&0x3F)<<2);
			B += (char)(((temp)&0x1F)<<3);
		}
		else
		{
			B += input3[0];
			G += input3[1];
			R += input3[2];
		}
		num++;
	}
	if(input4 !=NULL&&
		((!((input4[0]==0x1f)&&(input4[1]==0xf0))&&byte_per_pixel == 2)
		||!((input4[0]>=0xf7)&&(input4[1]==0x0)&&input4[2]>=0xf7)))
	{
		if(byte_per_pixel == 2)
		{
			short temp;
			memcpy(&temp,input4,2);
			R += (char)(((temp>>11)&0x1F)<<3);
			G += (char)(((temp>>5)&0x3F)<<2);
			B += (char)(((temp)&0x1F)<<3);
		}
		else
		{
			B += input4[0];
			G += input4[1];
			R += input4[2];
		}
		num++;
	}
    if(num == 0)
        num = 1;
	R = R/num;
	G = G/num;
	B = B/num;
	if(byte_per_pixel == 2)
	{
		short temp;
		temp = ((short)B<<11) | ((short)G<<5) | ((short)R);
		output[0] = (char)((temp&0x00ff));
		output[1] = (char)((temp&0xff00) >> 8);
	}
	else if(byte_per_pixel == 4)
	{
		output[0] = B;
		output[1] = G;
		output[2] = R;
		output[3] = (char)0xff;
	}
	else
	{
		output[0] = B;
		output[1] = G;
		output[2] = R;
	}
}
int DBufferGraphics::isBGTrans(int i, int j, int transform)
{
	TRImagePtr bg_img = this->bg_img;
	if((transform&TRANS_WITH_BG_IMG) == 0 || bg_img==NULL || i<0 ||j<0 ||i>=48||j>=48*byte_per_pixel)
	{
		return 1;
	}
	UUCHAR** img_buf = (UUCHAR**)bg_img->m_bitmap;

	return isBMPTransparent(&img_buf[i][j]) ? 1 : 0;
}
void DBufferGraphics::drawRegion_UnNormal(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y)
{
	if (width <= 0 || height <= 0 || x >= clipX2 || x + width <= clipX1 || y >= clipY2 || y + height <= clipY1)
	{
		return;
	}

	//处理 clip
	if (x < clipX1)
	{
		int off = clipX1 - x;
		src_x += off;
		x += off;
		width -= off;
	}
	if (x + width > clipX2)
	{
		int off = x + width - clipX2;
		width -= off;
	}
	if (y < clipY1)
	{
		int off = clipY1 - y;
		src_y += off;
		y += off;
		height -= off;
	}
	if (y + height > clipY2)
	{
		int off = y + height - clipY2;
		height -= off;
	}

	const int linewidth = width * byte_per_pixel;
	const int screenlinewidth = screen_size_x*byte_per_pixel;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	UUCHAR** img_buf = (UUCHAR**)img->m_bitmap;

	char trans_result[4] = {0};
	char bg_color[4] = {0};

	int xOfs = x;
	int xOfi1 = src_x;
	int xOfi2 = src_x+width;
	if (transform & TRANS_MIRROR)
	{
		int temp = xOfi1;
		xOfi1 = src_x + width - xOfi2;
		xOfi2 = src_x + width - temp;
	}

	int yOfs = y;
	int yOfi1 = src_y;
	int yOfi2=(src_y+height);

	for(int i = yOfi1;i<yOfi2;i++)
	{
		int transIndex2 = (yOfs + i - yOfi1)*screen_size_x*byte_per_pixel + (xOfs - xOfi1)*byte_per_pixel + byte_per_pixel*xOfi2 - byte_per_pixel;
		int transIndexOther = (yOfs + i - yOfi1)*screen_size_x*byte_per_pixel + (xOfs - xOfi1)*byte_per_pixel;
		for(int j = byte_per_pixel*xOfi1;j<byte_per_pixel*xOfi2;j+=byte_per_pixel)
		{//  
			if (isBGTrans(i - src_y - (height - 48) / 2, j - src_x - (width - 48) / 2, transform) && (!isBMPTransparent(&img_buf[i][j])))
			{
				if(transform & 2) // 镜像
				{
					memcpy(trans_result, &img_buf[i][j],byte_per_pixel);
					memcpy(bg_color, &screen_buf[transIndex2 - j], byte_per_pixel);
					if (trans_result[0] != 0 || trans_result[1] != 0 || trans_result[2] != 0 || trans_result[3] != 0)
					TransPoint(&trans_result,bg_color,transform);
					drawPixel(&screen_buf[transIndex2 - j], (UUCHAR*)trans_result);
				}
				else
				{
					memcpy(trans_result, &img_buf[i][j],byte_per_pixel);
					memcpy(bg_color, &screen_buf[transIndexOther + j], byte_per_pixel);
					if (trans_result[0] != 0 || trans_result[1] != 0 || trans_result[2] != 0 || trans_result[3] != 0)
					TransPoint(&trans_result, bg_color, transform);
					drawPixel(&screen_buf[transIndexOther + j], (UUCHAR*)trans_result);
				}
			}
		}
	}
}
void DBufferGraphics::drawRegion_UnNormal(TRImagePtr img, int src_x, int src_y, int width, int height, int real_width, int real_height, int transform, int x, int y)
{
	if (width <= 0 || height <= 0 || x >= clipX2 || x + width <= clipX1 || y >= clipY2 || y + height <= clipY1)
	{
		return;
	}

	//处理 clip
	if (x < clipX1)
	{
		int off = clipX1 - x;
		src_x += off;
		x += off;
		width -= off;
	}
	if (x + width > clipX2)
	{
		int off = x + width - clipX2;
		width -= off;
	}
	if (y < clipY1)
	{
		int off = clipY1 - y;
		src_y += off;
		y += off;
		height -= off;
	}
	if (y + height > clipY2)
	{
		int off = y + height - clipY2;
		height -= off;
	}

	const int linewidth = width * byte_per_pixel;
	const int screenlinewidth = screen_size_x*byte_per_pixel;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	UUCHAR** img_buf = (UUCHAR**)img->m_bitmap;

	char trans_result[4] = { 0 };
	char bg_color[4] = { 0 };

	int _xOfi1 = 0;
	int _xOfi2 = 0;
	int xOfs = x;
	int xOfi1 = src_x;
	int xOfi2 = src_x + width;
	_xOfi1 = xOfi1;
	_xOfi2 = xOfi2;
	int enable = 0;
	if (transform & TRANS_MIRROR)
	{
		int temp = xOfi1;
		xOfi1 = src_x + width - xOfi2;
		xOfi2 = src_x + width - temp;
		_xOfi1 = xOfi1;
		_xOfi2 = xOfi2;
		if (width <= real_width)
		{
			_xOfi1 = src_x + real_width - xOfi2;
			_xOfi2 = src_x + real_width - temp;
			enable = 1;
		}
	}

	int yOfs = y;
	int yOfi1 = src_y;
	int yOfi2 = (src_y + height);

	for (int i = yOfi1; i < yOfi2; i++)
	{
		int transIndex2 = (yOfs + i - yOfi1)*screen_size_x*byte_per_pixel + (xOfs - xOfi1)*byte_per_pixel + byte_per_pixel*xOfi2 - byte_per_pixel;
		int transIndexOther = (yOfs + i - yOfi1)*screen_size_x*byte_per_pixel + (xOfs - xOfi1)*byte_per_pixel;
		for (int j = byte_per_pixel*_xOfi1; j < byte_per_pixel*_xOfi2; j += byte_per_pixel)
		{//  
			if (isBGTrans(i - src_y - (height - 48) / 2, j - src_x - (width - 48) / 2, transform) && (!isBMPTransparent(&img_buf[i][j])))
			{
				if (transform & 2) // 镜像
				{
					memcpy(trans_result, &img_buf[i][j], byte_per_pixel);
					memcpy(bg_color, &screen_buf[transIndex2 - j + enable * (real_width-width) * byte_per_pixel], byte_per_pixel);
					if (trans_result[0] != 0 || trans_result[1] != 0 || trans_result[2] != 0 || trans_result[3] != 0)
					TransPoint(&trans_result, bg_color, transform);
					drawPixel(&screen_buf[transIndex2 - j + enable * (real_width - width) * byte_per_pixel], (UUCHAR*)trans_result);
				}
				else
				{
					memcpy(trans_result, &img_buf[i][j], byte_per_pixel);
					memcpy(bg_color, &screen_buf[transIndexOther + j], byte_per_pixel);
					if (trans_result[0] != 0 || trans_result[1] != 0 || trans_result[2] != 0 || trans_result[3] != 0)
					TransPoint(&trans_result, bg_color, transform);
					drawPixel(&screen_buf[transIndexOther + j], (UUCHAR*)trans_result);
				}
			}
		}
	}
}
void DBufferGraphics::drawRegion_Large(TRImagePtr img, int src_x, int src_y, int width, int height, int transform, int x, int y)
{
	if (width <= 0 || height <= 0 || x >= clipX2 || x + width <= clipX1 || y >= clipY2 || y + height <= clipY1)
	{
		return;
	}

//	int screen_size_x_bytes = screen_size_x*byte_per_pixel;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	UUCHAR** img_buf = (UUCHAR**)img->m_bitmap;
	int yOfs,xOfs,xOfi1,yOfi1,xOfi2,yOfi2;
	int scr_x,scr_y,scr_width;
	char trans_result[4];
	if(x>this->clipX1)
	{
		xOfs=x;
		xOfi1=src_x;
	}
	else
	{
		xOfs=this->clipX1;
		xOfi1=src_x+this->clipX1-x;
	}
	if((x+2*width)>this->clipX2+1)
	{
		xOfi2 = src_x + (this->clipX2 - x)/2;
	}
	else
	{
		xOfi2 = src_x+width;
	}
	
	if(y>this->clipY1)
	{
		yOfs=y;
		yOfi1=src_y;
	}
	else
	{
		yOfs = this->clipY1;
		yOfi1 = src_y + this->clipY1 - y;
	}
	if((y+2*height)>this->clipY2+1)
	{
		yOfi2=0;
	}
	else
	{
		yOfi2=(src_y+height);
	}
	for(int i = yOfi1;i<yOfi2;i++)
	{
		for(int j = byte_per_pixel*xOfi1;j<byte_per_pixel*xOfi2;j+=byte_per_pixel)
		{
			if (!isBMPTransparent(&img_buf[i][j]))
			{
				scr_x = xOfs-2*xOfi1;
				scr_y = yOfs+2*i-2*yOfi1; 
				scr_width = screen_size_x*byte_per_pixel;
				if(transform & 1) //普通
				{
					memcpy(trans_result, &img_buf[i][j],byte_per_pixel);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[scr_y*scr_width + scr_x*byte_per_pixel + 2 * j], (UUCHAR*)trans_result);
					//这里会有一个像素超出范围，不知道会不会出问题。
					if(j !=(img->width -1)*byte_per_pixel)
						AveragePoint(trans_result,&img_buf[i][j],&img_buf[i][j+byte_per_pixel],NULL,NULL);
					else
						AveragePoint(trans_result,&img_buf[i][j],NULL,NULL,NULL);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[scr_y*scr_width + (scr_x + 1)*byte_per_pixel + 2 * j], (UUCHAR*)trans_result);
					if (i != img->height - 1)
						AveragePoint(trans_result,&img_buf[i][j],&img_buf[i+1][j],NULL,NULL);
					else
						AveragePoint(trans_result,&img_buf[i][j],NULL,NULL,NULL);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[(scr_y + 1)*scr_width + (scr_x)*byte_per_pixel + 2 * j], (UUCHAR*)trans_result);
					if (i != img->height - 1)
					{
						if (j != (img->width - 1)*byte_per_pixel)
							AveragePoint(trans_result,&img_buf[i][j],&img_buf[i+1][j],&img_buf[i][j+byte_per_pixel],&img_buf[i+1][j+byte_per_pixel]);
						else
							AveragePoint(trans_result,&img_buf[i][j],&img_buf[i+1][j],NULL,NULL);
					}
					else
					{
						if (j != (img->width - 1)*byte_per_pixel)
							AveragePoint(trans_result,&img_buf[i][j],NULL,&img_buf[i][j+byte_per_pixel],NULL);
						else
							AveragePoint(trans_result,&img_buf[i][j],NULL,NULL,NULL);
					}

					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[(scr_y + 1)*scr_width + (scr_x + 1)*byte_per_pixel + 2 * j], (UUCHAR*)trans_result);
					//memcpy(&screen_buf[scr_y*scr_width+(scr_x+1)*byte_per_pixel+2*j],
					//	&screen_buf[scr_y*scr_width+scr_x*byte_per_pixel+2*j],byte_per_pixel);
					//memcpy(&screen_buf[(scr_y+1)*scr_width+scr_x*byte_per_pixel+2*j],
					//	&screen_buf[scr_y*scr_width+scr_x*byte_per_pixel+2*j],byte_per_pixel);
					//memcpy(&screen_buf[(scr_y+1)*scr_width+(scr_x+1)*byte_per_pixel+2*j],
					//	&screen_buf[scr_y*scr_width+scr_x*byte_per_pixel+2*j],byte_per_pixel);
				}
				else if(transform & 2) // 镜像
				{
					memcpy(trans_result, &img_buf[i][j],byte_per_pixel);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[scr_y*scr_width + (scr_x + 2 * xOfi2 - 1)*byte_per_pixel - 2 * j], (UUCHAR*)trans_result);
					if (j != (img->width - 1)*byte_per_pixel)
						AveragePoint(trans_result, &img_buf[i][j], &img_buf[i][j + byte_per_pixel], NULL, NULL);
					else
						AveragePoint(trans_result, &img_buf[i][j], NULL, NULL, NULL);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[scr_y*scr_width + (scr_x + 2 * xOfi2 - 2)*byte_per_pixel - 2 * j], (UUCHAR*)trans_result);
					
					if (i != img->height - 1)
						AveragePoint(trans_result, &img_buf[i][j], &img_buf[i + 1][j], NULL, NULL);
					else
						AveragePoint(trans_result, &img_buf[i][j], NULL, NULL, NULL);

					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[(scr_y + 1)*scr_width + (scr_x + 2 * xOfi2 - 1)*byte_per_pixel - 2 * j], (UUCHAR*)trans_result);
					if (i != img->height - 1)
					{
						if (j != (img->width - 1)*byte_per_pixel)
							AveragePoint(trans_result, &img_buf[i][j], &img_buf[i + 1][j], &img_buf[i][j + byte_per_pixel], &img_buf[i + 1][j + byte_per_pixel]);
						else
							AveragePoint(trans_result, &img_buf[i][j], &img_buf[i + 1][j], NULL, NULL);
					}
					else
					{
						if (j != (img->width - 1)*byte_per_pixel)
							AveragePoint(trans_result, &img_buf[i][j], NULL, &img_buf[i][j + byte_per_pixel], NULL);
						else
							AveragePoint(trans_result, &img_buf[i][j], NULL, NULL, NULL);
					}
					//AveragePoint(trans_result,&img_buf[i][j],&img_buf[i+1][j],&img_buf[i][j+byte_per_pixel],&img_buf[i+1][j+byte_per_pixel]);
					TransPoint(&trans_result,0,transform);
					drawPixel(&screen_buf[(scr_y + 1)*scr_width + (scr_x + 2 * xOfi2 - 2)*byte_per_pixel - 2 * j], (UUCHAR*)trans_result);
					
					//memcpy(&screen_buf[scr_y*scr_width+(scr_x+xOfi2)*byte_per_pixel-2*j],
					//	&screen_buf[scr_y*scr_width+(scr_x+xOfi2-1)*byte_per_pixel-2*j],byte_per_pixel);
					//memcpy(&screen_buf[(scr_y+1)*scr_width+(scr_x+xOfi2-1)*byte_per_pixel-2*j],
					//	&screen_buf[scr_y*scr_width+(scr_x+xOfi2-1)*byte_per_pixel-2*j],byte_per_pixel);
					//memcpy(&screen_buf[(scr_y+1)*scr_width+(scr_x+xOfi2)*byte_per_pixel-2*j],
					//	&screen_buf[scr_y*scr_width+(scr_x+xOfi2-1)*byte_per_pixel-2*j],byte_per_pixel);
				}
			}
		}
	}
}
// void DBufferGraphics::drawRegion_narrow(TRImagePtr img, int src_x, int src_y, int img_width, int img_height, int width, int height, int transform, int x, int y)
// {
// 	if (width <= 0 || height <= 0 || x >= clipX2 || x + width <= clipX1 || y >= clipY2 || y + height <= clipY1 || img_width <= 0 || img_height <= 0 || src_x + img_width >= img->width || src_y + img_height >= img->height)
// 	{
// 		return;
// 	}
// 
// 	//	int screen_size_x_bytes = screen_size_x*byte_per_pixel;
// 	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
// 	UUCHAR** img_buf = (UUCHAR**)img->m_bitmap;
// 	int yOfs, xOfs;
// 	int scr_x, scr_y, scr_width;
// 	char trans_result[4];
// 	if (x > this->clipX1)
// 	{
// 		scr_x = x;
// 	}
// 	if ((x + width) > this->clipX2 + 1)
// 	{
// 		xOfs = this->clipX2 - x;
// 	}
// 	else
// 		xOfs = width;
// 	if (y > this->clipY1)
// 	{
// 		scr_y = y;
// 	}
// 	else
// 	{
// 		scr_y = this->clipY1;
// 	}
// 	if ((y + height) > this->clipY2 + 1)
// 	{
// 		yOfs = this->clipY2 - y;
// 	}
// 	else
// 		yOfs = height;
// 
// 	float fw = float(img_width) / width;
// 	float fh = float(img_height) / height;
// 	int imgx = 0;
// 	int imgy = 0;
// 	
// 	for (int i = 0; i < yOfs; i++)
// 	{
// 		imgy = int(fh*i);
// 		for (int j = 0; j < xOfs; j++)
// 		{
// 			if (!isBMPTransparent(&img_buf[src_y + imgy][(src_x + imgx)*byte_per_pixel]))
// 			{
// 				imgx = int(fw*j);
// 				scr_width = screen_size_x*byte_per_pixel;
// 				memcpy(trans_result, &img_buf[src_y + imgy][(src_x + imgx)*byte_per_pixel], byte_per_pixel);
// 				drawPixel(&screen_buf[(scr_y + i)*scr_width + (scr_x + j)*byte_per_pixel], (UUCHAR*)trans_result);
// 			}
// 		}
// 	}
// }
void DBufferGraphics::resetClip()
{
	this->clipX1 = 0;
	this->clipY1 = 0;
	this->clipX2 = screen_size_x;
	this->clipY2 = screen_size_y;
}
void DBufferGraphics::setClip(int x, int y, int width, int height)
{
	if(width<0)
		width=0;
	if(height<0)
		height = 0;
	this->clipX1 = x;
	this->clipY1 = y;
	this->clipX2 = x + width;
	this->clipY2 = y + height;
	if(this->clipX1<0)
		this->clipX1=0;
	if(this->clipY1<0)
		this->clipY1=0;
	if(this->clipX2>screen_size_x)
		this->clipX2 = screen_size_x;
	if(this->clipY2>screen_size_y)
		this->clipY2 = screen_size_y;
}

int DBufferGraphics::getStringWidth(const char *str)
{
	int result = 0;
	const int len = strlen(str);
	DArray<unsigned short> ucsArray(len + 1);
	ucsArray.memset(0);
	const int ucsLen = MoreUTF8ToUCS(str, len, ucsArray.getAddress(), ucsArray.size());
	for (int i = 0; i < ucsLen; i++)
	{
		DPixelFont::GlyphItem* item = currentPixelFont->findGlyphItem(ucsArray[i]);
		if (item == NULL)
		{
			result += currentPixelFont->mapWidth;
		}
		else
		{
			result += item->charWidth;
		}
	}
	return result;
}

int DBufferGraphics::getStringFontWidth(const char* str, int font)
{
	int result = 0;
	const int len = strlen(str);
	DArray<unsigned short> ucsArray(len + 1);
	ucsArray.memset(0);
	const int ucsLen = MoreUTF8ToUCS(str, len, ucsArray.getAddress(), ucsArray.size());
	DPixelFont* dpFont = GUI_GetPixelFont(font);
	for (int i = 0; i < ucsLen; i++)
	{
		DPixelFont::GlyphItem* item = dpFont->findGlyphItem(ucsArray[i]);
		if (item == NULL)
		{
			result += dpFont->mapWidth;
		}
		else
		{
			result += item->charWidth;
		}
	}
	return result;
}
int DBufferGraphics::getCurrFontHeight()
{
	return currentPixelFont->fontHeight;	
}
int DBufferGraphics::getDefaultFontHeight()
{
	return GUI_GetPixelFont(0)->fontHeight;
}
int DBufferGraphics::getCurrFontWidth()
{
	return currentPixelFont->mapWidth;	
}
int DBufferGraphics::getFontWidth(int font)
{
	return GUI_GetPixelFont(0)->mapWidth;
}
void DBufferGraphics::resetFont()
{
	currentPixelFont = GUI_GetPixelFont(0);
}
void DBufferGraphics::setFont(int font)
{	
	currentPixelFont = GUI_GetPixelFont(font);
}
void DBufferGraphics::setFont(DPixelFont* pxfont)
{
	currentPixelFont = pxfont;
}
void DBufferGraphics::setColor_R_G_B(int R, int G, int B)
{
	if(byte_per_pixel==2)
		this->currentColor =  ((unsigned short)R>>3<<11) | ((unsigned short)G>>2<<5) | ((unsigned short)B>>3);
	else
		this->currentColor =  R|G<<8|B<<16|0xff<<24;
	this->currentColor_old = this->currentColor;
}
void DBufferGraphics::setColor_RGB(int RGB)
{	
	if(byte_per_pixel == 2)
		this->currentColor = ((unsigned short)(RGB>>3)&0x001f) | ((unsigned short)(RGB>>5)&0x7e0) | ((unsigned short)(RGB>>8)&0xf800);
	else
		this->currentColor = ((RGB&0xff)<<16) | ((RGB&0xff00)) |((RGB&0xff0000)>>16)  | 0xff << 24;
	this->currentColor_old = this->currentColor;
}
void DBufferGraphics::setBgColor_R_G_B(int R, int G, int B)
{
	if(byte_per_pixel==2)
		this->bgColor =  ((unsigned short)R>>3<<11) | ((unsigned short)G>>2<<5) | ((unsigned short)B>>3);
	else
		this->bgColor = R | G << 8 | B << 16 | 0xff << 24;
}
void DBufferGraphics::setBgColor_RGB(int RGB)
{	
	if(byte_per_pixel == 2)
		this->bgColor = ((unsigned short)(RGB>>3)&0x001f) | ((unsigned short)(RGB>>5)&0x7e0) | ((unsigned short)(RGB>>8)&0xf800);
	else
		this->bgColor = ((RGB&0xff)<<16) | ((RGB&0xff00)) |((RGB&0xff0000)>>16)  | 0xff << 24;
}

void DBufferGraphics::drawPoint(int x, int y)
{
	if(x<this->clipX1||y<this->clipY1||x>this->clipX2||y>this->clipY2)
		return;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	if(byte_per_pixel==2)
		(*(unsigned short*)&screen_buf[y*screen_size_x*byte_per_pixel+x*byte_per_pixel]) = this->currentColor;
	else if(byte_per_pixel==4)
		(*(unsigned int*)&screen_buf[y*screen_size_x*byte_per_pixel+x*byte_per_pixel]) = this->currentColor;
	else
	{
		memcpy(((UUCHAR*)&screen_buf[y*screen_size_x*byte_per_pixel+x*byte_per_pixel]),
			(UUCHAR *)&this->currentColor + 1,3);
	}
}
void DBufferGraphics::drawRect(int x1, int y1, int x2, int y2)
{
	drawLine(x1,y1,x1,y2);
	drawLine(x1,y1,x2,y1);
	drawLine(x2,y1,x2,y2);
	drawLine(x1,y2,x2,y2);
}
void DBufferGraphics::drawLine(int x1, int y1, int x2, int y2)
{
	int x, y, dx, dy, c1, c2, err, swap = 0;
	if(x1<this->clipX1)
		x1 = this->clipX1;
	if(y1<this->clipY1)
		y1 = this->clipY1;
	if(x2>this->clipX2)
		x2 = this->clipX2;
	if(y2>this->clipY2)
		y2 = this->clipY2;
	if(x1==x2)
	{
	}
	else if(y1==y2)
	{
	}

	dx = x2 - x1; dy = y2 - y1;
	if (((dx < 0) ? -dx : dx) < ((dy < 0) ? -dy : dy))
	{
		swap = 1;                       /* take the long way        */
		x = x1; x1 = y1; y1 = x;
		x = x2; x2 = y2; y2 = x;
	}
	if (x1 > x2)
	{
		x = x1; x1 = x2; x2 = x;        /* always move to the right */
		y = y1; y1 = y2; y2 = y;
	}
	dx = x2 - x1; dy = y2 - y1;
	c1 = dy * 2; dy = 1;
	if (c1 < 0)
	{
		c1 = -c1;
		dy = -1;
	}
	err = c1 - dx; c2 = err - dx;
	x = x1; y = y1;
	while (x <= x2)
	{
		drawPoint(swap?y:x,swap?x:y);
		x++;
		if (err < 0)
			err += c1;
		else
		{
			y += dy;
			err += c2;
		}
	}
}
void DBufferGraphics::fillRect(int i, int j, int k, int l)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	//UUCHAR lowbyte = (UUCHAR)(this->currentColor&0xff);
	//UUCHAR highbyte = (UUCHAR)(this->currentColor>>8&0xff);
	if(i<0)
	{
		k +=i;
		i = 0;
	}
	if(j<0)
	{
		l+=j;
		j = 0;
	}
	if(i+k>screen_size_x)
	{
		k = screen_size_x - i;
	}
	if(j+l>screen_size_y)
	{
		l = screen_size_y - j;
	}
	if(k<=0||l<=0)
		return;
	//if(lowbyte == highbyte)
	//{
	//	if(k==screen_size_x&&i==0)
	//	{
	//		memset(&(screen_buf[(j*screen_size_x)*byte_per_pixel]) ,highbyte,l*screen_size_x*byte_per_pixel);
	//		return;
	//	}
	//	else
	//	{
	//		for (y=j; y<j+l; y++)
	//		{
	//			memset(&(screen_buf[(y*screen_size_x+i)*byte_per_pixel]) ,highbyte,k*byte_per_pixel);
	//		}
	//		return;
	//	}
	//}
	{
		if(byte_per_pixel == 2)
		{
			short *bgdColourArray = (short *)ChessAlloc(byte_per_pixel*screen_size_x);
			for (int x = 0; x < k; x++)
			{
				bgdColourArray[x] = this->currentColor;
			}		
			for (int y=j; y<j+l; y++)
			{
				memcpy(&(screen_buf[(y*screen_size_x+i)*byte_per_pixel]) ,bgdColourArray,k*byte_per_pixel);
			}
			ChessFree(bgdColourArray);
		}
		else if (byte_per_pixel == 4)
		{
			int *bgdColourArray = (int *)ChessAlloc(byte_per_pixel*screen_size_x);
			for (int x = 0; x < k; x++)
			{
				bgdColourArray[x] = this->currentColor;
			}
			for (int y=j; y<j+l; y++)
			{
				memcpy(&(screen_buf[(y*screen_size_x+i)*byte_per_pixel]) ,bgdColourArray,k*byte_per_pixel);
			}
			ChessFree(bgdColourArray);
		}
		else
		{
			char *bgdColourArray = (char *)ChessAlloc(byte_per_pixel*screen_size_x);
			for (int x=0;x<k;x++)
			{
				bgdColourArray[byte_per_pixel*x] = *((char*)this->currentColor);
				bgdColourArray[byte_per_pixel*x] = *((char*)this->currentColor+1);
				bgdColourArray[byte_per_pixel*x] = *((char*)this->currentColor+2);
			}
			for (int y=j; y<j+l; y++)
			{
				memcpy(&(screen_buf[(y*screen_size_x+i)*byte_per_pixel]) ,bgdColourArray,k*byte_per_pixel);
			}
			ChessFree(bgdColourArray);
		}
	}
}
void DBufferGraphics::DrawTransRect_FrameBlack(int x, int y, int w, int h)
{
	int i,j;
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	if(byte_per_pixel == 2)
	{
		short *screen;
		if (y + h > screen_size_y)
		{
			h = screen_size_y - y;
		}
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				*screen = *screen>>1&0x7BEF;
			}
		}
	}
	else if(byte_per_pixel == 4)
	{
		int *screen;
		if (y + h >screen_size_y)
		{
			h = screen_size_y - y;
		}
		for(i=y;i<y+h;i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				*screen = *screen>>1&0xff7f7f7f;
			}
		}
	}
	else
	{
		char *screen;
		if (y + h >screen_size_y)
		{
			h = screen_size_y - y;
		}
		for(i=y;i<y+h;i++)
		{
			screen = (char *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for(j=x;j<(x+w)*byte_per_pixel;j++,screen++)
			{
				*screen = *screen>>1&0x7f;
			}
		}
	}
	setColor_R_G_B(148,148,148);
	drawLine(x,y,x+w-1,y);
	drawLine(x,y,x,y+h-1);
	setColor_R_G_B(0,0,0);
	drawLine(x,y+h-1,x+w-1,y+h-1);
	drawLine(x+w-1,y,x+w-1,y+h-1);
	setColor_R_G_B(32,32,32);
	drawLine(x+1,y+h-2,x+w-3,y+h-2);
	drawLine(x+w - 2,y,x+w-2,y+h-3);
	setColor_R_G_B(65,65,65);
	drawLine(x+2,y+h-3,x+w-4,y+h-3);
	drawLine(x+w-3,y+2,x+w-3,y+h-4);

}
void DBufferGraphics::fillTransRect_Grey(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		short *screen;
		short R,G,B;
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				//*screen = *screen&0xC618;
				R = ((*screen>>11)&0x1f)*7>>3;
				
				G = ((*screen>>5)&0x3f)*7>>3;
				B = ((*screen)&0x1f)*7>>3;
				*screen = R<<11|G<<5|B;
			}
		}
	}
	else
	{
		UUCHAR *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (UUCHAR *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for(j=x;j<x+w*byte_per_pixel;j++,screen++)
			{
				*screen = *screen*7>>3;
			}
		}
	}
}
void DBufferGraphics::fillTransRectButRect_Black(int x, int y, int w, int h, int bx1, int by1, int bw1, int bh1, int bx2, int by2, int bw2, int bh2)
{
	//TODO 对排除的范围进行验证保护
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 4)
	{
		int *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				if ((i >= by1&&i <= by1 + bh1&&j >= bx1&&j <= bx1 + bw1) || (i >= by2&&i <= by2 + bh2&&j >= bx2&&j <= bx2 + bw2)){
					continue;
				}
				*screen = *screen>>1&0xff7f7f7f;
			}
		}
	}
	
}


void DBufferGraphics::fillTransRect_Black(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i, j;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (y<0)
	{
		h += y;
		y = 0;
	}
	if (x + w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if (y + h > screen_size_y)
	{
		h = screen_size_y - y;
	}
	if (byte_per_pixel == 2)
	{
		short *screen;
		for (i = y; i < y + h; i++)
		{
			screen = (short *)screen_buf + i*screen_size_x + x;
			for (j = x; j < x + w; j++, screen++)
			{
				*screen = *screen >> 1 & 0x7BEF;
			}
		}
	}
	else if (byte_per_pixel == 4)
	{
		int *screen;
		for (i = y; i < y + h; i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for (j = x; j < x + w; j++, screen++)
			{
				*screen = *screen >> 1 & 0xff7f7f7f;
			}
		}
	}
	else
	{
		char *screen;
		for (i = y; i < y + h; i++)
		{
			screen = (char *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for (j = x; j < (x + w)*byte_per_pixel; j++, screen++)
			{
				*screen = *screen >> 1 & 0x7f;
			}
		}
	}
}
void DBufferGraphics::fillTransRect_White(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	short *screen;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				*screen = *screen>>1|0x8410;
			}
		}
	}
	else if(byte_per_pixel == 4)
	{
		int *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen += byte_per_pixel)
			{
				*screen = *screen>>1|0x808080;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for(j=x;j<(x+w)*byte_per_pixel;j++,screen++)
			{
				*screen = *screen>>1&0x80;
			}
		}
	}
}
void DBufferGraphics::fillTransRect_Red(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		short *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (short)0xF800;
				screen+=2;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char*)screen_buf + (i*screen_size_x+i%2 + x)*byte_per_pixel;
			for(j=x+i%2;j<x+w;j+=2)
			{
// 				*screen = (char)0x00;
// 				*(screen + 1) = (char)0x00;
// 				*(screen+2) = (char)0xFF;

				*screen = (char)0xFF;
				*(screen + 1) = (char)0x00;
				*(screen + 2) = (char)0x00;

				screen+=byte_per_pixel*2;
			}
		}
	}
}

void DBufferGraphics::fillTransRect_Green(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	short *screen;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (short)0x07e0;
				screen+=2;
			}
		}
	}	
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char*)screen_buf + (i*screen_size_x+i%2 + x)*byte_per_pixel;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (char)0x00;
				*(screen+1) = (char)0xFF;
				*(screen+2) = (char)0x00;
				screen+=byte_per_pixel*2;
			}
		}
	}
}
void DBufferGraphics::fillTransRect_Blue(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	short *screen;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (short)0x0019;
				screen+=2;
			}
		}
	}	
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char*)screen_buf + (i*screen_size_x+i%2 + x)*byte_per_pixel;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (char)0x00;
				*(screen+1) = (char)0x00;
				*(screen+2) = (char)0xFF;
				screen+=byte_per_pixel*2;
			}
		}
	}
 }
void DBufferGraphics::fillTransRect_TintRed(int x, int y, int w, int h)
{
UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		short *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (short)0x7c00;
				screen+=2;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char*)screen_buf + (i*screen_size_x+i%2 + x)*byte_per_pixel;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (char)0x7F;
				*(screen+1) = (char)0x00;
				*(screen+2) = (char)0x00;
				screen+=byte_per_pixel*2;
			}
		}
	}

}

void DBufferGraphics::fillTransRect_TintGreen(int x, int y, int w, int h)
{
UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	int i,j;
	short *screen;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = 0x0500;
				screen+=2;
			}
		}
	}
	else if(byte_per_pixel == 4)
	{
		int *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				*screen = *screen>>1&0x7f00;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for(j=x;j<(x+w)*byte_per_pixel;j++,screen++)
			{
				*screen = *screen>>1&0x7f00;
			}
		}
	}
}
void DBufferGraphics::fillTransRect_TintBlue(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;
	int i,j;
	short *screen;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = *screen|0x0019;
				screen+=2;
			}
		}
	}
	else if(byte_per_pixel == 4)
	{
		int *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (int *)screen_buf + i*screen_size_x + x;
			for(j=x;j<x+w;j++,screen++)
			{
				*screen = *screen>>1&0x7f;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char *)screen_buf + i*screen_size_x*byte_per_pixel + x*byte_per_pixel;
			for(j=x;j<(x+w)*byte_per_pixel;j++,screen++)
			{
				*screen = *screen>>1&0x7f;
			}
		}
	}
}
void DBufferGraphics::fillTransRect_TintBlack(int x, int y, int w, int h)
{
	UUCHAR* screen_buf = (UUCHAR*)this->screenBuf;

	int i,j;
	if(x<0)
	{
		w +=x;
		x = 0;
	}
	if(y<0)
	{
		h+=y;
		y = 0;
	}
	if(x+w>screen_size_x)
	{
		w = screen_size_x - x;
	}
	if(y+h>screen_size_y)
	{
		h = screen_size_y - y;
	}
	if(byte_per_pixel == 2)
	{
		short *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (short *)screen_buf + i*screen_size_x+i%2 + x;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen = (short)0x00;
				screen+=2;
			}
		}
	}
	else
	{
		char *screen;
		for(i=y;i<y+h;i++)
		{
			screen = (char*)screen_buf + (i*screen_size_x+i%2 + x)*byte_per_pixel;
			for(j=x+i%2;j<x+w;j+=2)
			{
				*screen =0;
				*(screen+1) = 0;
				*(screen+2) = 0;
				screen+=byte_per_pixel*2;
			}
		}
	}
}

void DBufferGraphics::postScreen()
{
	Texture2D a;
	a.initWithData(screenBuf, 1024 * 1024 * 12, Texture2D::PixelFormat::RGBA8888, screen_size_x, screen_size_y, Size(screen_size_x, screen_size_y));

	if ((history->game_state == GAME_STATE_BATTLE || history->game_state == GAME_STATE_BATTLE_END) && history->initilized)
	{
		//dx = 0;
		//dy = 0;
	}
	else
	{
		//dx = (bufferWidth - screen_size_x) / 2;
		//dy = (bufferHeight - screen_size_y) / 2;
	}
	a.drawAtPoint(Vec2(0, 0));
}

void DBufferGraphics::resetBuffer()
{
	memset(screenBuf, 0, bufferWidth * bufferHeight * byte_per_pixel);
}


