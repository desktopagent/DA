#pragma once
#include <cpprest/http_client.h>
#include <string>

class HeartBeatRequest : public web::http::http_request
{
public:
	HeartBeatRequest(const std::wstring &token );
	virtual ~HeartBeatRequest();
};

