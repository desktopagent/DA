/**
*******************************************************************************
* @file    UserDetails.h
* @author  Mindtree
* @par
* @brief   This file contains the Logged in User information of the class UserDetails.
* @date    2015-05-11
*******************************************************************************
*/

#ifndef USER_DETAILS_H
#define USER_DETAILS_H
#include <typeinfo.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include <string>
using namespace std;

class UserDetails
{
public:
	UserDetails();
	~UserDetails();

	/**************************************************************************
	* @brief   Present Logged In User Name in system.
	***************************************************************************/
	wstring getLoggedInUserName();

private:
	string LoggedInUserName;
	string UsrID;
	string UsremailID;


};

#endif