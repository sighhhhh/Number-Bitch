#include "number_bitch.h"
#include "tray.h"

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
{
    // ��ʼ��ӳ���ϵ
    initKeyMapping();

    // ͼ������
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

    // ��ʼ��ȫ�ִ��ھ��
    g_hwnd = hwnd;

    ShowWindow(hwnd, SW_HIDE);

    // ��������ͼ��
    CreateTrayIcon(hwnd);

    // ����ȫ�ֹ���
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (!hHook)
    {
        std::cerr << "�޷���װ����" << std::endl;
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

    return 0;
}
