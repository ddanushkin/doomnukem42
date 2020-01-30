cmake_minimum_required(VERSION 3.14)

set(EXECUTABLE_NAME doom-nukem)

file(GLOB_RECURSE PROJ_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "sources/*.c")
file(GLOB_RECURSE LIBFT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "libft/sources/*.c")

project(${EXECUTABLE_NAME} C)
set(CMAKE_C_STANDARD 99)

include_directories("includes")
include_directories("libft/includes")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-O1 -c")
set(CMAKE_CXX_FLAGS_RELEASE "-O1 -c")
set(CMAKE_CXX_FLAGS_DEBUG "-O1 -c")

if(MINGW OR CYGWIN)
    message("-- WINDOWS --")
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    include_directories("sdl2/main/include/SDL2/")
    link_directories(sdl2/main/lib/)
    include_directories("sdl2/ttf/include/SDL2/")
    link_directories(sdl2/ttf/lib/)
    add_executable(${EXECUTABLE_NAME} ${PROJ_SOURCES} ${LIBFT_SOURCES})
    if (MINGW)
        target_link_libraries(${EXECUTABLE_NAME} "-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf")
    elseif(CYGWIN)
        target_link_libraries(${EXECUTABLE_NAME} "-lcygwin -lpthread -lSDL2main -lSDL2 -lSDL2_ttf")
    endif()
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/sdl2/main/bin/"
            $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${PROJECT_SOURCE_DIR}/sdl2/ttf/bin/"
            $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)
endif()

if(APPLE)
    message("-- MACOS --")
    include_directories("/Users/$ENV{USER}/Library/Frameworks/SDL2.framework/Versions/A/Headers/")
    include_directories("/Users/$ENV{USER}/Library/Frameworks/SDL2_ttf.framework/Versions/A/Headers/")
    add_executable(${EXECUTABLE_NAME} ${PROJ_SOURCES} ${LIBFT_SOURCES})
    target_link_libraries(${EXECUTABLE_NAME} "-F /Users/$ENV{USER}/Library/Frameworks/ -framework SDL2")
    target_link_libraries(${EXECUTABLE_NAME} "-F /Users/$ENV{USER}/Library/Frameworks/ -framework SDL2_ttf")
endif()