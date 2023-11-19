#include "FontDialog.hpp"

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

FontDialog::FontDialog(HWND parent, const std::string& className, const std::string& lable, const std::string& FontName, const int FontSize) : 
    Dialog(parent, className, lable), 
    resFont{ FontName }, 
    resSize{ FontSize }
{
    
}

LRESULT FontDialog::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {  
        case WM_CREATE:
        {
            close = addButton("Close", {40, 200, 80, 30}, (HMENU)m_Close);
            ok = addButton("Ok", {40 + 160, 200, 80, 30}, (HMENU)m_Ok);

            editFont = addEdit(resFont.c_str(), 0,{30, 40, 200, 30}, (HMENU)m_EditFont);
            addLable("Font name: ", {30, 10, 100, 30}, 0);
            editSize = addEdit(std::to_string(resSize), ES_NUMBER, {30, 100, 100, 30}, (HMENU)m_EditSize);
            addLable("Font size: ", {30, 70, 100, 30}, 0);
            return true;
        }
        case WM_COMMAND:
        {
            switch (wParam)
            {
            case m_Ok:
            {
                char text[100];
                GetWindowText(editFont, text, 100);
                {
                    resFont = std::string(text);
                }
                GetWindowText(editSize, text, 100);
                {
                    std::istringstream in(text);
                    in >> resSize;
                }
            }
            case m_Close:
                DestroyWindow(hwnd);                
                break;
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
        case WM_SIZE:
        {
            Widht = LOWORD(lParam);
            Height = HIWORD(lParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
