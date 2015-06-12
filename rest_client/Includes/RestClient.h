#pragma once
#include <string>
#include "cpprest/http_client.h"

using namespace std;
using namespace web::http;
using namespace web::http::client;

class RestClient
{
public:
	RestClient();
	~RestClient();
	pplx::task<http_response> post(const wstring& url, const http_request& request);
};

