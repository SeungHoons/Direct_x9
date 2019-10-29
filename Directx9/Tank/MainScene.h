#pragma once
#include "Scene.h"
#include "Terrain.h"
#include "ZFrustum.h"
class Camera;
struct CUSTOMVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR3	t;
};
#define D3DFVF_CUSTOMVVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

struct MYINDEX
{
	WORD _0, _1, _2;
};
class MainScene :
	public Scene
{
private:
	Camera* m_pCamera;
	LPDIRECT3DVERTEXBUFFER9		m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9		m_pIB = NULL;
	LPDIRECT3DTEXTURE9			m_pTexHeight = NULL;
	LPDIRECT3DTEXTURE9			m_pTexDiffuse = NULL;
	D3DXMATRIXA16				m_matAni;
	D3DXMATRIXA16				m_matWorld;
	D3DXMATRIXA16				m_matView;
	D3DXMATRIXA16				m_matProj;
	DWORD						m_cxHeight = 0;
	DWORD						m_czHeight = 0;
	DWORD						m_dwMouseX = 0;
	DWORD						m_dwMouseY = 0;

	BOOL					m_bHideFrustum = TRUE;	// Frustum을 안그릴 것인가?
	BOOL					m_bLockFrustum = FALSE;	// Frustum을 고정할 것인가?
	BOOL					m_bWireframe = FALSE;	// 와이어프레임으로 그릴것인가?

	ZFrustum*			        m_pFrustum = NULL;	// Frustum 클래스
	Terrain*			        m_pTerrain = NULL;

public:
	MainScene();
	~MainScene();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void release();

	HRESULT InitTexture();
	HRESULT InitVB();
	HRESULT InitIB();
	void SetupCamera();
	HRESULT InitGeometry();
	void	ProcessMouse();
	void	ProcessKey();
	void	ProcessInputs();
	void	SetupLights();
	void	LogFPS();
	void	DrawMesh(D3DXMATRIXA16* pMat);
};

