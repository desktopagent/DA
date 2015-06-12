#ifndef HS_EVENT_LISTENER_H
#define HS_EVENT_LISTENER_H


#include "HSDefines.h"

class IHSEventListener
{
public:
	/*****************************************************************************
	* @brief   Handler for the various events of the headset
	* @return  none
	***************************************************************************
	*/

	virtual bool handleHSEvents(HSEvents aHSEvent, void* aHeadset) = 0;
};

#endif