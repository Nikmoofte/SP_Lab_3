#include <iostream>
#include <Windows.h>

int main()
{
    char arr[] = "Hello, world!";
    std::cout << arr << std::endl;

    auto dllmoduleHangle = LoadLibrary("ValChanger");   
    auto procAddr = GetProcAddress(dllmoduleHangle, "changeTextInProcces");
    auto val = GetLastError();
    reinterpret_cast<void (*)(const char*, const std::string&)>(procAddr)(
        "Hello, world!", 
        "Good bye, world!"
        );
    FreeLibrary(dllmoduleHangle);

    for(int i = 0; i < 10; ++i)
        std::cout << i << ": " << arr << std::endl;

    return 0;
}