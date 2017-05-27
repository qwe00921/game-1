#include "Common.h"
#include "main.h"
#include "Data.h"
#include "MapGenerate.h"

MapInfo::MapInfo()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_mTerrain.clear();
}

bool MapInfo::loadJson(Json::Value jsonVal)
{
	ParseInt(m_iWidth, jsonVal["width"]);
	ParseInt(m_iHeight, jsonVal["height"]);

	Json::Value mapList = jsonVal["mapList"];
	if (mapList.isArray())
	{
		for(UInt i = 0; i < mapList.size(); i++)
		{
			map<int, int> m_mRowElem;
			Json::Value mapVal = mapList[i][IntToString(i + 1)];
			if(mapVal.isArray())
			{
				for(UInt j = 0; j < mapVal.size(); j++)
				{
					Json::Value rowVal = mapVal[j];
					ParseInt(m_mRowElem[j], rowVal[IntToString(j + 1)]);
				}
				m_mTerrain[i] = m_mRowElem;
			}
		}
	}
	return true;
}

bool MapInfo::saveJson(int iIndex)
{
	Json::Value mapList(arrayValue);
	for(UInt i = 0; i < m_mTerrain.size(); i++)
	{
		Json::Value mapRow(arrayValue);
		map<int, int> m_mRowElem = m_mTerrain[i];
		for(UInt j = 0; j < m_mRowElem.size(); j++)
		{
			Json::Value rowVal;
			rowVal[IntToString(j + 1)] = m_mRowElem[j];
			mapRow.append(rowVal);
		}
		Json::Value mapVal;
		mapVal[IntToString(i + 1)] = mapRow;
		mapList.append(mapVal);
	}

	Json::Value jsonVal;
	jsonVal["width"] = m_iWidth;
	jsonVal["height"] = m_iHeight;
	jsonVal["mapList"] = mapList;

	FastWriter fastWriter;
	std::string out = fastWriter.write(jsonVal);

	__String* strFile = __String::createWithFormat("mapInfo/%d.json", iIndex+1);
	return ChessWriteFile(strFile->getCString(), (char*)out.c_str(), out.size()) ? true : false;
}

MapGenerate::MapGenerate()
{

}

MapGenerate::~MapGenerate()
{

}

void MapGenerate::GenerateMapJson()
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)
	char* chData = ChessReadFile("Hexzmap.e5");
	if(chData == NULL)
	{
		return;
	}

	int iSize = win32Const->iMapElemNum;
	for(int i = 0; i < iSize; i++)
	{
		GenerateMap(chData, i);
	}
	ChessFree(chData);
#endif
}

MapInfo* MapGenerate::getMapInfo(int iIndex)
{
	MapInfo* ptrMap = NULL;
	__String* strFile;
    strFile = __String::createWithFormat("mape5/%d.json", iIndex+1);
	char* chData = ChessReadFile(strFile->getCString());
	if (chData)
	{
		Reader reader;
		Json::Value jsonVal;
		if(reader.parse(chData, jsonVal, false))
		{
			ptrMap = new MapInfo();
			ptrMap->loadJson(jsonVal);
		}
		ChessFree(chData);
	}
	return ptrMap;
}

void MapGenerate::GenerateMap(char* chData, int iIndex)
{
	int point = 0x118 + iIndex * 12;
	point = (chData[point] & 0xff) << 24 | (chData[point + 1] & 0xff) << 16 | (chData[point + 2] & 0xff) << 8 | (chData[point + 3] & 0xff);

	MapInfo ptrMap;
	ptrMap.m_iWidth = chData[point++] / 3;
	ptrMap.m_iHeight = chData[point++] / 3;

	for(int i = 0; i < ptrMap.m_iHeight; i++)
	{
		map<int, int> m_mRowElem;
		for(int j = 0; j < ptrMap.m_iWidth; j++)
		{
			m_mRowElem[j] = chData[point++];
		}
		ptrMap.m_mTerrain[i] = m_mRowElem;
	}
	ptrMap.saveJson(iIndex);
}