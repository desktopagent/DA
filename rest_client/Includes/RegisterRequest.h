#pragma once
#include "StringUtil.h"
#include "cpprest/http_client.h"
#include <string>
#include <cpprest/json.h>
#include "LogManager.h"
#include "HttpResponse.h"

using namespace web::json;
using namespace std;
using namespace web::http;
using namespace web::http::client;

class AgentInfo 
{
private:
	const static wstring AGENT_DETAILS;
	wstring agentId;
	wstring agentVer;
public:
	AgentInfo();
	AgentInfo(const wstring& id, const wstring& ver);

	void setAgentId(const wstring& id);
	wstring getAgentId();
	void setAgentVersion(const wstring& str);
	wstring getAgentVersion();

	value toJSON();
};

class MachineInfo 
{
private:
	wstring machineId;
	wstring osVersion;
	wstring ipAddress;
	wstring machineName;
	wstring latitude;
	wstring longitude;
public:
	MachineInfo();
	MachineInfo(const wstring& id, const wstring& ver, const wstring& ip, 
		        const wstring& name, const wstring& lat, const wstring& longi);

	void setMachineId(const wstring& id);
	wstring getMachineId();
	void setOsVersion(const wstring& ver);
	wstring getOsVersion();
	void setIpAddress(const wstring& ip);
	wstring getIpAddress();
	void setMachineName(const wstring& name);
	wstring getMachineName();
	void setLatitude(const wstring& lat);
	wstring getLatitude();
	void setLongitude(const wstring& longi);
	wstring getLongitude();
	
	value toJSON();
};

class UserInfo 
{
private:
	const static wstring USER_DETAILS;
	wstring username;
	wstring email;
public:
	UserInfo();
	UserInfo(const wstring& name, const wstring& mail);

	void setUserName(const wstring& name);
	wstring getUserName();
	void setEmail(const wstring& mail);
	wstring getEmail();
	value toJSON();
};

class RegisterData 
{
private:
	const static wstring COMMAND;
	wstring tenantId;
	AgentInfo agentInfo;
	UserInfo userInfo;
	MachineInfo machineInfo;
public:
	RegisterData();
	void setTenantId(const wstring& id);
	wstring getTenantId();
	void setAgentInfo(const AgentInfo& agent);
	AgentInfo getAgentInfo();
	void setUserInfo(const UserInfo& user);
	UserInfo getUserInfo();
	void setMachineInfo(const MachineInfo& machine);
	MachineInfo getMachineINfo();

	value toJSON();
};

class RegisterResponse :public HttpResponse
{
public:
	RegisterResponse(const http_response res);
	bool validateResponse();
	bool registrationSucceeded();
};
class RegisterRequest : public http_request
{
private:

	RegisterData data;
public:
	RegisterRequest( RegisterData d );
	~RegisterRequest();

	http_request build();

};

//class HeartBeatRequest : public http_request {
//private:
//	wstring token;
//public:
//	HeartBeatRequest(const wstring& t);
//
//	http_request build();
//
//
//};