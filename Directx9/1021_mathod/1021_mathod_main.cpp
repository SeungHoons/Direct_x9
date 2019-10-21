
#include <Windows.h>
#include <vector>
#include <math.h>
#include <utility>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("CIRCUS_ copyright _ Hoons");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpszCmpParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

//메트릭스 벡터 정의
//메트릭스 x 메트릭스
//메트릭스 x 벡터 
//이동 
//아이덴티디
struct MY_MATRIX
{
	float mat[4][4];
};

struct MY_VECTOR3
{
	float x, y, z, w;
};

MY_VECTOR3 g_vector1;
MY_VECTOR3 g_vector2;

MY_MATRIX g_matrix1;
MY_MATRIX g_matrix2;
MY_MATRIX g_matrix3;
MY_MATRIX g_matrix_test;
MY_MATRIX g_matrix_result;

MY_MATRIX multifly(MY_MATRIX& _mat1, MY_MATRIX& _mat2);
MY_MATRIX multifly(MY_MATRIX& _mat1, MY_VECTOR3& _vec);

MY_MATRIX moveMatirx(MY_MATRIX& _mat1, MY_VECTOR3& _vec);
MY_MATRIX rotationX(MY_MATRIX& _mat1, float seta);

int Seta;



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int tempInt[4][4]=
	{
		{30,30,0,0},{60,60,0,0},{0,60,0,0},{0,0,0,0}
	};
	
	

	switch (iMessage)
	{
	case WM_CREATE:
		Seta = 0;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				g_matrix1.mat[i][j] = j;
				g_matrix2.mat[i][j] = i+2;
			}
		}
		g_matrix3 = multifly(g_matrix1, g_matrix2);

		g_vector1.x = 7;
		g_vector1.y = 3;
		g_vector1.z = 2;
		g_vector1.w = 5;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				g_matrix_test.mat[i][j] = tempInt[i][j];
			}
		}
		
		g_matrix_result =rotationX(g_matrix_test, 30.0f);


		g_matrix3 = multifly(g_matrix1, g_vector1);

		moveMatirx(g_matrix1, g_vector1);

		rotationX(g_matrix1, 90.0f);
		return 0;
	case WM_LBUTTONDOWN:

		//x = LOWORD(lParam);
		//y = HIWORD(lParam);
		//bNowDraw = TRUE;
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONUP:
		Seta++;
		g_matrix_result = rotationX(g_matrix_test, Seta);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MoveToEx(hdc, g_matrix_result.mat[0][0] +300, g_matrix_result.mat[0][1] + 300,NULL);
		LineTo(hdc, g_matrix_result.mat[1][0] + 300, g_matrix_result.mat[1][1] + 300);
		LineTo(hdc, g_matrix_result.mat[2][0] + 300, g_matrix_result.mat[2][1] + 300);
		LineTo(hdc, g_matrix_result.mat[0][0] + 300, g_matrix_result.mat[0][1] + 300);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}


MY_MATRIX multifly(MY_MATRIX& _mat1, MY_MATRIX& _mat2)
{
	MY_MATRIX result;
	float temp;
	float mi_x =0;
	float mi_y =0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.mat[i][j] =
				(_mat1.mat[i][0] * _mat2.mat[0][j])
				+ (_mat1.mat[i][1] * _mat2.mat[1][j])
				+ (_mat1.mat[i][2] * _mat2.mat[2][j])
				+ (_mat1.mat[i][3] * _mat2.mat[3][j]);
		}
	}

	return result;
}

MY_MATRIX multifly(MY_MATRIX& _mat1, MY_VECTOR3& _vec)
{
	MY_MATRIX result;
	float temp;
	float mi_x = 0;
	float mi_y = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.mat[i][j] =
				(_mat1.mat[i][0] * _vec.x)
				+ (_mat1.mat[i][1] * _vec.y)
				+ (_mat1.mat[i][2] * _vec.z)
				+ (_mat1.mat[i][3] * _vec.w);
		}
	}

	return result;
}

MY_MATRIX moveMatirx(MY_MATRIX& _mat1, MY_VECTOR3& _vec)
{
	MY_MATRIX result;
	MY_MATRIX tempMat;
	float temp[4][4] = 
	{
		{ 1,0,0,0 }, { 0,1,0,0 }, {0,0,1,0}, {_vec.x, _vec.y, _vec.z, 1}
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tempMat.mat[i][j] = temp[i][j];
		}
	}

	result = multifly(_mat1, tempMat);

	return result;
}

MY_MATRIX rotationX(MY_MATRIX& _mat1, float seta)
{
	MY_MATRIX result;
	MY_MATRIX tempMat;
	float temp[4][4] =
	{
		{ 1,0,0,0 }, { 0,cos(seta),sin(seta),0 }, {0,-sin(seta),cos(seta),0}, {0,0,0,1}
	};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tempMat.mat[i][j] = temp[i][j];
		}
	}

	result = multifly(_mat1, tempMat);

	return result;
}