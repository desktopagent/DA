#include "AgentApp.h"

class MainException : public exception
{
public:
	MainException(const char * const & msg);
};

void main(int argc, char* argv[], char* envp[]) 
{
	try{
		/*AgentApp aa;
		aa.startup();
		aa.runApp();*/
	}
	catch (SelfCheckException& ScEx)
	{
		// add the Log component and log it. 
		cerr << "SelfCheckException : " << ScEx.what() << endl;
		throw  MainException(ScEx.what());
	}
	catch (AgentAppException& AEx)
	{
		cerr << "AgentAppException : " << AEx.what() << endl;
		throw  MainException(AEx.what());
	}
	catch (const http_exception& ex) {
		cerr << "HTTP Exception: " << ex.what() << endl;
		throw  MainException(ex.what());
	}
	catch (runtime_error err)	  {
		cerr << err.what() << endl;
		throw  MainException(err.what());
	}
	catch (...)
	{
		cerr << "Catch All" <<  endl;
		throw  MainException("Exception");
	}


}

MainException::MainException(const char * const & msg) : exception(msg)
{
	CLogManager::getInstance()->writeLog(LogTypes::EXCEPTION_LOG, msg, __FUNCTION__);
}
