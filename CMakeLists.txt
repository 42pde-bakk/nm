cmake_minimum_required(VERSION 3.22)
project(nm C)

set(CMAKE_C_STANDARD 11)

add_executable(ft_nm srcs/main.c srcs/print_mach_header.c include/nm.h srcs/load_commands.c srcs/segments.c srcs/macho.c)
include_directories(include)