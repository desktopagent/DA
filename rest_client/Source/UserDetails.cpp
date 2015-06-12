/**
*******************************************************************************
* @file    UserDetails.cpp
* @author  Mindtree
* @par
* @brief   This file contains the definition of the methods defined in the
*          User Configuration Impl of class UserDetails.
* @date    2015-05-11
*******************************************************************************
*/

#include "UserDetails.h"	  
#include "StringUtil.h"

//******************************************************************************


UserDetails::UserDetails() : LoggedInUserName("Default"), UsrID(""), UsremailID("abc@xyz.com")
{
}

UserDetails::~UserDetails()
{  
}

/**************************************************************************
* @brief   Get Logged In User Name in system.
***************************************************************************/
wstring UserDetails::getLoggedInUserName()
{
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	try{
		GetUserName((TCHAR*)username, &size);

		LoggedInUserName = string(username, username + (UNLEN + 1));
	}
	catch (const exception& ex)
	{
		ex.what();
	}
	return StringUtil::stringTowstring(LoggedInUserName);
}


