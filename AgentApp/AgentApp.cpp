#include "AgentApp.h"
#include "HeartBeatRequest.h"
#include <Windows.h>
#include "AppManager.h"

AgentApp* AgentApp::iAgentAppInstance = NULL;

AgentApp* AgentApp::getInstance()
{
	if (!iAgentAppInstance)
	{
		iAgentAppInstance = new AgentApp();
	}
	return iAgentAppInstance;
}

AgentApp::AgentApp() : configurationManager(ConfigurationManager::getInstance())
{
	initializeVariables();

	//creating the HS factory
	startHSFactory();
}

AgentApp::~AgentApp()
{
}

void AgentApp::startup()
{
	cout << "AgentAp::startup()" << endl;
	//Check on configuration for TenantId & URL
	value val = configurationManager.getParam(L"URL");
	wstring  url = StringUtil::stripQuotes(val.serialize());
	if (StringUtil::isEmpty(url))
	{
		throw  AgentAppException("Empty URL");
	}
	wcout << L"URL" << url << endl;
	CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, __FUNCTION__, "URL");
	val = configurationManager.getParam(L"TenantID");

	wstring  tenantId = StringUtil::stripQuotes(val.serialize());

	//wstring tenantId = val.serialize();

	if (StringUtil::isEmpty(tenantId))
	{
		throw  AgentAppException("Empty TenentID");
	}
	wcout << L"TenantId: " << tenantId << endl << "startup checks..." << endl;
	//Check on token GUID 
	val = configurationManager.getParam(L"Token");
	token = StringUtil::stripQuotes(val.serialize());
	wcout << L"Token: " << token << endl;

	if (StringUtil::isEmpty(token))
	{
		cout << "Empty Token proceeding to perform Registration" << endl;
		RegisterData regData;
		regData.setTenantId(tenantId);

		AgentInfo agentInfo = configurationManager.getAgentInfo();
		regData.setAgentInfo(agentInfo);

		UserInfo userInfo = configurationManager.getUserInfo();
		regData.setUserInfo(userInfo);

		MachineInfo machineInfo = configurationManager.getMachineInfo();
		regData.setMachineInfo(machineInfo);

		RegisterRequest agentRegRequest(regData);
		try
		{
			cout << "Sending registration data" << endl;

			wstring newurl = url;
			newurl = newurl.append(L"/api/agent");

			try {
				http_response response = restClient.post(newurl, agentRegRequest.build()).get();
				RegisterResponse registerResponse(response);

				std::string responseStr = StringUtil::wstringTostring(response.to_string());
				CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, "Response For Agent Registration Request: ", __FUNCTION__);
				CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, responseStr, __FUNCTION__);
				wcout << L"Response: " << endl << response.to_string() << endl;
				token = registerResponse.getToken();
			}
			catch (const http_exception& ex) {
				cerr << "http_exception: " << ex.what();
			}

			// writing the json data to file
			value jsonData = regData.toJSON();
			jsonData[L"Token"] = value::string(token);
			jsonData[L"URL"] = value::string(url);

			configurationManager.saveParams(jsonData);
		}
		catch (const http_exception ex) {
			throw AgentAppException(ex.what());
		}
	}

	//SelfChecks to be done, the current implementation needs to return wstrings instead of converting them from string
	try
	{
		SelfCheck selfCheck;
		bool result = selfCheck.perform();
		if (!result)
		{
			throw AgentAppException("SelfCheck Failed");
		}
	}
	catch (const SelfCheckException& se)
	{
		throw se;
	}
}

void AgentApp::runApp()
{
	HeartBeatRequest heartBeatRequest(token);

	while (true) {
		cout << "Heart Beat sent" << endl;

		value val = configurationManager.getParam(L"URL");
		wstring url = StringUtil::stripQuotes(val.serialize());
		url += L"/api/manageagent";
		try {
			http_response response = (restClient.post(url, heartBeatRequest)).get();
			wcout << L"Response: " << endl << response.to_string() << endl;
		}
		catch (const http_exception& ex) {
			cerr << "http_exception: " << ex.what() << endl;
		}
		cout << "Sleeping for 30s" << endl;
		Sleep(30000);
		cout << "Thread wakeup" << endl;
	}
}

void AgentApp::populate(const wstring tenantId, RegisterData& data)
{
	ConfigurationManager configurationManager = ConfigurationManager::getInstance();

	data.setTenantId(tenantId);

	AgentInfo agentInfo = configurationManager.getAgentInfo();
	data.setAgentInfo(agentInfo);

	UserInfo userInfo = configurationManager.getUserInfo();
	data.setUserInfo(userInfo);

	MachineInfo machineInfo = configurationManager.getMachineInfo();
	data.setMachineInfo(machineInfo);
}

void AgentApp::initializeVariables()
{
	isDummyHeadset = NULL;
	m_boHSConnected = false;
	//m_boHSDefault = false;
}

void AgentApp::startHSFactory()
{
	m_hHSFactory = CHSFactory::getInstance(this);
	m_hHSManager = m_hHSFactory->getHSManagerInstance();

	if (m_hHSManager)
	{
		m_hHSManager->enableCheckForUSBUsagePages();
	}
}


bool AgentApp::handleHSEvents(HSEvents aHSEvent, void* aHeadset)
{
	bool returnValue = true;
	string strMessage = "";
	std::vector<CHeadsetInfo*>::iterator it;

	CHeadsetInfo* lHeadset = (CHeadsetInfo*)aHeadset;

	switch (aHSEvent)
	{
	case HSEVT_HSCONNECTED: // Connection established 
	{
		string l_hStrLog = " HSEVT_HSCONNECTED";
		manageAppAsHSConnected(lHeadset);
		break;
	}

	case HSEVT_HSCONNECTFAILED: // Connection establishment failed 
	{
		break;
	}
	case HSEVT_HSDISCONNECTED: // Connection lost 
	{
		CHeadsetInfo* l_hHSInList = getHeadsetFromPairedList(lHeadset);

		string l_hStrModel = "";
		if (l_hHSInList != NULL)
		{
			l_hStrModel = l_hHSInList->iStrModel;
			if (lHeadset->iIsUsb)
			{
				it = std::find(ilstHSList.begin(), ilstHSList.end(), l_hHSInList);
				if (it != ilstHSList.end())
					ilstHSList.erase(it);
					sendDisconnectEvent(lHeadset);
			}
		}
		break;
	}
	case HSEVT_SETASDEFAULTHS:
	{
		CHeadsetInfo* l_hHSInList = getHeadsetFromPairedList(lHeadset);
		if (nullptr != lHeadset)
		{
			iDefaultHeadset = new CHeadsetInfo();
			iDefaultHeadset->operator =(lHeadset);
			lHeadset->isDefault = true;
		}
		else
		{
			//Monitor::Exit(m_hlstCallList);
			return false;
		}
		break;
	}
	case HSEVT_REMOVEASDEFAULTHS:
	{	  
		bool l_boHSIsUSB = true;
		
		if (!ilstHSList.empty())
		{

			it = ilstHSList.begin();
			(*it)->isDefault = true;
			iDefaultHeadset = *it;
		}
		break;
	}
		
	}
	return returnValue;
}

void AgentApp::sendDisconnectEvent(CHeadsetInfo* aHeadset)
{
	if (aHeadset->srvHSStatus)
	{  
		try
		{
			DeviceInfo deviceInfo(StringUtil::stringTowstring(to_string(aHeadset->m_strVendorId)),
				StringUtil::stringTowstring(aHeadset->m_strDeviceId),
				StringUtil::stringTowstring(aHeadset->iStrHSName),
				StringUtil::stringTowstring(to_string(aHeadset->m_strManufacturer)),
				StringUtil::stringTowstring(to_string(aHeadset->m_strProductId)),
				L"NULL",
				L"NULL",
				getRetryCounter());

			UserInfo userInfo = configurationManager.getUserInfo();
			deviceInfo.toDisConnectJSON();
			DeviceUnpluginData deviceUnPluginData;
			deviceUnPluginData.setDeviceInfo(deviceInfo);
			deviceUnPluginData.setUserInfo(userInfo);
			DeviceUnplugRequest deviceUnPlugRequest(deviceUnPluginData);
			value val = configurationManager.getParam(L"URL");
			wstring url = StringUtil::stripQuotes(val.serialize());
			url += L"/api/ManageAgent/DeviceDisconnect";

			http_response response = restClient.post(url, deviceUnPlugRequest.build(token)).get();
			DeviceUnplugResponse dvcUnPlugResponse(response);
			std::string responseStr = StringUtil::wstringTostring(response.to_string());
			CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, "Response for Device Disconnection Request: ", __FUNCTION__);
			CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, responseStr, __FUNCTION__);
		}
		catch (exception &ex)
		{
			throw ex;
		}
	}
}

void AgentApp::manageAppAsHSConnected(CHeadsetInfo* aHeadset)
{
	CHeadsetInfo* l_hHeadset;

	if (aHeadset == NULL)
	{
		return;
	}

	if (aHeadset->iIsUsb)
	{
		l_hHeadset = new CHeadsetInfo();
		l_hHeadset->operator = (aHeadset);
		manageHeadsetInList(l_hHeadset);
	}

	try
	{
		DeviceInfo deviceInfo(StringUtil::stringTowstring(to_string(aHeadset->m_strVendorId)),
			                  StringUtil::stringTowstring(aHeadset->m_strDeviceId),
							  StringUtil::stringTowstring(aHeadset->iStrHSName), 
							  StringUtil::stringTowstring(to_string(aHeadset->m_strManufacturer)),
							  StringUtil::stringTowstring(to_string(aHeadset->m_strProductId)), 
							  L"NULL",
							  L"NULL", 
							  getRetryCounter());
		
		UserInfo userInfo = configurationManager.getUserInfo();
	
		DevicePluginData devicePluginData;
		devicePluginData.setDeviceInfo(deviceInfo);
		devicePluginData.setUserInfo(userInfo);
		DevicePluginRequest devicePluginRequest(devicePluginData);
		value val = configurationManager.getParam(L"URL");
		wstring url = StringUtil::stripQuotes(val.serialize());
		url += L"/api/ManageAgent/DeviceConnect";

		while (deviceInfo.getRetryCounter() > 0)
		{
			//sending device plugin request	  			
			http_response response = restClient.post(url, devicePluginRequest.build(token)).get();
			std::string responseStr = StringUtil::wstringTostring(response.to_string());
			CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, "Response For Device Connect Request: ", __FUNCTION__);
			CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, responseStr, __FUNCTION__);
			DevicePluginResponse dvcPluginResponse(response);

			if (!dvcPluginResponse.validateResponse())
			{
				deviceInfo.decreaseRetryCounter();
				deviceInfo.setDevicePluginStatus(TRYING);
				if (deviceInfo.getRetryCounter() > 0)
				{
					Sleep(500);
				}
				//resend the DevicePluginRequest
				continue;
			}
			deviceInfo.setDevicePluginStatus(CONNECTED);
			aHeadset->srvHSStatus = true;

			break;
		}
		if (deviceInfo.getRetryCounter() == 0)
			deviceInfo.setDevicePluginStatus(UNKNOWN);
	}

	catch (exception &ex)
	{
		CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, ex.what(), __FUNCTION__);
		// ToDo: break the flow
		//throw ex;
	}
}

void AgentApp::manageHeadsetInList(CHeadsetInfo* aHeadset)
{
	addHeadsetToList(aHeadset);
}


void AgentApp::addHeadsetToList(CHeadsetInfo* aHeadset)
{
	CHeadsetInfo* lHeadSet;
	for (unsigned int index = 0; index < ilstHSList.size(); index++)
	{
		lHeadSet = ilstHSList[0];

		if (lHeadSet == NULL)
			return;
	}
	ilstHSList.push_back(aHeadset);
	if (ilstHSList.size() == 1)
	{
		ilstHSList.front()->isDefault = true;
	}
}

CHeadsetInfo* AgentApp::getHeadsetFromPairedList(CHeadsetInfo* f_hHeadset)
{
	for (CHeadsetInfo* l_hHeadset : ilstHSList)
	{
		if (!f_hHeadset->iIsUsb)
		{
			return l_hHeadset;
		}
		else
		{
			if (l_hHeadset->iStrUSBHSPath == f_hHeadset->iStrUSBHSPath)
			{
				return l_hHeadset;
			}
		}
	}
	return NULL;   		
}

int AgentApp::getRetryCounter()
{
	return constRetryCounter;
}


void GetEvents(WPARAM wParam, LPARAM lParam)
{
	CUsbHidImpl::ms_hActiveInstance->onDeviceChange(wParam, lParam);
}

void InitializeAgent()
{
	AgentApp* iAgentAppInstance = AgentApp::getInstance();
	iAgentAppInstance->startup();
}

DeviceDetails* ExposeDatatoUI(int *iSize)
{
	AgentApp* iAgentAppInstance = AgentApp::getInstance();

	static DeviceDetails devDetails[5];

	int size = iAgentAppInstance->ilstHSList.size();
	for (unsigned int i = 0; i < iAgentAppInstance->ilstHSList.size(); i++)
	{
		CHeadsetInfo* ch = iAgentAppInstance->ilstHSList[i]; 		
		devDetails[i].deviceId = const_cast<char *>(ch->m_strDeviceId.c_str());
		devDetails[i].deviceName = const_cast<char *>(ch->iStrHSName.c_str());
		devDetails[i].firmwareVersion = ""; 
		devDetails[i].manufacturer = "Sennheiser Communications";
		devDetails[i].productId = ch->m_strProductId;
		devDetails[i].serialNo = "";	 
		// to set the HS is default or not
		devDetails[i].isDefault = ch->isDefault;
	}

	*iSize = size;
	return devDetails;
}

AgentAppException::AgentAppException(const char * const & msg) : exception(msg)
{
	CLogManager::getInstance()->writeLog(LogTypes::EXCEPTION_LOG, __FUNCTION__, msg);
}