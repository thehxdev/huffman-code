cmake_minimum_required(VERSION 3.25)

project(
	huffman-code
	VERSION 0.0.1
	LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
	set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES
		${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

add_executable(
	hc
	main.c
	huffman_code.c
	arena.c
)

target_compile_features(hc PRIVATE c_std_99)

if (MSVC)
	target_compile_options(hc PRIVATE $<BUILD_INTERFACE:/W3>)
else()
	target_compile_options(hc PRIVATE $<BUILD_INTERFACE:-Wall;-Wextra>)
	target_compile_options(hc PRIVATE $<$<CONFIG:Debug>:$<BUILD_INTERFACE:-g3>>)
endif()
