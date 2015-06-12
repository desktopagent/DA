
#include "UsbHidImpl.h"

CUsbHidImpl* CUsbHidImpl::ms_hActiveInstance = NULL;

CUsbHidImpl::CUsbHidImpl(IHSEventListener* f_hEventListener)
{
	//m_boCheckUsagePages = false;
	m_boCheckUsagePages = true;

	setEventListener(f_hEventListener);
}

CUsbHidImpl* CUsbHidImpl::getInstance(IHSEventListener* f_hEventListener)
{
	if (ms_hActiveInstance == NULL)
	{
		CUsbHidImpl::ms_hActiveInstance = new CUsbHidImpl(f_hEventListener);
	}
	return ms_hActiveInstance;
}


void CUsbHidImpl::setEventListener(IHSEventListener* f_hEventListener)
{
	m_hEventListener = f_hEventListener;
}

void CUsbHidImpl::enableCheckForUSBUsagePages()
{
	m_boCheckUsagePages = true;
}

void CUsbHidImpl::onDeviceChange(WPARAM wParam, LPARAM lParam)
{
	TCHAR *l_pcDevicePath;

	int tlParam = (int)lParam;

	if (DBT_DEVICEARRIVAL == wParam)
	{
		l_pcDevicePath = getDevicePath(lParam);

		if (l_pcDevicePath != NULL)
		{
			openHidDevice(l_pcDevicePath, true);
		}
	}
	else if (DBT_DEVNODES_CHANGED == wParam)
	{
	}

	else if (DBT_DEVICEREMOVECOMPLETE == wParam)
	{
		l_pcDevicePath = getDevicePath(lParam);
		if (l_pcDevicePath != NULL)
		{
			string l_hStrDevicePath = "";
			std::basic_string<TCHAR> strName = l_pcDevicePath;
			for (unsigned int i = 0; i < strName.length(); i++)
				l_hStrDevicePath += strName[i];

			for (CHeadsetInfo* l_hHSInfo : m_hlstHSList)
			{
				std::size_t found = l_hStrDevicePath.find(l_hHSInfo->iStrUSBHSPath);

				if (found != std::string::npos)
				{
					if (!m_boCheckUsagePages)
					{
						removeDevicePathAddress(l_pcDevicePath);
					}

					// Important to remove Headset from the list before
					string l_hStrHSModel = l_hHSInfo->iStrModel;
					m_hlstHSList.remove(l_hHSInfo);

					updateUsbHeadsetsName(l_hStrHSModel);

					m_hEventListener->handleHSEvents(HSEVT_HSDISCONNECTED, l_hHSInfo);
					break;
				}
			}
		}
	}
}


TCHAR*  CUsbHidImpl::getDevicePath(LPARAM lParam)
{
	DEV_BROADCAST_HDR* l_pDevBroadcastHeader = (DEV_BROADCAST_HDR*)lParam;

	if (l_pDevBroadcastHeader == NULL)
	{
		return NULL;
	}

	if (DBT_DEVTYP_DEVICEINTERFACE == l_pDevBroadcastHeader->dbch_devicetype)
	{
		DEV_BROADCAST_DEVICEINTERFACE* l_pDevBroadcastDevice = (DEV_BROADCAST_DEVICEINTERFACE*)l_pDevBroadcastHeader;

		return l_pDevBroadcastDevice->dbcc_name;
	}

	return NULL;
}

void CUsbHidImpl::openHidDevice(TCHAR *f_pcDevicePath, bool f_boSendConnectEvent)
{
	PHIDP_PREPARSED_DATA    l_pPreparsedData;
	PHIDP_BUTTON_CAPS       l_pButtonCaps;
	PHIDP_BUTTON_CAPS       l_pTempButtonCaps;
	HIDD_ATTRIBUTES         l_stDeviceAttributes;
	CUsbHidHeadset*         l_hUsbHidHeadset;
	CHeadsetInfo*           l_hHSInfo;
	HIDP_CAPS               l_stDeviceCapabilities;
	HANDLE                  l_hDeviceHandle;
	HANDLE                  l_hOverlappedHandle;
	string                  l_hStrDevicePath;
	string                  l_hStrHSModel;
	USHORT                  l_usNumCaps;
	LONG                    l_lResult;
	bool                    l_boHSSupported;
	bool                    l_boTelephonyUsageExists;
	bool                    l_boLedUsageExists;
	int                     l_nLength;
	int                     l_nlCount;
	int                     l_nStructSize;
	HINSTANCE               hinstLib;
	HANDLE                  hHandle;
	string                  strBdAddress;

	bool l_boFFFFUsageExists = false;

	l_boHSSupported = false;

	hinstLib = LoadLibrary(TEXT("SecomParser.dll"));

	l_hDeviceHandle = CreateFile(f_pcDevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
								 NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (INVALID_HANDLE_VALUE != l_hDeviceHandle)
	{
		l_stDeviceAttributes.Size = sizeof(l_stDeviceAttributes);

		l_lResult = HidD_GetAttributes(l_hDeviceHandle, &l_stDeviceAttributes);

		TCHAR l_pDeviceName[SECOM_USB_HID_NAME_BUFFER];
		memset(l_pDeviceName, '\0', sizeof(TCHAR)* SECOM_USB_HID_NAME_BUFFER);

		HidD_GetProductString(l_hDeviceHandle, l_pDeviceName, sizeof(TCHAR)* SECOM_USB_HID_NAME_BUFFER);

		l_hStrHSModel = "";
		std::basic_string<TCHAR> strName = l_pDeviceName;
		for (unsigned int i = 0; i < strName.length(); i++)
			l_hStrHSModel += strName[i]; 

		if (SECOM_USB_HID_VENDOR_ID == l_stDeviceAttributes.VendorID)
		{
			l_boHSSupported = true;

			// Added to get the BT Address
			if (hinstLib != NULL)
			{
				openDev = (OpenDevice)GetProcAddress(hinstLib, "SecomParserOpenDeviceVidPid");
				hHandle = (openDev)(l_stDeviceAttributes.VendorID, l_stDeviceAttributes.ProductID, 0);
				getBluetoothAddr = (GetBdAddr)GetProcAddress(hinstLib, "Prod_GetBdAddr");				
				int ret = getBluetoothAddr(hHandle, addr16);
				strBdAddress = to_string(addr16[0]) + "-" + to_string(addr16[1]) + "-" + to_string(addr16[2]) + "-" + to_string(addr16[2]);
			}
		}

		if (!l_boHSSupported)
		{
			for (string l_hSupportedModel : m_hlstModels)
			{
				std::size_t found = l_hStrHSModel.find(l_hSupportedModel);

				if (found != std::string::npos)
				{
					l_boHSSupported = true;
					break;
				}
			}
		}

		if (!l_boHSSupported)
		{
			CloseHandle(l_hDeviceHandle);
			l_hDeviceHandle = INVALID_HANDLE_VALUE;
			return;
		}

		HidD_GetPreparsedData(l_hDeviceHandle, &l_pPreparsedData);

		HidP_GetCaps(l_pPreparsedData, &l_stDeviceCapabilities);   		

		//Checking if the Collection contains either of the Telephony or 
		//LED usage page.
		l_boTelephonyUsageExists = false;
		l_boLedUsageExists = false;

		if ((SECOM_USB_CAPRICORN_USAGE_PAGE == l_stDeviceCapabilities.UsagePage)
			&& (SECOM_USB_CAPRICORN_USAGE == l_stDeviceCapabilities.Usage))
		{
			l_boFFFFUsageExists = true;

			//m_hStrFFFFUsageDevicePath = "";

			std::basic_string<TCHAR> strName = f_pcDevicePath;
			for (unsigned int i = 0; i < strName.length(); i++)
				m_hStrFFFFUsageDevicePath += strName[i];
		}

		// Handle for Presence
		if (l_stDeviceAttributes.ProductID == 46)
		{
			l_boTelephonyUsageExists = true;
			l_boLedUsageExists = true;
			l_boFFFFUsageExists = true;

			m_hStrFFFFUsageDevicePath = "";			  
			std::basic_string<TCHAR> strName = f_pcDevicePath;
			for (unsigned int i = 0; i < strName.length(); i++)
				m_hStrFFFFUsageDevicePath += strName[i];
		}

		l_usNumCaps = l_stDeviceCapabilities.NumberInputButtonCaps;

		l_nStructSize = sizeof(HIDP_BUTTON_CAPS)* l_usNumCaps;

		l_pButtonCaps = l_pTempButtonCaps = (PHIDP_BUTTON_CAPS) new BYTE[l_nStructSize];

		HidP_GetButtonCaps(HidP_Input, l_pButtonCaps, &l_usNumCaps, l_pPreparsedData);

		for (l_nlCount = 0;
			l_nlCount < l_stDeviceCapabilities.NumberInputButtonCaps;
			l_nlCount++, l_pTempButtonCaps++)
		{
			if (SECOM_USB_TELEPHONY_USAGE_PAGE == l_pTempButtonCaps->UsagePage)
				l_boTelephonyUsageExists = true;
			else if (SECOM_USB_LED_USAGE_PAGE == l_pTempButtonCaps->UsagePage)
				l_boLedUsageExists = true;
		}

		//free up memory
		if (NULL != l_pButtonCaps)
		{
			delete[]l_pButtonCaps;
			l_pButtonCaps = NULL;
		}

		if (m_boCheckUsagePages)
		{
			if (!l_boTelephonyUsageExists || !l_boLedUsageExists)
			{
				l_usNumCaps = l_stDeviceCapabilities.NumberOutputButtonCaps;

				l_nStructSize = sizeof(HIDP_BUTTON_CAPS)* l_usNumCaps;

				l_pButtonCaps = l_pTempButtonCaps =
					(PHIDP_BUTTON_CAPS) new BYTE[l_nStructSize];

				HidP_GetButtonCaps(HidP_Output, l_pButtonCaps,
					&l_usNumCaps, l_pPreparsedData);

				for (l_nlCount = 0;
					l_nlCount < l_stDeviceCapabilities.NumberOutputButtonCaps;
					l_nlCount++, l_pTempButtonCaps++)
				{
					if (SECOM_USB_TELEPHONY_USAGE_PAGE == l_pTempButtonCaps->UsagePage)
						l_boTelephonyUsageExists = true;
					else if (SECOM_USB_LED_USAGE_PAGE == l_pTempButtonCaps->UsagePage)
						l_boLedUsageExists = true;
				}
			}
		}
		else
		{
			if (!addDevicePathAddress(f_pcDevicePath))
			{
				return;
			}
			l_boTelephonyUsageExists = true;
			l_boLedUsageExists = true;
		}

		//free up memory
		if (NULL != l_pButtonCaps)
		{
			delete[]l_pButtonCaps;
			l_pButtonCaps = NULL;
		}

		if ((l_boTelephonyUsageExists || l_boLedUsageExists) && l_boHSSupported)
		{

			int* l_pnDevPath((int*)f_pcDevicePath);			//JAS?
			l_nLength = _tcslen((const wchar_t *)f_pcDevicePath);

			l_hStrDevicePath = "";
			for (int i = 0; i <= l_nLength; i++)
			{
				l_hStrDevicePath += l_pnDevPath[i];
			}

			//This overlapped handle would be used for overlapped ReadFile
			//operation
			l_hOverlappedHandle =
				CreateFile(f_pcDevicePath, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

			//Create a new CHeadsetInfo object.
			l_hHSInfo = new CHeadsetInfo();

			l_hHSInfo->iStrModel = l_hStrHSModel;
			if (l_boHSSupported == true)
			{
				l_hHSInfo->m_strManufacturer =l_stDeviceAttributes.VendorID;
			}
			
			// New name would be assigned in the function updateUsbHeadsetsName
			l_hHSInfo->m_strVendorId = l_stDeviceAttributes.VendorID;
			l_hHSInfo->m_strProductId = l_stDeviceAttributes.ProductID;
			l_hHSInfo->iStrHSName = l_hStrHSModel;
			l_hHSInfo->iIsConnected = true;
			l_hHSInfo->iIsUsb = true;
			l_hHSInfo->iHSPaired = true;
			l_hHSInfo->m_strDeviceId = strBdAddress;

			if (!m_hStrFFFFUsageDevicePath.empty())
			{
				l_hHSInfo->iStrUSBHSPath = m_hStrFFFFUsageDevicePath;
			}
			else
			{
				l_hHSInfo->iStrUSBHSPath = l_hStrDevicePath;
			}

			//Add device into the list before we call updateUsbHeadsetsName
			m_hlstHSList.push_back(l_hHSInfo);

			updateUsbHeadsetsName(l_hStrHSModel);

			//Create a CUsbHidHeadset object to be passed to HSManager
			l_hUsbHidHeadset = new CUsbHidHeadset(l_hDeviceHandle,
				l_hOverlappedHandle,
				l_stDeviceCapabilities,
				l_pPreparsedData,
				l_hHSInfo->iStrHSName,
				l_hStrDevicePath,
				l_hStrHSModel,
				l_hHSInfo);

			m_hStrFFFFUsageDevicePath = "";

			l_hUsbHidHeadset->setEventListener(m_hEventListener);

			// Set the headset object in the CHeadsetInfo object
			l_hHSInfo->iHeadset = (IHSInterface*)(l_hUsbHidHeadset);

			if (f_boSendConnectEvent)
			{
				m_hEventListener->handleHSEvents(HSEVT_HSCONNECTED, l_hHSInfo);
			}
		}
		else
		{
			CloseHandle(l_hDeviceHandle);
			l_hDeviceHandle = INVALID_HANDLE_VALUE;
			if (l_pPreparsedData != NULL)
			{
				HidD_FreePreparsedData(l_pPreparsedData);
				l_pPreparsedData = NULL;
			}
		}
	}
}

bool CUsbHidImpl::addDevicePathAddress(TCHAR *f_pcDevicePath)
{
	bool l_boIsNewDeviceDetected = true;
	string l_hStrDevPathAddr = "";
	list<string>::iterator it;

	getDevicePathAddress(f_pcDevicePath, l_hStrDevPathAddr);

	for (it = m_hlstDevPathAddresses.begin(); it != m_hlstDevPathAddresses.end(); it++)
	{
		if (l_hStrDevPathAddr == *it)
		{
			l_boIsNewDeviceDetected = false;
			break;
		}
	}
	if (l_boIsNewDeviceDetected)
	{
		m_hlstDevPathAddresses.push_back(l_hStrDevPathAddr);
	}
	return l_boIsNewDeviceDetected;
}


void CUsbHidImpl::getDevicePathAddress(TCHAR *f_pcDevicePath, string& f_hStrDevicePathAddress)
{
	char* l_hUSBAddr = new char[16];
	strcpy(l_hUSBAddr, "");
	int l_nPathLength = _tcslen((const wchar_t *)f_pcDevicePath);
	int l_nHashCnt = 0;
	int l_nAndCnt = 0;

	for (int i = 0; i < l_nPathLength; i++)
	{
		if (f_pcDevicePath[i] == '#')
		{
			l_nHashCnt++;
		}
		if (l_nHashCnt == 2)
		{
			if (f_pcDevicePath[i] == '&')
			{
				l_nAndCnt++;
			}
			if (l_nAndCnt == 1)
			{
				if (f_pcDevicePath[i] != '&')
				{
					strncat(l_hUSBAddr, (char*)(f_pcDevicePath + i), 1);
				}
			}
		}
	}
	f_hStrDevicePathAddress = string(l_hUSBAddr);
}


void CUsbHidImpl::updateUsbHeadsetsName(string f_hStrUsbModel)
{
	int l_nUsbHeadsetCount = 0;

	for (CHeadsetInfo* l_hHSInfo : m_hlstHSList)
	{
		std::size_t found = f_hStrUsbModel.find(l_hHSInfo->iStrModel);

		if (found != std::string::npos)
		{
			l_nUsbHeadsetCount++;
			l_hHSInfo->iStrHSName = "";

			if (l_nUsbHeadsetCount > 1)
			{
				stringstream s;
				s << l_nUsbHeadsetCount;

				l_hHSInfo->iStrHSName = (f_hStrUsbModel + " (" + s.str() + ")");
			}
			else
			{
				l_hHSInfo->iStrHSName = f_hStrUsbModel;
			}
		}
	}
}


void CUsbHidImpl::setSupportedHSVID(list<string> f_hlstStrVIDSupportedHS)
{
	if (f_hlstStrVIDSupportedHS.empty())
	{
		return;
	}

	m_hlstStrVIDSupportedHS.assign(f_hlstStrVIDSupportedHS.begin(), f_hlstStrVIDSupportedHS.end());

}

void CUsbHidImpl::removeDevicePathAddress(TCHAR *f_pcDevicePath)
{
	string l_hStrDevPathAddr = "";
	getDevicePathAddress(f_pcDevicePath, l_hStrDevPathAddr);

	for (string l_hStrPathAddr : m_hlstDevPathAddresses)
	{
		if (l_hStrDevPathAddr == l_hStrPathAddr)
		{
			m_hlstDevPathAddresses.remove(l_hStrPathAddr);
			break;
		}
	}
}