#pragma once

#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

class Address 
{
public:
	Address(string, string, string, string, string, int, int, int);

	string _szPhone;
	string _szSurname;
	string _szName;
	string _szPatronymic;
	string _szStreet;
	
	int _iHome;
	int _iHousing;
	int _iFlat;
};

const vector<LPCWSTR> g_szColumnsName
{ 
	L"Phone", 
	L"Surname", 
	L"Name", 
	L"Patronymic", 
	L"Street", 
	L"Home", 
	L"Housing", 
	L"Flat" 
};

struct node
{
	int key;
	Address* adr;
	unsigned char height;
	node* left;
	node* right;
	node(int k)
	{
		key = k;
		left = right = 0;
		height = 1;
	}
};

LPWSTR ConvertToLPWSTR(const string&);
string IntToStr(int);

typedef std::vector<Address*>(*ldb_t)();
typedef std::vector<Address*>(*search_t)(int, node*);
typedef int(*hash_t)(std::string);
typedef node*(*phone_t)();
typedef node* (*street_t)();
typedef node* (*surname_t)();