
include(FetchContent)
FetchContent_Declare(
    liquidcrystal_i2c
    GIT_REPOSITORY https://github.com/johnrickman/LiquidCrystal_I2C.git
    GIT_TAG        738765e388816fb2a687d40436cd2604febce1b1
    GIT_SUBMODULES_RECURSE OFF
    GIT_SUBMODULES ""
)

if(NOT liquidcrystal_i2c_POPULATED)
    FetchContent_Populate(liquidcrystal_i2c)
endif()

add_library(pac-liquidcrystal-i2c INTERFACE)

target_sources(pac-liquidcrystal-i2c INTERFACE
    "${liquidcrystal_i2c_SOURCE_DIR}/LiquidCrystal_I2C.cpp"
    "${liquidcrystal_i2c_SOURCE_DIR}/LiquidCrystal_I2C.h"
)

target_include_directories(pac-liquidcrystal-i2c INTERFACE
    "${liquidcrystal_i2c_SOURCE_DIR}"
)

if (NOT TARGET arduino-compat)
    add_subdirectory(../../arduino-pico build_arduino-compat)
endif()

target_link_libraries(pac-liquidcrystal-i2c INTERFACE
    arduino-compat
)
