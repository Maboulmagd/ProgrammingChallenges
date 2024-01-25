project(word_search_src)

add_library(word_search_src STATIC word_search.h word_search.cpp word_search_st.h word_search_st.cpp
 word_search_mt.h word_search_mt.cpp)
target_include_directories(word_search_src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})