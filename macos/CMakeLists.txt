cmake_minimum_required(VERSION 3.22)
project(nm_macos C)

set(CMAKE_C_STANDARD 11)

add_executable(ft_nm_macos srcs/main.c srcs/print_mach_header.c include/nm.h srcs/load_commands.c srcs/segments.c srcs/macho_64.c srcs/macho_32.c srcs/print_symbols.c srcs/sections.c)
include_directories(include)