cmake_minimum_required(VERSION 3.14)

file(GLOB_RECURSE PROJ_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "libft/sources/*.c")
file(GLOB_RECURSE PROJ_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "libft/includes/*.h")
file(GLOB_RECURSE LIBFT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "sources/*.c")
file(GLOB_RECURSE LIBFT_HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "includes/*.h")

project(doom-nukem C)

set(CMAKE_C_STANDARD 99)

include_directories("includes")
include_directories("libft/includes")

add_executable(doom-nukem ${PROJ_SOURCES} ${PROJ_HEADERS} ${LIBFT_SOURCES} ${LIBFT_HEADERS})

target_link_libraries(doom-nukem "-lmlx")
target_link_libraries(doom-nukem "-framework opengl")
target_link_libraries(doom-nukem "-framework appkit")