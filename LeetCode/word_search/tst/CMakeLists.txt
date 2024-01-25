project(word_search_tst)

add_executable(word_search_tst "main.cpp")
target_link_libraries(word_search_tst PUBLIC include word_search_src)