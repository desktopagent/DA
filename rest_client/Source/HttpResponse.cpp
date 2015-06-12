#include "HttpResponse.h"


HttpResponse::HttpResponse(const http_response res) :response(res)
{

}

bool HttpResponse::processCommand()
{
	bool result = true;
	switch (getResponseCommand())
	{
	case 1:
		cout << "Agent Registration Successful" << endl;
		break;
	case 2:
		cout << "Please Update the Agent" << endl;
		break;
	case 3:
		cout << "Agent Registration Failed" << endl;
		result = false;
		break;
	case 4:
		cout << "Device Connection Successful" << endl;
		break;
	case 5:
		cout << "Please Update the Device" << endl;
		break;
	case 6:
		cout << "Device Connection Failed" << endl;
		result = false;
		break;
	case 7:
		cout << "Device Disconnection Successful" << endl;
		break;
	case 8:
		cout << "Device Disconnection Failed" << endl;
		result = false;
		break;
	default:
		result = false;
		cout << "Unknown Command" << endl;
	}
	return result;
}

void HttpResponse::setResponseCommand(RESPONSE_COMMAND resCMD)
{
	responseCommand = resCMD;
}

RESPONSE_COMMAND HttpResponse::getResponseCommand()
{
	return responseCommand;
}

value HttpResponse::getResponseBody()
{
	return response.extract_json().get();
}

bool HttpResponse::responseSucceeded()
{
	return response.status_code() == status_codes::OK || status_codes::Created;
}

wstring HttpResponse::getToken()
{
	utility::string_t value;
	if (responseSucceeded() && response.headers().has(L"Token")) {
		auto Token = response.headers().find(L"Token");
		utility::string_t key = (Token)->first;
		utility::string_t value = (Token)->second;
		return value;
	}
	return L"";
}