cmake_minimum_required(VERSION 3.17)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in Pico Extras (Optional)
include(pico_extras_import.cmake)

project(pico_tusb_reset_interface_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(TARGET_NAME "pico_tusb_reset_interface_example")

set(FAMILY_MCUS RP2040)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
    -Wno-maybe-uninitialized
)

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c ${CMAKE_CURRENT_SOURCE_DIR}/src/usb_descriptors.c)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(../CMakeLists.txt)

# Pull in common dependencies
target_link_libraries(${TARGET_NAME}
    pico_stdlib
    pico_stdio_usb
    pico_unique_id
    tinyusb_device
    pico_tusb_reset_interface
)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${TARGET_NAME})
