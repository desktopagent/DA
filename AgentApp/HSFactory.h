
#ifndef HS_FACTORY_H
#define HS_FACTORY_H

#include "HSManager.h"

class CHSFactory
{
public:
	~CHSFactory(void);
	static CHSFactory* getInstance(IHSEventListener* aHSEventListener);
	CHSManager* getHSManagerInstance();

private:
	CHSFactory(IHSEventListener* aHSEventListener);
	static CHSFactory* iHSFActiveInstance;
	CHSManager* iHSManager;
};

#endif