
#ifndef HS_MANAGER_H
#define HS_MANAGER_H

#include "HSStructs.h"
#include "StackInterface.h"

#include "UsbHidImpl.h"


class CHSManager :public IHSEventListener
{
public:

	static CHSManager* getInstance(IHSEventListener* aHSEventListener);

	~CHSManager(void);

	virtual void enableCheckForUSBUsagePages();

	void setEventListener(IHSEventListener* aEventListener);

	virtual bool handleHSEvents(HSEvents aHSEvent, void* aHeadset);

	void updateUSBHeadsetInList(list<CHeadsetInfo*> alstUSBHSList);

	void setSupportedHSVID(list<string> alstStrVIDSupportedHS);

	void updateILstUSBHS(CHeadsetInfo* aLHeadsetInfo);

private:
	CHSManager(IHSEventListener* aHSEventListener);

	bool isDefaultHS(CHeadsetInfo* aHeadset);

	static CHSManager* iHSActiveInstance;

	IHSEventListener*   iHSEventListener;

	CHeadsetInfo*       iDefaultHeadset;

	IStackInterface*        iUSBStack;
	CHeadsetInfo*       iBTHSInUse;

	CHeadsetInfo*	iToBeConnectedHeadset;

	list<CHeadsetInfo*> ilstUSBHS;

	int	iConnectedHSCount;
};

#endif