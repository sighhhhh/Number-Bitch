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
    // �Ƿ񿪻�����
    if (IsFirstRun())
    {
        int result = MessageBox(NULL, _T("�Ƿ񽫳�����ӵ�������������"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION);
        if (result == IDYES)
        {
            AddToStartup();
        }
        SetFirstRunComplete();
    }

    // ����������
    HANDLE hMutex = CreateMutex(NULL, TRUE, _T("UniqueMutex"));

    // ����Ƿ�������ʵ��
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, _T("Ӧ�ó����Ѿ��������У�"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
        return 0;
    }

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

    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    return 0;
}

