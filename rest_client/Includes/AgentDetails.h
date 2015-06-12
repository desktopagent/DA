/**
*******************************************************************************
* @file    AgentDetails.h
* @author  Mindtree
* @par
* @brief   This file contains the Agent Information  information of the class AgentDetails.
* @date    2015-05-116
*******************************************************************************
*/

#ifndef AGENT_INFO_H
#define AGENT_INFO_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Assert.h>
#include <string>
#include <fstream>
#include <MachineDetails.h>
#include <UserDetails.h>

using namespace std;

class CAgentDetails
{
public:
	CAgentDetails();
	CAgentDetails(const MachineDetails& machineInfoObj);
	~CAgentDetails();

	/***********************************************************************************************
	* @brief   Get the salt value(string) and it takes Macid and User Name as input Argument.
	************************************************************************************************/
	string salt(const string& macId, const string& userName);

	/***********************************************************************************************
	* @brief   Generate the Agent Id by combination of Mac ID and salt value.
	************************************************************************************************/
	wstring getAgentId();
	
	/***********************************************************************************************
	* @brief   Convert hex vale to string
	************************************************************************************************/
	string hexToString(const string& s);

	/***********************************************************************************************
	* @brief   Generate the key value to use in salt() Function.
	************************************************************************************************/
	void genKey(const string& param, unsigned char* key);

	/***********************************************************************************************
	* @brief  String to Hex Conversation
	************************************************************************************************/
	string stringToHex(const char* s, const int n);	

	char hexToInt(char h);

	char intToHex(unsigned char i);

	MachineDetails machineDetails;
private:
	string agentId;
	string macId;
};
#endif;