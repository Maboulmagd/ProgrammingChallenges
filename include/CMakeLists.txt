project(include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/16f637fbf4ffc3f7a01fa4eceb7906634565242f.zip)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(include include.h include.cpp ../chapter_01_arrays_and_strings/problem_01_01_is_unique.cpp)
target_include_directories(include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(include gtest_main gmock_main)

#include(GoogleTest)
#gtest_discover_tests(include)