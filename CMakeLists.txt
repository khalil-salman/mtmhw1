cmake_minimum_required(VERSION 3.27)
project(mtmhw1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mtmhw1
        BlockChain.cpp
        BlockChain.h
        Transaction.h
        Utilities.cpp
        Utilities.h)
