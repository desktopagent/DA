

#ifndef STACK_INTERFACE_H
#define STACK_INTERFACE_H

#include "HSStructs.h"
#include "list"
class IStackInterface
{
public:
	virtual void setEventListener(IHSEventListener* aEventListener) = 0;

	void enableCheckForUSBUsagePages();
	void setSupportedHSVID(list<string> alstStrVIDSupportedHS);
};


#endif