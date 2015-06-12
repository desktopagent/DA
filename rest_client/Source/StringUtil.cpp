#include "StringUtil.h"
#include "windows.h"


wstring StringUtil::stringTowstring(const string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
	//return NULL;
}

bool StringUtil::isEmpty(const wstring& str) {
	return (str.empty() || !str.compare(L"null"));
}

wstring StringUtil::stripQuotes(const wstring& str) {

	size_t start = 0, end = str.length();

	if (str[0] == L'"') start = 1;
	if (str[str.length() - 1] == L'"')	end = str.length() - 2;

	return str.substr(start, end);
}

//conversion of LPCWSTR to std::string
string StringUtil::wstringTostring(const wstring &wstr)
{
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}


