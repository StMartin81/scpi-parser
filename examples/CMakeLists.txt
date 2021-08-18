cmake_minimum_required(VERSION 3.15.0)

project(tests)

add_subdirectory(test-interactive)
add_subdirectory(test-parser)
add_subdirectory(test-tcp)
add_subdirectory(test-tcp-srq)
#add_subdirectory(test-vxi11)

