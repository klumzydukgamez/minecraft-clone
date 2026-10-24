# Copyright (c) 2026 Klumzy Duk Gamez.
# See LICENSE.md for full terms.

add_library(glad STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")