#pragma once

#define WIN32_LEAN_AND_MEAN
//������ �ʴ� �������� ��� ������ ����
//#define NOMINMAX

#include <Windows.h>

#include <vector>

#include <d3d9.h>
#include <d3dx9.h>
//#include "ZCamera.h"
//#include "ZFrustum.h"
//#include "ZTerrain.h"
//#include "ZFLog.h"
#include "SAFE_DELETE.h"
#include "Dib.h"

//��ó����� ����
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

//Ű ���¸� üũ�ϱ� ���� �ڷᱸ��
#include <bitset>
using std::bitset;

//������ 
extern HWND g_hWnd;

//���� ���
#include "Application.h"		//�����ӿ�ũ ���
#include "DeviceManager.h"		//����̽��� ����
#include "KeyManager.h"			//����̽� ���鿡���� Ű ����


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#define D3DX_DEBUG