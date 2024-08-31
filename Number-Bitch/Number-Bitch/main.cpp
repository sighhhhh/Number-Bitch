#include <tchar.h>
#include "number_bitch.h"
#include "tray.h"
#include "Startup.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    // 是否开机自启
    if (IsFirstRun())
    {
        int result = MessageBox(NULL, _T("是否将程序添加到开机自启动？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);
        if (result == IDYES)
        {
            AddToStartup();
        }
        SetFirstRunComplete();
    }

    // 创建互斥体
    HANDLE hMutex = CreateMutex(NULL, TRUE, _T("UniqueMutex"));

    // 检查是否已运行实例
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, _T("应用程序已经在运行中！"), _T("错误"), MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

    // 初始化映射关系
    initKeyMapping();

    // 图标托盘
    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("TrayIconClass");

    if (!RegisterClass(&wc))
    {
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        TEXT("Tray Icon Demo"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd)
    {
        return -1;
    }

    // 初始化全局窗口句柄
    g_hwnd = hwnd;

    ShowWindow(hwnd, SW_HIDE);

    // 创建托盘图标
    CreateTrayIcon(hwnd);

    // 创建全局勾子
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (!hHook)
    {
        std::cerr << "无法安装钩子" << std::endl;
        return -1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    RemoveTrayIcon(hwnd);

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    return 0;
}

