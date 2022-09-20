cmake_minimum_required(VERSION 3.13)

project(ultraclient C)

include(wakaama.cmake)

add_executable(ultraclient main.c object_test.c)
target_compile_definitions(ultraclient PRIVATE LWM2M_CLIENT_MODE)
target_sources_wakaama(ultraclient)