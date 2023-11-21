#include <Windows.h>
#include <iostream>
#include <string>

#include "App/App.hpp"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE,  LPSTR szCmdLine, int nCmdShow)
{
    App app;

    return app.run();
}