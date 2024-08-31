#include "Startup.h"

void AddToStartup()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS)
    {
        TCHAR path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        RegSetValueEx(hKey, TEXT("NB"), 0, REG_SZ, (BYTE*)path, (lstrlen(path) + 1) * sizeof(TCHAR));
        RegCloseKey(hKey);
    }
}

void RemoveFromStartup()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS)
    {
        RegDeleteValue(hKey, TEXT("NB"));
        RegCloseKey(hKey);
    }
}

bool IsFirstRun()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("Software\\YourCompany\\YourApp");
    DWORD dwType = REG_DWORD;
    DWORD dwValue = 0;
    DWORD dwSize = sizeof(DWORD);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hKey, TEXT("FirstRun"), NULL, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            return dwValue == 1;
        }
        RegCloseKey(hKey);
    }
    else
    {
        if (RegCreateKeyEx(HKEY_CURRENT_USER, sk, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS)
        {
            dwValue = 1;
            RegSetValueEx(hKey, TEXT("FirstRun"), 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(DWORD));
            RegCloseKey(hKey);
            return true;
        }
    }
    return false;
}

void SetFirstRunComplete()
{
    HKEY hKey;
    LPCTSTR sk = TEXT("Software\\YourCompany\\YourApp");
    DWORD dwValue = 0;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        RegSetValueEx(hKey, TEXT("FirstRun"), 0, REG_DWORD, (const BYTE*)&dwValue, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

