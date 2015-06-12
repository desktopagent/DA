/********************************************************************************
* @file    LogManager.h
* @author  
* @brief   This file contains the CLogManager class declaration
* @date    2015-04-10
**********************************************************************************/

#ifndef SECOM_LOG_MANAGER_H
#define SECOM_LOG_MANAGER_H

//******************************************************************************

#define USB_LOGFILE_NAME    "USBLog"

#include <fstream>			//Stream class to both read and write from/to files

//******************************************************************************	  
using namespace std;
//******************************************************************************

	/** @enum LogTypes
	* This enum will represent which log file to use while logging the message.
	*/
	enum LogTypes
	{
		EXCEPTION_LOG = 0,
		INFO_LOG,
		WARN_LOG
	};

	class CFileManager
	{
	public:
		CFileManager();
		~CFileManager();
	public:
		fstream fs;
		void Open(string strLogFileName);
		void Write(const char *chBuff);
		void Read();
		void Close();
	};

	/*****************************************************************************
	*  @brief  CLogManager class is responsible for logging 
	*          the error messages in the log file.
	***************************************************************************
	*/
	class CLogManager
	{
	public:
		CLogManager(const string& aStrLogFileName);

		CLogManager(string f_hStrLogFilePath, string f_hStrLogFileName);

		~CLogManager(void);

		void  writeLog(LogTypes f_eLogType, string aStrFunName, string f_hStrLogMessage);

		void initializeLogFile(string f_hStrLogFilePath, string f_hStrLogFileName);

		bool dirExists(const std::string& dirName_in);

		string getSystemTime();

		wstring stringTowstring(const string& s);

		string CLogManager::wstringTostring(const wstring &wstr);
		string ifilePath;

		static CLogManager* getInstance();

	private:
		static CLogManager* m_LogInstance;
		CFileManager oFile;
		bool m_boLoggingEnabled;
		bool m_boShowMilliseconds;
		char systemTime[20];
	};

	/** @class CLogManagerOptions
	***************************************************************************
	* @brief   This class hold the option for enabling or disabling the
	*          logging. User can enable the logging by passing TRUE as command
	*           line argument while invoking the application.
	****************************************************************************/

	class CLogManagerOptions
	{
	public: // functions
		/**
		***********************************************************************
		* @brief    This is a singleton class. This class would hold the option
		*           whether to enable debugging or disable debugging. This
		*           function would return the active and only instance of
		*           CLogManagerOptions class.
		* @return   None.
		**************************************************************************/
		static CLogManagerOptions* getInstance();

		/**************************************************************************
		* @brief    This function returns the logging status.
		* @return   Bool.
		**************************************************************************/
		bool getLogEnableStatus();

		bool getMillisecondLogStatus();

		/**************************************************************************
		* @brief    This function seta the logging status.
		* @return   Bool.
		**************************************************************************/
		void setLogEnableStatus(bool f_boLoggingStatus);

		void setMillisecondLogging(bool f_boLoggingStatus);

		/**************************************************************************
		* @brief   This function sets the path for the custom log folder.
		* @param   f_hStrCustomLogFolder: (IN) Path for custom log folder.
		* @return  None.
		**************************************************************************/
		void setCustomLogFolder(string f_hStrCustomLogFolder);

		/**************************************************************************
		* @brief   This function sets the path for the custom log folder.
		* @return  String^: Handle to the custom log folder.
		**************************************************************************/
		string getCustomLogFolder();

	protected: // functions
		/**
		***********************************************************************
		* @brief   Destructor
		* @return  none
		**************************************************************************/
		~CLogManagerOptions();

	private: // functions

		/*************************************************************************
		* @brief   Contructor
		* @return  none
		**************************************************************************/
		CLogManagerOptions();  

	private: // variables

		/**************************************************************************
		* @brief   Handle to the active instance of the class.
		**************************************************************************/
		static CLogManagerOptions*      ms_hActiveInstance;


		/*************************************************************************
		* @brief   To hold the logging preference.
		**************************************************************************/
		bool                    m_boLoggingEnabled;

		/*************************************************************************
		* @brief   To hold the logging preference.
		**************************************************************************/
		bool                    m_boShowMilliseconds;

		string                 m_hStrCustomLogFolder;
	};


#endif