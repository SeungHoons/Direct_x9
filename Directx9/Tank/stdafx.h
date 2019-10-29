#pragma once

#define WIN32_LEAN_AND_MEAN
//사용되지 않느 ㄴ윈도우 헤더 컴파일 제외
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

//전처리기로 세팅
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")

//키 상태를 체크하기 위한 자료구조
#include <bitset>
using std::bitset;

//윈도우 
extern HWND g_hWnd;

//마이 헤더
#include "Application.h"		//프레임워크 기능
#include "DeviceManager.h"		//디바이스를 관리
#include "KeyManager.h"			//디바이스 측면에서의 키 관리


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif
#define D3DX_DEBUG