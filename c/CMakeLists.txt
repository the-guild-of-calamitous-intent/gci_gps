
cmake_minimum_required(VERSION 3.20)

# Set project name and language
project(gci_gps C)
set(CMAKE_C_STANDARD 23) # Options: 90, 99, 11, 17, 23 (or gnu90, gnu99, etc.)
set(CMAKE_C_STANDARD_REQUIRED ON) # Enforce the standard
# set(CMAKE_C_EXTENSIONS OFF) # Disable GNU extensions (optional, use ON for gnuXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")
# string(PREPEND CMAKE_EXE_LINKER_FLAGS " -Wl,--print-memory-usage ")

if(PROJECT_IS_TOP_LEVEL)
  cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
  cmake_host_system_information(RESULT OSN QUERY OS_NAME)
  cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
  cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

  message(STATUS "-------------------------------------")
  message(STATUS "  Project: ${PROJECT_NAME}")
  message(STATUS "  C ${CMAKE_C_STANDARD}")
  message(STATUS "-------------------------------------")
  message(STATUS " ${HOST}")
  message(STATUS " ${OSN}: ${OS_VERSION}")
  message(STATUS " ${PROC}")
  message(STATUS "-------------------------------------")

  set(BUILD_EXAMPLES ON)
  set(BUILD_TESTS ON)
else()
  message(STATUS "-> ${PROJECT_NAME} is submodule")
  set(BUILD_EXAMPLES OFF)
  set(BUILD_TESTS OFF)
endif()

add_library(${PROJECT_NAME}
  ../src/gga.c
  ../src/rmc.c
  ../src/gsa.c
  ../src/gps_time.c
  ../src/checksum.c
)
target_include_directories(${PROJECT_NAME} PUBLIC ../src)

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if (BUILD_TESTS)
  add_subdirectory(scruffy)
endif()