cmake_minimum_required(VERSION 3.7)
project(nm C)

set(CMAKE_C_STANDARD 11)

file(GLOB SOURCE_FILES "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")

add_executable(ft_nm ${SOURCE_FILES} ${HEADER_FILES})
include_directories(include libftprintf/include libftprintf/libft/include)
