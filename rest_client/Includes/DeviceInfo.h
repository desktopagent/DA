#pragma once

#include <cpprest/json.h>
#include <string>
#include <iostream>
using namespace std;
using namespace web::json;

enum DevicePluginStatus{
	UNKNOWN,
	TRYING,
	CONNECTED,
	DISCONNECTED
};

class DeviceInfo 
{
private:
	int retryCounter;
	DevicePluginStatus devicePluginStatus;
	wstring vendorId;
	wstring deviceId;
	wstring iProduct;
	wstring iManufacturer;
	wstring productId;
	wstring firmwareVersion;
	wstring serialNumber;
public:
	DeviceInfo();
	DeviceInfo(const wstring& vId, const wstring& dId, const wstring& product, const wstring& manufacturer,
		const wstring& pId, const wstring& fwVer, const wstring& serNum, const int &rtCounter);
	void setRetryCounter(const int &rtCounter);
	int getRetryCounter();
	void decreaseRetryCounter();
	void setDevicePluginStatus(const DevicePluginStatus &dvcPluginStatus);
	DevicePluginStatus getDevicePluginStatus();
	void setVendorId(const wstring& vId);
	wstring getVendorId();
	void setDeviceId(const wstring& dId);
	wstring getDeviceId();
	void setIProduct(const wstring& product);
	wstring getIProduct();
	void setIManufacturer(const wstring& manufacturer);
	wstring getIManufacturer();
	void setProductId(const wstring& pId);
	wstring getProductId();
	void setFirmwareVersion(const wstring& fwVer);
	wstring getFirmwareVersion();
	void setSerialNumber(const wstring& serNum);
	wstring getSerialNumber();
	value toJSON();
	value toDisConnectJSON();
};
