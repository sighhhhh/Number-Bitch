#include "tray.h"
#include "number_bitch.h"

NOTIFYICONDATA IconData;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// ��ʼ������ͼ��
		CreateTrayIcon(hwnd);
		// ����ʾ����
		ShowWindow(hwnd, SW_HIDE);
		break;
	case WM_DESTROY:
		// �Ƴ�����ͼ��
		RemoveTrayIcon(hwnd);
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		// ����˵�ָ��
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
				// ��ʾ�����Ĳ˵�
				ShowMenu();
			}
		}
		// ��������ͼ����ض���Ϣ
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// ��������ͼ��
void CreateTrayIcon(HWND hwnd)
{
	//��ʼ������ͼ������
	memset(&IconData, 0, sizeof(IconData));
	IconData.cbSize = sizeof(IconData);
	IconData.hWnd = hwnd;
	IconData.uID = 1;
	IconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	IconData.uCallbackMessage = WM_TRAYICON;
	lstrcpy(IconData.szTip, TEXT("Tray Icon"));
	IconData.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_NORMAL));

	// �������ͼ��
	Shell_NotifyIcon(NIM_ADD, &IconData);
}

void UpdateTrayIcon(HWND hWnd, HICON hIcon)
{
	// ��������ͼ��
	IconData.hIcon = hIcon;
	Shell_NotifyIcon(NIM_MODIFY, &IconData);
}

void RemoveTrayIcon(HWND hWnd)
{
	// �Ƴ�����
	IconData.hWnd = hWnd;
	Shell_NotifyIcon(NIM_DELETE, &IconData);
}

void ShowMenu()
{
	HMENU hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, ID_EXIT, TEXT("Bye!~"));

	POINT pt;
	GetCursorPos(&pt);

	//��ʾ�����Ĳ˵�
	SetForegroundWindow(IconData.hWnd);
	UINT clicked = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, IconData.hWnd, NULL);
	SendMessage(IconData.hWnd, WM_COMMAND, clicked, 0);
	/*TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, IconData.hWnd, NULL);*/

	// ���ٲ˵�
	DestroyMenu(hMenu);
}