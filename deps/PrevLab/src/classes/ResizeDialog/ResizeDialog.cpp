#include "ResizeDialog.hpp"

#include <sstream>

ResizeDialog::ResizeDialog(HWND parent, const std::string& className, const std::string& lable) : 
    Dialog(parent, className, lable)
{

}

LRESULT ResizeDialog::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {  
        case WM_CREATE:
        {
            close = addButton("Close", {40, 200, 80, 30}, (HMENU)m_Close);
            ok = addButton("Ok", {40 + 160, 200, 80, 30}, (HMENU)m_Ok);

            editRow = addEdit("", ES_NUMBER, {30, 40 + 50, 100, 30}, (HMENU)m_EditRow);
            addLable("rows: ", {30, 10 + 50, 100, 30}, 0);
            editCol = addEdit("", ES_NUMBER, {30 + 160, 40 + 50, 100, 30}, (HMENU)m_EditCol);
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
        case WM_SIZE:
        {
            Widht = LOWORD(lParam);
            Height = HIWORD(lParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

