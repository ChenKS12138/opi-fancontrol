cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(
  opi-fancontrol
  LANGUAGES CXX
  VERSION 1.0
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BIN_NAME ${PROJECT_NAME})

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

file(
  GLOB_RECURSE
  BIN_FILES
  ${CMAKE_SOURCE_DIR}/src/*
)
add_executable(
  ${BIN_NAME}
  ${BIN_FILES}
)

# Find and link wiringPi and pthread libraries
find_library(WIRINGPI_LIB wiringPi)
if (WIRINGPI_LIB)
    message(STATUS "Found wiringPi: ${WIRINGPI_LIB}")
else()
    message(FATAL_ERROR "wiringPi library not found")
endif()

find_package(Threads REQUIRED)

# Link the libraries
target_link_libraries(${BIN_NAME} ${WIRINGPI_LIB} Threads::Threads)

# For generate compile_commands.json
add_custom_target(
  copy-if-compile ALL
  ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/compile_commands.json
  ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)
