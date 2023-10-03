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

set(LIFT_BUILD_TESTS FALSE)
set(LIFT_CODE_COVERAGE FALSE)
set(LIFT_BUILD_EXAMPLES FALSE)
FetchContent_Declare(
    lifthttp
    SYSTEM
    GIT_REPOSITORY https://github.com/jbaldwin/liblifthttp.git
    GIT_TAG v2022.1
    GIT_SHALLOW TRUE
)

get_directory_property(OLD_OPTIONS COMPILE_OPTIONS)
add_compile_options(-w)
set(OLD_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OLD_RUNTIME_DIR}/tools)
FetchContent_MakeAvailable(thrill lifthttp)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OLD_RUNTIME_DIR})
set_directory_properties(PROPERTIES COMPILE_OPTIONS "${OLD_OPTIONS}")

get_target_property(thrill_srcdir thrill SOURCE_DIR)
add_custom_target(
    copy_thrill_scripts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${thrill_srcdir}/../run ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/deploy
    COMMAND ${CMAKE_COMMAND} -E copy
    ${thrill_srcdir}/../misc/json2graphviz.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools
    COMMAND ${CMAKE_COMMAND} -E copy
    ${thrill_srcdir}/../misc/standalone_profiler_run.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tools
    DEPENDS thrill
)
