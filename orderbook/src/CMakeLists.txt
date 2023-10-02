project(orderbook_src)

add_library(orderbook_src STATIC enums.h orderbook.h orderbook.cpp utilities.h utilities.cpp)
target_include_directories(orderbook_src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})