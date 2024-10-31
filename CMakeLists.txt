# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD adafruit_feather_rp2040 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(EmbeddedDriverLab C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

#add_executable(EmbeddedDriverLab EmbeddedDriverLab.c )
add_executable(EmbeddedDriverLab src/level.c libraries/LIS3DH.c libraries/neomatrix.c )
pico_generate_pio_header(EmbeddedDriverLab ${CMAKE_CURRENT_LIST_DIR}/libraries/neopixel.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/libraries/generated)

pico_set_program_name(EmbeddedDriverLab "EmbeddedDriverLab")
pico_set_program_version(EmbeddedDriverLab "0.1")

# Generate PIO header
pico_generate_pio_header(EmbeddedDriverLab ${CMAKE_CURRENT_LIST_DIR}/libraries/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(EmbeddedDriverLab 0)
pico_enable_stdio_usb(EmbeddedDriverLab 1)

# Add the standard library to the build
target_link_libraries(EmbeddedDriverLab
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(EmbeddedDriverLab PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(EmbeddedDriverLab 
        hardware_i2c
        hardware_pio
        )

pico_add_extra_outputs(EmbeddedDriverLab)

