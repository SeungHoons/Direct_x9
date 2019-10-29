#include "stdafx.h"
#include "Application.h"
#include "SceneManager.h"

Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	g_pDeviceManager->Init();
	g_pSceneManager->Init();
}

void Application::Destroy()
{
	g_pSceneManager->Release();
	g_pDeviceManager->Destroy();
}

void Application::Update()
{
	g_pSceneManager->Update();
}

void Application::Render()
{
	g_pSceneManager->Render();
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_pSceneManager->WndProc(hWnd, message, wParam, lParam);
}
