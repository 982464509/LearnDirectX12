#pragma once

#include "BaseDX.h"

class BaseDX;

class WinApplication
{
public:    
    static int Run(BaseDX* pBase, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hwnd; }

protected:
    static void InitMainWindow(HINSTANCE hInstance, BaseDX* pBaseDX);
    static LRESULT CALLBACK WindowProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam);

private:
    static HWND m_hwnd;
};
