
#ifndef HS_INTERFACE_H
#define HS_INTERFACE_H

#include "HSEventListener.h"

class IHSInterface
{
public:
	virtual void setEventListener(IHSEventListener* f_hEventListener) = 0;
};


#endif