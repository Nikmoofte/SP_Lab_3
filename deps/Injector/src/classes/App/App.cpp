#include "App.hpp"

#include <stdexcept>
#include <sstream>


App::App() : Dialog(0, "Injector_Class", "Injector")
{
}

void App::Inject(DWORD pid, const std::string &origin, const std::string &replacement)
{

    if (pid == NULL) 
        throw std::runtime_error("pid == NULL");

    HANDLE process;
    if (!(process = OpenProcess(PROCESS_ALL_ACCESS, false, pid)))
        throw std::runtime_error("Failed to open process!");

    struct threadParams
    {
        char dllName[128];
        char procName[128];
        char origin[128];
        char replacement[128];
    } params;
    strcpy(params.dllName, c_dllName);
    strcpy(params.origin, origin.c_str());
    strcpy(params.replacement, replacement.c_str());
    strcpy(params.procName, "changeTextInProcces");
    
    auto varSize = sizeof(threadParams);
    LPVOID alloc = (LPVOID)VirtualAllocEx(process, 0, varSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (alloc == NULL) 
        throw std::runtime_error("Failed to create variable!");

    if (!WriteProcessMemory(process, (LPVOID)alloc, &params, varSize, 0)) 
        throw std::runtime_error("Failed to change variable!");


    auto threadProg = [](void* _params) -> void
    {
        reinterpret_cast<void (*)(const char*, const std::string&)>(GetProcAddress
            (
                LoadLibrary(reinterpret_cast<threadParams*>(_params)->dllName), 
                reinterpret_cast<threadParams*>(_params)->procName)
            )
        (
            reinterpret_cast<threadParams*>(_params)->origin, 
            reinterpret_cast<threadParams*>(_params)->replacement
        );
    };
    HANDLE hThread = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)&LoadLibraryA, (LPVOID)alloc, 0, 0);
    if (hThread == NULL) 
        throw std::runtime_error("Failed to create thread!");

    CloseHandle(process);
}

LRESULT App::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
        {
            addLable("PID: ", {200, 100, 240, 30}, (HMENU)m_LBL);
            editPID = addEdit("", ES_NUMBER, {200, 130, 240, 30}, (HMENU)m_PIDEdit);

            addLable("Original string: ", {200, 160, 240, 30}, (HMENU)m_LBL);
            editOrigin = addEdit("Lorem", 0, {200, 190, 240, 30}, (HMENU)m_OriginEdit);

            addLable("Replacement: ", {200, 250, 240, 30}, (HMENU)m_LBL);
            editReplacement = addEdit("MMMMM", 0, {200, 280, 240, 30}, (HMENU)m_ReplacementEdit);

            close = addButton("Close", {200, 340, 60, 30}, (HMENU)m_Close);
            inject = addButton("Inject", {380, 340, 60, 30}, (HMENU)m_Inject);
            errLbl = addLable("", {200, 370, 240, 30}, (HMENU)m_LBL);
            return 1;
        }
        case WM_COMMAND:
        {
            switch (wParam)
            {
            case m_Inject:
            {
                char text[100];
                DWORD pid;
                std::string replacement;
                std::string origin;
                GetWindowText(editPID, text, 100);
                {
                    std::istringstream in(text);
                    in >> pid;
                }

                GetWindowText(editReplacement, text, 100);
                replacement = std::string(text);

                GetWindowText(editReplacement, text, 100);
                origin = std::string(text);

                try
                {
                    Inject(pid, origin, replacement);
                }
                catch(const std::exception& e)
                {
                    auto var = GetLastError();
                    SetWindowText(errLbl, e.what());
                    break;
                }
                SetWindowText(errLbl, "OK!");

                break;
            }
            case m_Close:
                DestroyWindow(hwnd);                
                break;
            }
            break;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            break;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
