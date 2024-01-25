project(hash_map_src)

add_library(hash_map_src hash_map.h hash_map.cpp)
target_link_libraries(hash_map_src include)