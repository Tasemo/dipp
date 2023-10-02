get_filename_component(SRC_DIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(BIN_DIR "${CMAKE_BINARY_DIR}" REALPATH)

if("${SRC_DIR}" STREQUAL "${BIN_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed!")
endif()
