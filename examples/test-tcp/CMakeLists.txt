add_executable(test-tcp)
target_sources(test-tcp
  PRIVATE
  "main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/scpi-def.c"
  )
target_link_libraries(test-tcp PRIVATE libscpi::scpi)
set_target_properties(test-tcp PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  )
