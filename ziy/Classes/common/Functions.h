#ifndef _CHESS_FUNCTIONS_H_
#define _CHESS_FUNCTIONS_H_

char* GetConstStr(const char* str);
char *IntToString(int i);
int StringToInt(const char* str);
char* ChessSprintf(const char *fmt,...);
const char* IntToStringEx(int i); //超过99999显示w

int bytesToInt(const char* byteArrayData, int offset);
short bytesToShort(const char* byteArrayData, int offset);
DString bytesToString(const char* byteArrayData, int offset);

int isPointerInBound(int x, int y, int boundX, int boundY, int boundWidth, int boundHeight);
char *IntToString_Sign(int i);

////////////////////////////////////////////////////////////////////////////////////

#include "DArray.h"

using namespace std;

/**
* 从内存中读取数据
*/
class DMemoryReader
{
private:
	char* mData;
	int mLength;
	int mIndex;
	int mAutoRelease;
public:
	DMemoryReader(char* data, int length, bool autoRelease);
	~DMemoryReader();
public:
	/**
	* 读取一个字符串
	* @param [out] strValue 读出的内容
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadString(string& strValue);
	/**
	* 读取一个整数
	* @param [out] charValue 读出的内容
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadChar(char& charValue);
	/**
	* 读取一个整数
	* @param [out] shortValue 读出的内容
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadShort(short& shortValue);
	/**
	* 读取一个整数
	* @param [out] intValue 读出的内容
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadInt(int& intValue);	
	/**
	* 读取一个数组
	* @param [out] charArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadCharArray(char* charArray, int readLen);
	/**
	* 读取一个长度和数组
	* @param [out] charArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadSizeCharArray(char* charArray, int arrLen);
	/**
	* 读取一个数组
	* @param [out] shortArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadShortArray(short* shortArray, int readLen);
	/**
	* 读取一个长度和数组
	* @param [out] shortArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadSizeShortArray(short* shortArray, int arrLen);
	/**
	* 读取一个数组
	* @param [out] intArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadIntArray(int* intArray, int readLen);
	/**
	* 读取一个长度和数组
	* @param [out] intArray 读出的内容
	* @param readLen 读取内容的长度
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadSizeIntArray(int* intArray, int arrLen);
	/**
	* 读取一个变长字符串
	* @param [out] strArray 读出的内容
	* @param arrayLen 读取内容的长度，包含0
	* @return false，读取失败；true，读取成功
	*/
	bool tryReadSizeString(char* strArray, int arrayLen);

	bool tryReadDPoint(DPoint& point);
	bool tryReadMapInt(map<int, int>& mMapInt);
public:
	/**
	* 设置当前下标
	* @param index 下标
	*/
	void setIndex(int index)
	{
		mIndex = index;
	}
	/**
	* 获取当前下标
	* @return 下标
	*/
	int getIndex() const
	{
		return mIndex;
	}
	/**
	* 获取数据长度
	* @return 数据长度
	*/
	int getDataLength() const
	{
		return mLength;
	}
	/**
	* 获取数据地址
	* @return 数据地址
	*/
	char* getDataAddr() const
	{
		return mData;
	}
};


/**
* 从写入数据到内存
*/
class DMemoryWriter
{
private:
	char* mData;
	int mAlloc;
	int mIndex;
public:
	DMemoryWriter();
	~DMemoryWriter();
public:
	/**
	* 获取数据地址
	* @return 数据地址
	*/
	char* getDataAddr(){ return mData; }
	/**
	* 获取数据长度
	* @return 数据长度
	*/
	int getDataLength(){ return mIndex; }
public:
	/**
	* 写入一个字符串
	*/
	void writeString(string strValue);
	/**
	* 写入一个整数
	* @param intValue 写入整数
	*/
	void writeInt(int intValue);
	/**
	* 写入一个二字节整数
	* @param intValue 写入二字节整数
	*/
	void writeShort(short shortValue);
	/**
	* 写入一个一字节整数
	* @param intValue 写入二字节整数
	*/
	void writeChar(char charValue);
	/**
	* 写入一个数组
	* @param charArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeCharArray(char* charArray, int writeLen);
	/**
	* 写入一个数组长度和数组内容
	* @param charArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeSizeCharArray(char* charArray, int writeLen);
	/**
	* 写入一个short数组
	* @param shortArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeShortArray(short* shortArray, int writeLen);
	/**
	* 写入一个short数组长度和内容
	* @param shortArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeSizeShortArray(short* shortArray, int writeLen);
	/**
	* 写入一个int数组
	* @param intArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeIntArray(int* intArray, int writeLen);
	/**
	* 写入一个int数组长度和内容
	* @param intArray 写入的内容
	* @param writeLen 写入的数组长度
	*/
	void writeSizeIntArray(int* intArray, int writeLen);

	bool writeDPoint(DPoint point);
	void writeMapInt(map<int, int> mMapData);
private:
	void reAllocData(int length);
};



#endif