#include "Common.h"
//#include "malloc.h"
#include "main.h"
//#include "Font.h"
#include "PasswordLib.h"
#include "Data.h"
#include "cocos2d.h"

//UUCHAR byte_per_pixel = BYTES_PER_PIXEL;
//unsigned short screen_size_x	= SCREEN_SIZE_X;		
//unsigned short screen_size_y	= SCREEN_SIZE_Y;

////////////////////////////////////////////////////////////////////////////////////

DPointF::DPointF(const cocos2d::Vec2& pt)
{
	x = pt.x;
	y = pt.y;
}

DPointF::operator cocos2d::Vec2() const
{
	return cocos2d::Vec2(x, y);
}

DSizeF::DSizeF(const cocos2d::Size& sz)
{
	width = sz.width;
	height = sz.height;
}

DSizeF::operator cocos2d::Size() const
{
	return cocos2d::Size(width, height);
}

DRectF::DRectF(const cocos2d::Rect& rect)
{
	x = rect.origin.x;
	y = rect.origin.y;
	width = rect.size.width;
	height = rect.size.height;
}

DRectF::operator cocos2d::Rect() const
{
	return cocos2d::Rect(x, y, width, height);
}

float DRectF::getTop() const
{
	return y + height;
}

float DRectF::getRight() const
{
	return x + width;
}

DPointF DRectF::getLeftBottom() const
{
	return DPointF(x, y);
}

DPointF DRectF::getRightTop() const
{
	return DPointF(x + width, y + height);
}

DSizeF DRectF::getSize() const
{
	return DSizeF(width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////

ChessViewTool* ChessViewTool::s_instance = NULL;
ChessViewTool* ChessViewTool::getInstance()
{
	if (s_instance == NULL)
	{
		s_instance = new ChessViewTool();
	}

	return s_instance;
}

ChessViewTool::ChessViewTool()
{
	defaultSize = DSizeF(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	curSize = fullSize = defaultSize;
}

void ChessViewTool::setScreenSize(float scrWidth, float scrHeight)
{
	//CCEGLView::sharedOpenGLView()->setDesignResolutionSize(scrWidth, scrHeight, kResolutionShowAll);
	Chess::getInstance()->getGraphics()->resetClip();
}

DSizeF ChessViewTool::getScreenSize()
{
	return curSize;
}

void ChessViewTool::setScreenDefaultSize()
{
// 	curSize = defaultSize;
// 	ChessApplication::getInstance()->setChessLayerPointX((fullSize.width - defaultSize.width)/2);
// 	ChessApplication::getInstance()->setChessLayerPointY((fullSize.height - defaultSize.height) / 2);
// 	setScreenSize(defaultSize.width, defaultSize.height);
	setScreenFullSize();
}

void ChessViewTool::setScreenFullSize()
{
	curSize = fullSize;
	ChessApplication::getInstance()->setChessLayerPointX(0);
	ChessApplication::getInstance()->setChessLayerPointY(0);
	setScreenSize(fullSize.width, fullSize.height);
}

DSizeF ChessViewTool::getFullSize()
{
	return fullSize;
}

void ChessViewTool::computeFullSize(float frameWidth, float frameHeight)
{
	float h = 0;
	float w = 0;
	if(1.0f * frameHeight / frameWidth > 1.0f * SCREEN_SIZE_Y / SCREEN_SIZE_X)
	{
		float bili = (1.0f * frameHeight / frameWidth) / (1.0f * SCREEN_SIZE_Y / SCREEN_SIZE_X);
		h = SCREEN_SIZE_Y * bili;
		w = SCREEN_SIZE_X;
	}
	else
	{
		float bili = (1.0f * SCREEN_SIZE_Y / SCREEN_SIZE_X) / (1.0f * frameHeight / frameWidth);
		h = SCREEN_SIZE_Y;
		w = SCREEN_SIZE_X * bili;
	}
	fullSize = DSizeF(w, h);
}

DPointF ChessViewTool::screen2GL(const DPointF& pt)
{
	Vec2 ptView = pt;
	Vec2 ret = Director::getInstance()->convertToGL(ptView);
	return DPointF(ret);
}

DRectF ChessViewTool::screen2GL(const DRectF& rct)
{
	DPointF dstP = screen2GL(DPointF(rct.x, rct.getTop()));
	DRectF ret = DRectF(dstP, rct.getSize());
	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////

static DPixelFont* sPixelFontArray[Chess_FONT_MAX] = { 0 };

bool DPixelFont::loadGlobalFile(const char* fileName)
{
	unsigned long dataLen = 0;
	char *imgData = ChessReadFile(fileName, 0, 1, &dataLen);
	if (imgData == NULL)
	{
		return false;
	}

	DMemoryReader mr(imgData, dataLen, true);
	DMemoryReader* reader = &mr;

	char fileHead[4] = { 0 };
	int fileVersion = 0;
	if (!(reader->tryReadCharArray(fileHead, 4)
		&& reader->tryReadInt(fileVersion)
		&& reader->tryReadCharArray(fontName, 32)
		&& reader->tryReadInt(fontHeight)
		&& reader->tryReadInt(mapWidth)
		&& reader->tryReadInt(mapHeight)))
	{
		return false;
	}

	int arrLen = 0;
	if (!reader->tryReadInt(arrLen))
	{
		return false;
	}
	charArray.resize(arrLen);

	for (int i = 0; i < arrLen; i++)
	{
		if (!(reader->tryReadShort((short&)charArray[i].code)
			&& reader->tryReadShort((short&)charArray[i].charWidth)))
		{
			return false;
		}
	}

	for (int i = 0; i < arrLen; i++)
	{
		int bufLen = (charArray[i].charWidth + 7) / 8 * fontHeight;
		charArray[i].dataArray.resize(bufLen);
		if (!reader->tryReadCharArray(charArray[i].dataArray.getAddress(),
			charArray[i].dataArray.size()))
		{
			return false;
		}
	}
	
	return true;
}

DPixelFont::GlyphItem* DPixelFont::findGlyphItem(unsigned short code)
{
	int low = 0;
	int high = charArray.size() - 1;
	int position = (high + low) / 2;

	while ((high - low) > 1)
	{
		if (code == charArray[position].code)
		{
			return &charArray[position];
		}

		if (code > charArray[position].code)
		{
			low = position;
			position = (high + low) / 2;
		}
		else
		{
			high = position;
			position = (high - low) / 2;
		}
	}

	if (code == charArray[high].code)
	{
		return &charArray[high];
	}
	else if (code == charArray[low].code)
	{
		return &charArray[low];
	}

	return NULL;
}

DPixelFont* GUI_GetPixelFont(int fontId)
{
	if (fontId < 0 || fontId >= Chess_FONT_MAX)
	{
		return NULL;
	}

	return sPixelFontArray[fontId];
}

////////////////////////////////////////////////////////////////////////////////////////////////

void InitFont()
{

}

//static int mem_amount = 0;

void *ChessAlloc (unsigned int size)
{
	//mem_amount +=size;
	size = (size+7)/8*8;
	char* charArray = new char[size];
	memset(charArray, 0, size);
	void* ret = (void*)charArray;
	return ret;
}
void ChessFree (void *mem)
{
	if(mem)
	{
		//mem_amount -= _msize(mem);
		//free(mem);
		delete (char*)mem;
	}
	mem = 0;
}

#ifndef WIN32
//#include "jni.h"
//#include "jni/JniHelper.h"
#endif

ChessFile::ChessFile()
{
	mFile = NULL;
}
ChessFile::~ChessFile()
{
	close();
}

bool ChessFile::open(const char *filename, const char *mode)
{
	close();
	mFile = fopen(filename, mode);
	return mFile != NULL;
}

bool ChessFile::openWrite(const char *filename)
{
	return open(filename, "wb");
}

int ChessFile::write(const void *buffer, unsigned int size)
{
	if (mFile == NULL)
	{
		return 0;
	}
	return fwrite(buffer, size, 1, mFile);
}

void ChessFile::close()
{
	if (mFile != NULL)
	{
		fclose(mFile);
		mFile = NULL;
	}
}

int ChessFile::fremove(const char *filename)
{
	return remove(filename);
}

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include <sys/stat.h>
#endif
void ChessFile::createDirectory(const char* dirPath)
{
	DString newDir = dirPath;
	if (!isDirectoryExist(newDir->getChar()))
	{
		for (int i = 1; i < newDir->length(); i++)
		{
			if (newDir->charAt(i) == '\\' || newDir->charAt(i) == '/')
			{
				DString subDir = newDir->substring(0, i);
				if (!isDirectoryExist(subDir->getChar()))
				{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
					mkdir(subDir->getChar(), S_IRWXU | S_IRWXG | S_IRWXO);
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
					CreateDirectoryA(subDir->getChar(), 0);
#endif
				}
			}
		}

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
		mkdir(newDir->getChar(), S_IRWXU | S_IRWXG | S_IRWXO);
#elif (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
		CreateDirectoryA(newDir->getChar(), 0);
#endif
	}

}

bool ChessFile::isDirectoryExist(const char* dirPath)
{
	return FileUtils::getInstance()->isFileExist(dirPath);
}

bool ChessFile::isFileExist(const char* filePath)
{
	return FileUtils::getInstance()->isFileExist(filePath);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
static void logNotFoundFile(const char* filePath)
{
	static int status = -1;
	if (status == 0)
	{
		return;
	}
	DString logPath = ChessFile::getResourcePath("/notfound.log", 1, 1);
	if (status == -1)
	{
		if (ChessFile::isFileExist(logPath->getChar()))
		{
			status = 1;
		}
		else
		{
			status = 0;
			return;
		}
	}
	ChessFile file;
	if (file.open(logPath->getChar(), "a"))
	{
		time_t tmt;
		time(&tmt);
		struct tm * tm = localtime(&tmt);

		DString strContent = DString::format("%d:%d:%d %s not found!!!\r\n", tm->tm_hour, tm->tm_min, tm->tm_sec, filePath);
		file.write(strContent->getChar(), strContent->length());
		file.close();
	}
}
#endif

bool ChessFile::directReadFile(const char* filepath, char** data, unsigned long* dataLength)
{
	string strPath = filepath;

	ssize_t size = 0;
	char* file = NULL;
	Data fileData = FileUtils::getInstance()->getDataFromFile(strPath);
	if(!fileData.isNull())
	{
		file = (char*)fileData.getBytes();
		size = fileData.getSize();
	}

    char * file1 = new char[size + 1]; //FIXME 在结尾增加一个byte的0，防止上层处理strlen时越界
    memcpy(file1, file, size);
    file1[size] = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (file == NULL)
	{
		logNotFoundFile(filepath);
	}
#endif

	if (file == NULL)
	{
		return false;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	ChessFile::saveResourceGenData(filepath, file1, size);
#endif

	*data = file1;
	*dataLength = size;
	return true;
}

bool ChessFile::encryptReadFile(const char* filepath, char** data, unsigned long* dataLength)
{
	string strPath = filepath;

	ssize_t size = 0;
	char* file = NULL;
	Data fileData = FileUtils::getInstance()->getDataFromFile(strPath);
	if(!fileData.isNull())
	{
		file = (char*)fileData.getBytes();
		size = fileData.getSize();
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (file == NULL)
	{
		logNotFoundFile(filepath);
	}
#endif

	if (file == NULL)
	{
		return false;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	ChessFile::saveResourceGenData(filepath, file, size);
#endif

	SecretFile sf;
	int len = 0;
	char* temp = sf.decryptData(file, size, &len);

	if (temp == NULL)
	{
		return false;
	}
    char * temp1 = new char[len + 1]; //FIXME 在结尾增加一个byte的0，防止上层处理strlen时越界
    memcpy(temp1, temp, len);
    free(temp);
    temp1[len] = 0;


	*data = temp1;
	*dataLength = len;
	return true;
}

bool ChessFile::directWriteFile(const char* filepath, const char* data, unsigned long dataLength)
{
	ChessFile file;
	if (!file.openWrite(filepath))
	{
		return false;
	}

	file.write(data, dataLength);
	file.close();

	return true;
}

bool ChessFile::encryptWriteFile(const char* filepath, const char* data, unsigned long dataLength)
{
	ChessFile file;
	if (!file.openWrite(filepath))
	{
		return false;
	}

	int afterSize = 0;
	SecretFile sf;
	sf.setCodeSecret(1);
	char* temp = sf.encryptData((char*)data, dataLength, &afterSize);

	file.write(temp, afterSize);
	file.close();

	SAFE_DELETE_ARRAY(temp);
	return true;
}

DString ChessFile::findParentDir(DString path)
{
	DString newDir = path;
	for (int i = newDir->length() - 1; i >= 0; i--)
	{
		if (newDir->charAt(i) == '\\' || newDir->charAt(i) == '/')
		{
			newDir = newDir->substring(0, i);
			break;
		}
	}
	return newDir;
}

bool ChessFile::needGenResource()
{
	static int miniState = -1;
	if (miniState == -1)
	{
		bool miniExist = isDirectoryExist("mini_res_gen");
		if (miniExist)
		{
			miniState = 1;
		}
		else
		{
			miniState = 0;
		}
	}
	if (miniState != 1)
	{
		return false;
	}
	return true;
}

void ChessFile::saveResourceGen(const char* srcpath)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	if (!needGenResource())
	{
		return;
	}
	DString newPath = DString(DIR_RESOURCE_GEN) + "\\" + DString(srcpath);
	if (isFileExist(newPath->getChar()))
	{
		return;
	}

	DString newDir = findParentDir(newPath);
	if (!isDirectoryExist(newDir->getChar()))
	{
		createDirectory(newDir->getChar());
	}

	Data fileData = FileUtils::getInstance()->getDataFromFile(srcpath);
	if (!fileData.isNull())
	{
		directWriteFile(newPath->getChar(), (char*)fileData.getBytes(), fileData.getSize());
	}
#endif
}

void ChessFile::saveResourceGenData(const char* srcpath, char* data, int length)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	if (data == NULL)
	{
		return;
	}

	if (!needGenResource())
	{
		return;
	}
	DString newPath = DString(DIR_RESOURCE_GEN) + "\\" + DString(srcpath);
	if (isFileExist(newPath->getChar()))
	{
		return;
	}

	DString newDir = findParentDir(newPath);
	if (!isDirectoryExist(newDir->getChar()))
	{
		createDirectory(newDir->getChar());
	}

	directWriteFile(newPath->getChar(), data, length);
#endif
}

#ifdef GAME_RES_MINI
char* getMiniResourceRelativeFile(const char *filename, int is_writeable, int is_globe)
{
	static char respath[200];
	strcpy(respath, "");

	DString resultPath = ChessFile::getMiniResourceRelativeFile(filename, is_writeable, is_globe);
	strcpy(respath, resultPath->getChar());

	return (char*)respath;
}
#endif

#ifdef GAME_RES_MINI
DString ChessFile::getMiniResourceRelativeFile(const char *filename, int is_writeable, int is_globe)
{
	DString strFileName = "";
	if (filename[0] == '/')
	{
		strFileName = DString(filename)->substring(1);
	}
	else
	{
		if (is_globe == 1)
		{
			strFileName = DString::format("globalres/%s", filename);
		}
		else if (is_globe == 2)
		{
			strFileName = DString::format("Animation/%s", filename);
		}
		else
		{
			strFileName = DString::format("%s/%s", funConst->GamePath->getChar(), filename);
		}
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	strFileName = "GameResources/" + strFileName;
#endif	

	if (is_writeable)
	{
		DString result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		result = strFileName;
#else
		result = FileUtils::getInstance()->getWritablePath().c_str();
		result = result + filename;
		CCLOG("getWritablePath path is %s", result->getChar());
#endif
		return result;
	}
	else
	{
		DString result = strFileName->getChar();

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID		
		return result;
#else
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		DString strTemp = RES_PATH + strFileName;
		result = strTemp;
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		strTemp = RES_PATH2 + strFileName;
		result = strTemp;
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		CCLOG("%s result2 = not Exist£°£°£°", result->getChar());
		return result;
#endif
	}
}
#endif

DString ChessFile::getResourcePath(const char *filename, int is_writeable, int is_globe)
{
	DString strFileName = "";
	if (filename[0] == '/')
	{
		strFileName = DString(filename)->substring(1);
	}
	else
	{
		if (is_globe == 1)
		{
			strFileName = DString::format("globalres/%s", filename);
		}
		else if(is_globe == 2)
		{
			strFileName = DString::format("Animation/%s", filename);
		}
		else
		{
			strFileName = DString::format("%s/%s", funConst->GamePath->getChar(), filename);
		}
	}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	strFileName = "GameResources/" + strFileName;
#endif	

	if (is_writeable)
	{
		DString result = "";
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		result = strFileName;
#else
		result = FileUtils::getInstance()->getWritablePath().c_str();
		result = result + filename;
		CCLOG("getWritablePath path is %s", result->getChar());
#endif
		return result;
	}
	else
	{
		DString result = strFileName->getChar();

#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID		
		return result;
#else
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		DString strTemp = RES_PATH + strFileName;
		result = strTemp;
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		strTemp = RES_PATH2 + strFileName;
		result = strTemp;
		if (ChessFile::isFileExist(result->getChar()))
		{
			return result;
		}

		CCLOG("%s result2 = not Exist£°£°£°", result->getChar());
		return result;
#endif
	}
}

int ChessFile::removeFile(const char* filepath)
{
	ChessFile file;
	return file.fremove(filepath);
}

char* getRespath(const char *filename,int is_writeable,int is_globe)
{
	static char respath[200];
	strcpy(respath, "");

	DString resultPath = ChessFile::getResourcePath(filename, is_writeable, is_globe);
	strcpy(respath, resultPath->getChar());
	
	return (char*)respath;	
}


char* ChessReadFile(const char *filename, int is_writeable, int is_globe, unsigned long* fileLength)
{
#ifdef GAME_RES_MINI
	MiniSupport::checkMiniTRImagePtr(filename, is_writeable, is_globe);
#endif
	if (is_globe || !funConst->EncryptResource)
	{
		//全局资源不加密
		//设置为不加密的时候不加密
		return ChessDirectReadFile(filename, is_writeable, is_globe, fileLength);
	}
	else
	{
		return ChessEncryptReadFile(filename, is_writeable, is_globe, fileLength);
	}
}

char* ChessDirectReadFile(const char *filename, int is_writeable, int is_globe, unsigned long* fileLength)
{
	char * path = getRespath(filename, is_writeable, is_globe);

	if (path == NULL)
	{
		return NULL;
	}

	char* result = NULL;
	unsigned long readLen = 0;
	if (!ChessFile::directReadFile(path, &result, &readLen))
	{
		return NULL;
	}

	if (fileLength != NULL)
	{
		*fileLength = readLen;
	}
	return result;
}

char* ChessEncryptReadFile(const char *filename, int is_writeable, int is_globe, unsigned long* fileLength)
{
	char * path = getRespath(filename, is_writeable, is_globe);

	if (path == NULL)
	{
		return NULL;
	}

	char* result = NULL;
	unsigned long readLen = 0;
	if (!ChessFile::encryptReadFile(path, &result, &readLen))
	{
		return NULL;
	}

	if (fileLength != NULL)
	{
		*fileLength = readLen;
	}
	return result;
}

int ChessWriteFile(const char *filename,char* buffer,int size)
{		
	if (funConst->EncryptResource)
	{
		return ChessEncryptWriteFile(filename, buffer, size);
	}
	else
	{
		return ChessDirectWriteFile(filename, buffer, size);
	}
}

int ChessDirectWriteFile(const char *filename, char* buffer, int size)
{
	char * path = getRespath(filename, 1, 0);
	CCLOG("path = %s", path);

	bool result = ChessFile::directWriteFile(path, buffer, size);

	return result ? 1 : 0;
}

int ChessEncryptWriteFile(const char *filename, char* buffer, int size)
{
	char * path = getRespath(filename, 1, 0);
	CCLOG("path = %s", path);

	bool result = ChessFile::encryptWriteFile(path, buffer, size);

	return result ? 1 : 0;
}

int ChessRemoveFile(const char* filename)
{
	char * path = getRespath(filename, 1, 0);
	CCLOG("path = %s", path);
	return ChessFile::removeFile(path);
}

char * getTimeString()
{
	static char time_str[30] = {0};
#ifndef WIN32
    struct timeval now;
    gettimeofday(&now, NULL);
	struct tm *tm;
	tm = localtime((const time_t *)&now.tv_sec);
	//int year = tm->tm_year + 1900;
	int wMonth = tm->tm_mon + 1;
	int wDay = tm->tm_mday;
	int wHour = tm->tm_hour;
	int wMinute = tm->tm_min;
	int wSecond = tm->tm_sec;

	
	strcpy(time_str,IntToString(wMonth));
	strcat(time_str,"M");
	strcat(time_str,IntToString(wDay));
	strcat(time_str,"D-");
	strcat(time_str,IntToString(wHour));
	strcat(time_str,":");
	strcat(time_str,IntToString(wMinute));
	strcat(time_str,".");
	strcat(time_str,IntToString(wSecond));
	sprintf(time_str, LL("xitongshijian"), wMonth, wDay, wHour, wMinute);
#else
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	int year = systemTime.wYear;
	int wMonth = systemTime.wMonth;
	int wDay = systemTime.wDay;
	int wHour = systemTime.wHour;
	int wMinute = systemTime.wMinute;
	int wSecond = systemTime.wSecond;
	sprintf(time_str, LL("xitongshijian"), wMonth, wDay, wHour, wMinute);
#endif
	return time_str;
}

char* GetNewTimeString()
{
	static char time_str[20] = {0};
#ifndef WIN32
    struct timeval now;
    gettimeofday(&now, NULL);
	struct tm *tm;
	tm = localtime((const time_t *)&now.tv_sec);
	int year = tm->tm_year + 1900;
	int wMonth = tm->tm_mon + 1;
	int wDay = tm->tm_mday;
	int wHour = tm->tm_hour;
	int wMinute = tm->tm_min;
	int wSecond = tm->tm_sec;
	sprintf(time_str, "%d%02d%02d%02d%02d%02d", year, wMonth, wDay, wHour, wMinute, wSecond);
#else
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	int year = systemTime.wYear;
	int wMonth = systemTime.wMonth;
	int wDay = systemTime.wDay;
	int wHour = systemTime.wHour;
	int wMinute = systemTime.wMinute;
	int wSecond = systemTime.wSecond;
	sprintf(time_str, "%d%02d%02d%02d%02d%02d", year, wMonth, wDay, wHour, wMinute, wSecond);
#endif
	return time_str;
}

namespace DebugPerformance
{

	long getCurrentTick()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	long setCurrentTick()
	{
		long ret = getCurrentTick();
		setTickValue(ret);
		return ret;
	}

	static long currentTick = 0;

	void setTickValue(long l)
	{
		currentTick = l;
	}

	long printOffsetTick(const char* text)
	{
		long ret = getCurrentTick() - currentTick;
		CCLOG("tick:%s=%ld", text, ret);
		return ret;
	}

	long printOffsetTickAndSet(const char* text)
	{
		long l = getCurrentTick();
		long ret = l - currentTick;
		CCLOG("tick:%s=%ld", text, ret);
		setTickValue(l);
		return ret;
	}

}


int __signatureHashcode = 0;

