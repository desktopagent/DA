
/**
*******************************************************************************
* @file    SelfCheckManager.h
* @author  Mindtree
* @par
* @brief   This file contains the self check function.
* @date    2015-05-20
*******************************************************************************
*/

#ifndef APP_SELF_CHECK_H
#define APP_SELF_CHECK_H

#include "ConfigurationManager.h"

class SelfCheckException : public exception
{
public:
	enum errors{
	  INSUFFICIENT_DATA = 0,
	  UNKNOWN
	};
	SelfCheckException(const char * const& msg /*, errors error*/);
private:
	errors errorCode;
};

class SelfCheck
{
public:
	SelfCheck();
	virtual ~SelfCheck();

	bool perform();

private:
	ConfigurationManager& configurationManager;
};

#endif