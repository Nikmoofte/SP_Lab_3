cmake_minimum_required(VERSION 3.5)
project(WinProj)

set(ClassesDir src/classes/)
set(AssetsDir src/assets/)
set(Dependencies deps/)

cmake_policy(SET CMP0079 NEW)

set(Classes 
    ${ClassesDir}App/App.cpp
    ${ClassesDir}Dialog/Dialog.cpp
)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
add_executable(main WIN32 src/main.cpp ${Classes})

target_link_libraries(main PUBLIC windowscodecs)
target_link_libraries(main PUBLIC Msimg32)
target_link_libraries(main PUBLIC user32)
target_link_libraries(main PUBLIC kernel32)
target_link_libraries(main PUBLIC gdi32)

target_include_directories(main PUBLIC ${ClassesDir})
target_include_directories(main PUBLIC ${AssetsDir})
