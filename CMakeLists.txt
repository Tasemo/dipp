cmake_minimum_required(VERSION 3.14)

project(
    dipp
    VERSION 0.0.1
    HOMEPAGE_URL "https://github.com/Tasemo/dipp"
    DESCRIPTION "Distributed image processing pipeline with various distribution methods"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# all subprojects also use the INTERPROCEDURAL_OPTIMIZATION flag
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# don´t allow a option() in a suproject to override normal variables
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# don´t allow a cached variable in a subproject to override normal variables
set(CMAKE_POLICY_DEFAULT_CMP0126 NEW)

include(PreventInSourceBuilds)
include(CompilerOptions)
include(Dependencies)

add_subdirectory(src)
