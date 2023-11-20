#include <windows.h>
#include <string>
#include <iostream>

#define DllExport   __declspec( dllexport )

extern "C" DllExport void changeTextInProcces(const char* origin, const std::string& replasement) 
{
    static constexpr size_t max_read = 128;
    HANDLE hProc = GetCurrentProcess();
    MEMORY_BASIC_INFORMATION mbi;
    SYSTEM_INFO msi;
    ZeroMemory(&mbi, sizeof(mbi));
    GetSystemInfo(&msi);

    SIZE_T dwRead = 0;

    std::string str;
    str.resize(max_read);
    str.shrink_to_fit();

    for (auto lpAddress = (LPBYTE)msi.lpMinimumApplicationAddress; 
    lpAddress <= (LPBYTE)msi.lpMaximumApplicationAddress; 
    lpAddress += mbi.RegionSize) 
    {
        if (VirtualQuery(lpAddress, &mbi, sizeof(mbi))) 
        {
            if ((mbi.Protect & PAGE_READWRITE) || (mbi.Protect & PAGE_WRITECOPY)) 
            {
                for (LPBYTE tempLpAddress = lpAddress; 
                tempLpAddress < (lpAddress + mbi.RegionSize); 
                tempLpAddress += max_read) 
                {
                    if (ReadProcessMemory(hProc, tempLpAddress, &str.front(), max_read, &dwRead)) 
                    {
                        decltype(str.find(origin)) pos; 
                        if ((pos = str.find(origin)) != std::string::npos) 
                        {
                            LPBYTE replaceAddr = tempLpAddress + pos;
                            if((void*)replaceAddr != origin)
                                WriteProcessMemory(
                                hProc,
                                replaceAddr,
                                replasement.c_str(),
                                replasement.size(), 
                                &dwRead
                                );
                        }
                        
                    }
                }
            }
        }
    }
}