cmake_minimum_required(VERSION 3.10)

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
endif()


set(program2_SRC
	src/program2.cpp
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
endif()

set(program3_SRC
	src/program3.cpp
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
endif()
