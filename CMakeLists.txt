cmake_minimum_required(VERSION 3.12)
set(PICO_SDK_PATH "~/Libraries/pico-sdk")

# pull in pico sdk (must be before project)
include(pico_sdk_import.cmake)

project(program C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# initialize the sdk
pico_sdk_init()

add_executable(program
    program.c
)

# pull in out pico_stdlib which pulls in common-
target_link_libraries(program pico_stdlib pico_malloc hardware_pwm hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(program)
