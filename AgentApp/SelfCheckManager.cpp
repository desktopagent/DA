
#include "SelfCheckManager.h"

SelfCheck::SelfCheck() : configurationManager(ConfigurationManager::getInstance())
{

}


SelfCheck::~SelfCheck()
{

}


bool SelfCheck::perform()
{
	//Reading from the configuration file
	value agentID = configurationManager.getParam( L"AgentID" );
	if ( StringUtil::isEmpty(agentID.serialize() ) )
	{
		throw  SelfCheckException( "Empty AgentId" /*, SelfCheckException::errors::INSUFFICIENT_DATA*/ );
	}

	value tenentID = configurationManager.getParam(L"TenantID");
	if ( StringUtil::isEmpty(tenentID.serialize()) )
	{
		throw  SelfCheckException( "Empty TenantId"/*, SelfCheckException::errors::INSUFFICIENT_DATA*/ );
	}

	value macID = configurationManager.getParam(L"MACID");
	if ( StringUtil::isEmpty(macID.serialize()) )
	{
		throw  SelfCheckException( "Empty MACID"/*, SelfCheckException::errors::INSUFFICIENT_DATA*/ );
	}

	//TODO: Gather AgentId, TokenId & MacId from the environment and compare against above values
	AgentInfo agentInfo = configurationManager.getAgentInfo();

	wstring sAgentId = agentID.serialize();
	if ( agentInfo.getAgentId() != StringUtil::stripQuotes( sAgentId ) )
	{
		return false;
	}

	MachineInfo machineInfo= configurationManager.getMachineInfo();
	wstring sMacId = macID.serialize();
	if ( machineInfo.getMachineId() != StringUtil::stripQuotes( sMacId )	 )
	{
		return false;
	}
	return true;
}

SelfCheckException::SelfCheckException(const char * const & msg) : exception(msg)
{
	CLogManager::getInstance()->writeLog(LogTypes::EXCEPTION_LOG, __FUNCTION__, msg);
}