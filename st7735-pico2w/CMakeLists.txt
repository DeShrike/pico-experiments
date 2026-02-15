# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(PROJECTNAME st7735test)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()


# library configuration (override if necessary)
set(SPI_TFT_PORT "spi1" CACHE STRING "TFT spi port-name")
set(SPI_TFT_CS   "9"    CACHE STRING "TFT CS  pin number")
set(SPI_TFT_DC   "10"   CACHE STRING "TFT DC  pin number")
set(SPI_TFT_RST  "11"   CACHE STRING "TFT RST pin number")
set(TFT_OPTIONS TFT_ENABLE_GREEN TFT_ENABLE_RESET TFT_ENABLE_TEXT TFT_ENABLE_SHAPES
                TFT_ENABLE_ROTATE TFT_ENABLE_SCROLL
            CACHE STRING "TFT options/functions")

# TFT options/functions. Complete list:
#  - TFT_ENABLE_BLACK
#  - TFT_ENABLE_GREEN
#  - TFT_ENABLE_RED
#  - TFT_ENABLE_GENERIC
#  - TFT_ENABLE_TEXT
#  - TFT_ENABLE_FONTS
#  - TFT_ENABLE_ALL
#  - TFT_ENABLE_RESET
#  - TFT_ENABLE_SCROLL
#  - TFT_ENABLE_SHAPES
#  - TFT_ENABLE_ROTATE
#  - TFT_ENABLE_BMP  (not implemented yet)

add_compile_definitions(SPI_TFT_PORT=${SPI_TFT_PORT})
add_compile_definitions(SPI_TFT_CS=${SPI_TFT_CS})
add_compile_definitions(SPI_TFT_DC=${SPI_TFT_DC})
add_compile_definitions(SPI_TFT_RST=${SPI_TFT_RST})
foreach(opt IN LISTS TFT_OPTIONS)
    message(">>> using: ${opt}")
    add_compile_definitions(${opt})
endforeach(opt)

project(${PROJECTNAME} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

#add_subdirectory(./lib/ST7735)

#include_directories(./lib/ST7735)

add_executable(${PROJECTNAME}
               main.c
               hw.c
               ST7735_TFT.c
              )

pico_set_program_name(${PROJECTNAME}  "ST7735 Test")
pico_set_program_version(${PROJECTNAME} "0.1")

pico_enable_stdio_uart(${PROJECTNAME} 0)
pico_enable_stdio_usb(${PROJECTNAME} 1)

# Add the standard library to the build

add_compile_options(-O2)

target_link_libraries(${PROJECTNAME}
                        #ST7735
                        pico_stdlib
                        hardware_spi
                        hardware_i2c
                        hardware_dma
                        )

# Add the standard include files to the build
target_include_directories(${PROJECTNAME}   PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(${PROJECTNAME})
