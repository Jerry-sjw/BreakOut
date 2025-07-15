# 指定最低的 CMake 版本
cmake_minimum_required (VERSION 3.8)

# 项目名称
project("test")

# C++17 标准
set(CMAKE_CXX_STANDARD 17)

# 资源文件拷贝
file(GLOB ASSETS "./assets")
file(GLOB DLL "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/dll/*.dll")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${DLL} DESTINATION ${CMAKE_BINARY_DIR})

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Debug、Release 等配置下的输出目录也一起设置（重要）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin)

# 包含目录和库目录
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include)
link_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib)

# 添加子目录
add_subdirectory(application)
add_subdirectory(loader)

# 添加可执行文件
add_executable(test "main.cpp" "glad.c")

# 链接库
target_link_libraries(test glfw3.lib irrKlang.lib freetype.lib app loader)

# 拷贝资源到 exe 所在的 bin 目录
file(GLOB DLL "${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/dll/*.dll")
add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DLL}
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")