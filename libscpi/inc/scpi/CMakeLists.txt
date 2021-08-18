set(SCPI_PUBLIC_HEADER_FILES
  "scpi.h"
  "constants.h"
  "error.h"
  "ieee488.h"
  "minimal.h"
  "parser.h"
  "types.h"
  "units.h"
  "expression.h"
  "utils.h"
  "cc.h"
  )
target_sources(scpi
  PRIVATE
  ${SCPI_PUBLIC_HEADER_FILES}
  )
list(TRANSFORM SCPI_PUBLIC_HEADER_FILES PREPEND "inc/scpi/")
set_property(
  TARGET scpi
  APPEND PROPERTY PUBLIC_HEADER
  "${SCPI_PUBLIC_HEADER_FILES}"
  )
