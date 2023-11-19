#include "Dialog.hpp"

#include <stdexcept>
#include <memory>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iterator>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <mshtmcid.h>
#include <Windef.h>

Dialog::Dialog(HWND parent, const std::string& className, const std::string& lable) : 
className{className}, 
lable{ lable }, 
parent{ parent }
{
    
    
}

HWND Dialog::addButton(const std::string &lable, const RECT &posSize, HMENU id)
{
    return CreateWindow("Button", lable.c_str(), WS_VISIBLE | WS_CHILD, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}
HWND Dialog::addEdit(const std::string &lable, const UINT type, const RECT &posSize, HMENU id)
{
    return CreateWindow("Edit", lable.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | type, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}
HWND Dialog::addLable(const std::string &lable, const RECT &posSize, HMENU id)
{
    return CreateWindow("Static", lable.c_str(), WS_VISIBLE | WS_CHILD, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}

int Dialog::run()
{
    MSG msg{};
    createWindow();
    ShowWindow(wndHandle, SW_SHOW);

    while (GetMessage(&msg, wndHandle, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)(msg.wParam);
}

LRESULT Dialog::appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Dialog* pWindow;
    if(uMsg == WM_NCCREATE)
    {
        pWindow = (Dialog*)(((CREATESTRUCTW*)lParam)->lpCreateParams);
        SetLastError(0);
        if(!SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow))
        {
            if(GetLastError() != 0)
                return false;
        }
    }
    else
        pWindow = (Dialog*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    if(pWindow)
    {
        pWindow->wndHandle = hwnd;
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT Dialog::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_CLOSE)
    {
        DestroyWindow(hwnd);
        ShowWindow(parent, true);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Dialog::registerClass(const CHAR* className_)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc   = Dialog::appProg;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = className_;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    
    if(!RegisterClassEx(&wc))
        throw std::runtime_error{"Failed to create window class!"};
}
void Dialog::createWindow()
{
    try
    {
        RECT rect;
        GetWindowRect(parent, &rect);
        static WNDCLASS info;
        if(!GetClassInfo(GetModuleHandle(nullptr), className.c_str(), &info))
            registerClass(className.c_str());

        wndHandle = CreateWindowEx(
        0,                              // Optional window styles.
        className.c_str(),              // Window class
        lable.c_str(),                  // Window text
        WS_DLGFRAME,   // Window style
        // Size and position
        rect.right / 2 - 160, rect.bottom / 2 - 90, 320, 300,
        parent,                        // Parent window    
        nullptr,                        // Menu
        GetModuleHandle(nullptr),       // Instance handle
        this                            // Additional application data
        );

        if (!wndHandle)
            throw std::runtime_error{"Failed to create window!"};

    }
    catch(const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "ERROR", MB_OK | MB_ICONERROR);
        throw;
    }
}
