#ifndef RESIZEDIALOG_HPP
#define RESIZEDIALOG_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <vector>
#include <exception>

class ResizeDialog
{
public:
    static void registerClass(const CHAR* className);
    explicit ResizeDialog(HWND parent, const std::string& className, const std::string& lable);

    HWND addButton(const std::string& lable, const RECT& posSize, HMENU id);
    HWND addEdit(const std::string& lable, const RECT& posSize, HMENU id);
    HWND addLable(const std::string& lable, const RECT& posSize, HMENU id);

    int resRow{0}, resCol{0};
    int run();

    HWND getHandle() { return wndHandle; };
private:
    static LRESULT CALLBACK appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


    enum messages
    {
        m_Close, m_Ok, m_EditRow, m_EditCol
    };


    HWND wndHandle{};
    HWND parent{};
    HWND close, ok, editRow, editCol;
    std::string className;
    std::string lable;
    long Widht, Height;
};

#endif