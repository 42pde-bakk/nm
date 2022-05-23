cmake_minimum_required(VERSION 3.7)
project(nm C)

set(CMAKE_C_STANDARD 11)

add_executable(ft_nm include/nm.h srcs/main.c srcs/elf32.c srcs/elf64.c srcs/archive.c srcs/endian.c srcs/sort.c srcs/parse_options.c)
include_directories(include libc_vector/include)
