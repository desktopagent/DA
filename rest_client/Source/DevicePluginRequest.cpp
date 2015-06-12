#include "DevicePluginRequest.h"

const wstring DevicePluginData::COMMAND = L"Command";

DevicePluginData::DevicePluginData()
{

}

void DevicePluginData::setDeviceInfo(const DeviceInfo& device)
{
	deviceInfo = device;
}

DeviceInfo DevicePluginData::getDeviceInfo()
{
	return deviceInfo;
}

void DevicePluginData::setUserInfo(const UserInfo& user)
{
	userInfo = user;
}

UserInfo DevicePluginData::getUserInfo()
{
	return userInfo;
}

value DevicePluginData::toJSON()
{
	value json;
	json[COMMAND] = value::string(L"APP_DVC_CON_REQ");
	json[L"DeviceDetails"] = deviceInfo.toJSON();
	json[L"UserDetails"] = userInfo.toJSON();
	return json;
}

DevicePluginRequest::DevicePluginRequest(const DevicePluginData aDevice)
{
	devicePluginData = aDevice;
}

DevicePluginRequest::~DevicePluginRequest()
{
}

http_request DevicePluginRequest::build(const std::wstring token)
{
	headers().clear();
	value json = devicePluginData.toJSON();
	headers().set_content_type(L"application/json");
	headers().add(L"Token", token);
	set_body(json);
	return *this;
}

DevicePluginResponse::DevicePluginResponse(const http_response res) :HttpResponse(res)
{

}

bool DevicePluginResponse::pluginSucceeded()
{
	return responseSucceeded();
}

bool DevicePluginResponse::validateResponse()
{
	bool result = false;
	value responseBody = getResponseBody();
	if (responseBody.is_object())
	{
		setResponseCommand(NOTKNOWN);
		if (responseBody.has_field(L"Command"))
		{
			wstring cmd = StringUtil::stripQuotes(responseBody.at(L"Command").serialize());
			if (cmd.compare(L"SRV_DVC_CON_ACK") == 0)
				setResponseCommand(SRV_DVC_CON_ACK);
			else if (cmd.compare(L"SRV_DVC_UPDT_ACK") == 0)
				setResponseCommand(SRV_DVC_UPDT_ACK);
			else if (cmd.compare(L"SRV_DVC_FAILED_ACK") == 0)
				setResponseCommand(SRV_DVC_FAILED_ACK);
		}
		result = processCommand();
	}
	return result;
}

