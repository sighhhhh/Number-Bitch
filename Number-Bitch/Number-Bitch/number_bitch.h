#pragma once

#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <unordered_map>
#include <set>

//const UINT WM_BITCH_FALSE = WM_USER + 101;
//const UINT WM_BITCH_TRUE = WM_USER + 102;

extern HWND g_hwnd;

void initKeyMapping();

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

