#include "Sun.h"



Sun::Sun()
{
	m_rotSpeed = 0;
	m_rot = 0;
}


Sun::~Sun()
{
}

void Sun::init(LPDIRECT3DDEVICE9 _device, LPDIRECT3DVERTEXBUFFER9 _pVb,
	LPDIRECT3DINDEXBUFFER9 _pIb)
{
	m_pd3dDevice = _device;
	m_pVB = _pVb;
	m_pIB = _pIb;

	if (!m_vecChiled.empty())
	{
		for (auto iter = m_vecChiled.begin(); iter != m_vecChiled.end(); iter++)
		{
			(*iter)->init(m_pd3dDevice, m_pVB, m_pIB);
		}
	}
}

void Sun::render(D3DXMATRIXA16* _parentMat)
{
	animate();
	D3DXMATRIXA16 my_mat;

	D3DXMATRIXA16 tempR;
	D3DXMatrixRotationY(&tempR, m_rot*0.1f);

	my_mat = m_matR * m_matTM * (tempR *(*_parentMat));
	DrawMesh(&my_mat);
	if (!m_vecChiled.empty())
	{
		for (auto iter = m_vecChiled.begin(); iter != m_vecChiled.end(); iter++)
		{
			(*iter)->render(&my_mat);
		}
	}

}

void Sun::animate()
{
	if (m_rot > 62.4f)
		m_rot = 0.0f;
	D3DXMatrixTranslation(&m_matTM, m_v.x, m_v.y, m_v.z);
	D3DXMatrixRotationY(&m_matR, m_rot);
	m_rot += m_rotSpeed;
}

void Sun::DrawMesh(D3DXMATRIXA16 * pMat)
{
	m_pd3dDevice->SetTransform(D3DTS_WORLD, pMat);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));
	m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void Sun::setChiled(Sun& chiled, float distance)
{
	m_vecChiled.push_back(&chiled);
}

void Sun::setAngle(float _rot)
{
	m_rotSpeed = _rot;
}

void Sun::setPosition(D3DXVECTOR3 _v)
{
	m_v = _v;
}


