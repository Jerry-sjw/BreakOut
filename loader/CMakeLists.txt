#递归遍历文件夹下所有的cpp文件
file(GLOB_RECURSE LOADER ./ *.cpp)

#将所有cpp编译为app这个lib库
add_library(loader ${LOADER})