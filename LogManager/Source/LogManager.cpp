/********************************************************************************
* @file    LogManager.cpp
* @author  
* @brief   This file contains the CLogManager class implementation
* @date    2015-04-01
**********************************************************************************/

#include "stdafx.h"
#include "LogManager.h"
#include <ctime>

//******************************************************************************


CLogManager* CLogManager::m_LogInstance = 0;
//******************************************************************************
CLogManager* CLogManager::getInstance()
{
	if (m_LogInstance == NULL)
	{
		m_LogInstance = new CLogManager("LogFile");
	}
	return m_LogInstance;
}


CLogManager::CLogManager(const string& aStrLogFileName)
{
	string l_hStrCustomLogFolder;
	string l_hStrLogsDirectory;
	char *pValue;
	size_t len;
	if (l_hStrCustomLogFolder.empty())
	{
		//ToDO: Need to be handled in the unix or MAC os
		_dupenv_s(&pValue, &len, "userprofile");
		l_hStrLogsDirectory.assign(pValue);
		l_hStrLogsDirectory = l_hStrLogsDirectory + "\\AppData\\Local\\Sennheiser\\HeadSetup";

	}
	else
	{
		l_hStrLogsDirectory.assign(l_hStrCustomLogFolder);
	}
	l_hStrLogsDirectory += "\\Logs\\";

	initializeLogFile(l_hStrLogsDirectory, aStrLogFileName);
}


CLogManager::CLogManager(string f_hStrLogFilePath, string aStrLogFileName)
{
	initializeLogFile(f_hStrLogFilePath, aStrLogFileName);
}

CLogManager::~CLogManager()
{
	oFile.Close();
}

void CLogManager::initializeLogFile(string f_hStrLogFilePath, string f_hStrLogFileName)
{
	string l_hStrDeveloperLogFile;
	string l_hStrUserLogFile;
	bool  l_boExceptionThrown = false;

	//In the existing system we are getting the status of it.but as of now it is 
	//always in enable status
	m_boLoggingEnabled = true; // l_hCLogManagerOptions->getLogEnableStatus();

	//If logging is not enabled must return from this function
	if (!m_boLoggingEnabled)
		return;

	try
	{
		//If the "logs" directory doesnt exist then it must be created
		//ToDO: Need to be handled in the unix or MAC os
		if (!dirExists(f_hStrLogFilePath))
		{
			wstring wstemp = stringTowstring(f_hStrLogFilePath);
			LPCWSTR lstr = wstemp.c_str();
			if (!CreateDirectory(lstr, NULL))
			{
				printf("CreateDirectory failed (%d)\n", GetLastError());
				DWORD d = GetLastError();
				return;
			}
		}
	}
	catch (...)
	{
		l_boExceptionThrown = true;
	}
	if (l_boExceptionThrown)
	{
		f_hStrLogFilePath = "";
	}
	else
	{
		//f_hStrLogFilePath += "\\";
	}
	if (f_hStrLogFileName.empty())
	{
		l_hStrDeveloperLogFile = f_hStrLogFilePath + "Developer.log";
		l_hStrUserLogFile = f_hStrLogFilePath + "User.log";
	}
	else
	{
		l_hStrDeveloperLogFile = f_hStrLogFilePath + f_hStrLogFileName + "Developer.log";
		l_hStrUserLogFile = f_hStrLogFilePath + f_hStrLogFileName + "User.log";
	}
	ifilePath = l_hStrDeveloperLogFile;
	oFile.Open(ifilePath);
}

void CLogManager::writeLog(LogTypes f_eLogType, string aStrFunName, string f_hStrLogMessage)
{
	try
	{
		string l_hStrLog = aStrFunName + " : " + f_hStrLogMessage;

		if (m_boLoggingEnabled)
		{
			//must append the current timestamp in the message before storing it
			string l_hStrMessageFormat;

			switch (f_eLogType)
			{
			case LogTypes::EXCEPTION_LOG:
			{
											l_hStrMessageFormat = getSystemTime() + "Exception : " + l_hStrLog;
											oFile.Write(l_hStrMessageFormat.c_str());
											break;
			}
			case LogTypes::INFO_LOG:
			{
									   //ToDo: Have to insert our code for replacement of this.
									   l_hStrMessageFormat = getSystemTime() + "Info : " + l_hStrLog;
									   oFile.Write(l_hStrMessageFormat.c_str());
									   break;
			}
			case LogTypes::WARN_LOG:
			{
										 //ToDo: Have to insert our code for replacement of this.
										 break;
			}
			default:
			{
					   break;
			}
			}
		}
	}
	catch (...)
	{
	}
}


//Method returns currentsystem time
string CLogManager::getSystemTime()
{
	time_t rawtime;
	// struct tm* timeinfo;
	time_t l_Now = time(0);
	struct tm  tstruct;
	string strTimestamp;
	char buf[80];
	time(&rawtime);
	localtime_s(&tstruct, &l_Now);

	strftime(buf, sizeof(buf), "%d/%m/%Y %X ", &tstruct);

	strTimestamp = string(buf);
	return strTimestamp;
}

//Method to check directory exists or not
bool CLogManager::dirExists(const string& dirName_in)
{
	wstring wstemp = stringTowstring(dirName_in);
	LPCWSTR lstr = wstemp.c_str();
	DWORD ftyp = GetFileAttributes(lstr);
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

//conversion of std::string to LPCWSTR
wstring CLogManager::stringTowstring(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}

//conversion of LPCWSTR to std::string
string CLogManager::wstringTostring(const wstring &wstr)
{
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}

CFileManager::CFileManager()
{
}


CFileManager::~CFileManager()
{
	if (fs.is_open())
	{
		fs.close();
	}
}

void CFileManager::Open(string aLogFileName)
{
	fs.open(aLogFileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
}

void CFileManager::Write(const char *chBuff)
{
	fs << chBuff << endl;
}

void CFileManager::Close()
{
	fs.close();
}

CLogManagerOptions::CLogManagerOptions()
{
	m_boLoggingEnabled = false;
}

CLogManagerOptions::~CLogManagerOptions()
{
}


void CLogManagerOptions::setLogEnableStatus(bool f_boLoggingStatus)
{
	m_boLoggingEnabled = f_boLoggingStatus;
}

bool CLogManagerOptions::getLogEnableStatus()
{
	return m_boLoggingEnabled;
}

void CLogManagerOptions::setMillisecondLogging(bool f_boLoggingStatus)
{
	m_boShowMilliseconds = f_boLoggingStatus;
}

bool CLogManagerOptions::getMillisecondLogStatus()
{
	return m_boShowMilliseconds;
}

void CLogManagerOptions::setCustomLogFolder(string f_hStrCustomLogFolder)
{
	m_hStrCustomLogFolder = f_hStrCustomLogFolder;
}

string CLogManagerOptions::getCustomLogFolder()
{
	return m_hStrCustomLogFolder;
}





