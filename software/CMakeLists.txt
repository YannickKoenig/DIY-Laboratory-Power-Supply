

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

#init sdk
pico_sdk_init()
add_library(tm1637_lib
    displaylib_LED_PICO/tm1637.cpp
    displaylib_LED_PICO/seven_segment_font_data.cpp
)
target_link_libraries(tm1637_lib pico_stdlib)

target_include_directories(tm1637_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/displaylib_LED_PICO
)

add_executable(main main.cpp
    DisplayManager.cpp
    RegulatorManager.cpp
)

target_link_libraries(main pico_stdlib tm1637_lib hardware_pwm hardware_adc)


pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)
