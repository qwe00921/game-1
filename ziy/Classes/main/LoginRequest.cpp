//
//  LoginRequest.cpp
//  TRGame
//
//  Created by Jimmy on 16/04/16.
//
//

#include "Common.h"
#include "main.h"
#include "Defines.h"
#include "LoginRequest.h"

#define FILE_LOGIN_INFO "login.dat"

LoginInfo::LoginInfo()
{
	m_iLoginId = LOGIN_NONE;
}

LoginInfo::~LoginInfo()
{

}

bool LoginInfo::loadLogin()
{
	char* sData = ChessReadFile(FILE_LOGIN_INFO, 1);
	if(sData == NULL)
	{
		return false;
	}

	Reader reader;
	Json::Value jsonVal;
	if(!reader.parse(sData, jsonVal, false))
	{
		ChessFree(sData);
		return false;
	}
	ChessFree(sData);
	return parseJson(jsonVal);
}

bool LoginInfo::saveLogin()
{
	FastWriter fastWriter;
	std::string out = fastWriter.write(saveJson());
	return ChessWriteFile(FILE_LOGIN_INFO, (char*)out.c_str(), out.size()) ? true : false;
}

bool LoginInfo::parseJson(Json::Value jsonVal)
{
	m_iLoginId = jsonVal["loginId"].asInt();

	Json::Value loginList = jsonVal["loginList"];
	if(!loginList.isArray())
	{
		return false;
	}
	m_mLogInfo.clear();
	for(UInt i = 0; i < loginList.size(); i++)
	{
		int iCurId = loginList[i]["typeId"].asInt();
		m_mLogInfo[iCurId] = new LoginElem();
		m_mLogInfo[iCurId]->setTypeId(iCurId);
		m_mLogInfo[iCurId]->setTypeName(loginList[i]["typeName"].asString());
		m_mLogInfo[iCurId]->setUserName(loginList[i]["userName"].asString());
		m_mLogInfo[iCurId]->setPassword(loginList[i]["password"].asString());
		m_mLogInfo[iCurId]->setOpenId(loginList[i]["openId"].asString());
	}

	Json::Value heroicList = jsonVal["heroicList"];
	if(!heroicList.isArray())
	{
		return false;
	}
	m_mHeroicOpen.clear();
	for(UInt i = 0; i < heroicList.size(); i++)
	{
		int iWarId = heroicList[i]["warId"].asInt();
		int iOpen = heroicList[i]["isOpen"].asInt();
		m_mHeroicOpen[iWarId] = iOpen;
	}

	return true;
}

Json::Value LoginInfo::saveJson()
{
	Json::Value jsonVal;
	jsonVal["loginId"] = m_iLoginId;

	Json::Value loginList(arrayValue);
	map<int, LoginElem*>::iterator itLogInfo;
	for(itLogInfo = m_mLogInfo.begin(); itLogInfo != m_mLogInfo.end(); itLogInfo++)
	{
		Json::Value jsonInfo;
		jsonInfo["typeId"] = itLogInfo->second->getTypeId();
		jsonInfo["typeName"] = itLogInfo->second->getTypeName();
		jsonInfo["userName"] = itLogInfo->second->getUserName();
		jsonInfo["password"] = itLogInfo->second->getPassword();
		jsonInfo["openId"] = itLogInfo->second->getOpenId();
		loginList.append(jsonInfo);
	}
	jsonVal["loginList"] = loginList;

	Json::Value heroicList(arrayValue);
	for(map<int, int>::iterator it = m_mHeroicOpen.begin(); it != m_mHeroicOpen.end(); it++)
	{
		Json::Value heroic;
		heroic["warId"] = it->first;
		heroic["isOpen"] = it->second;
		heroicList.append(heroic);
	}
	jsonVal["heroicList"] = heroicList;

	return jsonVal;
}

LoginElem* LoginInfo::getLoginElem(int iTypeId)
{
	map<int, LoginElem*>::iterator itLogin = m_mLogInfo.find(iTypeId);
	if(itLogin != m_mLogInfo.end())
	{
		return m_mLogInfo[iTypeId];
	}
	return NULL;
}

void LoginInfo::addLoginElem(int iTypeId, string strType, string strUser, string strPass, string strOpen)
{
	if(m_mLogInfo[iTypeId] == NULL)
	{
		m_mLogInfo[iTypeId] = new LoginElem();
	}

	m_mLogInfo[iTypeId]->setTypeId(iTypeId);
	m_mLogInfo[iTypeId]->setTypeName(strType);
	m_mLogInfo[iTypeId]->setUserName(strUser);
	m_mLogInfo[iTypeId]->setPassword(strPass);
	m_mLogInfo[iTypeId]->setOpenId(strOpen);

	m_iLoginId = iTypeId;
	saveLogin();
}

int LoginInfo::getHeroicOpen(int iWarId)
{
	bool bIsOpen = false;
	if(m_mHeroicOpen.count(iWarId) && m_mHeroicOpen[iWarId])
	{
		bIsOpen = true;
	}
	return bIsOpen;
}

void LoginInfo::setHeroicOpen(int iWarId)
{
	m_mHeroicOpen[iWarId] = 1;
	saveLogin();
}

void LoginInfo::removeLoginElem(int iTypeId)
{
	if (m_mLogInfo.count(iTypeId))
	{
		SAFE_DELETE(m_mLogInfo[iTypeId]);
		m_mLogInfo.erase(iTypeId);
		m_iLoginId = LOGIN_NONE;
		saveLogin();
	}
}