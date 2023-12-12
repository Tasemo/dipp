add_compile_options(
    -Wall -Wextra -Wpedantic -Wcast-qual -Wold-style-cast -Wsuggest-override -Wextra-semi
    -Wnon-virtual-dtor -Woverloaded-virtual -Wnull-dereference -Wdouble-promotion
    -Wcast-align -Wsign-conversion -Wformat=2 -Wimplicit-fallthrough
)

if(NOT CPU_ARCHITECTURE)
    set(CPU_ARCHITECTURE "native")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=${CPU_ARCHITECTURE}" COMPILER_SUPPORTS_ARCH)

if(COMPILER_SUPPORTS_ARCH)
    add_compile_options(-march=${CPU_ARCHITECTURE})
else()
    message(FATAL_ERROR "-march=${CPU_ARCHITECTURE} is not supported by the compiler")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -Og -fno-omit-frame-pointer)
else()
    add_compile_options(-DNDEBUG -O3)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_OUTPUT)

    if(IPO_SUPPORTED)
        if(NOT(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION LESS 17))
            # build fails with IPO on clang <= 16, as they need a special linker (not just ld)
            # clang 17 supports fat LTO, which works universally but with longer compile times
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif()
    else()
        message(NOTICE "IPO/LTO is not supported: ${IPO_OUTPUT}")
    endif()
endif()
