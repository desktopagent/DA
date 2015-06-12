#ifndef APP_MANAGER_H
#define APP_MANAGER_H

class CAppManager
{
public:
	static CAppManager* getInstance();
	~CAppManager();

private:
	CAppManager();

	void initializeVariables();

	void startHSFactory();

	static CAppManager*  appManageActiveInstance;

	// need to replace void with CHeadsetInfo class
	void* isDummyHeadset;

	bool iHSConnected;

	CHSManager*  m_hHSManager;
};

#endif