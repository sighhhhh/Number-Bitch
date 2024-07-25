#include "number_bitch.h"
#include "tray.h"

std::set<int> pressed_keys;
std::unordered_map<int, int> keyMapping;

extern bool g_capsLockWasOn = false;
extern bool g_capsLockIsOn = false;
extern bool g_spaceIsOn = false;
extern bool g_bitch = false;
extern HWND g_hwnd = NULL;

int pressed_pre_len = 0;

void initKeyMapping()
{
    keyMapping['u'] = '7';
    keyMapping['i'] = '8';
    keyMapping['o'] = '9';
    keyMapping['j'] = '4';
    keyMapping['k'] = '5';
    keyMapping['l'] = '6';
    keyMapping['m'] = '1';
    keyMapping[VK_OEM_COMMA] = '2';
    keyMapping[VK_OEM_PERIOD] = '3';
    keyMapping['h'] = '0';
    keyMapping[VK_SPACE] = VK_OEM_PERIOD; // �ո��ӳ��Ϊ '.'
    keyMapping['U'] = '7';
    keyMapping['I'] = '8';
    keyMapping['O'] = '9';
    keyMapping['J'] = '4';
    keyMapping['K'] = '5';
    keyMapping['L'] = '6';
    keyMapping['M'] = '1';
    keyMapping['H'] = '0';
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)
        return CallNextHookEx(NULL, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

    switch (wParam)
    {
    case WM_KEYDOWN:
    {
        // ������ CapsLock �� ESC ��ʱ�˳�����
        if (g_capsLockIsOn && p->vkCode == VK_ESCAPE)
        {
            PostThreadMessage(GetCurrentThreadId(), WM_QUIT, 0, 0);
            return 1; // ��������¼�
        }
        if (p->vkCode == VK_CAPITAL)
        {
            g_capsLockIsOn = true;
            g_capsLockWasOn = (GetKeyState(VK_CAPITAL) & 0x0001) ? true : false;
        }
        // CapsLock �Ѵ���ͬʱ��������ƥ���ӳ���ϵ
        if (g_capsLockIsOn && keyMapping.find(p->vkCode) != keyMapping.end())
        {
            g_bitch = true;
            PostMessage(g_hwnd, ID_SWITCH_ICON_TRUE, g_bitch, 0);
            /*std::cout << "g_spaceIsOn : " << g_spaceIsOn << std::endl;*/
            // ������ǿո���ҿո��û�м���
            if (!g_spaceIsOn) {
                /*std::cout << p->vkCode << " key pressed : " << keyMapping[p->vkCode] << std::endl;*/
                //����ǿո��
                if (p->vkCode == VK_SPACE)
                {
                    g_spaceIsOn = true;
                }
                INPUT input;
                input.type = INPUT_KEYBOARD;
                input.ki.wScan = MapVirtualKey(keyMapping[p->vkCode], MAPVK_VK_TO_VSC);
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                input.ki.wVk = keyMapping[p->vkCode];
                SendInput(1, &input, sizeof(INPUT));
                return 1; // ���ش��¼�
            }
        }
        break;
    }
    case WM_KEYUP:
    {
        if (p->vkCode == VK_SPACE)
        {
            g_spaceIsOn = false;
            /*std::cout << p->vkCode << " key released : " << keyMapping[p->vkCode] << std::endl;*/
        }
        if (p->vkCode == VK_CAPITAL)
        {
            g_capsLockIsOn = false;
            /*std::cout << p->vkCode << " key released : " << keyMapping[p->vkCode] << " : " << g_bitch << std::endl;*/
            // ����Ƿ���Ҫ�ָ�CapsLock״̬
            std::cout << " Bitch : " << g_bitch << std::endl;
            if (g_bitch)
            {
                keybd_event(VK_CAPITAL, MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC), 0, 0);
                Sleep(5);
                keybd_event(VK_CAPITAL, MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
                g_bitch = false; // ������
                PostMessage(g_hwnd, ID_SWITCH_ICON_FALSE, g_bitch, 0);
            }
        }
        break;
    }
    default:
        break;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

