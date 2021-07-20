cmake_minimum_required(VERSION 3.14)
project(
  "haf_app_middleware"
  VERSION 0.1.1.1
  DESCRIPTION "HAF Application middleware"
  # [HOMEPAGE_URL <url-string>]
  LANGUAGES CXX)

file (GLOB_RECURSE HEADERS "*.hpp")
file (GLOB_RECURSE SOURCES "*.cpp")

set (CURRENT_TARGET ${PROJECT_NAME})

include(client_functions)

build_client_library(
    HEADERS 
    ${HEADERS}
    
    SOURCES
    ${SOURCES}
)

# Extension libraries
target_link_libraries(${CURRENT_TARGET} PRIVATE haf_user)
target_link_libraries(${CURRENT_TARGET} PRIVATE host_connector)
