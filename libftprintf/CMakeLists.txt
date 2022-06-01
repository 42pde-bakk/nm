cmake_minimum_required(VERSION 3.7)
project(ft_printf C)
set(CMAKE_C_STANDARD 11)

# Add c files of project root directory as source file
file(GLOB SOURCE_FILES "src/*.c")
file(GLOB HEADER_FILES "include/*.h")
file(GLOB HEADER_FILES "include_internal/*.h")

include_directories(include libft/include include_internal)
add_library(ftprintf ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(ftprintf PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(tests)