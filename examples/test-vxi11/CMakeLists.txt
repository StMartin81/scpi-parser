add_executable(test-vxi11)
target_sources(test-vxi11
  PRIVATE
  "main.c"
  "vxi11_xdr.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/../common/scpi-def.c"
  )
target_link_libraries(test-vxi11 PRIVATE libscpi::scpi)
set_target_properties(test-vxi11 PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  )
