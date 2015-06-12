#include "RegisterRequest.h"

const wstring UserInfo::USER_DETAILS = L"UserDetails";
const wstring RegisterData::COMMAND = L"Command";


AgentInfo::AgentInfo()
{

}

AgentInfo::AgentInfo(const wstring& id, const wstring& ver)
{
	setAgentId(id);
	setAgentVersion(ver);
}

void AgentInfo::setAgentId(const wstring& id)
{
	agentId = id;
}

wstring AgentInfo::getAgentId()
{
	return agentId;
}

void AgentInfo::setAgentVersion(const wstring& str)
{
	agentVer = str;
}

wstring AgentInfo::getAgentVersion()
{
	return agentVer;
}

value AgentInfo::toJSON()
{
	value json;
	json[L"AgentID"] = value::string(agentId);
	json[L"AgentSWversion"] = value::string(agentVer);
	return json;
}

MachineInfo::MachineInfo()
{

}

MachineInfo::MachineInfo(const wstring& id, const wstring& ver, const wstring& ip,
	const wstring& name, const wstring& lat, const wstring& longi)
{
	setMachineId(id);
	setOsVersion(ver);
	setIpAddress(ip);
	setMachineName(name);
	setLatitude(lat);
	setLongitude(longi);
}

void MachineInfo::setMachineId(const wstring& id)
{
	machineId = id;
}

wstring MachineInfo::getMachineId()
{
	return machineId;
}

void MachineInfo::setOsVersion(const wstring& ver)
{
	osVersion = ver;
}

wstring MachineInfo::getOsVersion()
{
	return osVersion;
}

void MachineInfo::setIpAddress(const wstring& ip)
{
	ipAddress = ip;
}

wstring MachineInfo::getIpAddress()
{
	return ipAddress;
}

void MachineInfo::setMachineName(const wstring& name)
{
	machineName = name;
}

wstring MachineInfo::getMachineName()
{
	return machineName;
}

void MachineInfo::setLatitude(const wstring& lat)
{
	latitude = lat;
}

wstring MachineInfo::getLatitude()
{
	return latitude;
}

void MachineInfo::setLongitude(const wstring& longi)
{
	longitude = longi;
}

wstring MachineInfo::getLongitude()
{
	return longitude;
}

value MachineInfo::toJSON()
{
	value json;
	json[L"MACID"] = value::string(machineId);
	json[L"OSVersion"] = value::string(osVersion);
	json[L"IPadress"] = value::string(ipAddress);
	json[L"MachineName"] = value::string(machineName);
	json[L"GeoLocation"] = value::string(L"Lat:" + latitude + L"," + L"Long:" + longitude);
	return json;
}

UserInfo::UserInfo()
{

}

UserInfo::UserInfo(const wstring& name, const wstring& mail)
{
	setUserName(name);
	setEmail(mail);
}

void UserInfo::setUserName(const wstring& name)
{
	username = name;
}

wstring UserInfo::getUserName()
{
	return username;
}

void UserInfo::setEmail(const wstring& mail)
{
	email = mail;
}

wstring UserInfo::getEmail()
{
	return email;
}

value UserInfo::toJSON()
{
	value json;
	json[L"UserName"] = value::string(username);
	json[L"Email"] = value::string(email);
	return json;
}

RegisterData::RegisterData()
{

}

void RegisterData::setTenantId(const wstring& id)
{
	tenantId = id;
}

wstring RegisterData::getTenantId()
{
	return tenantId;
}

void RegisterData::setAgentInfo(const AgentInfo& agent)
{
	agentInfo = agent;
}

AgentInfo RegisterData::getAgentInfo()
{
	return agentInfo;
}

void RegisterData::setUserInfo(const UserInfo& user)
{
	userInfo = user;
}

UserInfo RegisterData::getUserInfo()
{
	return userInfo;
}

void RegisterData::setMachineInfo(const MachineInfo& machine)
{
	machineInfo = machine;
}

MachineInfo RegisterData::getMachineINfo()
{
	return machineInfo;
}

value RegisterData::toJSON()
{
	value json;
	json[COMMAND] = value::string(L"APP_AGT_REGN");
	json[L"TenantID"] = value::string(tenantId);
	json[L"AgentDetails"] = agentInfo.toJSON();
	json[L"MachineDetails"] = machineInfo.toJSON();
	json[L"UserDetails"] = userInfo.toJSON();
	return json;
}

RegisterResponse::RegisterResponse(const http_response res) :HttpResponse(res)
{

}

RegisterRequest::RegisterRequest(RegisterData aData)
{
	data = aData;
}

RegisterRequest::~RegisterRequest()
{
}

http_request RegisterRequest::build() {
	//data.populate();
	value json = data.toJSON();

	headers().set_content_type(L"application/json");
	set_body(json);
	return *this;
}

bool RegisterResponse::registrationSucceeded()
{
	return responseSucceeded();
}

bool RegisterResponse::validateResponse()
{
	bool result = false;
	value responseBody = getResponseBody();
	if (responseBody.is_object())
	{
		setResponseCommand(NOTKNOWN);
		if (responseBody.has_field(L"Command"))
		{
			wstring cmd = StringUtil::stripQuotes(responseBody.at(L"Command").serialize());

			if (cmd.compare(L"SRV_AGT_REGD") == 0)
				setResponseCommand(SRV_AGT_REGD);
			else if (cmd.compare(L"SRV_AGT_UPDT_ACK") == 0)
				setResponseCommand(SRV_AGT_UPDT_ACK);
			else if (cmd.compare(L"SRV_AGT_REGN_FAILED") == 0)
				setResponseCommand(SRV_AGT_REGN_FAILED);
		}

		result = processCommand();
	}
	return result;
}

//HeartBeatRequest::HeartBeatRequest(const wstring& t)
//{
//	token = t;
//}
//
//http_request HeartBeatRequest::build()
//{
//	headers().add(L"Token", token);
//	value json;
//	json[L"Command"] = value::string(L"APP_HEARTBEAT");
//	headers().set_content_type(L"application/json");
//	set_method(methods::POST);
//	return *this;
//}
