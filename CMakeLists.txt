cmake_minimum_required(VERSION 3.14)

project(tortoise_graphics C)

include_directories(
        /usr/local/include/guile/2.2
        /usr/local/include/SDL2
)

set(CMAKE_C_STANDARD 99)

add_library(
        tortoise_graphics SHARED
        tortoise.c tortoise.h)

# This must come after "add_library"
target_link_libraries(
        tortoise_graphics
        /usr/lib/x86_64-linux-gnu/libm.so
#        /usr/lib/x86_64-linux-gnu/libgc.so
#        /usr/local/lib/libguile-2.2.so
)

