

set(TEST_NAMES
    evaluator_tests
    utils_tests)

foreach(TEST_NAME ${TEST_NAMES})

    add_executable(${TEST_NAME} ${TEST_NAME}.cc)

    target_link_libraries(${TEST_NAME} PRIVATE
        GTest::GTest GTest::Main
        wordle_evaluator)

    add_test(${TEST_NAME} ${TEST_NAME})

endforeach()
