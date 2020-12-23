// pch.cpp: файл исходного кода, соответствующий предварительно скомпилированному заголовочному файлу
#include "pch.h"
#include <sstream>
#include <string>
#include "CInfo.h"
#include <vector>
#include <fstream>

std::string dbPath = "D:\\Education\\Operation Systems and System Programming\\4 Lab\\PhoneDirectory\\Debug\\addresses.txt";

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
 node* PhoneIndexTreeCurrNode;

__declspec(dllexport) node* GetSurnameIndex() 
{
	return surnameIndexTreeCurrNode;
}

__declspec(dllexport) node* GetStreetIndex() 
{
	return streetIndexTreeCurrNode;
}

__declspec(dllexport) node* GetPhoneIndex() 
{
	return PhoneIndexTreeCurrNode;
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

__declspec(dllexport) int HashCode(std::string s) 
{
	long seed = 31;
	long hash = 0;
	for (int i = 0; i < s.length(); i++) 
	{
		hash = (hash * seed) + s[i];
	}
	return hash % 10007;
};

 node* insert(node* p, int k, Address* adr) // вставка ключа k в дерево с корнем p
{
	if (!p) 
	{
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

std::vector<std::string> explode(std::string const& s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

__declspec(dllexport) std::vector<Address*> LoadDataBase() 
{
	std::string line;
	std::vector<Address*> result;
	std::ifstream in(dbPath); // окрываем файл для чтения
	if (in.is_open())
	{
		while (getline(in, line))
		{
			std::vector<std::string> parsedAddress = explode(line, ' ');
			Address* adr = new Address(
				parsedAddress[1],
				parsedAddress[2],
				parsedAddress[3],
				parsedAddress[4],
				parsedAddress[5],
				atoi(parsedAddress[6].c_str()),
				atoi(parsedAddress[7].c_str()),
				atoi(parsedAddress[8].c_str())
			);

			result.push_back(adr);

			surnameIndexTreeCurrNode = insert(surnameIndexTreeCurrNode, HashCode(parsedAddress[2]), adr);

			streetIndexTreeCurrNode = insert(streetIndexTreeCurrNode, HashCode(parsedAddress[5]), adr);

			PhoneIndexTreeCurrNode = insert(PhoneIndexTreeCurrNode, HashCode(parsedAddress[1]), adr);
		}
	}
	in.close();
	return result;
}

__declspec(dllexport) std::vector<Address*> SearchAddresses(int key, node* root) 
{
	node* tempRoot = root;
	std::vector<Address*> addresses;
	while (tempRoot) 
	{
		if (key <= tempRoot->key) 
		{
			if (tempRoot->key == key) 
			{
				addresses.push_back(tempRoot->adr);
			}

			tempRoot = tempRoot->left;
		}
		else if (key > tempRoot->key) 
		{
			tempRoot = tempRoot->right;
		}
	}

	return addresses;
}