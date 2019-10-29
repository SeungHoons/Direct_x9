#include "stdafx.h"
#include "MainScene.h"
#include "Camera.h"


MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	m_pCamera = new Camera();
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	if (FAILED(InitTexture()))
	{
	}

	if (FAILED(InitVB()))
	{
	}

	if (FAILED(InitIB()))
	{
	}

	SetupCamera();
	// 최초의 마우스 위치 보관
	POINT	pt;
	GetCursorPos(&pt);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;

	const char*	tex[256] = { "tile2.tga", "lightmap.tga", "", "" };
	D3DXVECTOR3	vScale;

	vScale.x = vScale.z = 1.0f; vScale.y = .1f;
	g_pLog = new ZFLog(ZF_LOG_TARGET_WINDOW | ZF_LOG_TARGET_FILE, "log.txt");
	m_pFrustum = new ZFrustum();
	m_pTerrain = new Terrain();
	if (FAILED(m_pTerrain->Create(g_pDevice, &vScale, 0.05f, "map159.bmp", tex)))
	{

	}

}

void MainScene::Update()
{
	D3DXMatrixIdentity(&m_matAni);
	SetupLights();
	ProcessInputs();
	D3DXMATRIXA16	m;
	D3DXMATRIXA16	*pView;
	pView = m_pCamera->GetViewMatrix();	// 카메라 클래스로부터 행렬정보를 얻는다.
	m = *pView * m_matProj;				// World좌표를 얻기위해서 View * Proj행렬을 계산한다.
	if (!m_bLockFrustum) m_pFrustum->Make(&m);	// View*Proj행렬로 Frustum을 만든다.
}

void MainScene::Render()
{
	g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
	g_pDevice->SetRenderState(D3DRS_FILLMODE, m_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	if (SUCCEEDED(g_pDevice->BeginScene()))
	{
		m_pTerrain->Draw(m_pFrustum);
		if (!m_bHideFrustum) m_pFrustum->Draw(g_pDevice);

		//DrawMesh(&m_matAni);

		g_pDevice->EndScene();
	}

	g_pDevice->Present(NULL, NULL, NULL, NULL);
}

void MainScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void MainScene::release()
{
}

HRESULT MainScene::InitTexture()
{
	if (FAILED(D3DXCreateTextureFromFileEx(g_pDevice, "map128.bmp", D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
		&m_pTexHeight)))
		return E_FAIL;

	if (FAILED(D3DXCreateTextureFromFile(g_pDevice, "tile2.tga", &m_pTexDiffuse)))
		return E_FAIL;

	return S_OK;
}

HRESULT MainScene::InitVB()
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT	d3drc;

	m_pTexHeight->GetLevelDesc(0, &ddsd);
	m_cxHeight = ddsd.Width;
	m_czHeight = ddsd.Height;
	//ZFLog::GetInstance()->Log("Texturel Size:[%d,%d]", g_cxHeight, g_czHeight);
	if (FAILED(g_pDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height * sizeof(CUSTOMVERTEX), 0,
		D3DFVF_CUSTOMVVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		return E_FAIL;

	m_pTexHeight->LockRect(0, &d3drc, NULL, D3DLOCK_READONLY);
	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, m_cxHeight * m_czHeight * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
		return E_FAIL;

	CUSTOMVERTEX v;
	CUSTOMVERTEX* pV = (CUSTOMVERTEX*)pVertices;
	for (DWORD z = 0; z < m_czHeight; z++)
	{
		for (DWORD x = 0; x < m_cxHeight; x++)
		{
			v.p.x = (float)x - m_cxHeight / 2.0f;
			v.p.z = -((float)z - m_czHeight / 2.0f);
			v.p.y = ((float)(*((LPDWORD)d3drc.pBits + x + z * (d3drc.Pitch / 4)) & 0x0000ff)) / 10.0f;
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;
			D3DXVec3Normalize(&v.n, &v.n);
			v.t.x = (float)x / (m_cxHeight - 1);
			v.t.y = (float)z / (m_czHeight - 1);
			*pV++ = v;
			//g_pLog->Log("[%f ,%f ,%f]", v.p.x, v.p.y, v.p.z);
		}
	}

	m_pVB->Unlock();
	m_pTexHeight->UnlockRect(0);
	return S_OK;
}

HRESULT MainScene::InitIB()
{
	if (FAILED(g_pDevice->CreateIndexBuffer((m_cxHeight - 1) * (m_czHeight - 1) * 2 * sizeof(MYINDEX), 0,
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
		return E_FAIL;

	MYINDEX		i;
	MYINDEX*	pI;
	if (FAILED(m_pIB->Lock(0, (m_cxHeight - 1) * (m_czHeight - 1) * 2 * sizeof(MYINDEX), (void**)&pI, 0)))
		return E_FAIL;

	for (DWORD z = 0; z < m_czHeight - 1; z++)
	{
		for (DWORD x = 0; x < m_cxHeight - 1; x++)
		{
			i._0 = (z*m_cxHeight + x);
			i._1 = (z*m_cxHeight + x + 1);
			i._2 = ((z + 1)*m_cxHeight + x);
			*pI++ = i;

			i._0 = ((z + 1)*m_cxHeight + x);
			i._1 = (z*m_cxHeight + x + 1);
			i._2 = ((z + 1)*m_cxHeight + x + 1);
			*pI++ = i;
		}
	}

	m_pIB->Unlock();
	return S_OK;
}

void MainScene::SetupCamera()
{
	/// 월드 행렬 설정
	D3DXMatrixIdentity(&m_matWorld);
	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	/// 뷰 행렬을 설정
	D3DXVECTOR3 vEyePt(0.0f, 50.0f, (float)-30.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matWorld, &vEyePt, &vLookatPt, &vUpVec);
	g_pDevice->SetTransform(D3DTS_VIEW, &m_matView);

	/// 실제 프로젝션 행렬
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	/// 프러스텀 컬링용 프로젝션 행렬
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 200.0f);

	/// 카메라 초기화
	m_pCamera->SetView(&vEyePt, &vLookatPt, &vUpVec);
}

HRESULT MainScene::InitGeometry()
{
	return E_NOTIMPL;
}

void MainScene::ProcessMouse()
{
	POINT pt;
	float fDelta = 0.001f;
	GetCursorPos(&pt);
	int dx = pt.x - m_dwMouseX;
	int dy = pt.y - m_dwMouseY;

	m_pCamera->RotateLocalX(dy * fDelta);
	m_pCamera->RotateLocalY(dx * fDelta);
	D3DXMATRIXA16* pmatView = m_pCamera->GetViewMatrix();
	g_pDevice->SetTransform(D3DTS_VIEW, pmatView);

	RECT	rc;
	GetClientRect(g_hWnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	m_dwMouseX = pt.x;
	m_dwMouseY = pt.y;
}

void MainScene::ProcessKey()
{
	//g_pKeyaManager->IsStayKeyDown
	if (g_pKeyaManager->IsStayKeyDown('W'))
	{
		m_pCamera->MoveLocalZ(0.5f);
	}
	if (g_pKeyaManager->IsStayKeyDown('S'))
	{
		m_pCamera->MoveLocalZ(-0.5f);
	}
	if (GetAsyncKeyState(VK_LBUTTON))
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (GetAsyncKeyState(VK_RBUTTON))
		g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void MainScene::ProcessInputs()
{
	ProcessMouse();
	ProcessKey();
}

void MainScene::SetupLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	//vecDir = D3DXVECTOR3(1, 1, 1);
	vecDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f), 1.0f, sinf(GetTickCount() / 350.0f));
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.0f;

	g_pDevice->SetLight(0, &light);
	g_pDevice->LightEnable(0, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pDevice->SetRenderState(D3DRS_AMBIENT, 0x00909090);
}

void MainScene::LogFPS()
{
}

void MainScene::DrawMesh(D3DXMATRIXA16 * pMat)
{
	g_pDevice->SetTransform(D3DTS_WORLD, pMat);
	g_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pDevice->SetFVF(D3DFVF_CUSTOMVVERTEX);
	g_pDevice->SetIndices(m_pIB);
	g_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_cxHeight * m_czHeight, 0,
		(m_cxHeight - 1) * (m_czHeight - 1) * 2);
}
