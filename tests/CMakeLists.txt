#@+leo-ver=5-thin
#@+node:gcross.20101121162317.1585: * @thin CMakeLists.txt
add_executable(test EXCLUDE_FROM_ALL
    examples
    fake2
    utility
    fake1
    template
)
target_link_libraries(test unit-- unit--main)
set_property(TARGET test PROPERTY RUNTIME_OUTPUT_DIRECTORY ..)
#@-leo
