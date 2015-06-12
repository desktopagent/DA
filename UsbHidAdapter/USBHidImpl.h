
#ifndef USB_HID_IMPL_H
#define USB_HID_IMPL_H

#include "StackInterface.h"

#include <Windows.h>
#include <Dbt.h>
#include <tchar.h>
#include <sstream>
#include <exception>

#include "StackInterface.h"
#include "UsbHidHeadset.h"
#include "USBHidDef.h"
#include <Windows.h>
#include <stdint.h>

class CUsbHidImpl : public IStackInterface
{
public:
	CUsbHidImpl(IHSEventListener*);
	~CUsbHidImpl();

	static CUsbHidImpl* getInstance(IHSEventListener*);

	static CUsbHidImpl*     ms_hActiveInstance;

	virtual void enableCheckForUSBUsagePages();

	void    onDeviceChange(WPARAM wParam, LPARAM lParam);
	virtual void setSupportedHSVID(list<string> f_hlstStrVIDSupportedHS);

	// To Get the BT address

	typedef HANDLE(*OpenDevice)(int VendorId, int ProductId, int i);
	typedef int(*GetBdAddr)(HANDLE Handle, uint16_t out_bd_add[4]);
	
	typedef int(*GetBdAddr)(HANDLE Handle, uint16_t out_bd_add[4]);
	GetBdAddr getBluetoothAddr;
	OpenDevice openDev;
	uint16_t addr16[4];
	

private:
	void setEventListener(IHSEventListener* f_hEventListener);

	TCHAR*  getDevicePath(LPARAM lParam);

	void openHidDevice(TCHAR *f_pcDevicePath, bool f_boSendConnectEvent);

	bool addDevicePathAddress(TCHAR *f_pcDevicePath);
	void getDevicePathAddress(TCHAR *f_pcDevicePath, string&_hStrDevicePathAddress);

	void updateUsbHeadsetsName(string f_hStrUsbModel);

	void removeDevicePathAddress(TCHAR *f_pcDevicePath);

	IHSEventListener* m_hEventListener;

	bool	m_boCheckUsagePages;	

	list<string>	m_hlstStrAMSupportedHS;

	string	m_hStrFFFFUsageDevicePath;

	list<string> m_hlstStrVIDSupportedHS;

	list<CHeadsetInfo*> m_hlstHSList;

	list<string> m_hlstModels;
	list<string> m_hlstDevPathAddresses;
};

#endif