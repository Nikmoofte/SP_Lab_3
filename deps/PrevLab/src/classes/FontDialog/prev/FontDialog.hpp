#ifndef FONTDIALOG_HPP
#define FONTDIALOG_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

class FontDialog : public Dialog
{
public:
    static void registerClass(const CHAR* className);

    HWND addButton(const std::string& lable, const RECT& posSize, HMENU id);
    HWND addEdit(const std::string &lable, const UINT type, const RECT &posSize, HMENU id);
    HWND addLable(const std::string& lable, const RECT& posSize, HMENU id);

    std::string resFont;
    int resSize;
    int run();

    HWND getHandle() { return wndHandle; };
private:
    static LRESULT CALLBACK appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


    enum messages
    {
        m_Close, m_Ok, m_EditFont, m_EditSize
    };



    HWND wndHandle{};
    HWND parent{};
    HWND close, ok, editFont, editSize;
    std::string className;
    std::string lable;
    long Widht, Height;
};

#endif