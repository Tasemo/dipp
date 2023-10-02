include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)

if(COMPILER_SUPPORTS_MARCH_NATIVE)
    add_compile_options(-march=native)
else()
    message(NOTICE "-march=native is not supported")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -Og -fno-omit-frame-pointer)
else()
    add_compile_options(-DNDEBUG -O3)

    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_OUTPUT)

    if(IPO_SUPPORTED)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(NOTICE "IPO/LTO is not supported: ${IPO_OUTPUT}")
    endif()
endif()
