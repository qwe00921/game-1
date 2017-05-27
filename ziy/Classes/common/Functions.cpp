#include "Common.h"
#include "main.h"

char* GetConstStr(const char* str)
{
	if(!str)
		return 0;
	else
	{
		char * ret = (char*)ChessAlloc(strlen(str)+1);
		strcpy(ret,str);
		return ret;
	}
}

char intString[32];
char *IntToString(int i)
{
	char buf[33];
	char neg = 0;
	int f, n;
	if(i<0) 
	{ neg= 1; i= -i; }
	f = 32;
	buf[f--]= 0;
	do
	{
		n= i%10;
		buf[f]=(char)('0'+n);
		i= i/10;
		f--;
	}while(i>0);
	if(neg) buf[f--]= '-';
	//str = DAlloc(32-f);
	strcpy(intString,&buf[f+1]);
	return intString;
}
char *IntToString_Sign(int i)
{
	char buf[33];
	char neg = 0;
	int f, n;
	if(i<0) 
	{ neg= 1; i= -i; }
	f = 32;
	buf[f--]= 0;
	do
	{
		n= i%10;
		buf[f]=(char)('0'+n);
		i= i/10;
		f--;
	}while(i>0);
	if(neg) buf[f--]= '-';
	else buf[f--] = '+';
	//str = DAlloc(32-f);
	strcpy(intString,&buf[f+1]);
	return intString;
}
int StringToInt(const char* str)
{
	char* end;
	const char* start;
	int ret;
	start = str;
	ret= strtol(start, &end, 10);
	if(start==end) 
		ret = 0;
	return ret;
}
#include "stdarg.h"
char chessTempStr[3][1000];
int chessTempStr_index = 0;
char * ChessSprintf(const char *fmt,...)
{
	chessTempStr_index++;
	if(chessTempStr_index==3)
		chessTempStr_index = 0;
	
	va_list vl;
	va_start (vl, fmt);
#ifdef WIN32
	_vsnprintf(chessTempStr[chessTempStr_index], 200, fmt, vl);
#else
	vsnprintf(chessTempStr[chessTempStr_index], 200, fmt, vl);
#endif
	va_end (vl);
	return chessTempStr[chessTempStr_index];
}
//////////////////////////////////////////////////////////////////////////////////////
//
int bytesToInt(const char* byteArrayData, int offset)
{
	return (byteArrayData[offset+3] & 0xff) << 24
		| (byteArrayData[offset+2] & 0xff) << 16
		| (byteArrayData[offset+1] & 0xff) << 8
		| (byteArrayData[offset] & 0xff);
}

short bytesToShort(const char* byteArrayData, int offset) {
	short a = (short) ((byteArrayData[offset + 1] & 0xff) << 8 | (byteArrayData[offset] & 0xff));
	return (short) (a > 32767 ? a - 65536 : a);
}


DString bytesToString(const char* byteArrayData, int offset)
{
	return DString((const char*)(byteArrayData+offset));
}

int isPointerInBound(int x, int y, int boundX, int boundY, int boundWidth, int boundHeight)
{
	if(x >= boundX && x < boundX + boundWidth && y >= boundY && y <= boundY + boundHeight)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////

DMemoryReader::DMemoryReader(char* data, int length, bool autoRelease)
{
	mData = data;
	mLength = length;
	mAutoRelease = autoRelease;
	mIndex = 0;
}

DMemoryReader::~DMemoryReader()
{
	if (mAutoRelease)
	{
		SAFE_DELETE_ARRAY(mData);
	}
	mLength = 0;
	mIndex = 0;
}

bool DMemoryReader::tryReadString(string& strValue)
{
	int strLen = 0;
	if (!tryReadInt(strLen) || strLen < 0 || mIndex + strLen > mLength)
	{
		return false;
	}
	char* buffer = new char[strLen + 1];
	memset(buffer, 0, strLen + 1);
	if (!tryReadCharArray(buffer, strLen))
	{
		SAFE_DELETE_ARRAY(buffer);
		return false;
	}
	strValue = buffer;
	SAFE_DELETE_ARRAY(buffer);
	return true;
}

bool DMemoryReader::tryReadChar(char& charValue)
{
	return tryReadCharArray(&charValue, 1);
}

bool DMemoryReader::tryReadShort(short& shortValue)
{
	char charShort[2] = { 0 };
	if (!tryReadCharArray(charShort, 2))
	{
		return false;
	}

	shortValue = (charShort[0] & 0xff) | ((charShort[1] & 0xff) << 8);
	return true;
}

bool DMemoryReader::tryReadInt(int& intValue)
{
	char charInt[4] = { 0 };
	if (!tryReadCharArray(charInt, 4))
	{
		return false;
	}

	intValue = (charInt[0] & 0xff) | ((charInt[1] & 0xff) << 8) | ((charInt[2] & 0xff) << 16) | ((charInt[3] & 0xff) << 24);
	return true;
}

bool DMemoryReader::tryReadCharArray(char* charArray, int readLen)
{
	if (charArray == NULL || readLen < 0 || mIndex + readLen > mLength)
	{
		return false;
	}

	if (readLen == 0)
	{
		return true;
	}

	memcpy(charArray, mData + mIndex, readLen);
	mIndex += readLen;
	return true;
}

bool DMemoryReader::tryReadSizeCharArray(char* charArray, int arrLen)
{
	int readLen = 0;
	if (!tryReadInt(readLen))
	{
		return false;
	}

	if (readLen > arrLen)
	{
		return false;
	}
	memset(charArray, 0, arrLen*sizeof(charArray[0]));
	return tryReadCharArray(charArray, readLen);
}

bool DMemoryReader::tryReadShortArray(short* shortArray, int readLen)
{
	if (shortArray == NULL || readLen < 0 || mIndex + readLen > mLength)
	{
		return false;
	}

	if (readLen == 0)
	{
		return true;
	}

	for (int i = 0; i < readLen; i++)
	{
		if (!tryReadShort(shortArray[i]))
		{
			return false;
		}
	}
	return true;
}

bool DMemoryReader::tryReadSizeShortArray(short* shortArray, int arrLen)
{
	int readLen = 0;
	if (!tryReadInt(readLen))
	{
		return false;
	}

	if (readLen > arrLen)
	{
		return false;
	}
	memset(shortArray, 0, arrLen*sizeof(shortArray[0]));
	return tryReadShortArray(shortArray, readLen);
}

bool DMemoryReader::tryReadIntArray(int* intArray, int readLen)
{
	if (intArray == NULL || readLen < 0 || mIndex + readLen > mLength)
	{
		return false;
	}

	if (readLen == 0)
	{
		return true;
	}

	for (int i = 0; i < readLen; i++)
	{
		if (!tryReadInt(intArray[i]))
		{
			return false;
		}
	}
	return true;
}

bool DMemoryReader::tryReadSizeIntArray(int* intArray, int arrLen)
{
	int readLen = 0;
	if (!tryReadInt(readLen))
	{
		return false;
	}

	if (readLen > arrLen)
	{
		return false;
	}
	memset(intArray, 0, arrLen*sizeof(intArray[0]));
	return tryReadIntArray(intArray, readLen);
}

bool DMemoryReader::tryReadMapInt(map<int, int>& mMapInt)
{
	int iMapSize = 0;
	tryReadInt(iMapSize);
	for (int i = 0; i < iMapSize; i++)
	{
		int iKey = 0;
		int iValue = 0;
		tryReadInt(iKey);
		tryReadInt(iValue);
		mMapInt[iKey] = iValue;
	}

	return true;
}

bool DMemoryReader::tryReadSizeString(char* strArray, int arrayLen)
{
	if (arrayLen <= 0)
	{
		return false;
	}
	int readLen = 0;
	if (!tryReadInt(readLen) || readLen > arrayLen)
	{
		return false;
	}

	if (!tryReadCharArray(strArray, readLen))
	{
		return false;
	}

	strArray[arrayLen - 1] = '\0';
	return true;
}

bool DMemoryReader::tryReadDPoint(DPoint& point)
{
	tryReadInt(point.x);
	tryReadInt(point.y);
	return true;
}

///////////////////////////////////////////////////////////////////////////

DMemoryWriter::DMemoryWriter()
{
	mData = new char[40];
	mAlloc = 40;
	mIndex = 0;
}

DMemoryWriter::~DMemoryWriter()
{
	SAFE_DELETE_ARRAY(mData);
	mAlloc = 0;
	mIndex = 0;
}

void DMemoryWriter::writeString(string strValue)
{
	int iSize = strValue.size();
	writeInt(iSize);
	writeCharArray((char*)strValue.c_str(), iSize);
}

void DMemoryWriter::writeInt(int intValue)
{
	char charInt[4] = { 0 };
	charInt[0] = intValue & 0xff;
	charInt[1] = (intValue >> 8) & 0xff;
	charInt[2] = (intValue >> 16) & 0xff;
	charInt[3] = (intValue >> 24) & 0xff;
	writeCharArray(charInt, 4);
}

void DMemoryWriter::writeShort(short shortValue)
{
	char charShort[2] = { 0 };
	charShort[0] = shortValue & 0xff;
	charShort[1] = (shortValue >> 8) & 0xff;
	writeCharArray(charShort, 2);
}

void DMemoryWriter::writeChar(char charValue)
{
	writeCharArray(&charValue, 1);
}

void DMemoryWriter::writeCharArray(char* charArray, int writeLen)
{
	if (mIndex + writeLen >= mAlloc)
	{
		int alloc1 = mIndex + writeLen + 1;
		int alloc2 = mAlloc * 2 + 1;
		reAllocData(max(alloc1, alloc2));
	}
	memcpy(mData + mIndex, charArray, writeLen);
	mIndex += writeLen;
}

void DMemoryWriter::writeSizeCharArray(char* charArray, int writeLen)
{
	writeInt(writeLen);
	writeCharArray(charArray, writeLen);
}

void DMemoryWriter::writeShortArray(short* shortArray, int writeLen)
{
	for (int i = 0; i < writeLen; i++)
	{
		writeShort(shortArray[i]);
	}
}

void DMemoryWriter::writeSizeShortArray(short* shortArray, int writeLen)
{
	writeInt(writeLen);
	writeShortArray(shortArray, writeLen);
}

void DMemoryWriter::writeIntArray(int* intArray, int writeLen)
{
	for (int i = 0; i < writeLen; i++)
	{
		writeInt(intArray[i]);
	}
}

void DMemoryWriter::writeSizeIntArray(int* intArray, int writeLen)
{
	writeInt(writeLen);
	writeIntArray(intArray, writeLen);
}

void DMemoryWriter::writeMapInt(map<int, int> mMapData)
{
	writeInt((int)mMapData.size());
	for (map<int, int>::iterator it = mMapData.begin(); it != mMapData.end(); it++)
	{
		writeInt(it->first);
		writeInt(it->second);
	}
}

bool DMemoryWriter::writeDPoint(DPoint point)
{
	writeInt(point.x);
	writeInt(point.y);
	return true;
}

void DMemoryWriter::reAllocData(int length)
{
	if (length <= mAlloc)
	{
		return;
	}

	char* newData = new char[length];
	memset(newData, 0, length);
	memcpy(newData, mData, mIndex);

	SAFE_DELETE_ARRAY(mData);

	mData = newData;
	mAlloc = length;
}

static const std::string NumToCN(int val){
    switch (val) {
        case 0: return LL("cn0");
        case 1: return LL("cn1");
        case 2: return LL("cn2");
        case 3: return LL("cn3");
        case 4: return LL("cn4");
        case 5: return LL("cn5");
        case 6: return LL("cn6");
        case 7: return LL("cn7");
        case 8: return LL("cn8");
        case 9: return LL("cn9");
        default:
            return NULL;
    }
}

const char * IntToCN(int val) {
    static std::string result;
    result.clear();
    if (val == 0)
        return LL("cn0");
    if(val / 10000 > 0) {
        result += NumToCN(val/10000) + LL("cn0");
        val = val - val / 10000 * 10000;
    }
    if(val / 1000 > 0) {
        result += NumToCN(val/1000) + LL("cnqian");
        val = val - val / 1000 * 1000;
    }
    if(val / 100 > 0) {
        if(!result.empty() && (result.find(LL("cnqian")) == std::string::npos))
            result += LL("cn0");
        result += NumToCN(val/100) + LL("cnbai");
        val = val - val / 100 * 100;
    }
    if(val / 10 > 0) {
        if(!result.empty() && (result.find(LL("cnbai")) == std::string::npos))
            result += LL("cn0");
        result += NumToCN(val/10) + LL("cnshi");
        val = val - val / 10 * 10;
    }
    if(val > 0) {
        if(!result.empty() && (result.find(LL("cnshi")) == std::string::npos))
            result += LL("cn0");
        result += NumToCN(val);
    }
    if(result.empty())
        return NULL;
    else
        return result.c_str();
}

const char* IntToStringEx(int i)
{
	char buf[33];
	char neg = 0;
	bool bNeedW = false;
	int f, n;
	if(i < 0)
	{
		neg = 1; 
		i = -i;
	}
	else if (i > 99999)
	{
		bNeedW = true;
		i /= 10000;
	}

	f = 32;
	buf[f--] = 0;

	if (bNeedW)
	{
		int iLenth = strlen(LL("cnwan"));
		memcpy(buf + f - iLenth+1, LL("cnwan"), iLenth*sizeof(char));
		f -= iLenth;
	}
	do
	{
		n = i % 10;
		buf[f] = (char)('0' + n);
		i = i / 10;
		f--;
	}
	while(i > 0);
	if(neg) buf[f--] = '-';
	strcpy(intString, &buf[f + 1]);
	return intString;
}