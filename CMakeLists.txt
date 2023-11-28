cmake_minimum_required(VERSION 3.26)
project(japanese_dictionary)

set(CMAKE_CXX_STANDARD 14)

add_executable(japanese_dictionary
        src/main.cpp
        src/DictionaryEntry.h
        src/DictionaryEntry.cpp
        src/JishoDict.cpp
        src/JishoDict.h)
