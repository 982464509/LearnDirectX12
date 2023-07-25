//***************************************************************************************
// @author : zxz
// @file :
// @description :
//***************************************************************************************

#include "WindowsWapper.h"
#include <windowsx.h>

HWND WinApplication::m_hwnd = nullptr;

void WinApplication::InitMainWindow(HINSTANCE hInstance, BaseDX* pBaseDX)
{
    // Initialize the window class.
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"DXSampleClass";
    RegisterClassEx(&windowClass);

    RECT windowRect = { 0, 0, static_cast<LONG>(pBaseDX->GetWidth()), static_cast<LONG>(pBaseDX->GetHeight()) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    // Create the window and store a handle to it.
    m_hwnd = CreateWindow(
        windowClass.lpszClassName,
        pBaseDX->GetTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,
        nullptr,
        hInstance,
        pBaseDX);
}

int WinApplication::Run(BaseDX* pBaseDX, HINSTANCE hInstance, int nCmdShow)
{      
    WinApplication::InitMainWindow(hInstance, pBaseDX);

    pBaseDX->OnInit();

    ShowWindow(m_hwnd, nCmdShow);

    // Main loop.
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {       
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    pBaseDX->OnDestroy();

    return static_cast<char>(msg.wParam);
}

LRESULT CALLBACK WinApplication::WindowProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
    BaseDX* pBaseDX = reinterpret_cast<BaseDX*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_CREATE:
    {
        // Save the DXSample* passed in to CreateWindow.
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
    return 0;

    case WM_RBUTTONDOWN:
        pBaseDX->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_RBUTTONUP:
        pBaseDX->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_MOUSEMOVE:
        pBaseDX->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_PAINT:
        if (pBaseDX)
        {
            pBaseDX->OnUpdate();
            pBaseDX->OnRender();
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_KEYUP:
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
