set(TEST_SOURCES "test_mylib.cpp")

add_executable(${TEST_MAIN} ${TEST_SOURCES})

#settings
target_link_libraries(${TEST_MAIN} PRIVATE ${LIBRARY_NAME} ${Boost_FILESYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
target_compile_features(${TEST_MAIN} PUBLIC cxx_std_17)

# test
add_test(NAME ${LIBRARY_NAME}.${TEST_MAIN} COMMAND ${TEST_MAIN})

# Coverage
if (ENABLE_COVERAGE)
    set(COVERAGE_MAIN "${PROJECT}_coverage")
    set(COVERAGE_LCOV_EXCLUDES
        "${PROJECT_SOURCE_DIR}/app/*"
        "${PROJECT_SOURCE_DIR}/cmake/*"
        "${PROJECT_SOURCE_DIR}/docs/*"
        "${PROJECT_SOURCE_DIR}/external/*"
        "${PROJECT_SOURCE_DIR}/tests/*"
        "/usr/include/*")
    setup_target_for_coverage_lcov(
        NAME ${COVERAGE_MAIN}
        EXECUTABLE ctest
        DEPENDENCIES ${TEST_MAIN}
    )
endif()