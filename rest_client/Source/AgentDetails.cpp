/**
*******************************************************************************
* @file    AgentDetails.cpp
* @author  Mindtree
* @par
* @brief   This file contains the definition of the methods defined in the
*          Agent Infornation in class AgentDetails.
* @date    2015-05-16
*******************************************************************************
*/

#include "AgentDetails.h"	
#include <algorithm>
#include "StringUtil.h"
#pragma comment(lib, "iphlpapi.lib")


//******************************************************************************

CAgentDetails::CAgentDetails()
{	

}
CAgentDetails::CAgentDetails(const MachineDetails& machineDetailsObj)
{
	machineDetails = machineDetailsObj;
}

CAgentDetails::~CAgentDetails()
{
}

string CAgentDetails :: salt(const string& value, const string& param)
{
	string rslt;
	string src = hexToString(value);
	int len = min(src.length(), (size_t)16);
	char* buf = new char[16];
	try{
		memset(buf, 0, 16);	
		memcpy_s(buf, buf[16], src.data(), len);
		unsigned char saltValue[16];		
		try
		{
			genKey(param, saltValue);
			for (int i = 0; i < 16; ++i)
				buf[i] ^= saltValue[i];

			rslt = stringToHex(buf, 16);
		}
		catch (const runtime_error&)
		{
			delete[] buf;
			return rslt;
		}
	}
	catch (...)
	{
		delete[] buf;
		return rslt;
	}
	delete[] buf;
	return rslt;
}

wstring CAgentDetails::getAgentId()
{
	string str = "";
	locale loc;
	try
	{
		wstring macId = machineDetails.getMachineId();
		char *szMachineId = new char[macId.length()];
		wcstombs(szMachineId, macId.c_str(), macId.length());
		str = string(szMachineId, macId.length());
		delete[] szMachineId;

		agentId = salt(str, "");
	}
	catch (const exception& ex)
	{
		ex.what();
	}
	return StringUtil::stringTowstring(agentId);
}

string CAgentDetails :: hexToString(const string& s)
{
	string rslt;

	for (size_t i = 0; i<s.length(); ++i)
	{
		if (i % 2)
		{
			char c = (hexToInt(s[i - 1]) << 4) + hexToInt(s[i]);
			rslt += string(1, c);
		}
	}
	return rslt;
}

void CAgentDetails :: genKey(const string& param, unsigned char* key)
{
	
	try{
		key[0] = 0x42;
		key[1] = 0x5d;
		key[2] = 0x6d;
		key[3] = 0x00;
		key[4] = 0xbe;
		key[5] = 0xea;
		key[6] = 0xd1;
		key[7] = 0xe0;
		key[8] = 0xbc;
		key[9] = 0x65;
		key[10] = 0xc4;
		key[11] = 0x27;
		key[12] = 0xc5;
		key[13] = 0x6c;
		key[14] = 0x9d;
		key[15] = 0x5a;

		if (param.length())
		for (int i = 0; i < 16; ++i)
			key[i] ^= tolower(param[i%param.length()]);
	}
	catch (const runtime_error&)
	{
	}
}

string CAgentDetails :: stringToHex(const char* s, const int n)
{
	string rslt;
	for (int i = 0; i<n; ++i)
	{
		rslt += string(1, intToHex((s[i] >> 4) & 0x0f));
		rslt += string(1, intToHex(s[i] & 0x0f));
	}
	return rslt;
}
char CAgentDetails :: intToHex(unsigned char i)
{
	if (i<10)
		return i + '0';
	else
		return (i & 0xff) - 10 + 'a';

}
char CAgentDetails::  hexToInt(char h)
{
	if ((h >= '0') && (h <= '9'))
		return h - '0';
	if ((h >= 'A') && (h <= 'F'))
		return 10 + h - 'A';
	if ((h >= 'a') && (h <= 'f'))
		return 10 + h - 'a';
	return 0;
}
