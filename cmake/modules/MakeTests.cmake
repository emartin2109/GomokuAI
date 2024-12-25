if (BUILD_TESTS)
    file(GLOB_RECURSE SOURCES_TESTS ${CMAKE_SOURCE_DIR}/tests/src/*.cpp)

    add_executable(${PROJECT_NAME}-tests ${SOURCES_TESTS} ${CMAKE_SOURCE_DIR}/src/Brain.cpp ${CMAKE_SOURCE_DIR}/src/Board.cpp ${CMAKE_SOURCE_DIR}/src/Brain.cpp)

    target_link_libraries(${PROJECT_NAME}-tests PRIVATE gtest gtest_main)
    target_include_directories(${PROJECT_NAME}-tests PRIVATE ${gtest_SOURCE_DIR}/googletest/include ${CMAKE_SOURCE_DIR}/include)

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}-tests)
endif ()