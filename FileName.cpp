#include "Windows.h"
#include "windowsx.h"
#include "tchar.h"
#include <vector>

HINSTANCE hInstance;
HWND hStatic;
std::vector<HWND> staticControls;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	WCHAR className[] = L"FirstWindow";

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm = NULL;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;

	wndClass.lpfnWndProc = WindowProc;

	if (!RegisterClassEx(&wndClass)) {
		return 0;
	}

	HWND hWnd = CreateWindowEx(
		0,
		className,
		L"My first window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, //x,y
		CW_USEDEFAULT, CW_USEDEFAULT, //width, height
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT cursorPos1;
	static POINT cursorPos2;
	static POINT cursorPos3;
	_TCHAR str[50];
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN: {
		
		GetCursorPos(&cursorPos1);
		ScreenToClient(hwnd, &cursorPos1);

	}

	case WM_LBUTTONUP: {


		GetCursorPos(&cursorPos2);
		ScreenToClient(hwnd, &cursorPos2);

		int x, y;
		if (cursorPos1.x > cursorPos2.x)
		{
			x = cursorPos1.x - cursorPos2.x;
			y = cursorPos1.y - cursorPos2.y;
			if (x < 10 || y < 10)
			{
				x = 10;
				y = 10;
				MessageBox(hwnd, L"Cant create statics smaller then 10x10", L"Warning", MB_OK);
			}
			
			hStatic = CreateWindow(L"static", L"static", WS_VISIBLE | WS_CHILD, cursorPos2.x, cursorPos2.y, x, y, hwnd, nullptr, hInstance, nullptr);
			staticControls.push_back(hStatic);
		}
		else if(cursorPos2.x > cursorPos1.x)
		{
			x = cursorPos2.x - cursorPos1.x;
			y = cursorPos2.y - cursorPos1.y;
			if (x < 10 || y < 10)
			{
				x = 10;
				y = 10;
				MessageBox(hwnd, L"Cant create statics smaller then 10x10", L"Warning", MB_OK);
			}
			hStatic = CreateWindow(L"static", L"static", WS_VISIBLE | WS_CHILD, cursorPos1.x, cursorPos1.y, x, y, hwnd, nullptr, hInstance, nullptr);
			staticControls.push_back(hStatic);
		}


		break;
	}

	case WM_LBUTTONDBLCLK:
	{
		GetCursorPos(&cursorPos3);
		for (auto it = staticControls.begin(); it != staticControls.end(); ++it) {
			RECT staticRect;
			GetWindowRect(*it, &staticRect);

			if (PtInRect(&staticRect, cursorPos3)) {
				DestroyWindow(*it);
				staticControls.erase(it);
				break;
			}
		}
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	return 0;
}