project(gtest)

# 输出位置
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)
# 指定文件
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
message(${SRC_LIST})
# 头文件路径
include_directories(${HOME}/MySTL)
include_directories(${HOME}/Test/include)
link_directories(${HOME}/Test/lib)
# gtest使用单线程
# set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -Dgtest_disable_pthreads=ON -pthread")
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -pthread")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

add_definitions(-DMYTEST)
add_definitions(-DMYTESTLOG)

add_executable(test ${SRC_LIST})
target_link_libraries(test gtest gtest_main gmock gmock_main)