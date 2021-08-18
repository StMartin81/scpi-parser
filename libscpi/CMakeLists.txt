cmake_minimum_required(VERSION 3.2)

project(libscpi
  VERSION "${LIBSCPI_VERSION_STRING}")

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(WITH_COMMAND_TAGS "Enable command tags field" ON)
option(WITH_USER_ERROR_LIST "Enable also LIST_OF_USER_ERRORS to be included" OFF)
option(WITH_FULL_ERROR_LIST "Enable full error list" ON)

set(LIBSCPI_VERSION_MAJOR 2)
set(LIBSCPI_VERSION_MINOR 1)
set(LIBSCPI_VERSION_PATCH 0)
set(LIBSCPI_VERSION_STRING "${LIBSCPI_VERSION_MAJOR}.${LIBSCPI_VERSION_MINOR}.${LIBSCPI_VERSION_PATCH}")

include(GNUInstallDirs)
include(GenerateExportHeader)

if(WITH_COMMAND_TAGS)
  set(USE_COMMAND_TAGS 1)
endif()
if(WITH_USER_ERROR_LIST)
  set(USE_USER_ERROR_LIST 1)
endif()
if(WITH_FULL_ERROR_LIST)
  set(USE_FULL_ERROR_LIST 1)
endif()

configure_file("inc/scpi/config.h.in" "scpi/config.h")

set(SCPI_SRCS
  "src/error.c"
  "src/fifo.c"
  "src/ieee488.c"
  "src/minimal.c"
  "src/parser.c"
  "src/units.c"
  "src/utils.c"
  "src/lexer.c"
  "src/expression.c"
  )
set(SCPI_HDRS_PUBLIC
  "inc/scpi/scpi.h"
  "${CMAKE_CURRENT_BINARY_DIR}/scpi/config.h"
  "inc/scpi/constants.h"
  "inc/scpi/error.h"
  "inc/scpi/ieee488.h"
  "inc/scpi/minimal.h"
  "inc/scpi/parser.h"
  "inc/scpi/types.h"
  "inc/scpi/units.h"
  "inc/scpi/expression.h"
  )
set(SCPI_HDRS_PRIVATE
  "src/lexer_private.h"
  "src/utils_private.h"
  "src/fifo_private.h"
  "src/parser_private.h"
  )

add_library(scpi
  ${SCPI_SRCS}
  ${SCPI_HDRS_PUBLIC}
  ${SCPI_HDRS_PRIVATE}
  )
add_library(libscpi::scpi ALIAS scpi)

generate_export_header(scpi
  EXPORT_FILE_NAME "scpi/scpi_export.h"
  )

set_target_properties(scpi PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
  PUBLIC_HEADER "${SCPI_HDRS_PUBLIC}"
  VERSION "${LIBSCPI_VERSION_STRING}"
  SOVERSION "${LIBSCPI_VERSION_MAJOR}"
  C_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN TRUE
  )
target_include_directories(scpi
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/scpi>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/inc>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
