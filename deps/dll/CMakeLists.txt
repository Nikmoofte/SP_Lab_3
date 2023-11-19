cmake_minimum_required(VERSION 3.5)
project(dllWinProj)

add_library(Injector SHARED src/main.cpp)
cmake_policy(SET CMP0079 NEW)

target_link_libraries(main PUBLIC windowscodecs)
target_link_libraries(main PUBLIC Msimg32)
target_link_libraries(main PUBLIC user32)
target_link_libraries(main PUBLIC kernel32)
target_link_libraries(main PUBLIC gdi32)