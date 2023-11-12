cmake_minimum_required(VERSION 3.26)
project(japanese_dictionary2)

set(CMAKE_CXX_STANDARD 14)

add_executable(japanese_dictionary2
        src/main.cpp
        src/DictionaryEntry.h
        src/DictionaryEntry.cpp)
