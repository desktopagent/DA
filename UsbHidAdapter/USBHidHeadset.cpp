
#include "UsbHidHeadset.h"
#include <string>

using namespace std;

CUsbHidHeadset::CUsbHidHeadset(HANDLE f_hDeviceHandle,
	HANDLE f_hOverlappedHandle,
	HIDP_CAPS f_stDeviceCaps,
	PHIDP_PREPARSED_DATA f_pPreParsedData,
	string f_hStrDeviceName,
	string f_hStrDevicePath,
	string f_hStrDeviceModel,
	CHeadsetInfo *f_hstHSInfo
	)
{
	//TODO: adding LogManager

	m_pHidDevice = new HID_DEVICE();

	m_hStrDeviceName = f_hStrDeviceName;
	m_hStrDevicePath = f_hStrDevicePath;
	m_hStrDeviceModel = f_hStrDeviceModel;

	// Parent CHeadsetInfo handle
	m_hstHeadsetInfo = f_hstHSInfo;

	//Device handle
	m_pHidDevice->m_hDeviceHandle = f_hDeviceHandle;

	//Overlapped handle for ReadFile
	m_pHidDevice->m_hHidDeviceOverlapped = f_hOverlappedHandle;

	//Preparsed data
	m_pHidDevice->m_pPreParsedData = f_pPreParsedData;

	//Device capabilities
	m_pHidDevice->m_hDeviceCaps = f_stDeviceCaps;

	//set device opened
	m_boDeviceOpened = true;

	//Fill the PHID_DEVICE structure.
	fillHidDeviceInfo();
}

void CUsbHidHeadset::setEventListener(IHSEventListener* f_hEventListener)
{
	m_hEventListener = f_hEventListener;
}


// fill the PHID_DEVICE structure

void CUsbHidHeadset::fillHidDeviceInfo()
{
	USHORT              l_usNumCaps;
	PHIDP_BUTTON_CAPS   l_pButtonCaps;
	PHID_DATA           l_pDeviceData;
	ULONG               l_ulCount;
	int                 l_nStructSize;
	int                 l_nLength;

	m_pHidDevice->m_pcInputReportBuffer =
		new char[m_pHidDevice->m_hDeviceCaps.InputReportByteLength];

	if (NULL == m_pHidDevice->m_pcInputReportBuffer)
	{
		//ToDo: Throw an error 	  "Memory could not be allocated"
	}
	memset(m_pHidDevice->m_pcInputReportBuffer, '\0', m_pHidDevice->m_hDeviceCaps.InputReportByteLength);

	l_nStructSize = sizeof(HIDP_BUTTON_CAPS)* m_pHidDevice->m_hDeviceCaps.NumberInputButtonCaps;

	m_pHidDevice->m_pInputButtonCaps = l_pButtonCaps
		= (PHIDP_BUTTON_CAPS) new BYTE[l_nStructSize];

	if (NULL == l_pButtonCaps)
	{
		//ToDo: Throw an error 	  "Memory could not be allocated"
	}


	memset(l_pButtonCaps, '\0', l_nStructSize);

	l_usNumCaps = m_pHidDevice->m_hDeviceCaps.NumberInputButtonCaps;

	HidP_GetButtonCaps(HidP_Input, l_pButtonCaps,
		&l_usNumCaps, m_pHidDevice->m_pPreParsedData);

	m_pHidDevice->m_ulInputDataLength =
		m_pHidDevice->m_hDeviceCaps.NumberInputButtonCaps;

	l_nStructSize =
		sizeof (HID_DATA)* m_pHidDevice->m_ulInputDataLength;

	m_pHidDevice->m_pInputData = l_pDeviceData
		= (PHID_DATA) new BYTE[l_nStructSize];

	if (l_pDeviceData == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated");
	}

	memset(l_pDeviceData, '\0', l_nStructSize);

	for (l_ulCount = 0;
		l_ulCount < m_pHidDevice->m_hDeviceCaps.NumberInputButtonCaps;
		l_ulCount++, l_pDeviceData++, l_pButtonCaps++)
	{
		l_pDeviceData->m_boIsButtonData = TRUE;
		l_pDeviceData->m_ulStatus = HIDP_STATUS_SUCCESS;
		l_pDeviceData->m_usUsagePage = l_pButtonCaps->UsagePage;
		l_pDeviceData->m_boIsAbsolute = !l_pButtonCaps->IsAbsolute;
		if (l_pButtonCaps->IsRange)
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin =
				l_pButtonCaps->Range.UsageMin;
			l_pDeviceData->m_stButtonData.m_ulUsageMax =
				l_pButtonCaps->Range.UsageMax;
		}
		else
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin =
				l_pDeviceData->m_stButtonData.m_ulUsageMax =
				l_pButtonCaps->NotRange.Usage;
		}

		l_pDeviceData->m_stButtonData.m_ulMaxUsageLength =
			HidP_MaxUsageListLength(HidP_Input, l_pButtonCaps->UsagePage,
			m_pHidDevice->m_pPreParsedData);

		l_nLength = l_pDeviceData->m_stButtonData.m_ulMaxUsageLength + 1;
		l_pDeviceData->m_stButtonData.m_psUsages
			= (PUSAGE) new BYTE[l_nLength * sizeof (USAGE)];

		if (l_pDeviceData->m_stButtonData.m_psUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}

		memset(l_pDeviceData->m_stButtonData.m_psUsages, '\0',
			(l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_stButtonData.m_psPrevUsages
			= (PUSAGE) new BYTE[l_nLength * sizeof (USAGE)];

		if (l_pDeviceData->m_stButtonData.m_psPrevUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}
		memset(l_pDeviceData->m_stButtonData.m_psPrevUsages, '\0', (l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_ulReportID = l_pButtonCaps->ReportID;

	}

	//output buffer
	m_pHidDevice->m_pcOutputReportBuffer =
		new char[m_pHidDevice->m_hDeviceCaps.OutputReportByteLength];

	if (m_pHidDevice->m_pcOutputReportBuffer == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
	}

	memset(m_pHidDevice->m_pcOutputReportBuffer,
		'\0', m_pHidDevice->m_hDeviceCaps.OutputReportByteLength);

	l_nStructSize =
		sizeof(HIDP_BUTTON_CAPS)* m_pHidDevice->m_hDeviceCaps.NumberOutputButtonCaps;

	m_pHidDevice->m_pOutputButtonCaps = l_pButtonCaps
		= (PHIDP_BUTTON_CAPS) new BYTE[l_nStructSize];
	if (l_pButtonCaps == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
	}
	memset(l_pButtonCaps, '\0', l_nStructSize);

	l_usNumCaps = m_pHidDevice->m_hDeviceCaps.NumberOutputButtonCaps;

	HidP_GetButtonCaps(HidP_Output, l_pButtonCaps,
		&l_usNumCaps, m_pHidDevice->m_pPreParsedData);

	m_pHidDevice->m_ulOutputDataLength =
		m_pHidDevice->m_hDeviceCaps.NumberOutputButtonCaps;

	l_nStructSize = sizeof (HID_DATA)* m_pHidDevice->m_ulOutputDataLength;

	m_pHidDevice->m_pOutputData = l_pDeviceData
		= (PHID_DATA) new BYTE[l_nStructSize];

	if (l_pDeviceData == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
	}

	memset(l_pDeviceData, '\0', l_nStructSize);

	for (l_ulCount = 0;
		l_ulCount < m_pHidDevice->m_hDeviceCaps.NumberOutputButtonCaps;
		l_ulCount++, l_pDeviceData++, l_pButtonCaps++)
	{
		l_pDeviceData->m_boIsButtonData = TRUE;
		l_pDeviceData->m_ulStatus = HIDP_STATUS_SUCCESS;
		l_pDeviceData->m_usUsagePage = l_pButtonCaps->UsagePage;
		l_pDeviceData->m_boIsAbsolute = l_pButtonCaps->IsAbsolute;
		if (l_pButtonCaps->IsRange)
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin
				= l_pButtonCaps->Range.UsageMin;
			l_pDeviceData->m_stButtonData.m_ulUsageMax
				= l_pButtonCaps->Range.UsageMax;
		}
		else
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin =
				l_pDeviceData->m_stButtonData.m_ulUsageMax =
				l_pButtonCaps->NotRange.Usage;
		}

		l_pDeviceData->m_stButtonData.m_ulMaxUsageLength =
			HidP_MaxUsageListLength(HidP_Output, l_pButtonCaps->UsagePage,
			m_pHidDevice->m_pPreParsedData);

		l_nLength = l_pDeviceData->m_stButtonData.m_ulMaxUsageLength + 1;
		l_pDeviceData->m_stButtonData.m_psUsages
			= (PUSAGE) new BYTE[l_nLength * sizeof (USAGE)];
		if (l_pDeviceData->m_stButtonData.m_psUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}
		memset(l_pDeviceData->m_stButtonData.m_psUsages, '\0',
			(l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_stButtonData.m_psPrevUsages
			= (PUSAGE) new BYTE[(l_nLength)* sizeof (USAGE)];
		if (l_pDeviceData->m_stButtonData.m_psPrevUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}
		memset(l_pDeviceData->m_stButtonData.m_psPrevUsages, '\0',
			(l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_ulReportID = l_pButtonCaps->ReportID;

	}

	//feature report
	m_pHidDevice->m_pcFeatureReportBuffer = new char[m_pHidDevice->m_hDeviceCaps.FeatureReportByteLength];

	if (m_pHidDevice->m_pcFeatureReportBuffer == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
	}
	memset(m_pHidDevice->m_pcFeatureReportBuffer, '\0', m_pHidDevice->m_hDeviceCaps.FeatureReportByteLength);

	l_nStructSize = sizeof(HIDP_BUTTON_CAPS)* m_pHidDevice->m_hDeviceCaps.NumberFeatureButtonCaps;

	m_pHidDevice->m_pFeatureButtonCaps = l_pButtonCaps = (PHIDP_BUTTON_CAPS) new BYTE[l_nStructSize];

	if (l_pButtonCaps == NULL)
	{
		//TODO: need to find exception for it 
	}

	memset(l_pButtonCaps, '\0', l_nStructSize);


	l_usNumCaps = m_pHidDevice->m_hDeviceCaps.NumberFeatureButtonCaps;

	HidP_GetButtonCaps(HidP_Feature, l_pButtonCaps,
		&l_usNumCaps, m_pHidDevice->m_pPreParsedData);

	m_pHidDevice->m_ulFeatureDataLength
		= m_pHidDevice->m_hDeviceCaps.NumberFeatureButtonCaps;

	l_nStructSize =
		sizeof (HID_DATA)* m_pHidDevice->m_ulFeatureDataLength;

	m_pHidDevice->m_pFeatureData = l_pDeviceData
		= (PHID_DATA) new BYTE[l_nStructSize];

	if (l_pDeviceData == NULL)
	{
		//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
	}

	memset(l_pDeviceData, '\0', l_nStructSize);

	for (l_ulCount = 0;
		l_ulCount < m_pHidDevice->m_hDeviceCaps.NumberFeatureButtonCaps;
		l_ulCount++, l_pDeviceData++, l_pButtonCaps++)
	{
		l_pDeviceData->m_boIsButtonData = TRUE;
		l_pDeviceData->m_ulStatus = HIDP_STATUS_SUCCESS;
		l_pDeviceData->m_usUsagePage = l_pButtonCaps->UsagePage;
		l_pDeviceData->m_boIsAbsolute = l_pButtonCaps->IsAbsolute;
		if (l_pButtonCaps->IsRange)
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin =
				l_pButtonCaps->Range.UsageMin;
			l_pDeviceData->m_stButtonData.m_ulUsageMax =
				l_pButtonCaps->Range.UsageMax;
		}
		else
		{
			l_pDeviceData->m_stButtonData.m_ulUsageMin =
				l_pDeviceData->m_stButtonData.m_ulUsageMax =
				l_pButtonCaps->NotRange.Usage;
		}

		l_pDeviceData->m_stButtonData.m_ulMaxUsageLength =
			HidP_MaxUsageListLength(HidP_Feature, l_pButtonCaps->UsagePage,
			m_pHidDevice->m_pPreParsedData);

		l_nLength = l_pDeviceData->m_stButtonData.m_ulMaxUsageLength + 1;
		l_pDeviceData->m_stButtonData.m_psUsages
			= (PUSAGE) new BYTE[(l_nLength)* sizeof (USAGE)];
		if (l_pDeviceData->m_stButtonData.m_psUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}
		memset(l_pDeviceData->m_stButtonData.m_psUsages, '\0',
			(l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_stButtonData.m_psPrevUsages
			= (PUSAGE) new BYTE[(l_nLength)* sizeof (USAGE)];
		if (l_pDeviceData->m_stButtonData.m_psPrevUsages == NULL)
		{
			//TODO: need to put exception NullReferenceException("Memory couldnot be allocated")
		}
		memset(l_pDeviceData->m_stButtonData.m_psPrevUsages, '\0',
			(l_nLength)* sizeof (USAGE));

		l_pDeviceData->m_ulReportID = l_pButtonCaps->ReportID;
	}
}