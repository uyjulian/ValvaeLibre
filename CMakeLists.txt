cmake_minimum_required(VERSION 3.10)

project(SeniorCapstoneProject2022)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(program1_SRC
	src/program1.cpp
)

set(program1_NAME program1)
add_executable(${program1_NAME})

target_sources(${program1_NAME} PRIVATE ${program1_SRC})


set(program2_SRC
	src/program2.cpp
)

set(program2_NAME program2)
add_executable(${program2_NAME})

target_sources(${program2_NAME} PRIVATE ${program2_SRC})

