project(gtest)

# 输出位置
set(EXECUTABLE_OUTPUT_PATH ${HOME}/bin)
# 指定文件
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)
# 头文件路径
include_directories(${HOME}/MySTL)
include_directories(${HOME}/Test/include)
link_directories(${HOME}/Test/lib)

add_executable(test ${SRC_LIST})
target_link_libraries(test gtest gtest_main gmock gmock_main)