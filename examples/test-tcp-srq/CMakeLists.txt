add_executable(test-tcp-srq)
target_sources(test-tcp-srq
  PRIVATE
  "main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/scpi-def.c"
  )
target_link_libraries(test-tcp-srq PRIVATE libscpi::scpi)
set_target_properties(test-tcp-srq PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  )
