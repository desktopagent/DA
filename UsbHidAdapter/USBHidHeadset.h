
#ifndef USB_HID_HEADSET_H
#define USB_HID_HEADSET_H

//******************************************************************************

#include <windows.h>

#include "UsbHidDef.h"

#include "HSInterface.h"
#include "StackInterface.h"

class CUsbHidHeadset
{
public:
	CUsbHidHeadset(HANDLE f_hDeviceHandle,
		HANDLE f_hOverlappedHandle,
		HIDP_CAPS f_stDeviceCaps,
		PHIDP_PREPARSED_DATA f_pPreParsedData,
		string f_hStrDevcieName,
		string f_hStrDevicePath,
		string f_hStrDeviceModel,
		CHeadsetInfo* f_hstHSInfo
		);

	~CUsbHidHeadset();

	virtual void setEventListener(IHSEventListener* f_hEventListener);

private:	
	void fillHidDeviceInfo();

	PHID_DEVICE             m_pHidDevice;	
	CHeadsetInfo*           m_hstHeadsetInfo;  
	IHSEventListener*       m_hEventListener;

	bool			m_boDeviceOpened; 
	string			m_hStrDeviceName;
	string			m_hStrDeviceModel;
	string			m_hStrDevicePath;
};

#endif