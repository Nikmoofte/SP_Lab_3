cmake_minimum_required(VERSION 3.5)
project(dllWinProj)

add_library(ValChanger SHARED src/main.cpp)
cmake_policy(SET CMP0079 NEW)

add_subdirectory(deps/staticImport)
add_subdirectory(deps/dynamicImport)
add_subdirectory(deps/Injector)

target_link_libraries(main PUBLIC windowscodecs)
target_link_libraries(main PUBLIC Msimg32)
target_link_libraries(main PUBLIC user32)
target_link_libraries(main PUBLIC kernel32)
target_link_libraries(main PUBLIC gdi32)