#ifndef DIALOG_HPP
#define DIALOG_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

class Dialog
{
public:
    explicit Dialog(HWND parent, const std::string& className, const std::string& lable);
    int run();

    HWND getHandle() { return wndHandle; };

protected:
    HWND addButton(const std::string& lable, const RECT& posSize, HMENU id);
    HWND addEdit(const std::string &lable, const UINT type, const RECT &posSize, HMENU id);
    HWND addLable(const std::string& lable, const RECT& posSize, HMENU id);

    HWND wndHandle{};
    HWND parent{};
    std::string className;
    std::string lable;
    long Widht, Height;

    virtual LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    void registerClass(const char* className);
    void createWindow();
    static LRESULT CALLBACK appProg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

#endif