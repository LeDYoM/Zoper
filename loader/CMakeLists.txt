cmake_minimum_required(VERSION 3.14)
project(
  "Loader"
  VERSION 1.1.1.2
  DESCRIPTION "Multiplatform library to load shared libraries system agnostic"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX)

set(CURRENT_TARGET loader)

set(SOURCES "src/loadedinstance.cpp" "src/loader.cpp")

add_library(${CURRENT_TARGET} SHARED ${SOURCES})

target_compile_definitions(${CURRENT_TARGET} PRIVATE loader_EXPORTS)

target_include_directories(
  ${CURRENT_TARGET}
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/)

target_link_libraries(${CURRENT_TARGET} ${CMAKE_DL_LIBS})
