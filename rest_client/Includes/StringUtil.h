#pragma once
#include <string>
using namespace std;

class StringUtil
{
public:
	static string wstringTostring(const wstring &wstr);
	static wstring stringTowstring(const string& str);
	static bool isEmpty(const wstring& str);
	static wstring stripQuotes(const wstring& str);
};

