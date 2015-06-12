#pragma once

#include "StringUtil.h"
#include "cpprest/http_client.h"
#include <string>
#include <windows.h>
#include <cpprest/json.h>
#include "RegisterRequest.h"
#include "DeviceInfo.h"
#include "RestClient.h"
#include "ConfigurationManager.h"


using namespace web::json;
using namespace std;
using namespace web::http;
using namespace web::http::client;

class DevicePluginData
{
private:
	const static wstring COMMAND;
	UserInfo userInfo;
	DeviceInfo deviceInfo;
	
public:
	DevicePluginData();
	void setDeviceInfo(const DeviceInfo& device);
	DeviceInfo getDeviceInfo();
	void setUserInfo(const UserInfo& user);
	UserInfo getUserInfo();

	value toJSON();
};

class DevicePluginRequest :public http_request
{
private:
	DevicePluginData devicePluginData;
public:
	DevicePluginRequest(DevicePluginData aDevice);
	~DevicePluginRequest();
	http_request build(const std::wstring token);
	http_response retry();
};

class DevicePluginResponse : public HttpResponse
{
private:
	http_response response;
public:
	DevicePluginResponse(const http_response res);
	bool pluginSucceeded();
	bool validateResponse();
};