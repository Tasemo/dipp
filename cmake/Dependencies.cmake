include(FetchContent)

set(THRILL_BUILD_TESTS FALSE)
set(THRILL_BUILD_EXAMPLES FALSE)
set(THRILL_BUILD_EXAMPLES_MORE FALSE)
FetchContent_Declare(
    thrill
    SYSTEM
    GIT_REPOSITORY https://github.com/thrill/thrill.git
    GIT_TAG origin/master
    GIT_SHALLOW TRUE
)

get_directory_property(OLD_OPTIONS COMPILE_OPTIONS)
add_compile_options(-w)
FetchContent_MakeAvailable(thrill)
set_directory_properties(PROPERTIES COMPILE_OPTIONS "${OLD_OPTIONS}")
