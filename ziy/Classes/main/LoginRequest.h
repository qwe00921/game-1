//
//  LoginRequest.h
//  TRGame
//
//  Created by Jimmy on 16/04/16.
//
//

#ifndef _LOGINREQUEST_H_
#define _LOGINREQUEST_H_

#include "Defines.h"

enum
{
	LOGIN_NONE = 0,
	LOGIN_USER,
	LOGIN_GUEST,
	LOGIN_WX,
	LOGIN_QQ,
	LOGIN_UC,
	LOGIN_AM
};

class LoginElem
{
public:
	LoginElem()
	{
		m_iId = LOGIN_NONE;
		m_strType = "";
		m_strUser = "";
		m_strPass = "";
		m_strOpen = "";
	}

	CC_SYNTHESIZE(int, m_iId, TypeId); //类型ID
	CC_SYNTHESIZE(string, m_strType, TypeName);//类型名称
	CC_SYNTHESIZE(string, m_strUser, UserName);//用户名
	CC_SYNTHESIZE(string, m_strPass, Password);//用户密码
	CC_SYNTHESIZE(string, m_strOpen, OpenId);//OpenID
};

class LoginInfo
{
public:
	LoginInfo();
	~LoginInfo();
	bool loadLogin();
	bool saveLogin();
	LoginElem* getLoginElem(int iTypeId);
	void addLoginElem(int iTypeId, string strType, string strUser, string strPass, string strOpen);
	void removeLoginElem(int iTypeId);
	int getHeroicOpen(int iWarId);
	void setHeroicOpen(int iWarId);
private:
	bool parseJson(Json::Value jsonVal);
	Json::Value saveJson();
public:
	map<int, LoginElem*> m_mLogInfo;
	map<int, int> m_mHeroicOpen;
	CC_SYNTHESIZE(int, m_iLoginId, LoginId);
	CC_SYNTHESIZE(int, m_iMark, MarkCode);
	CC_SYNTHESIZE(string, m_strToken, GameToken);
};


#endif //_LOGINREQUEST_H_