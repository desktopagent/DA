#ifndef SECOM_HS_STRUCT_H
#define SECOM_HS_STRUCT_H

#include "HSInterface.h"
#include "AppDefs.h"

#include <string>

using namespace std;

class CHeadsetInfo
{
public:
	string	iStrHSName;
	string	iStrModel;
	string	iPSM;
	string	iStrUSBHSPath;
	int     m_strManufacturer;
	int     m_strVendorId;
	int     m_strProductId;
	string  m_strDeviceId;

	bool	iHSPaired;
	bool	iIsUsb;
	bool	iIsConnected;
	bool	srvHSStatus;
	bool    isDefault;

	IHSInterface*	iHeadset;

	HSType	iHSType;

	CHeadsetInfo() : iStrHSName(""), iStrModel(""), iHSPaired(false), iIsConnected(false), iPSM(""),		
		iIsUsb(false), iHeadset(NULL), iStrUSBHSPath(""), m_strManufacturer(0), m_strVendorId(0), 
		m_strProductId(0), m_strDeviceId(""), iHSType(HSTYPE_NOT_CSR), srvHSStatus(false), isDefault(false)
	{
	}

	~CHeadsetInfo()
	{
	}

	CHeadsetInfo* operator= (CHeadsetInfo* f_hOtherHS)
	{
		 iStrHSName = f_hOtherHS->iStrHSName;

		iStrModel = f_hOtherHS->iStrModel;

		iHSPaired = f_hOtherHS->iHSPaired;
		iPSM = f_hOtherHS->iPSM;
		iIsUsb = f_hOtherHS->iIsUsb;
		iIsConnected = f_hOtherHS->iIsConnected;
		iHeadset = f_hOtherHS->iHeadset;

		iStrUSBHSPath = f_hOtherHS->iStrUSBHSPath;

		iHSType = f_hOtherHS->iHSType;
		m_strDeviceId = f_hOtherHS->m_strDeviceId;
		m_strProductId = f_hOtherHS->m_strProductId;
		m_strVendorId = f_hOtherHS->m_strVendorId;
		srvHSStatus = f_hOtherHS->srvHSStatus;
		return this;
	}

	void assignDefaultValues()
	{
		iStrHSName = "";

		//m_eHSModel = HS_NONE_SELECTED;
		iStrModel = "";

		iHSPaired = false;
		iPSM = "";
		iIsUsb = false;
		iIsConnected = false;
		iStrUSBHSPath = "";
		iHeadset = NULL;
		srvHSStatus = false;

		iHSType = HSTYPE_NOT_CSR;
	}
};

#endif