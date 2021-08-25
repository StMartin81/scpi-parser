set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake-cunit/cmake/Modules")

include(CUnit)
enable_testing()

add_cunit_executable(tests test_fifo.c test_lexer_parser.c test_parser.c test_scpi_utils.c)
target_link_libraries(tests PRIVATE libscpi::scpi)
