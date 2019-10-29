#include "stdafx.h"
#include "SceneManager.h"
#include "MainScene.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pNowScene = new MainScene();
	m_pNowScene->Init();
}

void SceneManager::Update()
{
	m_pNowScene->Update();
}

void SceneManager::Render()
{
	m_pNowScene->Render();
}

void SceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//m_pNowScene->WndProc( hWnd,  message,  wParam,  lParam);
}

void SceneManager::Release()
{
	m_pNowScene->release();
}
