cmake_minimum_required(VERSION 3.20)

#set(VCPKG_ROOT "/usr/local/vcpkg")
#message("VCPKG_ROOT: ${VCPKG_ROOT}")

# Since I'm developing in VSCode within a Docker container that has Vcpkg installed
#set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(programming_challenges)

set(CMAKE_CXX_STANDARD 23)

# execute_process(
#     COMMAND bash -c "whereis cmake"
#     OUTPUT_VARIABLE output
#     OUTPUT_STRIP_TRAILING_WHITESPACE
# )

# message("Output: ${output}")

# # Print the value of CMAKE_PREFIX_PATH
# message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

# # Print the value of CMAKE_PREFIX_PATH
# message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

add_subdirectory(include)
add_subdirectory(LeetCode)
add_subdirectory(DSA)
add_subdirectory(orderbook)
add_subdirectory(shared_memory)