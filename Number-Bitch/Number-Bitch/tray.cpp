#include "tray.h"
#include "number_bitch.h"

NOTIFYICONDATA IconData;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// 初始化托盘图标
		CreateTrayIcon(hwnd);
		// 不显示窗口
		ShowWindow(hwnd, SW_HIDE);
		break;
	case WM_DESTROY:
		// 移除托盘图标
		RemoveTrayIcon(hwnd);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		// 处理菜单指令
		if (LOWORD(wParam) == ID_EXIT)
		{
			DestroyWindow(hwnd);
		}
		break;
	case ID_SWITCH_ICON_TRUE:
		UpdateTrayIcon(hwnd,LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_ACTIVE)));
		break;
	case ID_SWITCH_ICON_FALSE:
		UpdateTrayIcon(hwnd,LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_NORMAL)));
		break;
	default:
		if (message == WM_TRAYICON)
		{
			if (lParam == WM_RBUTTONDOWN)
			{
				// 显示上下文菜单
				ShowMenu();
			}
		}
		// 处理托盘图标的特定消息
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// 创建托盘图标
void CreateTrayIcon(HWND hwnd)
{
	//初始化托盘图标数据
	memset(&IconData, 0, sizeof(IconData));
	IconData.cbSize = sizeof(IconData);
	IconData.hWnd = hwnd;
	IconData.uID = 1;
	IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	IconData.uCallbackMessage = WM_TRAYICON;
	lstrcpy(IconData.szTip, TEXT("Tray Icon"));
	IconData.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_NORMAL));

	// 添加托盘图标
	Shell_NotifyIcon(NIM_ADD, &IconData);
}

void UpdateTrayIcon(HWND hWnd, HICON hIcon)
{
	// 更新托盘图标
	IconData.hIcon = hIcon;
	Shell_NotifyIcon(NIM_MODIFY, &IconData);
}

void RemoveTrayIcon(HWND hWnd)
{
	// 移除托盘
	IconData.hWnd = hWnd;
	Shell_NotifyIcon(NIM_DELETE, &IconData);
}

void ShowMenu()
{
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("Bye!~"));

	POINT pt;
	GetCursorPos(&pt);

	//显示上下文菜单
	SetForegroundWindow(IconData.hWnd);
	UINT clicked = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, IconData.hWnd, NULL);
	SendMessage(IconData.hWnd, WM_COMMAND, clicked, 0);
	/*TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, IconData.hWnd, NULL);*/

	// 销毁菜单
	DestroyMenu(hMenu);
}