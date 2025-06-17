cmake_minimum_required(VERSION 3.10)
project(Simulation)

# Configuration de base
find_package(OpenGL REQUIRED)
add_executable(${PROJECT_NAME} WIN32 main.cpp)

# Configuration FreeGLUT
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/freeglut/include")
target_link_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/freeglut/lib/x64")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL freeglut)

# Copier la DLL
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/freeglut/bin/x64/freeglut.dll"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>) 