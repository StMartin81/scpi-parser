add_executable(test-interactive)
target_sources(test-interactive
  PRIVATE
  "main.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/scpi-def.c"
  )
target_link_libraries(test-interactive PRIVATE libscpi::scpi)
set_target_properties(test-interactive PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  )
