#pragma once
#include <d3dx9.h>
#include <mmsystem.h>
#include "SAFE_DELETE.h"
#include <time.h>
#include <chrono>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
using namespace std;

#define	FPS 60

class Camera
{
	float m_x;
	float m_y;
	float m_z;
	bool m_clicked;
	float m_speed;
	D3DXVECTOR3* m_pEye;
	D3DXVECTOR3* m_pLootat;


	chrono::system_clock::time_point m_LastTime;
	float		m_fElapseTime;
public:
	Camera();
	~Camera();

	void init();
	void update();
	void input();
	void test(D3DXVECTOR3 & _eye, D3DXVECTOR3 &_lookat);
};

