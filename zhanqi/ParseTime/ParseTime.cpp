// ParseTime.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DString.h"
#include "iostream"
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	int iNum = 0;
	int iAccontId = 0;
	cout << "请输入需要查询的号码：";
	cin >> iNum;
	cout << "请输入TRGAME帐号（仅数字，不需要TR）：";
	cin >> iAccontId;
	cout << endl;

	int iTime = iNum ^ iAccontId;
	DString strTime = IntToString(iTime);
	strTime = strTime->reverse();

	cout << "Time : " << strTime->getNativeCharPtr() << endl;
	system("PAUSE");
	return 0;
}

