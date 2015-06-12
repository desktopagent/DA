
#ifndef USB_HID_DEF_H
#define USB_HID_DEF_H

//******************************************************************************

#include <wtypes.h>

extern "C" {
#include "setupapi.h"
#include "hidsdi.h"
}

#define     HID_EXPECTED_NUM_OF_DEVICES         64
#define     SECOM_USB_HID_VENDOR_ID             0x1395
#define     SECOM_USB_HID_NAME_BUFFER           562

#define     SECOM_USB_TELEPHONY_USAGE_PAGE              0x0B

#define     SECOM_USB_LED_USAGE_PAGE                    0x08

/* Constants for Capricorn devices */
#define     SECOM_USB_CAPRICORN_USAGE_PAGE              0xFFFF
#define     SECOM_USB_CAPRICORN_USAGE                   ((USAGE) 0x0001)

typedef struct
{
	ULONG       m_ulUsageMin;       // Variables to track the usage minimum and max
	ULONG       m_ulUsageMax;       // If equal, then only a single usage
	ULONG       m_ulMaxUsageLength; // Usages buffer length.
	PUSAGE      m_psPrevUsages;     // list of usages (buttons ``down'' on the device .
	PUSAGE      m_psUsages;         // list of usages (buttons ``down'' on the device.

}ButtonData;


typedef struct _HID_DATA
{
	BOOLEAN     m_boIsButtonData;
	USAGE       m_usUsagePage;		// The usage page for which we are looking.
	ULONG       m_ulStatus;			// The last status returned from the accessor function
	// when updating this field.
	ULONG       m_ulReportID;		// ReportID for this given data structure
	//  has already been added to a report structure
	BOOLEAN     m_boSet;			// Call HidP_SetUsages if TRUE, otherwise call HidP_UnsetUsages

	ButtonData  m_stButtonData;
	bool        m_boIsAbsolute;

} HID_DATA, *PHID_DATA;

typedef struct _HID_DEVICE
{
	HANDLE                  m_hDeviceHandle;		// A file handle to the hid device.
	HANDLE                  m_hHidDeviceOverlapped;

	PHIDP_PREPARSED_DATA    m_pPreParsedData;		// The opaque parser info describing this device
	HIDP_CAPS               m_hDeviceCaps;			// The Capabilities of this hid device.

	PCHAR                   m_pcInputReportBuffer;
	PHID_DATA               m_pInputData;			// array of hid data structures
	ULONG                   m_ulInputDataLength;	// Num elements in this array.
	PHIDP_BUTTON_CAPS       m_pInputButtonCaps;

	PCHAR                   m_pcOutputReportBuffer;
	PHID_DATA               m_pOutputData;
	ULONG                   m_ulOutputDataLength;
	PHIDP_BUTTON_CAPS       m_pOutputButtonCaps;

	PCHAR                   m_pcFeatureReportBuffer;
	PHID_DATA               m_pFeatureData;
	ULONG                   m_ulFeatureDataLength;
	PHIDP_BUTTON_CAPS       m_pFeatureButtonCaps;

} HID_DEVICE, *PHID_DEVICE;

#endif