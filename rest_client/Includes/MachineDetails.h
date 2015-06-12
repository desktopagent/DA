/**
*******************************************************************************
* @file    MachineDetails.h
* @author  Mindtree
* @par
* @brief   This file contains the System info like MAC Id, Computer Name etc..of the class MachineDetails.
* @date    2015-05-11
*******************************************************************************
*/

#ifndef MACHINE_CONFIG_H
#define MACHINE_CONFIG_H

#include <typeinfo.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include <string>

using namespace std;

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))



class MachineDetails
{
public:
	MachineDetails();
	~MachineDetails();


	wstring getMachineId();
	wstring getHostName();
	wstring getOperatingSystemVer();
	wstring getIpAddress();

private: 
	BOOL EqualsMajorVersion(DWORD majorVersion);
	BOOL EqualsMinorVersion(DWORD minorVersion);

	string machineId;
	string machineName;
};

#endif