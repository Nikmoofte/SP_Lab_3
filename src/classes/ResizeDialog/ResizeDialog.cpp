#include "ResizeDialog.hpp"

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

ResizeDialog::ResizeDialog(HWND parent, const std::string& className, const std::string& lable) : className{className}, lable{ lable }
{
    try
    {
        RECT rect;
        GetWindowRect(parent, &rect);

        this->parent = parent;

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

HWND ResizeDialog::addButton(const std::string &lable, const RECT &posSize, HMENU id)
{
    return CreateWindow("Button", lable.c_str(), WS_VISIBLE | WS_CHILD, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}
HWND ResizeDialog::addEdit(const std::string &lable, const RECT &posSize, HMENU id)
{
    return CreateWindow("Edit", lable.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}
HWND ResizeDialog::addLable(const std::string &lable, const RECT &posSize, HMENU id)
{
    return CreateWindow("Static", lable.c_str(), WS_VISIBLE | WS_CHILD, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
}

int ResizeDialog::run()
{
    MSG msg{};
    ShowWindow(wndHandle, SW_SHOW);

    while (GetMessage(&msg, wndHandle, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)(msg.wParam);
}

LRESULT ResizeDialog::appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ResizeDialog* pWindow;
    if(uMsg == WM_NCCREATE)
    {
        pWindow = (ResizeDialog*)(((CREATESTRUCTW*)lParam)->lpCreateParams);
        SetLastError(0);
        if(!SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow))
        {
            if(GetLastError() != 0)
                return false;
        }
    }
    else
        pWindow = (ResizeDialog*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    if(pWindow)
    {
        pWindow->wndHandle = hwnd;
        return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT ResizeDialog::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {  
        case WM_CREATE:
        {
            close = addButton("Close", {40, 200, 80, 30}, (HMENU)m_Close);
            ok = addButton("Ok", {40 + 160, 200, 80, 30}, (HMENU)m_Ok);

            editRow = addEdit("", {30, 40 + 50, 100, 30}, (HMENU)m_EditRow);
            addLable("rows: ", {30, 10 + 50, 100, 30}, 0);
            editCol = addEdit("", {30 + 160, 40 + 50, 100, 30}, (HMENU)m_EditCol);
            addLable("cols: ", {30 + 160, 10 + 50, 100, 30}, 0);
            return true;
        }
        case WM_COMMAND:
        {
            switch (wParam)
            {
            case m_Ok:
            {
                char text[100];
                GetWindowText(editRow, text, 100);
                {
                    std::istringstream in(text);
                    in >> resRow;
                }
                GetWindowText(editCol, text, 100);
                {
                    std::istringstream in(text);
                    in >> resCol;
                }
            }
            case m_Close:
                EnableWindow(parent, true);
                DestroyWindow(hwnd);                
                break;
            case m_EditRow:
            {
                
                break;
            }
            case m_EditCol:
            {
                char text[100];
                GetWindowText(editRow, text, 100);
                //SetWindowText(editRow, text);
                break;
            }
            }
            break;
        }
        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = 300;
            lpMMI->ptMinTrackSize.y = 300;
            return 0;
        }
        case WM_DESTROY:
        {
            EnableWindow(parent, true);
            return 0;
        }
        case WM_SIZE:
        {
            Widht = LOWORD(lParam);
            Height = HIWORD(lParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void ResizeDialog::registerClass(const CHAR* className_)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc   = ResizeDialog::appProg;
    wc.hInstance     = GetModuleHandleW(nullptr);
    wc.lpszClassName = className_;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    
    if(!RegisterClassEx(&wc))
        throw std::runtime_error{"Failed to create window class!"};
}

