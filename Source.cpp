#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "Shobjidl.h"  //←[2] 

#include "locale.h"    //←[3] 
TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	static HWND hButton;
	switch (msg)
	{
	case WM_CREATE:
		hEdit = CreateWindow(TEXT("EDIT"), TEXT("C:\\Users\\Public\\Pictures"), WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("取得"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)IDOK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hEdit, 10, 10, 512, 32, TRUE);
		MoveWindow(hButton, 10, 50, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR szText[MAX_PATH];
			GetWindowText(hEdit, szText, MAX_PATH);
			HRESULT hRes = CoInitialize(NULL);
			if (hRes == S_OK)
			{
				IShellItem *pShellItem;
				hRes = SHCreateItemFromParsingName(szText, NULL, IID_IShellItem, (void**)&pShellItem);
				if (hRes == S_OK)
				{
					LPWSTR pLocalizedName;
					hRes = pShellItem->GetDisplayName(SIGDN_NORMALDISPLAY, &pLocalizedName);  //←[10] 
					if (hRes == S_OK)
					{
						MessageBox(hWnd, pLocalizedName, 0, 0);
						CoTaskMemFree((LPVOID)pLocalizedName);
					}
					pShellItem->Release();
				}
				CoUninitialize();
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
