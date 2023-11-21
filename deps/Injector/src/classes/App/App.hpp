#ifndef APP_HPP
#define APP_HPP

#include <Windows.h>

#include <cstdint>
#include <string>
#include <exception>

#include "Dialog/Dialog.hpp"


class App : public Dialog
{
public:
    explicit App();

private:
    void Inject(DWORD pid, const std::string& origin, const std::string& replacement);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    const char* c_dllName = "ValChanger.dll";  

    enum menus
    {
        m_LBL, m_PIDEdit, m_OriginEdit, m_ReplacementEdit, m_Close, m_Inject 
    };
    HWND close, inject, editPID, editOrigin, editReplacement, errLbl;
};

#endif