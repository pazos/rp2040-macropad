cmake_minimum_required(VERSION 3.13)

include (pico_sdk_import.cmake)

project(rp2040-macropad C CXX ASM)
set(TARGET_NAME macropad)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 5)
set(VERSION_PATCH 1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

add_executable(${TARGET_NAME})
configure_file(${CMAKE_CURRENT_LIST_DIR}/src/version.h.in version.h @ONLY)

pico_sdk_init()
add_subdirectory("./Pico-PIO-USB" pico_pio_usb)
pico_generate_pio_header(macropad ${CMAKE_CURRENT_LIST_DIR}/src/led/ws2812.pio)
pico_add_extra_outputs(${TARGET_NAME})

target_include_directories(${TARGET_NAME} PRIVATE
 ${CMAKE_CURRENT_LIST_DIR}/src
 ${CMAKE_CURRENT_LIST_DIR}/src/layers
 ${CMAKE_CURRENT_LIST_DIR}/src/led
 ${CMAKE_CURRENT_LIST_DIR}/src/usb)

target_sources(${TARGET_NAME} PRIVATE
 ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/layer.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/keypad.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/led/led.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_host.cpp
 ${CMAKE_CURRENT_LIST_DIR}/src/usb/usb_device.cpp
 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/dcd_pio_usb.c
 ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c)

target_compile_definitions(${TARGET_NAME} PRIVATE PIO_USB_USE_TINYUSB)

target_link_options(${TARGET_NAME} PRIVATE)
target_link_libraries(${TARGET_NAME} PRIVATE
  pico_stdlib
  pico_unique_id
  pico_pio_usb
  tinyusb_device
  tinyusb_host)

# Use ws2812 rgb led on gp16
ADD_DEFINITIONS(-DUSE_RGB_LED)

# Use extra keys
ADD_DEFINITIONS(-DUSE_EXTRA_KEYS_EPOMAKER_TH21)

# Upload firmware as part of the build process
ADD_DEFINITIONS(-DUSE_CDC_DEVICE)
add_custom_target(upload_firmware ALL
    COMMAND ${CMAKE_SOURCE_DIR}/tools/macropad-dfu.sh ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.uf2
    DEPENDS ${TARGET_NAME}
    COMMENT "Upload firmware")
