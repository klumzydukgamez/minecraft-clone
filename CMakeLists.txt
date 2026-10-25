# Copyright (c) 2026 Klumzy Duk Gamez.
# See LICENSE.md for full terms.

cmake_minimum_required(VERSION 3.25)
project(
	minecraft-clone
	VERSION 0.1.0
	LANGUAGES C CXX
)

set(CMAKE_C_STANDARD            23)
set(CMAKE_C_STANDARD_REQUIRED   ON)
set(CMAKE_C_EXTENSIONS          OFF)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build_out" CACHE PATH "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32 AND MINGW)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()

set(MC_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")
set(MC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(MC_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(CGLM_STATIC ON  CACHE BOOL "" FORCE)
set(CGLM_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory("${MC_THIRD_PARTY_DIR}/SDL" EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory("${MC_THIRD_PARTY_DIR}/glad" EXCLUDE_FROM_ALL SYSTEM)
add_subdirectory("${MC_THIRD_PARTY_DIR}/cglm" EXCLUDE_FROM_ALL SYSTEM)

add_executable(mc)
set_target_properties(
	mc PROPERTIES
	OUTPUT_NAME "minecraft-clone"
)
file(
	GLOB_RECURSE MC_SOURCES CONFIGURE_DEPENDS
	"${MC_SOURCE_DIR}/*.c"
)
target_sources(
	mc PRIVATE
	${MC_SOURCES}
)
target_include_directories(
	mc PRIVATE
	"${MC_SOURCE_DIR}"
)
target_link_libraries(
	mc PRIVATE
	SDL3::SDL3
	glad
	cglm
)
target_compile_options(
	mc PRIVATE
	"-O3"
	"--embed-dir=${MC_ASSETS_DIR}"
)

install(
	FILES
		$<TARGET_FILE:mc>
	DESTINATION "."
)

