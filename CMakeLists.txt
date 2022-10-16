cmake_minimum_required(VERSION 3.10)

find_package (Python3 COMPONENTS Interpreter)

if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python3 interpreter not found!!!")
endif()

message(STATUS "Python3: " ${Python3_EXECUTABLE})

add_custom_command(
    OUTPUT table.cpp
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/json_to_header.py ${CMAKE_CURRENT_SOURCE_DIR}/src/table.json ${CMAKE_CURRENT_BINARY_DIR}/table.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/json_to_header.py ${CMAKE_CURRENT_SOURCE_DIR}/src/table.json
)


if(TARGET_RASPI_PICO)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

project(SeniorCapstoneProject2022)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(TARGET_RASPI_PICO)
pico_sdk_init()
endif()

set(program1_SRC
	src/program1.cpp
    table.cpp
)

set(program1_NAME program1)
add_executable(${program1_NAME})

target_sources(${program1_NAME} PRIVATE ${program1_SRC})

if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${program1_NAME} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${program1_NAME} 1)
    pico_enable_stdio_uart(${program1_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${program1_NAME})

    target_compile_definitions(${program1_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
endif()


set(program2_SRC
	src/program2.cpp
    table.cpp
)

set(program2_NAME program2)
add_executable(${program2_NAME})

target_sources(${program2_NAME} PRIVATE ${program2_SRC})


if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${program2_NAME} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${program2_NAME} 1)
    pico_enable_stdio_uart(${program2_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${program2_NAME})

    target_compile_definitions(${program2_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
endif()

set(program3_SRC
	src/program3.cpp
    table.cpp
)

set(program3_NAME program3)
add_executable(${program3_NAME})

target_sources(${program3_NAME} PRIVATE ${program3_SRC})


if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${program3_NAME} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${program3_NAME} 1)
    pico_enable_stdio_uart(${program3_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${program3_NAME})

    target_compile_definitions(${program3_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
endif()

set(input_from_gpio_sample_SRC
    src/input_from_gpio_sample.cpp
    table.cpp
)

set(input_from_gpio_sample_NAME input_from_gpio_sample)
add_executable(${input_from_gpio_sample_NAME})

target_sources(${input_from_gpio_sample_NAME} PRIVATE ${input_from_gpio_sample_SRC})


if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${input_from_gpio_sample_NAME} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${input_from_gpio_sample_NAME} 1)
    pico_enable_stdio_uart(${input_from_gpio_sample_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${input_from_gpio_sample_NAME})

    target_compile_definitions(${input_from_gpio_sample_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
endif()
