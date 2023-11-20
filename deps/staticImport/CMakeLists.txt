cmake_minimum_required(VERSION 3.5)
project(consoleApp)

add_library(changeLib SHARED IMPORTED)
set_property(TARGET changeLib PROPERTY
    IMPORTED_IMPLIB ${dllWinProj_BINARY_DIR}/ValChanger.lib)

add_executable(staticImport src/main.cpp)

add_custom_target(copy ALL COMMAND ${CMAKE_COMMAND} -E
                  copy ${dllWinProj_BINARY_DIR}/ValChanger.dll ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Copying dll into binary directory")


add_dependencies(staticImport copy)

target_link_libraries(staticImport PUBLIC changeLib)

