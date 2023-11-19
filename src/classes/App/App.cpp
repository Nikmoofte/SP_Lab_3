#include "App.hpp"

#include <stdexcept>
#include <memory>
#include <chrono>
#include <fstream>
#include <iterator>
#include <Windowsx.h>
#include <CommCtrl.h>
#include <mshtmcid.h>
#include <Windef.h>
#include <ResizeDialog/ResizeDialog.hpp>
#include <FontDialog/FontDialog.hpp>

App::App() : className{ "Lab_2_WNDCLASS" }, lable{ "Lab 2" }
{
    try
    {
        registerClass(className.c_str());
        wndHandle = CreateWindowEx(
        0,                              // Optional window styles.
        className.c_str(),              // Window class
        lable.c_str(),                  // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,                        // Parent window    
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
        ExitProcess(EXIT_FAILURE);
    }
    
}

App::App(std::string filePath) : App()
{
    table.resize(5, 4);
    std::ifstream file(filePath);
    std::istream_iterator<char> beg(file);
    table.setText(std::string(beg, {}));
    RegisterHotKey(wndHandle, 1, MOD_CONTROL, VK_RETURN);
}

int App::run()
{
    MSG msg{};
    ShowWindow(wndHandle, SW_SHOW);
    UpdateWindow(wndHandle);

    while(msg.message != WM_QUIT)
    {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)(msg.wParam);
}

LRESULT App::appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    App* pApp;
    if(uMsg == WM_NCCREATE)
    {
        pApp = (App*)(((CREATESTRUCTW*)lParam)->lpCreateParams);
        SetLastError(0);
        if(!SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pApp))
        {
            if(GetLastError() != 0)
                return false;
        }
    }
    else
        pApp = (App*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    if(pApp)
    {
        pApp->wndHandle = hwnd;
        return pApp->WindowProc(hwnd, uMsg, wParam, lParam);

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {  
        case WM_MOUSEWHEEL:
        {
            int prevOffset = offset;
            offset += GET_WHEEL_DELTA_WPARAM(wParam);
            offset = min(offset, 0);
            if(offset < -table.getRect().bottom + 100)
                offset = -table.getRect().bottom + 100;
            if(offset != prevOffset)
            {
                invalidateRect();
            }

            break;
        }
        case WM_CREATE:
        {
            auto hMenu = CreateMenu();
            AppendMenu(hMenu, MF_ENABLED | MF_STRING, m_Resize, "Resize");
            AppendMenu(hMenu, MF_ENABLED | MF_STRING, m_ChangeFont, "Change font");
            SetMenu(hwnd, hMenu);


            break;
        }
        case WM_RBUTTONDOWN:
        {
            auto tableRect = table.getRect();
            tableRect.bottom += offset;
            tableRect.top += offset;
            std::pair<int, int> mousePos{ LOWORD(lParam), HIWORD(lParam)};

            if(mousePos.first > tableRect.left && mousePos.first < tableRect.right &&
                mousePos.second > tableRect.top && mousePos.second < tableRect.bottom && !edit)
            {
                mousePos = table.getCellIndies(mousePos.first, mousePos.second - offset);
                auto cellRect = table.getCellRect(mousePos.first, mousePos.second);
                cellRect.bottom += offset;
                cellRect.top += offset;

                cellRect.right -= cellRect.left;
                cellRect.bottom -= cellRect.top;

                cellTextPtr = &table.getCellText(mousePos.first, mousePos.second);
                editHandle = addEdit(cellTextPtr->c_str(), ES_MULTILINE, cellRect, (HMENU)m_ChangeText);

                edit = true;
            }

            break;
        }
        case WM_HOTKEY:
        {
            if(wParam == 1 && edit)
            {
                char text[256];
                GetWindowText(editHandle, text, 256);
                *cellTextPtr = text; 

                DestroyWindow(editHandle);
                invalidateRect();
                edit = false;
            }

            break;
        }
        case WM_COMMAND:
        {
            switch (wParam)
            {
            case m_Resize:
            {
                if(edit)
                {
                    DestroyWindow(editHandle);
                    edit = false;
                }
                ResizeDialog resizeDialog(hwnd, "ResizeDLG", "Resize"); 
                resizeDialog.run();

                if(resizeDialog.resCol && resizeDialog.resRow)
                {
                    table.resize(resizeDialog.resRow, resizeDialog.resCol);
                    invalidateRect();
                }
                break;
            }
            case m_ChangeFont:
            {
                FontDialog fontDialog(hwnd, "FontDLG", "Resize", fontName, fontSize); 
                fontDialog.run();

                fontName = fontDialog.resFont;
                fontSize = fontDialog.resSize;

                invalidateRect();
                break;
            }
            }
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            auto secHDC = CreateCompatibleDC(hdc);
            auto secBM = CreateCompatibleBitmap(hdc, appWidht, appHeight);
            
            auto origBM = (HBITMAP)SelectBitmap(secHDC, secBM);


            auto hFont = CreateFont(-MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72), 
                                    0, 0, 0, 
                                    FW_NORMAL, 
                                    false, false, false,
                                    ANSI_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, fontName.c_str());
            
            SelectObject(secHDC, hFont);
            table.calcCellsSize(secHDC);
            FillRect(secHDC, &ps.rcPaint, (HBRUSH) (WHITE_BRUSH));
            table.draw(secHDC, offset);

            BitBlt(hdc, 0, 0, appWidht, appHeight, secHDC, 0, 0, SRCCOPY);

            SelectBitmap(secHDC, origBM);
            DeleteObject(secBM);
            DeleteDC(secHDC);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = 300;
            lpMMI->ptMinTrackSize.y = 300;
            return 0;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
            return 0;
        }
        case WM_SIZE:
        {
            appWidht = LOWORD(lParam);
            appHeight = HIWORD(lParam);
            table.resize({0, 0, appWidht, appHeight});
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void App::invalidateRect()
{
    RECT rect{0, 0, appWidht, appHeight};
    InvalidateRect(wndHandle, &rect, false);
}

inline void App::registerClass(const CHAR* className)
{
     WNDCLASSEX wc = { sizeof(WNDCLASSEX) };

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.lpfnWndProc   = App::appProg;
        wc.hInstance     = GetModuleHandleW(nullptr);
        wc.lpszClassName = className;
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.lpszMenuName = nullptr;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        
        if(!RegisterClassEx(&wc))
            throw std::runtime_error{"Failed to create window class!"};
}

