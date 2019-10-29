#include "ZFLog.h"
#include "Terrain.h"
#include "dib.h"
#include "GlobalFunc.h"
#include "Dib.h"

Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

HRESULT	Terrain::Create(LPDIRECT3DDEVICE9 pDev, D3DXVECTOR3* pvfScale, float fLODRatio, const char* lpBMPFilename, const char* lpTEXFilename[4])
{
	m_pd3dDevice = pDev;
	m_vfScale = *pvfScale;
	//m_fLODRatio = fLODRatio;
	if (FAILED(_BuildHeightMap(lpBMPFilename))) { _Destroy(); return E_FAIL; }
	if (FAILED(_LoadTextures(lpTEXFilename))) { _Destroy(); return E_FAIL; }
	if (FAILED(_CreateVIB())) { _Destroy(); return E_FAIL; }
	//m_pQuadTree = new ZQuadTree(m_cxDIB, m_czDIB);
	if (FAILED(_BuildQuadTree())) { _Destroy(); return E_FAIL; }

	return S_OK;
}

/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
HRESULT	Terrain::_Destroy()
{
	//DEL(m_pQuadTree);
	DELS(m_pvHeightMap);
	REL(m_pVB);
	REL(m_pIB);
	for (int i = 0; i < 4; i++) REL(m_pTex[i]);

	return S_OK;
}

/// ������ü���� ����� �ؽ�ó�� �о���δ�.
HRESULT	Terrain::_LoadTextures(const char* lpTexFilename[4])
{
	for (int i = 0; i < 4; i++)
		D3DXCreateTextureFromFile(m_pd3dDevice, lpTexFilename[i], &m_pTex[i]);

	return S_OK;
}

/// BMP������ ��� ���̸��� �����Ѵ�.
HRESULT	Terrain::_BuildHeightMap(const char* lpFilename)
{
	int		n;
	LPBYTE	pDIB = DibLoadHandle(lpFilename);
	if (!pDIB) return E_FAIL;

	m_cxDIB = DIB_CX(pDIB);
	m_czDIB = DIB_CY(pDIB);

	// ���Ӱ� �߰��� ��ƾ
	// m_cxDIB�� m_czDIB�� (2^n+1)�� �ƴѰ�� E_FAIL�� ��ȯ
	n = Log2(m_cxDIB);
	if ((Pow2(n) + 1) != m_cxDIB) return E_FAIL;
	n = Log2(m_czDIB);
	if ((Pow2(n) + 1) != m_czDIB) return E_FAIL;

	m_pvHeightMap = new TERRAINVERTEX[m_cxDIB * m_czDIB];

	TERRAINVERTEX v;
	for (int z = 0; z < m_czDIB; z++)
	{
		for (int x = 0; x < m_cxDIB; x++)
		{
			v.p.x = (float)((x - m_cxDIB / 2) * m_vfScale.x);
			v.p.z = -(float)((z - m_czDIB / 2) * m_vfScale.z);
			v.p.y = (float)(*(DIB_DATAXY_INV(pDIB, x, z))) * m_vfScale.y;
			D3DXVec3Normalize(&v.n, &v.p);
			v.t.x = (float)x / (float)(m_cxDIB - 1);
			v.t.y = (float)z / (float)(m_czDIB - 1);
			m_pvHeightMap[x + z * m_czDIB] = v;
		}
	}

	DibDeleteHandle(pDIB);
	return S_OK;
}

/// BMP������ ũ�⿡ ���缭 ����Ʈ���� �����Ѵ�.
HRESULT	Terrain::_BuildQuadTree()
{
	m_pQuadTree->Build(m_pvHeightMap);
	return S_OK;
}

/// ����, �ε��� ���۸� �����Ѵ�.
HRESULT	Terrain::_CreateVIB()
{
	// VB����
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(m_cxDIB*m_czDIB * sizeof(TERRAINVERTEX),
		0, TERRAINVERTEX::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		_Destroy();
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, m_cxDIB*m_czDIB * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
	{
		_Destroy();
		return E_FAIL;
	}
	memcpy(pVertices, m_pvHeightMap, m_cxDIB*m_czDIB * sizeof(TERRAINVERTEX));
	m_pVB->Unlock();

#ifdef _USE_INDEX16
	// IB����
	if (FAILED(m_pd3dDevice->CreateIndexBuffer((m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		_Destroy();
		return E_FAIL;
	}
	g_pLog->Log("Using 16bit index buffer...");
#else
	// IB����
	if (FAILED(m_pd3dDevice->CreateIndexBuffer((m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		_Destroy();
		return E_FAIL;
	}
	g_pLog->Log("Using 32bit index buffer...");
#endif
	return S_OK;
}

/// ȭ�鿡 ������ ����Ѵ�.
HRESULT	Terrain::_Render()
{
	m_pd3dDevice->SetTexture(0, m_pTex[0]);								// 0�� �ؽ��� ���������� �ؽ��� ����(�����)
	m_pd3dDevice->SetTexture(1, m_pTex[1]);								// 1�� �ؽ��� ���������� �ؽ��� ����(������)
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0�� �ؽ�ó ���������� Ȯ�� ����
	m_pd3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0�� �ؽ�ó ���������� Ȯ�� ����
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);		// 1�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// MODULATE�� ���´�.
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �ؽ�ó
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// ������
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);	// MODULATE2�� ���´�.
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �ؽ�ó
	m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);		// �����
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(TERRAINVERTEX));
	m_pd3dDevice->SetFVF(TERRAINVERTEX::FVF);
	m_pd3dDevice->SetIndices(m_pIB);
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_cxDIB * m_czDIB, 0, m_nTriangles);

	return S_OK;
}

/// ȭ�鿡 ������ ����Ѵ�.
HRESULT	Terrain::Draw(ZFrustum* pFrustum)
{
	LPDWORD		pI;

	if (FAILED(m_pIB->Lock(0, (m_cxDIB - 1)*(m_czDIB - 1) * 2 * sizeof(TRIINDEX), (void**)&pI, 0)))
		return E_FAIL;
	m_nTriangles = m_pQuadTree->GenerateIndex(pI, m_pvHeightMap, pFrustum, m_fLODRatio);
	m_pIB->Unlock();
	_Render();

	return S_OK;
}
