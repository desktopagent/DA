
#ifndef SECOM_HS_INTERFACE_DEFINES_H
#define SECOM_HS_INTERFACE_DEFINES_H

/** @enum HSEvents
* This is a list of the events that originate from the Headset.
*/
enum HSEvents
{
	HSEVT_CALLACCEPT /**< Call accepted */
	, HSEVT_CALLACCEPTFAILED /**< Call accept failed */
	, HSEVT_CALLINCOMINGFAILED /**< Call incoming failed */
	, HSEVT_CALLOUTGOINGFAILED /**< Call outgoing failed */
	, HSEVT_CALLUNHOLDFAILED /**< Call unhold failed */
	, HSEVT_CALLEND /**< Call ended */
	, HSEVT_CALLENDFAILED /**< Call end failed */
	, HSEVT_CALLREJECT /**< Call reject */
	, HSEVT_CALLREJECTFAILED /**< Call reject failed */
	, HSEVT_CALLMUTE /**< Mute status of the headset changed */
	, HSEVT_CALLUNMUTE /**< Mute status of the headset changed */
	, HSEVT_CALLMUTEFAILED /**< Mute failed */
	, HSEVT_CALLREDIAL /**< Redialing the last user */
	, HSEVT_CALLREDIALFAILED  /**< Redialing the last user failed */
	, HSEVT_HSCONNECTED /**< Connection established */
	, HSEVT_HSCONNECTFAILED /**< Connection establishment failed */
	, HSEVT_HSDISCONNECTED /**< Connection lost */
	, HSEVT_HSDISCONNECTFAILED /**< Disconnect headset failed */
	, HSEVT_HSDISCOVERED /**< Headset discovered in vicinity */
	, HSEVT_HSDISCOVEREFAILED /**< Headset discovery failed */
	, HSEVT_HSVOLUMECHANGE /**< Volume changed on headset */
	, HSEVT_HSVOLUMECHANGEFAILED /**< Volume changed on headset failed*/
	, HSEVT_HSPAIRED /**< Headset successfully paired */
	, HSEVT_HSPAIRFAILED /**< Headset successfully pair failed */
	, HSEVT_HSNOTAVAILABLE /**< Paired Headset not available */
	, HSEVT_HSAUTOCONNECT /**<Connect to this headset */
	, HSEVT_SETASDEFAULTHS /**<Set this headset as default */
	, HSEVT_REMOVEASDEFAULTHS /**<Set this headset as default */
	, HSEVT_OFFHOOK /**<Off Hook event from USB headset */
	, HSEVT_ONHOOK /**<On Hook event from USB headset */
	, HSEVT_SELECTPSTN /**<PSTN mode selected from base station */

	// TWC events - Start
	, HSEVT_TWC_ENDACTIVE_ANSWERINCOMING /**< Accept incoming & End Active */
	, HSEVT_TWC_HOLDCALL /**< TWC - Hold call */
	// TWC events - End
	, HSEVT_TWC_UNHOLDCALL 	/**< TWC - Unhold call */
	// TWC events - End
};



#endif