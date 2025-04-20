cmake_minimum_required(VERSION 3.20)
project(gps_tests C CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)


# Find Google Test
find_package(GTest REQUIRED)

add_library(gps
  ../src/checksum.c
  ../src/gga.c
  ../src/gps_time.c
  ../src/gsa.c
  ../src/rmc.c
)
target_include_directories(gps PUBLIC ../src)

# Test executable
add_executable(${PROJECT_NAME} gtests.cpp)
target_link_libraries(${PROJECT_NAME} GTest::GTest GTest::Main gps)

# Enable testing
enable_testing()
add_test(NAME AllTests COMMAND ${PROJECT_NAME})