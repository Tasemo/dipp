option(USE_CLANG_FORMAT "Use the clang-format tool if found" ON)

if(USE_CLANG_FORMAT)
    find_program(CLANG_FORMAT clang-format)

    if(CLANG_FORMAT)
        file(GLOB_RECURSE SOURCES src/*.[ch]pp)
        add_custom_target(format ALL COMMAND ${CLANG_FORMAT} --Wno-error=unknown -i ${SOURCES})
    else()
        message(NOTICE "clang-format requested, but not found")
    endif()
endif()
