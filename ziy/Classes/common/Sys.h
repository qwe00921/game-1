#ifndef _CHESS_SYS_H_
#define _CHESS_SYS_H_

#include "DString.h"
#include "Const.h"

char* getRespath(const char *filename, int is_writeable = 0, int is_globe = 0);
#ifdef GAME_RES_MINI
	char* getMiniResourceRelativeFile(const char *filename, int is_writeable = 0, int is_globe = 0);
#endif
void StartChessTimer(int ticks);
void UpdateChessWindow(DRectPtr temp_rect);
//#define ChessAlloc malloc
void *ChessAlloc(unsigned int size);
void ChessFree(void *mem);
char* getTimeString();
char* GetNewTimeString();
#ifndef SEEK_SET
#define SEEK_SET        0       /* Seek from beginning of file.  */
#define SEEK_CUR        1       /* Seek from current position.  */
#define SEEK_END        2       /* Set file pointer to EOF plus "offset" */
#endif

class ChessFile
{
public:
	ChessFile();
	~ChessFile();
public:
	bool open(const char *filename, const char *mode);
	bool openWrite(const char *filename);
	int write(const void *buffer, unsigned int size);
	void close();
	int fremove(const char *filename);
public:
	static void createDirectory(const char* dirPath);
	static bool isDirectoryExist(const char* dirPath);
	static bool isFileExist(const char* dirPath);
	static bool directReadFile(const char* filepath, char** data, unsigned long* dataLength);
	static bool encryptReadFile(const char* filepath, char** data, unsigned long* dataLength);
	static bool directWriteFile(const char* filepath, const char* data, unsigned long dataLength);
	static bool encryptWriteFile(const char* filepath, const char* data, unsigned long dataLength);
	static DString findParentDir(DString path);
	static void saveResourceGen(const char* srcpath);
	static void saveResourceGenData(const char* srcpath, char* data, int length);
	static DString getResourcePath(const char *filename, int is_writeable, int is_globe);
#ifdef GAME_RES_MINI
	static DString getMiniResourceRelativeFile(const char *filename, int is_writeable, int is_globe);
#endif
	static int removeFile(const char* filepath);
private:
	static bool needGenResource();
private:
	FILE* mFile;
};

char* ChessReadFile(const char *filename, int is_writeable = 0, int is_globe = 0, unsigned long* fileLength = NULL);
char* ChessDirectReadFile(const char *filename, int is_writeable = 0, int is_globe = 0, unsigned long* fileLength = NULL);
char* ChessEncryptReadFile(const char *filename, int is_writeable = 0, int is_globe = 0, unsigned long* fileLength = NULL);
int ChessWriteFile(const char *filename, char* buffer, int size);
int ChessDirectWriteFile(const char *filename, char* buffer, int size);
int ChessEncryptWriteFile(const char *filename, char* buffer, int size);
int ChessRemoveFile(const char* filename);
/////////////////////////////////////////////////////////////////////
#include "DArray.h"
#include "DString.h"

//编码转换接口
enum
{
	Chess_FONT_SONG_12 = 0,
	Chess_FONT_SONG_13,
	Chess_FONT_SONG_14,
	Chess_FONT_SONG_15,
	Chess_FONT_SONG_16,
	Chess_FONT_SONG_20,
	Chess_FONT_KAI_12,
	Chess_FONT_KAI_14,
	Chess_FONT_KAI_16,
	Chess_FONT_KAI_20,
	Chess_FONT_SONG_BORDER_14,
	Chess_FONT_SONG_BORDER_16,
	Chess_FONT_SONG_BORDER_72,
	Chess_FONT_CYJHKAI_80,
	Chess_FONT_CYJLI_67,
	Chess_FONT_MAX
};

void InitFont();

/////////////////////////////////////////////////////////////////////
//点整字库对象
class DPixelFont
{
public:
	//点阵字信息
	class GlyphItem
	{
	public:
		GlyphItem()
		{
			code = '\0';
			charWidth = 0;
		}
	public:
		//字符unicode编码
		unsigned short code;
		//字符宽度
		unsigned short charWidth;
		//字符绘制信息
		DCharArray dataArray;
	};
public:
	DPixelFont()
	{
		memset(fontName, 0, sizeof(fontName));
		fontHeight = 0;
		mapWidth = 0;
		mapHeight = 0;
	}
public:
	bool loadGlobalFile(const char* fileName);
	GlyphItem* findGlyphItem(unsigned short code);
public:
	//字体名称
	char fontName[33];
	//字体高度
	int fontHeight;
	//字符位图宽度
	int mapWidth;
	//字符位图高度
	int mapHeight;
	//字符绘制数组
	DArray<GlyphItem> charArray;
};

DPixelFont* GUI_GetPixelFont(int fontId);

/////////////////////////////////////////////////////////////////////

#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
using std::min;
using std::max;
#else
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
#endif

const UUCHAR byte_per_pixel = BYTES_PER_PIXEL;

#define screen_size_x ((int)ChessViewTool::getInstance()->getScreenSize().width)
#define screen_size_y ((int)ChessViewTool::getInstance()->getScreenSize().height)


////////////////////////////////////////////////////////////////////////////////////
#include "cocos2d.h"

class DPointF
{
public:
	DPointF()
	{
		x = 0;
		y = 0;
	}
	DPointF(float px, float py)
	{
		x = px;
		y = py;
	}
public:
	DPointF(const DPoint& pt)
	{
		x = pt.x;
		y = pt.y;
	}
	operator DPoint() const
	{
		return DPoint(x, y);
	}
	DPointF(const cocos2d::Vec2& pt);
	operator cocos2d::Vec2() const;
public:
	float x;
	float y;
};

class DSizeF
{
public:
	DSizeF()
	{
		width = 0;
		height = 0;
	}
	DSizeF(float w, float h)
	{
		width = w;
		height = h;
	}
public:
	DSizeF(const DSize& sz)
	{
		width = sz.width;
		height = sz.height;
	}
	operator DSize() const
	{
		return DSize(width, height);
	}
	DSizeF(const cocos2d::Size& sz);
	operator cocos2d::Size() const;
public:
	float width;
	float height;
};

class DRectF
{
public:
	DRectF()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
	DRectF(float px, float py, float rw, float rh)
	{
		x = px;
		y = py;
		width = rw;
		height = rh;
	}
	DRectF(const DPointF& pt, const DSizeF& size)
	{
		x = pt.x;
		y = pt.y;
		width = size.width;
		height = size.height;
	}
public:
	float getTop() const;
	float getRight() const;
	DPointF getLeftBottom() const;
	DPointF getRightTop() const;
	DSizeF getSize() const;
public:
	DRectF(const DRect& rect)
	{
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
	}
	operator DRect() const
	{
		return DRect(x, y, width, height);
	}
	DRectF(const cocos2d::Rect& rect);
	operator cocos2d::Rect() const;
public:
	float x;
	float y;
	float width;
	float height;
};

////////////////////////////////////////////////////////////////////////////////////

class ChessViewTool
{
public:
	void setScreenSize(float scrWidth, float scrHeight);
	DSizeF getScreenSize();
	void setScreenDefaultSize();
	void setScreenFullSize();
	DSizeF getFullSize();
	void computeFullSize(float frameWidth, float frameHeight);
public:
	DPointF screen2GL(const DPointF& pt);
	DRectF screen2GL(const DRectF& rct);
public:
	static ChessViewTool* getInstance();
private:
	ChessViewTool();
private:
	DSizeF fullSize;
	DSizeF defaultSize;
	static ChessViewTool* s_instance;
	DSizeF curSize;
};

// begin_r_winuser

/*
 * Virtual Keys, Standard Set
 */
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5


namespace DebugPerformance
{

	//调试性能用代码
	long getCurrentTick();
	long setCurrentTick();
	void setTickValue(long l);
	long printOffsetTick(const char* text);
	long printOffsetTickAndSet(const char* text);

}


EXPORT_LOCAL inline void setAppSignatureCode(int code)
{
	extern int __signatureHashcode;
	__signatureHashcode = code;
}




#endif