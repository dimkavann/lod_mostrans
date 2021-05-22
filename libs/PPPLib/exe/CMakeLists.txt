PROJECT(test_main)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0015 NEW)

add_executable(PPPLibMain PPPLibMain.cc)
target_link_libraries(PPPLibMain PPPLib)



