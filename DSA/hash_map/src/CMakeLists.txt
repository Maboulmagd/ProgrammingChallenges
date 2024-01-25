project(hash_map_src)

add_library(hash_map_src hash_map.hpp)
target_include_directories(hash_map_src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(hash_map_src include)