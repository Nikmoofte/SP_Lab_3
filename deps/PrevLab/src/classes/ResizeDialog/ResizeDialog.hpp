#ifndef RESIZEDIALOG_HPP
#define RESIZEDIALOG_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

#include "Dialog/Dialog.hpp"

class ResizeDialog : public Dialog
{
public:
    explicit ResizeDialog(HWND parent, const std::string& className, const std::string& lable);

    int resRow{0}, resCol{0};
private:
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    enum messages
    {
        m_Close, m_Ok, m_EditRow, m_EditCol
    };

    HWND close, ok, editRow, editCol;
};

#endif