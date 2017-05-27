#ifndef _MAPGENERATE_H_
#define _MAPGENERATE_H_
    
#include "DList.h"
#include "DString.h"
#include "jsons.h"

using namespace Json;
using namespace std;


class MapInfo
{
public:
	MapInfo();
	bool loadJson(Json::Value jsonVal);
	bool saveJson(int iIndex);
public:
	int m_iWidth;
	int m_iHeight;
	map<int, map<int, int> > m_mTerrain;
};

class MapGenerate
{
public:
	MapGenerate();
	~MapGenerate();

	static void GenerateMapJson();
	static MapInfo* getMapInfo(int iIndex);
private:
	static void GenerateMap(char* chData, int iIndex);
};

#endif // _MAPGENERATE_H_