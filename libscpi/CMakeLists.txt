cmake_minimum_required(VERSION 3.15.0)

project(libscpi
  VERSION "${LIBSCPI_VERSION_STRING}")

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(WITH_COMMAND_TAGS "Enable command tags field" ON)
option(WITH_USER_ERROR_LIST "Enable also LIST_OF_USER_ERRORS to be included" OFF)
option(BUILD_TESTS "Build test executables" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/..cmake-cunit/cmake/Modules")

set(LIBSCPI_VERSION_MAJOR 2)
set(LIBSCPI_VERSION_MINOR 1)
set(LIBSCPI_VERSION_PATCH 0)
set(LIBSCPI_VERSION_STRING "${LIBSCPI_VERSION_MAJOR}.${LIBSCPI_VERSION_MINOR}.${LIBSCPI_VERSION_PATCH}")

include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)

if(WITH_COMMAND_TAGS)
  set(USE_COMMAND_TAGS 1)
endif()
if(WITH_USER_ERROR_LIST)
  set(USE_USER_ERROR_LIST 1)
endif()

add_library(scpi)
add_library(libscpi::scpi ALIAS scpi)

generate_export_header(scpi
  EXPORT_FILE_NAME "scpi/scpi_export.h"
  )
configure_file("inc/scpi/config.h.in" "scpi/config.h")

set(SCPI_PUBLIC_HEADER_FILES
  "${CMAKE_CURRENT_BINARY_DIR}/scpi/config.h"
  "${CMAKE_CURRENT_BINARY_DIR}/scpi/scpi_export.h"
  )

target_sources(scpi
  PRIVATE
  ${SCPI_PUBLIC_HEADER_FILES}
  )

set_property(
  TARGET scpi
  APPEND PROPERTY PUBLIC_HEADER
  "${SCPI_PUBLIC_HEADER_FILES}"
  )

add_subdirectory("src")
add_subdirectory("inc/scpi")

set_target_properties(scpi PROPERTIES
  LANGUAGES C
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
  VERSION "${LIBSCPI_VERSION_STRING}"
  SOVERSION "${LIBSCPI_VERSION_MAJOR}"
  C_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN TRUE
  )

target_include_directories(scpi
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc/scpi>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )


if(BUILD_TESTS)
  add_subdirectory(test)
endif()

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/scpi)

# 'make install' to the correct locations (provided by GNUInstallDirs).
install(TARGETS scpi
  EXPORT scpiTargets
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT library
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT library
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"  COMPONENT library # This is for Windows
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/scpi" COMPONENT development
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )

install(EXPORT scpiTargets
  FILE "scpiTargets.cmake"
  NAMESPACE "libscpi::"
  DESTINATION "${ConfigPackageLocation}"
  COMPONENT development
  )

configure_package_config_file(
  "scpiConfig.cmakeIn"
  "scpiConfig.cmake"
  INSTALL_DESTINATION "${ConfigPackageLocation}"
  PATH_VARS CMAKE_INSTALL_PREFIX
  )

write_basic_package_version_file(
  "scpiConfigVersion.cmake"
  VERSION ${LIBSCPI_VERSION_STRING}
  COMPATIBILITY AnyNewerVersion
  )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/scpiConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/scpiConfigVersion.cmake"
  DESTINATION
  "${ConfigPackageLocation}"
  COMPONENT
  development
  )


