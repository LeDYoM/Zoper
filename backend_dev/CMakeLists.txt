project (backend_dev)
cmake_minimum_required (VERSION 3.11)

set (CURRENT_TARGET ${PROJECT_NAME})

build_lib_interface_component(
    HEADER_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
