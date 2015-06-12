#include "DeviceInfo.h"

DeviceInfo::DeviceInfo()
{

}

DeviceInfo::DeviceInfo(const wstring& vId, const wstring& dId, const wstring& product, const wstring& manufacturer,
	const wstring& pId, const wstring& fwVer, const wstring& serNum, const int &rtCounter)
{
	setVendorId(vId);
	setDeviceId(dId);
	setIProduct(product);
	setIManufacturer(manufacturer);
	setProductId(pId);
	setFirmwareVersion(fwVer);
	setSerialNumber(serNum);
	setRetryCounter(rtCounter);
	setDevicePluginStatus(UNKNOWN);

}

void DeviceInfo::setDevicePluginStatus(const DevicePluginStatus &dvcPluginStatus)
{
	devicePluginStatus = dvcPluginStatus;
}

DevicePluginStatus DeviceInfo::getDevicePluginStatus()
{
	return devicePluginStatus;
}

void DeviceInfo::setVendorId(const wstring& vId)
{
	vendorId = vId;
}

wstring DeviceInfo::getVendorId()
{
	return vendorId;
}

void DeviceInfo::setDeviceId(const wstring& dId)
{
	deviceId = dId;
}

wstring DeviceInfo::getDeviceId()
{
	return deviceId;
}

void DeviceInfo::setIProduct(const wstring& product)
{
	iProduct = product;
}

wstring DeviceInfo::getIProduct()
{
	return iProduct;
}

void DeviceInfo::setIManufacturer(const wstring& manufacturer)
{
	iManufacturer = manufacturer;
}

wstring DeviceInfo::getIManufacturer()
{
	return iManufacturer;
}

void DeviceInfo::setProductId(const wstring& pId)
{
	productId = pId;
}

wstring DeviceInfo::getProductId()
{
	return productId;
}

void DeviceInfo::setFirmwareVersion(const wstring& fwVer)
{
	firmwareVersion = fwVer;
}

wstring DeviceInfo::getFirmwareVersion()
{
	return firmwareVersion;
}

void DeviceInfo::setSerialNumber(const wstring& serNum)
{
	serialNumber = serNum;
}

wstring DeviceInfo::getSerialNumber()
{
	return serialNumber;
}

value DeviceInfo::toJSON()
{
	value json;
	json[L"IDVendor"] = value::string(getVendorId());
	json[L"DeviceID"] = value::string(getDeviceId());
	json[L"IProduct"] = value::string(getIProduct());
	json[L"IDProduct"] = value::string(getProductId());
	json[L"FWVersion"] = value::string(getFirmwareVersion());
	json[L"ISerialNo"] = value::string(getSerialNumber());
	json[L"IManufacturer"] = value::string(getIManufacturer());

	//json[L"IDVendor"] = value::string(L"");
	//json[L"DeviceID"] = value::string(L"");
	//json[L"IProduct"] = value::string(L"");
	//json[L"IDProduct"] = value::string(L"");
	//json[L"FWVersion"] = value::string(L"");
	//json[L"ISerialNo"] = value::string(L"");
	//json[L"IManufacturer"] = value::string(L"");

	return json;
}

value DeviceInfo::toDisConnectJSON()
{
	value json;
	json[L"IDVendor"] = value::string(getVendorId());
	json[L"DeviceID"] = value::string(getDeviceId());
	json[L"IDProduct"] = value::string(getProductId());
	json[L"ISerialNo"] = value::string(getSerialNumber());
	
	return json;
}


void DeviceInfo::setRetryCounter(const int &rtCounter)
{
	retryCounter = rtCounter;
}

int DeviceInfo::getRetryCounter()
{
	return retryCounter;
}

void DeviceInfo::decreaseRetryCounter()
{
	--retryCounter;
}