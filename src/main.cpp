#include <windows.h>
#include <string>
#include <iostream>

#define DllExport   __declspec( dllexport )


char origin[] = "Lorem";
char dest[] = "MMMMM";
extern "C" DllExport void changeTextInProcces(const char* origin, const std::string& replasement);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
   
     // Perform actions based on the reason for calling.
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
        changeTextInProcces(origin, dest);
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

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