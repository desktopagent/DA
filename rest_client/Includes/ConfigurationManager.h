#pragma once
#include "AgentDetails.h"
#include "MachineDetails.h"
#include "UserDetails.h"
#include "RegisterRequest.h"
#include "StringUtil.h"
#include <cpprest/json.h>
#include "DeviceInfo.h"

class FileManager
{
public:
	FileManager();
	~FileManager();
public:
	/*fstream fs;*/
	void Open(string strLogFileName);
	void Write(value writeData);
	wstring GetFileData();
	void Read();
	void Close();
private:
	wstring fileData;
	wfstream fs;
};

class ConfigurationManager
{
public:
	ConfigurationManager(ConfigurationManager &oConfigurationManager);
	virtual ~ConfigurationManager();
	static ConfigurationManager& getInstance(); //make this static

	UserInfo getUserInfo();
	MachineInfo getMachineInfo();
	AgentInfo getAgentInfo();
	value getConfigurationData();
	void saveParams(value inputDate);
	
	value getParam(const wstring& key);

	string geFilePath();

	DeviceInfo getDeviceInfo();

	
private:
	ConfigurationManager();

	ConfigurationManager operator= (ConfigurationManager &oConfigurationManager);
	FileManager fileManager;
 	CAgentDetails agentDetails;
	MachineDetails machineDetails;
	UserDetails userDetails;
	string strPDSFolder;
	string strPDSDirectory;
	value jsonFileData;
	//ConfigurationManager oconfigurationManager;
};


