project(examples)

add_library(mpn STATIC ../src/mpn.c)

add_executable(mpnclient src/mpnclient.c)
target_link_libraries(mpnclient mpn)