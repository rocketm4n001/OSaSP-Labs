#include "pch.h"
#include <sstream>
#include <string>
#include "Address.h"
#include <vector>
#include <fstream>
#include <windows.h>
#pragma warning(disable : 4251)

std::string dbPath = "D:\\Education\\Operation Systems and System Programming\\5 Lab\\phoneCatalog\\Debug\\test.txt";
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

 node* surnameIndexTreeCurrNode;

 node* streetIndexTreeCurrNode;

 node* telIndexTreeCurrNode;

__declspec(dllexport) node* getSurnameIndex() {
	return surnameIndexTreeCurrNode;
}

__declspec(dllexport) node* getStreetIndex() {
	return streetIndexTreeCurrNode;
}

__declspec(dllexport) node* getTelIndex() {
	return telIndexTreeCurrNode;
}

 unsigned char height(node* p)
{
	return p ? p->height : 0;
}

 int bfactor(node* p)
{
	return height(p->right) - height(p->left);
}

 void fixheight(node* p)
{
	unsigned char hl = height(p->left);
	unsigned char hr = height(p->right);
	p->height = (hl > hr ? hl : hr) + 1;
}

 node* rotateright(node* p)
{
	node* q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

 node* rotateleft(node* q)
{
	node* p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

 node* balance(node* p)
{
	fixheight(p);
	if (bfactor(p) == 2)
	{
		if (bfactor(p->right) < 0)
			p->right = rotateright(p->right);
		return rotateleft(p);
	}
	if (bfactor(p) == -2)
	{
		if (bfactor(p->left) > 0)
			p->left = rotateleft(p->left);
		return rotateright(p);
	}
	return p;
}

__declspec(dllexport) int hashcode(std::string s) {
	long seed = 31;
	long hash = 0;
	for (int i = 0; i < s.length(); i++) {
		hash = (hash * seed) + s[i];
	}
	return hash % 10007;
};

 node* insert(node* p, int k, Address* adr)
{
	if (!p) {
		node* n = new node(k);
		n->adr = adr;
		return n;
	}
	if (k <= p->key)
		p->left = insert(p->left, k, adr);
	else
		p->right = insert(p->right, k, adr);
	return balance(p);
}

std::vector<std::string> explode(std::string& s, char delim)
{
	if (s[0] != '1') {
		s = std::string("1 " + s);
	}
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

bool isMathcedRecord(std::vector<std::string> adr, std::vector<int> indexes, std::vector<std::string> values) {
	bool isMatched = true;
	for (int i = 0; i < indexes.size(); i++) {
		if (adr[indexes[i]] != values[i]) {
			isMatched = false;
			break;
		}
	}
	return isMatched;
}

DWORD fOffsetHigh = 0;
DWORD fOffsetLow = 0;
SIZE_T fBufferSize = 255;
std::string buffStr = "";
std::vector<Address*> records;
std::vector<std::string> values;
std::vector<int> indexes;

void addAddress(std::vector<std::string> data) {
	Address* adr = new Address(
		data[1],
		data[2],
		data[3],
		data[4],
		data[5],
		atoi(data[6].c_str()),
		atoi(data[7].c_str()),
		atoi(data[8].c_str())
	);
	records.push_back(adr);
}

void ReadInChunks(const char* pszFileName, bool isSeaerchMode, int page) {
	records = std::vector<Address*>();
	SYSTEM_INFO sysinfo = { 0 };
	::GetSystemInfo(&sysinfo);
	DWORD cbView = sysinfo.dwAllocationGranularity;

	HANDLE hfile = CreateFileA(pszFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, 0, NULL);
	if (hfile != INVALID_HANDLE_VALUE) {
		LARGE_INTEGER file_size = { 0 };
		::GetFileSizeEx(hfile, &file_size);
		const unsigned long long cbFile =
			static_cast<unsigned long long>(file_size.QuadPart);

		HANDLE hmap = CreateFileMappingA(hfile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (hmap != NULL) {
			int currentPage = 0;
			for (unsigned long long offset = 0; offset < cbFile; offset += cbView) {
				DWORD high = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFFul);
				DWORD low = static_cast<DWORD>(offset & 0xFFFFFFFFul);
				if (offset + cbView > cbFile) {
					cbView = static_cast<int>(cbFile - offset);
				}
				if (page == currentPage) {
					const char* pView = static_cast<const char*>(
						MapViewOfFile(hmap, FILE_MAP_READ, high, low, cbView));
					if (pView != NULL) {
						char test;
						for (int i = 0; i < cbView; i++) {
							test = pView[i];
							if (test == '\n' || test == '\r') {
								if (buffStr != "") {
									std::vector<std::string> parsedAddress = explode(buffStr, ' ');
									if (isSeaerchMode) {
										if (isMathcedRecord(parsedAddress, indexes, values)) {
											addAddress(parsedAddress);
										}
									}
									else {
										addAddress(parsedAddress);
									}
								}
								buffStr = "";
								continue;
							}

							buffStr += test;
						}
						std::vector<std::string> tempParsedAddress = explode(buffStr, ' ');
						if (tempParsedAddress.size() == 9) {
							if (isSeaerchMode) {
								if (isMathcedRecord(tempParsedAddress, indexes, values)) {
									addAddress(tempParsedAddress);
								}
							}
							else {
								addAddress(tempParsedAddress);
							}
							buffStr = "";
						}
					}
				}
				currentPage++;
			}
			CloseHandle(hmap);
		}
		CloseHandle(hfile);
	}
}

__declspec(dllexport) std::vector<Address*> loadDB(int cP) {
	ReadInChunks(dbPath.c_str(), false, cP);
	
	return records;
}

__declspec(dllexport) std::vector<Address*> searchAddresses(std::vector<int> _indexes, std::vector<std::string> _values) {
	indexes = _indexes;
	values = _values;
	ReadInChunks(dbPath.c_str(), true, 0);

	return records;
}