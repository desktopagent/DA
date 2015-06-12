
#include "HSManager.h"
#include <iostream>

CHSManager* CHSManager::iHSActiveInstance = NULL;

CHSManager* CHSManager::getInstance(IHSEventListener* aHSEventListener)
{
	if (iHSActiveInstance == NULL)
	{
		iHSActiveInstance = new CHSManager(aHSEventListener);
	}
	return iHSActiveInstance;
}

CHSManager::CHSManager(IHSEventListener* aHSEventListener)
{
	iConnectedHSCount = 0;
	iDefaultHeadset = NULL;
	iToBeConnectedHeadset = NULL;

	iHSEventListener = aHSEventListener;
	CUsbHidImpl* l_hUSBObject;
	iUSBStack = NULL;

	l_hUSBObject = CUsbHidImpl::getInstance(this);
	iUSBStack = (IStackInterface*)(l_hUSBObject);
	iUSBStack->setEventListener((IHSEventListener *)this);
	
}

void CHSManager::enableCheckForUSBUsagePages()
{ 
	if (iUSBStack != NULL)
	{
		//iUSBStack->enableCheckForUSBUsagePages();
	}
}

void CHSManager::setEventListener(IHSEventListener* aEventListener)
{
	if (iUSBStack != NULL)
	{
		iUSBStack->setEventListener(aEventListener);
	}
}

bool CHSManager::handleHSEvents(HSEvents f_eHSEvent, void* aHeadset)
{ 
	CHeadsetInfo* lHeadsetInfo = (CHeadsetInfo*)aHeadset;

	switch (f_eHSEvent)
	{
	case HSEVT_HSCONNECTED:
		iConnectedHSCount = iConnectedHSCount + 1;
		if (lHeadsetInfo->iIsUsb == true)
		{
			ilstUSBHS.push_back(lHeadsetInfo);
		}
		else
		{
			iBTHSInUse = lHeadsetInfo;
		}

		iHSEventListener->handleHSEvents(f_eHSEvent, aHeadset);
		if ((iConnectedHSCount == 1) && (iToBeConnectedHeadset == NULL))
		{
			iDefaultHeadset = lHeadsetInfo;
			lHeadsetInfo->isDefault = true;
			iHSEventListener->handleHSEvents(HSEVT_SETASDEFAULTHS, aHeadset);
		}
		//else
		//{
	
		//	//if (CCommonUtilities::isSameHeadset(iToBeConnectedHeadset, aHeadset))
		//	{
		//		iToBeConnectedHeadset = NULL;

		//		iDefaultHeadset = lHeadsetInfo;
		//		iHSEventListener->handleHSEvents(HSEVT_SETASDEFAULTHS, aHeadset);
		//	}
		//}

		if ((iDefaultHeadset != NULL)
			&& (iDefaultHeadset->iHeadset != NULL))
		{
			// iDefaultHeadset->iHeadset->setCallControlState(iCallCtrlStateForMSOC);
		}
		break;
	case HSEVT_HSCONNECTFAILED:

		iHSEventListener->handleHSEvents(f_eHSEvent, aHeadset);
		break;

	case HSEVT_HSDISCONNECTED:
		if (0 < iConnectedHSCount)
		{
			iConnectedHSCount = iConnectedHSCount - 1;
		}
		iHSEventListener->handleHSEvents(f_eHSEvent, lHeadsetInfo);
		if ((iDefaultHeadset != NULL) && (isDefaultHS(lHeadsetInfo)))
		{
			iHSEventListener->handleHSEvents(HSEVT_REMOVEASDEFAULTHS, iDefaultHeadset);
			iDefaultHeadset = NULL;
		}
		updateILstUSBHS(lHeadsetInfo);
		//if ((lHeadsetInfo != NULL) && (lHeadsetInfo->iIsUsb))
		//{
		//	ilstUSBHS.pop_back();  
		//	delete lHeadsetInfo->iHeadset;
		//}
		break;
	}
	return true;
}

bool CHSManager::isDefaultHS(CHeadsetInfo* aHeadset)
{
	/*return true;*/ /* CCommonUtilities::isSameHeadset(m_hDefaultHeadset, aHeadset);*/
	return aHeadset->isDefault;
}


//void CHSManager::updateUSBHeadsetInList(list<CHeadsetInfo*> f_hlstUSBHSList)
//{
//	if (f_hlstUSBHSList.empty())
//	{
//		return;
//	}
//}

void CHSManager::updateILstUSBHS(CHeadsetInfo* f_hHeadset)
{
	std::list<CHeadsetInfo*>::iterator it;
	it = std::find(ilstUSBHS.begin(), ilstUSBHS.end(), f_hHeadset);
	if (it != ilstUSBHS.end())
	{
		ilstUSBHS.erase(it);
	}
	if (!ilstUSBHS.empty())
	{

		it = ilstUSBHS.begin();
		(*it)->isDefault = true;
		iDefaultHeadset = *it;
	}

};