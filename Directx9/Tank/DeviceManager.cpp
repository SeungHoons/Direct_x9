#include "stdafx.h"
#include "DeviceManager.h"



DeviceManager::DeviceManager()
{
	
}


DeviceManager::~DeviceManager()
{
}

HRESULT DeviceManager::Init()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL) return E_FAIL;

	D3DCAPS9	caps;
	int			vp;

	//�� �׷���ī���� ������ D3DCAPS9 �� �޾ƿ´�.
	if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;

	//�ϵ��� ����ó���� �����ϴ��� Ȯ��
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;				//Z(����)���۸� ����ϰڴ� ����
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//��ȿ�� ���� ���� ������ �����ϴ°� / ������ D3DFMT_D16�� 16��Ʈ�� ���� ���۸� ����Ҽ� �ִ� ��쿡 �����մϴ�.
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//�ִ� �ʴ� �������� �ش� ����� �ֻ����� �����ϰ� �������� ����Ʈ �Ǿ��ִµ� �װ��� Ǯ���ִ� ����
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//���̷�Ʈ�� ������ ���� ã�� ������ ������ �����Ѵ�.
	/*if (FAILED(m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, vp, &d3dpp, &m_pD3DDevice)))
	{
		return E_FAIL;
	}*/

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		return E_FAIL;

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}

LPDIRECT3DDEVICE9 DeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

void DeviceManager::Destroy()
{
}
