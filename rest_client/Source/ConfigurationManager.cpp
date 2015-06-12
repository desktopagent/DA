#include "ConfigurationManager.h"
#include "cpprest\json.h"

using namespace web;
using namespace web::json;

//ConfigurationManager ConfigurationManager::configurationManager;

ConfigurationManager::ConfigurationManager()
{

}


ConfigurationManager::~ConfigurationManager()
{
}


ConfigurationManager::ConfigurationManager(ConfigurationManager &aConfigurationManager)
{
	
}

ConfigurationManager ConfigurationManager::operator=(ConfigurationManager &aConfigurationManager)
{
	return *this;
}

ConfigurationManager& ConfigurationManager::getInstance()
{
	//Mutex mutex;
	static ConfigurationManager configurationManager;
	return configurationManager;
}

UserInfo ConfigurationManager::getUserInfo()
{
	wstring userName = userDetails.getLoggedInUserName();
	wstring Email = L"AgentUser@mindtree.com";
	return UserInfo(userName, L"AgentUser@mindtree.com");
}

MachineInfo ConfigurationManager::getMachineInfo()
{
	wstring MachineId = machineDetails.getMachineId();
	wstring MachineOsVer = machineDetails.getOperatingSystemVer();
	wstring MachineIp = machineDetails.getIpAddress();
	wstring MachineNmae = machineDetails.getHostName();

	return MachineInfo(MachineId, MachineOsVer, MachineIp,
		MachineNmae, L"12.9715987", L"77.5945627");
}

AgentInfo ConfigurationManager::getAgentInfo()
{
	wstring AgentId = agentDetails.getAgentId();
	string AgentVersion = "1.0.0.0";
	return AgentInfo(AgentId, L"1.0.0.0");
}

value ConfigurationManager::getConfigurationData()
{
	value jsonValue;
	try
	{
		if (strPDSFolder.empty())
		{
			std::string str(getenv("userprofile"));
			strPDSDirectory = str + "\\AppData\\Local\\Sennheiser\\HeadSetup";
		}
		else
		{
			strPDSDirectory.assign(strPDSFolder);
		}

		strPDSDirectory += "\\PDS\\";
		strPDSDirectory += "TenantConfiguration.dat";
	}

	catch (exception&)
	{
		strPDSDirectory = "";
	}
	try
	{
		fileManager.Open(strPDSDirectory);
		fileManager.Read();

		wstring Wstr = fileManager.GetFileData();
		jsonValue = value::parse(Wstr);
	}
	catch (...)
	{		
		return false;
	}
	return jsonValue;
}

value ConfigurationManager::getParam(const wstring& inputkey)
{
	jsonFileData = getConfigurationData();
	value outputValue;

	if (jsonFileData.is_object())
	{
		for (auto iter = jsonFileData.as_object().cbegin(); iter != jsonFileData.as_object().cend(); ++iter)
		{
			utility::string_t key = (iter)->first;
			const value &value = iter->second;

			if ((!key.empty()))
			{
				if (key.compare(inputkey) == 0)
				{
					outputValue = value;
				}
			}

			if (value.is_object())
			{
				json::value jsonSubObject = value;
				for (auto iter = jsonSubObject.as_object().cbegin(); iter != jsonSubObject.as_object().cend(); ++iter)
				{
					utility::string_t Subkey = (iter)->first;
					const json::value &Subvalue = iter->second;
					if ((!Subkey.empty()))
					{
						if (Subkey.compare(inputkey) == 0)
						{
							outputValue = Subvalue;
						}
					}
				}
			}
		}
	}
	return outputValue;
}

void ConfigurationManager::saveParams(value inputData)
{
	fileManager.Open(strPDSDirectory);
	fileManager.Write(inputData);
	fileManager.Close();
}

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
	if (fs.is_open())
	{
		fs.close();
	}
}
void FileManager::Open(string aLogFileName)
{
	fs.open(aLogFileName.c_str(), std::fstream::in | std::fstream::out);
}

void FileManager::Write(value writeData)
{
	fs << writeData.serialize() << endl;
}

void FileManager::Read()
{
	std::getline(fs, fileData);
	Close();
}

void FileManager::Close()
{
	fs.close();
}

wstring FileManager::GetFileData()
{
	return fileData;
}

 string ConfigurationManager::geFilePath()
{
	return strPDSDirectory;
}

 DeviceInfo ConfigurationManager::getDeviceInfo()
 {
	 return DeviceInfo();
 }
