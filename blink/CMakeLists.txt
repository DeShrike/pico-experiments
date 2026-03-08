cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(PICO_BOARD pico2 CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(blink C CXX ASM)

pico_sdk_init()

add_executable(blink
    blink.c
    )

target_link_libraries(blink pico_stdlib)

pico_add_extra_outputs(blink)

