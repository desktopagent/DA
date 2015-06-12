#pragma once
#include <iostream>
#include <cpprest/json.h>
#include "cpprest/http_client.h"

using namespace web::http::client;
using namespace web::json;
using namespace std;
using namespace web::http;

enum RESPONSE_COMMAND
{
	SRV_AGT_REGD = 1,
	SRV_AGT_UPDT_ACK,
	SRV_AGT_REGN_FAILED,
	SRV_DVC_CON_ACK,
	SRV_DVC_UPDT_ACK,
	SRV_DVC_FAILED_ACK,
	SRV_DVC_DSCON_ACK,
	SRV_DVC_DSCON_FAILED_ACK,
	NOTKNOWN
};

class HttpResponse
{
private:
	http_response response;
	RESPONSE_COMMAND responseCommand;
public:
	HttpResponse(const http_response res);
	virtual bool validateResponse() = 0;
	bool processCommand();
	void setResponseCommand(RESPONSE_COMMAND resCMD);
	RESPONSE_COMMAND getResponseCommand();
	value getResponseBody();
	bool responseSucceeded();
	wstring getToken();
};