# CMakeList.txt: REST_API_SAMPLE 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

project(RestAPISample)

set(CMAKE_CXX_STANDARD 17)

# 生成可执行文件
add_executable(rest_api src/main.cpp src/RESTfulServer.cpp src/DataBaseManager.cpp)

target_include_directories(rest_api PRIVATE 
    include 
    third_party/cpp-httplib-0.20.0
    third_party/nlohmannjson/single_include
	third_party/Crow-1.2.1.2/include
	third_party/asio-1.30.2/include
	third_party/ormpp-0.1.3/include
)

target_link_directories(rest_api PRIVATE 
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/third_party/ormpp-0.1.3/lib/win64d>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/third_party/ormpp-0.1.3/lib/win64>
)

target_link_libraries(rest_api PRIVATE 
    $<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/third_party/ormpp-0.1.3/lib/win64d/sqlite3.lib>
    $<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/third_party/ormpp-0.1.3/lib/win64/sqlite3.lib>
)

target_compile_definitions(rest_api PRIVATE 
    $<$<CONFIG:Debug>:ORMPP_ENABLE_SQLITE3>
    $<$<CONFIG:Release>:ORMPP_ENABLE_SQLITE3>
)


# 输出调试信息
message(STATUS "")
