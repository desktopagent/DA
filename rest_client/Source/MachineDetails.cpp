/**
*******************************************************************************
* @file    MachineDetails.cpp
* @author  Mindtree
* @par
* @brief   This file contains the definition of the methods defined in the
*          User Configuration Impl class MachineDetails.
* @date    2015-05-11
*******************************************************************************
*/

#include "MachineDetails.h"
#include "StringUtil.h"

//******************************************************************************	

MachineDetails::MachineDetails()
{ 	
	
}

MachineDetails::~MachineDetails()
{ 

}


wstring MachineDetails::getMachineId()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	DWORD dwBufLen = sizeof(PIP_ADAPTER_INFO);
	wchar_t szBuffer[256];
	wstring macAddress;

	GetAdaptersInfo(NULL, &dwBufLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
	ULONG result = GetAdaptersInfo(pAdapterInfo, &dwBufLen);


	if (result != NO_ERROR)  {
		free(pAdapterInfo);
		//throw exception
	}

	do {
		wsprintf(szBuffer, L"%02X:%02X:%02X:%02X:%02X:%02X",
			pAdapterInfo->Address[0], pAdapterInfo->Address[1],
			pAdapterInfo->Address[2], pAdapterInfo->Address[3],
			pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

		macAddress += wstring(szBuffer) + L",";

		pAdapterInfo = pAdapterInfo->Next;
	} while (pAdapterInfo);


	free(pAdapterInfo);
	return macAddress;
}




wstring MachineDetails::getHostName()
{
	wchar_t hostName[MAX_PATH];
	DWORD len = MAX_PATH;
	if (!GetComputerNameW(hostName, &len)) {
		//throw exception;
	}

	return wstring(hostName, len);
}

wstring MachineDetails::getIpAddress()
{
	PIP_ADAPTER_INFO pAdapterInfo;
	DWORD dwBufLen = sizeof(PIP_ADAPTER_INFO);

	string ipAddress;

	GetAdaptersInfo(NULL, &dwBufLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(dwBufLen);
	ULONG result = GetAdaptersInfo(pAdapterInfo, &dwBufLen);

	if (result != NO_ERROR)  {
		free(pAdapterInfo);
		//throw exception
	}

	do {
		string str = pAdapterInfo->IpAddressList.IpAddress.String;
		if (str != "0.0.0.0")
		{
			ipAddress += str;
		}
		pAdapterInfo = pAdapterInfo->Next;
	} while (pAdapterInfo);


	free(pAdapterInfo);
	return StringUtil::stringTowstring( ipAddress );
}

wstring MachineDetails::getOperatingSystemVer()
{
	wstring rslt;
	if (EqualsMajorVersion(6) && EqualsMinorVersion(1))
		rslt = L"7.0.0.0";
	else if (EqualsMajorVersion(6) && EqualsMinorVersion(2))
		rslt = L"8.0.0.0";
	else if (EqualsMajorVersion(6) && EqualsMinorVersion(3))
		rslt = L"8.1.0.0";

	return rslt;

}
  
BOOL MachineDetails :: EqualsMajorVersion(DWORD majorVersion)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.dwMajorVersion = majorVersion;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_MAJORVERSION, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_MAJORVERSION, maskCondition);
}
BOOL MachineDetails :: EqualsMinorVersion(DWORD minorVersion)
{
	OSVERSIONINFOEX osVersionInfo;
	::ZeroMemory(&osVersionInfo, sizeof(OSVERSIONINFOEX));
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	osVersionInfo.dwMinorVersion = minorVersion;
	ULONGLONG maskCondition = ::VerSetConditionMask(0, VER_MINORVERSION, VER_EQUAL);
	return ::VerifyVersionInfo(&osVersionInfo, VER_MINORVERSION, maskCondition);
}

