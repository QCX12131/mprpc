cmake_minimum_required(VERSION 3.0)

project(mprcp)

#生成debug版本，可以进行调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置项目库文件的输出搜索路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
#  设置项目编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/examples) 
#  设置项目库文件的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)


#src包含了mprpc框架所有的相关代码
add_subdirectory(src)
#examples包含了框架使用的相关代码
add_subdirectory(examples)