#pragma once
#include "StringUtil.h"
#include "cpprest/http_client.h"
#include <string>
#include <cpprest/json.h>
#include "RegisterRequest.h"
#include "DeviceInfo.h"
using namespace web::json;
using namespace std;
using namespace web::http;
using namespace web::http::client;

class DeviceUnpluginData{
private:
	const static wstring COMMAND;
	UserInfo userInfo;
	DeviceInfo deviceInfo;
public:
	DeviceUnpluginData();
	void setDeviceInfo(const DeviceInfo& device);
	DeviceInfo getDeviceInfo();
	void setUserInfo(const UserInfo& user);
	UserInfo getUserInfo();
	value toJSON();
};

class DeviceUnplugRequest :public http_request
{
private:

	DeviceUnpluginData deviceUnpluginData;
public:
	DeviceUnplugRequest(DeviceUnpluginData aDevice);
	~DeviceUnplugRequest();
	http_request build(const std::wstring &token);
};

class DeviceUnplugResponse :public HttpResponse
{
private:
	http_response response;
public:
	bool unplugSucceeded();
	DeviceUnplugResponse(const http_response res);
	bool validateResponse();
};