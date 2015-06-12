#include "HeartBeatRequest.h"
#include <cpprest/json.h>

HeartBeatRequest::HeartBeatRequest( const std::wstring &token )
{
	using namespace web::json;
	headers().add(L"Token", token);
	value json;
	json[L"Command"] = value::string(L"APP_HEARTBEAT");
	set_body(json);
	headers().set_content_type(L"application/json");
	set_method(web::http::methods::POST);
}

HeartBeatRequest::~HeartBeatRequest()
{
}
