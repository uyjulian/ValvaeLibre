
include(FetchContent)
FetchContent_Declare(
    arduino_compat
    GIT_REPOSITORY https://github.com/earlephilhower/arduino-pico.git
    GIT_TAG        cf63040c5aa8306cfdb34c66b5a9cb4e75a4fad8
    GIT_SUBMODULES_RECURSE OFF
    GIT_SUBMODULES ""
)

if(NOT arduino_compat_POPULATED)
    FetchContent_Populate(arduino_compat)
endif()

add_library(arduino-compat INTERFACE)

target_compile_options(arduino-compat INTERFACE -DARDUINO=200 -DARDUINO_ARCH_RP2040 -DF_CPU=125000000UL -DNO_USB)

target_sources(arduino-compat INTERFACE
    # The Serial global object uses pico stdio so the following are not necessary
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/SerialUSB.cpp
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/RP2040USB.cpp

    # nb. serialEvent() functions will never be called when this is disabled
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/main.cpp

    # not needed
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/posix.cpp

    # millis already defined in delay.cpp
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/millis.cpp

    # ?
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/FS.cpp

    ${arduino_compat_SOURCE_DIR}/cores/rp2040/wiring_shift.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/delay.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/wiring_pulse.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/wiring_analog.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/Tone.cpp

    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/Common.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/Stream.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/String.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/PluggableUSB.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/IPAddress.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/Print.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/stdlib_noniso.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/wiring_digital.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/wiring_private.cpp
    #${arduino_compat_SOURCE_DIR}/cores/rp2040/Bootsel.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/SerialUART.cpp
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/WMath.cpp

    ${arduino_compat_SOURCE_DIR}/libraries/SPI/SPI.cpp
    ${arduino_compat_SOURCE_DIR}/libraries/Wire/src/Wire.cpp

    # ${CMAKE_CURRENT_LIST_DIR}/arduino-pico-extra/HardwareSerialStdio.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/arduino-pico-extra/SerialExtern.cpp
)

target_include_directories(arduino-compat INTERFACE
    # ${CMAKE_CURRENT_LIST_DIR}/arduino-pico-extra/

    ${arduino_compat_SOURCE_DIR}/cores/rp2040/
    ${arduino_compat_SOURCE_DIR}/libraries/Wire/src/
    ${arduino_compat_SOURCE_DIR}/libraries/SPI/
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/deprecated
    ${arduino_compat_SOURCE_DIR}/cores/rp2040/api/deprecated-avr-comp
    ${arduino_compat_SOURCE_DIR}/variants/rpipico/
)

target_link_libraries(arduino-compat INTERFACE
    hardware_adc
    hardware_exception
    hardware_i2c
    hardware_pio
    hardware_pwm
    hardware_spi
    pico_multicore
    pico_stdlib
)
