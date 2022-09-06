cmake_minimum_required(VERSION 3.17)
project(projetoPPPfinal C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra)

add_executable(parte1 main.c aux_main.c head.h lib-utf8.c lib-utf8.h)
add_executable(parte2 main2.c aux_main2.c head2.h lib-utf8.c lib-utf8.h)