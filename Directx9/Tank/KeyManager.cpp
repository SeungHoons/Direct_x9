#include "stdafx.h"
#include "KeyManager.h"


KeyManager::KeyManager()
{
}


KeyManager::~KeyManager()
{
}

HRESULT KeyManager::Init()
{
	//키 값 초기화
	for (int i = 0; i < m_kKeyMax; i++)
	{
		m_keyUp.set(i, false);
		m_keyDown.set(i, false);
	}
	return S_OK;
}

void KeyManager::Update()
{
}

void KeyManager::Destroy()
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0X8000)
	{
		if (m_keyDown[key])
		{
		}
		else
		{
			m_keyDown.set(key, true);
		}
	}
	else
	{
		m_keyDown.set(key, false);
	}
	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_keyUp.set(key, true);
	}
	else
	{
		if (m_keyUp[key])
		{
			m_keyUp.set(key, false);
			return true;
		}
		else
		{
			/* Do nothing */
		}
	}
	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

bool KeyManager::IsToggleKey(int key)
{
	if (GetKeyState(key) & 0x0001)
		return true;
	return false;
}
