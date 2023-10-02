option(USE_CLANG_FORMAT "Use the clang-format tool if found" ON)
option(USE_CLANG_TIDY "Use the clang-tidy tool if found" OFF)

if(USE_CLANG_FORMAT)
    find_program(CLANG_FORMAT clang-format)

    if(CLANG_FORMAT)
        file(GLOB_RECURSE SOURCES src/*.[ch]pp)
        add_custom_target(format ALL COMMAND ${CLANG_FORMAT} --Wno-error=unknown -i ${SOURCES})
    else()
        message(NOTICE "clang-format requested, but not found")
    endif()
endif()

if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY clang-tidy)

    if(CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY clang-tidy)
    else()
        message(NOTICE "clang-tidy requested, but not found")
    endif()
endif()
