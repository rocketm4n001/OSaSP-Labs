#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "misc.h"

using namespace std;

Address::Address(string szPhone, string szSurname, string szName, string szPatronymic, string szStreet, int iHome, int iHousing, int iFlat)
{
	_szPhone = szPhone;
	_szSurname = szSurname;
	_szName = szName;
	_szPatronymic = szPatronymic;
	_szStreet = szStreet;
	_iHome = iHome;
	_iHousing = iHousing;
	_iFlat = iFlat;
}

LPWSTR ConvertToLPWSTR(const std::string& s)
{
	LPWSTR ws = new wchar_t[s.size() + 1];
	
	copy(s.begin(), s.end(), ws);
	
	ws[s.size()] = 0;

	return ws;
}

std::string IntToStr(int x) 
{
	std::stringstream ss;
	
	ss << x;
	
	std::string str = ss.str();
	
	return str;
}