#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "SAFE_DELETE.h"
#include <vector>

struct CUSTOMVERTEX
{
	FLOAT x, y, z;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

class Sun
{
	float m_rot;
	float m_rotSpeed;
	D3DXVECTOR3 m_v;
	D3DXMATRIXA16	m_matTM;
	D3DXMATRIXA16	m_matR;
	std::vector<Sun*> m_vecChiled;
	LPDIRECT3DDEVICE9 m_pd3dDevice; 
	LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9 m_pIB = NULL;

public:
	Sun();
	~Sun();

	void init(LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXBUFFER9 _pVb,
		LPDIRECT3DINDEXBUFFER9 _pIb);
	void render(D3DXMATRIXA16* _parentMat);
	void animate();
	void DrawMesh(D3DXMATRIXA16* pMat);
	void setChiled(Sun& chiled, float distance);
	void setAngle(float _rot);
	void setPosition(D3DXVECTOR3 _v);
	
};

