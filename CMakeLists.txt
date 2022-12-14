cmake_minimum_required(VERSION 3.10)

find_package (Python3 COMPONENTS Interpreter)

if(NOT Python3_Interpreter_FOUND)
    message(FATAL_ERROR "Python3 interpreter not found!!!")
endif()

message(STATUS "Python3: " ${Python3_EXECUTABLE})
message(STATUS "Command: " "${Python3_EXECUTABLE} " "${CMAKE_CURRENT_SOURCE_DIR}/src/scripts/json_to_header.py " "${CMAKE_CURRENT_SOURCE_DIR}/src/json/table.json " "${CMAKE_CURRENT_BINARY_DIR}/table.cpp")


add_custom_command(
    OUTPUT table.cpp
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/scripts/json_to_header.py ${CMAKE_CURRENT_SOURCE_DIR}/src/json/table.json ${CMAKE_CURRENT_BINARY_DIR}/table.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/scripts/json_to_header.py ${CMAKE_CURRENT_SOURCE_DIR}/src/json/table.json
)

include_directories(src)

if(TARGET_RASPI_PICO)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
endif()

project(SeniorCapstoneProject2022)

set(CMAKE_CXX_STANDARD 17)
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
	src/picoEsim.cpp
    table.cpp
)

set(program2_NAME picoEsim)
add_executable(${program2_NAME})

target_sources(${program2_NAME} PRIVATE ${program2_SRC})


if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${program2_NAME} pico_stdlib pico_multicore)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${program2_NAME} 1)
    pico_enable_stdio_uart(${program2_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${program2_NAME})

    target_compile_definitions(${program2_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
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

set(VCP_SRC
    src/VCP.cpp
    table.cpp
)

set(VCP_NAME VCP)
add_executable(${VCP_NAME})

target_sources(${VCP_NAME} PRIVATE ${VCP_SRC})

if(TARGET_RASPI_PICO)
    # pull in common dependencies
    target_link_libraries(${VCP_NAME} pico_stdlib)

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${VCP_NAME} 1)
    pico_enable_stdio_uart(${VCP_NAME} 0)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${VCP_NAME})

    target_compile_definitions(${VCP_NAME} PRIVATE COMPILING_FOR_RASPI_PICO=1)
endif()


if(WIN32)

set(engineSim_SRC
    src/engineSim.cpp
    table.cpp
)

set(engineSim_NAME engineSim)
add_executable(${engineSim_NAME})

target_sources(${engineSim_NAME} PRIVATE ${engineSim_SRC})


set(run_both_programs_SRC
    src/run_both_programs.cpp
)

set(run_both_programs_NAME run_both_programs)
add_executable(${run_both_programs_NAME})

target_link_libraries(${run_both_programs_NAME} shlwapi.lib)

target_sources(${run_both_programs_NAME} PRIVATE ${run_both_programs_SRC})

add_dependencies(${run_both_programs_NAME} ${engineSim_NAME} ${VCP_NAME})

endif()
