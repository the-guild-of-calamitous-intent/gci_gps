
cmake_minimum_required(VERSION 3.25)

# Set project name and language
project(gci_gps LANGUAGES C VERSION "2024.06.16")
set(CMAKE_C_STANDARD 23) # Options: 90, 99, 11, 17, 23 (or gnu90, gnu99, etc.)
# set(CMAKE_C_STANDARD_REQUIRED ON) # Enforce the standard
# set(CMAKE_C_EXTENSIONS OFF) # Disable GNU extensions (optional, use ON for gnuXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
# string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")

# message(STATUS "toplevel: ${PROJECT_IS_TOP_LEVEL}")

if(PROJECT_IS_TOP_LEVEL)
  cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
  cmake_host_system_information(RESULT OSN QUERY OS_NAME)
  cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
  cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

  message(STATUS "-------------------------------------")
  message(STATUS " Project: ${PROJECT_NAME}")
  message(STATUS "-------------------------------------")
  message(STATUS " Compiler")
  message(STATUS "  C ${CMAKE_C_STANDARD}: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} (${CMAKE_C_COMPILER})")
# message(STATUS "  CXX ${CMAKE_CXX_STANDARD}: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION} (${CMAKE_CXX_COMPILER})")
  # message(STATUS " ")
  # message(STATUS " Hardware: ${PICO_BOARD} ${PICO_PLATFORM}")
  message(STATUS "-------------------------------------")
  message(STATUS " ${HOST}")
  message(STATUS " ${OSN}: ${OS_VERSION}")
  message(STATUS " ${PROC}")
  message(STATUS "-------------------------------------")

  set(BUILD_EXAMPLES ON)
  set(BUILD_TESTS ON)
else()
  if(PICO_PLATFORM)
    message(STATUS "-> ${PROJECT_NAME} is submodule for ${PICO_PLATFORM}")
  else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
  endif()
  set(BUILD_EXAMPLES OFF)
  set(BUILD_TESTS OFF)
endif()

add_library(${PROJECT_NAME}
  src/nema/gga.c
  src/nema/rmc.c
  src/nema/gsa.c
  src/nema/gps_time.c
  src/nema/checksum.c
  src/ublox/ubx_parser.c
  src/gps.c
)
target_include_directories(${PROJECT_NAME} PUBLIC src)

message(STATUS "Building ${PROJECT_NAME} examples is ${BUILD_EXAMPLES}")
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

message(STATUS "Building ${PROJECT_NAME} scruffy is ${BUILD_TESTS}")
if (BUILD_TESTS)
  add_subdirectory(scruffy)
endif()