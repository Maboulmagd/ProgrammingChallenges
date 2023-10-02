project(orderbook_tst)

#add_executable(orderbook_tst "main.cpp")
#target_link_libraries(orderbook_tst PUBLIC orderbook_src)

add_executable(orderbook_tests "tests.cpp")
target_link_libraries(orderbook_tests PUBLIC orderbook_src GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(orderbook_tests)