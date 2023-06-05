cmake_minimum_required(VERSION 3.17)
project(pico_tusb_reset_interface C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(PTUSB_RESET_TARGET "pico_tusb_reset_interface")

add_library(${PTUSB_RESET_TARGET} INTERFACE)
target_sources(${PTUSB_RESET_TARGET} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/pico_tusb_reset_interface.c)
target_include_directories(${PTUSB_RESET_TARGET} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(${PTUSB_RESET_TARGET} INTERFACE pico_stdlib pico_stdio_usb tinyusb_device)
