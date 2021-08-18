set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake-cunit/cmake/Modules")

include(CUnit)
include(CTest)

add_cunit_executable(test_fifo test_fifo.c)
target_link_libraries(test_fifo PRIVATE libscpi::scpi)

add_cunit_executable(test_lexer_parser test_lexer_parser.c)
target_link_libraries(test_lexer_parser PRIVATE libscpi::scpi)

add_cunit_executable(test_parser test_parser.c)
target_link_libraries(test_parser PRIVATE libscpi::scpi)

add_cunit_executable(test_scpi_utils test_scpi_utils.c)
target_link_libraries(test_scpi_utils PRIVATE libscpi::scpi)
