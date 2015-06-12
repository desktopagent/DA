#include "RestClient.h"
#include "StringUtil.h"
#include "LogManager.h"

RestClient::RestClient()
{
}

RestClient::~RestClient()
{
}

pplx::task<http_response> RestClient::post(const wstring& url, const http_request& request)
{
	try {
		http_client_config config;

		// setting the proxy 	
		web_proxy wp(L"http://172.22.218.218:8085");
		config.set_proxy(wp);

		http_client client(url, config);
		request.set_method(methods::POST);
		std::string requestStr = StringUtil::wstringTostring(request.to_string());
		CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, "Request: ", __FUNCTION__);
		CLogManager::getInstance()->writeLog(LogTypes::INFO_LOG, requestStr, __FUNCTION__);
		return client.request(request);
	}
	catch (const http_exception &ex) {
		//log exception and rethrow
		throw ex;
	}
}
