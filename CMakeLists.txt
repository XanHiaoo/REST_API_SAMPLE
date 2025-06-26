# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.10)

project ("REST_API_SAMPLE")


set(CMAKE_GENERATOR_PLATFORM x64)

# 包含子项目
add_subdirectory ("REST_API_SAMPLE")
