
#include "HSFactory.h"
#include <iostream>

CHSFactory* CHSFactory::iHSFActiveInstance = NULL;

CHSFactory::CHSFactory(IHSEventListener* aHSEventListener)
{
	iHSManager = CHSManager::getInstance(aHSEventListener);
}

CHSFactory::~CHSFactory(void)
{
}

CHSFactory* CHSFactory::getInstance(IHSEventListener* aHSEventListener)
{
	if (! iHSFActiveInstance)
	{
		iHSFActiveInstance = new CHSFactory(aHSEventListener);
	}
	return iHSFActiveInstance;
}

CHSManager* CHSFactory::getHSManagerInstance()
{
	return iHSManager;
}