option(USE_CLANG_FORMAT "Use the clang-format tool if found" ON)
option(USE_CLANG_TIDY "Use the clang-tidy tool if found" OFF)
option(USE_DOXYGEN "Use the doxygen tool if found" ON)

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

if(USE_DOXYGEN)
    find_package(Doxygen OPTIONAL_COMPONENTS dot)

    if(DOXYGEN_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            doxygen_theme
            GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
            GIT_TAG v2.2.1
        )
        FetchContent_MakeAvailable(doxygen_theme)

        set(DOXYGEN_QUIET YES)
        set(DOXYGEN_OUTPUT_DIRECTORY ../docs)
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DODYGEN_DISABLE_INDEX NO)
        set(DOXYGEN_FULL_SIDEBAR NO)
        set(DOXYGEN_HTML_COLORSTYLE LIGHT)
        set(DOXYGEN_HTML_EXTRA_STYLESHEET "${doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")
        doxygen_add_docs(docs src ALL)
    else()
        message(NOTICE "doxygen requested, but not found")
    endif()
endif()