#pragma once

#include <windows.h>
#include <shellapi.h>

#define IDI_ICON_NORMAL 1001
#define IDI_ICON_ACTIVE 1002

#define ID_STARTUP_OFF	1003
#define ID_STARTUP_ON	1004
#define ID_STARTUP_TOGGLE	1005

#define ID_SWITCH_ICON_FALSE 1011
#define ID_SWITCH_ICON_TRUE  1012

// 菜单项ID
#define ID_EXIT 1000

// 通知消息
const UINT WM_TRAYICON = RegisterWindowMessage(TEXT("TaskbarCreated"));

// 托盘图标信息结构体
extern NOTIFYICONDATA IconData;

void CreateTrayIcon(HWND hwnd);

void UpdateTrayIcon(HWND hWnd, HICON hIcon);

void RemoveTrayIcon(HWND hWnd);

void ShowMenu();

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

