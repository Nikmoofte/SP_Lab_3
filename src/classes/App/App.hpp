#ifndef APP_HPP
#define APP_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

#include "Table/Table.hpp"


class App
{
public:
    explicit App(std::string filePath);

    int run();
private:
    explicit App();
    static LRESULT CALLBACK appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void invalidateRect();
    HWND addEdit(const std::string &lable, const UINT type, const RECT &posSize, HMENU id)
    {
        return CreateWindow("Edit", lable.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | type, posSize.left, posSize.top, posSize.right, posSize.bottom, wndHandle, id, NULL, NULL);
    }

    inline void registerClass(const CHAR* className);
    Table table;

    enum Menu
    {
        m_Resize, m_ChangeFont, m_ChangeText
    };

    std::string fontName{"Times new roman"};
    int fontSize{12};
    int offset{0};

    bool edit = false;
    HWND editHandle = 0;
    std::string* cellTextPtr;

    HWND wndHandle{};
    std::string className;
    std::string lable;
    long appWidht, appHeight;
};

#endif