cmake_minimum_required(VERSION 3.5)
project(consoleApp)

add_executable(dynamicImport src/main.cpp)
# add_library(changeLib SHARED IMPORTED)
# set_property(TARGET changeLib PROPERTY
#     IMPORTED_IMPLIB ${dllWinProj_BINARY_DIR}/Injector.lib)

add_custom_target(copyD ALL COMMAND ${CMAKE_COMMAND} -E
                  copy ${dllWinProj_BINARY_DIR}/ValChanger.dll ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Copying dll into binary directory")

add_dependencies(dynamicImport copy)

# target_link_libraries(main PUBLIC changeLib)
