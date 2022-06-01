cmake_minimum_required(VERSION 3.7)
project(nm C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")


file(GLOB SOURCE_FILES "srcs/*.c")
file(GLOB HEADER_FILES "include/*.h")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ")

add_executable(ft_nm ${SOURCE_FILES} ${HEADER_FILES})
add_subdirectory(libft)
#find_package(libft REQUIRED)
#target_link_libraries(ft_nm libft ${CMAKE_DL_LIBS})
target_link_libraries(ft_nm libft)
include_directories(include libft/include)
