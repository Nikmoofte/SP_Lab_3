#include <iostream>

#define DllImport   __declspec( dllimport )

extern "C" DllImport void changeTextInProcces(const char* origin, const std::string& replasement);

int main()
{
    char arr[] = "Hello, world!";
    std::cout << arr << std::endl;
    changeTextInProcces("Hello, world!", "Good bye, world!");    

    for(int i = 0; i < 10; ++i)
        std::cout << i << ": " << arr << std::endl;

    return 0;
}