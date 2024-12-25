add_library(thirdparty INTERFACE)

#======================================= GoogleTest =======================================#
if (BUILD_TESTS)
    add_subdirectory(googletest)
endif()
