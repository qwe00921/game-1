#include "Common.h"
#include "jsons.h"
#include "main.h"
#include "TrPayInfo.h"
#include "DString.h"
#include "otherfunction.h"
#include "ActionLayer.h"
#include "BattleLadder.h"

#define JSON_READ_HEAD(A)char* sData = ChessReadFile(A);if (sData == NULL){return;};Json::Reader reader;Json::Value line;if (!reader.parse(sData, line, false)){ CCLOG("WTF %s",A);assert(false); return;}int size = line.size()
#define JSON_READ_END ChessFree(sData)
#define JSON_WRITE_HEAD Json::FastWriter fast_writer;Json::Value root;Json::Value line
#define JSON_WRITE_END(A) 	std::string out = fast_writer.write(root);ChessWriteFile(A, (char*)out.c_str(), out.size());
#define FUZHIINT(A,B) A = B
#define FUZHI1INT(A,B) B = A
#define FUZHI1INT2(A,B) 		if (A == 255)FUZHI1INT(0, B);else FUZHI1INT(A + 1, B);
#define FUZHIINT2(A,B) FUZHIINT(A, B - 1);
#define FUZHISTRING(A,B) strcpy(A,B)
#define FUZHI1STRING(A,B) B = A;
#define FUZHIINTEX(A, B) if (B.isInt()) A = B.asInt();
#define FUZHISTRINGEX(A,B) if (B.isString()) A = B.asCString();

#define FUZHIMAX(A, B) if(A > B) B = A;
#define FUZHI2MAX(A, B) if(A + 1 > B) B = A + 1;
#define FUZHIMIN(A, B) if(A < B) B = A;


//arr:数组
//json_test: json中的字符串;
int ChessData::getjson_stringeffect(int**arr, const char* json_test)
{
	int *intarr=NULL;
	int next_record = 0;
	int test_len = strlen(json_test);
	int j = 0, k = 0, l = 0, m = 0;
	while (j < test_len)
	{
		next_record = StringToInt((char*)json_test + j);
		j++;
		j += GetNumDigit(next_record);
		m++;
	}

	intarr = new int[m];

	while (k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		FUZHIINT(intarr[l++], next_record);
	}
	*arr = intarr;
	intarr = NULL;
	return m;
}
void ChessData::ArrGetJsonInfo(int* arr, const char* json_test,int arrLen)
{
	int next_record = 0;
	int test_len = strlen(json_test);
	int k = 0,l = 0;
	while (k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		FUZHIINT(arr[l++], next_record);
		if (l >= arrLen)
			break;
	}
}

void ChessData::getdicseid()
{
	Json::Reader reader;
	Json::Value line;
	char* chData = NULL;

	chData = ChessReadFile("json/dic_seid.json");
	if(chData && reader.parse(chData, line, false))
	{
		m_mSeid[0] = new Seid(); //防止seid为零时闪退
		for (UInt i = 0; i < line.size(); i++)
		{
			Seid* ptrSeid = new Seid();
			ptrSeid->seid = line[i]["seid"].asInt();
			ptrSeid->type = line[i]["type"].asInt();
			ptrSeid->imgid = line[i]["img"].asInt();
			FUZHISTRING(ptrSeid->sename, line[i]["sename"].asCString());
			FUZHISTRING(ptrSeid->Info, line[i]["Info"].asCString());
			GetSeidData(line[i]["special_effe"].asCString(), ptrSeid->special_effe_num, ptrSeid->special_effe);
			if(line[i]["quality"].isString())
				ptrSeid->mOpenCondition = getSeidData(line[i]["quality"].asCString());
			m_mSeid[ptrSeid->seid] = ptrSeid;
		}
		ChessFree(chData);
	}

	chData = ChessReadFile("json/dic_strengthen_property.json");
	if(chData && reader.parse(chData, line, false))
	{
		strengthen_property_num = line.size();
		strengthen_property = new StrengthenProperty[strengthen_property_num];

		for(UInt i = 0; i < line.size(); i++)
		{
			FUZHIINT(strengthen_property[i].id, line[i]["id"].asInt());
			FUZHIINT(strengthen_property[i].se_type, line[i]["se_type"].asInt());
			FUZHIINT(strengthen_property[i].quality, line[i]["quality"].asInt());
			FUZHIINT(strengthen_property[i].odds, line[i]["odds"].asInt());
			FUZHISTRING(strengthen_property[i].name, line[i]["name"].asCString());
			FUZHIINT(strengthen_property[i].share, line[i]["share"].asInt());
			FUZHIINT(strengthen_property[i].seid, line[i]["seid"].asInt());

			//"effect"数据加入到Seid数组，id+MAX_SEID_NUM
			int iSeid = strengthen_property[i].id + MAX_SEID_NUM;
			Seid* ptrSeid = new Seid();
			ptrSeid->seid = iSeid;
			ptrSeid->type = strengthen_property[i].se_type;
			GetSeidData(line[i]["effect"].asCString(), ptrSeid->special_effe_num, ptrSeid->special_effe);
			m_mSeid[ptrSeid->seid] = ptrSeid;
		}
		ChessFree(chData);
	}

	chData = ChessReadFile("json/net_seid_add.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			SeidType* ptrType = new SeidType();
			ptrType->iTypeId = line[i]["type"].asInt();
			ptrType->iAddType = line[i]["add_type"].asInt();
			ptrType->iAddIndex = line[i]["add_pos"].asInt();
			ptrType->iClass = line[i]["class"].asInt();
			m_mSeidType[ptrType->iTypeId] = ptrType;
		}
		ChessFree(chData);
	}

	for (map<int, Seid*>::iterator it = m_mSeid.begin(); it != m_mSeid.end(); it++)
	{
		Seid* ptrSeid = it->second;
		if(m_mSeidType.count(ptrSeid->type) && !m_mSeidType[ptrSeid->type]->iEffNum)
		{
			m_mSeidType[ptrSeid->type]->iEffNum = ptrSeid->special_effe_num;
		}
	}
}

void ChessData::getItemType()
{
	m_mItemType.clear();
	JSON_READ_HEAD("json/dic_itemType.json");
	for (int i = 0, index = 0; i < size; i++)
	{
		ItemType* ptrItem = new ItemType();
		FUZHIINT(ptrItem->itid, line[i]["itid"].asInt());
		FUZHIINT(ptrItem->typeId, line[i]["typeid"].asInt());
		FUZHISTRING(ptrItem->itname, line[i]["itname"].asCString());
		FUZHIINT(ptrItem->itfun, line[i]["itfun"].asInt());
		FUZHIINT(ptrItem->itpos, line[i]["itpos"].asInt());
		m_mItemType[ptrItem->itid] = ptrItem;
		if (ptrItem->itpos >0 && ptrItem->itpos < 3)
		{
			m_mGoodsItemItid[index++] = ptrItem->itid;
		}
	}
	JSON_READ_END;
}


void ChessData::getDicJobClass()
{
	JSON_READ_HEAD("json/dic_job_class.json");
	occupation_type_num = size;
	occu_types = new OccupationType[occupation_type_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(occu_types[i].occupation_type_id, line[i]["cid"].asInt());
		FUZHIINT(occu_types[i].cval, line[i]["cval"].asInt());
		FUZHIINT2(occu_types[i].occupation_start, line[i]["jobid"].asInt());
		FUZHISTRING(occu_types[i].occupation_type_name, line[i]["name"].asCString());
		FUZHIINT(occu_types[i].atk_music, line[i]["atk_music"].asInt());
		FUZHIINT(occu_types[i].job_type, line[i]["job_type"].asInt());
	}
	JSON_READ_END;
}
void ChessData::getDicJobPk()
{
	JSON_READ_HEAD("json/dic_job_pk.json");
	jobpk_num = size;
	jobpk = new JobPK[jobpk_num];
	for (int i = 0; i < jobpk_num; i++)
	{
		FUZHIINT(jobpk[i].cid, line[i]["cid"].asInt());
		FUZHIINT(jobpk[i].cidpk, line[i]["cidpk"].asInt());
		FUZHIINT(jobpk[i].hart, line[i]["hart"].asInt());
	}
	JSON_READ_END;
}
void ChessData::getDicJobItem()
{
	JSON_READ_HEAD("json/dic_jobItem.json");

	int iMaxItid = 1;
	for(UInt i = 0; i < m_mGoodsItemItid.size(); i++)
	{
		if(iMaxItid < m_mGoodsItemItid[i] + 1)
		{
			iMaxItid = m_mGoodsItemItid[i] + 1;
		}
	}

	occupation_good_info = new UUCHAR*[size];
	for (int i = 0; i < size; i++)
	{
		occupation_good_info[i] = new UUCHAR[iMaxItid];
		memset(occupation_good_info[i], 0, iMaxItid);

		for(UInt j = 0; j < m_mGoodsItemItid.size(); j++)
		{
			int iItid = m_mGoodsItemItid[j];
			occupation_good_info[i][iItid] = line[i][IntToString(iItid)].asInt();
		}
		occupation_good_info[i][0] = 1;
	}
	JSON_READ_END;
}

void ChessData::getDicJobTerrain()
{
	JSON_READ_HEAD("json/dic_jobTerrain.json");
	occupation_land_info = new UUCHAR*[occupation_num];
	for (int i = 0; i < occupation_num; i++)
	{
		occupation_land_info[i] = new UUCHAR[land_num];
		memset(occupation_land_info[i], 0, land_num);
	}
	int* land_attr = new int[land_num + 1];
	for (int i = 0; i < occupation_type_num - 1; i++)//去掉一个所有兵种
	{
		for (int j = 0; j < land_num; j++)
		{
			FUZHIINT(land_attr[j + 1], line[i][IntToString(j + 1)].asInt());
		}
		for (int k = 0; k < land_num; k++)
		{
			for (int j = 0; j < occu_types[i].cval; j++)
			{
				occupation_land_info[occu_types[i].occupation_start + j][k] = land_attr[k + 1];
			}
		}
	}
	delete[] land_attr;
	land_attr = NULL;
	JSON_READ_END;
}
void ChessData::getDicJobWalk()
{
	JSON_READ_HEAD("json/dic_jobWalk.json");
	occupation_landmove_info = new UUCHAR*[occupation_num];
	for (int i = 0; i < occupation_num; i++)
	{
		occupation_landmove_info[i] = new UUCHAR[land_num];
		memset(occupation_landmove_info[i], 0, land_num);
	}
	int* land_attr = new int[land_num + 1];
	for (int i = 0; i < occupation_type_num - 1; i++)//去掉一个所有兵种
	{
		for (int j = 0; j < land_num; j++)
		{
			FUZHIINT(land_attr[j + 1], line[i][IntToString(j + 1)].asInt());
		}
		for (int k = 0; k < land_num; k++)
		{
			for (int j = 0; j < occu_types[i].cval; j++)
			{
				occupation_landmove_info[occu_types[i].occupation_start + j][k] = land_attr[k + 1];
			}
		}

	}
	delete[] land_attr;
	land_attr = NULL;
	JSON_READ_END;
}

void ChessData::getDicKillTxt()
{
	JSON_READ_HEAD("json/dic_kill_txt.json");
	int killid;
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(killid, line[i]["killid"].asInt());
		FUZHISTRING(kill_words[killid][0], line[i]["1"].asCString());
		FUZHISTRING(kill_words[killid][1], line[i]["2"].asCString());
		FUZHISTRING(kill_words[killid][2], line[i]["3"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getAtkSe(const char* sData, int index)
{
	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))
		return;
	int size = line.size();
	hitArea[index].length = size;
	hitArea[index].pointlist = new DPoint[size];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(hitArea[index].pointlist[i].x, line[i]["x"].asInt());
		FUZHIINT(hitArea[index].pointlist[i].y, line[i]["y"].asInt());
	}
	if (size == 1)
	{
		if (hitArea[index].pointlist[0].x == 999)//全屏
		{
			hitArea[index].length = -1;
		}
		if (hitArea[index].pointlist[0].x == 995)//无攻击力
		{
			hitArea[index].length = -5;
		}
	}
}

void ChessData::getDicAtkSe()
{
	JSON_READ_HEAD("json/dic_atk_se.json");
	int atk_id;
	char name[100];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(atk_id, line[i]["atk_id"].asInt());
		FUZHISTRING(name, line[i]["name"].asCString());
		getAtkSe(line[i]["json"].asCString(), i);
		FUZHISTRING(hitArea[i].cname, line[i]["cname"].asCString());
	}
	JSON_READ_END;
}
void ChessData::getMpSe(const char* sData, int index)
{
	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))
		return;
	int size = line.size();
	effectArea[index].length = size;
	effectArea[index].pointlist = new DPoint[size];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(effectArea[index].pointlist[i].x, line[i]["x"].asInt());
		FUZHIINT(effectArea[index].pointlist[i].y, line[i]["y"].asInt());
	}
	if (size == 1)
	{
		if (effectArea[index].pointlist[0].x == 996)//蛇矛
		{
			effectArea[index].length = -4;
		}
		if (effectArea[index].pointlist[0].x == 997)//爆炎
		{
			effectArea[index].length = -3;
		}
		if (effectArea[index].pointlist[0].x == 998)//全屏
		{
			effectArea[index].length = -2;
		}
		if (effectArea[index].pointlist[0].x == 993)//穿透两个
		{
			effectArea[index].length = -6;
		}
        if (effectArea[index].pointlist[0].x == 992)//穿透四个
        {
            effectArea[index].length = -8;
        }
		if (effectArea[index].pointlist[0].x == 800)//穿透前方水平线3个
		{
			effectArea[index].length = -10;
		}
		if (effectArea[index].pointlist[0].x == 801)//穿透前方凸四人个
		{
			effectArea[index].length = -11;
		}
		if (effectArea[index].pointlist[0].x == 802)//穿透前方垂直线3人个
		{
			effectArea[index].length = -12;
		}
		if (effectArea[index].pointlist[0].x == 803)//刀轮攻击沿途两侧敌人
		{
			effectArea[index].length = -13;
		}
	}
}

void ChessData::getDicMpSe()
{
	JSON_READ_HEAD("json/dic_mp_se.json");
	int mp_id;
	char name[100];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(mp_id, line[i]["atk_id"].asInt());
		FUZHISTRING(name, line[i]["name"].asCString());
		getMpSe(line[i]["json"].asCString(), i);
		FUZHISTRING(effectArea[i].cname, line[i]["cname"].asCString());
	}
	JSON_READ_END;
}


void ChessData::getDicTurn()
{
   	JSON_READ_HEAD("json/dic_turn.json");
	int hid, imgid, turn;
	actor_turn_num = actor_num;
	actor_turn = new ActorTurn[actor_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(hid, line[i]["hid"].asInt());
		FUZHIINT(imgid, line[i]["imgid"].asInt());
		FUZHIINT(turn, line[i]["turn"].asInt());
		actor_turn[hid].hid = hid;
		actor_turn[hid].turn = turn;
		actors[hid].image_fight_id = imgid;
	}
	JSON_READ_END;
}

void ChessData::getTips()
{
	JSON_READ_HEAD("json/tips.json");
	hint_num = size;
	scene_hint = new char*[size];
	for (int i = 0; i < size; i++)
	{
		const char* hint = line[i]["content"].asCString();
		scene_hint[i] = new char[strlen(hint) + 1];
		strcpy(scene_hint[i], hint);
	}
	JSON_READ_END;
}

//i ：actors下标
//type : 1.人物特技（seid）
void ChessData::getherodata(const char* json_test, int i, int type)
{
	int next_record = 0;
	int test_len = strlen(json_test);
	int j = 0, k = 0, l = 0, m = 0;
	while (j < test_len)
	{
		next_record = StringToInt((char*)json_test + j);
		j++;
		j += GetNumDigit(next_record);
		m++;
	}
	switch (type){
	case 1:
		FUZHIINT(actors[i].seid_num, m);
		actors[i].seid = new unsigned short[m];
		break;
	}

	while (k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		switch (type){
		case 1:
			FUZHIINT(actors[i].seid[l++], next_record);
			break;
		}

	}
}

void ChessData::getBustHImg()
{
	JSON_READ_HEAD("json/dic_h_img.json");
	bustHImg_num = size;
	bustHImg = new BustHImg[size];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(bustHImg[i].face_id, line[i]["face_id"].asInt());
		FUZHIINT(bustHImg[i].src_x, line[i]["x"].asInt());
		FUZHIINT(bustHImg[i].src_y, line[i]["y"].asInt());
	}
	JSON_READ_END;
}

void ChessData::getDicSuitEffect()
{
	return;//TODO
	JSON_READ_HEAD("json/dic_suiteffect.json");
	suiteffect_num = size;
	suiteffect = new SuitEffect[size];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(suiteffect[i].seid, line[i]["seid"].asInt());
		FUZHIINT(suiteffect[i].seid_num, line[i]["seid_num"].asInt());
		suiteffect[i].effect_seid_num = getjson_stringeffect(&(suiteffect[i].effect_seid), line[i]["effect_seid"].asCString());
	}
	JSON_READ_END;
}
void ChessData::getDicSuitInfo()
{
	return;//TODO
	JSON_READ_HEAD("json/dic_suitinfo.json");
	suitinfo_num = size;
	suitinfo = new SuitInfo[size];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(suitinfo[i].suitinfo_id, line[i]["suit_id"].asInt());
		FUZHIINT(suitinfo[i].suit_image_id, line[i]["image_id"].asInt());
		FUZHIINT(suitinfo[i].suit_gold_id, line[i]["gold_id"].asInt());
		FUZHISTRING(suitinfo[i].suit_name, line[i]["name"].asCString());
		FUZHISTRING(suitinfo[i].suit_info, line[i]["info"].asCString());
		suitinfo[i].suitarr_num = getjson_stringeffect(&(suitinfo[i].suitarr), line[i]["suit"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getDicHero()
{
   	JSON_READ_HEAD("json/dic_hero.json");
	actor_num = size;
	actors = new ActorAttr[actor_num];
	killword = new int[actor_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(actors[i].actor_id, line[i]["hid"].asInt());
		FUZHIINT2(actors[i].face_id, line[i]["face_id"].asInt());
		FUZHIINT2(actors[i].occupation_id, line[i]["jobid"].asInt());
		FUZHIINT2(killword[i], line[i]["killid"].asInt());
		FUZHIINT(actors[i].power, line[i]["atk"].asInt());
		FUZHIINT(actors[i].defence, line[i]["def"].asInt());
		FUZHIINT(actors[i].intellegence, line[i]["ints"].asInt());
		FUZHIINT(actors[i].explosion, line[i]["burst"].asInt());
		FUZHIINT(actors[i].fortune, line[i]["fortune"].asInt());
		FUZHIINT(actors[i].level, line[i]["level"].asInt());
		FUZHIINT(actors[i].hp, line[i]["hp"].asInt());
		FUZHIINT(actors[i].mp, line[i]["mp"].asInt());
		FUZHIINT(actors[i].skill, line[i]["skill"].asInt());
		FUZHIINT(actors[i].actor_quality, line[i]["quality"].asInt());
		FUZHIINT(actors[i].qid, line[i]["qid"].asInt());
		FUZHIINT(actors[i].job_img, line[i]["job_img"].asInt());
		FUZHIINT(actors[i].iGender, line[i]["gender"].asInt());
		FUZHIINT(actors[i].iHeroGrade, line[i]["hero_grade"].asInt());
		FUZHISTRING(actors[i].actor_name, line[i]["name"].asCString());
		FUZHISTRING(actors[i].actor_info, line[i]["info"].asCString());
		FUZHISTRING(actors[i].rouse_word, line[i]["kill"].asCString());
		FUZHISTRING(actors[i].retreat_word, line[i]["retreat"].asCString());
		FUZHIINT(actors[i].iRecruit, line[i]["recruit"].asInt());
		if (!line[i]["border_color"].isNull()) {
			//添加武将品阶字段
			FUZHIINT(actors[i].actor_grade, line[i]["border_color"].asInt());
		}
		getherodata(line[i]["seid"].asCString(), i, 1);
		actors[i].image_fight_id = 0;

		if (actors[i].iRecruit)
		{
			history->setRecruitTotle(history->getRecruitTotle() + 1);
		}
	}
	JSON_READ_END;
}

void ChessData::saveSettingFlag()
{
	JSON_WRITE_HEAD;

	FUZHI1INT(history->setting_flag[0], line["setting_flag0"]);
	FUZHI1INT(history->setting_flag[1], line["setting_flag1"]);
	FUZHI1INT(history->setting_flag[2], line["setting_flag2"]);
	FUZHI1INT(history->setting_flag[3], line["setting_flag3"]);
	FUZHI1INT(history->setting_flag[4], line["setting_flag4"]);
	FUZHI1INT(history->setting_flag[5], line["setting_flag5"]);
	FUZHI1INT(history->setting_flag[6], line["setting_flag6"]);
	FUZHI1INT(history->setting_flag[7], line["setting_flag7"]);
	FUZHI1INT(history->setting_flag[8], line["setting_flag8"]);

	root.append(line);

	std::string out = fast_writer.write(root);
	ChessWriteFile(FILE_SETTING_FLAG, (char*)out.c_str(), out.size());

}
void ChessData::loadSettingFlag()
{
	char* sData = ChessReadFile(FILE_SETTING_FLAG, 1);

	if (sData == NULL)
	{
		return ;
	}

	Json::Reader reader;
	Json::Value line;
	if (!reader.parse(sData, line, false))
	{
		return ;
	}
	int size = line.size();
	int setting = 0;
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(history->setting_flag[0], line[i]["setting_flag0"].asInt());
		FUZHIINT(history->setting_flag[1], line[i]["setting_flag1"].asInt());
		FUZHIINT(history->setting_flag[2], line[i]["setting_flag2"].asInt());
		FUZHIINT(history->setting_flag[3], line[i]["setting_flag3"].asInt());
		FUZHIINT(history->setting_flag[4], line[i]["setting_flag4"].asInt());
		FUZHIINT(history->setting_flag[5], line[i]["setting_flag5"].asInt());
		FUZHIINT(history->setting_flag[6], line[i]["setting_flag6"].asInt());
		FUZHIINT(history->setting_flag[7], line[i]["setting_flag7"].asInt());
	}
	ChessFree(sData);
	return ;
}

//i ：occupations下标
//type : 1.兵种特技（seid）
void ChessData::getjobdata(const char* json_test, int i, int type)
{
	int next_record = 0;
	int test_len = strlen(json_test);
	int j = 0, k = 0, l = 0, m = 0;
	while (j < test_len)
	{
		next_record = StringToInt((char*)json_test + j);
		j++;
		j += GetNumDigit(next_record);
		m++;
	}
	switch (type){
	case 1:
		FUZHIINT(occupations[i].seid_num, m);
		occupations[i].seid = new unsigned short[m];
		break;
	}

	while (k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		switch (type){
		case 1:
			FUZHIINT(occupations[i].seid[l++], next_record);
			break;
		}

	}
	occupations[i].attack_special_effect = occupations[i].seid[0];//测试用
}



void ChessData::getDicJob()
{
	JSON_READ_HEAD("json/dic_job.json");
	occupation_num = size;
	occupations = new OccupationAttr[occupation_num];
	occupation_image_id = new unsigned short[occupation_num];
	occupation_special_ability = new int[occupation_num];
	memset(occupation_special_ability, 0, occupation_num*sizeof(int));
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(occupations[i].occupation_id, line[i]["jobid"].asInt());
		FUZHIINT(occupations[i].base_speed, line[i]["spe"].asInt());
		FUZHIINT2(occupations[i].attack_type_id, line[i]["atkid"].asInt());
		FUZHIINT(occupations[i].is_robot, line[i]["is_robot"].asInt());
		FUZHIINT(occupations[i].moveId, line[i]["move"].asInt());
		FUZHIINT(occupations[i].musicId, line[i]["music"].asInt());
		FUZHISTRING(occupations[i].occupation_name, line[i]["name"].asCString());
		FUZHISTRING(occupations[i].occupation_info, line[i]["info"].asCString());
		FUZHIINT(occupation_image_id[i], line[i]["imgid"].asInt());

		getjobdata(line[i]["seid"].asCString(), i, 1);
		//		FUZHIINT(occupations[i].special_power, line[i]["special_power"].asInt());
		FUZHIINT(occupations[i].changejob_imgid, line[i]["changejob_imgid"].asInt());
		FUZHIINT(occupations[i].effect, line[i]["effect"].asCString());
	}
	JSON_READ_END;
}

// i:Lands下标
// type :1.可用效果类型 （0：火类，1：水类，2：地类，3：风类，&：没有任何效果）注意 0 是有用的\2.seid (type)
void ChessData::getLandsdata(const char* json_test, int i, int type)
{
	int next_record = 0;
	int test_len = strlen(json_test);
	int j = 0, k = 0, l = 0, m = 0;
	while (j < test_len)
	{
		next_record = StringToInt((char*)json_test + j);
		j++;
		j += GetNumDigit(next_record);
		m++;
	}
	switch (type){
	case 2:
		FUZHIINT(lands[i].seid_num, m);
		lands[i].seid = new unsigned short[m];
		break;
	}

	while (k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		switch (type){
		case 1:
			lands[i].atk_up[900 + l] = next_record;
			l++;
			break;
		case 2:
			FUZHIINT(lands[i].seid[l++], next_record);
			break;
		}

	}
}

void ChessData::getDicLands()
{
	JSON_READ_HEAD("json/map_terrain.json");

	land_num = size;
	lands = new LandformAttr[land_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(lands[i].landform_id, line[i]["mapid"].asInt());
		FUZHISTRING(lands[i].landform_name, line[i]["name"].asCString());

		getLandsdata(line[i]["atk_up"].asCString(), i, 1);
		FUZHIINT(lands[i].music, line[i]["music"].asInt());
		FUZHIINT(lands[i].type, line[i]["type"].asInt());
		getLandsdata(line[i]["seid"].asCString(), i, 2);

	}
	JSON_READ_END;
}

void ChessData::getDicItem()
{
	JSON_READ_HEAD("json/dic_item.json");
	for(int i = 0; i < size; i++)
	{
		GoodsItem* ptrGoods = new GoodsItem();
		ptrGoods->iGoodsId = line[i]["good_id"].asInt();
		ptrGoods->strName = line[i]["name"].asString();
		ptrGoods->iImgId = line[i]["image_id"].asInt();
		ptrGoods->iItId = line[i]["itid"].asInt();
		ptrGoods->iTypeId = line[i]["typeid"].asInt();
		ptrGoods->mSeid = getSeidData(line[i]["seid"].asCString());
		ptrGoods->iSeSeid = line[i]["se_seid"].asInt();
		ptrGoods->iPower = line[i]["power"].asInt();
		ptrGoods->iPrice = line[i]["price"].asInt();
		ptrGoods->iUpVal = line[i]["up_val"].asInt();
		ptrGoods->iUeid = line[i]["ueid"].asInt();
		ptrGoods->iIsOnly = line[i]["isOnlyOne"].asInt();
		ptrGoods->strUeid = line[i]["ueid_txt"].asString();
		ptrGoods->strInfo = line[i]["info"].asString();
		ptrGoods->mSuitList = getMapData(line[i]["suit_id"].asCString());
		ptrGoods->mHid = getMapData(line[i]["hid"].asCString());
		ptrGoods->mNoHid = getMapData(line[i]["no_hid"].asCString());
		ptrGoods->mFmItid = getMapData(line[i]["itid_xz"].asCString());
		m_mGoodsItem[ptrGoods->iGoodsId] = ptrGoods;
		if (ptrGoods->mFmItid.begin()->first) //记录附魔宝石GoodsID
		{
			m_mFmStoneId[ptrGoods->iGoodsId] = 1;
		}
	}
	InitGoodAttrInfo();
	JSON_READ_END;
}

void ChessData::getTrGamePay()
{
	trGamePay = new TrGamePay();
	trGamePay->loadDefault();//新版本全部从本地读取
}

void ChessData::getTrAccountBuy()
{
	trAccountBuy = new TrAccountBuy();
	trAccountBuy->LoadPayRecord();
	trAccountBuy->LoadExchangeGift();
	trAccountBuy->LoadConfig();
}

void ChessData::getDicSkill()
{
	JSON_READ_HEAD("json/dic_skill.json");
	skill_num = size;
	skills = new SkillAttr[skill_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(skills[i].skill_id, line[i]["skid"].asInt());
		FUZHIINT2(skills[i].image_id, line[i]["imgid"].asInt());
		FUZHIINT2(skills[i].effect_region_type, line[i]["mpid"].asInt());
		FUZHIINT2(skills[i].use_region, line[i]["atkid"].asInt());
		FUZHIINT(skills[i].boss_limit, line[i]["boss_limit"].asInt());
		FUZHIINT(skills[i].skill_object, line[i]["object"].asInt());
		FUZHIINT(skills[i].mp_consume, line[i]["mp_consume"].asInt());
		FUZHIINT(skills[i].hit_per, line[i]["hit_per"].asInt());
		FUZHIINT(skills[i].buff_hit_per, line[i]["buff_hit_per"].asInt());
		FUZHIINT(skills[i].hurt_num, line[i]["hurt_num"].asInt());
		FUZHIINT(skills[i].power, line[i]["power"].asInt());
		FUZHIINT(skills[i].magic_type, line[i]["skill_type"].asInt());
		FUZHIINT(skills[i].full_anim, line[i]["full_anim"].asInt());
		FUZHIINT(skills[i].seid, line[i]["seid"].asInt());
		FUZHIINT(skills[i].prop_num, line[i]["prop_num"].asInt());
		FUZHIINT(skills[i].prop_per, line[i]["prop_per"].asInt());
		FUZHIINT(skills[i].seid_round, line[i]["seid_round"].asInt());
		FUZHIINT(skills[i].skill_mp3, line[i]["mp3"].asInt());
		FUZHIINT(skills[i].effect, line[i]["effect"].asCString());
		FUZHIINT(skills[i].delay1, line[i]["delay1"].asInt());
		FUZHIINT(skills[i].delay2, line[i]["delay2"].asInt());
		FUZHIINT(skills[i].type, line[i]["type"].asInt());
		FUZHIINT(skills[i].iNoCopy, line[i]["no_copy"].asInt());
		FUZHIINT(skills[i].explain, line[i]["explain"].asCString());
		FUZHIINT(skills[i].iNoCost, line[i]["no_cost"].asInt());
		FUZHIINT(skills[i].iTimes, line[i]["times"].asInt());
		FUZHIINT(skills[i].iActorUseNum, line[i]["hero_times"].asInt());
		FUZHIINT(skills[i].iControlAtkid, line[i]["control_atkid"].asInt());
		FUZHISTRING(skills[i].skill_name, line[i]["name"].asCString());
		FUZHISTRING(skills[i].skill_info, line[i]["info"].asCString());//新json中没有该字段
		FUZHISTRING(skills[i].skill_effect, line[i]["eff_txt"].asCString());
		getjson_stringeffect(&skills[i].animation, line[i]["animation"].asCString());
	}
	JSON_READ_END;
}

void ChessData::initIOSPrice()
{
	return;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
	extern void iosRefreshProductList(DList<DString>* productIDs);
	DList<DString> productIDs(true);
	TrPayInfo* payInfo = trData->trGamePay->getPayInfo();
	for (int i = 0; i < payInfo->getChargeSize(); i++)
	{
		TrPayInfo::TrChargeItem* item = payInfo->getChargeItem(i);
		TrPayInfo::TrChargeChannel* channel = item->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY);
		if (channel != NULL)
		{
			productIDs.addElement(new DString(channel->iosProductID));

			printf("iosProductID: %s\n", channel->iosProductID->getChar());
		}
	}

	iosRefreshProductList(&productIDs);
#endif
}

void initIOSBuyItems()//初始化ios购买列表
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS && !defined(HMW_PAY))
	extern void iosInitItems(DList<DString>* productIDs);
	DList<DString> productIDs(true);
	TrPayInfo* payInfo = trData->trGamePay->getPayInfo();
	for (int i = 0; i < payInfo->getChargeSize(); i++)
	{
		TrPayInfo::TrChargeItem* item = payInfo->getChargeItem(i);
		TrPayInfo::TrChargeChannel* channel = item->findChannelItemById(TrPayInfo::CHANNEL_IOSPAY);
		if (channel != NULL)
		{
			productIDs.addElement(new DString(channel->iosProductID));
		}
	}
	iosInitItems(&productIDs);

	extern void iosRefreshProductList(DList<DString>* productIDs);
	iosRefreshProductList(&productIDs);
#endif
}

bool ChessData::canRangeEffect(int areaid, int x1, int y1, int x2, int y2) //位于x1, y1处的人，是否可以攻击位于x2, y2处的人
{
	if(this->hitArea[areaid].length == 0)
		return false;
	if(this->hitArea[areaid].pointlist[0].x == 999)
		return true;
	for(int i = 0; i < hitArea[areaid].length; i++)
	{
		if(x1 + hitArea[areaid].pointlist[i].x == x2 && y1 + hitArea[areaid].pointlist[i].y == y2)
			return true;
	}
	return false;
}

void ChessData::ParseData()
{
	getBustHImg();
	getdicseid();//特技
	getItemType();//道具普通效果
	getTrAccountBuy();//已购买计费信息
	getTips();
	getDicHero(); // 武将
	getDicJobClass();
	getDicJob(); //兵种
	getDicItem();
	getTrGamePay();//计费信息
	getDicSkill(); //技能
	getDicMpSe(); //影响范围
	getDicAtkSe(); //攻击范围
	getDicKillTxt();
	getDicJobPk();
	getDicJobItem();
	getDicLands();
	getDicJobTerrain();
	getDicJobWalk();
	getDicTurn();//特殊造型
	//getDicTurn2();//正式版特殊造型
	initIOSBuyItems();
	initSkillLandInfo();
	getDicSuitEffect();
	getDicSuitInfo();
	getDicStrengthenConsume();
	getDicMaterialExchange();
	getDicFits();
	getDicItemClass();
	getDicTreasureInfo();
	getAnimationInfo();
	getNetJobInfo();
	getNetMapWar();
	getNetMapName();
	getNetWarCondition();
	getNetHeroName();
	getNetHeroNameForbid();
	getNetExternalEquip();
	getNetHeroList();
	getNetHeroQuality();
	getNetHeroSkill();
	getNetHeroExp();
	getNetHeroGrown();
	getNetShopInfo();
	getMysNetShop();
	getNetSkillUpgrade();
	getNetSkillGold();
	getNetWarGoods();
	getNetGoldExchange();
	getNetMapChest();
	getNetPubInfo();
	getNetJadeCharge();
	getNetHerozWar();
	getNetVipInfo();
	getNetHerozPrice();
	getNetFbWar();
	getMapWeather();
	getNetPrompt();
    getNetMapCity();
	getNetRecastPrice();
	getNetJadeOfVitality();
	getNetShopRefresh();
	getNetMapClear();
	getVipIncome();
	getNetStory();
	getNetStoryWar();
	getNetStoryCondition();
	getNetStoryGoods();	
	getNetBuff();
	getNetWarHero();
    getNetMapPremise();
	getNetErrorMSG();
	getNetSuit();
	getNetStalkerWar();
	getNetHeroDress();
	getNetGlcWar();
	getNetStalkerReward();
	getNetMapWarJy();
	getNetAutomaticEquip();
	getNetEliteEquip();
	getNetCloudXy();
	getNetHeroNoDress();
	getNetXmtWar();
	getNetActiveTask();
	getNetWarGhFb();
	getNetZmzyWar();
	getNetZmzyMap();
	getNetRatioBuff();

	parseJsonData();
}

int ChessData::ChessInitData()
{
	reloadData();
	modConst->loadConfig();
	ParseData();
	modConst->InitConfig();
	history->loadInitialData();
	return 1;
}

void ChessData::initSkillLandInfo()
{
	skill_land_info = new UUCHAR*[4];
	for(int i = 0; i < 4; i++)
	{
		skill_land_info[i] = new UUCHAR[land_num];
		memset(skill_land_info[i], 0, land_num);
	}
}

void ChessData::InitGoodAttrInfo()
{
	map<int, GoodsItem*>::iterator itGoods;
	for(itGoods = m_mGoodsItem.begin(); itGoods != m_mGoodsItem.end(); itGoods++)
	{
		GoodsItem* ptrGoods = itGoods->second;
		ptrGoods->InitGoodsType();
		if(ptrGoods->isGeneral())
		{
			if(ptrGoods->m_iGoodType == GOOD_TYPE_WEAPON)
			{
				iWeaponNum++;
			}
			else if(ptrGoods->m_iGoodType == GOOD_TYPE_COAT)
			{
				iCoatNum++;
			}
		}
	}

	ptrDefWeapon = new int[iWeaponNum];
	memset(ptrDefWeapon, 0, iWeaponNum*sizeof(ptrDefWeapon[0]));
	ptrDefCoat = new int[iCoatNum];
	memset(ptrDefCoat, 0, iCoatNum*sizeof(ptrDefCoat[0]));

	int iWeapon = 0, iCoat = 0;
	for(itGoods = m_mGoodsItem.begin(); itGoods != m_mGoodsItem.end(); itGoods++)
	{
		GoodsItem* ptrGoods = itGoods->second;
		if(ptrGoods->isGeneral())
		{
			if(ptrGoods->m_iGoodType == GOOD_TYPE_WEAPON)
			{
				ptrDefWeapon[iWeapon++] = ptrGoods->iGoodsId;
			}
			else if(ptrGoods->m_iGoodType == GOOD_TYPE_COAT)
			{
				ptrDefCoat[iCoat++] = ptrGoods->iGoodsId;
			}
		}

		if (ptrGoods->m_iGoodType == GOOD_TYPE_FASHING|| ptrGoods->m_iGoodType == GOOD_TYPE_WAP_FASHING)
		{
			mFashionList[ptrGoods->iGoodsId] = 1;
		}
	}
}

void ChessData::getDicStrengthenConsume()
{
	JSON_READ_HEAD("json/dic_strengthen_consume.json");

	strengthen_consume_num = size;
	strengthen_consume = new StrengthenConsume[strengthen_consume_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(strengthen_consume[i].level, line[i]["level"].asInt());
		FUZHIINT(strengthen_consume[i].money, line[i]["money"].asInt());
		FUZHIINT(strengthen_consume[i].special_goods, line[i]["special_goods"].asInt());
		FUZHIINT(strengthen_consume[i].quality, line[i]["quality"].asInt());
		FUZHIINT(strengthen_consume[i].gem_num, line[i]["gem_num"].asInt());
		FUZHIINT(strengthen_consume[i].success_rate, line[i]["success_rate"].asInt());
		FUZHIINT(strengthen_consume[i].atk_num, line[i]["atk_num"].asInt());
		FUZHIINT(strengthen_consume[i].def_num, line[i]["def_num"].asInt());
		FUZHIINT(strengthen_consume[i].iTotalMoney, line[i]["money_sum"].asInt());
		FUZHIINT(strengthen_consume[i].iTotalGem, line[i]["gem_sum"].asInt());
	}
	JSON_READ_END;
}
void ChessData::getDicMaterialExchange()
{
	return;//TODO
	JSON_READ_HEAD("json/dic_material_exchange.json");

	material_exchange_num = size;
	material_exchange = new MaterialExchange[material_exchange_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(material_exchange[i].goods_id, line[i]["goods_id"].asInt());
		FUZHIINT(material_exchange[i].gem_num, line[i]["gem_num"].asInt());
		FUZHISTRING(material_exchange[i].name, line[i]["name"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getNetMapWar()
{
	JSON_READ_HEAD("json/net_map_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_SIXWAR;
		ptrInfo->iModel = NORMAL_MODEL;
		FUZHIINT(ptrInfo->iId, line[i]["id"].asInt());
		FUZHIINT(ptrInfo->iMapIndex, line[i]["mapid"].asInt());
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iImgId, line[i]["img"].asInt());
		FUZHIINT(ptrInfo->iPosX, line[i]["x"].asInt());
		FUZHIINT(ptrInfo->iPosY, line[i]["y"].asInt());
		FUZHIINT(ptrInfo->iLevel, line[i]["levels"].asInt());
		FUZHIINT(ptrInfo->iRound, line[i]["round"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		ptrInfo->mBossId = getSeidData(line[i]["boss"].asCString());
		ptrInfo->mStarLimit = getSeidData(line[i]["star_limit"].asCString());
		FUZHIINT(ptrInfo->iGold, line[i]["gold"].asInt());
		FUZHIINT(ptrInfo->iExp, line[i]["hExp"].asInt());
		FUZHISTRINGEX(ptrInfo->strInfo, line[i]["describe"]);
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		FUZHIINT(ptrInfo->iMaxTimes, line[i]["number_battle"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		map<int, string> mstrEnemy = getStringMap(line[i]["enemy"].asCString(), '|');
		for(UInt k = 0; k < mstrEnemy.size(); k++)
		{
			ptrInfo->mEnemyMap[k] = getSeidData(mstrEnemy[k].c_str());
		}
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = i;
	}
	JSON_READ_END;
}

void ChessData::getNetMapWarJy()
{
	JSON_READ_HEAD("json/net_map_war_jy.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_SIXWAR;
		ptrInfo->iModel = DISSICULT_MODEL;
		FUZHIINT(ptrInfo->iId, line[i]["id"].asInt());
		FUZHIINT(ptrInfo->iMapIndex, line[i]["mapid"].asInt());
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iImgId, line[i]["img"].asInt());
		FUZHIINT(ptrInfo->iPosX, line[i]["x"].asInt());
		FUZHIINT(ptrInfo->iPosY, line[i]["y"].asInt());
		FUZHIINT(ptrInfo->iLevel, line[i]["levels"].asInt());
		FUZHIINT(ptrInfo->iRound, line[i]["round"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		ptrInfo->mBossId = getSeidData(line[i]["boss"].asCString());
		ptrInfo->mStarLimit = getSeidData(line[i]["star_limit"].asCString());
		FUZHIINT(ptrInfo->iGold, line[i]["gold"].asInt());
		FUZHIINT(ptrInfo->iExp, line[i]["hExp"].asInt());
		FUZHISTRINGEX(ptrInfo->strInfo, line[i]["describe"]);
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		FUZHIINT(ptrInfo->iMaxTimes, line[i]["number_battle"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		map<int, string> mstrEnemy = getStringMap(line[i]["enemy"].asCString(), '|');
		for (UInt k = 0; k < mstrEnemy.size(); k++)
		{
			ptrInfo->mEnemyMap[k] = getSeidData(mstrEnemy[k].c_str());
		}
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = i;
	}
	JSON_READ_END;
}

void ChessData::getNetMapName()
{
	JSON_READ_HEAD("json/net_map_name.json");
	for (int i = 0; i < size; i++)
	{
		NetMapName* ptrNMN = new NetMapName();
		FUZHIINT(ptrNMN->mapid, line[i]["mapid"].asInt());
		FUZHISTRING(ptrNMN->name, line[i]["name"].asCString());
		FUZHIINT(ptrNMN->mapBgId, line[i]["map"].asInt());
		FUZHIINT(ptrNMN->mModel, line[i]["model"].asInt());
		net_map_name[ptrNMN->mapid] = ptrNMN;
	}

	JSON_READ_END;
}

void ChessData::getNetWarCondition()
{
   	JSON_READ_HEAD("json/net_war_conditions.json");
	win_condition_num = size;
	win_condition = new WinCondition[win_condition_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(win_condition[i].warid, line[i]["warid"].asInt());
		FUZHIINT(win_condition[i].c_type, line[i]["c_type"].asInt());
		win_condition[i].data_num = getjson_stringeffect(&win_condition[i].data, line[i]["data"].asCString());
		win_condition[i].describe = line[i]["describe"].asCString();
	}

	JSON_READ_END;
}

void ChessData::getNetExternalEquip()
{
	JSON_READ_HEAD("json/net_external_equip.json");

	for (int i = 0; i < size; i++)
	{
		NetExternalEquipPtr ptrNetExtEqu = new NetExternalEquip();
		FUZHIINT(ptrNetExtEqu->equipid, line[i]["equipid"].asInt());
		ArrGetJsonInfo(ptrNetExtEqu->weapon, line[i]["weapon_id"].asCString(),5);
		ArrGetJsonInfo(ptrNetExtEqu->armour, line[i]["armour_id"].asCString(), 5);
		ArrGetJsonInfo(ptrNetExtEqu->assist, line[i]["assist_id"].asCString(), 5);
		FUZHIINT(ptrNetExtEqu->skill_lv, line[i]["skill_lv"].asInt());
		FUZHIINT(ptrNetExtEqu->quality, line[i]["quality"].asInt());
		if (!line[i]["boss"].isNull()) 
		{
			ptrNetExtEqu->boss = getSeidData(line[i]["boss"].asCString());
		}
		trData->net_external_equip[ptrNetExtEqu->equipid] = ptrNetExtEqu;
	}

	JSON_READ_END;
}

void ChessData::getNetAutomaticEquip()
{
	JSON_READ_HEAD("json/net_automatic_equip.json");

	int iArrSize = 0;
	for (int i = 0; i < size; i++)
	{
		NetExternalEquipPtr ptrNetExtEqu = new NetExternalEquip();
		FUZHIINT(ptrNetExtEqu->jobid, line[i]["jobid"].asInt());
		ArrGetJsonInfo(ptrNetExtEqu->weapon, line[i]["weapon_id"].asCString(), 5);
		ArrGetJsonInfo(ptrNetExtEqu->armour, line[i]["armour_id"].asCString(), 5);
		ArrGetJsonInfo(ptrNetExtEqu->assist, line[i]["assist_id"].asCString(), 5);
		FUZHIINT(ptrNetExtEqu->skill_lv, line[i]["skill_lv"].asInt());
		ptrNetExtEqu->level_area = getSeidData(line[i]["level_area"].asCString());
		
		if (trData->net_automatic_equip.count(ptrNetExtEqu->jobid))
			iArrSize = trData->net_automatic_equip[ptrNetExtEqu->jobid].size();
		else
			iArrSize = 0;
		trData->net_automatic_equip[ptrNetExtEqu->jobid][iArrSize] = ptrNetExtEqu;
	}

	JSON_READ_END;
}

void ChessData::getNetEliteEquip()
{
	JSON_READ_HEAD("json/net_elite_equip.json");

	int iArrSize = 0;
	for (int i = 0; i < size; i++)
	{
		NetExternalEquipPtr ptrNetExtEqu = new NetExternalEquip();
		FUZHIINT(ptrNetExtEqu->equipid, line[i]["equipid"].asInt());
		ArrGetJsonInfo(ptrNetExtEqu->weapon, line[i]["weapon_id"].asCString(), 5);
		ArrGetJsonInfo(ptrNetExtEqu->armour, line[i]["armour_id"].asCString(), 5);
		ArrGetJsonInfo(ptrNetExtEqu->assist, line[i]["assist_id"].asCString(), 5);
 		FUZHIINT(ptrNetExtEqu->skill_lv, line[i]["skill_lv"].asInt());
		ptrNetExtEqu->level_area = getSeidData(line[i]["level_area"].asCString());
		if (!line[i]["boss"].isNull())
		{
			ptrNetExtEqu->boss = getSeidData(line[i]["boss"].asCString());
		}
		if (trData->net_elite_equip.count(ptrNetExtEqu->equipid))
			iArrSize = trData->net_elite_equip[ptrNetExtEqu->equipid].size();
		else
			iArrSize = 0;
		trData->net_elite_equip[ptrNetExtEqu->equipid][iArrSize] = ptrNetExtEqu;
	}

	JSON_READ_END;
}

void ChessData::getNetHeroList()
{
	JSON_READ_HEAD("json/net_hero_list.json");

	for (int i = 0; i < size; i++)
	{
		NetHeroListPtr ptrNetHeroL = new NetHeroList();
		FUZHIINT(ptrNetHeroL->hid, line[i]["hid"].asInt());
		FUZHIINT(ptrNetHeroL->good_id, line[i]["good_id"].asInt());
		trData->net_hero_list[ptrNetHeroL->hid] = ptrNetHeroL;
		getNetHeroListElem(ptrNetHeroL->hid, line[i]["obtain_method"].asCString());
	}

	JSON_READ_END;
}

void ChessData::getNetHeroListElem(int iIndex, const char* json_test)
{
	const char* point = 0, *lastPoint = 0;
	int add = 1;
	const char *buf = json_test;
	point = lastPoint = buf;
	int arr_index = 0;
	while (point < buf + strlen(buf))
	{
		point = strstr(lastPoint, "|");
		if (point == NULL)
		{
			point = buf + strlen(buf);
		}
		{
			int next_record = 0;
			int test_len = strlen(lastPoint) - strlen(point);
			int j = 0, k = 0, l = 0;
			while (j < test_len)
			{
				next_record = StringToInt((char*)lastPoint + j);
				j++;
				j += GetNumDigit(next_record);
				trData->net_hero_list[iIndex]->obtain_method[arr_index][l++] = next_record;
			}
		}
		arr_index++;
		lastPoint = point + add;
	}
}

void ChessData::getNetHeroQuality()
{
	JSON_READ_HEAD("json/net_hero_quality.json");

	for (int i = 0; i < size; i++)
	{
		NetHeroQualityPtr ptrNetHeroQ = new NetHeroQuality();
		FUZHIINT(ptrNetHeroQ->quality, line[i]["quality"].asInt());
		FUZHIINT(ptrNetHeroQ->call_soul, line[i]["call_soul"].asInt());
		FUZHIINT(ptrNetHeroQ->rank_soul, line[i]["rank_soul"].asInt());
		FUZHIINT(ptrNetHeroQ->gold, line[i]["gold"].asInt());
		trData->net_hero_quality[ptrNetHeroQ->quality] = ptrNetHeroQ;
	}

	JSON_READ_END;
}

void ChessData::getNetHeroSkill()
{
	JSON_READ_HEAD("json/net_hero_skill.json");

	for (int i = 0; i < size; i++)
	{
		NetHeroSkillPtr ptrNetHeroSkill = new NetHeroSkill();
		FUZHIINT(ptrNetHeroSkill->skillid, line[i]["skillid"].asInt());
		FUZHISTRING(ptrNetHeroSkill->info, line[i]["info"].asCString());
		trData->net_hero_skill[ptrNetHeroSkill->skillid] = ptrNetHeroSkill;
		getNetHeroSkillElem(ptrNetHeroSkill->skillid, line[i]["skid"].asCString(), line[i]["lv_up"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getNetHeroExp()
{
	JSON_READ_HEAD("json/net_hero_exp.json");
	int iLevel = 0, iExp = 0;
	for (int i = 0; i < size; i++)
	{
		NetHeroLvUpExpPtr ptrNHLUE = new NetHeroLvUpExp();
		FUZHIINT(iLevel, line[i]["lv"].asInt());
		FUZHIINT(ptrNHLUE->mExp, line[i]["exp"].asInt());
		net_hero_exp[iLevel] = ptrNHLUE;
	}

	int iTotalExp = 0;
	for (map<int, NetHeroLvUpExpPtr>::iterator it = net_hero_exp.begin(); it != net_hero_exp.end(); it++)
	{
		it->second->mTotalExp = iTotalExp;
		iTotalExp += it->second->mExp;
	}
	JSON_READ_END;
}

void ChessData::getNetHeroGrown()
{
 	JSON_READ_HEAD("json/net_hero_grown.json");
	int iHid = 0, iQuality = 0;
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(iHid, line[i]["hid"].asInt());
		FUZHIINT(iQuality, line[i]["quality"].asInt());
		GrownAbilityInfoPtr ptrGrown = new GrownAbilityInfo();
		FUZHIINT(ptrGrown->atk, line[i]["atk"].asDouble());
		FUZHIINT(ptrGrown->ints, line[i]["ints"].asDouble());
		FUZHIINT(ptrGrown->def, line[i]["def"].asDouble());
		FUZHIINT(ptrGrown->burst, line[i]["burst"].asDouble());
		FUZHIINT(ptrGrown->fortune, line[i]["fortune"].asDouble());
		FUZHIINT(ptrGrown->hp_up, line[i]["hp_up"].asDouble());
		FUZHIINT(ptrGrown->mp_up, line[i]["mp_up"].asDouble());
		net_hero_grown[iHid][iQuality] = ptrGrown;
	}
	JSON_READ_END;
}

void ChessData::getNetSkillUpgrade()
{
	JSON_READ_HEAD("json/net_skill_upgrade.json");
	for (int i = 0; i < size; i++)
	{
		NetSkillUpgradePtr ptrUpgrade = new NetSkillUpgrade();
		FUZHIINT(ptrUpgrade->skid, line[i]["skid"].asInt());
		FUZHIINT(ptrUpgrade->up_limit, line[i]["up_limit"].asInt());
		FUZHIINT(ptrUpgrade->up_hurt_num, line[i]["up_hurt_num"].asInt());
		FUZHIINT(ptrUpgrade->up_mp_consume, line[i]["up_mp_consume"].asDouble());
		FUZHIINT(ptrUpgrade->up_prop_num, line[i]["up_prop_num"].asDouble());
		FUZHIINT(ptrUpgrade->up_prop_per, line[i]["up_prop_per"].asDouble());
		net_skill_upgrade[ptrUpgrade->skid] = ptrUpgrade;
	}
	JSON_READ_END;
}

void ChessData::getNetSkillGold()
{
	JSON_READ_HEAD("json/net_skill_gold.json");
	int iSkill_lv = 0, iGold = 0;

	for (int i = 0; i < size; i++)
	{
		FUZHIINT(iSkill_lv, line[i]["skill_lv"].asInt());
		FUZHIINT(iGold, line[i]["gold"].asInt());
		net_skill_gold[iSkill_lv] = iGold;
	}

	JSON_READ_END;
}

void ChessData::getNetWarGoods()
{
	JSON_READ_HEAD("json/net_war_good.json");
	int iWarid = 0, iGoodsId = 0,iCount = 1;

	for (int i = 0; i < size; i++)
	{
		FUZHIINT(iWarid, line[i]["warid"].asInt());
		FUZHIINT(iGoodsId, line[i]["good_id"].asInt());
		FUZHIINT(iCount, line[i]["van"].asInt());
		net_war_goods[iWarid][iGoodsId] = iCount;
	}

	JSON_READ_END;
}

void ChessData::getNetMapChest()
{
	JSON_READ_HEAD("json/net_chest.json");
	int iMapid = 0, iStar = 0;
	for (int i = 0; i < size; i++)
	{
		NetMapChestPtr ptrNMC = new NetMapChest();
		FUZHIINT(ptrNMC->mapid, line[i]["mapid"].asInt());
		FUZHIINT(ptrNMC->star, line[i]["star"].asInt());
		net_map_chest[ptrNMC->mapid][ptrNMC->star] = ptrNMC;

		FUZHIINT(ptrNMC->gold, line[i]["gold"].asInt());
		FUZHIINT(ptrNMC->jade, line[i]["jade"].asInt());
		FUZHIINT(ptrNMC->improvedStone, line[i]["improvedStone"].asInt());
		FUZHIINT(ptrNMC->chest_img, line[i]["img"].asInt());
		getNetChestElem(ptrNMC->mapid, ptrNMC->star, line[i]["itemid"].asCString());
	}

	JSON_READ_END;
}

void ChessData::getNetHerozWar()
{
	JSON_READ_HEAD("json/net_heroz_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrNetWar = new NetWarInfo();
		ptrNetWar->iType = BATTLE_HEROIC;
		FUZHIINT(ptrNetWar->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNetWar->iOpenWarId, line[i]["open_gk"].asInt());
		FUZHIINT(ptrNetWar->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrNetWar->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrNetWar->iGold, line[i]["gold"].asInt());
		FUZHIINT(ptrNetWar->iExp, line[i]["hExp"].asInt());
		FUZHIINT(ptrNetWar->iEnemyLv, line[i]["enemy_lv"].asInt());
		FUZHIINT(ptrNetWar->iStore, line[i]["improvedStone"].asInt());
		FUZHIINT(ptrNetWar->strName, line[i]["name"].asCString());
		FUZHIINT(ptrNetWar->iMaxTimes, line[i]["number_battle"].asInt());
		ptrNetWar->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		FUZHIINTEX(ptrNetWar->iGkLv, line[i]["gk_lv"]);
		m_mNetWarInfo[ptrNetWar->iWarId] = ptrNetWar;
	}
	JSON_READ_END;
}

void ChessData::getNetHerozPrice()
{
	int iMaxCount = 0;
	JSON_READ_HEAD("json/net_heroz_price.json");
	for (int i = 0; i < size; i++)
	{
		int iId = 0, iJade = 0;
		FUZHIINT(iId, line[i]["id"].asInt());
		FUZHIINT(iJade, line[i]["jade"].asInt());
		net_heroz_price[iId] = iJade;
		if (iId>iMaxCount)
			iMaxCount = iId;
	}
	net_heroz_price[0] = iMaxCount;

	JSON_READ_END;
}

void ChessData::getMapWeather()
{
	JSON_READ_HEAD("json/map_weather.json");
	for (int i = 0; i < size; i++)
	{
		int iId = 0;
		FUZHIINT(iId, line[i]["id"].asInt());
		for (int j = 0; j < 5; j++)
		{
			FUZHIINT(skill_weather_addition[iId][j], line[i][IntToString(j+1)].asInt());
		}
	}

	JSON_READ_END;
}

void ChessData::getNetJadeCharge()
{
	JSON_READ_HEAD("json/net_jade_charge.json");
	for (int i = 0; i < size; i++)
	{
		NetJadeChargePtr ptrNJC = new NetJadeCharge();
		FUZHIINT(ptrNJC->mId, line[i]["id"].asInt());
		FUZHIINT(ptrNJC->mRmb, line[i]["rmb"].asInt());
		FUZHIINT(ptrNJC->mJade, line[i]["jade"].asInt());
		FUZHIINT(ptrNJC->mGiveJade, line[i]["give_jade"].asInt());
		FUZHIINT(ptrNJC->mFirstJade, line[i]["first_jade"].asInt());
		ptrNJC->mChargeId = line[i]["charge_id"].asCString();
		net_jade_charge[ptrNJC->mId] = ptrNJC;
	}

	JSON_READ_END;
}

void ChessData::getNetChestElem(int iMapId, int iStar, const char* json_item)
{
	const char* point = 0, *lastPoint = 0, *midpoint = 0;
	const char *buf = json_item;
	point = lastPoint = buf;
	while (point < buf + strlen(buf))
	{
		point = strstr(lastPoint, "|");
		if (point == NULL)
		{
			point = buf + strlen(buf);
		}
		{
			midpoint = strstr(lastPoint, "&");
			if (midpoint&&midpoint < point)
			{
				int iItemid = StringToInt((char*)lastPoint);
				int iNum = StringToInt((char*)midpoint+1);
				net_map_chest[iMapId][iStar]->itemid[iItemid] = iNum;
			}
		}
		lastPoint = point + 1;
	}
}

void ChessData::getNetStory()
{
	JSON_READ_HEAD("json/net_story.json");
	for (int i = 0; i < size; i++)
	{
		NetStory* ptrNS = new NetStory();
		FUZHIINT(ptrNS->mapid, line[i]["mapid"].asInt());
		FUZHIINT(ptrNS->name, line[i]["name"].asCString());
		FUZHIINT(ptrNS->story_type, line[i]["story_type"].asInt());
		FUZHIINT(ptrNS->jade, line[i]["jade"].asInt());
		FUZHIINT(ptrNS->author, line[i]["author"].asInt());
		FUZHIINT(ptrNS->jadeSeal, line[i]["jadeSeal"].asInt());
		FUZHIINT(ptrNS->describe, line[i]["describe"].asCString());
		FUZHIINT(ptrNS->map_img, line[i]["map_img"].asInt());
		FUZHIINT(ptrNS->condition, line[i]["condition"].asInt());
		ptrNS->hid = getSeidData(line[i]["hid"].asCString());
		ptrNS->good_id = getSeidData(line[i]["good_id"].asCString());
		net_story[ptrNS->mapid] = ptrNS;
		getNetStoryTxtColor(ptrNS->mapid,line[i]["txt_color"].asCString());
	}

	JSON_READ_END;
}

void ChessData::getNetStoryWar()
{
	JSON_READ_HEAD("json/net_story_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrNWI = new NetWarInfo();
		ptrNWI->iType = BATTLE_ROMANCE;
		FUZHIINT(ptrNWI->iMapIndex, line[i]["mapid"].asInt());
		FUZHIINT(ptrNWI->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNWI->iImgId, line[i]["img"].asInt());
		FUZHIINT(ptrNWI->iPosX, line[i]["x"].asInt());
		FUZHIINT(ptrNWI->iPosY, line[i]["y"].asInt());
		FUZHIINT(ptrNWI->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrNWI->iEnemyLv, line[i]["enemy_lv"].asInt());
		FUZHIINT(ptrNWI->iMapId, line[i]["map"].asInt());
		FUZHISTRINGEX(ptrNWI->strName, line[i]["name"]);
		FUZHISTRINGEX(ptrNWI->strInfo, line[i]["describe"]);
		FUZHIINTEX(ptrNWI->iGkLv, line[i]["gk_lv"]);
		ptrNWI->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		ptrNWI->mReach = getSeidData(line[i]["reach"].asCString());
		m_mNetWarInfo[ptrNWI->iWarId] = ptrNWI;
		map_total_war[ptrNWI->iMapIndex][ptrNWI->iWarId] = 1;
	}

	JSON_READ_END;
}

void ChessData::getNetStalkerWar()
{
	JSON_READ_HEAD("json/net_stalker_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrNWI = new NetWarInfo();
		ptrNWI->iType = BATTLE_LADDER;
		ptrNWI->iMapIndex = LADDER_SCREEN_MAPID;
		FUZHIINT(ptrNWI->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNWI->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrNWI->iEnemyLv, line[i]["enemy_lv"].asInt());
		FUZHIINT(ptrNWI->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrNWI->iGold, line[i]["gold"].asInt());
		FUZHIINT(ptrNWI->iJade, line[i]["jade"].asInt());
		FUZHIINT(ptrNWI->iExp, line[i]["hExp"].asInt());
		FUZHIINT(ptrNWI->strName, line[i]["name"].asCString());
		FUZHIINT(ptrNWI->iRound, line[i]["round"].asInt());
		FUZHIINTEX(ptrNWI->iGkLv, line[i]["gk_lv"]);
		ptrNWI->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		m_mNetWarInfo[ptrNWI->iWarId] = ptrNWI;
	}

	JSON_READ_END;
}

void ChessData::getNetStoryCondition()
{
	JSON_READ_HEAD("json/net_story_condition.json");
	for (int i = 0; i < size; i++)
	{
		NetStoryCondition* ptrNSC = new NetStoryCondition();
		FUZHIINT(ptrNSC->iWarId,line[i]["warid"].asInt());
		FUZHIINT(ptrNSC->iVariable, line[i]["variable"].asInt());
		FUZHIINT(ptrNSC->iType, line[i]["type"].asInt());
		FUZHIINT(ptrNSC->iGqId, line[i]["gk"].asInt());
		FUZHISTRINGEX(ptrNSC->strDescribe, line[i]["describe"]);
		net_story_condition[ptrNSC->iWarId][ptrNSC->iVariable] = ptrNSC;
	}

	JSON_READ_END;
}

void ChessData::getNetStoryGoods()
{
	JSON_READ_HEAD("json/net_story_goods.json");
	for (int i = 0; i < size; i++)
	{
		NetStoryGoods* ptrNSG = new NetStoryGoods();
		FUZHIINT(ptrNSG->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNSG->iVariable, line[i]["variable"].asInt());
		ptrNSG->mHid = getSeidData(line[i]["hid"].asCString());
		ptrNSG->mGoods = getGoodsMapData(line[i]["good_id"].asCString());
		net_story_goods[ptrNSG->iWarId][ptrNSG->iVariable] = ptrNSG;
	}

	JSON_READ_END;
}

void ChessData::getNetWarHero()
{
	JSON_READ_HEAD("json/net_war_hero.json");
	for (int i = 0; i < size; i++)
	{
		NetWarHero* ptrNWH = new NetWarHero();
		FUZHIINT(ptrNWH->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNWH->iDispatchActorMinNum, line[i]["number"].asInt());
		ptrNWH->fix_hid = getMapData(line[i]["fix_hid"].asCString());
		ptrNWH->avoid_hid = getMapData(line[i]["avoid_hid"].asCString());
		ptrNWH->allow_hid = getMapData(line[i]["allow_hid"].asCString());
		FUZHIINT(ptrNWH->allow_lv, line[i]["allow_lv"].asInt());
		net_war_hero[ptrNWH->iWarId] = ptrNWH;
	}

	JSON_READ_END;
}

void ChessData::getNetSuit()
{
	JSON_READ_HEAD("json/net_suit.json");
	for (int i = 0; i < size; i++)
	{
		NetSuitDataPtr ptrNSD = new NetSuitData();
		FUZHIINT(ptrNSD->id, line[i]["id"].asInt());
		FUZHIINT(ptrNSD->name, line[i]["name"].asString());
		FUZHIINT(ptrNSD->info, line[i]["info"].asString());
		ptrNSD->item_id = getMapData(line[i]["item_id"].asCString());
		ptrNSD->seid1 = getMapData(line[i]["seid1"].asCString());
		ptrNSD->hid = getMapData(line[i]["hid"].asCString());
		ptrNSD->seid2 = getMapData(line[i]["seid2"].asCString());
		net_suit_data[ptrNSD->id] = ptrNSD;
	}

	JSON_READ_END;
}

void ChessData::getNetMapPremise()
{
    //NetWarPermise,net_war_permise
    JSON_READ_HEAD("json/net_war_premise.json");
    for (int i = 0; i < size; i++)
    {
        NetWarPermise* ptrNWP = new NetWarPermise();
        FUZHIINT(ptrNWP->iId, line[i]["id"].asInt());
        ptrNWP->mP_warid = getMapData(line[i]["p_warid"].asCString());
        ptrNWP->mVar = getMapData(line[i]["var"].asCString());
        ptrNWP->mGoods = getGoodsMapData(line[i]["goods"].asCString());
        ptrNWP->mHid = getMapData(line[i]["hid"].asCString());
		ptrNWP->mExplain = getStringMap(line[i]["explain"].asCString(), '&');
        net_war_permise[ptrNWP->iId] = ptrNWP;
    }
    
    JSON_READ_END;
}
void ChessData::getNetStoryTxtColor(int iMapId, const char* json_test)
{
	const char* point = 0, *lastPoint = 0,*point_1 = 0;
	int add = 1;
	const char *buf = json_test;

	point = lastPoint = buf;
	while (point < buf + strlen(buf))
	{
		point = strstr(lastPoint, "|");
		if (point == NULL)
		{
			point = buf + strlen(buf);
		}
		{
			point_1 = strstr(lastPoint, ",");
			if (point_1 != NULL)
			{
				int next_record = 0;
				int test_len = strlen(lastPoint) - strlen(point_1);
				int j = 0,l = 0;
				map<int, int> mIntArray;
				map<int, int> mIndexArr;

				while (j < test_len)
				{
					next_record = StringToInt((char*)lastPoint + j);
					j++;
					j += GetNumDigit(next_record);
					mIndexArr[l++] = next_record;
				}

				j = 0;
				l = 0;
				next_record = 0;
				lastPoint = point_1 + 1;
				test_len = strlen(lastPoint) - strlen(point);

				while (j < test_len)
				{
					next_record = StringToInt((char*)lastPoint + j);
					j++;
					j += GetNumDigit(next_record);
					mIntArray[l++] = next_record;
				}

				for (size_t k = 0; k < mIndexArr.size(); k++)
					net_story[iMapId]->txt_color[mIndexArr[k]] = mIntArray;
			}
			
		}
		lastPoint = point + add;
	}
}

void ChessData::getNetHeroSkillElem(int iIndex, const char* json_skill, const char* json_level)
{
	map<int, int> skill_arr;
	map<int, int> level_arr;
	int next_record = 0;
	int test_len = strlen(json_skill);
	int j = 0, k = 0, l = 0, m = 0;
	while (j < test_len)
	{
		next_record = StringToInt((char*)json_skill + j);
		j++;
		j += GetNumDigit(next_record);
		skill_arr[m++] = next_record;
	}

	test_len = strlen(json_level);
	while (k < test_len)
	{
		next_record = StringToInt((char*)json_level + k);
		k++;
		k += GetNumDigit(next_record);
		level_arr[l++] = next_record;
	}

	for (int i = 0; i < m; i++)
	{
		trData->net_hero_skill[iIndex]->skill[skill_arr[i]] = level_arr[i];
	}
}

void ChessData::getDicFits()
{
	JSON_READ_HEAD("json/dic_fits.json");

	actor_fits_num = size;
	actor_fits = new ActorFits[actor_fits_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT2(actor_fits[i].id, line[i]["id"].asInt());
		actor_fits[i].hid_num = getjson_stringeffect(&actor_fits[i].hid, line[i]["hid"].asCString());
		FUZHIINT(actor_fits[i].fits, line[i]["fits"].asInt());
		FUZHIINT(actor_fits[i].atkid, line[i]["atkid"].asInt());
		actor_fits[i].sounds_num = getjson_stringeffect(&actor_fits[i].sounds, line[i]["sounds"].asCString());
		FUZHISTRING(actor_fits[i].info, line[i]["info"].asCString());
	}
	JSON_READ_END;
}
void ChessData::getAnimationInfo()
{
	JSON_READ_HEAD("json/net_db_info.json");

	animation_info_num = size;
	animation_info = new AnimationInfo[animation_info_num];
	for (int i = 0; i < size; i++)
	{
		FUZHIINT(animation_info[i].hid, line[i]["hid"].asInt());
		FUZHIINT(animation_info[i].gender, line[i]["gender"].asInt());
		FUZHISTRING(animation_info[i].name, line[i]["db_name"].asCString());
	}
	JSON_READ_END;
}
void ChessData::GetSeidData(const char* json_test, short& seid_num, short*& seid)
{
	int next_record = 0;
	int test_len = strlen(json_test);
	int j = 0, k = 0, l = 0, m = 0;
	while(j < test_len)
	{
		next_record = StringToInt((char*)json_test + j);
		j++;
		j += GetNumDigit(next_record);
		m++;
	}
	FUZHIINT(seid_num, m);
	seid = new short[m];

	while(k < test_len)
	{
		next_record = StringToInt((char*)json_test + k);
		k++;
		k += GetNumDigit(next_record);
		FUZHIINT(seid[l++], next_record);
	}
}
int ChessData::GetNumDigit(int iNum)
{
	int iRet = 0;
	if (iNum < 0)
	{
		iRet++; 
		iNum = -iNum;
	}
	do
	{
		iRet++;
		iNum /= 10;
	} while (iNum != 0);
	return iRet;
}

void ChessData::getDicItemClass()
{
	m_mItemClass.clear();
	JSON_READ_HEAD("json/dic_item_class.json");
	for(int i = 0; i < size; i++)
	{
		m_mItemClass[i] = new ItemClass();
		FUZHIINT(m_mItemClass[i]->id, line[i]["id"].asInt());
		FUZHISTRING(m_mItemClass[i]->name, line[i]["name"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getDicTreasureInfo()
{
	m_mTreasureInfo.clear();
	JSON_READ_HEAD("json/dic_good_info.json");
	for(int i = 0; i < size; i++)
	{
		int id = 0;
		FUZHIINT(id, line[i]["good_id"].asInt());
		m_mTreasureInfo[id] = new TreasureInfo();
		FUZHISTRING(m_mTreasureInfo[id]->name, line[i]["good_name"].asCString());
		strcpy(m_mTreasureInfo[id]->method, DString(line[i]["obtain_method"].asCString())->replace('&', '\n')->getChar());
		FUZHISTRING(m_mTreasureInfo[id]->condition, line[i]["special_condition"].asCString());
	}
	JSON_READ_END;
}

void ChessData::getNetJobInfo()
{
	m_mNetJob.clear();
	JSON_READ_HEAD("json/net_job_info.json");
	for(int i = 0; i < size; i++)
	{
		m_mNetJob[i] = new NetJobInfo();
		m_mNetJob[i]->jobId = i;
		GetSeidData(line[i]["skill"].asCString(), m_mNetJob[i]->skillNum, m_mNetJob[i]->skillId);
		FUZHISTRING(m_mNetJob[i]->jobInfo, line[i]["job_info"].asCString());
	}
	JSON_READ_END;
}

map<int, string> ChessData::getStringMap(string chData, char chSplit)
{
	map<int, string> mStringMap;
	DString strData = chData.c_str();
	DString strCur = "";
	int i = 0, index1 = -1, index2 = -1;
	do 
	{
		index1 = index2 + 1;
		index2 = strData->indexOf(chSplit, index1);

		if(index2 != -1)
		{
			strCur = strData->substring(index1, index2);
		}
		else
		{
			strCur = strData->substring(index1, chData.length());
		}
		if(strCur != "")
		{
			mStringMap[i++] = strCur->getChar();
		}
	}
	while(index2 != -1);

	return mStringMap;
}

void ChessData::getNetHeroName()
{
	JSON_READ_HEAD("json/net_hero_name.json");
	for(int i = 0; i < size; i++)
	{
		int index = line[i]["id"].asInt();
		if (index == 1)
		{
			m_mFirstName = getStringMap(line[i]["name"].asString(), ',');
		}
		else if (index == 2)
		{
			m_mSecName = getStringMap(line[i]["name"].asString(), ',');
		}
	}
	JSON_READ_END;
}

void ChessData::getNetHeroNameForbid()
{
	JSON_READ_HEAD("json/net_hero_name_forbid.json");
	for(int i = 0; i < size; i++)
	{
		m_mNameForbid[i] = line[i].asString();
	}
	JSON_READ_END;
}

map<int, int> ChessData::getSeidData(const char* chData)
{
	map<int, int> mSeidMap;
	UInt k = 0, l = 0;
	while(k < strlen(chData))
	{
		int next_record = StringToInt((char*)chData + k);
		k++;
		k+=GetNumDigit(next_record);
		mSeidMap[l++] = next_record;
	}
	return mSeidMap;
}

map<int, int> ChessData::getMapData(const char* chData)
{
	map<int, int> RetMap;
	UInt k = 0, l = 0;
	while (k < strlen(chData))
	{
		int next_record = StringToInt((char*)chData + k);
		k++;
		k += GetNumDigit(next_record);
		RetMap[next_record] = ++l;
	}
	return RetMap;
}

map<int, int> ChessData::getGoodsMapData(const char* chData)
{
	map<int, int> mGoodsMap;
	const char* point = 0, *lastPoint = 0;
	int add = 1;
	const char *buf = chData;
	point = lastPoint = buf;
	int arr_index = 0;
	while(point < buf + strlen(buf))
	{
		point = strstr(lastPoint, "|");
		if(point == NULL)
		{
			point = buf + strlen(buf);
		}
		{
			int next_record = 0;
			int test_len = strlen(lastPoint) - strlen(point);
			int j = 0, k = 0, l = 0;
			map<int, int> mIntArray;
			while(j < test_len)
			{
				next_record = StringToInt((char*)lastPoint + j);
				j++;
				j += GetNumDigit(next_record);
				mIntArray[l++] = next_record;
			}
			if (mIntArray.size() == 2)
			{
				mGoodsMap[mIntArray[0]] = mIntArray[1];
			}
		}
		arr_index++;
		lastPoint = point + add;
	}

	return mGoodsMap;
}

map<int, map<int, int> > ChessData::getMapJsonData(const char* chData)
{
	map<int, map<int,int> > mRetData;
	int iRetIndex = 0;
	const char* point = 0, *lastPoint = 0;
	int add = 1;
	const char *buf = chData;
	point = lastPoint = buf;
	int arr_index = 0;
	while (point < buf + strlen(buf))
	{
		point = strstr(lastPoint, "|");
		if (point == NULL)
		{
			point = buf + strlen(buf);
		}
		{
			int next_record = 0;
			int test_len = strlen(lastPoint) - strlen(point);
			int j = 0, k = 0, l = 0;
			map<int, int> mIntArray;
			while (j < test_len)
			{
				next_record = StringToInt((char*)lastPoint + j);
				j++;
				j += GetNumDigit(next_record);
				mIntArray[l++] = next_record;
			}
			if (mIntArray.size() >= 2)
			{
				mRetData[iRetIndex][0] = mIntArray[0];
				mRetData[iRetIndex][1] = mIntArray[1];
				iRetIndex++;
			}
		}
		arr_index++;
		lastPoint = point + add;
	}

	return mRetData;
}

bool ChessData::isConsume(int iGoodsId)
{
	bool bConsume = false;
	if(m_mGoodsItem.count(iGoodsId) > 0)
	{
		if(m_mGoodsItem[iGoodsId]->iItId == 999)
		{
			bConsume = true;
		}
	}
	return bConsume;
}
void ChessData::getMysNetShop()
{
	Json::Reader reader;
	Json::Value line;
	char* chData = NULL;

	chData = ChessReadFile("json/net_shop_item_spc.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			MysNetShop* pShopItem = new MysNetShop();
			pShopItem->iShopId = line[i]["id"].asInt();
			pShopItem->iGoodsId = line[i]["goodid"].asInt();
			pShopItem->iPayType = line[i]["money_type"].asInt();
			pShopItem->iPrice = line[i]["price"].asInt();
			pShopItem->iNum = line[i]["val"].asInt();
			m_mMysNetShop[pShopItem->iShopId] = pShopItem;
		}
		ChessFree(chData);
	}
}
void ChessData::getNetShopInfo()
{
	Json::Reader reader;
	Json::Value line;
	char* chData = NULL;

	chData = ChessReadFile("json/net_shop_item.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			NetShopItem* pShopItem = new NetShopItem();
			pShopItem->iShopId = line[i]["id"].asInt();
			pShopItem->iGoodsId = line[i]["goodid"].asInt();
			pShopItem->iPayType = line[i]["money_type"].asInt();
			pShopItem->iPrice = line[i]["price"].asInt();
			pShopItem->iNum = line[i]["val"].asInt();
			m_mNetShop[pShopItem->iShopId] = pShopItem;
		}
		ChessFree(chData);
	}

	chData = ChessReadFile("json/net_shop_price.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			m_mShopRefPrice[line[i]["id"].asInt()] = line[i]["jade"].asInt();
		}
		ChessFree(chData);
	}
}

void ChessData::getNetGoldExchange()
{
	JSON_READ_HEAD("json/net_jade_of_gold.json");
	for(int i = 0; i < size; i++)
	{
		NetGoldExchange* ptrGoldExchange = new NetGoldExchange();
		FUZHIINT(ptrGoldExchange->iId, line[i]["id"].asInt());
		FUZHIINT(ptrGoldExchange->iJade, line[i]["jade"].asInt());
		FUZHIINT(ptrGoldExchange->iGold, line[i]["gold"].asInt());
		m_mGoldExchange[ptrGoldExchange->iId] = ptrGoldExchange;
	}
	JSON_READ_END;
}

void ChessData::getNetPubInfo()
{
	Json::Reader reader;
	Json::Value line;
	char* chData = NULL;

	chData = ChessReadFile("json/net_pub_jade.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			m_mPubRefPrice[line[i]["id"].asInt()] = line[i]["jade"].asInt();
		}
		ChessFree(chData);
	}

	chData = ChessReadFile("json/net_pub_position.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			PubPosition* ptrPubPos = new PubPosition();
			ptrPubPos->iId = line[i]["id"].asInt();
			ptrPubPos->iPointX = line[i]["point_x"].asInt();
			ptrPubPos->iPointY = line[i]["point_y"].asInt();
			m_mPubPosition[ptrPubPos->iId] = ptrPubPos;
		}
		ChessFree(chData);
	}

	chData = ChessReadFile("json/net_pub_qinju.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			PubInvitePrice* ptrPubPrice = new PubInvitePrice();
			ptrPubPrice->iId = line[i]["id"].asInt();
			ptrPubPrice->iGold = line[i]["gold"].asInt();
			ptrPubPrice->iJade = line[i]["jade"].asInt();
			m_mPubInvite[ptrPubPrice->iId] = ptrPubPrice;
		}
		ChessFree(chData);
	}
}

void ChessData::getNetVipInfo()
{
	JSON_READ_HEAD("json/net_vip_info.json");
	for(int i = 0; i < size; i++)
	{
		VipInfo* ptrVip = new VipInfo();
		FUZHIINT(ptrVip->iVipId, line[i]["id"].asInt());
		ptrVip->strInfo = line[i]["vip_info"].asString();
		ptrVip->mapGifts = getGoodsMapData(line[i]["itemid"].asCString());
		m_mVipInfo[ptrVip->iVipId] = ptrVip;
	}
	JSON_READ_END;
}

void ChessData::getNetFbWar()
{
	JSON_READ_HEAD("json/net_fb_war.json");
	for(int i = 0; i < size; i++)
	{
		NetWarInfo* ptrActS = new NetWarInfo();
		ptrActS->iType = BATTLE_ACTSCENE;
		FUZHIINT(ptrActS->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrActS->iImgId, line[i]["imgid"].asInt());
		FUZHIINT(ptrActS->iMaxTimes, line[i]["number_battle"].asInt());
		FUZHIINT(ptrActS->iEnemyLv, line[i]["enemy_lv"].asInt());
		FUZHIINT(ptrActS->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrActS->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrActS->iGold, line[i]["gold"].asInt());
		FUZHIINTEX(ptrActS->iGkLv, line[i]["gk_lv"]);
		FUZHIINT(ptrActS->iStage, line[i]["stage"].asInt());
		ptrActS->strName = line[i]["name"].asString();
		ptrActS->strInfo = line[i]["describe"].asString();
		ptrActS->strLv = line[i]["lv"].asString();
		ptrActS->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		m_mNetWarInfo[ptrActS->iWarId] = ptrActS;
	}
	JSON_READ_END;
}

void ChessData::getNetPrompt()
{
	JSON_READ_HEAD("json/net_prompt.json");
	for(int i = 0; i < size; i++)
	{
		NoticeActOpen* ptrNotice = new NoticeActOpen();
		FUZHIINT(ptrNotice->iWarId, line[i]["warid"].asInt());
		FUZHIINT(ptrNotice->iOpenType, line[i]["open_tpye"].asInt());
		FUZHIINT(ptrNotice->iImgId, line[i]["icon_hot"].asInt());
		ptrNotice->strText1 = line[i]["txte_a"].asString();
		ptrNotice->strText2 = line[i]["txte_b"].asString();
		m_mNoticeOpen[ptrNotice->iWarId] = ptrNotice;
	}
	JSON_READ_END;
}

int ChessData::getOpenHeroicId(int iWarId)
{
	int iOpenId = 0;
	for(map<int, NetWarInfo*>::iterator it = m_mNetWarInfo.begin(); it != m_mNetWarInfo.end(); it++)
	{
		int iCurId = it->first;
		NetWarInfo* ptrNetWar = it->second;
		if(ptrNetWar->iType == BATTLE_HEROIC)
		{
			if(ptrNetWar->iOpenWarId == iWarId)
			{
				iOpenId = iCurId;
				break;
			}
		}
	}
	return iOpenId;
}
void ChessData::getNetShopRefresh()
{
	JSON_READ_HEAD("json/net_shop_refresh.json");
	for (int i = 0; i < size; i++)
	{
		NetRefresh* ptrNRF = new NetRefresh();
		FUZHIINT(ptrNRF->iVipLv, line[i]["VIP_LV"].asInt());
		FUZHIINT(ptrNRF->iFree_plain, line[i]["free_plain"].asInt());
		FUZHIINT(ptrNRF->iFree_special, line[i]["free_special"].asInt());
		FUZHIINT(ptrNRF->iMax_subst, line[i]["max_subst"].asInt());
		FUZHIINT(ptrNRF->iSubst20, line[i]["subst20"].asInt());
		FUZHIINT(ptrNRF->iSubst40, line[i]["subst40"].asInt());
		FUZHIINT(ptrNRF->iSubst80, line[i]["subst80"].asInt());
		m_mNetShopRef[ptrNRF->iVipLv] = ptrNRF;
	}
	JSON_READ_END;
}

void ChessData::getNetMapCity()
{
    JSON_READ_HEAD("json/net_map_city.json");
    for(int i = 0; i < size; i++)
    {
        NetMapCity* ptrNmc= new NetMapCity();
        FUZHIINT(ptrNmc->iId, line[i]["id"].asInt());
        FUZHIINT(ptrNmc->iPosX, line[i]["x"].asInt());
        FUZHIINT(ptrNmc->iPosY, line[i]["y"].asInt());
        FUZHIINT(ptrNmc->iImgId, line[i]["img"].asInt());
        ptrNmc->strName = line[i]["name"].asString();
        m_mNetMapCity[ptrNmc->iId] = ptrNmc;
    }
    JSON_READ_END;
}
void ChessData::getNetRecastPrice()
{
	JSON_READ_HEAD("json/net_item_recast_pirce.json");
	for (int i = 0; i < size; i++)
	{
		NetItemRecastPrice* ptrNmc = new NetItemRecastPrice();
		FUZHIINT(ptrNmc->iPrice, line[i]["pirce"].asInt());
		FUZHIINT(ptrNmc->iRecast, line[i]["recast"].asInt());
		m_mNetRecastPrice[ptrNmc->iRecast] = ptrNmc;
	}
	JSON_READ_END;
}
void ChessData::getNetJadeOfVitality()
{
	JSON_READ_HEAD("json/net_jade_of_vitality.json");
	for (int i = 0; i < size; i++)
	{
		NetJadeOfVitality* ptrNmc = new NetJadeOfVitality();
		FUZHIINT(ptrNmc->iId, line[i]["id"].asInt());
		FUZHIINT(ptrNmc->iJade, line[i]["jade"].asInt());
		FUZHIINT(ptrNmc->iVitality, line[i]["vitality"].asInt());
		m_mNetJadeOfVitality[ptrNmc->iId] = ptrNmc;
	}
	JSON_READ_END;
}
void ChessData::getNetMapClear()
{
	JSON_READ_HEAD("json/net_map_clear.json");
	for (int i = 0; i < size; i++)
	{
		NetMapClear* ptrNMC = new NetMapClear();
		FUZHIINT(ptrNMC->warid, line[i]["warid"].asInt());
		ptrNMC->position = getSeidData(line[i]["position"].asCString());
		FUZHIINT(ptrNMC->clear_img, line[i]["clear_img"].asCString());
		net_map_clear[ptrNMC->warid][net_map_clear[ptrNMC->warid].size()] = ptrNMC;
	}
	JSON_READ_END;
}
void ChessData::getVipIncome()
{
	JSON_READ_HEAD("json/vip_income.json");
	for (int i = 0; i < size; i++)
	{
		VipIncome* ptrVip = new VipIncome();
		ptrVip->iVIP = line[i]["VIP"].asInt();
		ptrVip->iMAX = line[i]["MAX"].asInt();
		ptrVip->iRate = line[i]["rate"].asInt();
		ptrVip->iSweep = line[i]["sweep"].asInt();
		ptrVip->iVipexp = line[i]["vipexp"].asInt();
		ptrVip->iTradings = line[i]["Tradings"].asInt();
		ptrVip->iRemission = line[i]["remission"].asInt();
		m_mVipIncome[ptrVip->iVIP] = ptrVip;
	}
	JSON_READ_END;
}
void ChessData::getNetBuff()
{
	JSON_READ_HEAD("json/net_buff.json");
	for(int i = 0; i < size; i++)
	{
		BuffElem* ptrBuff = new BuffElem();
		ptrBuff->iId = line[i]["id"].asInt();
		ptrBuff->iImgId = line[i]["img"].asInt();
		ptrBuff->iType = line[i]["type"].asInt();
		ptrBuff->iPercent = line[i]["prop_per"].asInt();
		ptrBuff->iReset = line[i]["reset"].asInt();
		ptrBuff->iEffectType = line[i]["effect_type"].asInt();
		ptrBuff->iRound = line[i]["round"].asInt();
		ptrBuff->iIsPlay = line[i]["is_play"].asInt();
		ptrBuff->iSound = line[i]["sound"].asInt();
		ptrBuff->strName = line[i]["name"].asString();
		ptrBuff->strAnima1 = line[i]["animation1"].asString();
		ptrBuff->strAnima2 = line[i]["animation2"].asString();
		m_mDataBuff[ptrBuff->iId] = ptrBuff;
	}
	JSON_READ_END;
}

void ChessData::getNetErrorMSG()
{
	JSON_READ_HEAD("json/net_error_msg.json");
	for(int i = 0; i < size; i++)
	{
		int iCode = line[i]["code"].asInt();
		m_mErrorMSG[iCode] = line[i]["info"].asString();
	}
	JSON_READ_END;
}

void ChessData::parseJsonData()
{
	Json::Reader reader;
	Json::Value line;
	char* chData = NULL;

	//每日副本
	chData = ChessReadFile("json/net_daily_war.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			NetWarInfo* ptrActS = new NetWarInfo();
			ptrActS->iType = BATTLE_DAILY;
			FUZHIINT(ptrActS->iWarId, line[i]["warid"].asInt());
			FUZHIINT(ptrActS->iMapIndex, line[i]["mapid"].asInt());
			FUZHIINT(ptrActS->iImgId, line[i]["imgid"].asInt());
			FUZHIINT(ptrActS->iMaxTimes, line[i]["number_battle"].asInt());
			FUZHIINTEX(ptrActS->iGkLv, line[i]["gk_lv"]);
			FUZHIINT(ptrActS->iEnemyLv, line[i]["enemy_lv"].asInt());
			FUZHIINT(ptrActS->iMapId, line[i]["map"].asInt());
			FUZHIINT(ptrActS->iNumLimit, line[i]["number_limit"].asInt());
			FUZHIINT(ptrActS->iGold, line[i]["gold"].asInt());
			ptrActS->strName = line[i]["name"].asString();
			ptrActS->strLv = line[i]["lv"].asString();
			ptrActS->strInfo = line[i]["describe"].asString();
			ptrActS->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
			m_mNetWarInfo[ptrActS->iWarId] = ptrActS;
		}
		ChessFree(chData);
	}

	//每日副本信息
	chData = ChessReadFile("json/net_daily.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			DailyWar* ptrDaily = new DailyWar();
			ptrDaily->iId = line[i]["id"].asInt();
			ptrDaily->iMapId = line[i]["mapid"].asInt();
			ptrDaily->iImgId = line[i]["map_img"].asInt();
			ptrDaily->iMaxTimes = line[i]["number_battle"].asInt();
			ptrDaily->strName = line[i]["name"].asString();
			ptrDaily->strInfo = line[i]["describe"].asString();

			int iIndex = 0;
			for(map<int, NetWarInfo*>::iterator it = trData->m_mNetWarInfo.begin(); it != trData->m_mNetWarInfo.end(); it++)
			{
				if(it->second->iMapIndex == ptrDaily->iMapId)
				{
					ptrDaily->mWarId[iIndex++] = it->first;
				}
			}

			m_mDailyWar[ptrDaily->iMapId] = ptrDaily;
		}
		ChessFree(chData);
	}

	//物品合成类型
	chData = ChessReadFile("json/net_make_type.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			NetMergeType* ptrType = new NetMergeType();
			ptrType->iId = line[i]["id"].asInt();
			ptrType->iImgId = line[i]["img"].asInt();
			ptrType->strName = line[i]["name"].asString();
			m_mMergeType[ptrType->iId] = ptrType;
		}
		ChessFree(chData);
	}

	//物品合成数据
	chData = ChessReadFile("json/net_make_data.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			NetMergeData* ptrData = new NetMergeData();
			ptrData->iGoodsId = line[i]["good_id"].asInt();
			ptrData->iType = line[i]["type"].asInt();
			ptrData->iCount = line[i]["num"].asInt();
			ptrData->iGold = line[i]["gold"].asInt();
			ptrData->barrage = line[i]["barrage"].asInt();
			ptrData->strName = line[i]["describe"].asString();
			ptrData->mMaterial = getGoodsMapData(line[i]["itemid"].asCString());
			m_mMergeData[ptrData->iGoodsId] = ptrData;
		}
		ChessFree(chData);
	}

	//附魔宝石套装
	chData = ChessReadFile("json/net_bw_suit.json");
	if(chData && reader.parse(chData, line, false))
	{
		for(UInt i = 0; i < line.size(); i++)
		{
			FmStoneSuit* ptrSuit = new FmStoneSuit();
			ptrSuit->iSuitId = line[i]["id"].asInt();
			ptrSuit->mNeedId = getSeidData(line[i]["item_need"].asCString());
			ptrSuit->mNorSeid = getSeidData(line[i]["seid"].asCString());
			ptrSuit->mHeroId = getSeidData(line[i]["hid"].asCString());
			ptrSuit->mHeroDel = getSeidData(line[i]["seid_delete"].asCString());
			ptrSuit->mHeroAdd = getSeidData(line[i]["seid_add"].asCString());
			ptrSuit->mHeroExcl = getSeidData(line[i]["seid_exclusive"].asCString());
			m_mFmStone[ptrSuit->iSuitId] = ptrSuit;
		}
		ChessFree(chData);
	}

	//系统开启配置表
	chData = ChessReadFile("json/system_open_control.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			SystemOpenItem* item = new SystemOpenItem(line[i]);
			if (line[i]["hero"].isString())
				item->hero = getMapData(line[i]["hero"].asCString());
			if (line[i]["goods"].isString())
				item->goods = getMapData(line[i]["goods"].asCString());
			m_mSystemOpen[line[i]["id"].asInt()] = item;
		}
		ChessFree(chData);
	}

	//PVP坐标系
	chData = ChessReadFile("json/net_group_xy.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			PVPPoint* ptrPoint = new PVPPoint();
			ptrPoint->iPointId = line[i]["id"].asInt();
			ptrPoint->iPosX = line[i]["x"].asInt();
			ptrPoint->iPosY = line[i]["y"].asInt();
			m_mPVPPoint[ptrPoint->iPointId] = ptrPoint;
		}
		ChessFree(chData);
	}

	//PVP阵形
	chData = ChessReadFile("json/net_group.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			PVPArray* ptrArray = new PVPArray();
			ptrArray->iArrayId = line[i]["id"].asInt();
			ptrArray->strName = line[i]["name"].asString();
			ptrArray->strInfo = line[i]["info"].asString();
			ptrArray->mPosGroup = getSeidData(line[i]["combine"].asCString());
			map<int, string> mstrSeid = getStringMap(line[i]["seid"].asCString(), '|');
			for (UInt k = 0; k < mstrSeid.size(); k++)
			{
				ptrArray->mSeidGroup[k] = getSeidData(mstrSeid[k].c_str());
			}
			m_mPVPArray[ptrArray->iArrayId] = ptrArray;
		}
		ChessFree(chData);
	}

	//PVP奖励
	chData = ChessReadFile("json/net_autopvp_reward.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			PVPReward* ptrReward = new PVPReward();
			ptrReward->iRewId = line[i]["id"].asInt();
			ptrReward->strName = line[i]["name"].asString();
			map<int, string> mstrRew = getStringMap(line[i]["itemid"].asCString(), '|');
			for (UInt k = 0; k < mstrRew.size(); k++)
			{
				ptrReward->mReward[k] = getSeidData(mstrRew[k].c_str());
			}
			m_mPVPReward[ptrReward->iRewId] = ptrReward;
		}
		ChessFree(chData);
	}

	//PVP副本
	chData = ChessReadFile("json/net_autopvp_war.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			NetWarInfo* ptrWarInfo = new NetWarInfo();
			ptrWarInfo->iType = BATTLE_PVP;
			FUZHIINT(ptrWarInfo->iWarId, line[i]["warid"].asInt());
			FUZHIINT(ptrWarInfo->iMapId, line[i]["map"].asInt());
			FUZHIINT(ptrWarInfo->iNumLimit, line[i]["number_limit"].asInt());
			FUZHIINTEX(ptrWarInfo->iGkLv, line[i]["gk_lv"]);
			FUZHIINT(ptrWarInfo->iEnemyLv, line[i]["enemy_lv"].asInt());
			ptrWarInfo->strName = line[i]["name"].asString();
			ptrWarInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
			m_mNetWarInfo[ptrWarInfo->iWarId] = ptrWarInfo;
		}
		ChessFree(chData);
	}

	//巅峰对决选项
	chData = ChessReadFile("json/net_pk.json");
	if (chData && reader.parse(chData, line, false))
	{
		for (UInt i = 0; i < line.size(); i++)
		{
			DailyWar* ptrItem = new DailyWar();
			ptrItem->iId = line[i]["id"].asInt();
			ptrItem->iMapId = line[i]["mapid"].asInt();
			ptrItem->iImgId = line[i]["map_img"].asInt();
			ptrItem->iMaxTimes = line[i]["number_battle"].asInt();
			ptrItem->strName = line[i]["name"].asString();
			ptrItem->strInfo = line[i]["describe"].asString();
			m_mPKMap[ptrItem->iId] = ptrItem;
		}
		ChessFree(chData);
	}
}


bool ChessData::isSystemOpen(const SystemOpenIDS& id, bool popTips) {
	int intId = id;
	SystemOpenItem* item = m_mSystemOpen[intId];
	if (item == nullptr) {
		if (popTips) {
			string strId = StringUtils::format("%d",intId);
			string tips = StringUtils::format(LL("tgsmkq"), "func", strId.c_str());
			ITools::getInstance()->createGlobalPrompt(tips, false);
		}
		return false;
	}
	if (item->hero_num > 0) {
		int iClanNum = 0;
		for (int i = 0; i < history->ownArmyNum; i++)
		{
			if (history->actors[history->ownArmy[i]].GetActorLv() >= item->lv_limit)
			{
				iClanNum++;
			}
		}
		string strId = item->strName;
		if (iClanNum < item->hero_num)
		{
			if (popTips) {

				string tips = StringUtils::format(LL("kqxyyxsldj"), strId.c_str(), item->hero_num, item->lv_limit);
				ITools::getInstance()->createGlobalPrompt(tips, false);
			}
			return false;
		}
	}
	/*
	if (!item->hero.empty()) {
		for (auto &kv : item->hero) {
			CCLOG("%d has value %d", kv.first, kv.second);
			auto found = false;
			for (int i = 0; i < history->ownArmyNum; i++)
			{
				if (history->actors[history->ownArmy[i]].actor_id == kv.first) {
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (popTips) {
					ITools::getInstance()->createGlobalPrompt(LL("gongnengweikaiqi"), false);
				}
				return false;
			}
		}
	}
	if (!item->goods.empty()) {
		for (auto &kv : item->goods) {
			CCLOG("%d has value %d"  ,kv.first,  kv.second );
			if (this->m_mGoodsItem.count(kv.first) == 0)
			{ 
				if (popTips) {
					ITools::getInstance()->createGlobalPrompt(LL("gongnengweikaiqi"), false);
				}
				return false;
			}
		}
	}
	*/
	if (item->war_id > 0) {
		if (!history->mMapDateInfo->getMapDateElem(item->war_id))
		{
			if (popTips) {
				ITools::getInstance()->createGlobalPrompt(__String::createWithFormat(LL("tgztxddgkq"), trData->m_mNetWarInfo[item->war_id]->strName.c_str())->getCString(), false);
			}
			return false;
		}
	}
	if (item->player_lv > 0) {
		if (history->actors[0].GetActorLv() < item->player_lv)
		{
			if (popTips) {
				string tips = StringUtils::format(LL("openafterherolv"), item->player_lv);
				ITools::getInstance()->createGlobalPrompt(tips, false);
			}
			return false;
		}
	}

	if (item->vip > 0) {
		if (history->m_iVIP) {

		}
		else {
			if (popTips) {
				ITools::getInstance()->createGlobalPrompt(LL("huangzukaifang"), false);
			}
			return false;
		}
	}


	//ITools::getInstance()->createGlobalPrompt(LL("jingqingqidai"), false);
	
	return true;
}

void ChessData::reloadData()
{
	FileUtils::getInstance()->purgeCachedEntries(); //清除搜索路径缓存

	//function.txt
	SAFE_DELETE(funConst);
	FunctionConst::initInstance();

	//strings.xml
	LL("", true);
}

void ChessData::getNetHeroDress()
{
	JSON_READ_HEAD("json/net_hero_dress.json");
	for (int i = 0; i < size; i++)
	{
		int iFashion = 0;
		FUZHIINT(iFashion, line[i]["id"].asInt());
		NetHeroDress* ptrNHDress = new NetHeroDress();
		ptrNHDress->mAtk = getSeidData(line[i]["atk"].asCString());
		ptrNHDress->mMov = getSeidData(line[i]["mov"].asCString());
		ptrNHDress->mSpc = getSeidData(line[i]["spc"].asCString());
		net_hero_dress[iFashion] = ptrNHDress;
	}
	JSON_READ_END;
}

void ChessData::getNetGlcWar()
{
	JSON_READ_HEAD("json/net_glc_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_WOLFCITY;
		ptrInfo->iMapIndex = MAPID_SCENE_WOLF_CITY;
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iImgId, line[i]["img"].asInt());
		FUZHIINT(ptrInfo->iPosX, line[i]["x"].asInt());
		FUZHIINT(ptrInfo->iPosY, line[i]["y"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrInfo->iMaxTimes, line[i]["number_battle"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		ptrInfo->guarding = getSeidData(line[i]["guarding"].asCString());
		ptrInfo->drop = getGoodsMapData(line[i]["drop"].asCString());
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = 1;
	}
	JSON_READ_END;
}

void ChessData::getNetXmtWar()
{
	JSON_READ_HEAD("json/net_xmt_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_IMAGE;
		ptrInfo->iMapIndex = MAPID_SCENE_IMAGE;
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iImgId, line[i]["img"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINT(ptrInfo->iMaxTimes, line[i]["number_battle"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		FUZHIINT(ptrInfo->iDegree, line[i]["difficulty"].asInt());
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = 1;
	}
	JSON_READ_END;
}

void ChessData::getNetStalkerReward()
{
	JSON_READ_HEAD("json/net_stalker_reward.json");
	for (int i = 0; i < size; i++)
	{
		NetStalkerReward* ptrNSR = new NetStalkerReward();
		FUZHIINT(ptrNSR->id, line[i]["id"].asInt());
		FUZHIINT(ptrNSR->name, line[i]["name"].asCString());
		ptrNSR->itemid = getMapJsonData(line[i]["itemid"].asCString());
		net_stalker_reward[ptrNSR->id] = ptrNSR;
	}
	JSON_READ_END;
}

void ChessData::getNetCloudXy()
{
	JSON_READ_HEAD("json/net_cloud_xy.json");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			net_cloud_xy[i][j] = line[i][IntToString(j+1)].asInt();
		}
	}
	JSON_READ_END;
}

void ChessData::getNetHeroNoDress()
{
	JSON_READ_HEAD("json/net_hero_no_dress.json");
	for (int i = 0; i < size; i++)
	{
		NetHeroNoDress* ptrHND = new NetHeroNoDress();
		FUZHIINT(ptrHND->hid, line[i]["hid"].asInt());
		FUZHIINT(ptrHND->gender, line[i]["gender"].asInt());
		FUZHIINT(ptrHND->fashion1, line[i]["fashion1"].asInt());
		FUZHIINT(ptrHND->fashion2, line[i]["fashion2"].asInt());
		net_hero_no_dress[ptrHND->hid] = ptrHND;
	}
	JSON_READ_END;
}

void ChessData::getNetActiveTask()
{
	JSON_READ_HEAD("json/net_active_task.json");
	for (int i = 0; i < size; i++)
	{
		NetActiveTask* ptrNAT = new NetActiveTask();
		FUZHIINT(ptrNAT->id, line[i]["id"].asInt());
		FUZHIINT(ptrNAT->active_type, line[i]["active_type"].asInt());
		FUZHIINT(ptrNAT->warid, line[i]["warid"].asInt());
		FUZHIINT(ptrNAT->condition, line[i]["condition"].asInt());
		FUZHIINT(ptrNAT->active, line[i]["active"].asInt());
		ptrNAT->itemid = getGoodsMapData(line[i]["itemid"].asCString());
		FUZHIINT(ptrNAT->img, line[i]["img"].asInt());
		FUZHIINT(ptrNAT->explain, line[i]["explain"].asCString());
		FUZHIINT(ptrNAT->nameS, line[i]["name"].asCString());
		net_active_task[ptrNAT->id] = ptrNAT;
	}
	JSON_READ_END;
}

void ChessData::getNetWarGhFb()
{
	JSON_READ_HEAD("json/net_war_ghfb.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_CLANARMYBATTLE;
		ptrInfo->iMapIndex = MAPID_SCENE_CLANARMYBATTLE;
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iImgId, line[i]["img"].asInt());
		FUZHIINT(ptrInfo->iActive, line[i]["active"].asInt());
		FUZHIINT(ptrInfo->iBossHp, line[i]["boss_hp"].asInt());
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		ptrInfo->drop = getGoodsMapData(line[i]["itemid"].asCString());
		FUZHISTRINGEX(ptrInfo->strInfo, line[i]["explain"]);
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		FUZHIINT(ptrInfo->iPawnHp, line[i]["pawn_hp"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		if (line[i]["bossid"].isInt())
			ptrInfo->mBossId[0] = line[i]["bossid"].asInt();
		else if (line[i]["bossid"].isString())
			ptrInfo->mBossId = getSeidData(line[i]["bossid"].asCString());
		FUZHIINT(ptrInfo->iMaxTimes, line[i]["number_battle"].asInt());
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = 1;
	}
	JSON_READ_END;
}

void ChessData::getNetZmzyWar()
{
	JSON_READ_HEAD("json/net_zmzy_war.json");
	for (int i = 0; i < size; i++)
	{
		NetWarInfo* ptrInfo = new NetWarInfo();
		ptrInfo->iType = BATTLE_GREATBATTLE;
		ptrInfo->iMapIndex = MAPID_SCENE_GREATBATTLE;
		FUZHIINT(ptrInfo->iWarId, line[i]["warid"].asInt());
		FUZHISTRINGEX(ptrInfo->strName, line[i]["name"]);
		FUZHIINT(ptrInfo->iMapId, line[i]["map"].asInt());
		ptrInfo->mstrScene = getStringMap(line[i]["scenes"].asCString(), '&');
		FUZHIINT(ptrInfo->iNumLimit, line[i]["number_limit"].asInt());
		FUZHIINTEX(ptrInfo->iGkLv, line[i]["gk_lv"]);
		FUZHIINTEX(ptrInfo->iEnemyLv, line[i]["enemy_lv"]);
		ptrInfo->drop = getGoodsMapData(line[i]["itemid"].asCString());
		FUZHISTRINGEX(ptrInfo->strInfo, line[i]["explain"]);
		m_mNetWarInfo[ptrInfo->iWarId] = ptrInfo;
		map_total_war[ptrInfo->iMapIndex][ptrInfo->iWarId] = 1;
	}
	JSON_READ_END;
}

void ChessData::getNetRatioBuff()
{
	JSON_READ_HEAD("json/dic_ratio_buff.json");
	for (int i = 0; i < size; i++)
	{
		NetRatioBuff* ptrInfo = new NetRatioBuff();
		FUZHIINT(ptrInfo->ratio, line[i]["ratio"].asInt());
		FUZHIINT(ptrInfo->level, line[i]["level"].asInt());
		ptrInfo->seid = getSeidData(line[i]["seid"].asCString());
		net_ratio_buff[ptrInfo->ratio] = ptrInfo;
	}
	JSON_READ_END;
}

void ChessData::getNetZmzyMap()
{
	JSON_READ_HEAD("json/net_zmzy_map.json");
	for (int i = 0; i < size; i++)
	{
		NetZmzyMap* ptrNetZmzyMap = new NetZmzyMap();
		FUZHIINT(ptrNetZmzyMap->id, line[i]["id"].asInt());
		FUZHISTRINGEX(ptrNetZmzyMap->name, line[i]["name"]);
		FUZHIINT(ptrNetZmzyMap->bgimg, line[i]["bgimg"].asInt());
		FUZHIINT(ptrNetZmzyMap->number_battle, line[i]["number_battle"].asInt());
		ptrNetZmzyMap->warid = getSeidData(line[i]["warid"].asCString());
		ptrNetZmzyMap->itemid = getSeidData(line[i]["itemid"].asCString());
		FUZHISTRINGEX(ptrNetZmzyMap->explain, line[i]["explain"]);
		net_zmzy_map[ptrNetZmzyMap->id] = ptrNetZmzyMap;
	}
	JSON_READ_END;
}

void ChessData::CopyActor2toActor1(ActorAttr* ptrActor1, ActorAttr* ptrActor2)
{
	ptrActor1->face_id = ptrActor2->face_id;
	ptrActor1->occupation_id = ptrActor2->occupation_id;
	ptrActor1->occupation_id = ptrActor2->occupation_id;
	killword[ptrActor1->actor_id] = killword[ptrActor2->actor_id];
	ptrActor1->power = ptrActor2->power;
	ptrActor1->defence = ptrActor2->defence;
	ptrActor1->intellegence = ptrActor2->intellegence;
	ptrActor1->explosion = ptrActor2->explosion;
	ptrActor1->fortune = ptrActor2->fortune;

	ptrActor1->level = ptrActor2->level;
	ptrActor1->hp = ptrActor2->hp;
	ptrActor1->mp = ptrActor2->mp;
	ptrActor1->skill = ptrActor2->skill;

	ptrActor1->actor_quality = ptrActor2->actor_quality;
	ptrActor1->qid = ptrActor2->qid;
	ptrActor1->job_img = ptrActor2->job_img;

	//memset(ptrActor1->actor_name, 0, sizeof(ptrActor1->actor_name));
	memset(ptrActor1->actor_info, 0, sizeof(ptrActor1->actor_info));
	memset(ptrActor1->rouse_word, 0, sizeof(ptrActor1->rouse_word));
	memset(ptrActor1->retreat_word, 0, sizeof(ptrActor1->retreat_word));

	//FUZHISTRING(ptrActor1->actor_name, ptrActor2->actor_name);
	FUZHISTRING(ptrActor1->actor_info, ptrActor2->actor_info);
	FUZHISTRING(ptrActor1->rouse_word, ptrActor2->rouse_word);
	FUZHISTRING(ptrActor1->retreat_word, ptrActor2->retreat_word);

	ptrActor1->actor_grade = ptrActor2->actor_grade;
	ptrActor1->image_id = ptrActor2->image_id;
	ptrActor1->image_fight_id = ptrActor2->image_fight_id;
	ptrActor1->tujian_id = ptrActor2->tujian_id;
	ptrActor1->iGender = ptrActor2->iGender;

	trData->actor_turn[ptrActor1->actor_id].turn = trData->actor_turn[ptrActor2->actor_id].turn;

	delete ptrActor1->seid;
	ptrActor1->seid = new unsigned short[ptrActor2->seid_num];

	for (int i = 0; i < ptrActor2->seid_num; i++)
	{
		ptrActor1->seid[i] = ptrActor2->seid[i];
	}
	ptrActor1->seid_num = ptrActor2->seid_num;
}

NetJadeCharge* ChessData::getJadeCharge(int iPrice)
{
	for (map<int, NetJadeCharge*>::iterator it = net_jade_charge.begin(); it != net_jade_charge.end(); it++)
	{
		NetJadeCharge* ptrJade = it->second;
		if (ptrJade->mRmb == iPrice)
		{
			return ptrJade;
		}
	}
	return nullptr;
}

int ChessData::getMakeGoodsId(int iMaterial)
{
	int iGoodsId = 0;
	for(map<int, NetMergeData*>::iterator it = trData->m_mMergeData.begin(); it != trData->m_mMergeData.end(); it++)
	{
		NetMergeData* ptrGoods = it->second;
		if (ptrGoods->mMaterial.begin()->first == iMaterial)
		{
			iGoodsId = ptrGoods->iGoodsId;
			break;
		}
	}
	return iGoodsId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define JSON_GET_OPT_INT(A,B) if(B.isInt()){A=B.asInt();}
#define JSON_GET_OPT_INT2(A,B) if(B.isInt()){A=B.asInt()-1;}
#define JSON_GET_OPT_BOOL(A,B) if(B.isBool()){A=B.asBool();}
#define JSON_GET_OPT_CSTRING(A,B) if(B.isString()){A=B.asCString();}

ModConst::ModConst()
{
	NEW_MAX_FRIEND_ARMY_NUM = 0;
	NEW_MAX_ENEMY_ARMY_NUM = 0;
	NEW_MAX_SKILL_NUM = 100;
	m_iSkillImg = 0;
    beginning_city=0;
	mIsTestService = false;
	OpenIosAliPay = false;
	m_bGuideSwitch = false;
	m_bRestoreClose = false;
}

void ModConst::loadConfig()
{
	JSON_READ_HEAD("json/const.json");
	for (int i = 0; i < size && i < 1; i++)
	{
		JSON_GET_OPT_INT(NEW_MAX_FRIEND_ARMY_NUM, line[i]["MAX_FRIEND_ARMY_NUM"]);
		JSON_GET_OPT_INT(NEW_MAX_ENEMY_ARMY_NUM, line[i]["MAX_ENEMY_ARMY_NUM"]);
		JSON_GET_OPT_INT(NEW_MAX_SKILL_NUM, line[i]["MAX_SKILL_NUM"]);
        JSON_GET_OPT_INT(beginning_city, line[i]["beginning_city"]);
		JSON_GET_OPT_BOOL(mIsTestService, line[i]["IsTestService"]);
		JSON_GET_OPT_BOOL(OpenIosAliPay, line[i]["OpenIosAliPay"]);
		JSON_GET_OPT_BOOL(m_bGuideSwitch, line[i]["GuideSwitch"]);
		JSON_GET_OPT_BOOL(m_bRestoreClose, line[i]["RestoreClose"]);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		mIsTestService = true;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		if(funConst->getChannelId() == "IOSZhengShiBan") //APPStore版本关闭第三方支付
		{
			OpenIosAliPay = false;
		}
#endif

	}
	JSON_READ_END;
}

void ModConst::InitConfig()
{
	for(int i = 0; i < trData->skill_num; i++)
	{
		FUZHI2MAX(trData->skills[i].image_id, m_iSkillImg);
	}

	FUZHIMAX(trData->skill_num, NEW_MAX_SKILL_NUM);
}

////////////////////////////////////////////////////////////////////////////////////////////////
FunctionConst::FunctionConst()
{
	GameName = "";
	GamePath = "";
	EnableTalkingData = false;
	TalkingDataAppId = "";
	TalkingDataChannelId = "";
	EncryptResource = false;
	ShowFrame = false;
	strPackageName = "";
	strWXAppid = "";
	strWXSecret = "";
	strQQAppid = "";
}

void FunctionConst::loadConfig()
{
	DString strBuffer = "";
	{
		char* sData = ChessDirectReadFile("/function.txt", 0, 1);
		if (sData == NULL)
		{
			fprintf(stderr, "function.txt 数据读取异常, 即将退出");
			exit(1);
		}
		strBuffer = sData;
		SAFE_DELETE_ARRAY(sData);
	}
	Json::Reader reader; 
	Json::Value item; 
	if (!reader.parse(strBuffer->getChar(), item, false))
	{
        fprintf(stderr, "function.txt JSON解析异常, 即将退出");
        exit(1);
	}
	JSON_GET_OPT_CSTRING(GameName, item["GameName"]);
	JSON_GET_OPT_CSTRING(GamePath, item["GamePath"]);
	JSON_GET_OPT_BOOL(EnableTalkingData, item["EnableTalkingData"]);
	JSON_GET_OPT_CSTRING(TalkingDataAppId, item["TalkingDataAppId"]);
	JSON_GET_OPT_CSTRING(TalkingDataChannelId, item["TalkingDataChannelId"]);
	JSON_GET_OPT_BOOL(EncryptResource, item["EncryptResource"]);
	JSON_GET_OPT_BOOL(ShowFrame, item["ShowFrame"]);
	JSON_GET_OPT_CSTRING(strPackageName, item["PackageName"]);
	JSON_GET_OPT_CSTRING(strWXAppid, item["WXAPPID"]);
	JSON_GET_OPT_CSTRING(strWXSecret, item["WXSecret"]);
	JSON_GET_OPT_CSTRING(strQQAppid, item["QQAPPID"]);
}

DString FunctionConst::getChannelId()
{
	return TalkingDataChannelId->getChar();
}

int SCREEN_SIZE_X = 640;
int SCREEN_SIZE_Y = 400;

void FunctionConst::initInstance()
{
	if (funConst == NULL)
	{
		funConst = new FunctionConst();
		funConst->loadConfig();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

#if (CC_TARGET_PLATFORM==CC_PLATFORM_WIN32)

Win32Const* win32Const = NULL;

void Win32Const::loadConfig()
{
	DString strBuffer = "";
	{
		char* sData = ChessDirectReadFile("/win32.txt", 0, 1);
		if (sData == NULL)
		{
			return;
		}
		strBuffer = sData;
		SAFE_DELETE_ARRAY(sData);
	}
	Json::Reader reader;
	Json::Value item;
	if (!reader.parse(strBuffer->getChar(), item, false))
	{
		return;
	}

	downloadJson = 0;
	packageName = "com.bantu.zqtxmini";
	versionName = "1.8.00";
	platform = "win32";
	yiDaoMiao = 0;
	mpEmpty = 0;
	iSingleSeckill = 0;
	iNolimitMove = 0;
	iMapElemNum = 0;

	JSON_GET_OPT_INT(downloadJson, item["IS_FUWUQI_TONGBU"]);
	JSON_GET_OPT_CSTRING(packageName, item["BAOMING"]);
	JSON_GET_OPT_CSTRING(versionName, item["BANBENHAO"]);
	JSON_GET_OPT_CSTRING(platform, item["PINGTAI"]);
	JSON_GET_OPT_INT(yiDaoMiao, item["IS_YIDAOMIAO"]);
	JSON_GET_OPT_INT(mpEmpty, item["IS_MPNULL"]);
	JSON_GET_OPT_INT(iSingleSeckill, item["SingleSeckill"]);
	JSON_GET_OPT_INT(iNolimitMove, item["NolimitMove"]);
	JSON_GET_OPT_INT(iMapElemNum, item["MapElemNum"]);
	JSON_GET_OPT_INT(ProtagonistLv, item["ProtagonistLv"]);
}

void Win32Const::initInstance()
{
	if (win32Const == NULL)
	{
		win32Const = new Win32Const();
		win32Const->loadConfig();
	}
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////


bool SkillAttr::BeingAbleToCastSpells(HistoryActor* curActor)
{
	bool isenable = true;

	int temp_mp_consume = GetMpConsume(curActor);
	int temp_hp_consume = 0;

    if(skill_limit == 31) // 31表示此法术是 自发技，用户不能直接点击使用，只能被动的释放使用
        return false;

	if (curActor->GetCurMp() < temp_mp_consume&&skill_limit != 10) {
		isenable = false;
	}
	else if (skill_limit == 1) {	//hp限制
		if (curActor->GetCurMp() > curActor->GetBaseMp())//该方案暂定为冥想时的标准
		{
			temp_hp_consume = curActor->GetBaseMp() - curActor->GetCurMp();
		}
		else
		{
			temp_hp_consume = 24;
		}
		if (temp_hp_consume >= curActor->GetCurHp())
			isenable = false;
	}
	else if (skill_limit == 2)//装备限制且仅恶劣天气
	{
		if (!curActor->good_call_myth)
			isenable = false;
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_CLOUDY:
			case AIR_TYPE_SUN:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 3)// 装备限制且仅晴天
	{
		if (!curActor->good_call_myth)
			isenable = false;
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_CLOUDY:
			case AIR_TYPE_RAIN:
			case AIR_TYPE_WIND:
			case AIR_TYPE_SNOW:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 4)// 装备限制且仅阴天
	{
		if (!curActor->good_call_myth)
			isenable = false;
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_SUN:
			case AIR_TYPE_RAIN:
			case AIR_TYPE_WIND:
			case AIR_TYPE_SNOW:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 5)// 装备限制
	{
		if (!curActor->good_call_myth)
			isenable = false;
	}
	else if (skill_limit == 6)//天气限制
	{
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_RAIN:
			case AIR_TYPE_WIND:
			case AIR_TYPE_SNOW:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 7)//仅恶劣天气
	{
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_CLOUDY:
			case AIR_TYPE_SUN:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 8)//阴天无效
	{
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_CLOUDY:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 9)//晴天无效
	{
		if (!curActor->good_magic_no_air) {
			switch (history->m_iCurAir) {
			case AIR_TYPE_SUN:
				isenable = false;
				break;
			default:
				break;

			}
		}
	}
	else if (skill_limit == 10)//金钱限制
	{
		if (curActor->relation < 2 && history->money < temp_mp_consume)
			isenable = false;
	}
	else if (skill_limit == 11)//每关限用一次
	{
		if (curActor->used_skill_ids[skill_id] == 1) {
			isenable = false;
		}
	}
	else if (skill_limit == 12) {
		isenable = false;
	}
	else if (skill_limit >= 13 && skill_limit <= 29) {
		isenable = false;
	}
	else if (skill_limit == 30)//消耗品限制
	{
		isenable = false;
	}

	if (seid == 42 && history->FindActor(power - 1) == NULL)//召唤人物法术，召唤的人物找不到时禁止释放
	{
		isenable = false;
	}

	if ((seid == 124|| iTimes) && curActor->mSkillUseNumLimt.count(skill_id) && curActor->mSkillUseNumLimt[skill_id] > (iTimes>0?iTimes - 1:0))
	{
		isenable = false;
	}
	
	HistoryActorPtr oppoActor = NULL;
	if (seid == 126 && isenable &&power > 900 && power < 904 && (oppoActor = history->FindActor(power - 1)) && oppoActor->hide == 0)
	{
		isenable = false;
	}

	if (history->var2[5009] == 1)//某种难度下无法使用这些法术
	{
		if (LimitUseSkill())
			isenable = false;
	}
	return isenable;
}
int SkillAttr::LimitUseSkill()
{
	if ((seid >= 4 && seid <= 7) || (seid >= 17 && seid <= 20) || seid == 23)
		return 1;
	return 0;
}

int SkillAttr::GetMpConsume(HistoryActor* curActor,int iIsShow)
{
	int iMpCon = mp_consume;

	if (mp_consume >= 9999)//消耗当前所有mp
	{
		if (history->game_state == GAME_STATE_BATTLE&&!iIsShow)
			return curActor->GetCurMp();
		else
			return 9999;
	}

	int mSkillId = skill_id + 1;
	int skill_level = (curActor->pursuit_skill_lv > 0 ? curActor->pursuit_skill_lv : curActor->GetCurSkillLevel(mSkillId)) - 1;
	if(skill_level > 0)
		iMpCon += mp_consume*skill_level * trData->net_skill_upgrade[mSkillId]->up_mp_consume / 100;

	if (curActor->good_retrench_MP)
	{
		iMpCon = iMpCon * (100 - curActor->good_retrench_MP) / 100;
	}

	return iMpCon;
}
float SkillAttr::GetExtraGainHp(HistoryActor* curActor,int iType)
{
	float iHpCon = 0;
	int mSkillId = skill_id + 1;
	if (curActor->CurSkillLevelIsNull(mSkillId) == false || curActor->pursuit_skill_lv > 0)
	{
		int skill_level = (curActor->pursuit_skill_lv > 0 ? curActor->pursuit_skill_lv:curActor->GetCurSkillLevel(mSkillId)) - 1;
		if (skill_level>0 && iType == 1)
			iHpCon += skill_level * trData->net_skill_upgrade[mSkillId]->up_hurt_num;
		else if (skill_level>0 && iType == 2)
			iHpCon += skill_level * trData->net_skill_upgrade[mSkillId]->up_prop_num;
		else if (skill_level>0 && iType == 3)
			iHpCon += skill_level * trData->net_skill_upgrade[mSkillId]->up_prop_per;
	}

	return iHpCon;
}

int SkillAttr::GetCasterActorId(HistoryActor* curActor)
{
	int iRet = 0;
	if (seid == 118)
		iRet = curActor->actor_id + 1;
	return iRet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* ActorAttr::getActorName()
{
	string namebuff = actor_name;
	bool is_Special = false;
	if (actor_id == 0)
	{
		namebuff = history->m_pPlayerInfo->getName();
		is_Special = true;
	}

	if (history->game_state != GAME_STATE_REDE&&history->m_iBattleType == BATTLE_LADDER)
	{
		string LadderName = BattleLadder::getInstance()->GetBattleActorName(actor_id + 1, 1);
		if (!LadderName.empty())
		{
			namebuff = LadderName;
			is_Special = true;
		}
	}

	strcpy(actorname_nonum, namebuff.c_str());
	if (!is_Special)
	{
		int i;
		int length = strlen(actorname_nonum) + 1;
		for (i = 0; i < length; i++)
		{
			if (actorname_nonum[i] <= '9'&&actorname_nonum[i] >= '0')
			{
				actorname_nonum[i] = 0;
				break;
			}
		}
	}
	return actorname_nonum;
}
