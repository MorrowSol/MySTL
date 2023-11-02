# 输出位置
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)
# 指定文件
# aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
# aux_source_directory(${HOME}/OtherTest SRC_LIST)

# set(SRC_LIST ${HOME}/OtherTest/typedef.cpp)
set(SRC_LIST ${HOME}/OtherTest/rebind.cpp)

message(${SRC_LIST})
# 头文件路径
include_directories(${HOME}/MySTL)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -Wall")

# add_executable(test2 ${SRC_LIST})