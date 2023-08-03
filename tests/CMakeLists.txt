find_package(GTest MODULE REQUIRED)

set(PARSER_TEST_NAME expressionParserTest)

add_executable(${PARSER_TEST_NAME} "main.cpp" "test_parser.cpp")

target_include_directories(${PARSER_TEST_NAME} PRIVATE ${GTest_INCLUDE_DIRS} ../src)

target_link_libraries(${PARSER_TEST_NAME} PRIVATE GTest::gtest)
