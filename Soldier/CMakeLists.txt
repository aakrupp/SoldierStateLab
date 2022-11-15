cmake_minimum_required(VERSION 3.23)
project(Soldier)

set(CMAKE_CXX_STANDARD 17)

add_executable(Soldier soldier.cpp Character.h Character.cpp)
