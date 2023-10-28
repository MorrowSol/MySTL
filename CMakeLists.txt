cmake_minimum_required(VERSION 3.14)
project(MySTL)
# c++14标准
set(CMAKE_CXX_STANDARD 14)
set(HOME /data/bcy/MySTL/)
# gtest使用单线程
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -Dgtest_disable_pthreads=ON -pthread")
# 输出位置
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 子节点
add_subdirectory(Test)

# 指定文件
# set(SRC_LIST main.cpp)
# 搜索指定目录
# aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
# file搜索需要指定后缀
# file(GLOB SRC_LIST ${PROJECT_SOURCE_DIR}/*.cpp)

# message("no TESTXXXXXXXXXXX")
# message(STATUS "STATUSXXXXXXXXXXX")
# message(STATUS ${SRC_LIST})

# 头文件路径
# include_directories(${PROJECT_SOURCE_DIR}/Test)
# 链接静态库
# link_libraries()
# link_directories()

# add_executable(app ${SRC_LIST})
# add_library(app STATIC ${SRC_LIST})
# add_library(app SHARED ${SRC_LIST})

# 链接动态库，一般放在最后
# target_link_libraries()

# list 字符串追加删除
# list(APPEND out ${SRC_LIST})
# list(REMOVE_ITEM out XXX)

# 加入自定义宏定义
# add_definitions(-DDEBUG)