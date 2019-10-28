#include "stdafx.h"
#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}

void Application::Init()
{
	g_pDeviceManager->Init();
}

void Application::Destroy()
{
	g_pDeviceManager->Destroy();
}

void Application::Update()
{
}

void Application::Render()
{
}

void Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}
