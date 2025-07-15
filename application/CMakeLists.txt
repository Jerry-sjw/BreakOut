#递归遍历文件夹下所有的cpp文件
file(GLOB_RECURSE APP ./ *.cpp)

#将所有cpp编译为app这个lib库
add_library(app ${APP})

#把顶层 CMakeLists.txt 所在目录位置 添加为头文件搜索路径。
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR})