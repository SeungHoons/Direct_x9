#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::init()
{
	m_LastTime = std::chrono::system_clock::now();
	m_speed = 3;
	m_clicked = false;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

void Camera::update()
{
	std::chrono::duration<float> sec = std::chrono::system_clock::now() - m_LastTime;
	if (sec.count() < (1 / FPS))
		return;

	input();

	m_fElapseTime = sec.count();
	m_LastTime = std::chrono::system_clock::now();
}

void Camera::input()
{
	if (GetKeyState(VK_UP) & 0x8000)
	{
		if (!m_clicked)
		{
			m_y = m_y - m_fElapseTime * m_speed;
		}
	}
	if (GetKeyState(VK_DOWN ) & 0x8000)
	{
		if (!m_clicked)
		{
			m_y = m_y + m_fElapseTime * m_speed;
		}
	}
	if (GetKeyState(VK_LEFT ) & 0x8000)
	{
		if (!m_clicked)
		{
			m_x = m_x + m_fElapseTime * m_speed;
		}
	}
	if (GetKeyState(VK_RIGHT ) & 0x8000)
	{
		if (!m_clicked)
		{
			m_x = m_x - m_fElapseTime * m_speed;
		}
	}
}

void Camera::test(D3DXVECTOR3 & _eye , D3DXVECTOR3 & _lookat)
{
	_eye = { m_x, m_y , m_z - 10.0f };
	_lookat = { m_x, m_y , m_z };
}
