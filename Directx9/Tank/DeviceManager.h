#pragma once

#define g_pDeviceManager	DeviceManager::GetInstance()
#define g_pDevice			DeviceManager::GetInstance()->GetDevice()

class DeviceManager
{
private:
	DeviceManager();
	~DeviceManager();

	LPDIRECT3DDEVICE9	m_pD3DDevice;
	LPDIRECT3D9			m_pD3D;
	

public:
	static DeviceManager* GetInstance()
	{
		static DeviceManager instance;
		return				&instance;
	}

	HRESULT				Init();
	LPDIRECT3DDEVICE9	GetDevice();
	void				Destroy();
};

