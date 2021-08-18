add_executable(test-parser)
target_sources(test-parser
  PRIVATE
  "main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/scpi-def.c"
  )
target_link_libraries(test-parser PRIVATE libscpi::scpi)
set_target_properties(test-parser PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  )
