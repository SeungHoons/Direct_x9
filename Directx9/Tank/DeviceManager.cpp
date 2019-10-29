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

	//주 그래픽카드의 정보를 D3DCAPS9 에 받아온다.
	if (FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, &caps)))
		return E_FAIL;

	//하드웨어가 정점처리를 지원하는지 확인
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;				//Z(깊이)버퍼를 사용하겠다 설정
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//유효한 깊이 버퍼 포맷을 설정하는것 / 대입한 D3DFMT_D16은 16비트의 깊이 버퍼를 사용할수 있는 경우에 지정합니다.
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;		//최대 초당 프레임이 해당 모니터 주사율과 동일하게 나오도록 디폴트 되어있는데 그것을 풀어주는 역할
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//다이렉트가 적절한 값을 찾아 렌더링 간격을 조정한다.
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
