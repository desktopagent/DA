#include "DeviceUnplugRequest.h"
const wstring DeviceUnpluginData::COMMAND = L"Command";

DeviceUnpluginData::DeviceUnpluginData()
{

}

void DeviceUnpluginData::setDeviceInfo(const DeviceInfo& device)
{
	deviceInfo = device;
}

DeviceInfo DeviceUnpluginData::getDeviceInfo()
{
	return deviceInfo;
}

void DeviceUnpluginData::setUserInfo(const UserInfo& user)
{
	userInfo = user;
}

UserInfo DeviceUnpluginData::getUserInfo()
{
	return userInfo;
}
value DeviceUnpluginData::toJSON()
{
	value json;
	json[COMMAND] = value::string(L"APP_DVC_DSCON_REQ");
	json[L"DeviceDetails"] = deviceInfo.toDisConnectJSON();
	json[L"UserDetails"] = userInfo.toJSON();
	return json;
}

DeviceUnplugRequest::DeviceUnplugRequest(DeviceUnpluginData aDevice)
{
	deviceUnpluginData = aDevice;
}

DeviceUnplugRequest::~DeviceUnplugRequest()
{
}
http_request DeviceUnplugRequest::build(const std::wstring &token)
{
	headers().clear();
	value json = deviceUnpluginData.toJSON();
	headers().add(L"Token", token);
	headers().set_content_type(L"application/json");
	set_body(json);
	return *this;
}

DeviceUnplugResponse::DeviceUnplugResponse(const http_response res) :HttpResponse(res)
{

}

bool DeviceUnplugResponse::unplugSucceeded()
{
	return responseSucceeded();
}

bool DeviceUnplugResponse::validateResponse()
{
	bool result = false;
	value responseBody = getResponseBody();
	if (responseBody.is_object())
	{
		setResponseCommand(NOTKNOWN);
		if (responseBody.has_field(L"Command"))
		{
			wstring cmd = StringUtil::stripQuotes(responseBody.at(L"Command").serialize());
			if (cmd.compare(L"SRV_DVC_DSCON_ACK") == 0)
				setResponseCommand(SRV_DVC_DSCON_ACK);
			else if (cmd.compare(L"SRV_DVC_DSCON_FAILED_ACK") == 0)
				setResponseCommand(SRV_DVC_DSCON_FAILED_ACK);
		}
		result = processCommand();
	}
	return result;
}

