#pragma once

#include <exception>

using namespace std;

class CThreadPoolException : public exception
{
public:
	CThreadPoolException(char*);
};