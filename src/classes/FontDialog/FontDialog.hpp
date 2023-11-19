#ifndef FONTDIALOG_HPP
#define FONTDIALOG_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

#include "Dialog/Dialog.hpp"

class FontDialog : public Dialog
{
public:
    explicit FontDialog(HWND parent, const std::string& className, const std::string& lable, const std::string& FontName, const int FontSize);
    std::string resFont;
    int resSize;
private:
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    enum messages
    {
        m_Close, m_Ok, m_EditFont, m_EditSize
    };

    HWND close, ok, editFont, editSize;
};

#endif