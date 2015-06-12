#include "AppManager.h"
#include <iostream>

CAppManager* CAppManager::appManageActiveInstance = NULL;

CAppManager* CAppManager::getInstance()
{
	if (appManageActiveInstance == NULL)
	{
		appManageActiveInstance = new CAppManager();
	}
	return appManageActiveInstance;
}

CAppManager::CAppManager()
{
	initializeVariables();

	//creating the HS factory
	startHSFactory();
}

void CAppManager::initializeVariables()
{
	isDummyHeadset = NULL;
	iHSConnected = false;
}

void CAppManager::startHSFactory()
{

}