#pragma once

#include "RestClient.h"
#include "ConfigurationManager.h"
#include <string>
#include "SelfCheckManager.h"
#include "HSManager.h"
#include "HSFactory.h"
#include "DeviceInfo.h"
#include "DevicePluginRequest.h"
#include "DeviceUnplugRequest.h"
#include "LogManager.h"

class AgentAppException : public exception
{
public:
	AgentAppException(const char * const & msg);
};

class AgentApp : public IHSEventListener
{
public:
	//AgentApp();
	virtual ~AgentApp();

	void startup();
	void runApp();

	bool handleHSEvents(HSEvents aHSEvent, void* aHeadset);
	static AgentApp* getInstance();
	int getRetryCounter();
	std::vector<CHeadsetInfo*>	ilstHSList;
	bool isDefaultsHS();
private:
	AgentApp();
	static AgentApp* iAgentAppInstance;
	void populate(const wstring tenantId, RegisterData& data);
	std::wstring token;
	const int constRetryCounter = 2;
	bool isToken;

	RestClient restClient;

	ConfigurationManager& configurationManager;

	void initializeVariables();
	void sendDisconnectEvent(CHeadsetInfo* aHeadset);

	void startHSFactory();
	void manageAppAsHSConnected(CHeadsetInfo* aHeadset);
	void manageHeadsetInList(CHeadsetInfo* aHeadset);
	void addHeadsetToList(CHeadsetInfo* aHeadset);
	CHeadsetInfo* getHeadsetFromPairedList(CHeadsetInfo* f_hpHeadset);

	// need to replace void with CHeadsetInfo class
	void* isDummyHeadset;

	// Object of HS manager.
	CHSManager*		iHSManager;

	bool m_boHSConnected;

	CHSManager*		m_hHSManager;
	CHSFactory*		m_hHSFactory;
	CHeadsetInfo*	iDefaultHeadset;

	void writeToLog(string aStrFunName, string aStrMsg, LogTypes aLogTypes = LogTypes::EXCEPTION_LOG);
};


// from Pradeep for UI integration
extern "C"
{
	struct DeviceDetails
	{
		char *deviceName;
		char *manufacturer;
		char *deviceId;
		int  productId;
		char *firmwareVersion;
		char *serialNo;
		bool isDefault;
	};		 	
	
	__declspec(dllexport) void GetEvents(WPARAM wParam, LPARAM lParam);
	__declspec(dllexport) void InitializeAgent();
	__declspec(dllexport) DeviceDetails* ExposeDatatoUI(int *iSize);
}